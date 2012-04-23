///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CTangentialCoverGeometry.cxx
//
// Creation : 2004/11/29
//
// Version : 2004/11/29
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
//	2004/11/29 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/base/Proxy.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.h"
#include "ImaGene/helper/C4CTangentialCoverGeometry.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/C4CTangentialCoverGeometry.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const C4CTangentialCoverGeometry_RCS_ID = "@(#)class C4CTangentialCoverGeometry definition.";



/** 
 * @return the (most probable) angle between the maximal segment and
 * its local x-axis.
 */
double
ImaGene::C4CTangentialCoverGeometry::MSGeometryComputerByLP::angle
( const C4CSegment & ms )
{
  Vector2i tgt( ms.getTangent() );
  return atan2( (double) tgt.y(), (double) tgt.x() );
}

/** 
 * @return the minimum angle between the maximal segment and
 * its local x-axis.
 */
double
ImaGene::C4CTangentialCoverGeometry::MSGeometryComputerByLP::minAngle
( const C4CSegment & ms )
{
  Vector2i tgt( ms.minSlopeLeaningPoints() );
  return atan2( (double) tgt.y(), (double) tgt.x() );
}

/** 
 * @return the maximum angle between the maximal segment and
 * its local x-axis.
 */
double
ImaGene::C4CTangentialCoverGeometry::MSGeometryComputerByLP::maxAngle
( const C4CSegment & ms )
{
  Vector2i tgt( ms.maxSlopeLeaningPoints() );
  return atan2( (double) tgt.y(), (double) tgt.x() );
}


/** 
 * @return the (most probable) angle between the maximal segment and
 * its local x-axis.
 */
double
ImaGene::C4CTangentialCoverGeometry::MSGeometryComputerByDual::angle
( const C4CSegment & ms )
{
  Vector2i tgt( ms.getTangent() );
  return atan2( (double) tgt.y(), (double) tgt.x() );
}

/** 
 * @return the minimum angle between the maximal segment and
 * its local x-axis.
 */
double
ImaGene::C4CTangentialCoverGeometry::MSGeometryComputerByDual::minAngle
( const C4CSegment & ms )
{
  Vector2i tgt( ms.minSlopeDual() );
  return atan2( (double) tgt.y(), (double) tgt.x() );
}

/** 
 * @return the maximum angle between the maximal segment and
 * its local x-axis.
 */
double
ImaGene::C4CTangentialCoverGeometry::MSGeometryComputerByDual::maxAngle
( const C4CSegment & ms )
{
  Vector2i tgt( ms.maxSlopeDual() );
  return atan2( (double) tgt.y(), (double) tgt.x() );
}


///////////////////////////////////////////////////////////////////////////////
// class C4CTangentialCoverGeometry
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// ------------------------- static services ------------------------------

/**
 * Given a digital curve specified by its 'C4CTangentialCover'
 * [tcover] and its 'C4CTangentialCoverGeometry' [tcover_geometry],
 * and given an interpolation function [lambda], computes the
 * lambda-MS tangent estimation of a surfel specified by its
 * 'SurfelMaximalSegments' [sms].
 *
 * @param tcover the tangential cover of the digital curve.
 * @param tcover_geometry the geometry of the tangential cover [tcover].
 * @param lambda an interpolation function.
 * @param sms the local maximal segment structure around the surfel of interest.
 * @return the angle of the curve wrt x-axis.
 */
double
ImaGene::C4CTangentialCoverGeometry::angleByLambdaMS
( const C4CTangentialCover & tcover,
  const C4CTangentialCoverGeometry & tcover_geometry,
  const R2RFunction & lambda,
  const C4CTangentialCover::SurfelMaximalSegments & sms )
{
  Mathutils::ModuloComputer mcms( tcover.nbMaximalSegments() );
  Mathutils::ModuloComputer mcsu( tcover.nbSurfels() );
  Mathutils::AngleComputer ac;
  
  double theta = 0.0f;
  double acc = 0.0f;
  double theta_ref = -1.0f;
  //cout << "  Pencil m=(" << m.x() << "," << m.y() << ")" << endl;
  
  for ( uint i = sms.begin_ms; 
	i != sms.end_ms; 
	mcms.increment( i ) )
    {
      const C4CTangentialCover::MaximalSegment & ms 
	= tcover.getMaximalSegment( i );
      
      double theta_i = tcover_geometry.geometry( i ).angle_to_x;
      if ( theta_ref < 0.0f ) theta_ref = theta_i;

      double e_i = 
	( 0.5f + (double) ( mcsu.posDiff( sms.idx_surfel, ms.back_surfel_idx ) ) )
	/ ( mcsu.posDiff( ms.front_surfel_idx, ms.back_surfel_idx ) + 1 );
      double lambda_i = lambda( e_i );
      theta += ac.deviation( theta_i, theta_ref ) * lambda_i;
      acc += lambda_i;
    }
  theta /= acc;
  // cout << "  => xg=(" << xg.x() << "," << xg.y() << ")" << endl;
  return ac.cast( theta + theta_ref );
}


/**
 * Given a digital curve specified by its 'C4CTangentialCover'
 * [tcover] and its 'C4CTangentialCoverGeometry' [tcover_geometry],
 * and given an interpolation function [lambda], computes the length
 * of a subset of the digital curve. This subset is located on a
 * common part of the tangential cover, specified by a
 * 'SurfelMaximalSegments' [sms] and two surfel indices [s1] and
 * [s2]. The length is estimated by integrated the averaged length
 * of the lambda-MS tangent direction.
 *
 * @param tcover the tangential cover of the digital curve.
 * @param tcover_geometry the geometry of the tangential cover [tcover].
 * @param lambda an interpolation function.
 * @param sms the local maximal segment structure around the surfels of interest.
 * @param s1 the first surfel index.
 * @param s2 the surfel index after the last.
 * @return the length of the digital curve from [s1] to [s2] (excluded).
 */
double
ImaGene::C4CTangentialCoverGeometry::lengthByLambdaMS
( const C4CTangentialCover & tcover,
  const C4CTangentialCoverGeometry & tcover_geometry,
  const R2RFunction & lambda,
  const C4CTangentialCover::SurfelMaximalSegments & sms,
  uint s1, uint s2 )
{
  double l = 0.0f;
  C4CTangentialCover::SurfelMaximalSegments sms2 = sms;

  // Mathutils::ModuloComputer mcms( tcover.nbMaximalSegments() );
  Mathutils::ModuloComputer mcsu( tcover.nbSurfels() );

  while ( s1 != s2 )
    {
      sms2.idx_surfel = s1;
      l += Mathutils::averagedLength( angleByLambdaMS( tcover,
						       tcover_geometry,
						       lambda,
						       sms2 ) );
      mcsu.increment( s1 );
    }
  return l;
}


/**
 * Given a digital curve specified by its 'C4CTangentialCoverGeometry'
 * [tcover_geometry], computes the length of a subset of the digital
 * curve. This subset is located on a common part of the tangential
 * cover, specified by a 'SurfelMaximalSegments' [sms] and two surfel
 * indices [s1] and [s2]. The length is estimated by averaging the
 * length of the portion of the maximal segments visiting these
 * surfels.
 *
 * @param tcover the tangential cover of the digital curve.
 * @param tcover_geometry the geometry of the tangential cover [tcover].
 * @param sms the local maximal segment structure around the surfels of interest.
 * @param s1 the first surfel index.
 * @param s2 the surfel index after the last.
 * @return the length of the digital curve from [s1] to [s2] (excluded).
 */
double
ImaGene::C4CTangentialCoverGeometry::lengthByAverage
( const C4CTangentialCover & tcover,
  const C4CTangentialCoverGeometry & tcover_geometry,
  const C4CTangentialCover::SurfelMaximalSegments & sms,
  uint s1, uint s2 )
{
  Mathutils::ModuloComputer mcms( tcover.nbMaximalSegments() );
  Mathutils::ModuloComputer mcsu( tcover.nbSurfels() );

  double l = 0.0;
  
  uint i = sms.begin_ms;
  uint n = 0;
  while ( i != sms.end_ms )
    {
      double theta_i = (double) tcover_geometry.geometry( i ).angle_to_x;
      l += 1.0 / ( fabs( cos( theta_i ) ) + fabs( sin( theta_i ) ) );
      mcms.increment( i );
      ++n;
    }
  uint s = 0;
  while ( s1 != s2 )
    {
      ++s;
      mcsu.increment( s1 );
    }
  return (double) ( ( (double) s ) * l / (double) n);
}



/**
 * Given a digital curve specified by its 'C4CTangentialCover'
 * [tcover] and its 'C4CTangentialCoverGeometry' [tcover_geometry],
 * and given an interpolation function [lambda], computes the
 * elementary lengthes of all surfels of the contour.
 *
 * @param tcover the tangential cover of the digital curve.
 * @param tcover_geometry the geometry of the tangential cover [tcover].
 * @param lambda an interpolation function.
 *
 * @return the elementary lengthes.
 */
ImaGene::C4CELength*
ImaGene::C4CTangentialCoverGeometry::computeELengthByLambdaMS
( const C4CTangentialCover & tcover,
  const C4CTangentialCoverGeometry & tcover_geometry,
  const R2RFunction & lambda )
{
  C4CELength* el = new C4CELength( tcover.nbSurfels() );
  Mathutils::ModuloComputer mcms( tcover.nbMaximalSegments() );
  Mathutils::ModuloComputer mcsu( tcover.nbSurfels() );

  C4CTangentialCover::SurfelMaximalSegments sms( tcover.beginSMS( 0 ) );
  do {
    double theta = angleByLambdaMS( tcover, 
				    tcover_geometry, 
				    lambda,
				    sms );
    double surf_angle 
      = tcover_geometry.sgeometry( sms.idx_surfel ).track_angle_to_x;
    // elementary length by projection.
    el->elength( sms.idx_surfel ) = cos( theta - surf_angle );
//     cerr << "ELLMS i=" << sms.idx_surfel
// 	 << " theta=" << theta
// 	 << " trivial=" << surf_angle
// 	 << " el=" << el->elength( sms.idx_surfel )
// 	 << endl;
  } while ( tcover.nextSMS( sms ) // in case of open contour
	    && ( sms.idx_surfel != 0 ) ); // in case of loop

  return el;
}




/**
 * Display the geometry of a tangential cover as a function of the
 * maximal segement.
 *
 * @param out any output stream.
 * @param tcover the tangential cover of the digital curve.
 * @param tcover_geometry the geometry of the tangential cover [tcover].
 */
void
ImaGene::C4CTangentialCoverGeometry::displayGeometry
( ostream & out, 
  const C4CTangentialCover & tcover,
  const C4CTangentialCoverGeometry & tcover_geometry )
{
  out << "# Maximal segment geometry" << endl
      << "# index back_surfel front_surfel a b angle min_angle max_angle" 
      << endl;
  for ( uint i = 0; i < tcover.nbMaximalSegments(); ++ i)
    {
      const C4CTangentialCover::MaximalSegment & ms 
	= tcover.getMaximalSegment( i );

      const MaximalSegmentGeometry & msgeom
	= tcover_geometry.geometry( i );

      out << i << " " << ms.back_surfel_idx << " " << ms.front_surfel_idx
	  << " " << ms.dss.a() << " " << ms.dss.b()
	  << " " << msgeom.angle_to_x
	  << " " << msgeom.min_angle
	  << " " << msgeom.max_angle
	  << endl;
    }
}




///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::C4CTangentialCoverGeometry::~C4CTangentialCoverGeometry()
{
  reset();
}

/**
 * Constructor. Empty object.
 */
ImaGene::C4CTangentialCoverGeometry::C4CTangentialCoverGeometry()
  :  m_ms_geometry( 0 ), m_surfel_geometry( 0 )
{
}


/**
 * Resets the object. As if it was just created.
 */
void
ImaGene::C4CTangentialCoverGeometry::reset()
{
  if ( m_ms_geometry != 0 ) delete[] m_ms_geometry;
  m_ms_geometry = 0;
  if ( m_surfel_geometry != 0 ) delete[] m_surfel_geometry;
  m_surfel_geometry = 0;
}

  
/**
 * (Re-) Initializes the object so that it can compute the geometry of the
 * contour given in parameter. The contour is represented by its
 * tangential cover [tcover] and needs also an iterator [it] to make
 * the correct change of frames.
 *
 * @param ks the digital space.
 * @param x_axis the first axis of the plane where the geometry is computed.
 * @param y_axis the second axis of the plane where the geometry is computed.
 * @param tcover the tangential cover of a digital curve.
 * @param it an iterator on the digital curve, pointing on the surfel of index 0 in the tangential cover.
 */
void
ImaGene::C4CTangentialCoverGeometry::init( KnSpace* ks, 
					   uint x_axis, uint y_axis,
					   const C4CTangentialCover & tcover,
					   const C4CIteratorOnSurface & it )
{
  reset();
  Mathutils::ModuloComputer mcsu( tcover.nbSurfels() );
  Mathutils::AngleComputer ac;

  m_nb_maximal_segments = tcover.nbMaximalSegments();
  m_ms_geometry = new MaximalSegmentGeometry[ m_nb_maximal_segments ];
  Frame2D frame;
  frame.init( ks, x_axis, y_axis );

  C4CIteratorOnSurface* it2 
    = dynamic_cast<C4CIteratorOnSurface*>( it.clone() );

  uint surfel_idx = 0;
  for ( uint ms = 0; ms < tcover.nbMaximalSegments(); ++ms )
    {
      // For each maximal segment, find reference frame as the front surfel.
      const C4CTangentialCover::MaximalSegment & msegment
	= tcover.getMaximalSegment( ms );
      while ( surfel_idx != msegment.front_surfel_idx )
	{
	  mcsu.increment( surfel_idx );
	  it2->next();
	}
      // Find tangent.
      Kn_sid surfel = it2->current();
      frame.setSurfelFrame( surfel, it2->trackDir() );
      Vector2i tgt = msegment.dss.getTangent();
      //int dx = msegment.dss.cp_n().x() - msegment.dss.c_n().x();
      Vector2D rtgt( (double) tgt.x(), (double) tgt.y() );
      double tan_angle = rtgt.ro( 1 ) / rtgt.ro( 0 );

      Vector2i min_slope = msegment.dss.minSlopeLeaningPoints();
      Vector2i max_slope = msegment.dss.maxSlopeLeaningPoints();
      double angle = atan( tan_angle );
      double angle_max = atan2( (double) max_slope.y(), 
			       (double) max_slope.x() );
      double angle_min = atan2( (double) min_slope.y(), 
			       (double) min_slope.x() );
      // Cast angles into reference frame.
      m_ms_geometry[ ms ].angle_to_x = frame.angleToX( angle );
      double a = ac.cast( frame.angleToX( angle_min ) );
      double b = ac.cast( frame.angleToX( angle_max ) );
      if ( ac.less( a, b ) )
	{
	  m_ms_geometry[ ms ].max_angle = b;
	  m_ms_geometry[ ms ].min_angle = a;
	}
      else 
	{
	  m_ms_geometry[ ms ].max_angle = a;
	  m_ms_geometry[ ms ].min_angle = b;
	}
    }

  delete it2;
}



/**
 * (Re-) Initializes the object so that it can compute the
 * geometry of the contour given in parameter. The contour is
 * represented by its tangential cover [tcover] and needs also
 * an iterator [it] to make the correct change of frames.
 *
 * @param tcover the tangential cover of a digital curve.
 *
 * @param it an iterator on the digital curve, pointing on the
 * surfel of index 0 in the tangential cover.
 *
 * @param msgeom an action object to compute maximal segment geometry.
 * 
 * @param frame the reference frame centered on the first surfel
 * ('*it'), may be modified.
 */
void
ImaGene::C4CTangentialCoverGeometry::init
( const C4CTangentialCover & tcover,
  const C4CIteratorOnSurface & it,
  MSGeometryComputer & msgeom,
  Frame2D & frame )
{
  reset();
  Mathutils::ModuloComputer mcsu( tcover.nbSurfels() );
  Mathutils::AngleComputer ac;

  // Prepare array to store geometric quantities.
  m_nb_maximal_segments = tcover.nbMaximalSegments();
  m_ms_geometry = new MaximalSegmentGeometry[ m_nb_maximal_segments ];
  m_nb_surfels = tcover.nbSurfels();
  m_surfel_geometry = new SurfelGeometry[ m_nb_surfels ];

  // Iterator to visit contour maximal segment per maximal segment.
  Proxy<C4CIteratorOnSurface> it2 
    ( dynamic_cast<C4CIteratorOnSurface*>( it.clone() ) );
  Proxy<C4CIteratorOnSurface> it3 
    ( dynamic_cast<C4CIteratorOnSurface*>( it.clone() ) );

  uint surfel_idx = 0;
  for ( uint ms = 0; ms < tcover.nbMaximalSegments(); ++ms )
    {
      //std::cerr << ((C4CIteratorOnFreemanChainSurface&)(*it2));
      // For each maximal segment, find reference frame as the front surfel.
      const C4CTangentialCover::MaximalSegment & msegment
	= tcover.getMaximalSegment( ms );
      while ( surfel_idx != msegment.front_surfel_idx )
	{
	  // go to next.
	  mcsu.increment( surfel_idx );
	  it2->next();
	}

      // Set reference frame.
      Kn_sid surfel = it2->current();
      // frame.space()->displayKn_sid( surfel, std::cerr );
      // std::cerr << endl;
      frame.setSurfelFrame( surfel, it2->trackDir() );

      // Compute and store geometric quantities.
      m_ms_geometry[ ms ].angle_to_x
	= frame.angleToX( msgeom.angle( msegment.dss ) );
      double a = frame.angleToX( msgeom.minAngle( msegment.dss ) );
      double b = frame.angleToX( msgeom.maxAngle( msegment.dss ) );
      m_ms_geometry[ ms ].max_angle = ac.max( a, b );
      m_ms_geometry[ ms ].min_angle = ac.min( a, b );
    }

  // Iterator to revisit contour surfel per surfel.
  for ( surfel_idx = 0; surfel_idx < tcover.nbSurfels(); ++surfel_idx )
    {
      // Set reference frame.
      Kn_sid surfel = it3->current();
      frame.setSurfelFrame( surfel, it3->trackDir() );
      m_surfel_geometry[ surfel_idx ].track 
	= frame.transform( Vector2i( 1, 0 ) );
      m_surfel_geometry[ surfel_idx ].track_angle_to_x 
	= frame.angleToX( 0.0 );
      m_surfel_geometry[ surfel_idx ].frame = frame;
      it3->next();
    }
}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::C4CTangentialCoverGeometry::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CTangentialCoverGeometry]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::C4CTangentialCoverGeometry::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
