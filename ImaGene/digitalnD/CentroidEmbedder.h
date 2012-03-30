/** @file CentroidEmbedder.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : CentroidEmbedder.h
//
// Creation : 2004/02/04
//
// Version : 2004/02/04
//
// Author : JOL
//
// Summary : Header file for files CentroidEmbedder.ih and CentroidEmbedder.cxx
//
// History :
//	2004/02/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class CentroidEmbedder declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(CentroidEmbedder_RECURSES)
#error Recursive header files inclusion detected in CentroidEmbedder.h
#else // defined(CentroidEmbedder_RECURSES)
#define CentroidEmbedder_RECURSES

#if !defined CentroidEmbedder_h
#define CentroidEmbedder_h

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
// class CentroidEmbedder
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'CentroidEmbedder' <p>
 * Aim: A concrete class that embeds cells into the Euclidean space with their
 * natural centroid (deduced from their coordinates).
 */
class CentroidEmbedder : public Embedder
{
  // ------------------------- Standard services ------------------------------
public:

  /**
   * Destructor. Does nothing.
   */
  INLINE virtual ~CentroidEmbedder();

  /**
   * Constructor.
   * The embedder is not valid yet.
   * @see init
   */
  INLINE CentroidEmbedder();
  
  /**
   * Constructor.
   * @param space the digital space where the cells are defined.
   */
  INLINE CentroidEmbedder( const KnSpace* space );

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
  
  // ------------------------- Internals --------------------------------------
private:
  
};

} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "CentroidEmbedder.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CentroidEmbedder_h

#undef CentroidEmbedder_RECURSES
#endif // else defined(CentroidEmbedder_RECURSES)
