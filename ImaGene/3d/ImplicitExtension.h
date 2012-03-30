/** @file ImplicitExtension.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImplicitExtension.h
//
// Creation : 2008/02/27
//
// Version : 2008/02/27
//
// Author : JOL
//
// Summary : Header file for files ImplicitExtension.ih and ImplicitExtension.cxx
//
// History :
//	2008/02/27 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImplicitExtension declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImplicitExtension_RECURSES)
#error Recursive header files inclusion detected in ImplicitExtension.h
#else // defined(ImplicitExtension_RECURSES)
#define ImplicitExtension_RECURSES

#if !defined ImplicitExtension_h
#define ImplicitExtension_h

//////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <iostream>
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/3d/ImplicitSurface.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class ImplicitExtension
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitExtension' <p> Aim: an abstract class to
   * represent an n+1-d scalar field F defined from an n-d scalar field
   * 'f(x,y,...)'. F is defined as 'F(x,y,...,t) = some function
   * mixing f and t'.
   *
   * This class is mainly used to cast the implicit surface 'f=0'
   * (which may have singularities) as a nice parameterized surface
   * t=g o f in higher dimension (without singularities).
   */
  class ImplicitExtension : public ImplicitSurface
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    virtual ~ImplicitExtension();

    /**
     * Constructor. 
     */
    ImplicitExtension();

    /**
     * Initializes the object by connecting it to an existing implicit
     * surface.
     *
     * @param is the implicit surface to cast into a higher dimensional space.
     * @param d the dimension of the implicit surface [is]
     */
    void init( ImplicitSurface* is, uint d );

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return the dimension of mapping domain (0 is any).
     * @see m_d
     */
    INLINE virtual uint dimIn() const;

    // ----------------------- overridable services ---------------------------
  public:

    /**
     * @return a clone of 'this'.
     */
    virtual ImplicitSurface* clone() const = 0;

    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    virtual float operator()( const float xyz[] ) const = 0;


    /**
     * Evaluates the gradient of the function at coordinates
     * [xyz]. The surface lies at isovalue 0. By convention, negative
     * is outside and positive is inside.
     *
     * @param xyz an array of coordinate.
     * @param grad (returns) the gradient vector.
     */
    virtual void gradient( const float xyz[], float grad[] ) const = 0;


    // ----------------------- Interface --------------------------------------
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
  

    // ------------------------- Datas ----------------------------------------
  protected:

    /**
     * The referenced implicit surface.
     */
    ImplicitSurface* m_is;

    /**
     * The dimension of the referenced implicit surface [m_is].
     */
    uint m_d;

    // ------------------------- Hidden services ------------------------------
  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImplicitExtension( const ImplicitExtension & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImplicitExtension & operator=( const ImplicitExtension & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };


  /////////////////////////////////////////////////////////////////////////////
  // class StandardExtension
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'StandardExtension' : represents the
   * n+1-scalar field 'F(x,y,z,t)=f(x,y,z)-t'.
   */
  class StandardExtension : public ImplicitExtension
  {
  public:
    /**
     * Destructor. 
     */
    INLINE virtual ~StandardExtension();

    /**
     * Constructor. 
     */
    INLINE StandardExtension();

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return a clone of 'this'.
     */
    INLINE virtual ImplicitSurface* clone() const;

    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    INLINE virtual float operator()( const float xyz[] ) const;


    /**
     * Evaluates the gradient of the function at coordinates
     * [xyz]. The surface lies at isovalue 0. By convention, negative
     * is outside and positive is inside.
     *
     * @param xyz an array of coordinate.
     * @param grad (returns) the gradient vector.
     */
    INLINE virtual void gradient( const float xyz[], float grad[] ) const;

  };


  /////////////////////////////////////////////////////////////////////////////
  // class GradientNormalizedExtension
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'GradientNormalizedExtension' : represents the
   * n+1-scalar field 'F(x,y,z,t)=f(x,y,z)/|grad_f(x,y,z)|-t'.
   */
  class GradientNormalizedExtension : public ImplicitExtension
  {
  public:
    /**
     * Destructor. 
     */
    INLINE virtual ~GradientNormalizedExtension();

    /**
     * Constructor. 
     */
    INLINE GradientNormalizedExtension();

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return a clone of 'this'.
     */
    INLINE virtual ImplicitSurface* clone() const;

    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    INLINE virtual float operator()( const float xyz[] ) const;


    /**
     * Evaluates the gradient of the function at coordinates
     * [xyz]. The surface lies at isovalue 0. By convention, negative
     * is outside and positive is inside.
     *
     * @param xyz an array of coordinate.
     * @param grad (returns) the gradient vector.
     */
    INLINE virtual void gradient( const float xyz[], float grad[] ) const;

    // ------------------------- Datas ----------------------------------------
  protected:


  };


  /////////////////////////////////////////////////////////////////////////////
  // class GradientProductExtension
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'GradientProductExtension' : represents the
   * n+1-scalar field 'F(x,y,z,t)=f(x,y,z)-t|grad_f(x,y,z)|'.
   */
  class GradientProductExtension : public ImplicitExtension
  {
  public:
    /**
     * Destructor. 
     */
    INLINE virtual ~GradientProductExtension();

    /**
     * Constructor. 
     */
    INLINE GradientProductExtension();

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return a clone of 'this'.
     */
    INLINE virtual ImplicitSurface* clone() const;

    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    INLINE virtual float operator()( const float xyz[] ) const;


    /**
     * Evaluates the gradient of the function at coordinates
     * [xyz]. The surface lies at isovalue 0. By convention, negative
     * is outside and positive is inside.
     *
     * @param xyz an array of coordinate.
     * @param grad (returns) the gradient vector.
     */
    INLINE virtual void gradient( const float xyz[], float grad[] ) const;

    // ------------------------- Datas ----------------------------------------
  protected:


  };


  /////////////////////////////////////////////////////////////////////////////
  // class MaxGradientProductExtension
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'MaxGradientProductExtension' : represents the
   * n+1-scalar field 'F(x,y,z,t)=f(x,y,z)-t|grad_f(x,y,z)|_\infty'.
   */
  class MaxGradientProductExtension : public ImplicitExtension
  {
  public:
    /**
     * Destructor. 
     */
    INLINE virtual ~MaxGradientProductExtension();

    /**
     * Constructor. 
     */
    INLINE MaxGradientProductExtension();

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return a clone of 'this'.
     */
    INLINE virtual ImplicitSurface* clone() const;

    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    INLINE virtual float operator()( const float xyz[] ) const;


    /**
     * Evaluates the gradient of the function at coordinates
     * [xyz]. The surface lies at isovalue 0. By convention, negative
     * is outside and positive is inside.
     *
     * @param xyz an array of coordinate.
     * @param grad (returns) the gradient vector.
     */
    INLINE virtual void gradient( const float xyz[], float grad[] ) const;

    // ------------------------- Datas ----------------------------------------
  protected:


  };


  /////////////////////////////////////////////////////////////////////////////
  // class GradientFactorExtension
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'GradientFactorExtension' : represents the
   * n+1-scalar field 'F(x,y,z,t)=a*(f(x,y,z)-t|grad_f(x,y,z)|)'.
   */
  class GradientFactorExtension : public ImplicitExtension
  {
  public:
    /**
     * Destructor. 
     */
    INLINE virtual ~GradientFactorExtension();

    /**
     * Constructor. 
     */
    INLINE GradientFactorExtension( float a );

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return a clone of 'this'.
     */
    INLINE virtual ImplicitSurface* clone() const;

    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    INLINE virtual float operator()( const float xyz[] ) const;


    /**
     * Evaluates the gradient of the function at coordinates
     * [xyz]. The surface lies at isovalue 0. By convention, negative
     * is outside and positive is inside.
     *
     * @param xyz an array of coordinate.
     * @param grad (returns) the gradient vector.
     */
    INLINE virtual void gradient( const float xyz[], float grad[] ) const;

    // ------------------------- Datas ----------------------------------------
  protected:
    /**
     * Coefficient factor.
     */
    float m_a;

  };


  /**
   * Overloads 'operator<<' for displaying objects of class 'ImplicitExtension'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ImplicitExtension' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const ImplicitExtension & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/ImplicitExtension.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImplicitExtension_h

#undef ImplicitExtension_RECURSES
#endif // else defined(ImplicitExtension_RECURSES)
