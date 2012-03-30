/** @file ObjectBoundary.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ObjectBoundary.h
//
// Creation : 2003/02/13
//
// Version : 2003/02/13
//
// Author : JOL
//
// Summary : Header file for files ObjectBoundary.ih and ObjectBoundary.cxx
//
// History :
//	2003/02/13 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ObjectBoundary declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ObjectBoundary_RECURSES)
#error Recursive header files inclusion detected in ObjectBoundary.h
#else // defined(ObjectBoundary_RECURSES)
#define ObjectBoundary_RECURSES

#if !defined ObjectBoundary_h
#define ObjectBoundary_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/DigitalSurface.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class ObjectBoundary
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'ObjectBoundary' <p>
 * Aim: To represent the boundary of a set of spels. 
 */
class ObjectBoundary : public DigitalSurface
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~ObjectBoundary();

  /**
   * Constructor. Represents the boundary of the set of spels [object]
   * considered with the [badj] bel adjacency.
   * @param badj any bel adjacency of the space where [object] is defined.
   * @param object the set of spels.
   */
  ObjectBoundary( const BelAdjacency & badj, const KnCharSet & object );


  /**
   * @return a pointer on the digital space in which the factory is embedded.
   */
  INLINE virtual const KnSpace* space() const;

  /**
   * @return the bel adjacency chosen for this surface.
   */
  INLINE const BelAdjacency & belAdjacency() const;
  
  /**
   * @return the object as a set of spels.
   */
  INLINE const KnCharSet & object() const;

  // ------------------------- set services -------------------------------
public:
  /**
   * Useful for 'SurfelNeighborhood::adjacentOnOrientedSurfaces'.
   *
   * @param surfel any surfel of the digital space.
   * @return 'true' if the surfel belongs to the oriented boundary of the object, 'false' otherwise.
   */
  bool operator[]( Kn_sid surfel ) const;

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
  virtual DigitalSurfaceTracker* newTracker( Kn_sid surfel ) const;


  /**
   * Returns an iterator pointing on [surfel] and moving along direction [dir]
   * in the direct tracking direction. Each movement is given as a direction
   * on a 4-connected contour.
   * @param surfel any surfel.
   * @param dir a tracking direction (different from the orthogonal direction to [surfel]).
   * @return a dyn. alloc. contour iterator (in fact, a 'C4CIteratorOnBdry').
   * @see C4CIterator,C4CIteratorOnBdry
   */
  virtual C4CIteratorOnSurface* newC4CIterator( Kn_sid surfel, uint dir ) const;

  
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

  /**
   * Reference on a bel adjacency relation, which indicates the topology of 
   * the bels.
   */
  const BelAdjacency & m_badj;
  /**
   * The object (as a set of spels) whose boundary is considered.
   */
  KnCharSet m_object;

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE ObjectBoundary();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE ObjectBoundary( const ObjectBoundary & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE ObjectBoundary & operator=( const ObjectBoundary & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'ObjectBoundary'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'ObjectBoundary' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const ObjectBoundary & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/ObjectBoundary.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ObjectBoundary_h

#undef ObjectBoundary_RECURSES
#endif // else defined(ObjectBoundary_RECURSES)
