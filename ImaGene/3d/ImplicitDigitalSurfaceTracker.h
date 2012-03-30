/** @file ImplicitDigitalSurfaceTracker.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImplicitDigitalSurfaceTracker.h
//
// Creation : 2005/01/18
//
// Version : 2005/01/18
//
// Author : JOL
//
// Summary : Header file for files ImplicitDigitalSurfaceTracker.ih and ImplicitDigitalSurfaceTracker.cxx
//
// History :
//	2005/01/18 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImplicitDigitalSurfaceTracker declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImplicitDigitalSurfaceTracker_RECURSES)
#error Recursive header files inclusion detected in ImplicitDigitalSurfaceTracker.h
#else // defined(ImplicitDigitalSurfaceTracker_RECURSES)
#define ImplicitDigitalSurfaceTracker_RECURSES

#if !defined ImplicitDigitalSurfaceTracker_h
#define ImplicitDigitalSurfaceTracker_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/DigitalSurfaceTracker.h"
#include "ImaGene/digitalnD/SurfelNeighborhood.h"
#include "ImaGene/3d/ImplicitDigitalSurface.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class ImplicitDigitalSurfaceTracker
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitDigitalSurfaceTracker' <p>
   * Aim: Implements a 'DigitalSurfaceTracker' on an 'ImplicitDigitalSurface'.
   * Uses a 'SurfelNeighborhood'.
   */
  class ImplicitDigitalSurfaceTracker : public DigitalSurfaceTracker
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    virtual ~ImplicitDigitalSurfaceTracker();

    /**
     * Constructor. The tracker is not valid.
     * see init
     */
    ImplicitDigitalSurfaceTracker();

    /**
     * Initializes the implicit digital surface tracker to move on the
     * implicit digital surface [ids].
     *
     * @param ids any implicit digital surface.
     */
    void init( const ImplicitDigitalSurface* ids );



    // ----------------------- digital surface services -----------------------
  public:

    /**
     * @return the digital surface on which the tracker is moving.
     */
    virtual const DigitalSurface* surface() const;

    /**
     * @return a clone of 'this'.
     */
    virtual DigitalSurfaceTracker* clone() const;

    /**
     * @return the current surfel at which the tracker is pointing.
     */
    virtual Kn_sid current() const;
    
    /**
     * @return the orthogonal direction to the current surfel.
     */
    virtual uint orthDir() const;
    
    /**
     * Moves the tracker to the surfel [s].
     * @param s any surfel on the surface.
     */
    virtual void move( Kn_sid s );

    /**
     * Computes the surfel adjacent to 'current()' in the direction
     * [dir] along orientation [pos]. The move code (0-3) is stored
     * for possible later use.
     *
     * @param dir any direction different from 'orthDir()'.
     * @param pos when 'true' look in positive direction along [track_dir] axis, 'false' look in negative direction.
     * @return the corresponding adjacent surfel or 0 if there was none.
     * @see lastCode
     */
    virtual Kn_sid adjacent( uint dir, bool pos );
    
    /**
     * @return the move code associated to the last call to
     * 'adjacent'. When '0', there was no adjacent surfel, when '=n'
     * (n=1..3), the adjacent surfel is the n-th interior surfel.
     * 
     * @see adjacent
     */
    virtual uint lastCode() const;
    
 

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
  private:


    /**
     * The implicit digital surface over which the tracker is moving.
     */
    const ImplicitDigitalSurface* m_ids;

    /**
     * A helper class to move on the boundary.
     */
    SurfelNeighborhood m_neighborhood;

    /**
     * The move code associated to the last call to 'adjacent'. When
     * '0', there was no adjacent surfel, when '=n' (n=1..3), the
     * adjacent surfel is the n-th interior surfel.
     */
    uint m_last_code;
    


    // ------------------------- Hidden services ------------------------------
  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImplicitDigitalSurfaceTracker( const ImplicitDigitalSurfaceTracker & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImplicitDigitalSurfaceTracker & operator=( const ImplicitDigitalSurfaceTracker & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'ImplicitDigitalSurfaceTracker'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ImplicitDigitalSurfaceTracker' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const ImplicitDigitalSurfaceTracker & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/ImplicitDigitalSurfaceTracker.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImplicitDigitalSurfaceTracker_h

#undef ImplicitDigitalSurfaceTracker_RECURSES
#endif // else defined(ImplicitDigitalSurfaceTracker_RECURSES)
