/** @file DLine.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DLine.h
//
// Creation : 2003/02/12
//
// Version : 2003/02/12
//
// Author : JOL
//
// Summary : Header file for files DLine.ih and DLine.cxx
//
// History :
//	2003/02/12 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DLine declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DLine_RECURSES)
#error Recursive header files inclusion detected in DLine.h
#else // defined(DLine_RECURSES)
#define DLine_RECURSES

#if !defined DLine_h
#define DLine_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/Vector2i.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class DLine
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'DLine' <p>
 * Aim: to represent a digital line in Z2, of the form a*x-b*y+mu.
 * Can represent naive, standard, or lines with any thickness.
 */
class DLine
{

  // ------------------------- Static services ------------------------------
public:

  /**
   * Computes the median line of [l1] and [l2], defined by the intersection of
   * the upper and ower leaning lines of [l1] and [l2] (forms a parallelogram).
   *
   * @pre l1 and l2 concurs and are defined in the same frame.
   * @param l1 the first discrete line
   * @param l2 the second discrete line
   * @return the median discrete line (not necessarily 4-connected).
   */
  static DLine medianLine( const DLine & l1, const DLine & l2 );
  

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  INLINE ~DLine();

  /**
   * Constructor. Does nothing.
   */
  INLINE DLine();

  /**
   * Constructor of the standard line.
   * @param coef_a the coefficient 'a' in the equation 'a*x-b*y+mu'.
   * @param coef_b the coefficient 'b' in the equation 'a*x-b*y+mu'.
   * @param coef_mu the coefficient 'mu' in the equation 'a*x-b*y+mu'.
   */
  INLINE DLine( int coef_a, int coef_b, int coef_mu );

  /**
   * Constructor of any discrete line.
   * @param coef_a the coefficient 'a' in the equation 'a*x-b*y+mu'.
   * @param coef_b the coefficient 'b' in the equation 'a*x-b*y+mu'.
   * @param coef_mu the coefficient 'mu' in the equation 'a*x-b*y+mu'.
   * @param coef_mup the coefficient 'mup' (lower leaning line).
   */
  INLINE DLine( int coef_a, int coef_b, int coef_mu, int coef_mup );

  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  INLINE DLine( const DLine & other );

  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE DLine & operator=( const DLine & other );

  // ------------------------- Geometry services ------------------------------
public:

  /**
   * @return 'true' if this is a standard line (ie. mup = mu + |a| + b - 1 ).
   */
  INLINE bool isStandard() const;

  /**
   * @return the vector tangent to this digital line (b,a)
   */
  INLINE Vector2i tangent() const;

  /**
   * @return the vector normal to this digital line (-a,b)
   */
  INLINE Vector2i normal() const;

  /**
   * @param x any value for the x-coordinate.
   * @return the uppermost value y for this [x] so that (x,y) belongs to the line.
   */
  INLINE float upperY( float x ) const;
  
  /**
   * @param x any value for the x-coordinate.
   * @return the uppermost value y for this [x] so that (x,y) belongs to the line.
   */
  INLINE float lowerY( float x ) const;
  

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
  
  // ------------------------- Datas ------------------------------------------
public:
  /**
   * the coefficient 'a' in the equation 'a*x-b*y+mu'.
   */
  int a;
  /**
   * @param b the coefficient 'b' in the equation 'a*x-b*y+mu'.
   */
  int b;
  /**
   * @param mu the coefficient 'mu' in the equation 'a*x-b*y+mu'.
   */
  int mu;
  /**
   * @param mup the coefficient defining the lower leaning line.
   * For standard line, mup = mu + |a| + b - 1
   */
  int mup;
  
  // ------------------------- Hidden services --------------------------------
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'DLine'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'DLine' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const DLine & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/DLine.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DLine_h

#undef DLine_RECURSES
#endif // else defined(DLine_RECURSES)
