/** @file EuclideanGeometry.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : EuclideanGeometry.h
//
// Creation : 2004/01/22
//
// Version : 2004/01/22
//
// Author : JOL
//
// Summary : Header file for files EuclideanGeometry.ih and EuclideanGeometry.cxx
//
// History :
//	2004/01/22 : ?Name? : ?What?
//
// Rcs Id : "@(#)class EuclideanGeometry declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(EuclideanGeometry_RECURSES)
#error Recursive header files inclusion detected in EuclideanGeometry.h
#else // defined(EuclideanGeometry_RECURSES)
#define EuclideanGeometry_RECURSES

#if !defined EuclideanGeometry_h
#define EuclideanGeometry_h

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class EuclideanGeometry
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'EuclideanGeometry' <p>
 * Aim: A set of standard geometric operations.
 */
class EuclideanGeometry
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Constructor.
   */
  EuclideanGeometry();

  /**
   * Destructor. 
   */
  ~EuclideanGeometry();

  // ------------------------- 2D geometric services --------------------------
public:

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
  static double radiusCircumscribedCircle( double xa, double ya,
					   double xb, double yb,
					   double xc, double yc,
					   double val_for_infinity );
  /**
   * Computes the curvature of the circle circumscribed to the 3
   * points (xa,ya), (xb,yb), (xc, yc).
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
  static double curvatureCircumscribedCircle( double xa, double ya,
					      double xb, double yb,
					      double xc, double yc );
  
  // ------------------------- Interface --------------------------------------
public:
  /**
   * Checks the validity/consistency of the object.
   * @return 'true' if the object is valid, 'false' otherwise.
   */
  bool OK() const;
  
  // ------------------------- Datas ------------------------------------------
private:

  // ------------------------- Hidden services --------------------------------
protected:
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE EuclideanGeometry( const EuclideanGeometry & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE EuclideanGeometry & operator=( const EuclideanGeometry & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "EuclideanGeometry.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined EuclideanGeometry_h

#undef EuclideanGeometry_RECURSES
#endif // else defined(EuclideanGeometry_RECURSES)
