/** @file ImplicitDigitalSurface.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImplicitDigitalSurface.h
//
// Creation : 2005/01/17
//
// Version : 2005/01/17
//
// Author : JOL
//
// Summary : Header file for files ImplicitDigitalSurface.ih and ImplicitDigitalSurface.cxx
//
// History :
//	2005/01/17 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImplicitDigitalSurface declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImplicitDigitalSurface_RECURSES)
#error Recursive header files inclusion detected in ImplicitDigitalSurface.h
#else // defined(ImplicitDigitalSurface_RECURSES)
#define ImplicitDigitalSurface_RECURSES

#if !defined ImplicitDigitalSurface_h
#define ImplicitDigitalSurface_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
#include "ImaGene/digitalnD/Embedder.h"
#include "ImaGene/digitalnD/DigitalSurface.h"
#include "ImaGene/3d/ImplicitSurface.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class ImplicitDigitalSurface
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitDigitalSurface' <p>
   * Aim: An adapter of 'ImplicitSurface' to 'DigitalSurface'.
   */
  class ImplicitDigitalSurface : public DigitalSurface
  {

    // ----------------------- Static services ------------------------------
  public:


    /**
     * Find a bel of the implicit digital surface by random tries then
     * dichotomy.
     *
     * @param ids any implicit digital surface.
     * @param nbries the maximum number of random tries.
     * @return a boundary surfel of [ids] or '0' if none was found after [nbtries] iterations. 
     *
     ** @todo check bounds of implicit surface.
     */
    static Kn_sid findABel( const ImplicitDigitalSurface & ids,
			    uint nbtries );
    


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    virtual ~ImplicitDigitalSurface();

    /**
     * Constructor. The space is referenced and must not be
     * destroyed. The implicit surface and the embedder are
     * cloned. The bel adjacency is referenced.
     *
     * @param ks the digital space.
     * @param is the implicit surface (cloned).
     * @param embedder the embedder defining how the spels of the digital space are embedded in R3 (cloned).
     * @param badj a bel adjacency.
     */
    ImplicitDigitalSurface( const KnSpace & ks, 
			    const BelAdjacency & badj,
			    const ImplicitSurface & is,
			    const Embedder & spel_embedder );



    // ------------------------- accessor services ----------------------------
  public:

    /**
     * @return the bel adjaceny.
     */
    INLINE const BelAdjacency & belAdjacency() const;
    
    /**
     * @return the associated implicit surface.
     */
    INLINE const ImplicitSurface & implicitSurface() const;

    /**
     * @return the associated embedder.
     */
    INLINE const Embedder & embedder() const;
    

    // ------------------------- space services -------------------------------
  public:
    
    /**
     * @return a pointer on the digital space in which the implicit
     * digital surface is embedded.
     */
    INLINE virtual const KnSpace* space() const;
    

    // ------------------------- set services -------------------------------
  public:
    
    /**
     * Useful for 'SurfelNeighborhood.adjacentOnAnyBdry'.
     *
     * @param spel any spel of the digital space.
     * @return 'true' if the implicit surface has a positive or zero value at the spel embedding location, 'false' otherwise.
     */
    bool operator[]( Kn_uid spel ) const;
    
    

    
    // ------------------------- Factory services ----------------------------
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
     * Returns an iterator pointing on [surfel] and moving along
     * direction [dir] in the direct tracking direction. Each movement
     * is given as a direction on a 4-connected contour.
     *
     * @param surfel any surfel.
     * @param dir a tracking direction (different from the orthogonal direction to [surfel]).
     *
     * @return a dyn. alloc. contour iterator (must be desallocated by
     * the caller).
     */
    virtual C4CIteratorOnSurface* newC4CIterator( Kn_sid surfel, uint dir ) const;
 

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
     * The digital space (not owned).
     */
    const KnSpace* m_space;

    /**
     * The bel adjacency (not owned).
     */
    const BelAdjacency & m_bel_adj;
    
    /**
     * The implicit surface (owned).
     */
    ImplicitSurface* m_is;
    
    /**
     * The spel embedder (owned).
     */
    Embedder* m_embedder;
    

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE ImplicitDigitalSurface();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImplicitDigitalSurface( const ImplicitDigitalSurface & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImplicitDigitalSurface & operator=( const ImplicitDigitalSurface & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'ImplicitDigitalSurface'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ImplicitDigitalSurface' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const ImplicitDigitalSurface & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/ImplicitDigitalSurface.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImplicitDigitalSurface_h

#undef ImplicitDigitalSurface_RECURSES
#endif // else defined(ImplicitDigitalSurface_RECURSES)
