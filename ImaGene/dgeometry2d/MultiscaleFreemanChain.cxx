///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : MultiscaleFreemanChain.cxx
//
// Creation : 2009/07/09
//
// Version : 2009/07/09
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
//	2009/07/09 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/Statistics.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/dgeometry2d/MultiscaleFreemanChain.h"
#include "ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.h"
#include "ImaGene/helper/ShapeHelper.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/MultiscaleFreemanChain.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const MultiscaleFreemanChain_RCS_ID = "@(#)class MultiscaleFreemanChain definition.";



///////////////////////////////////////////////////////////////////////////////
// class MultiscaleFreemanChain
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::MultiscaleFreemanChain::~MultiscaleFreemanChain()
{
}


/**
 * Defines the chain of 'FreemanChainTransform' used to define the
 * subsampling.
 *
 * @param transform the global subsampling transformation.
 *
 * @param subsample the subsampling transform that must be part
 * of the chain [transform].
 */
void
ImaGene::MultiscaleFreemanChain::chooseSubsampler
    ( const FreemanChainTransform & transform,
      FreemanChainSubsample & subsample )
{
  this->m_transform = &transform;
  this->m_subsample = &subsample;
}

/**
 * Computes the multiresolutions of the given Freeman chain [src]
 * from (1,1) up to (h,v)=(r,r) with shifts.
 *
 * @param src the source Freeman chain.
 * @param r the maximal resolution.
 */
void
ImaGene::MultiscaleFreemanChain::init( const FreemanChain & src, uint r )
{
  cerr << "+--- computing multiresolution " << flush;
  m_src = &src;
  for ( uint h = 1; h <= r; ++h )
    {
      cerr << "." << h << flush;
      m_subsample->m_h = h;
      m_subsample->m_v = h;
      // Computes all possible shifts for more robust multiscale analysis.
      for(int x0 = 0; x0 < h; x0++ ) {
	for(int y0 = 0; y0 < h; y0++ ) {
	  m_subsample->m_x0 = x0;
	  m_subsample->m_y0 = y0;
	  SubsampledChainKey key( h, h, x0, y0 );
	  SubsampledChain sub_chain;
	  if ( ! m_transform->apply( sub_chain.subc,
				     sub_chain.c2subc,
				     sub_chain.subc2c,
				     src ) )
	    cerr << endl 
		 << "      +--- ERROR at scale " 
		 << h << ", (" << x0 << "," << y0 << ")" << endl;
	  // cerr << sub_chain.subc.chain << endl;
	  m_map[ key ] = sub_chain;
	}
      }
    }
  cerr << " ended." << endl;
}


/**
 * @param key the key describing resolution and shifts.
 *
 * @return a pointer to the corresponding structure storing the
 * subsampled chain, or 0 if no one was found.
 */
const ImaGene::MultiscaleFreemanChain::SubsampledChain* 
ImaGene::MultiscaleFreemanChain::get
( const MultiscaleFreemanChain::SubsampledChainKey & key ) const
{
  std::map< SubsampledChainKey, SubsampledChain >::const_iterator it;
  it = m_map.find( key );
  if ( it == m_map.end() ) return 0;
  else return &( it->second );

}


/**
 * @param key the key describing resolution and shifts.
 *
 * @return a pointer to the corresponding structure storing the
 * subsampled chain, or 0 if no one was found.
 */
ImaGene::MultiscaleFreemanChain::SubsampledChain* 
ImaGene::MultiscaleFreemanChain::get
( const MultiscaleFreemanChain::SubsampledChainKey & key )
{
  std::map< SubsampledChainKey, SubsampledChain >::iterator it;
  it = m_map.find( key );
  if ( it == m_map.end() ) return 0;
  else return &( it->second );

}


/**
 * @param fc (maybe updated) the freeman chain code of the (closed)
 * contour. The chain code may be translated.
 * 
 * @return a dyn. alloc. statistics object storing the length of
 * maximal segments for each surfel. Contains as many statistics
 * variables as the number of surfels of [fc].
 */
ImaGene::Statistics*
ImaGene::MultiscaleFreemanChain::getStatsMaximalSegments( FreemanChain & fc )
{
  int width =0;
  KnSpace* ks = ShapeHelper::makeSpaceFromFreemanChain( fc );
  
  // Computes maximal segments.
  C4CIteratorOnFreemanChain itfc;
  itfc.init( fc.begin(), fc.isClosed()!=0 );
  C4CIteratorOnFreemanChainSurface itfcs;
  itfcs.init( ks, itfc );  
  C4CTangentialCover tcover;
  bool isInit = tcover.init( itfcs,0 );
  uint surfaceSize =   tcover.nbSurfels();
  Statistics* stats = new Statistics( surfaceSize, false );  

  uint idx = 0;
  Mathutils::ModuloComputer msmc( tcover.nbMaximalSegments());
  Mathutils::ModuloComputer sc( surfaceSize );
  // cerr << "[fillStatsMS] nbsurf=" << surfaceSize
  //      <<  " nbms=" << tcover.nbMaximalSegments() << endl;
  if(tcover.nbMaximalSegments()<=1){
    if ( ks != 0 ) delete ks;
    return stats;
  }
  
  // First, we look for the first index.
  uint idx_source = 0;
  C4CTangentialCover::SurfelMaximalSegments sms = 
    tcover.beginSMS( idx_source );
  do
    {
      // Compute statistics for this surfel.
      for ( uint idx_ms = sms.begin_ms; idx_ms != sms.end_ms; 
	    msmc.increment( idx_ms ) )
	{
	  const C4CTangentialCover::MaximalSegment & ms =
	    tcover.getMaximalSegment( idx_ms );
	  stats->addValue( sms.idx_surfel, (double) ms.dss.size() - 1.0 );
	}
      if ( ! tcover.nextSMS( sms ) ) break;
    }
  while ( sms.idx_surfel != idx_source );
  stats->terminate();      
  if ( ks != 0 ) delete ks;
  return stats;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::MultiscaleFreemanChain::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[MultiscaleFreemanChain]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::MultiscaleFreemanChain::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
