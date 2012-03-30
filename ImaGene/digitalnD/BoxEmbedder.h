/** @file BoxEmbedder.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : BoxEmbedder.h
//
// Creation : 2008/03/19
//
// Version : 2008/03/19
//
// Author : JOL
//
// Summary : Header file for files BoxEmbedder.ih and BoxEmbedder.cxx
//
// History :
//	2008/03/19 : ?Name? : ?What?
//
// Rcs Id : "@(#)class BoxEmbedder declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(BoxEmbedder_RECURSES)
#error Recursive header files inclusion detected in BoxEmbedder.h
#else // defined(BoxEmbedder_RECURSES)
#define BoxEmbedder_RECURSES

#if !defined BoxEmbedder_h
#define BoxEmbedder_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/Embedder.h"
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
  // class BoxEmbedder
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'BoxEmbedder' <p> Aim: A concrete class that
   * embeds cells into the Euclidean space so that to sample regularly
   * a euclidean "box". It aims at guaranteeing that contiguous boxes have
   * contiguous coordinates.
   */
  class BoxEmbedder : public Embedder
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    INLINE virtual ~BoxEmbedder();

    /**
     * Constructor.
     * The embedder is not valid yet.
     * @see init
     */
    INLINE BoxEmbedder();
    
    /**
     * Constructor. By default, the box is (0,...,0)->(1,...,1).
     *
     * @param space the digital space where the cells are defined.
     */
    INLINE BoxEmbedder( const KnSpace* space );

    /**
     * Constructor. 
     *
     * @param space the digital space where the cells are defined.
     *
     * @param x1 the lower-bound of the coordinates of the embedding
     * box.
     *
     * @param x2 when [dh] is 'false', it is the upper-bound of the
     * coordinates of the embedding box, otherwise it is the vector
     * representing the grid step of the embedding of [space].
     *
     * @param dh defines the initialization mode.
     */
    INLINE BoxEmbedder( const KnSpace* space,
			const float* x1, 
			const float* x2,
			bool dh = false );
    
    /**
     * @return a clone of 'this'.
     */
    INLINE virtual Embedder* clone() const;

    /**
     * @return the lower bound for the coordinates in the box.
     */
    INLINE const float* lowerBound() const;

    /**
     * @return the upper bound for the coordinates in the box.
     */
    INLINE const float* upperBound() const;

    /**
     * @return the grid step in the box.
     */
    INLINE const float* gridStep() const;

    /**
     * @param x (updated) the coordinates are translated in the
     * adjacent box along direction [i], positively or negatively
     * according to [pos].
     *
     * @param i the coordinate axis where the adjacent box is taken.
     *
     * @param pos a boolean telling whether the adjacent box is
     * further positively along the coordinate axis or negatively.
     */
    INLINE void shiftToAdjacentBox( float* x, uint i, bool pos ) const;

    // ----------------------- Embedding services ----------------------------
  public:
    
    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     *
     * @param p any unsigned cell.
     *
     * @param coords (returns) the coordinates of [p] when embedded in
     * the space.
     */
    virtual void uembed( Kn_uid p, float* coords ) const;
    
    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     *
     * @param p any signed cell.
     *
     * @param coords (returns) the coordinates of [p] when embedded in
     * the space.
     */
    virtual void sembed( Kn_sid p, float* coords ) const;
    
    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     *
     * @param p any unsigned cell.
     *
     * @param coords (returns) the coordinates of [p] when embedded in
     * the space.
     */
    virtual void uembed( Kn_uid p, Vector & coords ) const;

    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     *
     * @param p any signed cell.
     *
     * @param coords (returns) the coordinates of [p] when embedded in
     * the space.
     */
    virtual void sembed( Kn_sid p, Vector & coords ) const;


    // ----------------------- Other embedding services -----------------------
  public:
    
    /**
     * Embeds the vector [p] in the Euclidean space as a point.  By
     * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
     * ), coords ).
     *
     * @param p any vector expressed in the discrete space.  
     *
     * @param coords (returns) the coordinates of [p] when embedded in
     * the space.
     */
    virtual void embedVector( const float* p, float* coords ) const;
    
    /**
     * Embeds the vector [p] in the Euclidean space as a point.  By
     * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
     * ), coords ).
     *
     * @param p any vector expressed in the discrete space.  
     *
     * @param coords (returns) the coordinates of [p] when embedded in
     * the space.
     */
    virtual void embedVector( const Vector & p, Vector & coords ) const;

  
    // ----------------------- inverse embedding services ---------------------
  public:
    
    /**
     * @param coords any coordinates in the space.
     * @return the unsigned spel whose embedding is the closest to [coords].
     */
    virtual Kn_uid uinverseSpel( const float* coords ) const;
    
    /**
     * @param coords any coordinates in the space.
     * @return the unsigned spel whose embedding is the closest to [coords].
     */
    virtual Kn_uid uinverseSpel( const Vector & coords ) const;
    


    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * lower-bound for the coordinates of the box.
     */
    float* m_x1;

    /**
     * upper-bound for the coordinates of the box.
     */
    float* m_x2;

    /**
     * grid step of the box.
     */
    float* m_dh;


    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE BoxEmbedder( const BoxEmbedder & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE BoxEmbedder & operator=( const BoxEmbedder & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/BoxEmbedder.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined BoxEmbedder_h

#undef BoxEmbedder_RECURSES
#endif // else defined(BoxEmbedder_RECURSES)
