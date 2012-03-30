/** @file ImplicitSurfaceProjector.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImplicitSurfaceProjector.h
//
// Creation : 2008/03/31
//
// Version : 2008/03/31
//
// Author : JOL
//
// Summary : Header file for files ImplicitSurfaceProjector.ih and ImplicitSurfaceProjector.cxx
//
// History :
//	2008/03/31 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImplicitSurfaceProjector declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImplicitSurfaceProjector_RECURSES)
#error Recursive header files inclusion detected in ImplicitSurfaceProjector.h
#else // defined(ImplicitSurfaceProjector_RECURSES)
#define ImplicitSurfaceProjector_RECURSES

#if !defined ImplicitSurfaceProjector_h
#define ImplicitSurfaceProjector_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
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
  // class ImplicitSurfaceProjector
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitSurfaceProjector' <p> Aim: A class
   * to project a point onto the closest point which has zero value in
   * the associated implicit surface. By default, use Newton-Raphson
   * technique. May be overriden.
   */
  class ImplicitSurfaceProjector
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    virtual ~ImplicitSurfaceProjector();

    /**
     * Constructor.
     * The object is invalid.
     */
    INLINE ImplicitSurfaceProjector();

    /**
     * Initializes the object with the implicit surface and the
     * expected precision.
     *
     * @param dim the dimension of the domain.
     * @param is the implicit surface (the object is only referenced).
     * @param eps the required precision.
     * @param nmax the maximum number of iteration.
     */
    void init( uint dim, const ImplicitSurface & is, 
	       float eps, uint nmax = 20 );

    
    // ----------------------- Projection services ----------------------------
  public:

    /**
     * Project the point [xp] onto some zero-level of this implicit
     * surface [m_is] with precision [m_eps] (meaning '|m_is( xp )| <
     * m_eps' at the end of the process.
     *
     * @param xp (returns) a point close to f=0.
     * @param x0 initial point.
     *
     * @return 'true' if value is close to zero, 'false' if the
     * projection stopped because the number of alloted iterations has
     * been exceded.
     */
    virtual bool project( float* xp, 
			  const float* x0 ) const;

    

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
     * Dimension of the domain of the implicit function.
     */
    uint m_dim;

    /**
     * A pointer to a valid implicit surface object (not owned).
     */
    const ImplicitSurface* m_is;

    /**
     * Precision of the projection.
     */
    float m_eps;

    /**
     * The maximum number of iteration.
     */
    uint m_nmax;


    // ------------------------- Hidden services ------------------------------
  protected:

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImplicitSurfaceProjector( const ImplicitSurfaceProjector & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImplicitSurfaceProjector & operator=( const ImplicitSurfaceProjector & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'ImplicitSurfaceProjector'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ImplicitSurfaceProjector' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const ImplicitSurfaceProjector & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/ImplicitSurfaceProjector.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImplicitSurfaceProjector_h

#undef ImplicitSurfaceProjector_RECURSES
#endif // else defined(ImplicitSurfaceProjector_RECURSES)
