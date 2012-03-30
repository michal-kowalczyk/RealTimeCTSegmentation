/** @file GridEmbedder.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : GridEmbedder.h
//
// Creation : 2004/02/04
//
// Version : 2004/02/04
//
// Author : JOL
//
// Summary : Header file for files GridEmbedder.ih and GridEmbedder.cxx
//
// History :
//	2004/02/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class GridEmbedder declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(GridEmbedder_RECURSES)
#error Recursive header files inclusion detected in GridEmbedder.h
#else // defined(GridEmbedder_RECURSES)
#define GridEmbedder_RECURSES

#if !defined GridEmbedder_h
#define GridEmbedder_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/Embedder.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class GridEmbedder
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'GridEmbedder' <p> Aim: A concrete class that
 * embeds cells into the Euclidean space with their natural centroid
 * (deduced from their coordinates). It also applies an affine
 * transformation by first translating the coordinates (a new center
 * for the space is given) and then applying a resolution scale (dh)
 * onto the result (ie. each cell is separated from an adjacent one by
 * a distance dh).
 */
class GridEmbedder : public Embedder
{
  // ------------------------- Standard services ------------------------------
public:

  /**
   * Destructor. Does nothing.
   */
  INLINE virtual ~GridEmbedder();

  /**
   * Constructor.
   * The embedder is not valid yet.
   * @see init
   */
  INLINE GridEmbedder();
  
  /**
   * Constructor.
   * @param space the digital space where the cells are defined.
   */
  INLINE GridEmbedder( const KnSpace* space );

  /**
   * Copy constructor.
   * @param other the other grid embedder to copy
   */
  INLINE GridEmbedder( const GridEmbedder & other );

  /**
   * Sets the center of the space.
   * @param xcenter a vector with same dimension as the digital space.
   */
  INLINE void setCenter( const Vector & xcenter );

  /**
   * Sets the resolution of the space.
   * @param scale the resolution of the space (the closer to zero, the finer is the grid).
   */
  INLINE void setScale( float scale );
  

  /**
   * @return a clone of 'this'.
   */
  INLINE virtual Embedder* clone() const;



  // ----------------------- Embedding services ------------------------------
public:

  /**
   * Embeds the cell [p] in the Euclidean space as a point.
   * @param p any unsigned cell.
   * @param coords (returns) the coordinates of [p] when embedded in the space.
   */
  virtual void uembed( Kn_uid p, float* coords ) const;

  /**
   * Embeds the cell [p] in the Euclidean space as a point.
   * @param p any signed cell.
   * @param coords (returns) the coordinates of [p] when embedded in the space.
   */
  virtual void sembed( Kn_sid p, float* coords ) const;

  /**
   * Embeds the cell [p] in the Euclidean space as a point.
   * @param p any unsigned cell.
   * @param coords (returns) the coordinates of [p] when embedded in the space.
   * @see uembed( Kn_uid p, float* coords )
   * NB: calls 'uembed', could be optimized.
   */
  virtual void uembed( Kn_uid p, Vector & coords ) const;

  /**
   * Embeds the cell [p] in the Euclidean space as a point.
   * @param p any signed cell.
   * @param coords (returns) the coordinates of [p] when embedded in the space.
   * @see sembed( Kn_uid p, float* coords )
   * NB: calls 'sembed', could be optimized.
   */
  virtual void sembed( Kn_sid p, Vector & coords ) const;

  // ----------------------- Other embedding services -----------------------
public:
  
  /**
   * Embeds the vector [p] in the Euclidean space as a point.  By
   * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
   * ), coords ).
   *
   *
   * @param p any vector expressed in the discrete space.  
   * @param coords (returns) the coordinates of [p] when embedded in the space.
   */
  virtual void embedVector( const float* p, float* coords ) const;

  /**
   * Embeds the vector [p] in the Euclidean space as a point.  By
   * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
   * ), coords ).
   *
   *
   * @param p any vector expressed in the discrete space.  
   * @param coords (returns) the coordinates of [p] when embedded in the space.
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


  // ------------------------- datas --------------------------------------
private:

  /**
   * The coordinates of the center.
   */
  Vector* m_center;
  
  /**
   * The resolution of the grid (or scale or step).
   */
  float m_dh;
  
  
  // ------------------------- Internals --------------------------------------
private:

  /**
   * Avoid allocation/reallocation of vectors for embedding.
   */
  Vector* m_tmp;
  
};

} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "GridEmbedder.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined GridEmbedder_h

#undef GridEmbedder_RECURSES
#endif // else defined(GridEmbedder_RECURSES)
