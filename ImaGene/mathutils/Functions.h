/** @file Functions.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Functions.h
//
// Creation : 2004/09/10
//
// Version : 2004/09/10
//
// Author : JOL
//
// Summary : Header file for files Functions.ih and Functions.cxx
//
// History :
//	2004/09/10 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Functions declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Functions_RECURSES)
#error Recursive header files inclusion detected in Functions.h
#else // defined(Functions_RECURSES)
#define Functions_RECURSES

#if !defined Functions_h
#define Functions_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene { 

  /////////////////////////////////////////////////////////////////////////////
  // class R2RFunction
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'R2RFunction' <p>
   * Aim: An interface defining a real-valued function.
   */
  class R2RFunction
  {
    // ------------------------- Standard services ----------------------------
  public:
    /**
     * Destructor. 
     */
    INLINE virtual ~R2RFunction();
    
    // ------------------------- function services ----------------------------
  public:
    
    /**
     * Evaluates the function at [x].
     * @param x any value in the range.
     * @return the value of the function at [x].
     */
    virtual float value( float x ) const = 0;
    
    /**
     * Operator for evaluating the function at [x]. Calls the abstract
     * service 'value'.
     *
     * @param x any value.
     * @return the value of the function at [x].
     * @see value
     */
    INLINE float operator()( float x ) const;
        
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // class TriangleFunction
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'TriangleFunction' <p>
   * Aim: A class to describe a triangle function.
   */
  class TriangleFunction : public R2RFunction
  {
    // ------------------------- Standard services ----------------------------
  public:
    /**
     * Destructor. 
     */
    INLINE ~TriangleFunction();

    /**
     * Constructor. 
     */
    INLINE TriangleFunction();
    
    // ------------------------- function services ----------------------------
  public:
    
    /**
     * Evaluates the function at [x].
     * @param x any value in the range.
     * @return the value of the function at [x].
     */
    INLINE float value( float x ) const;
    
  };


  /////////////////////////////////////////////////////////////////////////////
  // class DTriangleFunction
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'DTriangleFunction' <p>
   * Aim: A class to describe the drivative of the triangle function.
   */
  class DTriangleFunction : public R2RFunction
  {
    // ------------------------- Standard services ----------------------------
  public:
    /**
     * Destructor. 
     */
    INLINE ~DTriangleFunction();

    /**
     * Constructor. 
     */
    INLINE DTriangleFunction();
    
    // ------------------------- function services ----------------------------
  public:
    
    /**
     * Evaluates the function at [x].
     * @param x any value in the range.
     * @return the value of the function at [x].
     */
    INLINE float value( float x ) const;
    
  };
  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "Functions.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Functions_h

#undef Functions_RECURSES
#endif // else defined(Functions_RECURSES)
