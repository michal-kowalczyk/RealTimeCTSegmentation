/** @file C4CIteratorOnBdry.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CIteratorOnBdry.h
//
// Creation : 2003/02/12
//
// Version : 2003/02/12
//
// Author : JOL
//
// Summary : Header file for files C4CIteratorOnBdry.ih and C4CIteratorOnBdry.cxx
//
// History :
//	2003/02/12 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CIteratorOnBdry declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CIteratorOnBdry_RECURSES)
#error Recursive header files inclusion detected in C4CIteratorOnBdry.h
#else // defined(C4CIteratorOnBdry_RECURSES)
#define C4CIteratorOnBdry_RECURSES

#if !defined C4CIteratorOnBdry_h
#define C4CIteratorOnBdry_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/BelAdjacency.h"
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class C4CIteratorOnBdry
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'C4CIteratorOnBdry' <p>
 * Aim: An adapter to see a tuple (boundary, bel, direction) as a 4-connected
 * contour. Useful for some discrete geometry computations. Represents an
 * iterator moving from bel to bel as a 4-connected contour.
 */
class C4CIteratorOnBdry : public C4CIteratorOnSurface
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  INLINE virtual ~C4CIteratorOnBdry();

  /**
   * Constructor.
   * The 4-connected contour is then defined by the bel [b] on the boundary of
   * the object [obj], given a direction of tracking [k] and a bel adjacency.
   * @param badj a bel adjacency defined in a digital space.
   * @param b any bel of the object [obj].
   * @param k any direction different from 'b.orthDir()'.
   * @param obj any set of spels in [ks].
   */
  INLINE C4CIteratorOnBdry( const BelAdjacency & badj,
			    Kn_sid b, 
			    uint k,
			    const KnCharSet & obj );
  
  /**
   * Constructor.
   * The 4-connected contour is then defined by the bel_iterator [iter] on
   * the boundary of the object [obj].
   * @param biter a bel iterator specifying the starting bel, the bel adjacency, the tracking direction.
   * @param obj any set of spels in [ks].
   */
  INLINE C4CIteratorOnBdry( const BelAdjacency::bel_iterator & biter,
			    const KnCharSet & obj );


  // ------------------- Implementation of abstract services ------------------
public:

  /**
   * @return a clone of the current iterator.
   */
  INLINE virtual C4CIterator* clone() const;

  /**
   * @param other any other iterator.
   * @return 'true' if [other] points to the same location as 'this'.
   */
  INLINE virtual bool equals( const C4CIterator & other ) const;


  /**
   * @return the current bel the iterator is pointing at.
   */
  INLINE virtual Kn_sid current() const;

  /**
   * @return the current track direction.
   */
  INLINE virtual uint trackDir() const;

  /**
   * @return the current orthogonal direction.
   */
  INLINE virtual uint orthDir() const;

  
  /**
   * Moves the iterator on the 4-connected contour to the next position.
   * @return 0 if the move was impossible, 1 if it was a move toward the interior, 2 if it was a straight movement, 3 if it was a move toward the exterior.
   * NB: If the C4CIterator is moving over a digital surface, then 'next'
   * means moving along a direct tracking direction.
   */
  INLINE virtual uint next();

  /**
   * Moves the iterator on the 4-connected contour to the previous position.
   * @return 0 if the move was impossible, 1 if it was a move toward the interior, 2 if it was a straight movement, 3 if it was a move toward the exterior.
   * NB: If the C4CIterator is moving over a digital surface, then 
   * 'previous' means moving along an indirect tracking direction.
   */
  INLINE virtual uint previous();

 
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
   * Object over which the tracking is taking place.
   */
  KnCharSet m_obj;
  
  /**
   * Bel iterator.
   */
  BelAdjacency::bel_iterator m_iterator;

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE C4CIteratorOnBdry();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE C4CIteratorOnBdry( const C4CIteratorOnBdry & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE C4CIteratorOnBdry & operator=( const C4CIteratorOnBdry & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'C4CIteratorOnBdry'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'C4CIteratorOnBdry' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const C4CIteratorOnBdry & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/C4CIteratorOnBdry.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CIteratorOnBdry_h

#undef C4CIteratorOnBdry_RECURSES
#endif // else defined(C4CIteratorOnBdry_RECURSES)
