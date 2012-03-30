/** @file C4CIteratorOnFreemanChainSurface.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CIteratorOnFreemanChainSurface.h
//
// Creation : 2008/05/22
//
// Version : 2008/05/22
//
// Author : JOL
//
// Summary : Header file for files C4CIteratorOnFreemanChainSurface.ih and C4CIteratorOnFreemanChainSurface.cxx
//
// History :
//	2008/05/22 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CIteratorOnFreemanChainSurface declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CIteratorOnFreemanChainSurface_RECURSES)
#error Recursive header files inclusion detected in C4CIteratorOnFreemanChainSurface.h
#else // defined(C4CIteratorOnFreemanChainSurface_RECURSES)
#define C4CIteratorOnFreemanChainSurface_RECURSES

#if !defined C4CIteratorOnFreemanChainSurface_h
#define C4CIteratorOnFreemanChainSurface_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class C4CIteratorOnFreemanChainSurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'C4CIteratorOnFreemanChainSurface' <p> Aim:
   * A version of the iterator on a FreemanChain that lies in a
   * digital space 'KnSpace'. To each code of the chain is thus
   * associated an oriented surfel of the digital space.
   */
  class C4CIteratorOnFreemanChainSurface : public C4CIteratorOnSurface
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    INLINE virtual ~C4CIteratorOnFreemanChainSurface();

    /**
     * Default Constructor.
     * The object is not valid.
     */
    INLINE C4CIteratorOnFreemanChainSurface();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    INLINE C4CIteratorOnFreemanChainSurface
    ( const C4CIteratorOnFreemanChainSurface & other );
 
    /**
     * Initializes the iterator as following the sequence of Freeman
     * moves [s] with initial index position [pos].
     *
     * @param ks the digital space.
     *
     * @param it an iterator in a Freeman chain code.
     */
    INLINE void init( const KnSpace* ks,
		      const C4CIteratorOnFreemanChain & it );


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


    /**
     * @return the current surfel the iterator is pointing at.
     */
    virtual Kn_sid current() const;
    
    /**
     * @return the current track direction.
     */
    virtual uint trackDir() const;
    
    /**
     * @return the current orthogonal direction.
     */
    virtual uint orthDir() const;

    
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
     * The digital space where the Freeman chain lies.
     */
    const KnSpace* m_ks;

    /**
     * The iterator on the Freeman chain.
     */
    C4CIteratorOnFreemanChain m_it;

    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE C4CIteratorOnFreemanChainSurface & operator=( const C4CIteratorOnFreemanChainSurface & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'C4CIteratorOnFreemanChainSurface'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'C4CIteratorOnFreemanChainSurface' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const C4CIteratorOnFreemanChainSurface & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CIteratorOnFreemanChainSurface_h

#undef C4CIteratorOnFreemanChainSurface_RECURSES
#endif // else defined(C4CIteratorOnFreemanChainSurface_RECURSES)
