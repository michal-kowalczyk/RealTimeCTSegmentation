/** @file DigitalSurface.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DigitalSurface.h
//
// Creation : 2003/07/08
//
// Version : 2003/07/08
//
// Author : JOL
//
// Summary : Header file for files DigitalSurface.ih and DigitalSurface.cxx
//
// History :
//	2003/07/08 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DigitalSurface declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DigitalSurface_RECURSES)
#error Recursive header files inclusion detected in DigitalSurface.h
#else // defined(DigitalSurface_RECURSES)
#define DigitalSurface_RECURSES

#if !defined DigitalSurface_h
#define DigitalSurface_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/digitalnD/DigitalSurfaceTracker.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene {

  class KnSpace;

///////////////////////////////////////////////////////////////////////////////
// class DigitalSurface
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'DigitalSurface' <p>
 * Aim: Abstract class to represent a digital surface, ie a set of n-1-cells
 * or surfels. Digital surfaces can be represented explicitly, for instance
 * a set of (oriented) surfels, or implicitly, for instance as the boundary of
 * a set of spels.
 * Furthermore, any digital surface can be tracked with "2D"-like iterators
 * on 4-connected contours, given a starting surfel and a direction to track.
 * @see KnCharSet,ObjectBoundary,C4CIterator
 */
class DigitalSurface
{
  // ------------------------- space services -------------------------------
 public:
  
  /**
   * @return a pointer on the digital space in which the factory is embedded.
   */
  virtual const KnSpace* space() const = 0;

  /** 
   * Destructor. Does nothing.
   */
  INLINE virtual ~DigitalSurface();

  // ------------------------- Factory services ------------------------------
 public:

  /**
   * Returns a surface tracker pointing on [surfel].
   *
   * @param surfel any surfel.
   *
   * @return a dyn. alloc. surface tracker (must be desallocated by
   * the caller).
   */
  virtual DigitalSurfaceTracker* newTracker( Kn_sid surfel ) const = 0;

  /**
   * Returns an iterator pointing on [surfel] and moving along direction [dir]
   * in the direct tracking direction. Each movement is given as a direction
   * on a 4-connected contour.
   *
   * @param surfel any surfel.
   * @param dir a tracking direction (different from the orthogonal direction to [surfel]).
   *
   * @return a dyn. alloc. contour iterator (must be desallocated by
   * the caller).
   */
  virtual C4CIteratorOnSurface* newC4CIterator( Kn_sid surfel, uint dir ) const = 0;
  

  // ------------------------- Interface --------------------------------------
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
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'DigitalSurface'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'DigitalSurface' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const DigitalSurface & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "DigitalSurface.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DigitalSurface_h

#undef DigitalSurface_RECURSES
#endif // else defined(DigitalSurface_RECURSES)
