/** @file VectorUtils.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : VectorUtils.h
//
// Creation : 2004/11/22
//
// Version : 2004/11/22
//
// Author : JOL
//
// Summary : Header file for files VectorUtils.ih and VectorUtils.cxx
//
// History :
//	2004/11/22 : ?Name? : ?What?
//
// Rcs Id : "@(#)class VectorUtils declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(VectorUtils_RECURSES)
#error Recursive header files inclusion detected in VectorUtils.h
#else // defined(VectorUtils_RECURSES)
#define VectorUtils_RECURSES

#if !defined VectorUtils_h
#define VectorUtils_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
// #include "LinAlg/LinAlg/Vector.hpp"
// #include "LinAlg/LinAlg2D/Vector2D.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Vector2i.h"
//////////////////////////////////////////////////////////////////////////////

//#include "ImaGene/planeRecognition/utils.h"
//#include "ImaGene/planeRecognition/Z2.h"

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class VectorUtils
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'VectorUtils' <p>
 * Aim: Provides a set of functions involving vectors.
 */
class VectorUtils
{

  // ------------------------- Static services ------------------------------
public:

  /**
   * @param v any vector.
   * @return its euclidean norm.
   */
  static float norm( const Vector2D & v );

  /**
   * @param v (returns) normalize the vector so that its euclidean norm is 1.
   */
  static void normalize( Vector2D & v );

  /**
   * @param v1 any vector.
   * @param v2 any vector.
   * @return the norm of the vector [v1]-[v2], ie the distance between [v1] and [v2] when seen as points.
   */
  static float distance( const Vector2D & v1, const Vector2D & v2 );

  /**
   * @param v1 any vector.
   * @param v2 any vector.
   * @return the dot product of [v1] and [v2].
   */
  static float dotProduct( const Vector2D & v1, const Vector2D & v2 );
  
  /**
 * @param v any vector.
 * @param s any scalar.
 * @return v*s.
 */
  static Vector2D product( const Vector2D & v, const float & s );
  
  /**
   * @param v1 any vector.
   * @param v2 any vector.
   * @return the determinant of [v1] and [v2].
   */
  static float det( const Vector2D & v1, const Vector2D & v2 );

  /**
   * @param v any vector.
   * @return the angle of [v] with the x-axis.
   */
  static float getAngle( const Vector2D & v );
  
//   /**
//  * @param v any vector.
//  * @param sign signe for the bezout (1 or -1).
//  * @return the Bezout vector of v.
//  */
//   static Vector2D bezout( const Vector2D & v, const int sign );
  
//   /**
//  * @param u any vector.
//  * @param sign signe for the bezout (1 or -1).
//  * @return the Bezout vector of u.
//  */
//   static Vector2D closestBezout( const Vector2D & u, const int sign );
  
//   /**
//  * @param u any vector.
//  * @param sign sign for the bezout (1 or -1).
//  * @return the Bezout vector of u.
//  */
//   static Vector2D bezoutWithConstraints( const Vector2D & u, const int sign,  Vector2i A, int & borneSup );


  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~VectorUtils();


  // ------------------------- Datas ------------------------------------------
private:

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden by default (protected to avoid g++ warnings).
   */
  INLINE VectorUtils();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE VectorUtils( const VectorUtils & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE VectorUtils & operator=( const VectorUtils & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};


} // namespace ImaGene

 

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/VectorUtils.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined VectorUtils_h

#undef VectorUtils_RECURSES
#endif // else defined(VectorUtils_RECURSES)
