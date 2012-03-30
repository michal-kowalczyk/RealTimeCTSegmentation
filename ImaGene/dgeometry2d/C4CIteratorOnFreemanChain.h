/** @file C4CIteratorOnFreemanChain.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CIteratorOnFreemanChain.h
//
// Creation : 2008/05/22
//
// Version : 2008/05/22
//
// Author : JOL
//
// Summary : Header file for files C4CIteratorOnFreemanChain.ih and C4CIteratorOnFreemanChain.cxx
//
// History :
//	2008/05/22 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CIteratorOnFreemanChain declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CIteratorOnFreemanChain_RECURSES)
#error Recursive header files inclusion detected in C4CIteratorOnFreemanChain.h
#else // defined(C4CIteratorOnFreemanChain_RECURSES)
#define C4CIteratorOnFreemanChain_RECURSES

#if !defined C4CIteratorOnFreemanChain_h
#define C4CIteratorOnFreemanChain_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/dgeometry2d/C4CIterator.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class C4CIteratorOnFreemanChain
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'C4CIteratorOnFreemanChain' <p>
   * Aim: An exemple of iterator on a 4-connected path : an iterator on a
   * Freeman chain. Adaptor class for FreemanChain::const_iterator.
   */
  class C4CIteratorOnFreemanChain : public C4CIterator
  {


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    INLINE virtual ~C4CIteratorOnFreemanChain();
    
    /**
     * Default Constructor.
     * The object is not valid.
     */
    INLINE C4CIteratorOnFreemanChain();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    INLINE C4CIteratorOnFreemanChain( const C4CIteratorOnFreemanChain & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE C4CIteratorOnFreemanChain & operator=( const C4CIteratorOnFreemanChain & other );

    
    /**
     * @return the iterator on FreemanChain.
     */
    INLINE const FreemanChain::const_iterator & getIterator() const;

    /**
     * Initializes the iterator as following the sequence of Freeman
     * moves [s] with initial index position [pos].
     *
     * @param it an iterator in a Freeman chain code.
     *
     * @param loop when 'true', the chain loops, otherwise it has a
     * beginning and an end.
     */
    INLINE void init( const FreemanChain::const_iterator & it,
		      bool loop = false );

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
     * Moves the iterator on the 4-connected contour to the next
     * position.  @return 0 if the move was impossible, 1 if it was a
     * move toward the interior, 2 if it was a straight movement, 3 if
     * it was a move toward the exterior.  NB: If the C4CIterator is
     * moving over a digital surface, then 'next' means moving along a
     * direct tracking direction.
     */
    virtual uint next();

    /**
     * Moves the iterator on the 4-connected contour to the previous
     * position.  @return 0 if the move was impossible, 1 if it was a
     * move toward the interior, 2 if it was a straight movement, 3 if
     * it was a move toward the exterior.  NB: If the C4CIterator is
     * moving over a digital surface, then 'previous' means moving
     * along an indirect tracking direction.
     */
    virtual uint previous();


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
     * Iterator on the FreemanChain.
     */
    FreemanChain::const_iterator m_it;

    /**
     * Indicates if the chain loops or not.
     */
    bool m_loop;

    // ------------------------- Hidden services ------------------------------
  protected:


  private:


  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'C4CIteratorOnFreemanChain'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'C4CIteratorOnFreemanChain' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const C4CIteratorOnFreemanChain & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CIteratorOnFreemanChain_h

#undef C4CIteratorOnFreemanChain_RECURSES
#endif // else defined(C4CIteratorOnFreemanChain_RECURSES)
