/** @file ImplicitSurfaces.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImplicitSurfaces.h
//
// Creation : 2005/01/19
//
// Version : 2005/01/19
//
// Author : JOL
//
// Summary : Header file for files ImplicitSurfaces.ih and ImplicitSurfaces.cxx
//
// History :
//	2005/01/19 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImplicitSurfaces declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImplicitSurfaces_RECURSES)
#error Recursive header files inclusion detected in ImplicitSurfaces.h
#else // defined(ImplicitSurfaces_RECURSES)
#define ImplicitSurfaces_RECURSES

#if !defined ImplicitSurfaces_h
#define ImplicitSurfaces_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
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
  // class ImplicitSphere
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitSphere' <p>
   * Aim: Represents implicitly a sphere. Implements 'ImplicitSurface' 
   */
  class ImplicitSphere : public ImplicitSurface
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    virtual ~ImplicitSphere();

    /**
     * Constructor.
     *
     * @param n the dimension of the sphere.
     * @param x0 the coordinates of the sphere center.
     * @param r the radius of the sphere.
     */
    ImplicitSphere( uint n, const float x0[], float r );


    /**
     * @return a clone of 'this'.
     */
    virtual ImplicitSurface* clone() const;

    
    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    virtual float operator()( const float xyz[] ) const;


    /**
     * Evaluates the gradient of the function at coordinates
     * [xyz]. The surface lies at isovalue 0. By convention, negative
     * is outside and positive is inside.
     *
     * @param xyz an array of coordinate.
     * @param grad (returns) the gradient vector.
     */
    virtual void gradient( const float xyz[], float grad[] ) const;


    /**
     * Returns the bounds of the implicit surface (isovalue 0), if known.
     * @param lower the lower bounds, when finite.
     * @param has_lower the type for lower bounds.
     * @param upper the upper bounds, when finite.
     * @param has_upper the type for upper bounds.
     * @return 'true' if every bound type is finite, 'false' otherwise.
     */
    virtual bool bounds( float lower[], BoundType has_lower[],
			 float upper[], BoundType has_upper[] ) const;

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return the dimension of mapping domain (0 is any).
     * @see m_n
     */
    INLINE virtual uint dimIn() const;


    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * The dimension of the sphere.
     */
    uint m_n;
    
    /**
     * The coordinates of the center of the sphere.
     */
    float* m_x0;
    
    /**
     * The radius of the sphere.
     */
    float m_r;
    

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE ImplicitSphere();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImplicitSphere( const ImplicitSphere & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImplicitSphere & operator=( const ImplicitSphere & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };



  /////////////////////////////////////////////////////////////////////////////
  // class ImplicitTorus3D
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitTorus3D' <p>
   * Aim: Represents implicitly a 3d torus. Implements 'ImplicitSurface' 
   */
  class ImplicitTorus3D : public ImplicitSurface
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~ImplicitTorus3D();

    /**
     * Constructor. Specifies a torus of given center [center],
     * symmetric around the z-axis.
     * 
     * @param center the center of the torus.
     * @param big_radius the big radius of the torus.
     * @param small_radius the small radius of the torus.
     */
    ImplicitTorus3D( const float center[],
		     float big_radius, float small_radius );

    /**
     * @return a clone of 'this'.
     */
    virtual ImplicitSurface* clone() const;

    
    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    virtual float operator()( const float xyz[] ) const;


    /**
     * Evaluates the gradient of the function at coordinates
     * [xyz]. The surface lies at isovalue 0. By convention, negative
     * is outside and positive is inside.
     *
     * @param xyz an array of coordinate.
     * @param grad (returns) the gradient vector.
     */
    virtual void gradient( const float xyz[], float grad[] ) const;


    /**
     * Returns the bounds of the implicit surface (isovalue 0), if known.
     * @param lower the lower bounds, when finite.
     * @param has_lower the type for lower bounds.
     * @param upper the upper bounds, when finite.
     * @param has_upper the type for upper bounds.
     * @return 'true' if every bound type is finite, 'false' otherwise.
     */
    virtual bool bounds( float lower[], BoundType has_lower[],
			 float upper[], BoundType has_upper[] ) const;


    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return the dimension of mapping domain (0 is any).
     */
    INLINE virtual uint dimIn() const;


    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * The center of the torus.
     */
    float m_center[ 3 ];
    
    /**
     * The big radius of the torus.
     */
    float m_br;
    
    /**
     * The small radius of the torus.
     */
    float m_sr;
    

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE ImplicitTorus3D();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImplicitTorus3D( const ImplicitTorus3D & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImplicitTorus3D & operator=( const ImplicitTorus3D & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };




  /////////////////////////////////////////////////////////////////////////////
  // class ImplicitSurfaceUnion
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitSurfaceUnion' <p> Aim: Represents
   * implicitly the union of two implicit surfaces as a
   * min. Implements 'ImplicitSurface'.
   */
  class ImplicitSurfaceUnion : public ImplicitSurface
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~ImplicitSurfaceUnion();

    /**
     * Constructor.
     *
     * @param is1 any implicit surface (cloned).
     * @param is2 any implicit surface (cloned).
     */
    ImplicitSurfaceUnion( const ImplicitSurface & is1,
			  const ImplicitSurface & is2 );
    
    /**
     * @return a clone of 'this'.
     */
    virtual ImplicitSurface* clone() const;

    
    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    virtual float operator()( const float xyz[] ) const;


    /**
     * Evaluates the gradient of the function at coordinates
     * [xyz]. The surface lies at isovalue 0. By convention, negative
     * is outside and positive is inside.
     *
     * @param xyz an array of coordinate.
     * @param grad (returns) the gradient vector.
     */
    virtual void gradient( const float xyz[], float grad[] ) const;



    /**
     * Returns the bounds of the implicit surface (isovalue 0), if known.
     * @param lower the lower bounds, when finite.
     * @param has_lower the type for lower bounds.
     * @param upper the upper bounds, when finite.
     * @param has_upper the type for upper bounds.
     * @return 'true' if every bound type is finite, 'false' otherwise.
     *
     * @todo NOT DONE
     */
    virtual bool bounds( float lower[], BoundType has_lower[],
			 float upper[], BoundType has_upper[] ) const;

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return the dimension of mapping domain (0 is any).
     */
    INLINE virtual uint dimIn() const;



    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * The first implicit surface.
     */
    ImplicitSurface* m_is1;

    /**
     * The second implicit surface.
     */
    ImplicitSurface* m_is2;
    


    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE ImplicitSurfaceUnion();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImplicitSurfaceUnion( const ImplicitSurfaceUnion & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImplicitSurfaceUnion & operator=( const ImplicitSurfaceUnion & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };


  /////////////////////////////////////////////////////////////////////////////
  // class ImplicitSurfaceModel
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitSurfaceModel' <p>
   * Aim: Represents implicitly a 3d sphere. Implements 'ImplicitSurface' 
   */
  class ImplicitSurfaceModel : public ImplicitSurface
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~ImplicitSurfaceModel();


    /**
     * @return a clone of 'this'.
     */
    virtual ImplicitSurface* clone() const;

    
    /**
     * Evaluates the function at coordinates [xyz]. The surface lies
     * at isovalue 0. By convention, negative is outside and positive
     * is inside.
     *
     * @param xyz an array of coordinate.
     * @return a real value.
     */
    virtual float operator()( const float xyz[] ) const;


    /**
     * Returns the bounds of the implicit surface (isovalue 0), if known.
     * @param lower the lower bounds, when finite.
     * @param has_lower the type for lower bounds.
     * @param upper the upper bounds, when finite.
     * @param has_upper the type for upper bounds.
     * @return 'true' if every bound type is finite, 'false' otherwise.
     */
    virtual bool bounds( float lower[], BoundType has_lower[],
			 float upper[], BoundType has_upper[] ) const;

    // ----------------------- implemented services ---------------------------
  public:

    /**
     * @return the dimension of mapping domain (0 is any).
     * @see m_d
     */
    INLINE virtual uint dimIn() const;



    // ------------------------- Datas ----------------------------------------
  private:


    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE ImplicitSurfaceModel();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImplicitSurfaceModel( const ImplicitSurfaceModel & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImplicitSurfaceModel & operator=( const ImplicitSurfaceModel & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/ImplicitSurfaces.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImplicitSurfaces_h

#undef ImplicitSurfaces_RECURSES
#endif // else defined(ImplicitSurfaces_RECURSES)
