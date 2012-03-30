/** @file Vector2i.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Vector2i.h
//
// Creation : 2003/02/12
//
// Version : 2003/02/12
//
// Author : JOL
//
// Summary : Header file for files Vector2i.ih and Vector2i.cxx
//
// History :
//	2003/02/12 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Vector2i declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Vector2i_RECURSES)
#error Recursive header files inclusion detected in Vector2i.h
#else // defined(Vector2i_RECURSES)
#define Vector2i_RECURSES

#if !defined Vector2i_h
#define Vector2i_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class Vector2i
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'Vector2i' <p>
 * Aim: Defines a vector of two integer numbers.
 */
class Vector2i
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  INLINE ~Vector2i();

  /**
   * Constructor. Does nothing.
   */
  INLINE Vector2i();

  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  INLINE Vector2i( const Vector2i & other );

  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   */
  INLINE Vector2i & operator=( const Vector2i & other );

  /**
   * Constructor from two values.
   * @param vx first value.
   * @param vy second value.
   */
  INLINE Vector2i( int vx, int vy );


  // ------------------------- Geometry services -----------------------
public:

  /**
   * This vector is supposed to be a tangent vector of some contour.
   * It can be used to compute a local length measure of the contour.
   * This length depends on the coherence of this vector with the
   * trivial x-axis.  Defined as y/sqrt(x*x+y*y).
   * @pre m_x > 0 
   * @return the corrected length associated with this line segment.
   */
  INLINE float correctedLength() const;

  /**
   * This vector is supposed to be a tangent vector of some contour.
   * It can be used to compute a local length measure of the contour.
   * This length depends only the normal to the segment.
   * Defined as sqrt(x*x+y*y)/(|x|+|y|).
   * @pre m_x > 0
   * @return the corrected length associated with this line segment.
   */
  INLINE float averagedLength() const;


  // ------------------------- Vector services --------------------------------
public:

  /**
   * @return the norm of the current vector.
   */
  INLINE float norm() const;

  /**
   * @return the norm1 of the current vector.
   */
  INLINE int norm1() const;


  // ------------------------- Freeman move services --------------------------
public:

  /**
   * Moves according to the 4-connected Freeman move [freeman_code].
   *
   * @param freeman_code the Freeman code (0 east, 1 north, 2 west, 3 south).
   */
  INLINE void move4( uint freeman_code );

  /**
   * Our convention for quadrants is :
   * - 0: x > 0, y >=0
   * - 1: y > 0, x <= 0
   * - 2: x < 0, y <= 0
   * - 3: y < 0, x >= 0
   * - 4 for vector (0,0)
   * This guarantees that turnLeft and turnRight change quadrant accordingly.
   *
   * @return the quadrant in which is the current vector (between 0-3).
   */
  INLINE uint quadrant() const;

  /**
   * @return the Vector2i with the absolute values of the components of 'this' 
   */
  INLINE Vector2i abs() const;

  /**
   * @return the Vector2i with the absolute values of the components
   * of 'this' and such that 0 <= y <= x.
   */
  INLINE Vector2i firstOctant() const;

  // ------------------------- Reference frame services -----------------------
public:

  /**
   Turn reference frame to the right (clockwise).
   (a,b) => (-b,a)
   */
  INLINE void turnRight();

  /**
   Turn reference frame to the left (counterclokckwise).
   (a,b) => (b,-a)
   */
  INLINE void turnLeft();

  /**
   * Flips vector so that its x-coordinate is positive.
   */
  INLINE void flipForPositiveX();

  // ------------------------- Accessors --------------------------------------
public:

  /**
   * @return the value of the first vector element.
   */
  INLINE int x() const;

  /**
   * @return a reference on the first vector element.
   */
  INLINE int & x();
  
  /**
   * @return the value of the second vector element.
   */
  INLINE int y() const;

  /**
   * @return a reference on the second vector element.
   */
  INLINE int & y();
  
  // ------------------------- Operations -------------------------------------
public:

  /**
   * Add [other] to 'this'.
   * @param other the object to add.
   * @return a reference on 'this'.
   */
  INLINE Vector2i & operator+=( const Vector2i & other );

  /**
   * Sub [other] from 'this'.
   * @param other the object to subtract.
   * @return a reference on 'this'.
   */
  INLINE Vector2i & operator-=( const Vector2i & other );

  /**
   * Multiply 'this' by integer [c].
   * @param c any integer.
   * @return a reference on 'this'.
   */
  INLINE Vector2i & operator*=( int c );

  /**
   * Divides 'this' by integer [c].
   * @param c any non-null integer.
   * @return a reference on 'this'.
   */
  INLINE Vector2i & operator/=( int c );

  /**
   * Computes the sum of 'this' and [other].
   * @param other the object to add.
   * @return a new object.
   */
  INLINE Vector2i operator+( const Vector2i & other ) const;

  /**
   * Unary minus operator.
   * @return a new object.
   */
  INLINE Vector2i operator-( ) const;

  /**
   * Computes the difference of 'this' and [other].
   * @param other the object to subtract.
   * @return a new object.
   */
  INLINE Vector2i operator-( const Vector2i & other ) const;

  /**
   * @param other any other vector.
   * @return the determinant of 'this' and [other].
   */
  INLINE int det( const Vector2i & other ) const;

  /**
   * @param other any other vector.
   * @return the scalar product of 'this' and [other].
   */
  INLINE int operator*( const Vector2i & other ) const;


  // ------------------------- Comparison -------------------------------------
public:

  /**
   * Compares [other] to 'this'.
   * @param other the object to compare.
   * @return 'true' if [other] is equal to 'this', 'false' otherwise.
   */
  INLINE bool operator==( const Vector2i & other ) const;

  /**
   * Compares [other] to 'this'.
   * @param other the object to compare.
   * @return 'true' if [other] is not equal to 'this', 'false' otherwise.
   */
  INLINE bool operator!=( const Vector2i & other ) const;


  // ------------------------- static services---------------------------------
 public:

  /**
   * Our convention for quadrants is :
   * - 0: x > 0, y >=0
   * - 1: y > 0, x <= 0
   * - 2: x < 0, y <= 0
   * - 3: y < 0, x >= 0
   * - 4 for vector (0,0)
   * This guarantees that turnLeft and turnRight change quadrant accordingly.
   *
   * @param quadrant a number between 0-3.
   * @return the natural x-vector corresponding to this quadrant.
   */
  static Vector2i quadrantX( uint quadrant );

  /**
   * Our convention for quadrants is :
   * - 0: x > 0, y >=0
   * - 1: y > 0, x <= 0
   * - 2: x < 0, y <= 0
   * - 3: y < 0, x >= 0
   * - 4 for vector (0,0)
   * This guarantees that turnLeft and turnRight change quadrant accordingly.
   *
   * @param quadrant a number between 0-3.  @return the natural
   * y-vector corresponding to this quadrant (to the left of
   * quadrantX).
   */
  static Vector2i quadrantY( uint quadrant );

  // ------------------------- Interface --------------------------------------
public:
  /**
   * Writes/Displays the object on an output stream.
   * @param that_stream the output stream where the object is written.
   */
  void selfDisplay( std::ostream & that_stream ) const;
  /**
   * Checks the validity/consistency of the object.
   * @return 'true' if the object is valid, 'false' otherwise.
   */
  bool OK() const;
  
  // ------------------------- datas ------------------------------------------
protected:

  /**
   * the x-coordinate.
   */
  int m_x;

  /**
   * the y-coordinate.
   */
  int m_y;
  

  // ------------------------- Hidden services --------------------------------
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'Vector2i'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'Vector2i' to write.
 * @return the output stream after the writing.
 */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, const Vector2i & that_object_to_display );
  
  /**
   * @param c any integer.
   * @param v any vector.
   * @return the multiplication of [v] by coefficient [c].
   */
  INLINE Vector2i
  operator*( int c, const Vector2i & v );
  
  /**
   * @param v any vector.
   * @param c any integer.
   * @return the multiplication of [v] by coefficient [c].
   */
  INLINE Vector2i
  operator*( const Vector2i & v, int c );

  /**
   * @param v any vector.
   * @param c any non-null integer.
   * @return the (euclidean) division of [v] by coefficient [c].
   */
  INLINE Vector2i
  operator/( const Vector2i & v, int c );

} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/Vector2i.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Vector2i_h

#undef Vector2i_RECURSES
#endif // else defined(Vector2i_RECURSES)
