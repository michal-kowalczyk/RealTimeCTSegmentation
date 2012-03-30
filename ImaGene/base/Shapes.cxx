///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Shapes.cxx
//
// Creation : 2003/02/12
//
// Version : 2003/02/12
//
// Author : Jacques-Olivier Lachaud
//
// email : lachaud@labri.fr
//
// Purpose : ??
//
// Distribution :
//
// Use :
//	??
//
// Todo :
//	O ??
//
// History :
//	2003/02/12 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/base/Shapes.h"
#include "ImaGene/base/VectorUtils.h"
#include "ImaGene/mathutils/Mathutils.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/base/Shapes.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


const char* const Shapes_RCS_ID = "@(#)class Shapes definition.";


  
/////////////////////////////////////////////////////////////////////////////
// class StarShaped
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Standard services - public :


/////////////////////////////////////////////////////////////////////////////
// ------------------------- star-shaped services -------------------------

/**
 * @param p any point in the plane.
 *
 * @return 'true' if the point is inside the shape, 'false' if it
 * is strictly outside.
 */
bool
ImaGene::StarShaped::isInside( const Vector & p ) const
{
  ASSERT_Shapes( p.size() == 2 );
  Vector2D p2( p );
  return isInside( p2 );
}

/**
 * @param p any point in the plane.
 *
 * @return 'true' if the point is inside the shape, 'false' if it
 * is strictly outside.
 */
bool
ImaGene::StarShaped::isInside( const Vector2D & p ) const
{
  float t = parameter( p );
  Vector2D x_rel = x( t );
  x_rel -= center();
  float d_x = x_rel.ro( 0 )*x_rel.ro( 0 ) + x_rel.ro( 1 )*x_rel.ro( 1 );
  Vector2D p_rel( p );
  p_rel -= center();
  float d_p = p_rel.ro( 0 )*p_rel.ro( 0 ) + p_rel.ro( 1 )*p_rel.ro( 1 );

//   if (d_p <= d_x)
//     std::cerr << t << " " << p.x() << " " << p.y() << std::endl;
    
  return d_p <= d_x;
}


/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x'(t),y'(t)) made unitary which is the unit
 * tangent to the shape boundary.  
 */
ImaGene::Vector2D
ImaGene::StarShaped::tangent( float t ) const
{
  Vector2D tgt( xp( t ) );
  float norm = sqrt( tgt.ro( 0 )*tgt.ro( 0 ) + tgt.ro( 1 )*tgt.ro( 1 ) );
  tgt /= norm;
  return tgt;
}


/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x''(t),y''(t)) made unitary which is the unit
 * normal to the shape boundary looking inside the shape.  
 */
ImaGene::Vector2D
ImaGene::StarShaped::normal( float t ) const
{
  Vector2D tgt( tangent( t ) );
  return Vector2D( -tgt.ro( 1 ), tgt.ro( 0 ) );
}


/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the algebraic curvature at point (x(t),y(t)), positive
 * is convex, negative is concave when shape is to the left and
 * the shape boundary is followed counterclockwise.
 */
float 
ImaGene::StarShaped::curvature( float t ) const
{
  Vector2D tgt( xp( t ) );
  Vector2D dt( xpp( t ) );
  float norm = sqrt( tgt.ro( 0 )*tgt.ro( 0 ) + tgt.ro( 1 )*tgt.ro( 1 ) );
  float curv = ( dt.ro( 0 ) * tgt.ro( 1 ) - dt.ro( 1 ) * tgt.ro( 0 ) ) / ( norm * norm * norm );
  return - curv;
}
  

/**
 * @param t1 any angle between 0 and 2*Pi.
 * @param t2 any angle between 0 and 2*Pi, further from [t1].
 * @param nb the number of points used to estimate the arclength between x(t1) and x(t2).
 * @return the estimated arclength.
 */
float
ImaGene::StarShaped::arclength( float t1, float t2, uint nb ) const
{
  while ( t2 < t1 ) t2 += 2.0*M_PI;
  // float dt = ( t2 - t1 ) / nb;
  Vector2D x0( x( t1 ) );
  float l = 0.0;
  // JOL 2008/08/28
  for ( uint i = 1; i <= nb; ++i )
    {
      float t = ( ( t2 - t1 ) * i ) / nb;
      Vector2D x1( x( t1 + t ) );
      l += sqrt( Mathutils::sqr( x1.ro( 0 ) - x0.ro( 0 ) )
		 + Mathutils::sqr( x1.ro( 1 ) - x0.ro( 1 ) ) );
      x0 = x1;
    }
  return l;
}


  
/////////////////////////////////////////////////////////////////////////////
// class Circle
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// ------------------------- Standard services ----------------------------

/**
 * Destructor. 
 */
ImaGene::Circle::~Circle()
{
}


/**
 * Constructor. 
 * @param x0 the x-coordinate of the circle center.
 * @param y0 the y-coordinate of the circle center.
 * @param r the radius of the circle.
 */
ImaGene::Circle::Circle( float x0, float y0, float r )
  : m_xcenter( x0, y0 ), m_radius( r )
{
}



/////////////////////////////////////////////////////////////////////////////
// ------------- Implementation of 'StarShaped' services ------------------

/**
 * @param p any point in the plane.
 *
 * @return the angle parameter between 0 and 2*Pi corresponding to
 * this point for the shape.
 */
float
ImaGene::Circle::parameter( const Vector2D & pp ) const
{
  Vector2D p( pp );
  p -= m_xcenter;
  
  float angle = 0.0;
  if ( ( p.ro( 0 ) == 0.0 ) && ( p.ro( 1 ) == 0.0 ) )
    return angle;
  if ( p.ro( 0 ) >= p.ro( 1 ) )
    {
      if ( p.ro( 0 ) >= -p.ro( 1 ) )
	angle = atan( p.ro( 1 ) / p.ro( 0 ) );
      else
	angle = 1.5* M_PI + atan( - p.ro( 0 ) / p.ro( 1 ) );
    }
  else // ( p.ro( 0 ) >= p.ro( 1 ) )
    {
      if ( p.ro( 0 ) >= -p.ro( 1 ) )
	angle = 0.5*M_PI - atan( p.ro( 0 ) / p.ro( 1 ) );
      else
	angle = M_PI + atan( p.ro( 1 ) / p.ro( 0 ) );
    }
  angle = ( angle < 0.0 ) ? angle + 2*M_PI : angle;
  return angle;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x(t),y(t)) which is the position on the
 * shape boundary.
 */
ImaGene::Vector2D
ImaGene::Circle::x( float t ) const
{
  Vector2D c( m_radius*cos(t), m_radius*sin(t) );
  c += m_xcenter;
  return c;
}
  

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x'(t),y'(t)) which is the tangent to the
 * shape boundary.
 */
ImaGene::Vector2D 
ImaGene::Circle::xp( float t ) const
{
  Vector2D c( -m_radius*sin(t), m_radius*cos(t) );
  return c;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x''(t),y''(t)).
 */
ImaGene::Vector2D
ImaGene::Circle::xpp( float t ) const
{
  Vector2D c( -m_radius*cos(t), -m_radius*sin(t) );
  return c;
}

/////////////////////////////////////////////////////////////////////////////
// class Ellipse
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// ------------------------- Standard services ----------------------------

/**
 * Destructor. 
 */
ImaGene::Ellipse::~Ellipse()
{
  // std::cerr << " dstr called " ; // << std::endl;
}


/**
 * Constructor. 
 * @param x0 the x-coordinate of the ellipse center.
 * @param y0 the y-coordinate of the ellipse center.
 * @param a1 the half big axis of the ellipse.
 * @param a2 the half small axis of the ellipse.
 * @param theta the orientation of the ellipse.
 */
ImaGene::Ellipse::Ellipse( float x0, float y0, float a1, float a2, float theta )
  : m_xcenter( x0, y0 ), m_axis1( a1 ), m_axis2( a2 ), m_theta( theta )
{
//   std::cerr << " cstr called " 
// 	    << x0 << " " 
// 	    << y0 << " " 
// 	    << a1 << " " 
// 	    << a2 << " " 
// 	    << theta << " " 
// 	    << std::endl;
}


/////////////////////////////////////////////////////////////////////////////
// ------------- Implementation of 'StarShaped' services ------------------

/**
 * @param p any point in the plane.
 *
 * @return the polar angle corresponding to this point (between 0 and
 * 2*Pi) for the shape.
 */
float
ImaGene::Ellipse::parameter( const Vector2D & pp ) const
{
  //  std::cerr << " parameter called : " << pp.x() << " " << pp.y() << " " << std::endl;
  Vector2D v2d( pp );
  v2d -= m_xcenter;
  
  float angle; 

  if ( v2d.x() == 0.0 ) 
    {
      if ( v2d.y() >0 )
        angle = M_PI/2.0; 
      else 
        angle = 1.5*M_PI; 
    }
  else if (  ( v2d.x() > 0.0 ) && (   v2d.y() >= 0.0 ) )
    angle = atan(v2d.y()/v2d.x());
  else if (  ( v2d.x() > 0.0 ) && (   v2d.y() <= 0.0 ) )
    angle = 2*M_PI + atan(v2d.y()/v2d.x());
  else if (  ( v2d.x() < 0.0 ) && (   v2d.y() >= 0.0 ) )
    angle = atan(v2d.y()/v2d.x()) + M_PI;
  else // (  ( v2d.x() < 0.0 ) && (   v2d.y() <= 0.0 ) )
    angle = atan(v2d.y()/v2d.x()) + M_PI;

   return angle;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x(t),y(t)) which is the position on the
 * shape boundary.
 */
ImaGene::Vector2D 
ImaGene::Ellipse::x( float t ) const
{
  double a2 = m_axis1*m_axis1;
  double b2 = m_axis2*m_axis2;
  double costth = cos( t - m_theta );
  //  double sintth = sin( t - m_theta );
  double cost = cos( t );
  double sint = sin( t );
  double rho = m_axis2 / sqrt( 1.0 - ((a2-b2)/a2)*costth*costth);
    //m_axis2*m_axis1 / sqrt( a2 - (a2-b2)*costth*costth);
  Vector2D v( rho*cost, 
	      rho*sint );
  v += m_xcenter;
  return v;
}
  
/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x'(t),y'(t)) which is the tangent to the
 * shape boundary.
 */
ImaGene::Vector2D 
ImaGene::Ellipse::xp( float t ) const
{
  double a2 = m_axis1*m_axis1;
  double b2 = m_axis2*m_axis2;
  double costth = cos( t - m_theta );
  double sintth = sin( t - m_theta );
  double cost = cos( t );
  double sint = sin( t );
  double rho = m_axis2 / sqrt( 1.0 - ((a2-b2)/a2)*costth*costth);
  // JOL
  // double rhod= - m_axis2*(a2-b2)*costth*sintth/( pow(1.0-((a2-b2)/a2)*costth*costth  ,1.5 ) *a2 );
  double a = m_axis1; 
  double b = m_axis2;
  double rhod= a*b*(b2-a2)*sintth*costth
    / pow( a2*sintth*sintth + b2*costth*costth, 1.5 );
  Vector2D v( rhod*cost - rho*sint, rhod*sint + rho*cost );
  //ImaGene::VectorUtils::normalize(v);
  return v;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x''(t),y''(t)).
 */
ImaGene::Vector2D 
ImaGene::Ellipse::xpp( float t ) const
{
  double a2 = m_axis1*m_axis1;
  double b2 = m_axis2*m_axis2;
  double costth = cos( t - m_theta );
  double sintth = sin( t - m_theta );
  double cost = cos( t );
  double sint = sin( t );
  double rho = m_axis2 / sqrt( 1.0 - ((a2-b2)/a2)*costth*costth);
  // JOL
  // double rhod = - m_axis2*(a2-b2)*costth*sintth/( pow(1.0-((a2-b2)/a2)*costth*costth  ,1.5 ) *a2 );
  // double rhodd =  m_axis2*(a2-b2)*sintth*sintth/( pow(1.0-((a2-b2)/a2)*costth*costth  ,1.5 ) *a2 ) 
  //     -  m_axis2*(a2-b2)*costth*costth/( pow(1.0-((a2-b2)/a2)*costth*costth  ,1.5 ) *a2 ) 
  //     + 3.0*m_axis2*(a2-b2)*(a2-b2)*costth*costth*sintth*sintth/( pow(1.0-((a2-b2)/a2)*costth*costth  ,2.5 ) *a2*a2 );
  double a = m_axis1; 
  double b = m_axis2;
  double rhod = a*b*(b2-a2)*sintth*costth
    / pow( a2*sintth*sintth + b2*costth*costth, 1.5 );
  double rhodd = a*b*(b2-a2)
    / pow( a2*sintth*sintth + b2*costth*costth, 2.5 )
    * ( (costth*costth - sintth*sintth) * (a2*sintth*sintth + b2*costth*costth)
	+ 3.0*(b2-a2)*sintth*sintth*costth*costth );
  
  // Vector2D v( rhod*cost - rho*sint, rhod*sint + rho*cost );
  // JOL
  //Vector2D v( rhodd*cost - 2*rhod*sint - rho*cost, rhodd*sint + rhod*cost - rho*sint );
  Vector2D v( rhodd*cost - 2.0*rhod*sint - rho*cost, rhodd*sint + 2.0*rhod*cost - rho*sint );
  //ImaGene::VectorUtils::normalize(v);
  return v;
}

/////////////////////////////////////////////////////////////////////////////
// class RoundedSquare
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// ------------------------- Standard services ----------------------------

/**
 * Destructor. 
 */
ImaGene::RoundedSquare::~RoundedSquare()
{
}


/**
 * Constructor. 
 * @param x0 the x-coordinate of the circle center.
 * @param y0 the y-coordinate of the circle center.
 * @param r the edge length of the square.
 * @param vr the radius of round corners.
 */
ImaGene::RoundedSquare::RoundedSquare( float x0, float y0, float r, float vr )
  : m_xcenter( x0, y0 ), m_r( r ), m_vr( vr )
{
  m_dtr = M_PI / 4.0 - atan( ( r - vr ) / r );
}



/////////////////////////////////////////////////////////////////////////////
// ------------- Implementation of 'StarShaped' services ------------------

/**
 * @param p any point in the plane.
 *
 * @return the angle parameter between 0 and 2*Pi corresponding to
 * this point for the shape.
 */
float
ImaGene::RoundedSquare::parameter( const Vector2D & pp ) const
{
  Vector2D p( pp );
  p -= m_xcenter;
  
  float angle = 0.0;
  if ( ( p.ro( 0 ) == 0.0 ) && ( p.ro( 1 ) == 0.0 ) )
    return angle;
  if ( p.ro( 0 ) >= p.ro( 1 ) )
    {
      if ( p.ro( 0 ) >= -p.ro( 1 ) )
	angle = atan( p.ro( 1 ) / p.ro( 0 ) );
      else
	angle = 1.5* M_PI + atan( - p.ro( 0 ) / p.ro( 1 ) );
    }
  else // ( p.ro( 0 ) >= p.ro( 1 ) )
    {
      if ( p.ro( 0 ) >= -p.ro( 1 ) )
	angle = 0.5*M_PI - atan( p.ro( 0 ) / p.ro( 1 ) );
      else
	angle = M_PI + atan( p.ro( 1 ) / p.ro( 0 ) );
    }
  angle = ( angle < 0.0 ) ? angle + 2*M_PI : angle;
  return angle;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x(t),y(t)) which is the position on the
 * shape boundary.
 */
ImaGene::Vector2D
ImaGene::RoundedSquare::x( float t ) const
{
  float tinf = M_PI / 4.0 - m_dtr;
  float tsup = M_PI / 4.0 + m_dtr;
  while ( t >= M_PI ) t -= 2 * M_PI; 
  while ( t < -M_PI ) t += 2 * M_PI;

  float rc = ( m_r - m_vr );

  float x, y;
  if ( ( t >= 0 ) && ( t <= tinf ) )
    {
      x = m_r;
      y = m_r * tan( t );
    }
  else if ( ( t > tinf ) && ( t <= M_PI / 4.0 ) )
    {
      float st = atan( ( m_r * tan( t ) - rc ) / m_vr );
      x = rc + m_vr * cos( st );
      y = rc + m_vr * sin( st );
    }
  else if ( ( t > M_PI / 4.0 ) && ( t <= tsup ) )
    {
      float st = atan( ( m_r * tan( M_PI / 2.0 - t ) - rc ) / m_vr );
      x = rc + m_vr * sin( st );
      y = rc + m_vr * cos( st );
    }
  else if ( ( t > tsup ) && ( t <= M_PI / 2.0 ) )
    {
      x= m_r * tan( M_PI / 2.0 - t );
      y = m_r;
    }
  else
    {
      x = m_r * cos( t );
      y = m_r * sin( t );
    }

  Vector2D c( x, y );
  c += m_xcenter;
  return c;
}
  

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x'(t),y'(t)) which is the tangent to the
 * shape boundary.
 */
ImaGene::Vector2D 
ImaGene::RoundedSquare::xp( float t ) const
{
  float tinf = M_PI / 4.0 - m_dtr;
  float tsup = M_PI / 4.0 + m_dtr;
  while ( t >= M_PI ) t -= 2 * M_PI; 
  while ( t < -M_PI ) t += 2 * M_PI;

  float rc = ( m_r - m_vr );

  float x, y;
  if ( ( t >= 0 ) && ( t <= tinf ) )
    {
      float tt = tan( t );
      x = 0;
      y = m_r * ( 1 + tt * tt );
    }
  else if ( ( t > tinf ) && ( t <= M_PI / 4.0 ) )
    {
      float tt = tan( t );
      float st = atan( ( m_r * tt - rc ) / m_vr );
      float stp = m_r * ( 1 + tt * tt )
	/ ( m_vr * ( 1 + 
		     ( m_r * ( tt - 1 ) + m_vr ) 
		     * ( m_r * ( tt - 1 ) + m_vr ) / ( m_vr * m_vr ) ) );

      x = - stp * m_vr * sin( st );
      y = stp * m_vr * cos( st );
    }
  else if ( ( t > M_PI / 4.0 ) && ( t <= tsup ) )
    {
      float tt = tan( M_PI / 2.0 - t );
      float st = atan( ( m_r * tt - rc ) / m_vr );
      float stp = m_r * ( -1 - tt * tt )
	/ ( m_vr * ( 1 + 
		     ( m_r * ( tt - 1 ) + m_vr ) 
		     * ( m_r * ( tt - 1 ) + m_vr ) / ( m_vr * m_vr ) ) );
      x = stp * m_vr * cos( st );
      y = -stp * m_vr * sin( st );
    }
  else if ( ( t > tsup ) && ( t <= M_PI / 2.0 ) )
    {
      float tt = tan( M_PI / 2.0 - t );
      x = - m_r * ( 1 + tt * tt );
      y = 0;
    }
  else
    {
      x = - m_r * sin( t );
      y = m_r * cos( t );
    }

  Vector2D c( x, y );
  return c;

}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x''(t),y''(t)).
 */
ImaGene::Vector2D
ImaGene::RoundedSquare::xpp( float t ) const
{
  float tinf = M_PI / 4.0 - m_dtr;
  float tsup = M_PI / 4.0 + m_dtr;
  while ( t >= M_PI ) t -= 2 * M_PI; 
  while ( t < -M_PI ) t += 2 * M_PI;

  float rc = ( m_r - m_vr );

  float x, y;
  if ( ( t >= 0 ) && ( t <= tinf ) )
    {
      float tt = tan( t );
      x = 0;
      y = m_r * 2 * tt * ( 1 + tt * tt );
    }
  else if ( ( t > tinf ) && ( t <= M_PI / 4.0 ) )
    {
      float tt = tan( t );
      float dt = 1 + tt * tt;
      float d2tt = 2 * tt * dt;
      float denom = 1 + ( m_r * ( tt - 1 ) + m_vr ) 
	* ( m_r * ( tt - 1 ) + m_vr ) / ( m_vr * m_vr );
      float ddenom = 2 * m_r * dt * ( m_r * ( tt - 1 ) + m_vr ) 
	/ ( m_vr * m_vr );
      
      float st = atan( ( m_r * tt - rc ) / m_vr );
      float stp = m_r * dt / ( m_vr * denom );
      float stpp = m_r * d2tt / ( m_vr * denom ) 
	- m_r * dt * ddenom / ( m_vr * denom * denom );

      x = m_vr * ( - stpp * sin( st ) - stp * stp * cos( st ) );
      y = m_vr * ( stpp * cos( st ) - stp * stp * sin( st ) );
    }
  else if ( ( t > M_PI / 4.0 ) && ( t <= tsup ) )
    {
      float tt = tan( M_PI / 2.0 - t );
      float dt = -1 - tt * tt;
      float d2tt = - 2 * tt * dt;
      float denom = 1 + ( m_r * ( tt - 1 ) + m_vr ) 
	* ( m_r * ( tt - 1 ) + m_vr ) / ( m_vr * m_vr );
      float ddenom = 2 * m_r * dt * ( m_r * ( tt - 1 ) + m_vr ) 
	/ ( m_vr * m_vr );

      float st = atan( ( m_r * tt - rc ) / m_vr );
      float stp = m_r * dt / ( m_vr * denom );
      float stpp = m_r * d2tt / ( m_vr * denom ) 
	- m_r * dt * ddenom / ( m_vr * denom * denom );

      x = m_vr * ( stpp * cos( st ) - stp * stp * sin( st ) );
      y = m_vr * ( -stpp * sin( st ) - stp * stp * cos( st ) );
    }
  else if ( ( t > tsup ) && ( t <= M_PI / 2.0 ) )
    {
      float tt = tan( M_PI / 2.0 - t );
      x = - m_r * 2 * tt * ( 1 + tt * tt );
      y = 0;
    }
  else
    {
      x = - m_r * cos( t );
      y = - m_r * sin( t );
    }

  Vector2D c( x, y );
  return c;

}

/////////////////////////////////////////////////////////////////////////////
// class Cngon
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// ------------------------- Standard services ----------------------------

/**
 * Destructor. 
 */
ImaGene::Cngon::~Cngon()
{
  // std::cerr << " dstr called " ; // << std::endl;
}

/**
 * Constructor. 
 * @param x0 the x-coordinate of the regular convex n-gon center.
 * @param y0 the y-coordinate of the regular convex n-gon center.
 * @param radius the radius of the regular convex n-gon.
 * @param n the numner of sides of the regularconvex n-gon.
 * @param theta the orientation of the regular convex n-gon.
 */

ImaGene::Cngon::Cngon( float x0, float y0, float radius, uint k, float theta )
  : m_xcenter( x0, y0 ), m_r ( radius ), m_side_number( k ), m_theta( theta )
{
//   std::cerr << " cstr called " 
// 	    << x0 << " " 
// 	    << y0 << " " 
// 	    << a1 << " " 
// 	    << a2 << " " 
// 	    << theta << " " 
// 	    << std::endl;
}


/////////////////////////////////////////////////////////////////////////////
// ------------- Implementation of 'StarShaped' services ------------------

/**
 * @param p any point in the plane.
 *
 * @return the polar angle corresponding to this point (between 0 and
 * 2*Pi) for the shape.
 */
float
ImaGene::Cngon::parameter( const Vector2D & pp ) const
{
  //  std::cerr << " parameter called : " << pp.x() << " " << pp.y() << " " << std::endl;
  Vector2D v2d( pp );
  v2d -= m_xcenter;
  return atan2( v2d.y(), v2d.x() );

  // float angle; 
  // if ( v2d.x() == 0.0 ) 
  //   {
  //     if ( v2d.y() >0 )
  //       angle = M_PI/2.0; 
  //     else 
  //       angle = 1.5*M_PI; 
  //   }
  // else if (  ( v2d.x() > 0.0 ) && (   v2d.y() >= 0.0 ) )
  //   angle = atan(v2d.y()/v2d.x());
  // else if (  ( v2d.x() > 0.0 ) && (   v2d.y() <= 0.0 ) )
  //   angle = 2*M_PI + atan(v2d.y()/v2d.x());
  // else if (  ( v2d.x() < 0.0 ) && (   v2d.y() >= 0.0 ) )
  //   angle = atan(v2d.y()/v2d.x()) + M_PI;
  // else // (  ( v2d.x() < 0.0 ) && (   v2d.y() <= 0.0 ) )
  //   angle = atan(v2d.y()/v2d.x()) + M_PI;

  // return angle;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x(t),y(t)) which is the position on the
 * shape boundary.
 */
ImaGene::Vector2D
ImaGene::Cngon::x( float t ) const
{
  double angle = t-m_theta; 
  while ( angle < 0.0 )
    angle += 2.0*M_PI;
  
  // seek the vertices between the point, then compute the vector from one vertex to the next one.
  
  uint intervale_lower = static_cast<uint>( floor( ( angle )* m_side_number / (2.0 * M_PI ) ) );
  uint intervale_upper = intervale_lower == ( m_side_number -1 ) ? 0 : intervale_lower+1;
  double dist = m_r*cos ( M_PI / m_side_number );
  Vector2D s1 ( m_r*cos(m_theta + intervale_lower*2.0*M_PI/m_side_number),
		m_r*sin(m_theta + intervale_lower*2.0*M_PI/m_side_number) );
  Vector2D s2 ( m_r*cos(m_theta + intervale_upper*2.0*M_PI/m_side_number),
		m_r*sin(m_theta + intervale_upper*2.0*M_PI/m_side_number) );
  Vector2D s3( s2.x() - s1.x(), s2.y() - s1.y());
  
  double line_angle = ImaGene::VectorUtils::getAngle (s3);
  
  double rho = dist/(cos (t - line_angle - 0.5*M_PI));
  
  Vector2D c( rho*cos(t), rho*sin(t) );

  c += m_xcenter;
  //   std::cerr << " x called for " << t << " result " << c.x() << " " << c.y() << std::endl;
  return c;
}
  

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x'(t),y'(t)) which is the tangent to the
 * shape boundary.
 */
ImaGene::Vector2D 
ImaGene::Cngon::xp( float t ) const
{
  // seek the vertices between the point, then compute the vector from one vertex to the next one.
  // TODO check if angle equals that of a vertex ?
  double angle = t-m_theta; 
  while ( angle < 0.0 )
    angle += 2.0*M_PI;
  
  uint intervalle_lower = static_cast<uint>( floor( angle * m_side_number / (2.0 * M_PI ) ) );
  uint intervalle_upper = intervalle_lower == ( m_side_number -1 ) ? 0 : intervalle_lower+1;
  float dist = m_r*sin ( M_PI / m_side_number );
  Vector2D s1 ( m_r*cos(m_theta + intervalle_lower*2.0*M_PI/m_side_number),
		m_r*sin(m_theta + intervalle_lower*2.0*M_PI/m_side_number) );
  Vector2D s2 ( m_r*cos(m_theta + intervalle_upper*2.0*M_PI/m_side_number),
		m_r*sin(m_theta + intervalle_upper*2.0*M_PI/m_side_number) );
  s2 -= s1;
  
  ImaGene::VectorUtils::normalize(s2);
  
  return s2;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x''(t),y''(t)).
 */
ImaGene::Vector2D
ImaGene::Cngon::xpp( float t ) const
{
  //   std::cerr << " xpp called "; 
  //  << std::endl;
  Vector2D c(0.0,0.0);
  return c;
}


/////////////////////////////////////////////////////////////////////////////
// class Flower
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// ------------------------- Standard services ----------------------------

/**
 * Destructor. 
 */
ImaGene::Flower::~Flower()
{
}


/**
 * Constructor. 
 * @param x0 the x-coordinate of the flower center.
 * @param y0 the y-coordinate of the flower center.
 * @param r the big radius of the flower.
 * @param vr the variable small radius of the flower.
 * @param k the number of flower extremeties.
 * @param phi the phase of the flower (in radian).
 */
ImaGene::Flower::Flower( float x0, float y0,
			 float r, float vr, uint k, float phi )
  : m_xcenter( x0, y0 ), m_r( r ), m_vr( vr ), m_k ( k ), m_phi( phi )
{
}



/////////////////////////////////////////////////////////////////////////////
// ------------- Implementation of 'StarShaped' services ------------------

/**
 * @param p any point in the plane.
 *
 * @return the angle parameter between 0 and 2*Pi corresponding to
 * this point for the shape.
 */
float
ImaGene::Flower::parameter( const Vector2D & pp ) const
{
  Vector2D p( pp );
  p -= m_xcenter;

  float angle = 0.0;
  if ( ( p.ro( 0 ) == 0.0 ) && ( p.ro( 1 ) == 0.0 ) )
    return angle;
  if ( p.ro( 0 ) >= p.ro( 1 ) )
    {
      if ( p.ro( 0 ) >= -p.ro( 1 ) )
	angle = atan( p.ro( 1 ) / p.ro( 0 ) );
      else
	angle = 1.5* M_PI + atan( - p.ro( 0 ) / p.ro( 1 ) );
    }
  else // ( p.ro( 0 ) >= p.ro( 1 ) )
    {
      if ( p.ro( 0 ) >= -p.ro( 1 ) )
	angle = 0.5*M_PI - atan( p.ro( 0 ) / p.ro( 1 ) );
      else
	angle = M_PI + atan( p.ro( 1 ) / p.ro( 0 ) );
    }
  angle = ( angle < 0.0 ) ? angle + 2*M_PI : angle;
  return angle;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x(t),y(t)) which is the position on the
 * shape boundary.
 */
ImaGene::Vector2D
ImaGene::Flower::x( float t ) const
{
  // x:=t->(R+r*cos(k*t+p))*cos(t);
  // y:=t->(R+r*cos(k*t+p))*sin(t);
  float r = m_r + m_vr * cos( m_k * t + m_phi );
  Vector2D c( r * cos( t ), r * sin( t ) );
  c += m_xcenter;
  return c;
}
  

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x'(t),y'(t)) which is the tangent to the
 * shape boundary.
 */
ImaGene::Vector2D 
ImaGene::Flower::xp( float t ) const
{
  float r = m_r + m_vr * cos( m_k * t + m_phi );
  float rp = - m_vr * sin( m_k * t + m_phi ) * m_k;
  Vector2D c( rp * cos( t ) - r * sin( t ),
	      rp * sin( t ) + r * cos( t ) );
  return c;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x''(t),y''(t)).
 */
ImaGene::Vector2D
ImaGene::Flower::xpp( float t ) const
{
  float r = m_r + m_vr * cos( m_k * t + m_phi );
  float rp = - m_vr * sin( m_k * t + m_phi ) * m_k;
  float rpp = - m_vr * cos( m_k * t + m_phi ) * m_k * m_k;

  Vector2D c( rpp * cos( t ) - 2 * rp * sin( t ) - r * cos( t ),
	      rpp * sin( t ) + 2 * rp * cos( t ) - r * sin( t ) );
  return c;
}




/////////////////////////////////////////////////////////////////////////////
// class AccFlower
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// ------------------------- Standard services ----------------------------

/**
 * Destructor. 
 */
ImaGene::AccFlower::~AccFlower()
{
}


/**
 * Constructor. 
 * @param x0 the x-coordinate of the flower center.
 * @param y0 the y-coordinate of the flower center.
 * @param r the big radius of the flower.
 * @param vr the variable small radius of the flower.
 * @param k the number of flower extremeties.
 * @param n the exponent giving the phase acceleration.
 */
ImaGene::AccFlower::AccFlower( float x0, float y0,
			       float r, float vr, uint k )
  : m_xcenter( x0, y0 ), m_r( r ), m_vr( vr ), m_k ( k )
{
  m_kp = 2 * m_k / ( M_PI * M_PI );
}



/////////////////////////////////////////////////////////////////////////////
// ------------- Implementation of 'StarShaped' services ------------------

/**
 * @param p any point in the plane.
 *
 * @return the angle parameter between 0 and 2*Pi corresponding to
 * this point for the shape.
 */
float
ImaGene::AccFlower::parameter( const Vector2D & pp ) const
{
  Vector2D p( pp );
  p -= m_xcenter;

  float angle = 0.0;
  if ( ( p.ro( 0 ) == 0.0 ) && ( p.ro( 1 ) == 0.0 ) )
    return angle;
  if ( p.ro( 0 ) >= p.ro( 1 ) )
    {
      if ( p.ro( 0 ) >= -p.ro( 1 ) )
	angle = atan( p.ro( 1 ) / p.ro( 0 ) );
      else
	angle = 1.5* M_PI + atan( - p.ro( 0 ) / p.ro( 1 ) );
    }
  else // ( p.ro( 0 ) >= p.ro( 1 ) )
    {
      if ( p.ro( 0 ) >= -p.ro( 1 ) )
	angle = 0.5*M_PI - atan( p.ro( 0 ) / p.ro( 1 ) );
      else
	angle = M_PI + atan( p.ro( 1 ) / p.ro( 0 ) );
    }
  angle = ( angle < 0.0 ) ? angle + 2*M_PI : angle;
  return angle;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x(t),y(t)) which is the position on the
 * shape boundary.
 */
ImaGene::Vector2D
ImaGene::AccFlower::x( float t ) const
{
  while ( t >= M_PI ) t -= 2 * M_PI; 
  while ( t < -M_PI ) t += 2 * M_PI;
 
  //double ktn = m_kp * pow( (double) t, m_n );
  double ktn = m_kp * t * t * t;
  double r = m_r + m_vr * cos( ktn );
  Vector2D c( r * cos( t ), r * sin( t ) );
  c += m_xcenter;
  return c;
}
  

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x'(t),y'(t)) which is the tangent to the
 * shape boundary.
 */
ImaGene::Vector2D 
ImaGene::AccFlower::xp( float t ) const
{
  while ( t >= M_PI ) t -= 2 * M_PI; 
  while ( t < -M_PI ) t += 2 * M_PI;

  //double ktn = m_kp * pow( (double) t, m_n );
  //double ktnp = m_kp * pow( (double) t, m_n - 1 ) * m_n;
  double ktn = m_kp * t * t * t;
  double ktnp = 3 * m_kp * t * t ;

  double r = m_r + m_vr * cos( ktn );
  double rp = - m_vr * sin( ktn ) * ktnp;
  Vector2D c( rp * cos( t ) - r * sin( t ),
	      rp * sin( t ) + r * cos( t ) );
  return c;
}

/**
 * @param t any angle between 0 and 2*Pi.
 *
 * @return the vector (x''(t),y''(t)).
 */
ImaGene::Vector2D
ImaGene::AccFlower::xpp( float t ) const
{
  while ( t >= M_PI ) t -= 2 * M_PI; 
  while ( t < -M_PI ) t += 2 * M_PI;

  // double ktn = m_kp * pow( (double) t, m_n );
  // double ktnp = m_kp * pow( (double) t, m_n - 1 ) * m_n;
  // double ktnpp = m_kp * pow( (double) t, m_n - 2 ) * m_n * ( m_n - 1 );
  double ktn = m_kp * t * t * t;
  double ktnp = 3 * m_kp * t * t;
  double ktnpp = 6 * m_kp * t;

  double r = m_r + m_vr * cos( ktn );
  double rp = - m_vr * sin( ktn ) * ktnp;
  double rpp = - m_vr * cos( ktn ) * ktnp * ktnp - m_vr * sin( ktn ) * ktnpp;

  Vector2D c( rpp * cos( t ) - 2 * rp * sin( t ) - r * cos( t ),
	      rpp * sin( t ) + 2 * rp * cos( t ) - r * sin( t ) );
  return c;
}


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////


