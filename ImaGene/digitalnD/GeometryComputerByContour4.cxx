///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : GeometryComputerByContour4.cxx
//
// Creation : 2003/02/27
//
// Version : 2003/02/27
//
// Author : Jacques-Olivier Lachaud
//
// email : lachaud@labri.fr
//
// Purpose : ??
//
// Distribution :
//
// Use :
//	??
//
// Todo :
//	O ??
//
// History :
//	2003/02/27 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
//#include "LinAlg/LinAlg2D/Vector2D.hpp"
//#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Proxy.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/mathutils/Polynomial.h"
#include "ImaGene/dgeometry2d/DLine.h"
#include "ImaGene/dgeometry2d/C4CGeometry.h"
#include "ImaGene/dgeometry2d/C4CSegmentPencil.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/GeometryComputerByContour4.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/GeometryComputerByContour4.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const GeometryComputerByContour4_RCS_ID = "@(#)class GeometryComputerByContour4 definition.";

///////////////////////////////////////////////////////////////////////////////
// class GeometryComputerByContour4
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
GeometryComputerByContour4::~GeometryComputerByContour4()
{
}

/**
 * Default constructor.
 */
GeometryComputerByContour4::GeometryComputerByContour4()
  : discrete_tangent_type( SYMMETRIC_TANGENT ),
    discrete_tangent_planes_type( BOUNDING_PLANES ),
    continuous_analog_type( TANGENT_PLANE_PROJECTION ),
    max_size( 0 )
{
}


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Implemented Geometry services ------------------

/**
 * Uses $n-1$ 2d tangent computations through [surf_geom_ctxt].
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
 * @see DigitalSurfaceGeometry::tangent2D
 */
void 
GeometryComputerByContour4::computeNormal
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b, Vector & vn )
{
  const KnSpace* space = surf_geom_ctxt.space();
  ASSERT_GeometryComputerByContour4( space->sisSurfel( b ) );
  
  uint j = space->sorthDir( b );
  KnSpace::dir_iterator q = space->sbegin_dirs( b );
  bool orth_pos = space->sdirect( b, j );
  float norm = 0.0;

  if ( discrete_tangent_planes_type == AVERAGE_CONTINUOUS_ANALOG )
    {
      Vector2D r0;
      Vector2D r1;

      Polynomial lambda = Polynomial::bellShape2( 1.0f );
      Polynomial lambda_p = lambda.derivative();
      
      while ( ! q.end() )
	{
	  uint i = *q;
	  C4CIterator* it = surf_geom_ctxt.surface()->newC4CIterator( b, i );
	  uint j = 0;
	  uint k;
	  uint m = 100;
	  if ( ! C4CGeometry::maximalSegments( *it, m_segments, j, k, m ) )
	    cerr << "[GeometryComputerByContour4::computeNormal]"
		 << " Not enough segments." << endl;
	  C4CSegmentPencil pencil( m_segments, j, k, m, lambda, lambda_p );
	  r0 = pencil.continuousAnalogDerivative( Vector2D( 0.5f, 0.0f ) );
	  // r0 /= sqrt( r0.ro( 0 ) * r0.ro( 0 ) + r0.ro( 1 ) * r0.ro( 1 ) );
	  // r1 = pencil.continuousAnalog( Vector2D( 0.75f, 0.0f ) );
	  // cout << "(" << r.x() << " " << r.y() << ")";
	  delete it;
	  bool track_pos = space->sdirect( b, i );
	  if ( ! track_pos ) r0.x() = -r0.x();
	  vn.rw( i ) = r0.y() / r0.x();
	  norm += vn.ro( i ) * vn.ro( i );
	  ++q;
	}
    }
  else
    {
      while ( ! q.end() )
	{
	  uint i = *q;
	  Vector2i tgt2d( surf_geom_ctxt.tangent2D( b, i ) );
	  bool track_pos = space->sdirect( b, i );
	  int si_bi = track_pos ? tgt2d.x() : -tgt2d.x();
	  //int sj_ai = orth_pos ? -tgt.y() : tgt.y();
	  int sj_ai = -tgt2d.y();
	  vn.rw( i ) = - sj_ai / (float) si_bi ;
	  norm += vn.ro( i ) * vn.ro( i );
	  //cout << " (" << i << "," << vn.ro( i ) << ")";
	  ++q;
	}
      //cout << endl;
    }
  vn.rw( j ) = orth_pos ? 1.0 : -1.0;
  vn /= sqrt( 1.0 + norm );
}


/**
 * A strong version of 'computeNormal' since it gives a tangent plane 
 * estimation. More precisely, it returns the upper and lower deviation to
 * the centroid of the surfel [b] along its orthogonal coordinate so that
 * all elements of the tangents are included in the plane. 
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
 * @param out_x_plane the deviation to the surfel centroid to get the lower leaning plane (outside the shape since normal points inside). 
 * @param in_x_plane the deviation to the surfel centroid to get the upper leaning plane (inside the shape since normal points inside). 
 * @see computeNormal
 */
void
GeometryComputerByContour4::computeTangentPlane
( DigitalSurfaceGeometry & surf_geom_ctxt, 
  Kn_sid b, Vector & vn,
  float & out_x_plane,
  float & in_x_plane )
{
  const KnSpace* space = surf_geom_ctxt.space();
  ASSERT_GeometryComputerByContour4( space->sisSurfel( b ) );

//   float z_i_left;
//   float z_i_right;
  
  // direction orthogonal to surfel.
  uint j = space->sorthDir( b );

  KnSpace::dir_iterator q = space->sbegin_dirs( b );
  bool orth_pos = space->sdirect( b, j );
  float norm = 0.0;
  Vector xc( vn.size() );

  switch ( discrete_tangent_planes_type ) {

  case BOUNDING_PLANES:

    out_x_plane = -100.0;
    in_x_plane = 100.0;
    while ( ! q.end() )
      {
	// Direction of contour.
	uint i = *q;
	
	// Discrete line approximating the 2D tangent.
	DLine line2d( surf_geom_ctxt.tangentLine2D( b, i ) );
	Vector2i tgt2d( line2d.tangent() );
	
	// Computing lower and upper bound in the local coordinate system.
	//       z_i_left = ( line2d.mu - 0.5 * line2d.a ) 
	// 	/ (float) line2d.b;
	//       z_i_right = ( line2d.mup - 0.5 * line2d.a )
	// 	/ (float) line2d.b;
	float mi = line2d.lowerY( 0.5f );
	float ma = line2d.upperY( 0.5f );
	
	//       z_i_right = ( line2d.mu + fabs( (float) line2d.a ) + line2d.b - 1
	// 		    - 0.5 * line2d.a )
	// 	/ (float) line2d.b;
	
	//       float mi,ma;
	//       mi = - z_i_right;
	//       ma = - z_i_left;
	
	if ( ma < mi )
	  cerr << "[(" << i << "," << j << "," << space->dim() << ")"
	       << " mi= " << mi << " ma=" << ma
	       << "]  ERR1" << endl;
	//      cout << "(out=" << mi << " in=" << ma << ")";
	if ( mi < in_x_plane ) in_x_plane = mi;
	if ( ma > out_x_plane ) out_x_plane = ma;
	
	// Computation of normal vector
	bool track_pos = space->sdirect( b, i );
	int si_bi = track_pos ? tgt2d.x() : -tgt2d.x();
	int sj_ai = -tgt2d.y();
	vn.rw( i ) = - sj_ai / (float) si_bi ;
	norm += vn.ro( i ) * vn.ro( i );
	++q;
	
      }
    //   cout << " => (out=" << out_x_plane << " in=" << in_x_plane << ")";
    //   cout << endl;
    
    // Inverts coordinates when inside normal vector is oriented oppositely to
    // trivial basis. 

    // normalize normal vector.
    vn.rw( j ) = orth_pos ? 1.0 : -1.0;
    vn /= sqrt( 1.0 + norm );

    // Inverts coordinates when inside normal vector is oriented oppositely to
    // trivial basis. 
    if ( orth_pos )
      {
	out_x_plane = -out_x_plane;
	in_x_plane = -in_x_plane;
      }

  
    break;


  case AVERAGE_CONTINUOUS_ANALOG:
  case ESTIMATED_PLANES:
    
    while ( ! q.end() )
      {
	// Direction of contour.
	uint i = *q;
	
	// Discrete line approximating the 2D tangent.
	C4CIterator* pfwd = surf_geom_ctxt.surface()->newC4CIterator( b, i );
	C4CIterator* pbwd = surf_geom_ctxt.surface()->newC4CIterator( b, i );
	ASSERT_GeometryComputerByContour4( ( pfwd != 0 ) && ( pbwd != 0 ) );

	// JOL 2004/08/03. Calls the appropriate tangent estimator instead
	// of 'C4CGeometry::tangentLine' 
	C4CSegment s;
	// Calls the appropriate discrete tangent estimator.
	switch ( discrete_tangent_type ) {
	case SYMMETRIC_TANGENT: 
	  s = C4CGeometry::symmetricTangent( *pfwd, *pbwd, this->max_size );
	  break;
	case EXTENDED_TANGENT:
	  s = C4CGeometry::extendedTangent( *pfwd, *pbwd, this->max_size );
	  break;
	case COMMON_TANGENT:
	  s = C4CGeometry::commonTangent( *pfwd, *pbwd, this->max_size );
	  break;
	}

	delete pbwd;
	delete pfwd;
	

	DLine line2d( s.getLine() );
	Vector2i tgt2d( line2d.tangent() );
	
	float x = s.meanX( 0.0f ) - 0.5f;
	
// 	if ( ( xc.ro( i ) < -0.5f ) || ( xc.ro( i ) > 0.5f ) )
// 	  cerr << "[(" << i << "," << j << "," << space->dim() << ")"
// 	       << " xc[i]= " << xc.ro( i ) << endl << " s=" << s 
// 	       << "]  ERR OUT OF BOUNDS" << endl;

	// Computation of normal vector
	bool track_pos = space->sdirect( b, i );
	xc.rw( i ) = track_pos ? x : -x;
	int si_bi = track_pos ? tgt2d.x() : -tgt2d.x();
	int sj_ai = -tgt2d.y();
	vn.rw( i ) = - sj_ai / (float) si_bi ;
	norm += vn.ro( i ) * vn.ro( i );
	++q;
	
      }

    // normalize normal vector.
    vn.rw( j ) = orth_pos ? 1.0 : -1.0;
    vn /= sqrt( 1.0 + norm );
    
    // Computes plane position and intersection with segment
    // orthogonal to surfel centroid.
    in_x_plane = 0.0f;
    q = space->sbegin_dirs( b );
    while ( ! q.end() )
      {
	// Direction of contour.
	uint i = *q;
	in_x_plane += vn.ro( i ) * xc.ro( i );
	++q;
      }
    in_x_plane /= vn.ro( j );
    out_x_plane = in_x_plane;

//     cout << "[j=" << j << " z=" << in_x_plane << " x0=" << xc.ro( 0 )
// 	 << " x1=" << xc.ro( 1 ) << " x2=" << xc.ro( 2 )
// 	 << " v0=" << vn.ro( 0 )
// 	 << " v1=" << vn.ro( 1 ) << " v2=" << vn.ro( 2 ) << "]";
    

    break;

  }
  
  
  
}





/**
 * Uses [surf_geom_ctxt] to get 'C4CIterator's, then call 
 * C4CGeometry::tangentLine
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param i any tracking direction from [b].
 * @return the estimated 2d tangent vector at surfel [b] in the tracking direction [i].
 * @see DigitalSurface::newC4CIterator,C4CIterator,C4CGeometry::tangentLine
 */
Vector2i 
GeometryComputerByContour4::computeTangent2D
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b, uint i )
{

  // JOL 2004/08/03. Calls 'computeTangentLine2D' instead of doing everything.
  if ( discrete_tangent_type != LAMBDA_MST ) {
    DLine median = computeTangentLine2D( surf_geom_ctxt, b, i );
    return median.tangent();
  }
  Proxy<C4CIteratorOnSurface> cur_it
    ( (C4CIteratorOnSurface*) 
      surf_geom_ctxt.surface()->newC4CIterator( b, i ) );
  
  // Builds the pencil of maximal segments.
  uint j = 0;
  uint k;
  uint m = 100; // ugly.
  const float omega = 10000.0f;
  Vector2i v(1,0);
  if ( C4CGeometry::maximalSegments( *cur_it, m_segments, j, k, m ) )
    {
      // All geometric computations are made in the local frame of
      // the current boundary element.
      C4CSegmentPencil pencil( m_segments, j, k, m, m_l, m_lp );
      float theta = pencil.angleToX( Vector2D( 0.5, 0.0 ) );
      v.x() = (int) round( cos( theta ) * omega );
      v.y() = (int) round( sin( theta ) * omega );
    }
  return v;
}


/**
 * Stronger version of 'computeTangent2D'. It provides also the offset 'mu'.
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param i any tracking direction from [b].
 * @return the estimated 2d tangent line at surfel [b] in the tracking direction [i].
 */
DLine
GeometryComputerByContour4::computeTangentLine2D
( DigitalSurfaceGeometry & surf_geom_ctxt, 
  Kn_sid b, uint i )
{
  C4CIterator* pfwd =
    surf_geom_ctxt.surface()->newC4CIterator( b, i );
  C4CIterator* pbwd =
    surf_geom_ctxt.surface()->newC4CIterator( b, i );

  ASSERT_GeometryComputerByContour4( ( pfwd != 0 ) && ( pbwd != 0 ) );

  // JOL 2004/08/03. Calls the appropriate tangent estimator instead
  // of 'C4CGeometry::tangentLine' 
  // DLine median = C4CGeometry::tangentLine( *pfwd, *pbwd, this->max_size );
  C4CSegment median;

  // Calls the appropriate discrete tangent estimator.
  switch ( discrete_tangent_type ) {
  case SYMMETRIC_TANGENT: 
    median = C4CGeometry::symmetricTangent( *pfwd, *pbwd, this->max_size );
    break;
  case EXTENDED_TANGENT:
    median = C4CGeometry::extendedTangent( *pfwd, *pbwd, this->max_size );
    break;
  case COMMON_TANGENT:
    median = C4CGeometry::commonTangent( *pfwd, *pbwd, this->max_size );
    break;
  }

  delete pbwd;
  delete pfwd;

  return median.getLine();
}



/**
 * Uses [surf_geom_ctxt] to get normals.
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @return the estimated area of the surfel [b].
 * @see DigitalSurfaceGeometry::normal
 */
float 
GeometryComputerByContour4::computeArea
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b )
{
  uint d = surf_geom_ctxt.space()->dim();
  Vector n( d );
  surf_geom_ctxt.normal( b, n );
  float abs_sum = fabs( n.ro( 0 ) );
  for ( uint i = 1; i < d; ++i )
    abs_sum += fabs( n.ro( i ) );
  return 1 / abs_sum;
}

/**
 * Default: returns 1.0
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param i any tracking direction from [b].
 * @return the estimated length of the elementary displacement in the neighborhood of [b].
 */
float 
GeometryComputerByContour4::computeLength
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b, uint i )
{
  Vector2i tgt2d( surf_geom_ctxt.tangent2D( b, i ) );
  return 1.0 / ( fabs( (float) tgt2d.x() ) + fabs( (float) tgt2d.y() ) );
}


/**
 * Uses [surf_geom_ctxt] to get 2d tangent.
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param i any tracking direction from [b].
 * @return the estimated distance to go from [b] to the next surfel in direction [i] (ie direct follower).
 * @see DigitalSurfaceGeometry::tangent2D
 */
float 
GeometryComputerByContour4::computeLengthNext
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b, uint i )
{
  return 1.0;
}

/**
 * Uses [surf_geom_ctxt] to get 2d tangent.
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param i any tracking direction from [b].
 * @return the estimated distance to go from [b] to the previous surfel in direction [i] (ie indirect follower).
 * @see DigitalSurfaceGeometry::tangent2D
 */
float 
GeometryComputerByContour4::computeLengthPrevious
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b, uint i )
{
  return 1.0;
}

/**
 * Uses [surf_geom_ctxt] to get normals and lengths.
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @return the estimated mean curvature at surfel [b].
 * @see DigitalSurfaceGeometry::normal,DigitalSurfaceGeometry::lengthNext,DigitalSurfaceGeometry::lengthPrevious
 */
float 
GeometryComputerByContour4::computeMeanCurvature
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b )
{
  return 0.0;
}


/**
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param i any tracking direction from [b].
 * @return the estimated 2d point in the Euclidean space associated to the surfel [b] using estimation along contour of direction [i].
 */
Vector2D
GeometryComputerByContour4::computeContinuousAnalog
( DigitalSurfaceGeometry & surf_geom_ctxt, 
  Kn_sid b, uint i )
{
  Vector2D r;
  if ( continuous_analog_type == TANGENT_PLANE_PROJECTION )
    {
      DLine median = computeTangentLine2D( surf_geom_ctxt, b, i );
      float mi = median.lowerY( 0.5f );
      float ma = median.upperY( 0.5f );
      r = Vector2D( 0.5f, ( mi + ma ) / 2 );
    }
  else if ( ( continuous_analog_type == PENCIL_INTERPOLATION )
	    || ( continuous_analog_type == PENCIL_INTERPOLATION2 ) )
    {
      C4CIterator* it = surf_geom_ctxt.surface()->newC4CIterator( b, i );
      uint j = 0;
      uint k;
      uint m = 100;
      if ( ! C4CGeometry::maximalSegments( *it, m_segments, j, k, m ) )
	cerr << "[GeometryComputerByContour4::computeContinuousAnalog]"
	     << " Not enough segments." << endl;
      Polynomial lambda = Polynomial::bellShape2( 1.0f );
      Polynomial lambda_p = lambda.derivative();
      C4CSegmentPencil pencil( m_segments, j, k, m, lambda, lambda_p );
      r = pencil.continuousAnalog( Vector2D( 0.5f, 0.0f ) );
      // cout << "(" << r.x() << " " << r.y() << ")";
      delete it;
    }
  
  return r;
}


/**
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param x (returns) the estimated point in the Euclidean space associated to the surfel [b].
 */
void
GeometryComputerByContour4::computeContinuousAnalog
( DigitalSurfaceGeometry & surf_geom_ctxt, 
  Kn_sid b, Vector & x )
{
  const KnSpace* space = surf_geom_ctxt.space();
  ASSERT_GeometryComputerByContour4( space->sisSurfel( b ) );
  space->scentroid( b, x );

  if ( continuous_analog_type == TANGENT_PLANE_PROJECTION )
    {
      Vector n( x.size() );
      float out_x, in_x;

      computeTangentPlane( surf_geom_ctxt, b, n, out_x, in_x );
      uint j = surf_geom_ctxt.space()->sorthDir( b );
      float m = ( out_x + in_x ) / 2.0;
      // Choose one of (1) and (2)
      
      // (1) Project onto mid-plane following direction normal to plane
      float mj_dot_u = m * n.ro( j );
      for ( uint i = 0; i < x.size(); ++i ) 
	x.rw( i ) += mj_dot_u * n.ro( i );
      // (2) Project onto mid-plane following direction of trivial normal
      //   for ( uint i = 0; i < 3; ++i ) 
      //     (*xyz)[ i ] = v.ro( i );
      //   (*xyz)[ j ] -= m;
    }
   else if ( continuous_analog_type == PENCIL_INTERPOLATION )
     {
       Vector vn( x.size() );
       Vector xc( x.size() );


       // direction orthogonal to surfel.
       uint j = space->sorthDir( b );
       
       KnSpace::dir_iterator q = space->sbegin_dirs( b );
       // bool orth_pos = space->sdirect( b, j );
       while ( ! q.end() )
	 {
	   // Direction of contour.
	   uint i = *q;
	   
	   // Continuous analog estimation on each direction.
	   Vector2D p = surf_geom_ctxt.continuousAnalog( b, i );
	   
	   // Tangent along each direction.
	   Vector2i tgt2d( surf_geom_ctxt.tangent2D( b, i ) );
	
	   float x_plane0 = ( tgt2d.y() != 0 )
	     ? p.x() - ( p.y() * tgt2d.x() ) / tgt2d.y()
	     : 0.5f;
	   bool track_pos = space->sdirect( b, i );
	   // Shift of 0.5 to get surfel centroid at (0...0).
	   xc.rw( i ) = track_pos ? x_plane0 - 0.5f  : 0.5f - x_plane0;
	   
	   ++q;
	 }
       xc.rw( j ) = 0.0;
       surf_geom_ctxt.normal( b, vn );

       // (1) Project onto mid-plane following direction normal to plane
       float mj_dot_u = 0.0f;
       for ( uint i = 0; i < x.size(); ++i ) 
	 mj_dot_u += xc.ro( i ) * vn.ro( i );
       for ( uint i = 0; i < x.size(); ++i ) 
	 x.rw( i ) += mj_dot_u * vn.ro( i );
     }

   else if ( continuous_analog_type == PENCIL_INTERPOLATION2 )
     {
       Vector xc( x.size() );

       // direction orthogonal to surfel.
       uint j = space->sorthDir( b );
       bool orth_pos = space->sdirect( b, j );
       KnSpace::dir_iterator q = space->sbegin_dirs( b );
       xc.rw( j ) = 0.0f;
       while ( ! q.end() )
	 {
	   // Direction of contour.
	   uint i = *q;
	   
	   // Continuous analog estimation on each direction.
	   Vector2D p = surf_geom_ctxt.continuousAnalog( b, i );
	   bool track_pos = space->sdirect( b, i );
	   // Shift of 0.5 to get surfel centroid at (0...0).
	   xc.rw( i ) = track_pos ? p.x() - 0.5f : 0.5f - p.x();
	   xc.rw( j ) += p.y();
	   ++q;
	 }
       xc /= (float) ( xc.size() - 1 );
       if ( orth_pos ) xc.rw( j ) = -xc.rw( j );
       for ( uint i = 0; i < x.size(); ++i ) 
	 x.rw( i ) += xc.ro( i );
       
     }
  

}





///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
GeometryComputerByContour4::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[GeometryComputerByContour4]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
GeometryComputerByContour4::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
