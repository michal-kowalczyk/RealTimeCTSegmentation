/** @file CubicalSpace.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : CubicalSpace.h
//
// Creation : 2008/03/20
//
// Version : 2008/03/20
//
// Author : JOL
//
// Summary : Header file for files CubicalSpace.ih and CubicalSpace.cxx
//
// History :
//	2008/03/20 : ?Name? : ?What?
//
// Rcs Id : "@(#)class CubicalSpace declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(CubicalSpace_RECURSES)
#error Recursive header files inclusion detected in CubicalSpace.h
#else // defined(CubicalSpace_RECURSES)
#define CubicalSpace_RECURSES

#if !defined CubicalSpace_h
#define CubicalSpace_h

//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <iostream>
#include "ImaGene/digitalnD/BoxEmbedder.h"
#include "ImaGene/digitalnD/CubicalCell.h"
#include "ImaGene/digitalnD/KnSpace.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class CubicalSpace
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CubicalSpace' <p> Aim: Represents the
   * binary hierarchical decomposition of a generally embedded cubical
   * space. Cells in this space have not only a position in a space
   * but a level in the space decomposition. The level is deduced from
   * the coordinates as in a heap structure.
   */
  class CubicalSpace
  {

  public:


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~CubicalSpace();

    /**
     * Constructor. Need dimension of space and embedding box.
     *
     * @param dim the dimension of the space.
     * @param x1 the lower-bound of the coordinates of the embedding
     * box.
     *
     * @param x2 it is the upper-bound of the coordinates of the
     * embedding box.
     *
     * @param k the number of 2^i digital spaces to compute, 0<=i<=k.
     */
    CubicalSpace( uint dim, const float* x1, const float* x2,
		  uint k );

    /**
     * @return the dimension of the cubical space.
     */
    INLINE uint dim() const;

    // ----------------------- cubical space services -----------------------
  public:

    /**
     * Given a cubical cell, return the space it spans as a box
     * embedder.  For instance, a n-1-cell spans a n-1-subspace. The
     * cell 3,3,3 spans the whole box.
     *
     * @param x1 (returns) the lower-bound of the coordinates of the
     * embedding box.
     *
     * @param x2 (returns) the upper-bound of the coordinates of the
     * embedding box.
     *
     * @param c a hierarchical cubical cell.
     */
    void getBoundingBox( float* x1, float* x2,
			 const CubicalCell & c ) const;

    /**
     * @param x (returns) the centroid of the cell [c].
     *
     * @param c a hierarchical cubical cell.
     */
    void getCentroid( float* x, const CubicalCell & c ) const;


    /**
     * @param k the degree of the subdivision (from 0 to 9).
     * @return a regular 2^k x ... x 2^k digital space.
     */
    KnSpace& getKnSpace( uint k ) const; 

    /**
     * @param k the degree of the subdivision (from 0 to 9).
     * @return a regular 2^k x ... x 2^k x 2 digital space.
     */
    KnSpace& getExtKnSpace( uint k ) const; 

    /**
     * This method returns the digital space of level [k] (sizes are
     * 2^k) and the associated box embedder, so that the digital space
     * spans the subspace defined by the hierarchical cubical cell
     * [c]. The digital space may be obtained either by
     * 'bemb->space()' or by this->getKnSpace( k )'.
     *
     * @param c a hierarchical cubical cell, that defines the spanned
     * subspace.
     *
     * @param k the subspace subdivision.
     *
     * @return a dyn. allocated box embedder spanning cell [c].
     */
    BoxEmbedder* getEmbedder( const CubicalCell & c, uint k ) const;

    /**
     * This method returns the digital space of level [k] (sizes are 2^k x
     * ... x 2^k x 2) and the associated box embedder, so that the digital
     * space spans the subspace defined by the hierarchical cubical cell
     * [c]. The digital space may be obtained either by 'bemb->space()' or
     * by this->getKnSpace( k )'. The embedding is centered around
     * [-thickness,+thickness] along last coordinate.
     *
     * @param c a hierarchical cubical cell, that defines the spanned
     * subspace.
     *
     * @param k the subspace subdivision.
     *
     * @param thickness the thickening of the space along the last
     * coordinate axis (positive, embedding is then in
     * [-thickness,+thickness]).
     *
     * @return a dyn. allocated box embedder spanning cell [c].
     */
    BoxEmbedder* getExtEmbedder( const CubicalCell & c, 
				 uint k,
				 float thickness ) const;

    /**
     * This method returns the digital space of level [k+1] (sizes are 2^k
     * x ... x 2^k x 2) and the associated box embedder, so that the
     * digital space spans the doubled subspace defined by the
     * hierarchical cubical cell [c]. The digital space may be obtained
     * either by 'bemb->space()' or by this->getKnSpace( k )'. The
     * embedding is centered around [-thickness,+thickness] along last
     * coordinate.
     *
     * @param c a hierarchical cubical cell, that defines the spanned
     * subspace.
     *
     * @param k the subspace subdivision.
     *
     * @param thickness the thickening of the space along the last
     * coordinate axis (positive, embedding is then in
     * [-thickness,+thickness]).
     *
     * @return a dyn. allocated box embedder spanning cell [c].
     */
    BoxEmbedder* getExtDoubledEmbedder( const CubicalCell & c, 
					uint k,
					float thickness ) const;


    /**
     * Compute the cubical cell corresponding to the digital cell [c] of
     * space [ks], where the space [ks] is a subdivision of the cell
     * [subspace_cell]. If the dimension of the digital space [ks] is
     * greater then the dimension of 'this', then the cell is simply
     * projected.
     *
     * @param subspace_cell the cubical cell (generally open) which
     * defines the subspace of the cubical space.
     *
     * @param ks a digital space, which is in fact a 2^k-subdivision
     * of [subspace_cell].
     *
     * @param c a digital cell
     *
     * @return the cubical cell analog to [c].
     */
    CubicalCell getCubicalCell( const CubicalCell & subspace_cell,
				const KnSpace & ks, Kn_uid c ) const;

    /**
     * Compute the cubical cells corresponding to the digital cells [qc] of
     * space [ks], where the space [ks] is a subdivision of the cell
     * [subspace_cell]. If the dimension of the digital space [ks] is
     * greater then the dimension of 'this', then the cells are simply
     * projected.
     *
     * @param qcc the list of the cubical cells analog to [qc].
     *
     * @param subspace_cell the cubical cell (generally open) which
     * defines the subspace of the cubical space.
     *
     * @param ks a digital space, which is in fact a 2^k-subdivision
     * of [subspace_cell].
     *
     * @param qc a list of digital cells
     *
     */
    void getCubicalCells( std::deque<CubicalCell> & qcc,
			  const CubicalCell & subspace_cell,
			  const KnSpace & ks, 
			  const std::deque<Kn_uid> & qc ) const;


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
     * Dimension of the cubical space.
     */
    const uint m_dim;
 
    /**
     * Number of subdivided digital spaces.
     */
    uint m_k;

    /**
     * lower-bound for the coordinates of the box.
     */
    float* m_x1;

    /**
     * upper-bound for the coordinates of the box.
     */
    float* m_x2;

    /**
     * Stores pre-computed KnSpace whose sizes are powers of 2.
     */
    KnSpace** m_knspaces;

    /**
     * Stores pre-computed KnSpace whose sizes are powers of 2 plus an
     * extra dimension of size 2.
     */
    KnSpace** m_ext_knspaces;

    /**
     * Stores precomputed steps along each direction.
     * m_steps[ i=0..dim-1 ][ j=0..k ]
     */
    float** m_steps;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE CubicalSpace();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE CubicalSpace( const CubicalSpace & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE CubicalSpace & operator=( const CubicalSpace & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  
  /**
   * A class to visit a subspace of a cubical space.
   */
  class CubicalSpaceScanner {

  public:
    /**
     * Constructor.
     *
     * @param space the cubical space to scan.
     */
    INLINE CubicalSpaceScanner( const CubicalSpace & space );

    /**
     * All cells of the same level and topology as start will be
     * scanned. The starting cell is generally some power of 2.
     * 
     * @param start the starting cell.
     */
    INLINE void init( const CubicalCell & start );

    /**
     * Copy constructor.
     * @param other the cell iterator to clone.
     */
    INLINE CubicalSpaceScanner( const CubicalSpaceScanner & other );
    
    /**
     * Assignment.
     * @param other the cell iterator to copy.
     * @return a reference on 'this'.
     */
    INLINE CubicalSpaceScanner& operator=( const CubicalSpaceScanner & other );
    
    /**
     * Destructor. Does nothing.
     */
    INLINE ~CubicalSpaceScanner();
    
    // ------------------------- iteration services ------------------------
  public:
    
    /**
     * @return the current cell (oriented or not depending on the set).
     * @see CubicalSpaceScanner::get
     */
    INLINE const CubicalCell & operator*() const;
    
    /**
     * @return the current cell (oriented or not depending on the set).
     * @see CubicalSpaceScanner::operator*
     */
    INLINE const CubicalCell & get() const;

    /**
     * @return the cell after the last one
     */
    INLINE const CubicalCell & end() const;

    /**
     * Goes to the next cell in the set.
     * @return 'true' is there is one, 'false' otherwise.
     */
    INLINE bool next();
    
    /**
     * Equality operator.
     *
     * @param other the scanner to compare with (must be defined in
     * the same set).
     *
     * @return 'true' if their current positions coincide.
     */
    INLINE bool operator==( const CubicalSpaceScanner & other ) const;
    
    /**
     * Inequality operator.
     *
     * @param other the scanner to compare with (must be defined in
     * the same set).
     *
     * @return 'true' if their current positions differs.
     */
    INLINE bool operator!=( const CubicalSpaceScanner & other ) const;
    
    /**
     * Less operator.
     *
     * @param other the scanner to compare with (must be defined in
     * the same set).
     *
     * @return 'true' if the current position of 'this' is before
     * the current position of [other].
     */
    INLINE bool operator<( const CubicalSpaceScanner & other ) const;
    
  private:
    /**
     * The space in which lies the cubical cell.
     */
    const CubicalSpace* m_space;
    
    /**
     * The current position of the scanner.
     */
    CubicalCell m_cell;

    /**
     * The end position of the scanner.
     */
    CubicalCell m_stop;
  };
  


  /**
   * Overloads 'operator<<' for displaying objects of class 'CubicalSpace'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'CubicalSpace' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const CubicalSpace & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/CubicalSpace.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CubicalSpace_h

#undef CubicalSpace_RECURSES
#endif // else defined(CubicalSpace_RECURSES)
