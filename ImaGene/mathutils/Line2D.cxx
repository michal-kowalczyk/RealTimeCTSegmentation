///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Line2D.cxx
//
// Creation : 2008/10/30
//
// Version : 2008/10/30
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
//	2008/10/30 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/VectorUtils.h"
#include "ImaGene/mathutils/Line2D.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/Line2D.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const Line2D_RCS_ID = "@(#)class Line2D definition.";



///////////////////////////////////////////////////////////////////////////////
// class Line2D
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::Line2D::~Line2D()
{
}

/**
 * Computes the point at the intersection of 'this' and [other] if any.
 *
 * @param other the other straight line.
 * @param p the point that is both on 'this' and 'other'.
 *
 * @return 'true' if [p] exists, 'false' otherwise (the two lines
 * are parallel).
 */
bool 
ImaGene::Line2D::intersectionPoint( const Line2D & other, Vector2D & p ) const
{
  float dvu = VectorUtils::det( other.m_u, m_u );
  if ( dvu == 0.0f )
    return false;

  Vector2D PQ( other.m_p1 ); PQ -= m_p1;
  float t = VectorUtils::det( m_u, PQ ) / dvu;
  p = other.m_u;
  p *= t;
  p += other.m_p1;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::Line2D::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Line2D]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::Line2D::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
