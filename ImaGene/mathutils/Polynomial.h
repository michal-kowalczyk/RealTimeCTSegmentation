/** @file Polynomial.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Polynomial.h
//
// Creation : 2004/08/10
//
// Version : 2004/08/10
//
// Author : JOL
//
// Summary : Header file for files Polynomial.ih and Polynomial.cxx
//
// History :
//	2004/08/10 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Polynomial declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Polynomial_RECURSES)
#error Recursive header files inclusion detected in Polynomial.h
#else // defined(Polynomial_RECURSES)
#define Polynomial_RECURSES

#if !defined Polynomial_h
#define Polynomial_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/base/Shared.h"
#include "ImaGene/mathutils/Functions.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {


///////////////////////////////////////////////////////////////////////////////
// class PolynomialData
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of struct 'PolynomialData' <p>
 * Aim: A structure to store the coefficients of a polynomial.
 */
class PolynomialData
{
public:
  /**
   * Destructor.
   */
  INLINE ~PolynomialData();

  /**
   * Empty polynomial data.
   */
  INLINE PolynomialData();

  /**
   * Constructor. Creates an array of [n] coefficients.
   * @param n the order + 1  of the polynomial.
   */
  INLINE PolynomialData( uint n );

  /**
   * Initialization with an array of [n]+1 coefficients.
   * @param n the order + 1 of the polynomial.
   */
  INLINE void init( uint n );
  
  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  INLINE PolynomialData( const PolynomialData & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   */
  INLINE PolynomialData & operator=( const PolynomialData & other );

public:

  /**
   * Order of the polynomial + 1 == size of the array 'coefs'.
   */
  uint size;
  /**
   * Array to store the coefficients.
   */
  float* coefs;
  
  
};


///////////////////////////////////////////////////////////////////////////////
// class Polynomial
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'Polynomial' <p>
 * Aim: A class to represent a polynomial of fixed order.
 */
  class Polynomial : public R2RFunction
{

  // ------------------------- Static services --------------------------------
public:


  /**
   * A bell shape between 0 and 1 with border conditions.
   * Defined as: p(0)=0, p(0.5)=a, p(1)=0.
   * p'(0)=0, p'(1)=0, p"(0)=0, p"(1)=0.
   * @return the corresponding polynomial.
   */
  static Polynomial bellShape2( float a );
  


  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~Polynomial();

  /**
   * Constructor.
   * Creates the polynomial P(x)=0.
   */
  INLINE Polynomial();

  /**
   * Constructor.
   * @param n the order of the polynomial.
   */
  INLINE Polynomial( int n );

  /**
   * Constructor from data.
   * @param other the object to clone.
   */
  INLINE Polynomial( const PolynomialData & data );


  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE Polynomial( const Polynomial & other );


  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE Polynomial & operator=( const Polynomial & other );


  /**
   * @return the order of the polynomial.
   */
  INLINE int order() const;


  /**
   * Sets the [k]th coefficient to the value [a].
   * @param k the monomial order.
   * @param a any value.
   */
  INLINE void setCoef( uint k, float a );

  /**
   * @param k the monomial order.
   * @return the [k]th coefficient value.
   */
  INLINE float getCoef( uint k ) const;
  
  /**
   * Evaluates the polynomial at [x].
   * @param x any value.
   */
  float value( float x ) const;

  /**
   * @return the derivative polynomial.
   */
  Polynomial derivative() const;

  
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

  Shared<PolynomialData> m_data;

  
  // ------------------------- Hidden services --------------------------------
protected:
private:
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'Polynomial'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'Polynomial' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const Polynomial & that_object_to_display );


} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "Polynomial.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Polynomial_h

#undef Polynomial_RECURSES
#endif // else defined(Polynomial_RECURSES)
