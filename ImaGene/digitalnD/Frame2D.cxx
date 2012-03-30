///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Frame2D.cxx
//
// Creation : 2004/11/24
//
// Version : 2004/11/24
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
//	2004/11/24 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/digitalnD/Frame2D.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/Frame2D.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const Frame2D_RCS_ID = "@(#)class Frame2D definition.";

///////////////////////////////////////////////////////////////////////////////
// class Frame2D
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::Frame2D::~Frame2D()
{
}


/**
 * Initializes the frame with a digital space [ks], the first axis
 * [i] and the second axis [j].
 *
 * @param ks a pointer on a digital space.
 * @param i the first axis (ie "x").
 * @param j the second axis (ie "y").
 */
void
ImaGene::Frame2D::init( KnSpace* ks, uint i, uint j )
{
  m_ks = ks;
  m_x = i;
  m_y = j;
}


/**
 * Most geometric computations of 'Frame2D' transforms
 * vectors/angles expressed in a surfel frame to the common
 * frame. This method sets the surfel frame.
 *
 * <pre> both the orthogonal direction of [s] and [track_dir] are
 * 'x()' or 'y()' </pre>
 *
 * @param s any surfel with 'orthDir( s )' equal to 'x()' or 'y()'.
 * @param track_dir a direction different from 'orthDir( s )' and
 * equal to 'x()' or 'y()'.
 */
void 
ImaGene::Frame2D::setSurfelFrame( Kn_sid s, uint track_dir )
{
  m_odir = m_ks->sorthDir( s );
  m_tdir = track_dir;
  ASSERT_Frame2D( ( ( m_odir == x() ) && ( m_tdir == y() ) )
		  || ( ( m_odir == y() ) && ( m_tdir == x() ) ) );
  m_opos = ! m_ks->sdirect( s, m_odir );
  m_osign = m_opos ? 1.0f : -1.0f;
  m_tpos = m_ks->sdirect( s, m_tdir );
  m_tsign = m_tpos ? 1.0f : -1.0f;

  // JOL 2006/02/21: added center of frame
  Kn_sid extremity = m_ks->sincident( s, m_tdir, ! m_tpos );
  m_x0 = (int) m_ks->sdecodeCoord( extremity, x() );
  m_y0 = (int) m_ks->sdecodeCoord( extremity, y() );
}




//////////////////////////////////////////////////////////////////////
// ------------------------- Geometric services -----------------------------
  

/**
 * Given an angle expressed in the current surfel frame, returns the
 * angle wrt axis 'x()'.
 *
 * @param angle any angle in radian.
 * @return the angle wrt axis 'x()' between [0;2pi[.
 */
float
ImaGene::Frame2D::angleToX( float angle ) const
{
  float a;
  if ( m_odir == y() )
    {
      a = m_tsign * m_osign * angle;
      if ( m_tsign < 0.0f ) a += M_PI;
    }
  else
    {
      a = - m_tsign * m_osign * angle + m_tsign * ( M_PI / 2.0 );
    }
  float twopi = 2.0 * M_PI;
  while ( a < 0.0f ) a += twopi;
  while ( a >= twopi ) a-= twopi;
  return a;
}


/**
 * Given an angle expressed in the current surfel frame, returns a
 * unit vector: the direction expressed in the frame.
 *
 * @param angle any angle in radian.
 * @return the direction as a vector in the frame.
 */
ImaGene::Vector2D
ImaGene::Frame2D::direction( float angle ) const
{
  float a = angleToX( angle );
  return Vector2D( cos( a ), sin( a ) );
}


/**
 * Given a vector expressed in the current surfel frame, returns the
 * corresponding vector expressed in the 2D frame.
 *
 * @param v any vector of the surfel frame.
 * @return the vector in the 2D frame.
 */
ImaGene::Vector2D
ImaGene::Frame2D::transform( const Vector2D & v ) const
{
  if ( m_odir == y() )
    {
      return Vector2D( v.ro( 0 ) * m_tsign, v.ro( 1 ) * m_osign );
    }
  else
    {
      return Vector2D( v.ro( 1 ) * m_osign, v.ro( 0 ) * m_tsign );
    }
}


/**
 * Given a point expressed in the current surfel frame, returns the
 * corresponding point expressed in the 2D frame.
 *
 * @param p any point of the surfel frame.
 * @return the point in the 2D frame.
 */
ImaGene::Vector2D
ImaGene::Frame2D::transformPoint( const Vector2D & p ) const
{
  Vector2D pp( transform( p ) );
  pp.x() += m_x0;
  pp.y() += m_y0;
  return pp;
}
 
/**
 * Given a vector expressed in the current surfel frame, returns the
 * corresponding vector expressed in the 2D frame.
 *
 * @param v any vector of the surfel frame.
 * @return the vector in the 2D frame.
 */
ImaGene::Vector2i
ImaGene::Frame2D::transform( const Vector2i & v ) const
{
  if ( m_odir == y() )
    {
      return Vector2i( m_tpos ? v.x() : -v.x(),
		       m_opos ? v.y() : -v.y() );
    }
  else
    {
      return Vector2i( m_opos ? v.y() : -v.y(),
		       m_tpos ? v.x() : -v.x() );
    }
}


/**
 * Given a point expressed in the current surfel frame, returns the
 * corresponding point expressed in the 2D frame.
 *
 * @param p any point of the surfel frame.
 * @return the point in the 2D frame.
 */
ImaGene::Vector2i
ImaGene::Frame2D::transformPoint( const Vector2i & p ) const
{
  Vector2i pp( transform( p ) );
  pp.x() += m_x0;
  pp.y() += m_y0;
  return pp;
}
 


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::Frame2D::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Frame2D]"
	      << " m_x0=" << m_x0 
	      << " m_y0=" << m_y0
	      << endl;
 
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::Frame2D::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
