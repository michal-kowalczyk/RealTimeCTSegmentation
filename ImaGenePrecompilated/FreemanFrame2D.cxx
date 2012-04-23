///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : FreemanFrame2D.cxx
//
// Creation : 2005/04/20
//
// Version : 2005/04/20
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
//	2005/04/20 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/dgeometry2d/FreemanFrame2D.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/FreemanFrame2D.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const FreemanFrame2D_RCS_ID = "@(#)class FreemanFrame2D definition.";



///////////////////////////////////////////////////////////////////////////////
// class FreemanFrame2D
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::FreemanFrame2D::~FreemanFrame2D()
{
}



///////////////////////////////////////////////////////////////////////////////
// ------------------------- Geometric services ---------------------------
    
    
/**
 * Given an angle expressed in the current surfel frame, returns the
 * angle wrt axis 'x()'.
 *
 * @param angle any angle in radian.
 * @return the angle wrt axis 'x()'.
 */
float
ImaGene::FreemanFrame2D::angleToX( float angle ) const
{
  float t = m_fc*M_PI/2.0 - angle;
  return t < 0.0 ? t + 2*M_PI : ( t >= 2*M_PI  ? t - 2*M_PI : t );
}


/**
 * Given an angle expressed in the current surfel frame, returns a
 * unit vector: the direction expressed in the frame.
 *
 * @param angle any angle in radian.
 * @return the direction as a vector in the frame.
 */
ImaGene::Vector2D
ImaGene::FreemanFrame2D::direction( float angle ) const
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
ImaGene::FreemanFrame2D::transform( const Vector2D & v ) const
{
  switch ( m_fc )
    {
    case 0: return Vector2D( v.ro( 0 ), -v.ro( 1 ) );
    case 1: return Vector2D( v.ro( 1 ), v.ro( 0 ) );
    case 2: return Vector2D( -v.ro( 0 ), -v.ro( 1 ) );
    case 3: return Vector2D( -v.ro( 1 ), v.ro( 0 ) );
    }
}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::FreemanFrame2D::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[FreemanFrame2D]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::FreemanFrame2D::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
