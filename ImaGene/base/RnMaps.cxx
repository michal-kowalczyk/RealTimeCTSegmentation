///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : RnMaps.cxx
//
// Creation : 2003/07/24
//
// Version : 2003/07/24
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
//	2003/07/24 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/base/RnMaps.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/base/RnMaps.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const RnMaps_RCS_ID = "@(#)class RnMaps definition.";

///////////////////////////////////////////////////////////////////////////////
// RnMaps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// class RnMap
///////////////////////////////////////////////////////////////////////////////

/**
 * Destructor. 
 */
RnMap::~RnMap()
{
  ASSERT_RnMaps( m_ptr_f != 0 );
  delete m_ptr_f;
}


/**
 * Constructor. Creates the identity mapping.
 */
RnMap::RnMap()
{
  m_ptr_f = new RnMapIdentity();
}


/**
 * Creates the mapping [f].
 * @param f the mapping.
 */
RnMap::RnMap( const RnMapImp & f )
  : m_ptr_f( f.clone() )
{
}


/**
 * Copy constructor.
 * @param other the object to copy.
 */
RnMap::RnMap( const RnMap & other )
  : m_ptr_f( other.m_ptr_f->clone() )
{
}


/**
 * Assignment.
 * @param other the object to copy.
 * @return a reference to 'this'.
 */
RnMap& 
RnMap::operator=( const RnMap & other )
{
  ASSERT_RnMaps( m_ptr_f != 0 );
  if ( &other != this )
    {
      delete m_ptr_f;
      m_ptr_f = other.m_ptr_f->clone();
    }
  return *this;
}
  

///////////////////////////////////////////////////////////////////////////////
// ------------------------- operator services ------------------------------


/**
 * Composition operator.
 * @param g any mapping.
 * @return the mapping 'this(g(x))'.
 */
RnMap
RnMap::operator()( const RnMap & g ) const
{
  RnMapComposer mc( *(this->m_ptr_f), *(g.m_ptr_f) );
  return RnMap( mc );
}


/**
 * Translation operator.
 * @param g any mapping.
 * @return the mapping 'this(x) + v'.
 */
RnMap
RnMap::operator+( const Vector & v ) const
{
  RnMapTranslater t( v );
  RnMapComposer mc( t, *(this->m_ptr_f) );
  return RnMap( mc );
}

  


///////////////////////////////////////////////////////////////////////////////
// ------------------------- static services ------------------------------


/**
 * Defines the identity mapping (of any dimension).
 * @return the corresponding mapping.
 */
RnMap
RnMap::makeIdentity()
{
  RnMapIdentity id;
  return RnMap( id );
}


/**
 * Defines a linear mapping through its matrix.
 * @param m the matrix defining the mapping.
 * @return the corresponding mapping.
 */
RnMap
RnMap::makeLinearMap( const Matrix & m )
{
  RnMapMatrixMultiplier mm( m );
  return RnMap( mm );
}

/**
 * Defines a scaling linear mapping through a diagonal matrix.
 * @param v the vector defining the mapping.
 * @return the corresponding mapping.
 */
RnMap
RnMap::makeScalingMap( const Vector & v )
{
  Matrix m( v.size(), v.size() );
  uint k, l;
  for ( k = 0; k < v.size(); ++k )
    {
      m.rw( k, k ) = v.ro( k );
      for ( l = k + 1; l < v.size(); ++l )
	m.rw( k, l ) = m.rw( l, k ) = 0.0;
    }
  return makeLinearMap( m );
}

/**
 * Defines a translation mapping.
 * @param v the vector defining the mapping.
 * @return the corresponding mapping.
 */
RnMap
RnMap::makeTranslationMap( const Vector & v )
{
  RnMapTranslater t( v );
  return RnMap( t );
}

/**
 * Defines the implicit mapping from R^center.size() to R defining
 * a centered sphere (for 0, inside is negative, outside positive).
 * @param r the radius of the sphere.
 * @return the corresponding mapping.
 */
RnMap
RnMap::makeImplicitSphere( float r )
{
  RnMapImplicitSphere ris( r );
  return RnMap( ris );
}

/**
 * Defines the implicit mapping from R^center.size() to R defining
 * a sphere (for 0, inside is negative, outside positive).
 * @param r the radius of the sphere.
 * @param center the center of the sphere.
 * @return the corresponding mapping.
 */
RnMap
RnMap::makeImplicitSphere( float r, const Vector & center )
{
  Vector neg_center( center );
  neg_center *= -1.0;
  // cout << center.size() << " " << neg_center.size() << endl;
  RnMapImplicitSphere ris( r );
  RnMapTranslater t( neg_center );
  RnMapComposer mc( ris, t );
  return RnMap( mc );
}

/**
 * Defines the implicit mapping from R^n.size() to R defining
 * a half-space (for 0, inside is negative, outside positive).
 * @param n the normal to the plane pointing outward.
 * @return the corresponding mapping.
 */
RnMap
RnMap::makeImplicitHalfSpace( const Vector & n )
{
  RnMapConstantDotProduct cdp( n );
  return RnMap( cdp );
}




/**
 * Defines a 3D rotation mapping along the x-axis.
 * @param theta the angle of the rotation (in radian).
 * @return the corresponding mapping.
 */
RnMap
RnMap::make3DRotationX( float theta )
{
  Matrix rot( 3, 3 );
  rot.rw( 0, 0 ) = 1.0;
  rot.rw( 0, 1 ) = 0.0;
  rot.rw( 0, 2 ) = 0.0;
  rot.rw( 1, 0 ) = 0.0;
  rot.rw( 1, 1 ) = cos( theta );
  rot.rw( 1, 2 ) = sin( theta );
  rot.rw( 2, 0 ) = 0.0;
  rot.rw( 2, 1 ) = -sin( theta );
  rot.rw( 2, 2 ) = cos( theta );
  return makeLinearMap( rot );
}


/**
 * Defines a 3D rotation mapping along the y-axis.
 * @param theta the angle of the rotation (in radian).
 * @return the corresponding mapping.
 */
RnMap
RnMap::make3DRotationY( float theta )
{
  Matrix rot( 3, 3 );
  rot.rw( 0, 0 ) = -sin( theta );
  rot.rw( 0, 1 ) = 0.0;
  rot.rw( 0, 2 ) = cos( theta );
  rot.rw( 1, 0 ) = 0.0;
  rot.rw( 1, 1 ) = 1.0;
  rot.rw( 1, 2 ) = 0.0;
  rot.rw( 2, 0 ) = cos( theta );
  rot.rw( 2, 1 ) = 0.0;
  rot.rw( 2, 2 ) = sin( theta );
  return makeLinearMap( rot );
}


/**
 * Defines a 3D rotation mapping along the z-axis.
 * @param theta the angle of the rotation (in radian).
 * @return the corresponding mapping.
 */
RnMap
RnMap::make3DRotationZ( float theta )
{
  Matrix rot( 3, 3 );
  rot.rw( 1, 0 ) = cos( theta );
  rot.rw( 1, 1 ) = sin( theta );
  rot.rw( 1, 2 ) = 0.0;
  rot.rw( 2, 0 ) = -sin( theta );
  rot.rw( 2, 1 ) = cos( theta );
  rot.rw( 2, 2 ) = 0.0;
  rot.rw( 0, 0 ) = 0.0;
  rot.rw( 0, 1 ) = 0.0;
  rot.rw( 0, 2 ) = 1.0;
  return makeLinearMap( rot );
}


/**
 * Minimum operator.
 * @param f any mapping.
 * @param g any mapping.
 * @return the mapping 'min(f(x),g(x))'
 */
RnMap 
RnMap::min( const RnMap & f, const RnMap & g )
{
  RnMapBinaryOperator bop( RnMapBinaryOperator::MIN, f.imp(), g.imp() );
  return RnMap( bop );
}


/**
 * Maximum operator.
 * @param f any mapping.
 * @param g any mapping.
 * @return the mapping 'min(f(x),g(x))'
 */
RnMap
RnMap::max( const RnMap & f, const RnMap & g )
{
  RnMapBinaryOperator bop( RnMapBinaryOperator::MAX, f.imp(), g.imp() );
  return RnMap( bop );
}

/**
 * Negation operator.
 * @param f any mapping.
 * @return the mapping '-f(x)'
 */
RnMap 
RnMap::neg( const RnMap & f )
{
  RnMapUnaryOperator nop( RnMapUnaryOperator::NEG, f.imp() );
  return RnMap( nop );
}





// ///////////////////////////////////////////////////////////////////////////////
// // class RnImplicitMap
// ///////////////////////////////////////////////////////////////////////////////

// // ------------------------- Standard services ------------------------------

// /**
//  * Destructor. 
//  */
// RnImplicitMap::~RnImplicitMap()
// {
//   ASSERT_RnMaps( m_implicit_map != 0 );
//   delete m_implicit_map;
//   ASSERT_RnMaps( m_transformation != 0 );
//   delete m_transformation;
// }


// /**
//  * Creates the mapping (imp,Id).
//  * @param imp the implicit mapping.
//  */
// RnImplicitMap::RnImplicitMap( const RnMap & imp )
// {
//   ASSERT_RnMaps( imp.dimOut() == 1 );
//   m_implicit_map = imp->clone();
//   m_transformation = new RnMapIdentity;
// }


// /**
//  * Creates the mapping (imp,g).
//  * @param imp the implicit mapping.
//  * @param g a mapping.
//  */
// RnImplicitMap::RnImplicitMap( const RnMap & imp, const RnMap & g )
// {
//   ASSERT_RnMaps( imp.dimOut() == 1 );
//   m_implicit_map = imp->clone();
//   m_transformation = g->clone();
// }


// /**
//  * Copy constructor.
//  * @param other the object to copy.
//  */
// RnImplicitMap::RnImplicitMap( const RnImplicitMap & other )
// {
//   m_implicit_map = other.m_implicit_map->clone();
//   m_transformation = other.m_transformation->clone();
// }


// /**
//  * Assignment.
//  * @param other the object to copy.
//  * @return a reference to 'this'.
//  */
// RnImplicitMap& 
// RnImplicitMap::operator=( const RnImplicitMap & other )
// {
//   if ( this != &other )
//     {
//       ASSERT_RnMaps( m_transformation != 0 );
//       delete m_transformation;
//       ASSERT_RnMaps( m_implicit_map != 0 );
//       delete m_implicit_map;
//       m_implicit_map = other.m_implicit_map->clone();
//       m_transformation = other.m_transformation->clone();
//     }
//   return *this;
// }

  

// ///////////////////////////////////////////////////////////////////////////////
// // ------------------------- operator services ------------------------------


// /**
//  * Post-composition operator.
//  * @param g any mapping.
//  * @return the mapping (imp,f(g(x)) from (imp,f).
//  */
// RnImplicitMap
// RnImplicitMap::operator()( const RnMap & g ) const
// {
//   return RnImplicitMap( *m_implicit_map, (*m_transformation)( g ) );
// }


// /**
//  * Intersection service.
//  * @param m any implicit mapping (imp2,g)
//  * @return the mapping 'max(imp(f(x)),imp2(g(x)))' from (imp,f)
//  */
// RnImplicitMap 
// RnImplicitMap::max( const RnImplicitMap & m ) const
// {
//   return RnMapMaxOp;
  
// }


// /**
//  * Union service.
//  * @param m any implicit mapping (imp2,g)
//  * @return the mapping 'max(imp(f(x)),imp2(g(x)))' from (imp,f)
//  */
// RnImplicitMap 
// RnImplicitMap::min( const RnImplicitMap & m ) const;


// /**
//  * Pre-composition operator.
//  * @param g any mapping.
//  * @param m any implicit mapping (imp,f).
//  * @return the mapping (imp,g(f(x))) from (imp,f).
//  */
// RnImplicitMap operator()( const RnMap & g, const RnImplicitMap & m );





///////////////////////////////////////////////////////////////////////////////
// class RnMapIdentity
///////////////////////////////////////////////////////////////////////////////

/**
 * Destructor. 
 */
RnMapIdentity::~RnMapIdentity()
{
}


/**
 * Constructor.
 * Creates the identity mapping.
 */
RnMapIdentity::RnMapIdentity()
{
}

  
/**
 * Clone service.
 * @return a dyn. alloc. clone of 'this'.
 */
RnMapImp* 
RnMapIdentity::clone() const
{
  return new RnMapIdentity;
}

/**
 * @return the dimension of mapping domain (0 is any).
 */
uint
RnMapIdentity::dimIn() const
{
  return 0;
}

/**
 * @return the dimension of mapping range (0 is same as in).
 */
uint
RnMapIdentity::dimOut() const
{
  return 0;
}

/**
 * Mapping operator.
 * @param x the in-vector
 * @return the out vector (ie [x]).
 */
Vector
RnMapIdentity::eval( const Vector & x ) const
{
  return x;
}





///////////////////////////////////////////////////////////////////////////////
// class RnMapComposer
///////////////////////////////////////////////////////////////////////////////

/**
 * Destructor. 
 */
RnMapComposer::~RnMapComposer()
{
  delete m_g;
  delete m_f;
}


/**
 * Constructor from two mappings.
 * Creates the mapping 'f o g'. We must have 'g.dimOut() == f.dimIn()'.
 * @param f the first mapping
 * @param g the second mapping.
 */
RnMapComposer::RnMapComposer( const RnMapImp & f, const RnMapImp & g )
{
  m_f = f.clone();
  m_g = g.clone();
}

/**
 * Clone service.
 * @return a dyn. alloc. clone of 'this'.
 */
RnMapImp* 
RnMapComposer::clone() const
{
  return new RnMapComposer( *m_f, *m_g );
}

/**
 * @return the dimension of mapping domain (0 is any).
 */
uint
RnMapComposer::dimIn() const
{
  return m_g->dimIn();
}

/**
 * @return the dimension of mapping range (0 is same as in).
 */
uint
RnMapComposer::dimOut() const
{
  return m_f->dimOut();
}


/**
 * Mapping operator.
 * @param x the in-vector
 * @return the out vector.
 */
Vector
RnMapComposer::eval( const Vector & x ) const
{
  // @TODO JOL: 2004/08 proper RnMap evaluation.
//   ASSERT_RnMaps( ( ( m_g->dimIn() == 0 ) || ( m_g->dimIn() == x.size() ) )
// 		 && ( ( ( m_g->dimOut() == 0 ) 
// 			&& ( ( m_f->dimIn() == x.size() )
// 			     || ( m_f->dimIn() == 0 ) ) )
// 		      || ( m_g->dimOut() == m_f->dimIn() ) ) );
		 
  return m_f->eval( m_g->eval( x ) );
}






///////////////////////////////////////////////////////////////////////////////
// class RnMapBinaryOperator
///////////////////////////////////////////////////////////////////////////////

/**
 * Destructor. 
 */
RnMapBinaryOperator::~RnMapBinaryOperator()
{
  ASSERT_RnMaps( m_g != 0 );
  delete m_g;
  ASSERT_RnMaps( m_f != 0 );
  delete m_f;
}


/**
 * Constructor from two mappings.
 * Creates the mapping 'op(f(x),g(x))'. 
 * One must have 'f.dimOut() == g.dimOut()'.
 * @param op the binary operator.
 * @param f the first mapping
 * @param g the second mapping.
 */
RnMapBinaryOperator::RnMapBinaryOperator
( RnMapBinaryOperator::oper2 op, const RnMapImp & f, const RnMapImp & g )
  : m_op( op )
{
  m_f = f.clone();
  m_g = g.clone();
}

  
/**
 * @return the dimension of mapping domain (0 is any).
 */
uint
RnMapBinaryOperator::dimIn() const
{
  return ( m_f->dimIn() == 0 ) ? m_g->dimIn() : m_f->dimIn();
}

/**
 * @return the dimension of mapping range (0 is same as in).
 */
uint
RnMapBinaryOperator::dimOut() const
{
  return ( m_f->dimOut() == 0 ) ? m_g->dimOut() : m_f->dimOut();
}

  
/**
 * Clone service.
 * @return a dyn. alloc. clone of 'this'.
 */
RnMapImp* 
RnMapBinaryOperator::clone() const
{
  return new RnMapBinaryOperator( m_op, *(m_f->clone()), *(m_g->clone()) );
}


/**
 * Mapping operator.
 * @param x the in-vector
 * @return the out vector.
 */
Vector
RnMapBinaryOperator::eval( const Vector & x ) const
{
  Vector v1 = m_f->eval( x );
  Vector v2 = m_g->eval( x );

  ASSERT_RnMaps( v1.size() == v2.size() );
  uint k = 0; // for loops
  uint n = v1.size();
  Vector v( n ); // result
  switch ( m_op ) 
    {
    case ADD: 
      for ( ; k < n; ++k ) 
	v.rw( k ) = v1.ro( k ) + v2.ro( k );
      break;
    case SUB: 
      for ( ; k < n; ++k ) 
	v.rw( k ) = v1.ro( k ) - v2.ro( k );
      break;
    case MUL: 
      for ( ; k < n; ++k ) 
	v.rw( k ) = v1.ro( k ) * v2.ro( k );
      break;
    case DIV: 
      for ( ; k < n; ++k ) 
	v.rw( k ) = v1.ro( k ) / v2.ro( k );
      break;
    case MIN: 
      for ( ; k < n; ++k ) 
	v.rw( k ) = ( v1.ro( k ) <= v2.ro( k ) ) ? v1.ro( k ) : v2.ro( k );
      break;
    case MAX: 
      for ( ; k < n; ++k ) 
	v.rw( k ) = ( v1.ro( k ) >= v2.ro( k ) ) ? v1.ro( k ) : v2.ro( k );
      break;
    }
  return v;
}



///////////////////////////////////////////////////////////////////////////////
// class RnMapUnaryOperator
///////////////////////////////////////////////////////////////////////////////

/**
 * Destructor. 
 */
RnMapUnaryOperator::~RnMapUnaryOperator()
{
  ASSERT_RnMaps( m_f != 0 );
  delete m_f;
}


/**
 * Constructor from two mappings.
 * Creates the mapping 'op(f(x),g(x))'. 
 * One must have 'f.dimOut() == g.dimOut()'.
 * @param op the binary operator.
 * @param f the first mapping
 * @param g the second mapping.
 */
RnMapUnaryOperator::RnMapUnaryOperator
( RnMapUnaryOperator::oper1 op, const RnMapImp & f )
  : m_op( op )
{
  m_f = f.clone();
}

  
/**
 * @return the dimension of mapping domain (0 is any).
 */
uint
RnMapUnaryOperator::dimIn() const
{
  return m_f->dimIn();
}

/**
 * @return the dimension of mapping range (0 is same as in).
 */
uint
RnMapUnaryOperator::dimOut() const
{
  return m_f->dimOut();
}

  
/**
 * Clone service.
 * @return a dyn. alloc. clone of 'this'.
 */
RnMapImp* 
RnMapUnaryOperator::clone() const
{
  return new RnMapUnaryOperator( m_op, *(m_f->clone()) );
}


/**
 * Mapping operator.
 * @param x the in-vector
 * @return the out vector.
 */
Vector
RnMapUnaryOperator::eval( const Vector & x ) const
{
  Vector v1 = m_f->eval( x );

  uint k = 0; // for loops
  uint n = v1.size();
  Vector v( n ); // result
  switch ( m_op ) 
    {
    case NEG: 
      for ( ; k < n; ++k ) 
	v.rw( k ) = -v1.ro( k );
      break;
    }
  return v;
}




///////////////////////////////////////////////////////////////////////////////
// class RnMapTranslater
///////////////////////////////////////////////////////////////////////////////

/**
 * Destructor. 
 */
RnMapTranslater::~RnMapTranslater()
{
}


/**
 * Constructor from two mappings.
 * Creates the translater mapping of vector [v]. Add [v] to any vector [x]
 * given in parameter in 'apply'.
 * @param v the translation vector.
 */
RnMapTranslater::RnMapTranslater( const Vector & v )
  : m_v( v )
{
}

  
/**
 * Clone service.
 * @return a dyn. alloc. clone of 'this'.
 */
RnMapImp* 
RnMapTranslater::clone() const
{
  return new RnMapTranslater( m_v );
}


/**
 * @return the dimension of mapping domain (0 is any).
 */
uint
RnMapTranslater::dimIn() const
{
  return m_v.size();
}

/**
 * @return the dimension of mapping range (0 is same as in).
 */
uint
RnMapTranslater::dimOut() const
{
  return m_v.size();
}


/**
 * Mapping operator.
 * [x] must have the same dimension as [m_v].
 * @param x the in-vector
 * @return the out vector.
 */
Vector
RnMapTranslater::eval( const Vector & x ) const
{
  ASSERT_RnMaps( x.size() == dimIn() );

  Vector t( m_v );
  t += x;
  return t;
}




///////////////////////////////////////////////////////////////////////////////
// class RnMapConstantDotProduct
///////////////////////////////////////////////////////////////////////////////

// ------------------------- Standard services ------------------------------

/**
 * Destructor. 
 */
RnMapConstantDotProduct::~RnMapConstantDotProduct()
{
}


/**
 * Constructor.
 * Creates the dot product mapping of vector [v]. Performs the scalar
 * product of [v] with any vector [x] given in parameter in 'eval'.
 * @param v the vector.
 */
RnMapConstantDotProduct::RnMapConstantDotProduct( const Vector & v )
  : m_v( v )
{
}

  
/**
 * @return the dimension of mapping domain (0 is any).
 */
uint
RnMapConstantDotProduct::dimIn() const
{
  return m_v.size();
}

/**
 * @return the dimension of mapping range (0 is same as in).
 */
uint
RnMapConstantDotProduct::dimOut() const
{
  return 1;
}

  
/**
 * Clone service.
 * @return a dyn. alloc. clone of 'this'.
 */
RnMapImp* 
RnMapConstantDotProduct::clone() const
{
  return new RnMapConstantDotProduct( m_v );
}


/**
 * Mapping operator.
 * [x] must have the same dimension as [m_v].
 * @param x the in-vector
 * @return the out vector.
 */
Vector
RnMapConstantDotProduct::eval( const Vector & x ) const
{
  Vector sum( 1 );
  sum.rw( 0 ) = 0.0;
  for ( uint k = 0; k < x.size(); ++k )
    sum.rw( 0 ) += m_v.ro( k ) * x.ro( k );
  return sum;
}

  



///////////////////////////////////////////////////////////////////////////////
// class RnMapMatrixMultiplier
///////////////////////////////////////////////////////////////////////////////

/**
 * Destructor. 
 */
RnMapMatrixMultiplier::~RnMapMatrixMultiplier()
{
}


/**
 * Constructor from matrix.
 * Creates the matrix multiplier mapping of matrix [m]. Multiplies [m] to any
 * vector [x] given in parameter in 'apply'.
 * @param m the matrix.
 */
RnMapMatrixMultiplier::RnMapMatrixMultiplier( const Matrix & m )
  : m_m( m )
{
}

/**
 * Clone service.
 * @return a dyn. alloc. clone of 'this'.
 */
RnMapImp* 
RnMapMatrixMultiplier::clone() const
{
  return new RnMapMatrixMultiplier( m_m );
}


/**
 * @return the dimension of mapping domain (0 is any).
 */
uint
RnMapMatrixMultiplier::dimIn() const
{
  return m_m.columns();
}

/**
 * @return the dimension of mapping range (0 is same as in).
 */
uint
RnMapMatrixMultiplier::dimOut() const
{
  return m_m.lines();
}


/**
 * Mapping operator.
 * [x] must have the same dimension as [m_v].
 * @param x the in-vector
 * @return the out vector.
 */
Vector
RnMapMatrixMultiplier::eval( const Vector & x ) const
{
  ASSERT_RnMaps( x.size() == dimIn() );

  Vector t( dimOut() );
  m_m.rMult( x, t );
  return t;
}



///////////////////////////////////////////////////////////////////////////////
// class RnMapImplicitSphere
///////////////////////////////////////////////////////////////////////////////

/**
 * Destructor. 
 */
RnMapImplicitSphere::~RnMapImplicitSphere()
{
}


/**
 * Constructor.
 * Creates the implicit sphere mapping of radius [r]. 
 * @param r the radius of the sphere.
 */
RnMapImplicitSphere::RnMapImplicitSphere( float r )
  : m_r( r )
{
}

  
/**
 * Clone service.
 * @return a dyn. alloc. clone of 'this'.
 */
RnMapImp* 
RnMapImplicitSphere::clone() const
{
  return new RnMapImplicitSphere( m_r );
}


/**
 * @return the dimension of mapping domain (0 is any).
 */
uint
RnMapImplicitSphere::dimIn() const
{
  return 0;
}

/**
 * @return the dimension of mapping range (0 is same as in).
 */
uint
RnMapImplicitSphere::dimOut() const
{
  return 1;
}


/**
 * Mapping operator.
 * @param x the in-vector
 * @return the out vector.
 */
Vector
RnMapImplicitSphere::eval( const Vector & x ) const
{
  Vector sum( 1 );
  sum.rw( 0 ) = - m_r * m_r;
  for ( uint k = 0; k < x.size(); ++k )
    sum.rw( 0 ) += x.ro( k ) * x.ro( k );
  return sum;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
