/** @file C4CGeometry.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CGeometry.h
//
// Creation : 2003/02/11
//
// Version : 2003/02/11
//
// Author : JOL
//
// Summary : Header file for files C4CGeometry.ih and C4CGeometry.cxx
//
// History :
//	2003/02/11 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CGeometry declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CGeometry_RECURSES)
#error Recursive header files inclusion detected in C4CGeometry.h
#else // defined(C4CGeometry_RECURSES)
#define C4CGeometry_RECURSES

#if !defined C4CGeometry_h
#define C4CGeometry_h

//////////////////////////////////////////////////////////////////////////////
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/dgeometry2d/DLine.h"
#include "ImaGene/dgeometry2d/C4CSegment.h"
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

  class C4CIterator;

///////////////////////////////////////////////////////////////////////////////
// class C4CGeometry
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'C4CGeometry' <p>
 * Aim: A utility class to perform basic discrete geometry. Provides algorithm
 * that applies discrete geometry tools to digital surface.
 */
class C4CGeometry
{
  // ------------------------- public types -----------------------------------
public:

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~C4CGeometry();

  /**
   * Constructor.
   */
  C4CGeometry();

  // ------------------------- plane to space services ------------------------
public:

  /**
   * Projects the plane vector [v2] into the space assuming [x_coord] was the
   * x-coordinate in the plane and [y_coord] the y-coordinate.
   * @param v2 any 2D vector.
   * @param vn (returns) a n-dimensional vector.
   * @param x_coord the coordinate of the space used for defining x-axis.
   * @param y_coord the coordinate of the space used for defining y-axis.
   */
  /*
  static void plane2Space( const Vector2i & v2, Vector & vn, 
			   uint x_coord, uint y_coord );
  */

  // ------------------------- Tangent services ------------------------------
public:


  /**
   * Computes the discrete tangent on a 4-connected contour. The contour
   * is defined through a forward iterator [fwd] and a backward iteraror [bwd].
   * Uses Vialard and Debled's algorithm of discrete line recognition.
   * @param fwd any 4-connected contour iterator.
   * @param bwd any 4-connected contour iterator (generally the same).
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
   * @return the digital line tangent to the contour.
   * @deprecated Better use 'symmetricTangent'.
   * @see symmetricTangent
   */
  static DLine tangentLine( C4CIterator & fwd,
			    C4CIterator & bwd, 
			    uint max_size = 0 );

  /**
   * Computes the curvature at the given position on a 4-connected
   * contour. The contour is defined through a forward iterator [fwd]
   * and a backward iteraror [bwd].  Uses Vialard and Debled's
   * algorithm of discrete line recognition.
   *
   * @param fwd any 4-connected contour iterator.
   * @param bwd any 4-connected contour iterator (generally the same).
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
   * @return the curvature at the given contour element.
   */
  static float curvatureByAngleVariation( C4CIterator & fwd,
					  C4CIterator & bwd, 
					  uint max_size = 0 );
  

  /**
   * Computes the curvature at the given position on a 4-connected
   * contour. The contour is defined through a forward iterator [fwd]
   * and a backward iteraror [bwd].  Uses Vialard and Debled's
   * algorithm of discrete line recognition.
   *
   * @param center any 4-connected contour iterator.
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
   * @return the curvature at the given contour element.
   */
  static float curvatureBySymAngleVariation( C4CIterator & center,
					     uint max_size = 0 );

  
  /**
   * Computes the positive discrete half-tangent on a 4-connected
   * contour. The contour is defined through a forward iterator [fwd].
   * Uses Vialard and Debled's algorithm
   * of discrete line recognition.
   *
   * @param fwd any 4-connected contour iterator.
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
   * @return the digital segment representing the positive half-tangent.
   */
  static C4CSegment positiveHalfTangent( C4CIterator & fwd,
				    uint max_size );

  /**
   * Computes the negative discrete half-tangent on a 4-connected
   * contour. The contour is defined through a backward iterator [bwd].
   * Uses Vialard and Debled's algorithm
   * of discrete line recognition.
   *
   * @param fwd any 4-connected contour iterator.
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
   * @return the digital segment representing the negative half-tangent.
   */
  static C4CSegment negativeHalfTangent( C4CIterator & bwd,
				    uint max_size );


  /**
   * Computes the symmetric discrete tangent on a 4-connected
   * contour. The contour is defined through a forward iterator [fwd]
   * and a backward iteraror [bwd].  Uses Vialard and Debled's algorithm
   * of discrete line recognition.
   *
   * @param fwd any 4-connected contour iterator.
   * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
   * @return the digital segment tangent to the contour.
   * @todo Cannot handle open contours.
   */
  static C4CSegment symmetricTangent( C4CIterator & fwd,
				 C4CIterator & bwd, 
				 uint max_size );

  /**
   * Computes the extended discrete tangent on a 4-connected contour. It
   * is defined as the longest discrete segment including the symmetric
   * tangent at the given starting point, extended to the front or to
   * the back only if there is no ambiguity.
   *
   * The contour is defined through a forward iterator [fwd] and a
   * backward iteraror [bwd].  Uses Vialard and Debled's algorithm of
   * discrete line recognition.
   *
   * @param fwd any 4-connected contour iterator.
   * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
   * @return the digital segment tangent to [b].
   */
  static C4CSegment extendedTangent( C4CIterator & fwd,
				C4CIterator & bwd, 
				uint max_size );


  /**
   * Computes the maximal front tangent on a 4-connected contour (ie the
   * segment [B(F(fwd)),F(fwd)]). The contour is defined through a
   * forward iterator [fwd] and a backward iteraror [bwd].  Uses Vialard
   * and Debled's algorithm of discrete line recognition.
   *
   * @param fwd any 4-connected contour iterator.
   * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
   * @return the digital segment tangent to [b].
   */
  static C4CSegment maximalFrontTangent( C4CIterator & fwd,
				    C4CIterator & bwd, 
				    uint max_size );


  /**
   * Computes the maximal back tangent on a 4-connected contour (ie the
   * segment [B(bwd),F(B(bwd))]). The contour is defined through a
   * forward iterator [fwd] and a backward iteraror [bwd].  Uses Vialard
   * and Debled's algorithm of discrete line recognition.
   *
   * @param fwd any 4-connected contour iterator.
   * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
   * @return the digital segment tangent to [b].
   */
  static C4CSegment maximalBackTangent( C4CIterator & fwd,
				   C4CIterator & bwd, 
				   uint max_size );


  /**
   * Computes the common tangent on a 4-connected contour (ie the
   * segment [B(F(fwd)),F(B(bwd))]). The contour is defined through a
   * forward iterator [fwd] and a backward iteraror [bwd].  Uses Vialard
   * and Debled's algorithm of discrete line recognition.
   *
   * @param fwd any 4-connected contour iterator.
   * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
   * @return the digital segment tangent to [b].
   *
   * @todo Cannot handle open contours.
   */
  static C4CSegment commonTangent( C4CIterator & fwd,
			      C4CIterator & bwd, 
			      uint max_size );


  /**
   * Computes the longest possible discrete segment on a 4-connected
   * contour in the positive direction (front). The contour is defined
   * through a forward iterator [fwd].  Uses Vialard and Debled's
   * algorithm of discrete line recognition.  Useful to compute
   * positive half tangents.
   *
   * @param segment a segment such that [fwd] is located on its front. Returns the longest segment obtained by extending its front.
   * @param fwd (updated) any 4-connected contour iterator.
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
   */
  static void longestPositiveSegment( C4CSegment & segment,
				      C4CIterator & fwd,
				      uint max_size );

  /**
   * Computes the longest possible discrete segment on a 4-connected
   * contour in the negative direction (back). The contour is defined
   * through a backward iterator [bwd].  Uses Vialard and Debled's
   * algorithm of discrete line recognition.  Useful to compute
   * negative half tangents.
   *
   * @param segment a segment such that [bwd] is located on its back. Returns the longest segment obtained by extending its back.
   * @param bwd (updated) any 4-connected contour iterator.
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
   */
  static void longestNegativeSegment( C4CSegment & segment,
				      C4CIterator & bwd,
				      uint max_size );

  /**
   * Computes the longest possible symmetric discrete segment on a
   * 4-connected contour. The contour is defined through a forward
   * iterator [fwd] and a backward iterator [bwd].  Uses Vialard and
   * Debled's algorithm of discrete line recognition.  Useful to
   * compute symmetric tangents.
   *
   * @param segment a segment such that [fwd] is located on its front and [bwd] is located on its back. Returns the longest segment obtained by extending its front and back.
   * @param fwd (updated) any 4-connected contour iterator.
   * @param bwd (updated) any 4-connected contour iterator.
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
   *
   * @todo Cannot handle open contours.
   */
  static void longestSymmetricSegment( C4CSegment & segment,
				       C4CIterator & fwd,
				       C4CIterator & bwd,
				       uint max_size );

  /**
   * Computes the longest possible extended discrete segment on a
   * 4-connected contour. The contour is defined through a forward
   * iterator [fwd] and a backward iterator [bwd].  Uses Vialard and
   * Debled's algorithm of discrete line recognition.  Useful to
   * compute extended tangents (a variant of symmetric tangents that
   * can be longer in some cases).
   *
   * @param segment a segment such that [fwd] is located on its front and [bwd] is located on its back. Returns the longest extended segment obtained by extending its front and back.
   * @param fwd (updated) any 4-connected contour iterator.
   * @param bwd (updated) any 4-connected contour iterator.
   * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
   * @deprecated Use extendedTangent instead.
   * @see extendedTangent
   *
   * @todo Cannot handle open contours.
   */
  static void longestExtendedSegment( C4CSegment & segment,
				      C4CIterator & fwd,
				      C4CIterator & bwd,
				      uint max_size );


  /**
   * From an already computed symmetric tangent (longest symmetric
   * tangent), computes the symmetric tangent on the next element of
   * the contour. The current element is given by [current] and the
   * extremities of the current symmetric tangent are given by
   * [forward] and [backward]. Use Debled's algorithm of discrete line
   * recognition.  Useful for incremental computation of symmetric
   * tangents.
   *
   * @param segment the symmetric tangent around [current].
   *
   * @param current (updated) a 4-connected contour iterator pointing
   * on the center element of [segment].
   *
   * @param fwd (updated) a 4-connected contour iterator pointing on
   * the last element to the front of [segment].
   *
   * @param bwd (updated) a 4-connected contour iterator pointing on
   * the last element to the back of [segment].
   *
   * @todo Cannot handle open contours.
   */
  static void nextSymmetricSegment( C4CSegment & segment,
				    C4CIterator & current,
				    C4CIterator & fwd,
				    C4CIterator & bwd );


  // -------------------- Maximal segments services ---------------------------
public:

  /**
   * Given a maximal segment [max_segment], returns the next one in front of
   * it. [fwd] and [bwd] are updated to point on [next_max_segment].
   *
   * @param max_segment any maximal segment.
   * @param next_max_segment (returns) the next maximal segment.
   * @param fwd an iterator that must point to the front of [max_segment].
   * @param bwd an iterator that must point to the back of [max_segment].
   *
   * @param 'true' if there is another maximal segment before the
   * current surfel, 'false' if there is no next maximal tangent that
   * contains the current surfel (perhaps there is one after the
   * current surfel or we have reached and extremity of the
   * contour). The next maximal tangent is then non valid.
   */
  static bool nextMaximalSegment( const C4CSegment & max_segment,
				  C4CSegment & next_max_segment,
				  C4CIterator & fwd,
				  C4CIterator & bwd );
  
  /**
   * Computes the maximal segments around surfel pointed by [it]. These
   * segments range from F(B(it)) to B(F(it)).
   *
   * @param it any 4-connected contour iterator.
   * @param m_segments an allocated array of segments of size [m]
   * @param j the index where to compute the first segment.
   * @param k (returns) the index just after the last computed segment.
   * @param m the size of the array [m_segments].
   * @return 'false' if the array m_segments was not big enough, 'true' otherwise.
   */
  static bool maximalSegments( const C4CIterator & it,
			       C4CSegment* m_segments, 
			       uint j, uint & k, uint m );
			       
			       
/**
 * Computes the maximal segments around surfel pointed by [it]. These
 * segments range from F(B(it)) to B(F(it)).
 *
 * @param it any 4-connected contour iterator.
 * @param m_segments an allocated array of segments of size [m]
 * @param j the index where to compute the first segment.
 * @param k (returns) the index just after the last computed segment.
 * @param m the size of the array [m_segments].
 * @param tabIteratorFwd the table of the foward iterator of each segment.
 * @param tabIteratorBwd the table of the backward iterator of each segment.
 * @return 'false' if the array m_segments was not big enough, 'true' otherwise.
 */
static bool maximalSegmentsWithMemory( const C4CIterator & it,
			      C4CSegment* m_segments, 
			      C4CIterator ** tabIteratorFwd,
			      C4CIterator ** tabIteratorBwd,
			      uint j, uint & k, uint m );


  // ------------------------- Interface --------------------------------------
public:

  /**
   * Checks the validity/consistency of the object.
   * @return 'true' if the object is valid, 'false' otherwise.
   */
  bool OK() const;
  
  // ------------------------- Datas ------------------------------------------
private:

  // ------------------------- Hidden services --------------------------------
protected:

private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE C4CGeometry( const C4CGeometry & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE C4CGeometry & operator=( const C4CGeometry & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/C4CGeometry.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CGeometry_h

#undef C4CGeometry_RECURSES
#endif // else defined(C4CGeometry_RECURSES)
