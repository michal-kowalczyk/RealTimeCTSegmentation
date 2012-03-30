/** @file ImplicitSurfaces3D.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImplicitSurfaces3D.h
//
// Creation : 2008/03/20
//
// Version : 2008/03/20
//
// Author : JOL
//
// Summary : Header file for files ImplicitSurfaces3D.ih and ImplicitSurfaces3D.cxx
//
// History :
//	2008/03/20 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImplicitSurfaces3D declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImplicitSurfaces3D_RECURSES)
#error Recursive header files inclusion detected in ImplicitSurfaces3D.h
#else // defined(ImplicitSurfaces3D_RECURSES)
#define ImplicitSurfaces3D_RECURSES

#if !defined ImplicitSurfaces3D_h
#define ImplicitSurfaces3D_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/mathutils/Polynomial3.h"
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
  // class ImplicitSurface3D
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of abstract class 'ImplicitSurface3D' <p> Aim: Represents some
   * function f(x,y,z) with range in R.
   */
  class ImplicitSurface3D : public ImplicitSurface
  {
  public:
    /**
     * Clone services.
     *
     * @return a dynamically allocated pointer on a clone of 'this'.
     */
    virtual ImplicitSurface3D* clone() const = 0;

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return the dimension of mapping domain (3).
     */
    INLINE virtual uint dimIn() const;

  };
  
  /////////////////////////////////////////////////////////////////////////////
  // class WhitneyUmbrella
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'WhitneyUmbrella' <p> Aim: Represents the
   * function f(x,y,z), where f(x,y,z)=0 is the Whitney
   * umbrella.
   */
  class WhitneyUmbrella : public ImplicitSurface3D
  {
  public:
    /**
     * Constructor.
     */
    INLINE WhitneyUmbrella();

    /**
     * Destructor.
     */
    INLINE virtual ~WhitneyUmbrella();

    /**
     * Clone services.
     *
     * @return a dynamically allocated pointer on a clone of 'this'.
     */
    INLINE virtual ImplicitSurface3D* clone() const;

    /**
     * Evaluates the implicit function at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @return the value f(x,y,z).
     */
    INLINE virtual float operator()( const float xyz[] ) const;

    /**
     * Evaluates the gradient at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @param grad (returns) a 3d vector containing the gradient of
     * f(x,y,z).
     */
     INLINE virtual void gradient( const float xyz[], float grad[] ) const;
 
  };



  /////////////////////////////////////////////////////////////////////////////
  // class FourLinesSurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'FourLinesSurface' <p> Aim: Represents
   * the function f(x,y,z), where f(x,y,z)=0 is the four
   * lines crossing at 0, one line turning around (x=y=0) along z.
   */
  class FourLinesSurface : public ImplicitSurface3D
  {
  public:
    /**
     * Constructor.
     */
    INLINE FourLinesSurface();
    /**
     * Destructor.
     */
    INLINE virtual ~FourLinesSurface();

    /**
     * Clone services.
     *
     * @return a dynamically allocated pointer on a clone of 'this'.
     */
    INLINE virtual ImplicitSurface3D* clone() const;

    /**
     * Evaluates the implicit function at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @return the value f(x,y,z).
     */
    INLINE virtual float operator()( const float xyz[] ) const;

    /**
     * Evaluates the gradient at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @param grad (returns) a 3d vector containing the gradient of
     * f(x,y,z).
     */
     INLINE virtual void gradient( const float xyz[], float grad[] ) const;
 
  };



  /////////////////////////////////////////////////////////////////////////////
  // class ConeSurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ConeSurface' <p> Aim: Represents
   * the function f(x,y,z), where f(x,y,z)=0 is the cone
   * with singular point at origin.
   */
  class ConeSurface : public ImplicitSurface3D
  {
  public:
    /**
     * Constructor.
     */
    INLINE ConeSurface();
    /**
     * Destructor.
     */
    INLINE virtual ~ConeSurface();

    /**
     * Clone services.
     *
     * @return a dynamically allocated pointer on a clone of 'this'.
     */
    INLINE virtual ImplicitSurface3D* clone() const;

    /**
     * Evaluates the implicit function at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @return the value f(x,y,z).
     */
    INLINE virtual float operator()( const float xyz[] ) const;

    /**
     * Evaluates the gradient at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @param grad (returns) a 3d vector containing the gradient of
     * f(x,y,z).
     */
     INLINE virtual void gradient( const float xyz[], float grad[] ) const;
 
  };


  /////////////////////////////////////////////////////////////////////////////
  // class SimonUmbrella
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'SimonUmbrella' <p> Aim: Represents the
   * function f(x,y,z), where f(x,y,z)=0 is a variant of
   * the Whitney umbrella, where sagital slices are parabolas.
   */
  class SimonUmbrella : public ImplicitSurface3D
  {
  public:
    /**
     * Constructor.
     */
    INLINE SimonUmbrella();
    /**
     * Destructor.
     */
    INLINE virtual ~SimonUmbrella();

    /**
     * Clone services.
     *
     * @return a dynamically allocated pointer on a clone of 'this'.
     */
    INLINE virtual ImplicitSurface3D* clone() const;

    /**
     * Evaluates the implicit function at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @return the value f(x,y,z).
     */
    INLINE virtual float operator()( const float xyz[] ) const;

    /**
     * Evaluates the gradient at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @param grad (returns) a 3d vector containing the gradient of
     * f(x,y,z).
     */
     INLINE virtual void gradient( const float xyz[], float grad[] ) const;
 
  };




  /////////////////////////////////////////////////////////////////////////////
  // class FunnySurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'FunnySurface' <p> Aim: Represents the
   * function f(x,y,z), where f(x,y,z)=0 is some mix of
   * hyperbolic paraboloid with trigonometric
   * variation. f(x,y,z)=(xy-cos(z))*(x^2+y^2-1-z).
   */
  class FunnySurface : public ImplicitSurface3D
  {
  public:
    /**
     * Constructor.
     */
    INLINE FunnySurface();
    /**
     * Destructor.
     */
    INLINE virtual ~FunnySurface();

    /**
     * Clone services.
     *
     * @return a dynamically allocated pointer on a clone of 'this'.
     */
    INLINE virtual ImplicitSurface3D* clone() const;

    /**
     * Evaluates the implicit function at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @return the value f(x,y,z).
     */
    INLINE virtual float operator()( const float xyz[] ) const;

    /**
     * Evaluates the gradient at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @param grad (returns) a 3d vector containing the gradient of
     * f(x,y,z).
     */
     INLINE virtual void gradient( const float xyz[], float grad[] ) const;
 
  };


  /////////////////////////////////////////////////////////////////////////////
  // class PaunescuSurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'PaunescuSurface' <p> Aim: Represents the
   * function f(x,y,z), where f(x,y,z)=0 is some mix of
   * hyperbolic paraboloid with trigonometric
   * variation. f(x,y,z)=(xy-cos(z))*(x^2+y^2-1-z).
   */
  class PaunescuSurface : public ImplicitSurface3D
  {
  public:
    /**
     * Constructor.
     */
    INLINE PaunescuSurface();
    /**
     * Destructor.
     */
    INLINE virtual ~PaunescuSurface();

    /**
     * Clone services.
     *
     * @return a dynamically allocated pointer on a clone of 'this'.
     */
    INLINE virtual ImplicitSurface3D* clone() const;

    /**
     * Evaluates the implicit function at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @return the value f(x,y,z).
     */
    INLINE virtual float operator()( const float xyz[] ) const;

    /**
     * Evaluates the gradient at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @param grad (returns) a 3d vector containing the gradient of
     * f(x,y,z).
     */
     INLINE virtual void gradient( const float xyz[], float grad[] ) const;
 
  };


  /////////////////////////////////////////////////////////////////////////////
  // class CayleyCubicSurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CayleyCubicSurface' <p> Aim: Represents
   * the function f(x,y,z), where f(x,y,z)=0 is some
   * polynomial. f(x,y,z)=4(x^2 + y^2 + z^2) + 16 xyz - 1
   */
  class CayleyCubicSurface : public ImplicitSurface3D
  {
  public:
    /**
     * Constructor.
     */
    INLINE CayleyCubicSurface();
    /**
     * Destructor.
     */
    INLINE virtual ~CayleyCubicSurface();

    /**
     * Clone services.
     *
     * @return a dynamically allocated pointer on a clone of 'this'.
     */
    INLINE virtual ImplicitSurface3D* clone() const;

    /**
     * Evaluates the implicit function at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @return the value f(x,y,z).
     */
    INLINE virtual float operator()( const float xyz[] ) const;

    /**
     * Evaluates the gradient at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @param grad (returns) a 3d vector containing the gradient of
     * f(x,y,z).
     */
     INLINE virtual void gradient( const float xyz[], float grad[] ) const;
 
  };

  /////////////////////////////////////////////////////////////////////////////
  // class PolynomialSurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'PolynomialSurface' <p> Aim: Represents
   * the function f(x,y,z), where f(x,y,z)=0 is some
   * three-variable polynomial.
   */
  class PolynomialSurface : public ImplicitSurface3D
  {
  public:
    /**
     * Constructor.
     *
     * @param p any three-variable polynomial.
     */
    INLINE PolynomialSurface( const Polynomial3<float> & p );

    /**
     * Destructor.
     */
    INLINE virtual ~PolynomialSurface();

    /**
     * Clone services.
     *
     * @return a dynamically allocated pointer on a clone of 'this'.
     */
    INLINE virtual ImplicitSurface3D* clone() const;

    /**
     * Evaluates the implicit function at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @return the value f(x,y,z).
     */
    INLINE virtual float operator()( const float xyz[] ) const;

    /**
     * Evaluates the gradient at point [xyz].
     * 
     * @param xyz a 3d point.
     *
     * @param grad (returns) a 3d vector containing the gradient of
     * f(x,y,z).
     */
    INLINE virtual void gradient( const float xyz[], float grad[] ) const;
    
  private:
    Polynomial3<float> m_p;
    Polynomial3<float> m_px;
    Polynomial3<float> m_py;
    Polynomial3<float> m_pz;
  };


  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/ImplicitSurfaces3D.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImplicitSurfaces3D_h

#undef ImplicitSurfaces3D_RECURSES
#endif // else defined(ImplicitSurfaces3D_RECURSES)
