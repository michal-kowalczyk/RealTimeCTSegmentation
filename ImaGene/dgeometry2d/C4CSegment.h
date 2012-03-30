/** @file C4CSegment.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CSegment.h
//
// Creation : 2003/11/05
//
// Version : 2003/11/05
//
// Author : JOL
//
// Summary : Header file for files C4CSegment.ih and C4CSegment.cxx
//
// History :
//	2003/11/05 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CSegment declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CSegment_RECURSES)
#error Recursive header files inclusion detected in C4CSegment.h
#else // defined(C4CSegment_RECURSES)
#define C4CSegment_RECURSES

#if !defined C4CSegment_h
#define C4CSegment_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
//#include "LinAlg/LinAlg2D/Vector2D.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/VectorUtils.h"
#include "ImaGene/dgeometry2d/DLine.h"
#include "ImaGene/dgeometry2d/C4CIterator.h"
#include "ImaGene/mathutils/PWFAffineFunction.h"
#include "ImaGene/mathutils/MeasureOfStraightLines.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class C4CSegment
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'C4CSegment' <p> Aim: Represents a 2D line
 * segment. A 2D line segment is a 4-connected contour with at least
 * two points. This contour forms a discrete line with characteristics
 * (a,b,mu). If the points have coordinates (x,y), then all the points
 * of the contour verify mu <= a*x-b*y <= mu + |a| + |b| - 1.  A
 * 'C4CSegment' suppose that the interior of the contour is "below" and
 * that it is moving clockwise (to get a direct basis). Points can be
 * added or removed to the "front" (further positively along x-axis)
 * or to the "back" (further negatively along x-axis). The center of
 * the reference frame can be moved along the contour (but not
 * arbitrarily since some reference frames are not valid for some line
 * segments).
 */
class C4CSegment
{

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  INLINE ~C4CSegment();

  /**
   * Default constructor. The segment is NOT valid.
   * @see init
   */
  INLINE C4CSegment();

  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  C4CSegment( const C4CSegment & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  C4CSegment & operator=( const C4CSegment & other );

  /**
   * (Re-)Initializes the line segment as an horizontal edge with
   * first point C_n in (0,0) and second point in (1,0). The
   * "interior" of the contour is to the right (clockwise) or
   * negatively along y axis.
   */
  void init();

  /**
   * @return 'true' if the segment is the trivial (0,1,0) and composed of the
   * two points (0,0) and (1,0). For instance, 'true' after call to 'init'.
   */
  INLINE bool isTrivial() const;

  /**
   * @return 'true' if the segment can be retracted at the back ( C_n
   * != (0,0) ).
   */
  INLINE bool isBackRetractable() const;

  /**
   * @return 'true' if the segment can be retracted at the front ( C'_n
   * != (1,0) ).
   */
  INLINE bool isFrontRetractable() const;

  /**
   * @return the number of points of the segment (at least 2).
   */
  INLINE uint size() const;

  /** 
   * Assuming point (0,0) touching the origin surfel has index 0,
   * returns the relative index on the digital contour of the digital
   * point [v] (which must lie on the segment). It is easy to see that
   * it corresponds to its L1-norm with some sign.
   * 
   * @param v the coordinate of a point on the segment.
   * 
   * @return its index relative to point (0,0), positive for positive x,
   * negative otherwise.
   */
  INLINE int relIndex( const Vector2i & v ) const;


  // ------------------------- Segment services ------------------------------
public:

//   bool belongToSegment(Vector2i & P );
  
    /**
   * Tries to transform the discrete line segment in a thiner one but included, slim it from right leaning points
   *
   * @param dir : the direction vector to consider 
   * @return '1' a thiner segment is computed, '0' impossible in that sense, '2' the computed Bezout vector is not usable
   */
//   uint slimRight( Vector2i & dir );
//     
//    uint slimLeft( Vector2i & dir );

  /**
   * Tries to transform the discrete line segment in a thiner one but included, slim it from upper leaning points
   *
   * @param dir : the direction vector to consider 
   * @return '1' a thiner segment is computed, '0' the origin lies on the selected border and the segment is not thiner, '2' the computed Bezout vector is not usable
   */
//   uint slimUp( Vector2i & dir );
//   
  /**
   * Tries to transform the discrete line segment in a thiner one but included, slim it from lower leaning points
   *
   * @param dir : the direction vector to consider 
   * @return '1' a thiner segment is computed, '0' the origin lies on the selected border and the segment is not thiner, '2' the computed Bezout vector is not usable
   */
//   uint slimLow( Vector2i & dir );

  /**
   * Tries to extend the discrete line segment to the "front"
   * (positively along the contour).
   *
   * @param code 0: no move, 1: turn right (toward interior), 2: move
   * straight ahead (no turn), 3: turn left (toward exterior).
   *
   * @return '0' if this extension was valid and the discrete line
   * segment has been updated, '1' if the extension was not valid
   * because of code incompatibility, '2' if the extension was not
   * valid because it cannot be a digital straight line. In case '!=0'
   * the discrete line segment is unchanged.
   */
  uint extendsFront( uint code );

  /**
   * Tries to extend the discrete line segment to the "back"
   * (negatively along the contour).
   *
   * @param code 0: no move, 1: turn left (toward interior), 2: move
   * straight ahead (no turn), 3: turn right (toward exterior).
   *
   * @return '0' if this extension was valid and the discrete line
   * segment has been updated, '1' if the extension was not valid
   * because of code incompatibility, '2' if the extension was not
   * valid because it cannot be a digital straight line. In case '!=0'
   * the discrete line segment is unchanged.
   */
  uint extendsBack( uint code );

  /**
   * Tries to retract the discrete line segment to the "front"
   * (positively along the contour).
   *
   * @param code the shape of the retraction going from the "front"
   * extremity: 0: no move, 1: turn left (toward interior), 2: move
   * straight ahead (no turn), 3: turn right (toward exterior).
   *
   * @return 'true' if this retraction was valid and the discrete line
   * segment has been updated, otherwise 'false' and the discrete line
   * segment is unchanged.
   *
   * @pre m_cp_n != (1,0)
   */
  bool retractsFront( uint code );

  /**
   * Tries to retract the discrete line segment to the "back"
   * (negatively along the contour).
   *
   * @param code the shape of the retraction going from the "back"
   * extremity: 0: no move, 1: turn left (toward interior), 2: move
   * straight ahead (no turn), 3: turn right (toward exterior).
   *
   * @return 'true' if this retraction was valid and the discrete line
   * segment has been updated, otherwise 'false' and the discrete line
   * segment is unchanged.
   *
   * @pre m_c_n != (0,0)
   */
  bool retractsBack( uint code );

  /**
   * Tries to extend the discrete line segment both to the "back"
   * (negatively along the contour) and to the "front" (positively
   * along the contour).
   *
   * @param code_back 0: no move, 1: turn right (toward interior), 2:
   * move straight ahead (no turn), 3: turn left (toward exterior).
   *
   * @param code_front 0: no move, 1: turn left (toward interior), 2:
   * move straight ahead (no turn), 3: turn right (toward exterior).
   *
   * @return 'true' if this symmetric extension was valid and the
   * discrete line segment has been updated, otherwise 'false' and the
   * discrete line segment is unchanged.
   */
  bool extends( uint code_back, uint code_front );

  /**
   * Tries to extend the discrete line segment both to the "back"
   * (negatively along the contour) and to the "front" (positively
   * along the contour).
   * Be careful, faster than 'extends' but the 'retractsSegment' is not valid for
   * further extension/suppression. Only (a,b,mu) remains valid.
   * @see extends
   *
   * @param code_back 0: no move, 1: turn left (toward interior), 2:
   * move straight ahead (no turn), 3: turn right (toward exterior).
   *
   * @param code_front 0: no move, 1: turn right (toward interior), 2:
   * move straight ahead (no turn), 3: turn left (toward exterior).
   *
   * @return 'true' if this symmetric extension was valid and the
   * discrete line segment has been updated, otherwise 'false' and the
   * discrete line segment is unchanged.
   */
  bool extendsUnsecure( uint code_back, uint code_front );


  /**
   * Tries to displace the reference frame one step forward. Useful
   * for incremental computation of tangents over a contour. The code
   * indicates how the new center is located with respect to the
   * preceding one. Note that the reference frame can be inversed.
   *
   * @param code 0: no move (! => does nothing), 1: turn right
   * (toward interior), 2: move straight ahead (no turn), 3: turn left
   * (toward exterior).
   *
   * @return 'true' in any case, except when the discrete line segment
   * is reduced to two points. Does nothing in this case and return
   * 'false'.
   */
  bool slidesForward( uint code );

  /**
   * Moves a vector one step forward along the contour. Useful
   * for incremental computation of tangents over a contour. The code
   * indicates how the new center is located with respect to the
   * preceding one. Note that the reference frame can be inversed.
   *
   * @param code 0: no move (! => does nothing), 1: turn right
   * (toward interior), 2: move straight ahead (no turn), 3: turn left
   * (toward exterior).
   * @param v any vector.
   */
  static void slidesForward( uint code, Vector2i & v );

  /**
   * Tries to displace the reference frame one step backward. Useful
   * for incremental computation of tangents over a contour. The code
   * indicates how the new center is located with respect to the
   * preceding one. Note that the reference frame can be inversed.
   *
   * @param code 0: no move (! => does nothing), 1: turn left
   * (toward interior), 2: move straight ahead (no turn), 3: turn right
   * (toward exterior).
   *
   * @return 'true' in any case, except when the discrete line segment
   * is reduced to two points. Does nothing in this case and return
   * 'false'.
   */
  bool slidesBackward( uint code );

  /**
   * Moves a vector one step backward along the contour. Useful
   * for incremental computation of tangents over a contour. The code
   * indicates how the new center is located with respect to the
   * preceding one. Note that the reference frame can be inversed.
   *
   * @param code 0: no move (! => does nothing), 1: turn right
   * (toward interior), 2: move straight ahead (no turn), 3: turn left
   * (toward exterior).
   * @param v any vector.
   */
  static void slidesBackward( uint code, Vector2i & v );
  

  // ------------------------- Reference frame services -----------------------
public:

  /**
   * Turn reference frame to the right (clockwise).  /!\ There are
   * some restrictions in changing a reference frame of a 'C4CSegment'.
   * @pre m_a < 0
   */
  void turnRight();

  /**
   * Turn reference frame to the left (counterclokckwise). /!\ There
   * are some restrictions in changing a reference frame of a
   * 'C4CSegment'.
   * @pre m_a > 0
   */
  void turnLeft();
  
  /**
   * Move reference frame along X. /!\ There
   * are some restrictions in changing a reference frame of a
   * 'C4CSegment'.
   * @param dx the variation in X.
   * @post m_mu <= 0
   */
  void moveAlongX( int dx );
  /**
   * Move reference frame along Y. /!\ There
   * are some restrictions in changing a reference frame of a
   * 'C4CSegment'.
   * @param dy the variation in Y.
   * @post m_mu <= 0
   */
  void moveAlongY( int dy );
  
  // ------------------------- Other geometric services -----------------------
public:

  /**
   * The length depends on the normal to the segment and the "natural"
   * orientation of the reference frame.
   * Defined as b/sqrt(a*a+b*b).
   * @return the corrected length associated with this line segment.
   */
  INLINE float correctedLength() const;

  /**
   * The length depends only the normal to the segment.
   * Defined as sqrt(a*a+b*b)/(|a|+|b|).
   * @return the corrected length associated with this line segment.
   */
  INLINE float averagedLength() const;
  

  /**
   * A discrete segment represents the digitization of some true
   * contour that is approximately straight.  For any [x] within the
   * bounds of the segment, there is a given probability function [y]
   * that estimates the probability that the point (x,y) lies on the true
   * contour.
   *
   * @param x a coordinate in the local frame (between 'c_n' and 'cp_n').
   * @return the probability density of the described function
   */
  PWFAffineFunction yDPFct( float x ) const;

  /**
   * A discrete segment represents the digitization of some true
   * contour that is approximately straight. For any [x] within the
   * bounds of the segment, there is a given probability function [y]
   * that estimates the probability that the point (x,y) lies on the true
   * contour. The return value is the mean of this probability function.
   *
   * @param x a coordinate in the local frame (between 'c_n' and 'cp_n').
   * @return the mean Y fct for this [x].
   */
  float meanY( float x ) const;

  /**
   * A discrete segment represents the digitization of some true
   * contour that is approximately straight. For any [y] within the
   * bounds of the segment, there is a given probability function [x]
   * that estimates the probability that the point (x,y) lies on the true
   * contour. The return value is the mean of this probability function.
   *
   * @param y a coordinate in the local frame (between 'c_n' and 'cp_n').
   * @return the mean X fct for this [y].
   * @TODO It is currently an approximation.
   */
  float meanX( float y ) const;

  /**
   * Projects the point [m] onto the average straight line (ie (mu+nu)/2).
   * @param m any point expressed in the local reference frame (may not be part of the segment).
   * @return the projected point.
   */
  Vector2D project( const Vector2i & m ) const;

  /**
   * Projects the point [m] onto the straight line whose points have
   * remainder [r].
   *
   * @param m any point expressed in the local reference frame (may not
   * be part of the segment).
   *
   * @param r the remainder (may not be an integer).
   * @return the projected point.
   */
  Vector2D project( const Vector2i & m, float r ) const;

  /**
   * Projects the point [m] onto the straight line going through point [p].
   *
   * @param m any point expressed in the local reference frame (may not
   * be part of the segment).
   *
   * @param p any point expressed in the local reference frame (may not
   * be part of the segment).
   *
   * @return the projected point.
   */
  Vector2D project( const Vector2i & m, const Vector2i & p ) const;

  /**
   * Defined as: norm( project(cp_n) - project(c_n) )
   * @return the projected length of the segment.
   * @see projectRegularly
   */
  float projectedSegmentLength() const;

  /**
   * Defined as: norm1( m - c_n ) / norm1( cp_n - c_n )
   * @param m any point of the segment.
   * @return the eccentricity of m
   * @see projectRegularly
   */
  float eccentricity( const Vector2i & m ) const;

  /**
   * Projects the point [m] onto the average straight line (ie
   * (mu+nu)/2). The projection is regular in the sense that any two
   * successive point of the segment are projected at equal distance.
   * Uses the eccentricity of the point and the length of the
   * projected segment.
   *
   * @param m any point of the segment.
   * @return the projected point.
   * @see projectedSegmentLength,eccentricity
   */
  Vector2D projectRegularly( const Vector2i & m ) const;

  /**
   * Defined as: norm1( m - c_n ) / norm1( cp_n - c_n )
   * @param m any point of the segment.
   * @return the eccentricity of m
   * @see projectRegularly
   */
  float eccentricity( const Vector2D & m ) const;

  /**
   * Projects the point [m] onto the average straight line (ie
   * (mu+nu)/2). The projection is regular in the sense that any two
   * successive point of the segment are projected at equal distance.
   * Uses the eccentricity of the point and the length of the
   * projected segment.
   *
   * @param m any point of the segment.
   * @return the projected point.
   * @see projectedSegmentLength,eccentricity
   */
  Vector2D projectRegularly( const Vector2D & m ) const;

  /**
   * @param ecc an eccentricity characterising a point on the segment.
   * @return the corresponding regularly projected point.
   * @see projectRegularly,projectedSegmentLength,eccentricity
   */
  Vector2D eccentricityPoint( float ecc ) const;
  

  /**
   * This minimum slope is obtained by assuming the digitization is
   * the same for any straight line in-between the leaning points.
   *
   * @return the minimum slope of the straight lines that are
   * digitized as this segment (the dx is in 'x()' and the dy is in
   * 'y()').
   */
  Vector2i minSlopeLeaningPoints() const;

  /**
   * This maximum slope is obtained by assuming the digitization is
   * the same for any straight line in-between the leaning points.
   *
   * @return the maximum slope of the straight lines that are
   * digitized as this segment (the dx is in 'x()' and the dy is in
   * 'y()').
   */
  Vector2i maxSlopeLeaningPoints() const;

  /**
   * This minimum slope is obtained by assuming the digitization is
   * the same for any straight line in-between the end points. The
   * computation is derived from the dual line parameter space.
   *
   * @return the minimum slope of the straight lines that are
   * digitized as this segment (the dx is in 'x()' and the dy is in
   * 'y()').
   */
  Vector2i minSlopeDual() const;

  /**
   * This minimum slope is obtained by assuming the digitization is
   * the same for any straight line in-between the end points. The
   * computation is derived from the dual line parameter space.
   *
   * @return the maximum slope of the straight lines that are
   * digitized as this segment (the dx is in 'x()' and the dy is in
   * 'y()').
   */
  Vector2i maxSlopeDual() const;
   
  

  /**
   *  Computes the coordinates of the right most and left most
   *  vertices of the preimage as defined in the dual space ([0,1]^2).
   *
   *  @return a 2D vector of floats with the vertices coordinates in
   *  the dual space.
   */
  Vector2D rightmostDualVertex() const;
  Vector2D leftmostDualVertex() const;
  Vector2D middleDownDualVertex() const;
  Vector2D middleUpDualVertex() const;

  /**
  * Computation of the preimage vertices associated to a C4CSegment
  * Helper function of {rightmost,leftmost,middleUp,middleDown}DualVertex() methods
  *
  * @param leftmost
  * @param middleUp
  * @param rightmost
  * @param middleDown
  */
  void getPreimage(Vector2D &leftmost, Vector2D &middleUp, Vector2D &rightmost, Vector2D &middleDown) const;


  // ------------------------- Accessors --------------------------------------
public:

  /**
   * @return the current discrete line segment as a discrete line (a,b,mu).
   */
  INLINE DLine getLine() const;

  /**
   * @return the current tangent vector associated to the segment (b,a).
   */
  INLINE Vector2i getTangent() const;

  /**
   * \see MeasureOfStraightLines.h
   * @return the current tangent vector slope associated to the segment (b,a) based on the desnity if straight Lines
   * 
   */
  double getTangentMeasure() const;

   /**
   * \see MeasureOfStraightLines.h
   * @return the measure of the preimage of the segment (b,a).
   */
  double getMeasure() const;


  /**
   * @return coefficient a in (a,b,mu) characterizing the line segment.
   */
  INLINE int a() const;
  /**
   * @return coefficient b in (a,b,mu) characterizing the line segment.
   */
  INLINE int b() const;
  /**
   * @return coefficient mu in (a,b,mu) characterizing the line segment.
   */
  INLINE int mu() const;
  /**
   * @return local coordinates of positive extremity C'_n.
   */
  INLINE Vector2i cp_n() const;
  /**
   * @return local coordinates of negative extremity C_n.
   */
  INLINE Vector2i c_n() const;

  /**
   * @return the number of upper leaning points.
   */
  INLINE uint nbU() const;

  /**
   * @return local coordinates of upper leaning point closest to
   * negative extremity (U).
   */
  INLINE Vector2i u() const;

  /**
   * @return local coordinates of upper leaning point closest to
   * positive extremity (U').
   */
  INLINE Vector2i up() const;

  /**
   * @return the number of lower leaning points.
   */
  INLINE uint nbL() const;

  /**
   * @return local coordinates of lower leaning point closest to
   * negative extremity (L).
   */
  INLINE Vector2i l() const;

  /**
   * @return local coordinates of lower leaning point closest to
   * positive extremity (L').
   */
  INLINE Vector2i lp() const;

  /**
   * This method has sense only for segments defined on the boundary
   * of the digitization of (at least locally) convex shape.
   *
   * @return 'true' if this segment has at least two upper leaning points. 
   */
  INLINE bool isULU() const;

  /**
   * This method has sense only for segments defined on the boundary
   * of the digitization of (at least locally) convex shape.
   *
   * @return 'true' if this segment has only one upper leaning point.
   */
  INLINE bool isLUL() const;

  /**
   * An upper-Christoffel word is a C4CSegment whose path is between
   * two upper leaning point.
   *
   * @return 'true' if this segment is an upper christoffel word
   * (eventually horizontal).
   */
  INLINE bool isUpperChristoffel() const;

  /**
   * A lower-Christoffel word is a C4CSegment whose path is between
   * two lower leaning point.
   *
   * @return 'true' if this segment is a lower christoffel word
   * (eventually horizontal).
   */
  INLINE bool isLowerChristoffel() const;
    
  

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
   * Coefficient a in (a,b,mu) characterizing the line segment.
   */
  int m_a;
  /**
   * Coefficient b in (a,b,mu) characterizing the line segment.
   */
  int m_b;
  /**
   * Coefficient mu in (a,b,mu) characterizing the line segment.
   */
  int m_mu;
  /**
   * Positive upper leaning point U'.
   */
  Vector2i m_up;
  /**
   * Positive lower leaning point L'.
   */
  Vector2i m_lp;
  /**
   * Negative upper leaning point U.
   */
  Vector2i m_u;
  /**
   * Positive lower leaning point L.
   */
  Vector2i m_l;
  /**
   * Positive extremity C'_n.
   */
  Vector2i m_cp_n;
  /**
   * Negative extremity C_n.
   */
  Vector2i m_c_n;
  /**
   * State describing the segment [C'_{n-1} <-> C'_n]. 0 : tangent, 4:
   * turned_int, 8: turned_ext
   */
  uint8 m_state_cp_n; 
  /**
   * State describing the segment [C_{n-1} <-> C_n]. 0 : tangent, 4:
   * turned_int, 8: turned_ext
   */
  uint8 m_state_c_n;

  // ------------------------- Hidden services --------------------------------
private:
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'C4CSegment'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'C4CSegment' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const C4CSegment & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "C4CSegment.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CSegment_h

#undef C4CSegment_RECURSES
#endif // else defined(C4CSegment_RECURSES)
