///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CSegmentPencil.cxx
//
// Creation : 2004/08/11
//
// Version : 2004/08/11
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
//	2004/08/11 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/C4CSegmentPencil.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/C4CSegmentPencil.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const C4CSegmentPencil_RCS_ID = "@(#)class C4CSegmentPencil definition.";

///////////////////////////////////////////////////////////////////////////////
// class C4CSegmentPencil
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::C4CSegmentPencil::~C4CSegmentPencil()
{
}


/**
 * Constructor.  The pencil will only reference the array of
 * segments [segments]. The user has to take care of its allocation.
 * @param segments an array of segments
 * @param j the index of the first segment in the pencil.
 * @param k the index after the last segment in the pencil.
 * @param m the modulo for the indices.
 * @param l the interpolation function.
 * @param lp the derivative of the interpolation function.
 */
ImaGene::C4CSegmentPencil::C4CSegmentPencil( C4CSegment* segments,
					     uint j, uint k, uint m,
					     const R2RFunction & l, 
					     const R2RFunction & lp )
  : m_segments( segments ),
    m_j( j ), m_k( k ), m_modulo( m ),
    m_bell_fct( l ), m_bell_fct_derivative( lp )
{
}



/**
 * Estimates the orientation of the tangent vector at the discrete
 * contour point [m]. Use an interpolation of the orientation of the
 * maximal segments surrounding the point.
 *
 * @param m any point of the segment expressed in the local reference frame.
 * @return the orientation of the tangent vector at [m] expressed in the local reference frame.
 */
float
ImaGene::C4CSegmentPencil::angleToX( const Vector2D & m ) const
{
  float theta = 0.0f;
  float acc = 0.0f;
  //cout << "  Pencil m=(" << m.x() << "," << m.y() << ")" << endl;
  // cerr << "Pencil ";
  for ( uint i = m_j; i != m_k; i = ( i+1 ) % m_modulo )
    {
      float e_i = m_segments[ i ].eccentricity( m );
      //float lambda_i = m_segments[ i ].b() * m_bell_fct( e_i );
      float lambda_i = m_bell_fct( e_i );
      Vector2i tgt_i = m_segments[ i ].getTangent();
      float theta_i = atan( (float) tgt_i.y() / (float) tgt_i.x() );
      theta += theta_i * lambda_i;
      acc += lambda_i;
      // cerr << " (" << e_i << " " << lambda_i << " " << tgt_i.x()
      // 	   << " " << tgt_i.y() << " " << theta_i << ")";
    }
  theta /= acc;
  // cout << "  => xg=(" << xg.x() << "," << xg.y() << ")" << endl;
  // cerr << " theta=" << theta << endl;
  return theta;
}


/**
 * Estimates the orientation of the tangent vector at the discrete
 * contour point [m]. Use an interpolation of the orientation of the
 * maximal segments surrounding the point.
 *
 * @param m any point of the segment expressed in the local reference frame.
 * @return the orientation of the tangent vector at [m] expressed in the local reference frame.
 */
float
ImaGene::C4CSegmentPencil::angleToXMeasure( const Vector2D & m ) const
{
  float theta = 0.0f;
  float acc = 0.0f;
  //cout << "  Pencil m=(" << m.x() << "," << m.y() << ")" << endl;
  // cerr << "Pencil ";
  for ( uint i = m_j; i != m_k; i = ( i+1 ) % m_modulo )
    {
      float e_i = m_segments[ i ].eccentricity( m );
      //float lambda_i = m_segments[ i ].b() * m_bell_fct( e_i );
      float lambda_i = m_bell_fct( e_i );
      Vector2i tgt_ii = m_segments[ i ].getTangent();
      double tgt_i =  m_segments[ i ].getTangentMeasure();
      float theta_ii = atan( (float) tgt_ii.y() / (float) tgt_ii.x() );
      float theta_i = atan(tgt_i);
      theta += theta_i * lambda_i;
      acc += lambda_i;
      //cerr << " (" << e_i << " " << lambda_i << " " << (float) tgt_ii.y() / (float) tgt_ii.x() << " " << theta_ii << " " << tgt_i << " " << theta_i<< ")" << endl;
    }
  theta /= acc;
  // cout << "  => xg=(" << xg.x() << "," << xg.y() << ")" << endl;
  // cerr << " theta=" << theta << endl;
  return theta;
}


/**
 * Estimates the projection of the discrete contour point [m] onto
 * its real contour. Uses interpolation between regular projection
 * onto each segment.
 *
 * @param m any point of the segment expressed in the local reference frame.
 * @return the continuous analog of m expressed in the local reference frame.
 */
ImaGene::Vector2D
ImaGene::C4CSegmentPencil::continuousAnalog( const Vector2D & m ) const
{
  Vector2D xg( 0.0f, 0.0f );
  
  float acc = 0.0f;
  //cout << "  Pencil m=(" << m.x() << "," << m.y() << ")" << endl;
  for ( uint i = m_j; i != m_k; i = ( i+1 ) % m_modulo )
    {
      float e_i = m_segments[ i ].eccentricity( m );
      //float lambda_i = m_segments[ i ].b() * m_bell_fct( e_i );
      float lambda_i = m_bell_fct( e_i );
      Vector2D p_i = m_segments[ i ].eccentricityPoint( e_i );
      //      cout << "    S[" << i << "] e_i=" << e_i << " l_i=" << lambda_i
      // << " p_i=(" << p_i.x() << "," << p_i.y() << ")" << endl;
      
      p_i *= lambda_i;
      xg += p_i;
      acc += lambda_i;
    }
  xg /= acc;
  // cout << "  => xg=(" << xg.x() << "," << xg.y() << ")" << endl;
  return xg;
}


/**
 * Estimates the first variation of the projection of the discrete
 * contour point [m] onto its real contour. Uses interpolation between
 * regular projection onto each segment.
 *
 * @param m any point of the segment expressed in the local reference frame.
 * @return the derivative of the continuous analog of m expressed in the local reference frame.
 */
ImaGene::Vector2D
ImaGene::C4CSegmentPencil::continuousAnalogDerivative
( const Vector2D & m ) const
{
  Vector2D q = continuousAnalog( m );
  Vector2D qp( 0.0f, 0.0f );
  float acc = 0.0f;
  //cout << "  Pencil m=(" << m.x() << "," << m.y() << ")" << endl;
  for ( uint i = m_j; i != m_k; i = ( i+1 ) % m_modulo )
    {
      Vector2i n_i( m_segments[ i ].cp_n() );
      n_i -= m_segments[ i ].c_n();
      float e_i = m_segments[ i ].eccentricity( m );
      //float lambda_i = m_segments[ i ].b() * m_bell_fct( e_i );
      float lambda_i = m_bell_fct( e_i );
      float lambdap_i = m_bell_fct_derivative( e_i );
      Vector2D p_i = m_segments[ i ].eccentricityPoint( e_i );
      Vector2D uv_i = m_segments[ i ].project( m_segments[ i ].cp_n() );
      uv_i -= m_segments[ i ].project( m_segments[ i ].c_n() );
      
      p_i -= q;
      p_i *= lambdap_i / n_i.norm1();
      uv_i *= lambda_i;
      qp += p_i;
      qp += uv_i;
      //      cout << "    S[" << i << "] e_i=" << e_i << " l_i=" << lambda_i
      // << " p_i=(" << p_i.x() << "," << p_i.y() << ")" << endl;
      acc += lambda_i;
    }
  qp /= acc;
  // cout << "  => xg=(" << xg.x() << "," << xg.y() << ")" << endl;
  return qp;
}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::C4CSegmentPencil::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CSegmentPencil]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::C4CSegmentPencil::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
