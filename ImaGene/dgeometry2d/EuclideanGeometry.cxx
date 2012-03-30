///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : EuclideanGeometry.cxx
//
// Creation : 2004/01/22
//
// Version : 2004/01/22
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
//	2004/01/22 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/dgeometry2d/EuclideanGeometry.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/EuclideanGeometry.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const EuclideanGeometry_RCS_ID = "@(#)class EuclideanGeometry definition.";

///////////////////////////////////////////////////////////////////////////////
// class EuclideanGeometry
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::EuclideanGeometry::~EuclideanGeometry()
{
}

/**
 * Constructor. 
 */
ImaGene::EuclideanGeometry::EuclideanGeometry()
{
}


///////////////////////////////////////////////////////////////////////////////
// ------------------------- 2D geometric services --------------------------

/**
 * Computes the radius of the circle circumscribed to the 3 points (xa,ya),
 * (xb,yb), (xc, yc).
 *
 * @param xa the x-coordinate of the first point.
 * @param ya the y-coordinate of the first point.
 * @param xb the x-coordinate of the second point.
 * @param yb the y-coordinate of the second point.
 * @param xc the x-coordinate of the third point.
 * @param yc the y-coordinate of the third point.
 * @param val_for_infinity the value to return when the three points are aligned and the circle is infinite.
 * @return the radius of the circumscribed circle or -1 if some numerical errors occured.
 *
 * NB. Taken from some code of David Coeurjolly, LIRIS, Lyon, France.
 */
double
ImaGene::EuclideanGeometry::radiusCircumscribedCircle( double xa, double ya,
						       double xb, double yb,
						       double xc, double yc,
						       double val_for_infinity = -2.0 )
{
  double curv = curvatureCircumscribedCircle( xa, ya, xb, yb, xc, yc );
  if ( curv == 0.0 )
    return val_for_infinity;
  return 1.0 / curv;
}


/**
 * Computes the curvature of the circle circumscribed to the 3 points (xa,ya),
 * (xb,yb), (xc, yc).
 *
 * @param xa the x-coordinate of the first point.
 * @param ya the y-coordinate of the first point.
 * @param xb the x-coordinate of the second point.
 * @param yb the y-coordinate of the second point.
 * @param xc the x-coordinate of the third point.
 * @param yc the y-coordinate of the third point.
 * @return the curvature of the circumscribed circle or -1 if some numerical errors occured.
 *
 * NB. Taken from some code of David Coeurjolly, LIRIS, Lyon, France.
 */
double
ImaGene::EuclideanGeometry::curvatureCircumscribedCircle
( double xa, double ya,
  double xb, double yb,
  double xc, double yc )
{
  double a,b,c;
  double A;

  a=sqrt((xb-xa)*(xb-xa)+(yb-ya)*(yb-ya));
  b=sqrt((xb-xc)*(xb-xc)+(yb-yc)*(yb-yc));
  c=sqrt((xa-xc)*(xa-xc)+(ya-yc)*(ya-yc));
  A=sqrt((b+c)*(b+c)-a*a)*sqrt(a*a-(b-c)*(b-c)) / 4.0;

  // Tests
  if (((b+c)*(b+c)-a*a <0) || (a*a-(b-c)*(b-c) <0))
    return -1;
  if (A==0)
    return -1;
  else
    return (4*A)/(a*b*c);
}

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::EuclideanGeometry::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
