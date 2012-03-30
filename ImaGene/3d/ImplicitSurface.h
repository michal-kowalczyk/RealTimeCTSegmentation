/** @file ImplicitSurface.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImplicitSurface.h
//
// Creation : 2005/01/17
//
// Version : 2005/01/17
//
// Author : JOL
//
// Summary : Header file for files ImplicitSurface.ih and ImplicitSurface.cxx
//
// History :
//	2005/01/17 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImplicitSurface declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImplicitSurface_RECURSES)
#error Recursive header files inclusion detected in ImplicitSurface.h
#else // defined(ImplicitSurface_RECURSES)
#define ImplicitSurface_RECURSES

#if !defined ImplicitSurface_h
#define ImplicitSurface_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/base/RnMaps.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class ImplicitSurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitSurface' <p>
   * Aim: an interface to describe a n-d implicit surface.
   */
  class ImplicitSurface : public RnMapImp
  {

    // ----------------------- public types ------------------------------
  public:

    /**
     * Classifies the different kinds of bounds.
     */
    enum BoundType { 
      /**
       * The bound is known and finite.
       */
      Finite,
      /**
       * The bound is known and infinite.
       */
      Infinite,
      /**
       * The bound is unknown.
       */
      Unknown 
    };
    
    

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    virtual ~ImplicitSurface();



    // ----------------------- abstract services ------------------------------
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


   // ---------------- Implementation of RnMapImp services --------------------
  public:

    /**
     * @return the dimension of mapping range (1).
     */
    INLINE virtual uint dimOut() const;
        
    /**
     * Abstract service. Mapping operator.
     * @param x the in-vector
     * @return the out vector.
     */
    INLINE virtual Vector eval( const Vector & x ) const;


    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param that_stream the output stream where the object is written.
     */
    virtual void selfDisplay( std::ostream & that_stream ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    virtual bool OK() const;
  

    // ------------------------- Datas ----------------------------------------
  private:


    // ------------------------- Hidden services ------------------------------
  protected:

  private:

    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'ImplicitSurface'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ImplicitSurface' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const ImplicitSurface & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/ImplicitSurface.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImplicitSurface_h

#undef ImplicitSurface_RECURSES
#endif // else defined(ImplicitSurface_RECURSES)
