/** @file C4CTangentialCover.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CTangentialCover.h
//
// Creation : 2004/11/10
//
// Version : 2004/11/10
//
// Author : JOL
//
// Summary : Header file for files C4CTangentialCover.ih and C4CTangentialCover.cxx
//
// History :
//	2004/11/10 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CTangentialCover declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CTangentialCover_RECURSES)
#error Recursive header files inclusion detected in C4CTangentialCover.h
#else // defined(C4CTangentialCover_RECURSES)
#define C4CTangentialCover_RECURSES

#if !defined C4CTangentialCover_h
#define C4CTangentialCover_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/dgeometry2d/C4CSegment.h"
#include "ImaGene/dgeometry2d/C4CIterator.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
///////////////////////////////////////////////////////////////////////////////
// class C4CTangentialCover
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'C4CTangentialCover' <p> Aim: A tangential
 * cover is the set of maximal segments associated with a digital
 * curve.
 */
class C4CTangentialCover
{
public:


  /**
   * Represents one maximal segment on a contour. We always have
   * [back_surfel_idx] < [front_surfel_idx] (in the modulo sense).
   */
  struct MaximalSegment
  {
    /** 
     * Digital straight segment (DSS) corresponding to this maximal
     * segment expressed in the frame of the front surfel.
     */
    C4CSegment dss;

    /**
     * Index of the surfel at the back of the MS (index is always positive).
     */
    uint back_surfel_idx;

    /**
     * Index of the surfel at the front of the MS (index is always positive).
     */
    uint front_surfel_idx;
  };


  /**
   * Represents the first maximal segment and the maximal segment
   * after the last for the surfel with index [idx_surfel] in the
   * tangential cover. [end_ms] can be smaller than [begin_ms], since
   * indices are modulo.
   *
   * @see beginSMS,nextSMS,previousSMS
   */
  struct SurfelMaximalSegments
  {
    /**
     * Index of the surfel of interest.
     */
    uint idx_surfel;
    /**
     * Index of the first maximal segment containing [idx_surfel].
     */
    uint begin_ms; 
    /**
     * Index of the maximal segment after the last containing [idx_surfel].
     */
    uint end_ms;   
  };
  
  
   /**
   * Represents common parts of the tangential cover (ie. connected
   * places where surfels have the same maximal segments). Stores the
   * first maximal segment and the maximal segment after the last and
   * also the back surfel and the front surfel of the
   * interval. Indices of surfels may be negative. [end_ms] can be
   * smaller than [begin_ms], since indices are modulo.
   *
   * @see beginCP,nextCP
   */
  struct CommonPart
  {
    /**
     * Index of the surfel at the back of the common part (index is
     * always positive).
     */
    uint back_surfel_idx;

    /**
     * Index of the surfel after the front of the common part (index is
     * always positive). 
     */
    uint after_front_surfel_idx;

    /**
     * Index of the first maximal segment in the common part.
     */
    uint begin_ms; 
    /**
     * Index of the maximal segment after the last in the common part.
     */
    uint end_ms;   

  };


 public:
  // ------------------------- internal classes services ------------------
  
  class TangentEstimator
    {

    };

  /**
   * A (derivable) class to specify what kind of estimators are
   * used to compute the geometry of the tangential cover.
   * 
   */
  class GeometricEstimator 
    {
      /** 
       * Constructs a geometric estimator based on tangential
       * cover analysis.
       * 
       * @param tcover the tangential cover of the digital curve.
       */
      GeometricEstimator( const C4CTangentialCover & cover );
      
      
    };
  


  // ------------------------- Static services ------------------------------
public:

  /** 
   * Given an iterator [it] on a digital contour, computes the number of
   * maximal segments covering the contour. Complexity is O(nbs) if nbs
   * is the number of surfels/points of the contour.
   * 
   * @param it an iterator on the contour. (returned as the first
   * surfel of the first maximal segment).
   * 
   * @return the number of maximal segments.
   */
  static int computeNumberMS( C4CIterator & it );


  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~C4CTangentialCover();

  /**
   * Constructor. Does nothing. The object is not valid.
   */
  C4CTangentialCover();

  /**
   * Computes the tangential cover.
   *
   * @param it an iterator on the digital curve. (returns) It points on
   * the surfel with index 0 in the tangential cover.
   *
   * @param nbmax_ms the maximal number of maximal segments on this
   * curve. If 0, computes the number of maximal segments before
   * proceeding, defaut is 0.
   *
   * @return 'true' if the initialization went ok, 'false' otherwise
   * (e.g., nbmax_ms is too small).
   */
  bool init( C4CIterator & it,  uint nbmax_ms = 0 );
  
  /**
   * Resets the tangential cover as if it was just instantiated.
   */
  void reset();

  // -------------------- Tangential cover services ---------------------------
public:

  /** 
   * @return 'true' if the contour is open, 'false' otherwise.
   */
  INLINE bool isContourOpen() const;

  /** 
   * @return a dyn. alloc. iterator on the first surfel of the contour.
   */
  INLINE C4CIterator* beginSurfel() const;

  /** 
   * @return a dyn. alloc. iterator on the last surfel of the
   * contour. If the contour is closed, same as 'beginSurfel'.
   *
   * @see beginSurfel
   */
  INLINE C4CIterator* endSurfel() const;

  
  /**
   * @return the number of maximal segments of the tangential cover.
   */
  INLINE uint nbMaximalSegments() const;

  /**
   * @return the number of surfels of the tangential cover.
   */
  INLINE uint nbSurfels() const;
  
  /**
   * @param i the index of the maximal segment.
   * @return the maximal segment of index [i].
   */
  INLINE const MaximalSegment & getMaximalSegment( uint i ) const;

  /**
   * @param i the index of the maximal segment.
   *
   * @return a dyn. alloc. iterator lying on the front surfel of the
   * [i]-th maximal segment.
   *
   * Nb: linear time algorithm.
   */
  C4CIterator* getSurfelFront( uint i ) const;

  /**
   * @param i the index of the maximal segment.
   *
   * @return a dyn. alloc. iterator lying on the front surfel of the
   * [i]-th maximal segment.
   *
   * Nb: linear time algorithm.
   */
  C4CIterator* getSurfelBack( uint i ) const;

  /**
   * @param idx a surfel index.
   * @return the maximal segments of the surfel of index [idx].
   */
  SurfelMaximalSegments beginSMS( uint idx ) const;

  /**
   * Given the maximal segments of a surfel given by [sms], computes
   * the maximal segments of the next surfel on the contour.
   *
   * @param sms the maximal segments of a surfel, (returns) the maximal segments of the surfel with an incremented index.
   * @return 'true' if there was a next surfel, false otherwise.
   */
  bool nextSMS( SurfelMaximalSegments & sms ) const;

  /**
   * Given the maximal segments of a surfel given by [sms], computes
   * the maximal segments of the previous surfel on the contour.
   *
   * @param sms the maximal segments of a surfel, (returns) the maximal segments of the surfel with a decremented index.
   * @return 'true' if there was a previous surfel, false otherwise.
   */
  bool previousSMS( SurfelMaximalSegments & sms ) const;
  

  /**
   * @return the first common part of the tangential cover.
   */
  CommonPart beginCP() const;

  /**
   * Given a common part specified by [cp], computes
   * the next common part of the tangential cover.
   *
   * @param cp a common part of the tangential cover, (returns) the one adjacent to it at the front.
   */
  void nextCP( CommonPart & cp ) const;


  // ------------------------- Interface --------------------------------------
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
  
  // ------------------------- Datas ------------------------------------------
private:

  /**
   * Number of maximal segments of the tangential cover.
   */
  uint m_nb_max_segments;

  /**
   * Number of surfels of the digital curve represented by the
   * tangential cover.
   */
  uint m_nb_surfels;

  /**
   * When 'true', the curve is open and the index of the first surfel
   * is on one end.
   */
  bool m_is_curve_open;
  
  /**
   * The array of maximal segments of the curve (owned, dyn. alloc.).
   */
  MaximalSegment* m_segments;

  /**
   * A clone of the iterator on the surfel with idx = 0.
   */
  C4CIterator* m_begin_it;

  /**
   * A clone of the iterator on the surfel with idx = m_nb_surfels - 1
   * if the contour is open, or same as 'm_begin_it' if the contour is
   * closed.
   */
  C4CIterator* m_end_it;

  // ------------------------- Hidden services --------------------------------
protected:
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE C4CTangentialCover( const C4CTangentialCover & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE C4CTangentialCover & operator=( const C4CTangentialCover & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'C4CTangentialCover'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'C4CTangentialCover' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const C4CTangentialCover & that_object_to_display );


} // namespace ImaGene 


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/C4CTangentialCover.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CTangentialCover_h

#undef C4CTangentialCover_RECURSES
#endif // else defined(C4CTangentialCover_RECURSES)
