///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : DigitalElevationMapStats.cxx
//
// Creation : 2010/05/21
//
// Version : 2010/05/21
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
//	2010/05/21 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/base/Proxy.h"
#include "ImaGene/timetools/Clock.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/digitalnD/KnRCellSet.h"
#include "ImaGene/digitalnD/KnShapes.h"
#include "ImaGene/helper/DigitalElevationMapStats.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/DigitalElevationMapStats.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const DigitalElevationMapStats_RCS_ID = "@(#)class DigitalElevationMapStats definition.";



///////////////////////////////////////////////////////////////////////////////
// class DigitalElevationMapStats
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Clear all stats.
 */
void 
ImaGene::DigitalElevationMapStats::clearStats()
{
  if ( m_stats != 0 )
    {
      delete m_stats;
      m_stats = 0;
    }
//   if ( m_sums != 0 )
//     {
//       delete m_sums;
//       m_sums = 0;
//     }
//   if ( m_nbs != 0 )
//     {
//       delete m_nbs;
//       m_nbs = 0;
//     }
}

/**
 * Compute all statistics (digital lengths of maximal segments).
 */
void
ImaGene::DigitalElevationMapStats::computeStats()
{
  clearStats();
  if ( ! OK() ) return;
  // Allocate vectors.
  m_stats = new KnRUCellVector< Statistic<float> >( *(this->space2D()), 1 );
//   m_sums = new KnRUCellVector<int>( *(this->space2D()), 1 );
//   m_nbs = new KnRUCellVector<int>( *(this->space2D()), 1 );
//   // Initializes vectors.
//   for ( KnRUCellVector<int>::iterator it = m_sums->begin();
// 	it != m_sums->end();
// 	++it )
//     *it = 0;
//   for ( KnRUCellVector<int>::iterator it = m_nbs->begin();
// 	it != m_nbs->end();
// 	++it )
//     *it = 0;

//   cerr << "[ImaGene::DigitalElevationMapStats::computeStats]"
//        << " computeStatsAlongXZ() in ";
//   Clock::startClock();
  computeStatsAlongIZ( 0 );
//   long t1 = Clock::stopClock();
//   cerr << t1 << " ms." << endl;

//   cerr << "[ImaGene::DigitalElevationMapStats::computeStats]"
//        << " computeStatsAlongYZ() in ";
//   Clock::startClock();
  computeStatsAlongIZ( 1 );
//   long t2 = Clock::stopClock();
//   cerr << t2 << " ms." << endl;

//   cerr << "[ImaGene::DigitalElevationMapStats::computeStats]"
//        << " computeStatsAlongXY() in ";
//   Clock::startClock();
  computeStatsAlongXY();
//   long t3 = Clock::stopClock();
//   cerr << t3 << " ms." << endl;
}


/**
 * Compute statistics (digital lengths of maximal segments) along iZ paths.
 */
void
ImaGene::DigitalElevationMapStats::computeStatsAlongIZ( uint i )
{
  // Starts along X.
  Pixel p = lowest();
  Pixel q = highest();
  uint j = i == 0 ? 1 : 0;
  C4CTangentialCover tcover;
  bool success;
  uint code;
  for ( Pixel z = p; z[ j ] < q[ j ]; ++z[ j ] )
    {
      C4CIteratorOnSurface* iter = this->createIterator( z, i );
      tcover.init( *iter, 0 );
      // Be careful: iter may not point on initial bel after
      // tcover.init.  It could be the "last". In fact, it is the
      // first when considering the direct orientation. This is why we
      // call sdirect.
      Mathutils::ModuloComputer mc( tcover.nbMaximalSegments() );
      C4CTangentialCover::SurfelMaximalSegments sms = tcover.beginSMS( 0 );
      Kn_sid bel = iter->current();
      bool orient = space()->sdirect( bel, i );
      Kn_uid linel2d = 
	uproject( space()->unsigns( space()->sincident( bel, i, orient ) ) );
//       cerr << i << ":" << z << " " << endl << "  bel=";
//       space()->displayKn_sid( bel, cerr );
//       cerr << endl << "  lin=";
//       space()->displayKn_uid( linel2d, cerr );
//       cerr << endl;
      while ( ( orient && ! space()->sisMax( bel, i ) )
	      || ( ! orient && ! space()->sisMin( bel, i ) ) )
	{
	  uint begin_ms = sms.begin_ms;
	  do {
	    success = tcover.nextSMS( sms );
	    code = iter->next();
	    ASSERT_DigitalElevationMapStats( success && ( code != 0 ) );
	  }
	  while ( space()->sorthDir( iter->current() ) != 2 );
	  uint end_ms = sms.end_ms;
// 	  uint nb = 0;
// 	  uint sum = 0;
	  Statistic<float> & s = (*m_stats)[ linel2d ];
	  while ( begin_ms != end_ms )
	    {
	      s.addValue
		( (float)((tcover.getMaximalSegment( begin_ms )).dss.size()) );
// 	      sum += (tcover.getMaximalSegment( begin_ms )).dss.size();
// 	      ++nb;
	      mc.increment( begin_ms );
	    }
	  // cerr << "  " << linel2d << " " << sum << " " << nb << endl;
// 	  (*m_sums)[ linel2d ] += sum;
// 	  (*m_nbs)[ linel2d ] += nb;
	  bel = iter->current();
	  linel2d = space2D()->uadjacent( linel2d, i, orient );
	}
      delete iter;
    } 
}

/**
 * Compute statistics (digital lengths of maximal segments) along XY paths.
 */
void
ImaGene::DigitalElevationMapStats::computeStatsAlongXY()
{
  // Extract surface.
  Pixel p = lowest();
  Pixel q = highest();
  Proxy<DigitalSurfaceTracker> tracker ( this->createTracker( p ) );
  KnRCellSet surface = KnShapes::strack( *(space()), 
					 *tracker );
  KnRCellSet marker( surface );
  C4CTangentialCover tcover;
  bool success;
  uint code;
  const uint j = 2;
  for ( KnRCellSet::cell_iterator p = surface.begin(); p != surface.end(); ++p )
    {
      Kn_sid bel = *p;
      // Test if bel has already been visited.
      if ( ! marker[ bel ] ) continue;
      if ( space()->sorthDir( bel ) == j ) break;
      KnSpace::dir_iterator q = space()->sbegin_dirs( bel );
      for ( ; ! q.end(); ++q )
	if ( *q != j ) break;
      uint i = *q;
      ASSERT_DigitalElevationMapStats( ( i == 0 ) || ( i == 1 ) );
      C4CIteratorOnSurface* iter = objectBoundary()->newC4CIterator( bel, i );
      tcover.init( *iter, 0 );
      // Be careful: iter may not point on initial bel after
      // tcover.init.  It could be the "last". In fact, it is the
      // first when considering the direct orientation. This is why we
      // call sdirect.
      Mathutils::ModuloComputer mc( tcover.nbMaximalSegments() );
      C4CTangentialCover::SurfelMaximalSegments sms = tcover.beginSMS( 0 );
      do 
	{
	  Kn_sid bel = iter->current();
	  marker[ bel ] = false;
	  bool orient = space()->sdirect( bel, j );
	  Kn_uid linel2d = 
	    uproject( space()->unsigns( space()->sincident( bel, i, orient ) ) );
	  uint begin_ms = sms.begin_ms;
	  const uint end_ms = sms.end_ms;
// 	  uint nb = 0;
// 	  uint sum = 0;
	  Statistic<float> & s = (*m_stats)[ linel2d ];
	  while ( begin_ms != end_ms )
	    {
	      s.addValue
		( (float)((tcover.getMaximalSegment( begin_ms )).dss.size()) );
// 	      sum += (tcover.getMaximalSegment( begin_ms )).dss.size();
// 	      ++nb;
	      mc.increment( begin_ms );
	    }
// 	  (*m_sums)[ linel2d ] += sum;
// 	  (*m_nbs)[ linel2d ] += nb;
	  
	  success = tcover.nextSMS( sms );
	  code = iter->next();
	}
      while ( success );
//       cerr << i << ":" << z << " " << endl << "  bel=";
//       space()->displayKn_sid( bel, cerr );
//       cerr << endl << "  lin=";
//       space()->displayKn_uid( linel2d, cerr );
//       cerr << endl;
      delete iter;
    } 
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::DigitalElevationMapStats::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[DigitalElevationMapStats]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::DigitalElevationMapStats::OK() const
{
  return DigitalElevationMap::OK();
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
