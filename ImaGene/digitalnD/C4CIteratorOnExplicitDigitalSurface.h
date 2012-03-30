/** @file C4CIteratorOnExplicitDigitalSurface.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CIteratorOnExplicitDigitalSurface.h
//
// Creation : 2011/05/04
//
// Version : 2011/05/04
//
// Author : JOL
//
// Summary : Header file for files C4CIteratorOnExplicitDigitalSurface.ih and C4CIteratorOnExplicitDigitalSurface.cxx
//
// History :
//	2011/05/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CIteratorOnExplicitDigitalSurface declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CIteratorOnExplicitDigitalSurface_RECURSES)
#error Recursive header files inclusion detected in C4CIteratorOnExplicitDigitalSurface.h
#else // defined(C4CIteratorOnExplicitDigitalSurface_RECURSES)
#define C4CIteratorOnExplicitDigitalSurface_RECURSES

#if !defined C4CIteratorOnExplicitDigitalSurface_h
#define C4CIteratorOnExplicitDigitalSurface_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/digitalnD/ExplicitDigitalSurface.h"
#include "ImaGene/digitalnD/ExplicitDigitalSurfaceTracker.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class C4CIteratorOnExplicitDigitalSurface
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'C4CIteratorOnExplicitDigitalSurface' <p>
 * Aim: An adapter to see a tuple (surface, bel, direction) as a 4-connected
 * contour. Useful for some discrete geometry computations. Represents an
 * iterator moving from bel to bel as a 4-connected contour.
 */
class C4CIteratorOnExplicitDigitalSurface : public C4CIteratorOnSurface
{
  // ------------------------- Standard services ------------------------------
public:
  /**
     Destructor. 
   */
  virtual ~C4CIteratorOnExplicitDigitalSurface();

  /**
    Constructor.
    The 4-connected contour is then defined by the bel [b] on the boundary of
    the object [obj], given a direction of tracking [k] and a bel adjacency.

    @param eds any explicit digital surface.
    @param b any bel of this surface.
    @param k any direction different from 'b.orthDir()'.
  */
  C4CIteratorOnExplicitDigitalSurface( const ExplicitDigitalSurface & eds,
				       Kn_sid b, 
				       uint k );
  
  // ------------------- Implementation of abstract services ------------------
public:

  /**
    @return a clone of the current iterator.
   */
  virtual C4CIterator* clone() const;

  /**
    @param other any other iterator.
    @return 'true' if [other] points to the same location as 'this'.
   */
  virtual bool equals( const C4CIterator & other ) const;

  /**
    @return the current bel the iterator is pointing at.
   */
  virtual Kn_sid current() const;

  /**
     @return the current track direction.
  */
  virtual uint trackDir() const;

  /**
     @return the current orthogonal direction.
  */
  virtual uint orthDir() const;

  
  /**
    Moves the iterator on the 4-connected contour to the next position.

    @return 0 if the move was impossible, 1 if it was a move toward
    the interior, 2 if it was a straight movement, 3 if it was a move
    toward the exterior.

    NB: If the C4CIterator is moving over a digital surface, then
    'next' means moving along a direct tracking direction.
   */
  virtual uint next();

  /**
     Moves the iterator on the 4-connected contour to the previous position.

     @return 0 if the move was impossible, 1 if it was a move toward
     the interior, 2 if it was a straight movement, 3 if it was a move
     toward the exterior.

     NB: If the C4CIterator is moving over a digital surface, then
    'previous' means moving along an indirect tracking direction.
   */
  virtual uint previous();

 
  // ------------------------- Interface --------------------------------------
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
  
  // ------------------------- Datas ------------------------------------------
private:

  /// Object over which the tracking is taking place.
  ExplicitDigitalSurfaceTracker m_tracker;

  /// Current direction of the tracker.
  uint m_dir;

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE C4CIteratorOnExplicitDigitalSurface();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE C4CIteratorOnExplicitDigitalSurface( const C4CIteratorOnExplicitDigitalSurface & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE C4CIteratorOnExplicitDigitalSurface & operator=( const C4CIteratorOnExplicitDigitalSurface & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'C4CIteratorOnExplicitDigitalSurface'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'C4CIteratorOnExplicitDigitalSurface' to write.
 * @return the output stream after the writing.
 */
std::ostream&
operator<<( std::ostream & that_stream, const C4CIteratorOnExplicitDigitalSurface & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/C4CIteratorOnExplicitDigitalSurface.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CIteratorOnExplicitDigitalSurface_h

#undef C4CIteratorOnExplicitDigitalSurface_RECURSES
#endif // else defined(C4CIteratorOnExplicitDigitalSurface_RECURSES)
