/** @file PWFAffineFunction.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : PWFAffineFunction.h
//
// Creation : 2004/04/14
//
// Version : 2004/04/14
//
// Author : JOL
//
// Summary : Header file for files PWFAffineFunction.ih and PWFAffineFunction.cxx
//
// History :
//	2004/04/14 : ?Name? : ?What?
//
// Rcs Id : "@(#)class PWFAffineFunction declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(PWFAffineFunction_RECURSES)
#error Recursive header files inclusion detected in PWFAffineFunction.h
#else // defined(PWFAffineFunction_RECURSES)
#define PWFAffineFunction_RECURSES

#if !defined PWFAffineFunction_h
#define PWFAffineFunction_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/base/Shared.h"
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class PWFAffineFunctionData
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'PWFAffineFunctionData' <p> Aim: Stores the
 * data for piecewise affine functions with a finite range. The class
 * 'PWFAffineFunction' performs the operation on these data.
 */
class PWFAffineFunctionData
{
  friend class PWFAffineFunction;
  
  typedef std::vector<float>::iterator viterator;
  typedef std::vector<float>::const_iterator const_viterator;
  
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~PWFAffineFunctionData();

  /**
   * Constructor. Empty affine function. 
   */
  PWFAffineFunctionData();

  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  PWFAffineFunctionData( const PWFAffineFunctionData & other );

  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   */
  PWFAffineFunctionData & operator=( const PWFAffineFunctionData & other );


  // ------------------------- Initializers ---------------------------------
public:

  /**
   * Initializer from points and affine pieces.
   * @param x the (sorted) positions of the samples.
   * @param fx the sample values.
   * @param alpha the slope coefficients for each piece.
   * @param beta the offset coefficients for each piece.
   */
  void init( const std::vector<float> & x, 
	     const std::vector<float> & fx,
	     const std::vector<float> & alpha,
	     const std::vector<float> & beta );

  /**
   * Initializer from points. The affine function is assumed continuous. O(n).
   * @param x the (sorted) positions of the samples.
   * @param fx the sample values.
   */
  void init( const std::vector<float> & x, 
	     const std::vector<float> & fx );


  // ------------------------- Basic modification services --------------------
public:

  /**
   * Splits the function at position [x]. The new function has the same
   * graph but not the same representation. Does nothing if the affine
   * function is already split at this coordinate. O(log(n)).
   * 
   * @param x the position where to split.
   */
  void split( float x );
  
  /**
   * Sets the value at point [x] to be [fx]. This may destroy continuity.
   * The function must be split at this point before. O(log(n)).
   * 
   * @param x a position.
   * @param fx the sample value.
   */
  void setFx( float x, float fx );

  /**
   * Sets the values for an open interval [x1,x2] to be the affine
   * function [alpha*x+beta]. This may destroy continuity. 
   * The function must be split at [x1] and [x2]. ~O(log(n)).
   *
   * @param x1 the left-handside of the interval.
   * @param x2 the right-handside of the interval.
   * @param alpha the slope of the straight line.
   * @param beta the offset of the straight line.
   */
  void setFinterval( float x1, float x2, float alpha, float beta );

  /**
   * Sets the values for an open interval [x1,x2] to be the affine
   * function [alpha*x+beta]. This may destroy continuity. 
   * The function must be split at [x1] and [x2]. ~O(log(n)).
   *
   * @param x1 the left-handside of the interval.
   * @param x2 the right-handside of the interval.
   * @param y1 the limit value at point x1. 
   * @param y2 the limit value at point x2. 
   */
  void setFintervalPoints( float x1, float x2, float y1, float y2 );

  /**
   * Multiplies the whole function with value [lambda] (O(n)).
   * @param lambda any factor.
   */
  void multiply( float lambda );

  /**
   * @return the first moment of the function (O(n)).
   */
  float centroid() const;
  
  /**
   * @return the variance of the function.
   * @see centroid.
   */
  float variance() const;

  /**
   * @param mu a given mean (can be 'centroid()').
   * @return the second moment of the function around the given mean [mu].
   * @see centroid.
   */
  float moment2( float mu ) const;
  

  // ------------------------- Basic function services --------------------
public:

  /**
   * @param x any position.
   * @return the value of the function at point [x].
   */
  float value( float x ) const;

  /**
   * @param x (returns) the position of the first maximal value.
   * @param limit (returns) if 0 the maximal value is at [x], if positive the maximal value is to the right of [x], if negative the maximal value is to the left of [x].
   * @return the maximal value of the function at point [x].
   */
  float firstMax( float & x, int & limit ) const;
  


//   // ------------------------- Internals --------------------------------------
// private:

//   /**
//    * Replace the interval at position [at_idx].
//    * 
//    * @param at_idx the index of the interval to replace.
//    * @param a the new alpha.
//    * @param b the new beta.
//    */
//   float replaceInterval( uint at_idx, float a, float b );
  
//   /**
//    * Replace the interval at position [at_idx].
//    * 
//    * @param at_idx the index of the interval to replace.
//    * @param y1 the value to the left of the interval.
//    * @param y2 the value to the right of the interval.
//    */
//   float replaceInterval( uint at_idx, float y1, float y2 );


  // ------------------------- Datas ------------------------------------------
private:

  uint m_nb_points;
  std::vector<float> m_x;
  std::vector<float> m_fx;
  std::vector<float> m_alpha;
  std::vector<float> m_beta;
  
};
  
  


///////////////////////////////////////////////////////////////////////////////
// class PWFAffineFunction
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'PWFAffineFunction' <p> Aim: Represents
 * (formally) a piecewise affine function with a finite range (has
 * value zero outside range). Uses 'PWFAffineFunctionData' to store
 * the values.
 */
class PWFAffineFunction
{

  // ------------------------- Static services ------------------------------
public:

  /**
   * Sums affine functions.
   * @param f1 any affine function.
   * @param f2 any affine function.
   * @return the sum of f1 and f2.
   */
  static PWFAffineFunction add( const PWFAffineFunction & f1,
				const PWFAffineFunction & f2 );

  /**
   * Multiply an affine function by a scalar and returns the result.
   * @param f any affine function.
   * @param lambda any scalar.
   * @return the product of f by lambda.
   */
  static PWFAffineFunction multiply( const PWFAffineFunction & f,
				     float lambda );

  /**
   * @param a any real value.
   * @param b any real value greater than a.
   * @return the affine function that is the probability density of
   * the uniform law on the interval [a,b].
   */
  static PWFAffineFunction uniformDP( float a, float b );

  /**
   * @param a any real value.
   * @param b any real value greater than a.
   * @return the affine function that is the probability density of
   * the triangular law on the interval [a,b].
   */
  static PWFAffineFunction triangularDP( float a, float b );

  /**
   * @param a any real value.
   * @param b any real value greater than a.
   * @param l a real value between 0 and 0.5 (0 gives uniform law, 0.5 gives triangular law, linear in-between).
   *
   * @return the affine function that is the probability density of
   * the symmetric trapezoidal law on the interval [a,b] with shape
   * coefficient [l].
   */
  static PWFAffineFunction trapezoidalDP( float a, float b, float l );
  
  
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~PWFAffineFunction();

  /**
   * Default Constructor. Builds the empty affine function.
   */
  PWFAffineFunction();

  /**
   * Constructor from data.
   * @param data any data corresponding to a piecewise affine function.
   */
  PWFAffineFunction( const PWFAffineFunctionData & data );

  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  PWFAffineFunction( const PWFAffineFunction & other );

  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   */
  PWFAffineFunction & operator=( const PWFAffineFunction & other );


  /**
   * Read-only accessor to data associated with affine function.
   * @return a const reference to the data of the affine function.
   */
  const PWFAffineFunctionData & data() const;

  /**
   * Read-write accessor to data associated with affine function.
   * @return a reference to the data of the affine function.
   */
  PWFAffineFunctionData & data();
  

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
private:

  /**
   * References this object to store its data.
   */
  Shared<PWFAffineFunctionData> m_data;
  

  // ------------------------- Hidden services --------------------------------
protected:
  
  // ------------------------- Internals --------------------------------------
private:
  
};


/**
 * Sums affine functions.
 * @param f1 any affine function.
 * @param f2 any affine function.
 * @return the sum of f1 and f2.
 */
INLINE PWFAffineFunction
operator+( const PWFAffineFunction & f1, const PWFAffineFunction & f2 );


/**
 * Multiply an affine function by a scalar and returns the result.
 * @param f any affine function.
 * @param lambda any scalar.
 * @return the product of f by lambda.
 */
INLINE PWFAffineFunction
operator*( const PWFAffineFunction & f, float lambda );


/**
 * Multiply an affine function by a scalar and returns the result.
 * @param f any affine function.
 * @param lambda any scalar.
 * @return the product of f by lambda.
 */
INLINE PWFAffineFunction
operator*( float lambda, const PWFAffineFunction & f );


/**
 * Overloads 'operator<<' for displaying objects of class 'PWFAffineFunction'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'PWFAffineFunction' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const PWFAffineFunction & that_object_to_display );
 

} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "PWFAffineFunction.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined PWFAffineFunction_h

#undef PWFAffineFunction_RECURSES
#endif // else defined(PWFAffineFunction_RECURSES)
