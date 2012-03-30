/** @file Line2D.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Line2D.h
//
// Creation : 2008/10/30
//
// Version : 2008/10/30
//
// Author : JOL
//
// Summary : Header file for files Line2D.ih and Line2D.cxx
//
// History :
//	2008/10/30 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Line2D declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Line2D_RECURSES)
#error Recursive header files inclusion detected in Line2D.h
#else // defined(Line2D_RECURSES)
#define Line2D_RECURSES

#if !defined Line2D_h
#define Line2D_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/Vector.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class Line2D
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Line2D' <p>
   * Aim: Represents a straight line in the Euclidean plane.
   */
  class Line2D
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~Line2D();

    /**
     * Constructor from two different points on the line.
     * @param p1 the first point
     * @param p2 the second point
     */
    INLINE Line2D( const Vector2D & p1, const Vector2D & p2 );

    /**
     * Constructor.
     * The line is not valid.
     */
    INLINE Line2D();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    INLINE Line2D( const Line2D & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    INLINE Line2D & operator=( const Line2D & other );
  
    // ----------------------- Line services ----------------------------------
  public:

    /**
     * @return a point belonging to the line.
     */
    INLINE const Vector2D & somePoint() const;

    /**
     * @return the direction of the line as a unit vector.
     */
    INLINE const Vector2D & direction() const;

    /**
     * @return the direction that is orthogonal to the line as a unit
     * vector (if considered oriented, it is turning counterclockwise).
     */
    INLINE Vector2D normal() const;

    /**
     * @param p any point.
     * @return the line parallel to 'this' and going through [p].
     */
    INLINE Line2D parallelLine( const Vector2D & p ) const;

    /**
     * @param d an algebraic distance.
     *
     * @return the line parallel to 'this' and going through points
     * that are at distance [d] of 'this'.
     */
    INLINE Line2D parallelLine( float d ) const;

    /**
     * @param p any point.
     * @return the line orthogonal to 'this' and going through [p].
     */
    INLINE Line2D orthogonalLine( const Vector2D & p ) const;

    /**
     * @param p any point.
     *
     * @return the positive distance from [p] to the line 'this'
     * (orthogonal distance).
     */
    INLINE float distance( const Vector2D & p ) const;

    /**
     * The distance is positive when the point is to the left of the
     * oriented line.
     * 
     * @param p any point.
     *
     * @return the algebraic distance from [p] to the line 'this'
     * (orthogonal distance).
     */
    INLINE float algebraicDistance( const Vector2D & p ) const;

    /**
     * @param p any point.
     *
     * @return the relative position of the orthogonal projection of p
     * on the line.
     */
    INLINE float position( const Vector2D & p ) const;


    /**
     * Computes the point at the intersection of 'this' and [other] if any.
     *
     * @param other the other straight line.
     * @param p the point that is both on 'this' and 'other'.
     *
     * @return 'true' if [p] exists and is unique, 'false' otherwise
     * (the two lines are parallel or the same).
     */
    bool intersectionPoint( const Line2D & other, Vector2D & p ) const; 


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
     * A point on the line.
     */
    Vector2D m_p1;

    /**
     * Another point on the line.
     */
    Vector2D m_p2;

    /**
     * Unit oriented direction from P1 to P2 on the line.
     */
    Vector2D m_u;


    // ------------------------- Hidden services ------------------------------
  protected:

    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'Line2D'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Line2D' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const Line2D & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/Line2D.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Line2D_h

#undef Line2D_RECURSES
#endif // else defined(Line2D_RECURSES)
