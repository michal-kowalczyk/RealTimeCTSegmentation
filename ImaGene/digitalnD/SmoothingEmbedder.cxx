///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : SmoothingEmbedder.cxx
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
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/DigitalSurfaceGeometry.h"
#include "ImaGene/digitalnD/SmoothingEmbedder.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/SmoothingEmbedder.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const SmoothingEmbedder_RCS_ID = "@(#)class SmoothingEmbedder definition.";

///////////////////////////////////////////////////////////////////////////////
// class SmoothingEmbedder
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
ImaGene::SmoothingEmbedder::uembed( Kn_uid p, float* coords ) const
{
  ASSERT_SmoothingEmbedder( false );
}


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any signed cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void
ImaGene::SmoothingEmbedder::sembed( Kn_sid p, float* coords ) const
{
  ASSERT_SmoothingEmbedder( space()->sisSurfel( p ) );

  uint d = space()->dim();
  Vector n( d );
  Vector v( space()->scentroid( p ) );
  float out_x, in_x;
  
  m_geom_ctxt->tangentPlane( p, n, out_x, in_x );
  uint j = space()->sorthDir( p );
  float m = ( out_x + in_x ) / 2.0;
  //m = ( m > 0.4999 ) ? 0.4999 : ( ( m < -0.4999 ) ? -0.4999 : m );

  // Choose one of (1) and (2)

  // (1) Project onto mid-plane following direction normal to plane
  float mj_dot_u = m * n.ro( j );
  for ( uint i = 0; i < d; ++i ) 
    coords[ i ] = v.ro( i ) + mj_dot_u * n.ro( i );

//  	  // (2) Project onto mid-plane following direction of trivial normal
//  	  for ( uint i = 0; i < 3; ++i ) 
//  	    coords[ i ] = v.ro( i );
//  	  coords[ j ] -= m;

}


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any unsigned cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 * @see uembed( Kn_uid p, float* coords )
 * NB: calls 'uembed', could be optimized.
 */
void
ImaGene::SmoothingEmbedder::uembed( Kn_uid p, Vector & coords ) const
{
  ASSERT_SmoothingEmbedder( false );
}


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any signed cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 * @see sembed( Kn_uid p, float* coords )
 * NB: calls 'sembed', could be optimized.
 */
void
ImaGene::SmoothingEmbedder::sembed( Kn_sid p, Vector & coords ) const
{
  ASSERT_SmoothingEmbedder( space()->sisSurfel( p ) );

  uint d = space()->dim();
  space()->scentroid( p, coords );
  Vector n( d );
  float out_x, in_x;
  
  m_geom_ctxt->tangentPlane( p, n, out_x, in_x );
  uint j = space()->sorthDir( p );
  float m = ( out_x + in_x ) / 2.0;
  //m = ( m > 0.4999 ) ? 0.4999 : ( ( m < -0.4999 ) ? -0.4999 : m );

  // Choose one of (1) and (2)

  // (1) Project onto mid-plane following direction normal to plane
  float mj_dot_u = m * n.ro( j );
  for ( uint i = 0; i < d; ++i ) 
    coords.rw( i ) += mj_dot_u * n.ro( i );

//  	  // (2) Project onto mid-plane following direction of trivial normal
//  	  for ( uint i = 0; i < 3; ++i ) 
//  	    coords[ i ] = v.ro( i );
//  	  coords[ j ] -= m;

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
ImaGene::SmoothingEmbedder::embedVector( const float* p, float* coords ) const
{
  uint n = space()->dim();
  for ( uint i = 0; i < n; ++i )
    coords[ i ] = p[ i ];
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
ImaGene::SmoothingEmbedder::embedVector( const Vector & p, 
					 Vector & coords ) const
{
  coords = p;
}





/**
 * @param coords any coordinates in the space.
 * @return the unsigned spel whose embedding is the closest to [coords].
 */
ImaGene::Kn_uid
ImaGene::SmoothingEmbedder::uinverseSpel( const float* coords ) const
{
  uint d = space()->dim();
  Kn_size* x = new Kn_size[ d ];
  for ( uint i = 0; i < d; ++i )
    {
      float c = coords[ i ];
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
ImaGene::SmoothingEmbedder::uinverseSpel( const Vector & coords ) const
{
  ASSERT_SmoothingEmbedder( coords.size() == space()->dim() );
  uint d = coords.size();
  Kn_size* x = new Kn_size[ d ];
  for ( uint i = 0; i < d; ++i )
    {
      float c = coords.ro( i );
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
