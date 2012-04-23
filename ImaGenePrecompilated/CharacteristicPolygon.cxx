///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : CharacteristicPolygon.cxx
//
// Creation : 2008/06/20
//
// Version : 2008/06/20
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
//	2008/06/20 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <string>
#include "ImaGene/base/Proxy.h"
#include "ImaGene/base/WordUtils.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/helper/CharacteristicPolygon.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/CharacteristicPolygon.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const CharacteristicPolygon_RCS_ID = "@(#)class CharacteristicPolygon definition.";



///////////////////////////////////////////////////////////////////////////////
// class CharacteristicPolygon
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::CharacteristicPolygon::~CharacteristicPolygon()
{
  if ( m_it_ll != 0 ) delete m_it_ll;
}

/**
 * Initializes the characteristic polygon with the structure and
 * geometry of a digital contour.
 *
 * @param tcover the tangential cover of a digital contour.
 * @param tcover_geometry the geometry of [tcover].
 */
void
ImaGene::CharacteristicPolygon::init
( const C4CTangentialCover & tcover,
  const C4CTangentialCoverGeometry & tcover_geometry )
{
  uint nb_surfels = tcover.nbSurfels();
  uint nb_ms = tcover.nbMaximalSegments();

  // Find lower left corner.
  Vector2i zero( 0, 0 );
  Vector2i one_x( 1, 0 );
  uint i_ll = 0;
  Vector2i ll( tcover_geometry.sgeometry( 0 )
	       .frame.transformPoint( one_x ) );
  for ( uint i = 1; i < nb_surfels; ++i )
    {
      const C4CTangentialCoverGeometry::SurfelGeometry & sg
	= tcover_geometry.sgeometry( i );
      // cerr << "surfel geometry of " << i << " "
      // 	   << sg.frame;
      Vector2i pt( sg.frame.transformPoint( one_x ) );
      if ( ( pt.x() < ll.x() )
	   || ( ( pt.x() == ll.x() ) && ( pt.y() < ll.y() ) ) )
	{
	  ll = pt;
	  i_ll = i;
	}
    }
   // cerr << "# lowest Left  is idx=" << i_ll 
   //      << " x=" << ll.x() 
   //      << " y=" << ll.y() << endl;

  // Checks in which maximal segment is this surfel.
  // Lowest left surfel is not always a front surfel of a maximal segment.
  // We look for the maximal segment containing lowest left surfel.
  uint ims = 0;          // maximal segment s.t. its front = [start_surfel]
  uint start_surfel = 0; // the front surfel of [ims]
  while ( ims < nb_ms )
    {
      //       cerr << "MS " << ims 
//   	   << " back=" << tcover.getMaximalSegment( ims ).back_surfel_idx
//   	   << " front=" << tcover.getMaximalSegment( ims ).front_surfel_idx
//   	   << endl;
      uint front = tcover.getMaximalSegment( ims ).front_surfel_idx;
      uint back = tcover.getMaximalSegment( ims ).back_surfel_idx;
      // Two cases according to back < front or the other way around.
      if ( ( ( back < i_ll ) && ( i_ll <= front ) )
	   || ( ( front < back ) 
		&& ( ( back < i_ll ) || ( i_ll <= front ) ) ) )
	{
	  start_surfel = front;
	  break;
	}
      ++ims;
    }
//   cerr << "# Is in MS=" << ims << " / " << nb_ms << endl;
  if ( ims == nb_ms ) 
    {
      cerr << "# ERROR. No maximal segment contains lowest left surfel !"
	   << endl;
      return;
    }

  m_idx_ms_ll = ims;
  m_it_ll = tcover.getSurfelFront( ims )->clone();

  // Computes Freeman chain code and quadrants.  The chain code of two
  // consecutive and perpendical surfels must be computed to (1) build
  // a Freeman chain code, (2) determine the starting quadrant of the
  // code.
  Mathutils::ModuloComputer msu( nb_surfels );
  Proxy<C4CIterator> it( tcover.getSurfelFront( ims ) );  
  it->next();
  uint idx_surfel = msu.next( start_surfel );
  const C4CTangentialCoverGeometry::SurfelGeometry & sg1
    = tcover_geometry.sgeometry( start_surfel );
  const C4CTangentialCoverGeometry::SurfelGeometry & sg2
    = tcover_geometry.sgeometry( idx_surfel );
  Vector2i pt0( sg1.frame.transformPoint( zero ) );
  Vector2i pt1( sg2.frame.transformPoint( zero ) );
  Vector2i pt2( sg2.frame.transformPoint( one_x ) );
  pt2 -= pt1;
  pt1 -= pt0;
  uint idx_surfel_code =
    pt2.y() == 0 ? (uint) ( 1 - pt2.x() ) : (uint) ( 2 - pt2.y() );
  uint start_surfel_code =
    pt1.y() == 0 ? (uint) ( 1 - pt1.x() ) : (uint) ( 2 - pt1.y() );

//   cerr << "# Start surfel is " << start_surfel << " in MS " << ims
//        << " / " << nb_ms << " code=" << start_surfel_code << endl;
//   cerr << "# Idx surfel is " << idx_surfel << " in MS " << ims + 1 
//        << " / " << nb_ms << " code=" << idx_surfel_code << endl;

  Mathutils::ModuloComputer mc4( 4 );
  string chain;
  string qchain;
  FreemanChain::create( chain, qchain, *it, nb_surfels, 
			idx_surfel_code, 
			mc4.next( start_surfel_code ) == idx_surfel_code 
			? start_surfel_code : idx_surfel_code, 
			idx_surfel );
//   cerr << "--- Controlling FreemanChain::create" << endl;
//   cerr << chain << endl;
//   cerr << qchain << endl;

  // Computes convexity zones, concavity zones, inflexion edges.
  Mathutils::ModuloComputer mms( nb_ms );
  Mathutils::AngleComputer ac;

  string shape( nb_surfels, 'e' );

//   uint idx_surfel = msu.next( i_ll );
  uint cur_ms = mms.next( ims );
  for ( uint i = 0; i <= nb_ms; ++i )
    {
      const C4CTangentialCoverGeometry::MaximalSegmentGeometry & msg_prev 
	= tcover_geometry.geometry( mms.previous( cur_ms ) );
      
      const C4CTangentialCover::MaximalSegment & ms
	= tcover.getMaximalSegment( cur_ms );
      const C4CSegment & dss = ms.dss;
      const C4CTangentialCoverGeometry::MaximalSegmentGeometry & msg
	= tcover_geometry.geometry( cur_ms );
      const Frame2D & frame
	= tcover_geometry.sgeometry( ms.front_surfel_idx ).frame;
      
      const C4CTangentialCoverGeometry::MaximalSegmentGeometry & msg_next
	= tcover_geometry.geometry( mms.next( cur_ms ) );

      bool convex_back = ac.less( msg_prev.angle_to_x, msg.angle_to_x );
      bool convex_front = ac.less( msg.angle_to_x, msg_next.angle_to_x );
      int idx_C = dss.relIndex( dss.c_n() );
      uint idx_U = ( ms.back_surfel_idx 
		     + (uint) ( dss.relIndex( dss.u() ) - idx_C ) ) 
	% nb_surfels;
      uint idx_L = ( ms.back_surfel_idx 
		     + (uint) ( dss.relIndex( dss.l() ) - idx_C ) ) 
	% nb_surfels;
      uint idx_Up = ( ms.back_surfel_idx 
		      + (uint) ( dss.relIndex( dss.up() ) - idx_C ) )
	% nb_surfels;
      uint idx_Lp = ( ms.back_surfel_idx 
		      + (uint) ( dss.relIndex( dss.lp() ) - idx_C ) )
	% nb_surfels;
      // Before U, shape is convex or concave.
      if ( convex_back )
	while ( idx_surfel != idx_U ) 
	  {
	    shape[ idx_surfel ] = '0';
	    msu.increment( idx_surfel );
	  }
      else
	while ( idx_surfel != idx_L ) 
	  {
	    shape[ idx_surfel ] = '1';
	    msu.increment( idx_surfel );
	  }
      if ( convex_front )
	while ( idx_surfel != idx_Up ) 
	  {
	    shape[ idx_surfel ] = convex_back ? '0' : '2';
	    msu.increment( idx_surfel );
	  }
      else 
	while ( idx_surfel != idx_Lp ) 
	  {
	    shape[ idx_surfel ] = convex_back ? '3' : '1';
	    msu.increment( idx_surfel );
	  }
      // Go to next maximal segment.
      mms.increment( cur_ms );
    }
//   cerr << shape << endl;
  m_chain = chain;
  m_qchain = qchain;
  m_shape = shape;

  extractEdges();
  computeEdgesInformation( tcover, tcover_geometry );
}

/**
 * After Initialization, extract all the edges from the computed
 * strings (Freeman chaincode [m_chain], quadrant info [m_qchain],
 * convexity info [m_shape].
 */
void
ImaGene::CharacteristicPolygon::extractEdges()
{
  uint nb = m_chain.size();
  Mathutils::ModuloComputer msu( nb );
  ASSERT_CharacteristicPolygon( nb != 0 );
  m_edges.clear();

  // Find a starting point.
  uint s = 0;
  char geom = m_qchain[ s++ ];
  while ( ( s != nb ) && ( m_qchain[ s ] == geom ) ) ++s;
  uint start = ( s < nb )
    ? s  // several quadrants.
    : 0; // one quadrant ! start at beginning.

  // Slice shape according to quadrant changes and convexity/concavity
  // changes.
  // cerr << "+- Cutting points: ";
  vector<uint> cut_points;
  uint i = start; 
  do
    {
      cut_points.push_back( i );
//       cerr << " " << i;
      uint j = msu.next( i );
      while ( ( m_qchain[ j ] == m_qchain[ i ] ) 
	      && ( m_shape[ j ] == m_shape[ i ] ) )
	msu.increment( j );
      i = j;
    }
  while ( i != s );
  // cerr << endl;

  for ( uint j = 0; j < cut_points.size(); ++j )
    {
//       cerr << "Zone " << j << ": " << cut_points[ j ]
// 	   << " -> " << cut_points[ (j+1) % cut_points.size() ]
// 	   << " : ";
      extractEdgesInZone( m_edges, 
			  cut_points[ j ],
			  cut_points[ (j+1) % cut_points.size() ] );
    }
}

/**
 * After initialization, extract an edge in a consistent zone (as
 * given by cutting points).
 *
 * @param i the start point (the corresponding surfel is included)
 * @param j the end point  (the corresponding surfel is excluded)
 */
void
ImaGene::CharacteristicPolygon::extractEdgesInZone
( vector<Edge> & edges, uint i, uint j )
{
  // Define the alphabet for the zone.
  char zero;
  char one;
  FreemanChain::alphabet( zero, one, m_qchain[ i ] );

  bool clockwise = false;
  // Inverse zero and one if counterclockwise.
  if ( ! clockwise ) std::swap( zero, one );
  WordUtils wu( zero, one );
  WordUtils wu2( one, zero );
  Mathutils::ModuloComputer mc( m_chain.size() );

  uint len;
  uint nb;

  Edge edge;
  // Examine geometry of zone.
  switch ( m_shape[ i ] )
    {
    case '0': // convex
//       cerr << "[ CVX ";
      while ( i != j )
	{
	  wu.firstLyndonFactorMod( len, nb, m_chain, i, j );
	  uint ni = ( i + nb*len ) % mc.k;
	  uint ci = ( i + len ) % mc.k;
// 	  cerr << "(" << i << "," << ni << ";x" << nb;
// 	  cerr << ";C" << wu.isChristoffelMod( m_chain, i, ci )
// 	       << ")";

	  edge.idx1 = i;
	  edge.idx2 = ni;
	  edge.convex_back = true;
	  edge.convex_front = true;
	  edges.push_back( edge );

	  i = ni;
	}
//       cerr << " ]" << endl;
      break;
    case '1': // concave
//       cerr << "[ CCV ";
      while ( i != j )
	{
	  wu2.firstLyndonFactorMod( len, nb, m_chain, i, j );
	  uint ni = ( i + nb*len ) % mc.k;
	  uint ci = ( i + len ) % mc.k;
// 	  cerr << "(" << i << "," << ni << ";x" << nb;
// 	  cerr << ";C" << wu2.isChristoffelMod( m_chain, i, ci )
// 	       << ")";

	  edge.idx1 = i;
	  edge.idx2 = ni;
	  edge.convex_back = false;
	  edge.convex_front = false;
	  edges.push_back( edge );

	  i = ni;
	}
//       cerr << " ]" << endl;
      break;

    case '2': // concave -> convex
//       cerr << "[ CCV->CVX ";
//       cerr << "(" << i << "," << j << ")";
      edge.idx1 = i;
      edge.idx2 = j;
      edge.convex_back = false;
      edge.convex_front = true;
      edges.push_back( edge );
//       cerr << " ]" << endl;
      break;

    case '3': // convex -> concave
//       cerr << "[ CVX->CCV ";
//       cerr << "(" << i << "," << j << ")";
      edge.idx1 = i;
      edge.idx2 = j;
      edge.convex_back = true;
      edge.convex_front = false;
      edges.push_back( edge );
//       cerr << " ]" << endl;
      break;

    default:
      cerr << "TODO" << endl;
    }
  
}

/** 
 * After extracting edges, compute all the associated information.
 */
void
ImaGene::CharacteristicPolygon::computeEdgesInformation
( const C4CTangentialCover & tcover,
  const C4CTangentialCoverGeometry & tcover_geometry )
{
  Proxy<C4CIterator> it( tcover.beginSurfel() );
  Mathutils::ModuloComputer msu( tcover.nbSurfels() );
  uint s = 0;
  for ( uint i = 0; i < m_edges.size(); ++i )
    {
      // Position iterator.
      while ( s != m_edges[ i ].idx1 )
	{
	  msu.increment( s );
	  it->next();
	}
      // Take care of frame.
      m_edges[ i ].frame_back = tcover_geometry.sgeometry( s ).frame;
      // Build DSS.
      m_edges[ i ].dss.init();
      while ( s != m_edges[ i ].idx2 )
	{
	  msu.increment( s );
	  uint code = it->next();
	  if ( s != m_edges[ i ].idx2 )
	    m_edges[ i ].dss.extendsFront( code );
	}
//       cerr << i << " " << m_edges[ i ] << endl;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::CharacteristicPolygon::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[CharacteristicPolygon]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::CharacteristicPolygon::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
