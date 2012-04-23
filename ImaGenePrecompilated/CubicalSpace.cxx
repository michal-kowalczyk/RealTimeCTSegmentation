///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : CubicalSpace.cxx
//
// Creation : 2008/03/20
//
// Version : 2008/03/20
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
//	2008/03/20 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnUtils.h"
#include "ImaGene/digitalnD/CubicalSpace.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/CubicalSpace.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const CubicalSpace_RCS_ID = "@(#)class CubicalSpace definition.";



///////////////////////////////////////////////////////////////////////////////
// class CubicalSpace
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::CubicalSpace::~CubicalSpace()
{
  if ( m_knspaces != 0 )
    {
      for ( uint i = 0; i <= m_k; ++i )
	delete m_knspaces[ i ];
      delete[] m_knspaces;
    }
  if ( m_ext_knspaces != 0 )
    {
      for ( uint i = 0; i <= m_k; ++i )
	delete m_ext_knspaces[ i ];
      delete[] m_ext_knspaces;
    }
  if ( m_x1 != 0 ) delete[] m_x1;
  if ( m_x2 != 0 ) delete[] m_x2;
  if ( m_steps != 0 )
    {
      for ( uint j = 0; j < m_dim; ++j )
	delete[] m_steps[ j ];
      delete[] m_steps;
    }
}


/**
 * Constructor. Need dimension of space and embedding box.
 *
 * @param dim the dimension of the space.
 * @param x1 the lower-bound of the coordinates of the embedding
 * box.
 *
 * @param x2 it is the upper-bound of the coordinates of the
 * embedding box.
 *
 * @param k the number of 2^i digital spaces to compute, 0<=i<=k.
 */
ImaGene::CubicalSpace::CubicalSpace
( uint dim, const float* x1, const float* x2, uint k )
  : m_dim( dim ), m_k ( k )
{
  Kn_size n[ dim+1 ];
  m_x1 = new float[ dim ];
  m_x2 = new float[ dim ];
  m_steps = new float*[ dim ];
  for ( uint j = 0; j < dim; ++j )
    {
      n[ j ] = 1;
      m_x1[ j ] = x1[ j ];
      m_x2[ j ] = x2[ j ];
      m_steps[ j ] = new float[ 64 ];
    }
  n[ dim ] = 2;
  m_knspaces = new KnSpace*[ k+1 ];
  m_ext_knspaces = new KnSpace*[ k+1 ];
  for ( uint i = 0; i <= k; ++i )
    {
      m_knspaces[ i ] = new KnSpace( dim, n );
      m_ext_knspaces[ i ] = new KnSpace( dim + 1, n );
      for ( uint j = 0; j < dim; ++j )
	n[ j ] <<= 1;
    }
  for ( uint j = 0; j < dim; ++j )
    {
      uint64 b = 1;
      for ( uint i = 0; i < 64; ++i  )
	{
	  m_steps[ j ][ i ] = ( m_x2[ j ] - m_x1[ j ] ) / b;
	  b <<= 1;
	}
    }
}


// ----------------------- cubical space services -----------------------


/**
 * Given a cubical cell, return the space it spans as a box
 * embedder.  For instance, a n-1-cell spans a n-1-subspace. The
 * cell 3,3,3 spans the whole box.
 *
 * @param x1 (returns) the lower-bound of the coordinates of the
 * embedding box.
 *
 * @param x2 (returns) the upper-bound of the coordinates of the
 * embedding box.
 *
 * @param c a hierarchical cubical cell.
 */
void
ImaGene::CubicalSpace::getBoundingBox( float* x1, float* x2,
				       const CubicalCell & c ) const
{
  for ( uint i = 0; i < m_dim; ++i )
    {
      uint b = KnUtils::getMSBbyLog( c[ i ] );
      ASSERT_CubicalSpace( b > 0 && "Invalid cubical cell" );
      float gdh = m_steps[ i ][ b - 1 ];
      if ( c[ i ] & 0x1 )
	{
	  x1[ i ] = m_x1[ i ] 
	    + gdh * ( ( c[ i ] ^ ( 0x1LL << b ) ) >> 1 );
	  x2[ i ] = x1[ i ] + gdh;
	}
      else
	{
	  x1[ i ] = m_x1[ i ] 
	    + gdh * ( ( c[ i ] ^ ( 0x1LL << b ) ) >> 1 );
	  x2[ i ] = x1[ i ];
	}
    }
}

/**
 * @param x (returns) the centroid of the cell [c].
 *
 * @param c a hierarchical cubical cell.
 */
void
ImaGene::CubicalSpace::getCentroid( float* x, const CubicalCell & c ) const
{
  for ( uint i = 0; i < m_dim; ++i )
    {
      uint b = KnUtils::getMSBbyLog( c[ i ] );
      ASSERT_CubicalSpace( b > 0 && "Invalid cubical cell" );
      float gdh = m_steps[ i ][ b - 1 ];
      x[ i ] = m_x1[ i ] 
	+ gdh * ( ( c[ i ] ^ ( 0x1LL << b ) ) >> 1 );
      if ( c[ i ] & 0x1 )
	x[ i ] += 0.5f * gdh;
    }
}


/**
 * @param k the degree of the subdivision (from 0 to 9).
 * @return a regular 2^k x ... x 2^k digital space.
 */
ImaGene::KnSpace& 
ImaGene::CubicalSpace::getKnSpace( uint k ) const
{
  ASSERT_CubicalSpace( k <= m_k && "Subdivision is too fine." );
  
  return *m_knspaces[ k ];
} 


/**
 * @param k the degree of the subdivision (from 0 to 9).
 * @return a regular 2^k x ... x 2^k x 2 digital space.
 */
ImaGene::KnSpace& 
ImaGene::CubicalSpace::getExtKnSpace( uint k ) const
{
  ASSERT_CubicalSpace( k <= m_k && "Subdivision is too fine." );
  
  return *m_ext_knspaces[ k ];
} 


/**
 * This method returns the digital space of level [k] (sizes are
 * 2^k) and the associated box embedder, so that the digital space
 * spans the subspace defined by the hierarchical cubical cell
 * [c]. The digital space may be obtained either by
 * 'bemb->space()' or by this->getKnSpace( k )'.
 *
 * @param c a hierarchical cubical cell, that defines the spanned
 * subspace.
 *
 * @param k the subspace subdivision.
 *
 * @return a dyn. allocated box embedder spanning cell [c].
 */
ImaGene::BoxEmbedder* 
ImaGene::CubicalSpace::getEmbedder( const CubicalCell & c, uint k ) const
{
  float x1[ m_dim ];
  float x2[ m_dim ];
  this->getBoundingBox( x1, x2, c );
  return new BoxEmbedder( & this->getKnSpace( k ),
			  x1, x2, false );
}

/**
 * This method returns the digital space of level [k] (sizes are 2^k x
 * ... x 2^k x 2) and the associated box embedder, so that the digital
 * space spans the subspace defined by the hierarchical cubical cell
 * [c]. The digital space may be obtained either by 'bemb->space()' or
 * by this->getKnSpace( k )'. The embedding is centered around
 * [-thickness,+thickness] along last coordinate.
 *
 * @param c a hierarchical cubical cell, that defines the spanned
 * subspace.
 *
 * @param k the subspace subdivision.
 *
 * @param thickness the thickening of the space along the last
 * coordinate axis (positive, embedding is then in
 * [-thickness,+thickness]).
 *
 * @return a dyn. allocated box embedder spanning cell [c].
 */
ImaGene::BoxEmbedder* 
ImaGene::CubicalSpace::getExtEmbedder( const CubicalCell & c, 
				       uint k,
				       float thickness ) const
{
  float x1[ m_dim+1 ];
  float x2[ m_dim+1 ];
  this->getBoundingBox( x1, x2, c );
  x1[ m_dim ] = -thickness;
  x2[ m_dim ] = thickness;
  return new BoxEmbedder( & this->getExtKnSpace( k ),
			  x1, x2, false );
}

/**
 * This method returns the digital space of level [k+1] (sizes are 2^k
 * x ... x 2^k x 2) and the associated box embedder, so that the
 * digital space spans the doubled subspace defined by the
 * hierarchical cubical cell [c]. The digital space may be obtained
 * either by 'bemb->space()' or by this->getKnSpace( k )'. The
 * embedding is centered around [-thickness,+thickness] along last
 * coordinate.
 *
 * @param c a hierarchical cubical cell, that defines the spanned
 * subspace.
 *
 * @param k the subspace subdivision.
 *
 * @param thickness the thickening of the space along the last
 * coordinate axis (positive, embedding is then in
 * [-thickness,+thickness]).
 *
 * @return a dyn. allocated box embedder spanning cell [c].
 */
ImaGene::BoxEmbedder* 
ImaGene::CubicalSpace::getExtDoubledEmbedder( const CubicalCell & c, 
					      uint k,
					      float thickness ) const
{
  float x1[ m_dim+1 ];
  float x2[ m_dim+1 ];
  this->getBoundingBox( x1, x2, c );
  for ( uint i = 0; i < m_dim; ++i )
    x2[ i ] += ( x2[ i ] - x1[ i ] );
  x1[ m_dim ] = -thickness;
  x2[ m_dim ] = thickness;
  return new BoxEmbedder( & this->getExtKnSpace( k+1 ),
			  x1, x2, false );
}


/**
 * Compute the cubical cell corresponding to the digital cell [c] of
 * space [ks], where the space [ks] is a subdivision of the cell
 * [subspace_cell]. If the dimension of the digital space [ks] is
 * greater then the dimension of 'this', then the cell is simply
 * projected.
 *
 * @param subspace_cell the cubical cell (generally open) which
 * defines the subspace of the cubical space.
 *
 * @param ks a digital space, which is in fact a 2^k-subdivision of
 * [subspace_cell], whose dimension is at least the dimension of the
 * cubical space.
 *
 * @param c a digital cell
 *
 * @return the cubical cell analog to [c].
 */
ImaGene::CubicalCell
ImaGene::CubicalSpace::getCubicalCell( const CubicalCell & subspace_cell,
				       const KnSpace & ks, Kn_uid c ) const
{
  ASSERT_CubicalSpace( ks.dim() >= m_dim );
  CubicalCell ccell( subspace_cell, m_dim );
  for ( uint i = 0; i < m_dim; ++i )
    {
      uint xt = ks.ukdecodeCoord( c, i );
      if ( subspace_cell[ i ] & 0x1 )
	ccell[ i ] = ( ( ccell[ i ] & 0xfffffffffffffffeLL )
		       << ks.logSize( i ) ) + xt;
      else
	ccell[ i ] = ( ccell[ i ] << ks.logSize( i ) );
    }
  return ccell;
}

/**
 * Compute the cubical cells corresponding to the digital cells [qc]
 * of space [ks], where the space [ks] is a subdivision of the
 * cell [subspace_cell]. 
 *
 * @param qcc the list of the cubical cells analog to [qc].
 *
 * @param subspace_cell the cubical cell (generally open) which
 * defines the subspace of the cubical space.
 *
 * @param ks a digital space, which is in fact a 2^k-subdivision
 * of [subspace_cell].
 *
 * @param qc a list of digital cells
 *
 */
void 
ImaGene::CubicalSpace::getCubicalCells
( deque<CubicalCell> & qcc,
  const CubicalCell & subspace_cell,
  const KnSpace & ks, 
  const deque<Kn_uid> & qc ) const
{
  ASSERT_CubicalSpace( ks.dim() >= m_dim );

  CubicalCell::default_dim = m_dim;
  for ( deque<Kn_uid>::const_iterator it = qc.begin();
	it != qc.end();
	++it )
    {
      Kn_uid c = *it;
      CubicalCell ccell( subspace_cell, m_dim );
      for ( uint i = 0; i < m_dim; ++i )
	{
	  uint xt = ks.ukdecodeCoord( c, i );
	  if ( subspace_cell[ i ] & 0x1 )
	    ccell[ i ] = ( ( ccell[ i ] & 0xfffffffffffffffeLL )
			   << ks.logSize( i ) ) + xt;
	  else
	    ccell[ i ] = ( ccell[ i ] << ks.logSize( i ) );
	}
      qcc.push_back( ccell );
    }
}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::CubicalSpace::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[CubicalSpace]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::CubicalSpace::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
