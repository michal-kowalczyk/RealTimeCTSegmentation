///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ImplicitDigitalSurface.cxx
//
// Creation : 2005/01/17
//
// Version : 2005/01/17
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
//	2005/01/17 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/3d/ImplicitDigitalSurface.h"
#include "ImaGene/3d/ImplicitDigitalSurfaceTracker.h"
#include "ImaGene/3d/C4CIteratorOnImplicitDigitalSurface.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/3d/ImplicitDigitalSurface.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ImplicitDigitalSurface_RCS_ID = "@(#)class ImplicitDigitalSurface definition.";



///////////////////////////////////////////////////////////////////////////////
// class ImplicitDigitalSurface
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// ----------------------- Static services ------------------------------

/**
 * Find a bel of the implicit digital surface by random tries then
 * dichotomy.
 *
 * @param ids any implicit digital surface.
 * @param nbries the maximum number of random tries.
 * @return a boundary surfel of [ids] or '0' if none was found after [nbtries] iterations. 
 *
 * @todo check bounds of implicit surface.
 */
ImaGene::Kn_sid
ImaGene::ImplicitDigitalSurface::findABel
( const ImplicitDigitalSurface & ids,
  uint nbtries )
{
  const KnSpace* ks = ids.space();
  if ( ks == 0 ) return 0;
  uint n = ks->dim();
  Kn_size x1[ n ];
  Kn_size x2[ n ];

  // (1) Find two candidates in the space.
  Kn_uid v1 = ks->uspel( ks->ufirst() );
  ks->udecodeCoords( v1, x1 );
  Kn_uid v2 = v1;
  bool val_v1 = ids[ v1 ];
  bool found = false;
  for ( uint j = 0; j < nbtries; ++j )
    {
      for ( uint i = 0; i < n; ++i )
	x2[ i ] = Mathutils::random( ks->size( i ) );
      v2 = ks->ucode( x2, v1 );
      if ( ids[ v2 ] != val_v1 )
	{
	  cerr << "[ImaGene::ImplicitDigitalSurface::findABel] "
	       << "found in " << j << " tries." << endl;
	  found = true;
	  break;
	}
    }
  if ( ! found ) return 0;
  // (2) Find two candidates on the same axis.
  uint d = 0;
  for ( uint i = 0; i < n; ++i )
    {
      if ( x1[ i ] != x2[ i ] )
	{
	  for ( uint j = i + 1; j < n; ++j )
	    {
	      if ( x1[ j ] != x2[ j ] )
		{
		  uint c = x2[ j ];
		  x2[ j ] = x1[ j ];
		  Kn_sid v3 = ks->ucode( x2, v1 );
		  if ( val_v1 != ids[ v3 ] )
		    { // v2 is updated.
		      v2 = v3;
		      d = i;
		    }
		  else
		    { // v1 is updated.
		      for ( uint k = 0; k < n; ++k )
			x1[ k ] = x2[ k ];
		      v1 = v3;
		      x2[ j ] = c;
		      d = j;
		    }
		} // if ( x1[ j ] != x2[ j ] )
	    } // for ( uint j = i + 1; j < n; ++ )
	} // if ( x1[ i ] != x2[ i ] )
    } // for ( uint i = 0; i < n; ++i )

  // (3) Check result.
  for ( uint i = 0; i < n; ++i )
    {
      if ( ( i == d ) && ( x1[ i ] == x2[ i ] ) )
	cerr << "Erreur sur " << i << endl;
      if ( ( i != d ) && ( x1[ i ] != x2[ i ] ) )
	cerr << "Erreur sur " << i << endl;
    }
  
  // (4) Dichotomy
  Kn_size xmid[ n ];
  for ( uint i = 0; i < n; ++i )
    xmid[ i ] = x1[ i ];
  while ( true )
    {
      xmid[ d ] = ( x1[ d ] + x2[ d ] ) / 2;
      Kn_sid v3 = ks->ucode( xmid, v1 );
      if ( ( v3 == v1 ) || ( v2 == v1 ) )
	break;
      if ( ids[ v3 ] != val_v1 )
	{
	  x2[ d ] = xmid[ d ];
	  v2 = v3;
	}
      else
	{
	  x1[ d ] = xmid[ d ];
	  v1 = v3;
	}
    }
  
  // (5) Check result.
  for ( uint i = 0; i < n; ++i )
    {
      cout << "i=" << i << " x1=" << x1[ i ] << " x2=" << x2[ i ]
	   << endl;
      if ( ( i == d ) && ( x1[ i ] != x2[ i ] - 1 ) )
	cerr << "Erreur 2a sur " << i << endl;
      if ( ( i != d ) && ( x1[ i ] != x2[ i ] ) )
	cerr << "Erreur 2b sur " << i << endl;
    }

  // (6) Computes bel.
  Kn_sid r;
  if ( val_v1 )
    r = ks->sincident( ks->signsPos( v1 ), d, true );
  else
    r = ks->sincident( ks->signsNeg( v1 ), d, true );
  return r;
}




///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ImplicitDigitalSurface::~ImplicitDigitalSurface()
{
  if ( m_is != 0 ) delete m_is;
  if ( m_embedder != 0 ) delete m_embedder;
}


/**
 * Constructor. The space is referenced and must not be destroyed. The
 * implicit surface and the embedder are cloned.  The bel adjacency is
 * referenced.
 *
 * @param ks the digital space.
 * @param is the implicit surface (cloned).
 * @param embedder the embedder defining how the spels of the digital space are embedded in R3 (cloned).
 */
ImaGene::ImplicitDigitalSurface::ImplicitDigitalSurface
( const KnSpace & ks, 
  const BelAdjacency & badj,
  const ImplicitSurface & is,
  const Embedder & spel_embedder )
  : m_space( &ks ),
    m_bel_adj( badj ),
    m_is( is.clone() ),
    m_embedder( spel_embedder.clone() )
{
}



///////////////////////////////////////////////////////////////////////////////
// ------------------------- set services -------------------------------

    
/**
 * Useful for 'SurfelNeighborhood.adjacentOnAnyBdry'.
 *
 * @param spel any spel of the digital space.
 * @return 'true' if the implicit surface has a positive or zero value at the spel embedding location, 'false' otherwise.
 */
bool
ImaGene::ImplicitDigitalSurface::operator[]( Kn_uid spel ) const
{
  float x[ m_space->dim() ];
  m_embedder->uembed( spel, x );
  return m_is->operator()( x ) >= 0.0f;
}




///////////////////////////////////////////////////////////////////////////////
// ------------------------- Factory services ----------------------------


/**
 * Returns a surface tracker pointing on [surfel].
 *
 * @param surfel any surfel.
 *
 * @return a dyn. alloc. surface tracker (must be desallocated by
 * the caller).
 */
ImaGene::DigitalSurfaceTracker* 
ImaGene::ImplicitDigitalSurface::newTracker( Kn_sid surfel ) const
{
  ImplicitDigitalSurfaceTracker* idst = new ImplicitDigitalSurfaceTracker();
  idst->init( this );
  idst->move( surfel );
  return idst;
}



/**
 * Returns an iterator pointing on [surfel] and moving along
 * direction [dir] in the direct tracking direction. Each movement
 * is given as a direction on a 4-connected contour.
 *
 * @param surfel any surfel.
 * @param dir a tracking direction (different from the orthogonal direction to [surfel]).
 *
 * @return a dyn. alloc. contour iterator (must be desallocated by
 * the caller).
 */
ImaGene::C4CIteratorOnSurface* 
ImaGene::ImplicitDigitalSurface::newC4CIterator( Kn_sid surfel, uint dir ) const
{
  C4CIteratorOnImplicitDigitalSurface* it_ids =
    new C4CIteratorOnImplicitDigitalSurface();
  it_ids->init( this );
  it_ids->start( surfel, dir );
  return it_ids;
}





///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ImplicitDigitalSurface::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ImplicitDigitalSurface]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ImplicitDigitalSurface::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
