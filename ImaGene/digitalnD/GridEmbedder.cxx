///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : GridEmbedder.cxx
//
// Creation : 2004/02/04
//
// Version : 2004/02/04
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
//	2004/02/04 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/GridEmbedder.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/GridEmbedder.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const GridEmbedder_RCS_ID = "@(#)class GridEmbedder definition.";

///////////////////////////////////////////////////////////////////////////////
// class GridEmbedder
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
ImaGene::GridEmbedder::uembed( Kn_uid p, float* coords ) const
{
  *m_tmp = space()->ucentroid( p );
  // Vector v( space()->ucentroid( p ) );
  for ( uint i = 0; i < m_tmp->size(); ++i )
    coords[ i ] = ( m_tmp->ro( i ) - m_center->ro( i ) ) * m_dh;
}


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any signed cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void
ImaGene::GridEmbedder::sembed( Kn_sid p, float* coords ) const
{
  *m_tmp = space()->scentroid( p );
  // Vector v( space()->scentroid( p ) );
  for ( uint i = 0; i < m_tmp->size(); ++i )
    coords[ i ] = ( m_tmp->ro( i ) - m_center->ro( i ) ) * m_dh;
}


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any unsigned cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 * @see uembed( Kn_uid p, float* coords )
 * NB: calls 'uembed', could be optimized.
 */
void
ImaGene::GridEmbedder::uembed( Kn_uid p, Vector & coords ) const
{
  space()->ucentroid( p, coords );
  coords -= *m_center;
  coords *= m_dh;
}


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any signed cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 * @see sembed( Kn_uid p, float* coords )
 * NB: calls 'sembed', could be optimized.
 */
void
ImaGene::GridEmbedder::sembed( Kn_sid p, Vector & coords ) const
{
  space()->scentroid( p, coords );
  coords -= *m_center;
  coords *= m_dh;
}


///////////////////////////////////////////////////////////////////////////////
// ----------------------- Other embedding services -----------------------

/**
 * Embeds the vector [p] in the Euclidean space as a point.  By
 * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
 * ), coords ).
 *
 
 * @param p any vector expressed in the discrete space.  
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void
ImaGene::GridEmbedder::embedVector( const float* p, float* coords ) const
{
  uint n = space()->dim();
  for ( uint i = 0; i < n; ++i )
    coords[ i ] = ( p[ i ] - m_center->ro( i ) ) * m_dh;
}


/**
 * Embeds the vector [p] in the Euclidean space as a point.  By
 * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
 * ), coords ).
 *
 
 * @param p any vector expressed in the discrete space.  
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void 
ImaGene::GridEmbedder::embedVector( const Vector & p, 
				    Vector & coords ) const
{
  coords = p;
  coords -= *m_center;
  coords *= m_dh;
}





/**
 * @param coords any coordinates in the space.
 * @return the unsigned spel whose embedding is the closest to [coords].
 */
ImaGene::Kn_uid
ImaGene::GridEmbedder::uinverseSpel( const float* coords ) const
{
  uint d = space()->dim();
  Kn_size* x = new Kn_size[ d ];
  for ( uint i = 0; i < d; ++i )
    {
      float c = coords[ i ] / m_dh + m_center->ro( i );
      if ( c <= 0.0 )
	x[ i ] = 0;
      else if ( c >= (float) space()->size( i ) ) 
	x[ i ] = space()->size( i ) - 1;
      else
	x[ i ] = (Kn_size) floor( c );
    }
  Kn_uid p = space()->ucode( x, space()->ufirstCell( d ) );
  delete[] x;
  return p;
}

  
/**
 * @param coords any coordinates in the space.
 * @return the unsigned spel whose embedding is the closest to [coords].
 */
ImaGene::Kn_uid
ImaGene::GridEmbedder::uinverseSpel( const Vector & coords ) const
{
  ASSERT_GridEmbedder( coords.size() == space()->dim() );
  uint d = coords.size();
  Kn_size* x = new Kn_size[ d ];
  for ( uint i = 0; i < d; ++i )
    {
      float c = coords.ro( i ) / m_dh + m_center->ro( i );
      if ( c <= 0.0 )
	x[ i ] = 0;
      else if ( c >= (float) space()->size( i ) ) 
	x[ i ] = space()->size( i ) - 1;
      else
	x[ i ] = (Kn_size) floor( c );
    }
  Kn_uid p = space()->ucode( x, space()->ufirstCell( d ) );
  delete[] x;
  return p;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
