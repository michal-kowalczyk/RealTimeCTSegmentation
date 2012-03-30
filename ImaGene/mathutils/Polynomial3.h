/** @file Polynomial3.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Polynomial3.h
//
// Creation : 2008/04/04
//
// Version : 2008/04/04
//
// Author : JOL
//
// Summary : Header file for files Polynomial3.ih and Polynomial3.cxx
//
// History :
//	2008/04/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Polynomial3 declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Polynomial3_RECURSES)
#error Recursive header files inclusion detected in Polynomial3.h
#else // defined(Polynomial3_RECURSES)
#define Polynomial3_RECURSES

#if !defined Polynomial3_h
#define Polynomial3_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <list>
#include <cmath>
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class Monomial3
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Monomial3' <p>
   * Aim: Represents a monomial of arbitrary degree with three variables.
   */
  class Monomial3
  {
  public:
    /**
     * degrees of the monomial.
     */
    uint deg[ 3 ];
    
    /**
     * constructor.
     */
    INLINE Monomial3();

    /**
     * Constructor.
     * Example: for "x*y^2*z^4", construct with (1,2,4).
     * 
     * @param dx the degree of x.
     * @param dy the degree of y.
     * @param dz the degree of z.
     */
    INLINE Monomial3( uint dx, uint dy, uint dz );

    /**
     * destructor.
     */
    INLINE ~Monomial3();
    
    /**
     * copy constructor.
     * @param other the object to clone.
     */
    INLINE Monomial3( const Monomial3 & other );

    /**
     * assignment.
     * @param other the object to clone.
     */
    INLINE Monomial3& operator=( const Monomial3 & other );

    /**
     * Evaluates the monomial at point xyz.
     * 
     * @param xyz the point at which the monomial is evaluate
     */
    template <typename Number>
    INLINE
    Number operator()( const Number* xyz ) const;
    
    /**
     * Differentiate the monomial along x(0), y(1) or z(2).
     * 
     * @param variable the differentiation variable.
     * @return the integer coefficient in front of the obtained monomial.
     */
    INLINE
    uint diff( uint variable );

    /**
     * lexicographic order on monomials.
     * @param other the other monomial to compare.
     * @return 'true' if 'this' precedes [other] in the lexicographic sense.
     */
    INLINE bool operator<( const Monomial3 & other ) const;
    
  };



  /////////////////////////////////////////////////////////////////////////////
  // class Polynomial3
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Polynomial3' <p> Aim: Represents a
   * polynomial of arbitrary degree with three variables.  @todo This
   * class is a sketch. It should be completed with all operations on
   * polynomials.
   */
  template <typename Number>
  class Polynomial3
  {

    // ----------------------- Standard services ------------------------------
  public:
    /**
     * A term in a polynomial expression is a pair (monomial,number).
     */
    typedef std::pair<Monomial3,Number> term;

    /**
     * Destructor. 
     */
    INLINE ~Polynomial3();

    /**
     * Constructor.
     */
    INLINE Polynomial3();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    INLINE Polynomial3( const Polynomial3<Number> & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    INLINE Polynomial3<Number> & 
    operator=( const Polynomial3<Number> & other );

    /**
     * Adds the given monomial to 'this'.
     * @param m the monomial.
     * @param c the coefficient is front of the monomial.
     */
    INLINE
    void add( const Monomial3 & m, Number c );

    /**
     * Lexicographic ordering on terms.
     * @param t1 the first term
     * @param t2 the second term
     * @return 'true' if [t1] precedes [t2] in the lexicographic sense.
     */
    INLINE 
    bool operator()( const term & t1, const term & t2 ) const;


    /**
     * Evaluates the polynomial at point xyz.
     * 
     * @param xyz the point at which the monomial is evaluate
     *
     * @todo At the moment, recomputes x^i,y^i,z^i for each
     * monomial. Should store results.
     */
    INLINE
    Number operator()( const Number* xyz ) const;

    /**
     * Differentiate the polynomial along x(0), y(1) or z(2).
     * 
     * @param variable the differentiation variable.
     */
    INLINE
    void diff( uint variable );
    

    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * List of terms.
     */
    std::list<term> m_terms;
    

    // ------------------------- Hidden services ------------------------------
  protected:

  
    // ------------------------- Internals ------------------------------------
  private:
  
  };


  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/Polynomial3.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Polynomial3_h

#undef Polynomial3_RECURSES
#endif // else defined(Polynomial3_RECURSES)
