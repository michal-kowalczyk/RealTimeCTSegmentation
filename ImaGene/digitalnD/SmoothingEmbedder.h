/** @file SmoothingEmbedder.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : SmoothingEmbedder.h
//
// Creation : 2004/02/04
//
// Version : 2004/02/04
//
// Author : JOL
//
// Summary : Header file for files SmoothingEmbedder.ih and SmoothingEmbedder.cxx
//
// History :
//	2004/02/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class SmoothingEmbedder declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(SmoothingEmbedder_RECURSES)
#error Recursive header files inclusion detected in SmoothingEmbedder.h
#else // defined(SmoothingEmbedder_RECURSES)
#define SmoothingEmbedder_RECURSES

#if !defined SmoothingEmbedder_h
#define SmoothingEmbedder_h

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

  class DigitalSurfaceGeometry;
  
///////////////////////////////////////////////////////////////////////////////
// class SmoothingEmbedder
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'SmoothingEmbedder' <p>
 *
 * Aim: A concrete class that embeds only surfels into the Euclidean
 * space by projecting them onto their mid-tangent plane.
 */
class SmoothingEmbedder : public Embedder
{
  // ------------------------- Standard services ------------------------------
public:

  /**
   * Destructor. Does nothing.
   */
  INLINE virtual ~SmoothingEmbedder();

  /**
   * Constructor.
   * The embedder is not valid yet.
   * @see init
   */
  INLINE SmoothingEmbedder();
  
  /**
   * Constructor.
   * @param geom_context the geometry context used for the smoothing.
   */
  INLINE SmoothingEmbedder( DigitalSurfaceGeometry* geom_context );

  /**
   * Sets the geometry (and the space) used for the smoothing.
   * @param geom_context the geometry context used for the smoothing.
   */
  INLINE void setGeometry( DigitalSurfaceGeometry* geom_context );  

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
   * @param p any vector expressed in the discrete space.  
   * @param coords (returns) the coordinates of [p] when embedded in the space.
   */
  virtual void embedVector( const float* p, float* coords ) const;

  /**
   * Embeds the vector [p] in the Euclidean space as a point.  By
   * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
   * ), coords ).
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
   * NB: Same as CentroidEmbedder::uinverseSpel.
   * @see CentroidEmbedder::uinverseSpel
   */
  virtual Kn_uid uinverseSpel( const float* coords ) const;
  
  /**
   * @param coords any coordinates in the space.
   * @return the unsigned spel whose embedding is the closest to [coords].
   * NB: Same as CentroidEmbedder::uinverseSpel.
   * @see CentroidEmbedder::uinverseSpel
   */
  virtual Kn_uid uinverseSpel( const Vector & coords ) const;


  // ------------------------- datas --------------------------------------
private:
  
  /**
   * The geometry context used for projecting cells onto their tangent plane.
   */
  DigitalSurfaceGeometry* m_geom_ctxt;
  
  // ------------------------- Internals --------------------------------------
private:
  
};

} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "SmoothingEmbedder.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined SmoothingEmbedder_h

#undef SmoothingEmbedder_RECURSES
#endif // else defined(SmoothingEmbedder_RECURSES)
