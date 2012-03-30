///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ImplicitSurfaceEmbedder.cxx
//
// Creation : 2005/01/18
//
// Version : 2005/01/18
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
//	2005/01/18 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/3d/ImplicitSurfaceEmbedder.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/3d/ImplicitSurfaceEmbedder.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ImplicitSurfaceEmbedder_RCS_ID = "@(#)class ImplicitSurfaceEmbedder definition.";



///////////////////////////////////////////////////////////////////////////////
// class ImplicitSurfaceEmbedder
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 * nothing to do.
 */
ImaGene::ImplicitSurfaceEmbedder::~ImplicitSurfaceEmbedder()
{
}


/**
 * Constructor. The embedder [spel_embedder] is used to embed
 * spels. It is referenced in the object. The object is not valid
 * yet and need an implicit surface.
 * @see init
 *
 * @param spel_embedder any embedder.
 *
* @param epsilon this positive value specifies the min distance
* between an embedding of a spel and the embedding of a surfel as a
* multiple of the distance between two adjacent voxels.
 */
ImaGene::ImplicitSurfaceEmbedder::ImplicitSurfaceEmbedder
( const Embedder & spel_embedder,
  float epsilon )
  : Embedder( spel_embedder.space() ),
    m_spel_embedder( spel_embedder ),
    m_epsilon( epsilon )
{
}


/**
 * Initializes the embedder for the implicit surface [is].
 *
 * @param is any implicit surface (with same dimension as the
 * embedder). It is referenced and not cloned.
 */
void
ImaGene::ImplicitSurfaceEmbedder::init( const ImplicitSurface* is )
{
  m_is = is;
}



///////////////////////////////////////////////////////////////////////////////
// ----------------------- Implemented abstract services ------------------

    
/**
 * @return a clone of 'this'.
 */
ImaGene::Embedder* 
ImaGene::ImplicitSurfaceEmbedder::clone() const
{
  ImplicitSurfaceEmbedder* ise = 
    new ImplicitSurfaceEmbedder( m_spel_embedder );
  ise->init( m_is );
  return ise;
}

    


///////////////////////////////////////////////////////////////////////////////
// ----------------------- Embedding services -----------------------------


/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any unsigned cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void
ImaGene::ImplicitSurfaceEmbedder::uembed( Kn_uid p, float* coords ) const
{
  if ( space()->uisSurfel( p ) )
    {
      uint i = space()->uorthDir( p );
      // spel pair around surfel.
      Kn_uid spel1 = space()->uincident( p, i, false );
      Kn_uid spel2 = space()->uincident( p, i, true );
      m_spel_embedder.uembed( spel1, coords );
      float x1 = coords[ i ];
      // float v1 = m_is->operator()( coords );
      m_spel_embedder.uembed( spel2, coords );
      float x2 = coords[ i ];
      // float v2 = m_is->operator()( coords );

      // ASSERT_ImplicitSurfaceEmbedder( v1 != v2 );

      uint n = space()->dim();
      float gx[ n ];
      coords[ i ] = 0.5 * ( x1 + x2 );
      float dx = 0.5 * ( x2 - x1 ) - m_epsilon;
      float v = m_is->operator()( coords );
      m_is->gradient( coords, gx );
//       cerr << "v1=" << v1 << " v=" << v << " v2=" << v2
// 	   << " gx=(" << gx[ 0 ] << "," << gx[ 1 ] << "," << gx[ 2 ] 
// 	   << ")" << endl;
      float ngx2 = 0.0;
      for ( uint i = 0; i < n; ++i )
	ngx2 += gx[ i ] * gx[ i ];
      for ( uint i = 0; i < n; ++i )
	{
	  float dl = - v * gx[ i ] / ngx2;
 	  if ( ( fabs( dl ) > dx ) )
 	    coords[ i ] += ( dl >= 0.0 ) ? dx : -dx;
 	  else
	    coords[ i ] += dl;
	}
      

//       float dx = m_epsilon * ( x2 - x1 );
//       coords[ i ] = ( v2 * x1 - v1 * x2 ) / ( v2 - v1 );
//       if ( coords[ i ] < ( x1 + dx ) )
// 	coords[ i ] = x1 + dx;
//       else if ( coords[ i ] > ( x2 - dx ) )
// 	coords[ i ] = x2 - dx;
    }
  else
    m_spel_embedder.uembed( p, coords );
}

    
/**
 * Embeds the cell [p] in the Euclidean space as a point.
 * @param p any signed cell.
 * @param coords (returns) the coordinates of [p] when embedded in the space.
 */
void
ImaGene::ImplicitSurfaceEmbedder::sembed( Kn_sid p, float* coords ) const
{
  this->uembed( space()->unsigns( p ), coords );
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
ImaGene::ImplicitSurfaceEmbedder::embedVector
( const float* p, float* coords ) const
{
  m_spel_embedder.embedVector( p, coords );
}



///////////////////////////////////////////////////////////////////////////////
// ----------------------- inverse embedding services ---------------------

/**
 * @param coords any coordinates in the space.
 * @return the unsigned spel whose embedding is the closest to [coords].
 */
ImaGene::Kn_uid
ImaGene::ImplicitSurfaceEmbedder::uinverseSpel
( const float* coords ) const
{
  return m_spel_embedder.uinverseSpel( coords );
}






///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ImplicitSurfaceEmbedder::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ImplicitSurfaceEmbedder]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ImplicitSurfaceEmbedder::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
