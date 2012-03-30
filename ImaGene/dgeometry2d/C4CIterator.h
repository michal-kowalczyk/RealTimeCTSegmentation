/** @file C4CIterator.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CIterator.h
//
// Creation : 2003/02/12
//
// Version : 2003/02/12
//
// Author : JOL
//
// Summary : Header file for files C4CIterator.ih and C4CIterator.cxx
//
// History :
//	2003/02/12 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CIterator declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CIterator_RECURSES)
#error Recursive header files inclusion detected in C4CIterator.h
#else // defined(C4CIterator_RECURSES)
#define C4CIterator_RECURSES

#if !defined C4CIterator_h
#define C4CIterator_h

//////////////////////////////////////////////////////////////////////////////
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class C4CIterator
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'C4CIterator' <p>
 * Aim: An interface to describe an iterator moving on an oriented
 * 4-connected contour. The iterator is situated on the edge of the 4-connected
 * contour.
 */
class C4CIterator
{
  // ------------------------- Static services ------------------------------
public:

  /**
   * Counts the number of elements of the digital contour implicitly
   * defined by the iterator. Works also if the contour is open, but
   * in this case, the iterator is moved to the back of the contour.
   *
   * @param it any iterator on a digital contour (modified only if the contour is open).
   * @param is_open (returns) 'true' if the digital contour is open, 'false' otherwise.
   * @return the number of elements of the digital contour.
   */
  static uint size( C4CIterator & it, bool & is_open );
  
  /**
   * Counts the number of elements of the digital contour implicitly
   * defined by the iterator. Works also if the contour is open, but
   * in this case, the iterator is moved to the back of the contour.
   *
   * @param it any iterator on a digital contour (modified only if the contour is open).
   * @return the number of elements of the digital contour.
   */
  static uint size( C4CIterator & it );

  // ------------------------- Contour services ------------------------------
public:
  /**
   * Destructor.
   */
  virtual ~C4CIterator() {}

  /**
   * @return a clone of the current iterator.
   */
  virtual C4CIterator* clone() const = 0;

  /**
   * @param other any other iterator.
   * @return 'true' if [other] points to the same location as 'this'.
   */
  virtual bool equals( const C4CIterator & other ) const = 0;

  /**
   * Moves the iterator on the 4-connected contour to the next position.
   *
   * @return 0 if the move was impossible, 1 if it was a move toward
   * the interior, 2 if it was a straight movement, 3 if it was a move
   * toward the exterior. It may be 4 in the case of a half-turn.
   *
   * NB: If the C4CIterator is moving over a digital surface, then 'next'
   * means moving along a direct tracking direction.
   */
  virtual uint next() = 0;

  /**
   * Moves the iterator on the 4-connected contour to the previous position.
   *
   * @return 0 if the move was impossible, 1 if it was a move toward
   * the interior, 2 if it was a straight movement, 3 if it was a move
   * toward the exterior. It may be 4 in the case of a half-turn.
   *
   * NB: If the C4CIterator is moving over a digital surface, then 
   * 'previous' means moving along an indirect tracking direction.
   */
  virtual uint previous() = 0;

};

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene


// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/C4CIterator.ih"
#endif


#endif // !defined C4CIterator_h

#undef C4CIterator_RECURSES
#endif // else defined(C4CIterator_RECURSES)
