/** @file DigitalSurfaceTracker.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DigitalSurfaceTracker.h
//
// Creation : 2005/01/11
//
// Version : 2005/01/11
//
// Author : JOL
//
// Summary : Header file for files DigitalSurfaceTracker.ih and DigitalSurfaceTracker.cxx
//
// History :
//	2005/01/11 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DigitalSurfaceTracker declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DigitalSurfaceTracker_RECURSES)
#error Recursive header files inclusion detected in DigitalSurfaceTracker.h
#else // defined(DigitalSurfaceTracker_RECURSES)
#define DigitalSurfaceTracker_RECURSES

#if !defined DigitalSurfaceTracker_h
#define DigitalSurfaceTracker_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{

  class DigitalSurface;

  
  /////////////////////////////////////////////////////////////////////////////
  // class DigitalSurfaceTracker
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'DigitalSurfaceTracker' <p> Aim: Interface
   * representing a surfel moving on a digital surface by following
   * adjacencies.
   */
  class DigitalSurfaceTracker
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. Does nothing.
     */
    INLINE virtual ~DigitalSurfaceTracker();


    // ----------------------- abstract services ------------------------------
  public:

    /**
     * @return the digital surface on which the tracker is moving.
     */
    virtual const DigitalSurface* surface() const = 0;

    /**
     * @return a clone of 'this'.
     */
    virtual DigitalSurfaceTracker* clone() const = 0;

    /**
     * @return the current surfel at which the tracker is pointing.
     */
    virtual Kn_sid current() const = 0;
    
    /**
     * @return the orthogonal direction to the current surfel.
     */
    virtual uint orthDir() const = 0;
    
    /**
     * Moves the tracker to the surfel [s].
     * @param s any surfel on the surface.
     */
    virtual void move( Kn_sid s ) = 0;

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
    virtual Kn_sid adjacent( uint dir, bool pos ) = 0;
    
    /**
     * @return the move code associated to the last call to
     * 'adjacent'. When '0', there was no adjacent surfel, when '=n'
     * (n=1..3), the adjacent surfel is the n-th interior surfel.
     * 
     * @see adjacent
     */
    virtual uint lastCode() const = 0;
    
  };

} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#include "ImaGene/digitalnD/DigitalSurfaceTracker.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DigitalSurfaceTracker_h

#undef DigitalSurfaceTracker_RECURSES
#endif // else defined(DigitalSurfaceTracker_RECURSES)
