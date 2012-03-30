/** @file Shapes.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Shapes.h
//
// Creation : 2003/07/24
//
// Version : 2003/07/24
//
// Author : JOL
//
// Summary : Header file for files Shapes.ih and Shapes.cxx
//
// History :
//	2003/07/24 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Shapes declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Shapes_RECURSES)
#error Recursive header files inclusion detected in Shapes.h
#else // defined(Shapes_RECURSES)
#define Shapes_RECURSES

#if !defined Shapes_h
#define Shapes_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/base/Vector.h"
//#include "LinAlg/LinAlg2D/Vector2D.hpp"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {


  /////////////////////////////////////////////////////////////////////////////
  // class Shapes
  /////////////////////////////////////////////////////////////////////////////


  /** 
   * Description of class 'Shape' <p> Aim: Interface that
   * represents any shape. The shape has to tell whether a point is
   * inside or outside.
   */
  class Shape
  {
    // ------------------------- Standard services ----------------------------
  public:
    
    /**
     * Destructor. 
     */
    INLINE virtual ~Shape();

    /**
     * @param p any point in the space.
     * @return 'true' if this point lies inside the shape, 'false' otherwise.
     */
    virtual bool isInside( const Vector & p ) const = 0;
    
  };


  /** 
   * Description of class 'EShape' <p> Aim: Interface that represents
   * any shape with one given interior point. The shape has to tell
   * whether a point is inside or outside.
   */
  class EShape : public Shape
  {
    // ------------------------- Standard services ----------------------------
  public:
    
    /**
     * Destructor. 
     */
    INLINE virtual ~EShape();

    /**
     * @return a point p such that 'isInside(p)' returns 'true'.
     */
    virtual Vector interiorPoint() const = 0;
    
  };
  
  
  /////////////////////////////////////////////////////////////////////////////
  // class StarShaped
  /////////////////////////////////////////////////////////////////////////////


  /** 
   * Description of class 'StarShaped' <p> Aim: Abstract class that
   * represents any star-shaped object. Such a shape as a center and any
   * segment from this center to the shape boundary is included in the
   * shape. These shapes can thus be parameterized by an angle 't'
   * turning around the center.
   */
  class StarShaped : public EShape
  {
    // ------------------------- Standard services ----------------------------
  public:
    
    /**
     * Destructor. 
     */
    INLINE virtual ~StarShaped();


    // ------------------------- Implemented services -------------------------
  public:
    /**
     * @return a point p such that 'isInside(p)' returns 'true'.
     */
    INLINE virtual Vector interiorPoint() const;
 

    // ------------------------- Abstract services ----------------------------
  public:

    /**
     * @return the center of the star-shaped object.
     */
    virtual Vector2D center() const = 0;
    
    /**
     * @param p any point in the plane.
     *
     * @return the angle parameter between 0 and 2*Pi corresponding to
     * this point for the shape.
     */
    virtual float parameter( const Vector2D & p ) const = 0;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x(t),y(t)) which is the position on the
     * shape boundary.
     */
    virtual Vector2D x( float t ) const = 0;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x'(t),y'(t)) which is the tangent to the
     * shape boundary.
     */
    virtual Vector2D xp( float t ) const = 0;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x''(t),y''(t)).
     */
    virtual Vector2D xpp( float t ) const = 0;
    

    // ------------------------- star-shaped services -------------------------
  public:

    /**
     * @param p any point in the plane.
     *
     * @return 'true' if the point is inside the shape, 'false' if it
     * is strictly outside.
     */
    virtual bool isInside( const Vector & p ) const;

    /**
     * @param p any point in the plane.
     *
     * @return 'true' if the point is inside the shape, 'false' if it
     * is strictly outside.
     */
    virtual bool isInside( const Vector2D & p ) const;

    
    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x'(t),y'(t)) made unitary which is the unit
     * tangent to the shape boundary.  
     */
    Vector2D tangent( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x''(t),y''(t)) made unitary which is the unit
     * normal to the shape boundary looking inside the shape.  
     */
    Vector2D normal( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the algebraic curvature at point (x(t),y(t)), positive
     * is convex, negative is concave when shape is to the left and
     * the shape boundary is followed counterclockwise.
     */
    float curvature( float t ) const;

    /**
     * @param t1 any angle between 0 and 2*Pi.
     * @param t2 any angle between 0 and 2*Pi, further from [t1].
     * @param nb the number of points used to estimate the arclength between x(t1) and x(t2).
     * @return the estimated arclength.
     */
    float arclength( float t1, float t2, uint nb ) const;
  };


  /////////////////////////////////////////////////////////////////////////////
  // class Circle
  /////////////////////////////////////////////////////////////////////////////


  /** 
   * Description of class 'Circle' <p> Aim: Concrete class that
   * represents any circle in the plane. It is an implementation of a
   * 'StarShaped'.
   */
  class Circle : public StarShaped
  {

    // ------------------------- Standard services ----------------------------
  public:
    
    /**
     * Destructor. 
     */
    ~Circle();

    /**
     * Constructor. 
     * @param x0 the x-coordinate of the circle center.
     * @param y0 the y-coordinate of the circle center.
     * @param r the radius of the circle.
     */
    Circle( float x0, float y0, float r );


    // ------------- Implementation of 'StarShaped' services ------------------
  public:

    /**
     * @return the center of the star-shaped object.
     */
    INLINE Vector2D center() const;
    
    /**
     * @param p any point in the plane.
     *
     * @return the angle parameter between 0 and 2*Pi corresponding to
     * this point for the shape.
     */
    float parameter( const Vector2D & p ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x(t),y(t)) which is the position on the
     * shape boundary.
     */
    Vector2D x( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x'(t),y'(t)) which is the tangent to the
     * shape boundary.
     */
    Vector2D xp( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x''(t),y''(t)).
     */
    Vector2D xpp( float t ) const;
    

    // ------------------------- data ----------------------------
  private:

    /**
     * Center of the circle.
     */
    Vector2D m_xcenter;
    
    /**
     * Radius of the circle.
     */
    float m_radius;
    
  };

  /////////////////////////////////////////////////////////////////////////////
  // class Ellipse
  /////////////////////////////////////////////////////////////////////////////


  /** 
   * Description of class 'Ellipse' <p> Aim: Concrete class that
   * represents any ellipse in the plane. It is an implementation of a
   * 'StarShaped'.
   */
  class Ellipse : public StarShaped
  {
    
    // ------------------------- Standard services ----------------------------
  public:
    
    /**
     * Destructor. 
     */
    ~Ellipse();

    /**
     * Constructor. 
     * @param x0 the x-coordinate of the ellipse center.
     * @param y0 the y-coordinate of the ellipse center.
     * @param a1 the half big axis of the ellipse.
     * @param a2 the half small axis of the ellipse.
     * @param theta the orientation of the ellipse.
     */
    Ellipse( float x0, float y0, float a1, float a2, float theta );


    // ------------- Implementation of 'StarShaped' services ------------------
  public:

    /**
     * @return the center of the star-shaped object.
     */
    INLINE Vector2D center() const;
    
    /**
     * @param p any point in the plane.
     *
     * @return the angle parameter between 0 and 2*Pi corresponding to
     * this point for the shape.
     */
    float parameter( const Vector2D & p ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x(t),y(t)) which is the position on the
     * shape boundary.
     */
    Vector2D x( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x'(t),y'(t)) which is the tangent to the
     * shape boundary.
     */
    Vector2D xp( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x''(t),y''(t)).
     */
    Vector2D xpp( float t ) const;
    
    // ------------------------- data ----------------------------
  private:

    /**
     * Center of the ellipse.
     */
    Vector2D m_xcenter;

    /**
     * Half great axis of the ellipse.
     */
    float m_axis1;

    /**
     * Half small axis of the ellipse.
     */
    float m_axis2;
    
    /**
     * Orientation of the ellipse.
     */
    float m_theta;
    
  };


  /////////////////////////////////////////////////////////////////////////////
  // class RoundedSquare
  /////////////////////////////////////////////////////////////////////////////


  /** 
   * Description of class 'RoundedSquare' <p> Aim: Concrete class that
   * represents any rounded square in the plane. It is an implementation of a
   * 'StarShaped'.
   */
  class RoundedSquare : public StarShaped
  {

    // ------------------------- Standard services ----------------------------
  public:
    
    /**
     * Destructor. 
     */
    ~RoundedSquare();

    /**
     * Constructor. 
     * @param x0 the x-coordinate of the circle center.
     * @param y0 the y-coordinate of the circle center.
     * @param r the edge length of the square.
     * @param vr the radius of round corners.
     */
    RoundedSquare( float x0, float y0, float r, float vr );


    // ------------- Implementation of 'StarShaped' services ------------------
  public:

    /**
     * @return the center of the star-shaped object.
     */
    INLINE Vector2D center() const;
    
    /**
     * @param p any point in the plane.
     *
     * @return the angle parameter between 0 and 2*Pi corresponding to
     * this point for the shape.
     */
    float parameter( const Vector2D & p ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x(t),y(t)) which is the position on the
     * shape boundary.
     */
    Vector2D x( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x'(t),y'(t)) which is the tangent to the
     * shape boundary.
     */
    Vector2D xp( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x''(t),y''(t)).
     */
    Vector2D xpp( float t ) const;
    

    // ------------------------- data ----------------------------
  private:

    /**
     * Center of the rounded square.
     */
    Vector2D m_xcenter;
    
    /**
     * edge length of the square.
     */
    float m_r;
    
    /**
     * radius of the round corners.
     */
    float m_vr;
    
    /**
     * Limit angle around Pi/4 where the square stops and the corner starts.
     */
    float m_dtr;
    
  };

  /////////////////////////////////////////////////////////////////////////////
  // class Cngon
  /////////////////////////////////////////////////////////////////////////////


  /** 
   * Description of class 'Cngon' <p> Aim: Concrete class that
   * represents any regular Convex N-Gon in the plane. It is an
   * implementation of a 'StarShaped'.
   */
  class Cngon : public StarShaped
  {
    
    // ------------------------- Standard services ----------------------------
  public:
    
    /**
     * Destructor. 
     */
    ~Cngon();

    /**
     * Constructor. 
     * @param x0 the x-coordinate of the regular convex n-gon center.
     * @param y0 the y-coordinate of the regular convex n-gon center.
     * @param radius the radius of the regular convex n-gon.
     * @param n the numner of sides of the regularconvex n-gon.
     * @param theta the orientation of the regular convex n-gon.
     */

    Cngon( float x0, float y0, float radius, uint k, float theta );

    // ------------- Implementation of 'StarShaped' services ------------------
  public:

    /**
     * @return the center of the star-shaped object.
     */
    INLINE Vector2D center() const;
    
    /**
     * @param p any point in the plane.
     *
     * @return the angle parameter between 0 and 2*Pi corresponding to
     * this point for the shape.
     */
    float parameter( const Vector2D & p ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x(t),y(t)) which is the position on the
     * shape boundary.
     */
    Vector2D x( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x'(t),y'(t)) which is the tangent to the
     * shape boundary.
     */
    Vector2D xp( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x''(t),y''(t)).
     */
    Vector2D xpp( float t ) const;
    
    // ------------------------- data ----------------------------
  private:

    /**
     * Center of the regular convex n-gon.
     */
    Vector2D m_xcenter;

    /**
     * Radius of the regular convex n-gon.
     */
    float m_r;

    /**
     * Number of side of the regular convex n-gon.
     */
    uint m_side_number;

    /**
     * Orientation of the regular convex n-gon.
     */
    float m_theta;
    
  };


  
  /////////////////////////////////////////////////////////////////////////////
  // class Flower
  /////////////////////////////////////////////////////////////////////////////


  /** 
   * Description of class 'Flower' <p> Aim: Concrete class that
   * represents any flower in the plane. It is an implementation of a
   * 'StarShaped'.
   */
  class Flower : public StarShaped
  {

    // ------------------------- Standard services ----------------------------
  public:
    
    /**
     * Destructor. 
     */
    ~Flower();

    /**
     * Constructor. 
     * @param x0 the x-coordinate of the flower center.
     * @param y0 the y-coordinate of the flower center.
     * @param r the big radius of the flower.
     * @param vr the variable small radius of the flower.
     * @param k the number of flower extremeties.
     * @param phi the phase of the flower (in radian).
     */
    Flower( float x0, float y0, float r, float vr, uint k, float phi );


    // ------------- Implementation of 'StarShaped' services ------------------
  public:

    /**
     * @return the center of the star-shaped object.
     */
    INLINE Vector2D center() const;
    
    /**
     * @param p any point in the plane.
     *
     * @return the angle parameter between 0 and 2*Pi corresponding to
     * this point for the shape.
     */
    float parameter( const Vector2D & p ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x(t),y(t)) which is the position on the
     * shape boundary.
     */
    Vector2D x( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x'(t),y'(t)) which is the tangent to the
     * shape boundary.
     */
    Vector2D xp( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x''(t),y''(t)).
     */
    Vector2D xpp( float t ) const;
    

    // ------------------------- data ----------------------------
  private:

    /**
     * Center of the flower.
     */
    Vector2D m_xcenter;
    
    /**
     * Big radius of the flower.
     */
    float m_r;

    /**
     * the variable small radius of the flower.
     */
    float m_vr;
    
    /**
     * the number of flower extremeties.
     */
    uint m_k;
    
    /**
     * the phase of the flower (in radian).
     */
    float m_phi;
    
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // class AccFlower
  /////////////////////////////////////////////////////////////////////////////


  /** 
   * Description of class 'AccFlower' <p> Aim: Concrete class that
   * represents any "accelerating flower in the plane. It is an
   * implementation of a 'StarShaped'.
   */
  class AccFlower : public StarShaped
  {

    // ------------------------- Standard services ----------------------------
  public:
    
    /**
     * Destructor. 
     */
    ~AccFlower();

    /**
     * Constructor. 
     * @param x0 the x-coordinate of the flower center.
     * @param y0 the y-coordinate of the flower center.
     * @param r the big radius of the flower.
     * @param vr the variable small radius of the flower.
     * @param k the number of positive flower extremeties.
     */
    AccFlower( float x0, float y0, float r, float vr, uint k );


    // ------------- Implementation of 'StarShaped' services ------------------
  public:

    /**
     * @return the center of the star-shaped object.
     */
    INLINE Vector2D center() const;
    
    /**
     * @param p any point in the plane.
     *
     * @return the angle parameter between 0 and 2*Pi corresponding to
     * this point for the shape.
     */
    float parameter( const Vector2D & p ) const;

    /**
     * For an accelerating flower:
     * k' = 2*k*Pi/(Pi^n)
     * x(t) = (R+vr*cos(k'*t^n))*cos(t)
     * y(t) = (R+vr*cos(k'*t^n))*sin(t)
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x(t),y(t)) which is the position on the
     * shape boundary.
     */
    Vector2D x( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x'(t),y'(t)) which is the tangent to the
     * shape boundary.
     */
    Vector2D xp( float t ) const;

    /**
     * @param t any angle between 0 and 2*Pi.
     *
     * @return the vector (x''(t),y''(t)).
     */
    Vector2D xpp( float t ) const;
    

    // ------------------------- data ----------------------------
  private:

    /**
     * Center of the flower.
     */
    Vector2D m_xcenter;
    
    /**
     * Big radius of the flower.
     */
    float m_r;

    /**
     * the variable small radius of the flower.
     */
    float m_vr;
    
    /**
     * the number of flower extremeties.
     */
    uint m_k;
    
    /**
     * The value 2*m_k*Pi/(Pi^m_n)
     */
    double m_kp;

  };
  
  

} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "Shapes.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Shapes_h

#undef Shapes_RECURSES
#endif // else defined(Shapes_RECURSES)
