/** @file ExplicitDigitalSurfaceTracker.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ExplicitDigitalSurfaceTracker.h
//
// Creation : 2011/05/04
//
// Version : 2011/05/04
//
// Author : JOL
//
// Summary : Header file for files ExplicitDigitalSurfaceTracker.ih and ExplicitDigitalSurfaceTracker.cxx
//
// History :
//	2011/05/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ExplicitDigitalSurfaceTracker declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ExplicitDigitalSurfaceTracker_RECURSES)
#error Recursive header files inclusion detected in ExplicitDigitalSurfaceTracker.h
#else // defined(ExplicitDigitalSurfaceTracker_RECURSES)
#define ExplicitDigitalSurfaceTracker_RECURSES

#if !defined ExplicitDigitalSurfaceTracker_h
#define ExplicitDigitalSurfaceTracker_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/DigitalSurfaceTracker.h"
#include "ImaGene/digitalnD/ExplicitDigitalSurface.h"
#include "ImaGene/digitalnD/SurfelNeighborhood.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class ExplicitDigitalSurfaceTracker
  /////////////////////////////////////////////////////////////////////////////
  /** 
      Description of class 'ExplicitDigitalSurfaceTracker' <p> Aim:
      Implements the interface 'DigitalSurfaceTracker' for explicit
      digital surfaces (ie. a set of signed surfels). It is based on
      the services of a 'SurfelNeighborhood'.
      
      @see ExplicitDigitalSurface
   */
  class ExplicitDigitalSurfaceTracker : public DigitalSurfaceTracker
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    virtual ~ExplicitDigitalSurfaceTracker();

    /**
     * Constructor. The tracker is not valid.
     * see init
     */
    ExplicitDigitalSurfaceTracker();

    /**
     * Initializes the tracker to move on the given explicit digital surface.
     *
     * @param eds any explicit digital surface.
     */
    void init( const ExplicitDigitalSurface* eds );




    // ------------ Implementation of abstract services -----------------------
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
     * The object boundary over which the tracker is moving.
     */
    const ExplicitDigitalSurface* m_eds;

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
    INLINE ExplicitDigitalSurfaceTracker( const ExplicitDigitalSurfaceTracker & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ExplicitDigitalSurfaceTracker & operator=( const ExplicitDigitalSurfaceTracker & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
     Overloads 'operator<<' for displaying objects of class 'ExplicitDigitalSurfaceTracker'.
     @param that_stream the output stream where the object is written.
     @param that_object_to_display the object of class 'ExplicitDigitalSurfaceTracker' to write.
     @return the output stream after the writing.
   */
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const ExplicitDigitalSurfaceTracker & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/ExplicitDigitalSurfaceTracker.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ExplicitDigitalSurfaceTracker_h

#undef ExplicitDigitalSurfaceTracker_RECURSES
#endif // else defined(ExplicitDigitalSurfaceTracker_RECURSES)
