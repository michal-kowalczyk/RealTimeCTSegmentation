/** @file C4CIteratorOnImplicitDigitalSurface.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CIteratorOnImplicitDigitalSurface.h
//
// Creation : 2005/01/18
//
// Version : 2005/01/18
//
// Author : JOL
//
// Summary : Header file for files C4CIteratorOnImplicitDigitalSurface.ih and C4CIteratorOnImplicitDigitalSurface.cxx
//
// History :
//	2005/01/18 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CIteratorOnImplicitDigitalSurface declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CIteratorOnImplicitDigitalSurface_RECURSES)
#error Recursive header files inclusion detected in C4CIteratorOnImplicitDigitalSurface.h
#else // defined(C4CIteratorOnImplicitDigitalSurface_RECURSES)
#define C4CIteratorOnImplicitDigitalSurface_RECURSES

#if !defined C4CIteratorOnImplicitDigitalSurface_h
#define C4CIteratorOnImplicitDigitalSurface_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
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
  // class C4CIteratorOnImplicitDigitalSurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'C4CIteratorOnImplicitDigitalSurface' <p>
   * Aim: Implements 'C4CIteratorOnSurface' on and 'ImplicitDigitalSurface'.
   * Uses 'SurfelNeighborhood'.
   */
  class C4CIteratorOnImplicitDigitalSurface : public C4CIteratorOnSurface
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~C4CIteratorOnImplicitDigitalSurface();

    /**
     * Constructor. The iterator is not valid.
     * @see init, start
     */
    C4CIteratorOnImplicitDigitalSurface();

    /**
     * Initializes the iterator on implicit digital surface to move on the
     * implicit digital surface [ids].
     *
     * @param ids any implicit digital surface (referenced, not cloned).
     */
    void init( const ImplicitDigitalSurface* ids );
      
    /**
     * Starts the iterator at surfel [s] in track direction [track_dir].
     *
     * @param s any surfel of the implicit digital surface.
     * @param track_dir any direction different from 'orthDir( s )'.
     */
    void start( Kn_sid s, uint track_dir );



    // ----------------------- C4CIteratorOnSurface services ----------------
  public:

    /**
     * @return a clone of the current iterator.
     */
    virtual C4CIterator* clone() const;
    
    /**
     * @param other any other iterator.
     * @return 'true' if [other] points to the same location as 'this'.
     */
    virtual bool equals( const C4CIterator & other ) const;
    
    /**
     * Moves the iterator on the 4-connected contour to the next position.
     * @return 0 if the move was impossible, 1 if it was a move toward the interior, 2 if it was a straight movement, 3 if it was a move toward the exterior.
     * NB: If the C4CIterator is moving over a digital surface, then 'next'
     * means moving along a direct tracking direction.
     */
    virtual uint next();
    
    /**
     * Moves the iterator on the 4-connected contour to the previous position.
     * @return 0 if the move was impossible, 1 if it was a move toward the interior, 2 if it was a straight movement, 3 if it was a move toward the exterior.
     * NB: If the C4CIterator is moving over a digital surface, then 
     * 'previous' means moving along an indirect tracking direction.
     */
    virtual uint previous();
    
    
    // ----------------------- C4CIteratorOnSurface services ----------------
  public:
    
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
     * The implicit digital surface over which the tracker is moving.
     */
    const ImplicitDigitalSurface* m_ids;

    /**
     * A helper class to move on the boundary.
     */
    SurfelNeighborhood m_neighborhood;

    /**
     * The current track direction.
     */
    uint m_track_dir;


    // ------------------------- Hidden services ------------------------------
  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE C4CIteratorOnImplicitDigitalSurface( const C4CIteratorOnImplicitDigitalSurface & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE C4CIteratorOnImplicitDigitalSurface & operator=( const C4CIteratorOnImplicitDigitalSurface & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'C4CIteratorOnImplicitDigitalSurface'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'C4CIteratorOnImplicitDigitalSurface' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const C4CIteratorOnImplicitDigitalSurface & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/C4CIteratorOnImplicitDigitalSurface.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CIteratorOnImplicitDigitalSurface_h

#undef C4CIteratorOnImplicitDigitalSurface_RECURSES
#endif // else defined(C4CIteratorOnImplicitDigitalSurface_RECURSES)
