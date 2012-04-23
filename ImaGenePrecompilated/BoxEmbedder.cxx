///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : BoxEmbedder.cxx
//
// Creation : 2008/03/19
//
// Version : 2008/03/19
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
//	2008/03/19 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/BoxEmbedder.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/BoxEmbedder.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const BoxEmbedder_RCS_ID = "@(#)class BoxEmbedder definition.";



///////////////////////////////////////////////////////////////////////////////
// class BoxEmbedder
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

///////////////////////////////////////////////////////////////////////////////
// ----------------------- Embedding services ------------------------------

/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any unsigned cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void
ImaGene::BoxEmbedder::uembed( Kn_uid p, float* coords ) const
{
  Vector v( space()->ucentroid( p ) );
  for ( uint i = 0; i < v.size(); ++i )
    coords[ i ] = m_x1[ i ] + v.ro( i ) * m_dh[ i ];
}


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any signed cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void
ImaGene::BoxEmbedder::sembed( Kn_sid p, float* coords ) const
{
  Vector v( space()->scentroid( p ) );
  for ( uint i = 0; i < v.size(); ++i )
    coords[ i ] = m_x1[ i ] + v.ro( i ) * m_dh[ i ];
}


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any unsigned cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 * @see uembed( Kn_uid p, float* coords )
 * NB: calls 'uembed', could be optimized.
 */
void
ImaGene::BoxEmbedder::uembed( Kn_uid p, Vector & coords ) const
{
  ASSERT_BoxEmbedder( coords.size() == space()->dim() );
  space()->ucentroid( p, coords );
  for ( uint i = 0; i < coords.size(); ++i )
    {
      coords.rw( i ) *= m_dh[ i ];
      coords.rw( i ) += m_x1[ i ];
    }
}


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any signed cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 * @see sembed( Kn_uid p, float* coords )
 * NB: calls 'sembed', could be optimized.
 */
void
ImaGene::BoxEmbedder::sembed( Kn_sid p, Vector & coords ) const
{
  ASSERT_BoxEmbedder( coords.size() == space()->dim() );
  space()->scentroid( p, coords );
  for ( uint i = 0; i < coords.size(); ++i )
    {
      coords.rw( i ) *= m_dh[ i ];
      coords.rw( i ) += m_x1[ i ];
    }
}



///////////////////////////////////////////////////////////////////////////////
// ----------------------- Other embedding services -----------------------


/**
 * Embeds the vector [p] in the Euclidean space as a point.  By
 * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
 * ), coords ).
 *
 *
 * @param p any vector expressed in the discrete space.  
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void
ImaGene::BoxEmbedder::embedVector( const float* p, float* coords ) const
{
  uint n = space()->dim();
  for ( uint i = 0; i < n; ++i )
    coords[ i ] = p[ i ] * m_dh[ i ];
}


/**
 * Embeds the vector [p] in the Euclidean space as a point.  By
 * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
 * ), coords ).
 *
 *
 * @param p any vector expressed in the discrete space.  
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void
ImaGene::BoxEmbedder::embedVector( const Vector & p, 
				   Vector & coords ) const
{
  ASSERT_BoxEmbedder( coords.size() == space()->dim() );
  uint n = coords.size(); 
  for ( uint i = 0; i < n; ++i )
    coords.rw( i ) = p.ro( i ) * m_dh[ i ];
}



/**
 * @param coords any coordinates in the space.
 * @return the unsigned spel whose embedding is the closest to [coords].
 */
ImaGene::Kn_uid
ImaGene::BoxEmbedder::uinverseSpel( const float* coords ) const
{
  uint d = space()->dim();
  Kn_size x[ d ];
  for ( uint i = 0; i < d; ++i )
    {
      float c = coords[ i ];
      if ( c <= m_x1[ i ] )
	x[ i ] = 0;
      else if ( c >= m_x2[ i ] )
	x[ i ] = space()->size( i ) - 1;
      else
	x[ i ] = (uint) floor( ( c - m_x1[ i ] ) / m_dh[ i ] );
    }
  Kn_uid p = space()->ucode( x, space()->ufirstCell( d ) );
  return p;
}

  
/**
 * @param coords any coordinates in the space.
 * @return the unsigned spel whose embedding is the closest to [coords].
 */
ImaGene::Kn_uid
ImaGene::BoxEmbedder::uinverseSpel( const Vector & coords ) const
{
  ASSERT_BoxEmbedder( coords.size() == space()->dim() );
  uint d = coords.size();
  Kn_size x[ d ];
  for ( uint i = 0; i < d; ++i )
    {
      float c = coords.ro( i );
      if ( c <= m_x1[ i ] )
	x[ i ] = 0;
      else if ( c >= m_x2[ i ] )
	x[ i ] = space()->size( i ) - 1;
      else
	x[ i ] = (uint) floor( ( c - m_x1[ i ] ) / m_dh[ i ] );
    }
  Kn_uid p = space()->ucode( x, space()->ufirstCell( d ) );
  return p;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
