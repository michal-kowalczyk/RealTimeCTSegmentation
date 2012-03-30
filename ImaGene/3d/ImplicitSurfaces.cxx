///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ImplicitSurfaces.cxx
//
// Creation : 2005/01/19
//
// Version : 2005/01/19
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
//	2005/01/19 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/3d/ImplicitSurfaces.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/3d/ImplicitSurfaces.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ImplicitSurfaces_RCS_ID = "@(#)class ImplicitSurfaces definition.";



///////////////////////////////////////////////////////////////////////////////
// class ImplicitSurfaces
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ImplicitSphere::~ImplicitSphere()
{
  if ( m_x0 != 0 ) delete[] m_x0;
}


/**
 * Constructor.
 *
 * @param n the dimension of the sphere.
 * @param x0 the coordinates of the sphere center.
 * @param r the radius of the sphere.
 */
ImaGene::ImplicitSphere::ImplicitSphere( uint n, const float x0[], float r )
  : m_n( n ), m_r( r )
{
  m_x0 = new float[ n ];
  for ( uint i = 0; i < n; ++i )
    m_x0[ i ] = x0[ i ];
}



/**
 * @return a clone of 'this'.
 */
ImaGene::ImplicitSurface* 
ImaGene::ImplicitSphere::clone() const
{
  return new ImplicitSphere( m_n, m_x0, m_r );
}



/**
 * Evaluates the function at coordinates [xyz]. The surface lies
 * at isovalue 0. By convention, negative is outside and positive
 * is inside.
 *
 * @param xyz an array of coordinate.
 * @return a real value.
 */
float
ImaGene::ImplicitSphere::operator()( const float xyz[] ) const
{
  float v = 0; 
  for ( uint i = 0; i < m_n; ++i )
    v += Mathutils::sqr( xyz[ i ] - m_x0[ i ] );
  return m_r - sqrt( v );
}


/**
 * Evaluates the gradient of the function at coordinates
 * [xyz]. The surface lies at isovalue 0. By convention, negative
 * is outside and positive is inside.
 *
 * @param xyz an array of coordinate.
 * @param grad (returns) the gradient vector.
 */
void
ImaGene::ImplicitSphere::gradient( const float xyz[], float grad[] ) const
{
  float n = 0.0;
  for ( uint i = 0; i < m_n; ++i )
    {
      grad[ i ] = m_x0[ i ] - xyz[ i ];
      n += Mathutils::sqr( grad[ i ] );
    }
  n = sqrt( n );
  for ( uint i = 0; i < m_n; ++i )
    {
      grad[ i ] /= n;
    }
}



/**
 * Returns the bounds of the implicit surface (isovalue 0), if known.
 * @param lower the lower bounds, when finite.
 * @param has_lower the type for lower bounds.
 * @param upper the upper bounds, when finite.
 * @param has_upper the type for upper bounds.
 * @return 'true' if every bound type is finite, 'false' otherwise.
 */
bool
ImaGene::ImplicitSphere::bounds
( float lower[], BoundType has_lower[],
  float upper[], BoundType has_upper[] ) const
{
    for ( uint i = 0; i < m_n; ++i )
      {
	lower[ i ] = m_x0[ i ] - m_r;
	has_lower[ i ] = ImplicitSurface::Finite;
	upper[ i ] = m_x0[ i ] + m_r;
	has_upper[ i ] = ImplicitSurface::Finite;
      }
    return true;
}




/**
 * Destructor. 
 */
ImaGene::ImplicitTorus3D::~ImplicitTorus3D()
{
}


/**
 * Constructor. Specifies a torus of given center [center],
 * symmetric around the z-axis.
 * 
 * @param center the center of the torus.
 * @param big_radius the big radius of the torus.
 * @param small_radius the small radius of the torus.
 */
ImaGene::ImplicitTorus3D::ImplicitTorus3D
( const float center[],
  float big_radius, float small_radius )
  : m_br( big_radius ), m_sr( small_radius )
{
  for ( uint i =0; i < 3; ++i )
    m_center[ i ] = center[ i ];
}


/**
 * @return a clone of 'this'.
 */
ImaGene::ImplicitSurface* 
ImaGene::ImplicitTorus3D::clone() const
{
  return new ImplicitTorus3D( m_center, m_br, m_sr );
}


    
/**
 * Evaluates the function at coordinates [xyz]. The surface lies
 * at isovalue 0. By convention, negative is outside and positive
 * is inside.
 *
 * @param xyz an array of coordinate.
 * @return a real value.
 */
float 
ImaGene::ImplicitTorus3D::operator()( const float xyz[] ) const
{
  float r = sqrt( Mathutils::sqr( xyz[ 0 ] - m_center[ 0 ] )
		  + Mathutils::sqr( xyz[ 1 ] - m_center[ 1 ] ) );
  float v = Mathutils::sqr( r - m_br ) 
    + Mathutils::sqr( xyz[ 2 ] - m_center[ 2 ] );
  return m_sr - sqrt( v );
}


/**
 * Evaluates the gradient of the function at coordinates
 * [xyz]. The surface lies at isovalue 0. By convention, negative
 * is outside and positive is inside.
 *
 * @param xyz an array of coordinate.
 * @param grad (returns) the gradient vector.
 */
void
ImaGene::ImplicitTorus3D::gradient( const float xyz[], float grad[] ) const
{
  float dx0 = xyz[ 0 ] - m_center[ 0 ];
  float dx1 = xyz[ 1 ] - m_center[ 1 ];
  float r = sqrt( dx0 * dx0 + dx1 * dx1 );
  float xt0 = m_center[ 0 ] + dx0 * m_br / r;
  float xt1 = m_center[ 1 ] + dx1 * m_br / r;
  float n = 0.0;
  grad[ 0 ] = xt0 - xyz[ 0 ]; 
  n += Mathutils::sqr( grad[ 0 ] );
  grad[ 1 ] = xt1 - xyz[ 1 ]; 
  n += Mathutils::sqr( grad[ 1 ] );
  grad[ 2 ] = m_center[ 2 ] - xyz[ 2 ];
  n += Mathutils::sqr( grad[ 2 ] );
  n = sqrt( n );
  for ( uint i = 0; i < 3; ++i )
    {
      grad[ i ] /= n;
    }
}




/**
 * Returns the bounds of the implicit surface (isovalue 0), if known.
 * @param lower the lower bounds, when finite.
 * @param has_lower the type for lower bounds.
 * @param upper the upper bounds, when finite.
 * @param has_upper the type for upper bounds.
 * @return 'true' if every bound type is finite, 'false' otherwise.
 */
bool 
ImaGene::ImplicitTorus3D::bounds
( float lower[], BoundType has_lower[],
  float upper[], BoundType has_upper[] ) const
{
  lower[ 0 ] = m_center[ 0 ] - m_br - m_sr;
  has_lower[ 0 ] = ImplicitSurface::Finite;
  lower[ 1 ] = m_center[ 1 ] - m_br - m_sr;
  has_lower[ 1 ] = ImplicitSurface::Finite;
  lower[ 2 ] = m_center[ 0 ] - m_sr;
  has_lower[ 2 ] = ImplicitSurface::Finite;
  upper[ 0 ] = m_center[ 0 ] + m_br + m_sr;
  has_upper[ 0 ] = ImplicitSurface::Finite;
  upper[ 1 ] = m_center[ 1 ] + m_br + m_sr;
  has_upper[ 1 ] = ImplicitSurface::Finite;
  upper[ 2 ] = m_center[ 0 ] + m_sr;
  has_upper[ 2 ] = ImplicitSurface::Finite;
  return true;
}





/**
 * Destructor. 
 */
ImaGene::ImplicitSurfaceUnion::~ImplicitSurfaceUnion()
{
  if ( m_is1 != 0 ) delete m_is1;
  if ( m_is2 != 0 ) delete m_is2;
}


/**
 * Constructor.
 *
 * @param is1 any implicit surface (cloned).
 * @param is2 any implicit surface (cloned).
 */
ImaGene::ImplicitSurfaceUnion::ImplicitSurfaceUnion
( const ImplicitSurface & is1,
  const ImplicitSurface & is2 )
  : m_is1( is1.clone() ), m_is2( is2.clone() )
{
}



/**
 * @return a clone of 'this'.
 */
ImaGene::ImplicitSurface* 
ImaGene::ImplicitSurfaceUnion::clone() const
{
  return new ImplicitSurfaceUnion( *m_is1, *m_is2 );
}


    
/**
 * Evaluates the function at coordinates [xyz]. The surface lies
 * at isovalue 0. By convention, negative is outside and positive
 * is inside.
 *
 * @param xyz an array of coordinate.
 * @return a real value.
 */
float
ImaGene::ImplicitSurfaceUnion::operator()( const float xyz[] ) const
{
  float v1 = m_is1->operator()( xyz );
  float v2 = m_is2->operator()( xyz );
  return v1 > v2 ? v1 : v2;
}


/**
 * Evaluates the gradient of the function at coordinates
 * [xyz]. The surface lies at isovalue 0. By convention, negative
 * is outside and positive is inside.
 *
 * @param xyz an array of coordinate.
 * @param grad (returns) the gradient vector.
 */
void
ImaGene::ImplicitSurfaceUnion::gradient
( const float xyz[], float grad[] ) const
{
  float v1 = m_is1->operator()( xyz );
  float v2 = m_is2->operator()( xyz );
  if ( v1 > v2 )
    m_is1->gradient( xyz, grad );
  else 
    m_is2->gradient( xyz, grad );
}


/**
 * Returns the bounds of the implicit surface (isovalue 0), if known.
 * @param lower the lower bounds, when finite.
 * @param has_lower the type for lower bounds.
 * @param upper the upper bounds, when finite.
 * @param has_upper the type for upper bounds.
 * @return 'true' if every bound type is finite, 'false' otherwise.
 */
bool
ImaGene::ImplicitSurfaceUnion::bounds
( float lower[], BoundType has_lower[],
  float upper[], BoundType has_upper[] ) const
{
  return true;  
}











///////////////////////////////////////////////////////////////////////////////
// Interface - public :


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
