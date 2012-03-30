/** @file C4CSegmentPencil.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CSegmentPencil.h
//
// Creation : 2004/08/11
//
// Version : 2004/08/11
//
// Author : JOL
//
// Summary : Header file for files C4CSegmentPencil.ih and C4CSegmentPencil.cxx
//
// History :
//	2004/08/11 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CSegmentPencil declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CSegmentPencil_RECURSES)
#error Recursive header files inclusion detected in C4CSegmentPencil.h
#else // defined(C4CSegmentPencil_RECURSES)
#define C4CSegmentPencil_RECURSES

#if !defined C4CSegmentPencil_h
#define C4CSegmentPencil_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
//#include "LinAlg/LinAlg2D/Vector2D.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/mathutils/Functions.h"
#include "ImaGene/dgeometry2d/C4CSegment.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {


///////////////////////////////////////////////////////////////////////////////
// class C4CSegmentPencil
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'C4CSegmentPencil' <p> Aim: Describes a pencil
 * of consecutive segments. These segments are located in the same
 * reference frame and have at least one point in common. These
 * segments are generally supposed to be maximal.
 */
class C4CSegmentPencil
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~C4CSegmentPencil();

  /**
   * Constructor.  The pencil will only reference the array of
   * segments [segments]. The user has to take care of its allocation.
   * @param segments an array of segments
   * @param j the index of the first segment in the pencil.
   * @param k the index after the last segment in the pencil.
   * @param m the modulo for the indices.
   * @param l the interpolation function.
   * @param lp the derivative of the interpolation function.
   */
  C4CSegmentPencil( C4CSegment* segments, uint j, uint k, uint m,
		    const R2RFunction & l, const R2RFunction & lp);

  /**
   * Estimates the orientation of the tangent vector at the discrete
   * contour point [m]. Use an interpolation of the orientation of the
   * maximal segments surrounding the point.
   *
   * @param m any point of the segment expressed in the local reference frame.
   * @return the orientation of the tangent vector at [m] expressed in the local reference frame.
   */
  float angleToX( const Vector2D & m ) const;

    /**
   * Estimates the orientation of the tangent vector at the discrete
   * contour point [m]. Use an interpolation of the orientation of the
   * maximal segments surrounding the point.
   *
   * @param m any point of the segment expressed in the local reference frame.
   * @return the orientation of the tangent vector at [m] expressed in the local reference frame.
   */
  float angleToXMeasure( const Vector2D & m ) const;


  /**
   * Estimates the projection of the discrete contour point [m] onto
   * its real contour. Uses interpolation between regular projection
   * onto each segment.
   *
   * @param m any point of the segment expressed in the local reference frame.
   * @return the continuous analog of m expressed in the local reference frame.
   */
  Vector2D continuousAnalog( const Vector2D & m ) const;

  /**
   * Estimates the first variation of the projection of the discrete
   * contour point [m] onto its real contour. Uses interpolation between
   * regular projection onto each segment.
   *
   * @param m any point of the segment expressed in the local reference frame.
   * @return the derivative of the continuous analog of m expressed in the local reference frame.
   */
  Vector2D continuousAnalogDerivative( const Vector2D & m ) const;
  

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
   * Point toward an array of segments (not owned).
   */
  C4CSegment* m_segments;

  /**
   * The index of the first segment in the pencil.
   */
  uint m_j;

  /**
   * The index after the last segment in the pencil.
   */
  uint m_k;

  /**
   * The modulo for the indices of the pencil segments.
   */
  uint m_modulo;

  /**
   * The function used for the interpolation between segments.
   * The eccentricity is used as an input to this polynomial.
   */
  const R2RFunction & m_bell_fct;

  /**
   * The derivative of the function used for the interpolation
   * between segments.  The eccentricity is used as an input to this
   * polynomial.
   * @see m_bell_fct
   */
  const R2RFunction & m_bell_fct_derivative;


  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden by default (protected to avoid g++ warnings).
   */
  INLINE C4CSegmentPencil();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE C4CSegmentPencil( const C4CSegmentPencil & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE C4CSegmentPencil & operator=( const C4CSegmentPencil & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'C4CSegmentPencil'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'C4CSegmentPencil' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const C4CSegmentPencil & that_object_to_display );

} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "C4CSegmentPencil.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CSegmentPencil_h

#undef C4CSegmentPencil_RECURSES
#endif // else defined(C4CSegmentPencil_RECURSES)
