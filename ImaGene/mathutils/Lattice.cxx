///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Lattice.cxx
//
// Creation : 2008/02/15
//
// Version : 2008/02/15
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
//	2008/02/15 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/Lattice.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/Lattice.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const Lattice_RCS_ID = "@(#)class Lattice definition.";



///////////////////////////////////////////////////////////////////////////////
// class Lattice
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::Lattice::~Lattice()
{
  if ( m_x0 != 0 ) delete[] m_x0;
  if ( m_v != 0 ) delete[] m_v;
}


/**
 * Constructor.
 */
ImaGene::Lattice::Lattice()
  : m_n( 0 ), m_m( 0 ), m_x0( 0 ), m_v( 0 )
{
}

/**
 * Defines the standard lattice when n=m, the standard injection
 * when n < m, and when m > n, vectors are overlapping.
 *
 * @param n the dimension of the lattice space
 * @param m the dimension of the target space where the lattice is represented.
 * @param dh the length of the elementary displacement.
 */
void ImaGene::Lattice::init( uint n, uint m, double dh )
{
  ASSERT_Lattice( n > 0 );
  ASSERT_Lattice( m > 0 );
  if ( ( m != m_m ) || ( n != m_n ) )
    {
      m_n = n;
      m_m = m;
      if ( m_x0 != 0 ) delete[] m_x0;
      if ( m_v != 0 ) delete[] m_v;
      m_x0 = new double[ m ];
      m_v = new double[ m*n ];
    }
  for ( uint j = 0; j < m; j++ )
    m_x0[ j ] = 0.0;
  for ( uint i = 0; i < n; i++ )
    {
      for ( uint j = 0; j < m; j++ )
	m_v[ m*i + j ] = ( j == ( i % m ) ) ? dh : 0.0;  
    }
}

/**
 * Sets the origin of the space.
 * @param x is a vector of size m.
 */
void ImaGene::Lattice::setOrigin( const double* x )
{
  for ( uint j = 0; j < m_m; j++ )
    m_x0[ j ] = x[ j ];
}


/**
 * Sets the elementary vector along direction i
 * @param i is a coordinate between 0 and n-1.
 * @param v is a vector of size m.
 */
void ImaGene::Lattice::setVector( uint i, const double* v )
{
  double* off_v = m_v + m_m*i;
  for ( uint j = 0; j < m_m; j++ )
    off_v[ j ] = v[ j ];
}

/**
 * Immerse a point of the lattice into the target space of dimension m.
 *
 * @param p the n coordinates of a point in the lattice.
 * @param x (returns) its m coordinates in the target space of dimension m.
 */
void ImaGene::Lattice::immerse( const lattice_int* p, double* x ) const
{
  for ( uint j = 0; j < m_m; j++ )
    x[ j ] = m_x0[ j ];

  double* off_v = m_v;
  for ( uint i = 0; i < m_n; i++ )
    {
      lattice_int pi = p[ i ];
      for ( uint j = 0; j < m_m; j++ )
	{
	  x[ j ] += (*off_v) * pi;
	  ++off_v;
	}
    }  
}

/**
 * Immerse a point of the lattice into the target space of dimension m.
 *
 * @param p the n coordinates of a point in the lattice.
 * @param x (returns) its m coordinates in the target space of dimension m.
 */
void ImaGene::Lattice::immerse( const lattice_int* p, float* x ) const
{
  for ( uint j = 0; j < m_m; j++ )
    x[ j ] = (float) m_x0[ j ];

  double* off_v = m_v;
  for ( uint i = 0; i < m_n; i++ )
    {
      lattice_int pi = p[ i ];
      for ( uint j = 0; j < m_m; j++ )
	{
	  x[ j ] += (float) ((*off_v) * pi);
	  ++off_v;
	}
    }  
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::Lattice::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Lattice]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::Lattice::OK() const
{
  return true;
}




///////////////////////////////////////////////////////////////////////////////
// Static services


/**
 * Constructs a default 2D lattice centered at [x0], with vectors
 * (dh,0,0) and (0,dh,0).
 *
 * @param l (returns) the lattice
 * @param x0 the origin of the lattice embedding (a 3D point)
 * @param dh the grid step or length of each vector.
 */
void
ImaGene::Lattice::defaultZ2toZ3( Lattice & l, const double* x0, double dh )
{
  l.init( 2, 3, dh );
  l.setOrigin( x0 );
} 

/**
 * Constructs a default 3D lattice centered at [x0], with vectors
 * (dh,0,0) and (0,dh,0) and (0,0,dh).
 *
 * @param l (returns) the lattice
 * @param x0 the origin of the lattice embedding (a 3D point)
 * @param dh the grid step or length of each vector.
 */
void
ImaGene::Lattice::defaultZ3toZ3( Lattice & l, const double* x0, double dh )
{
  l.init( 3, 3, dh );
  l.setOrigin( x0 );
}


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
