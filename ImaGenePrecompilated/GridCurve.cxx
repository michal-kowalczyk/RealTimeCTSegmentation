///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : GridCurve.cxx
//
// Creation : 2011/02/28
//
// Version : 2011/02/28
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
//	2011/02/28 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/GridCurve.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/GridCurve.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const GridCurve_RCS_ID = "@(#)class GridCurve definition.";



///////////////////////////////////////////////////////////////////////////////
// class GridCurve
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::GridCurve::~GridCurve()
{
}

/**
 * Constructor.
 */
ImaGene::GridCurve::GridCurve()
  : myCurve(), myTwiceDV( 0, 0 ), 
    myCW( true ), myLength( 0.0 ), myUpdateNb( 0 )
{
}

/** Initializes the grid curve from an arbitrary RPR. */
void
ImaGene::GridCurve::initFromReversiblePolygonalRepresentation
( const ReversiblePolygonalRepresentation & rpr )
{
  int dx, dy;
  unsigned int a, ap;
  unsigned int b, bp;
  unsigned int quad, quadp;
  bool ccw;
  myCurve.clear();
  myUpdateNb = 0;
  myLength = 0.0;
  myCW = rpr.cw;
  myTwiceDV = rpr.twice_dv;
  #ifdef TRACE_DMLP
    cerr << "Twice_dv=" << myTwiceDV << endl;
  #endif
  unsigned int n = rpr.vx.size();
  for ( unsigned int i = 0; i < n; ++i )
    #ifdef TRACE_DMLP
      cerr << ( rpr.vx[ i ] + 0.5 * rpr.twice_dv.x() ) 
        << " " << ( rpr.vy[ i ] + 0.5 * rpr.twice_dv.y() )
        << " " << rpr.vt[ i ] << endl;
    #endif
  // Checks if the RPR is closed
  myIsClosed = ( rpr.vx[ 0 ] == rpr.vx[ n - 1 ] )
    && ( rpr.vy[ 0 ] == rpr.vy[ n - 1 ] )
    && ( rpr.vt[ 0 ] == rpr.vt[ n - 1 ] );
  #ifdef TRACE_DMLP
    cerr << ( myIsClosed ? "Closed" : "Open" ) << endl;
  #endif
  // Take care of first vector.
  Edge edge;
  Edge quadrant;
  unsigned int i = 0;
  unsigned int j = 0;
  Vector2i v;
  Vector2i pointel( rpr.vx[ i ], rpr.vy[ i ] );
  // pointel.x() += ( rpr.twice_dv.x() == -1 ) ? 1 : 0;
  // pointel.y() += ( rpr.twice_dv.y() == -1 ) ? 1 : 0;
  do
    {
      ++j;
      v = Vector2i( rpr.vx[ j % n ] - rpr.vx[ i % n ], 
		    rpr.vy[ j % n ] - rpr.vy[ i % n ] );
    }
  while ( ( v.x() == 0 ) && ( v.y() == 0 ) );
  bool tilde = rpr.vt[ j % n ] != rpr.vt[ i % n ];
  
  // Vector2i v( rpr.vx[ ( i+1 ) % n ] - rpr.vx[ i ], 
  // 	       rpr.vy[ ( i+1 ) % n ] - rpr.vy[ i ] );
  // while ( ( v.x() == 0 ) && ( v.y() == 0 ) && ( i < n ) )
  //   {
  //     ++i;
  //     v = Vector2i( rpr.vx[ ( i+1 ) % n ] - rpr.vx[ i ], 
  // 		    rpr.vy[ ( i+1 ) % n ] - rpr.vy[ i ] );
  //   }
  Vector2i former_v( v );
  ASSERT_GridCurve( j < n );
  FreemanChain::basis( a, b, v, rpr.vt[ i ], rpr.cw );
  #ifdef TRACE_DMLP
    cerr << "init letters: a=" << (char) ('0'+a) << " b=" << (char) ('0'+b)
       << flush;
  #endif
  quad = FreemanChain::quadrant( ccw, a, b );
  #ifdef TRACE_DMLP
    cerr << " q=" << quad << endl;
  #endif
  // Compute starting pointel.
  Vector2i p2p = myTwiceDV + FreemanChain::displacement( b )
    - FreemanChain::displacement( a );   // p2p = ( twice_dv -a+b ) /2
  p2p /= 2;
  pointel += p2p;

  edge.initContext( pointel, a, b, rpr.vt[ i ] );
  unsigned int p;
  unsigned int q;
  switch (a) 
    {
    case 0: case 2: q = v.x() < 0 ? -v.x() : v.x(); break;
    case 1: case 3: q = v.y() < 0 ? -v.y() : v.y(); break;
    }
  switch (b) 
    {
    case 0: case 2: p = v.x() < 0 ? -v.x() : v.x(); break;
    case 1: case 3: p = v.y() < 0 ? -v.y() : v.y(); break;
    }
  edge.initEdge( tilde ? q : p,
		 tilde ? p : q,
		 tilde );
  EdgeListIterator it_edge = myCurve.end();
  insertEdge( it_edge, edge );
  edge.getPostAlphabet( a, b );
  pointel = edge.postPointel();

  // Loop over the vertices. Completes the gridcurve if it is a loop
  // (gridcontour).
  for ( i = j; 
	( myIsClosed && ( i <= n ) ) || ( ! myIsClosed && ( i < ( n - 1 ) ) );
	i = j )
    {
      if ( ( v.x() != 0 ) || ( v.y() != 0 ) )
	former_v = v;
      do
	{
	  ++j;
	  v = Vector2i( rpr.vx[ j % n ] - rpr.vx[ i % n ], 
			rpr.vy[ j % n ] - rpr.vy[ i % n ] );
	}
      while ( ( v.x() == 0 ) && ( v.y() == 0 ) );
      bool tilde = rpr.vt[ j % n ] != rpr.vt[ i % n ];
      quadp = quad;
      // Computes the basis and the quadrant for the new vector.
      FreemanChain::basis( ap, bp, v, rpr.vt[ i % n], rpr.cw );
      quad = FreemanChain::quadrant( ccw, ap, bp ); 
      #ifdef TRACE_DMLP
        cerr << "loop letters: a=" << (char) ('0'+ap) 
         << " b=" << (char) ('0'+bp)
         << " q=" << quad
         << endl;
      #endif
      bool cnd = ( rpr.cw && rpr.vt[ i % n ] ) 
	|| ( !rpr.cw && !rpr.vt[ i % n ] );
      int nb = cnd ? ( quad + 4 - quadp ) % 4
	: ( quadp + 4 - quad ) % 4;
      #ifdef TRACE_DMLP
        cerr << "** " << former_v << "->" << v 
       << " fq=" << quadp
       << " q=" << quad << " cnd=" << cnd << endl;
      #endif
      switch ( nb )
	{
	case 0: break;
	case 3: // 'plus'
	case 1: // 'moins'
	  #ifdef TRACE_DMLP
      cerr << i << ',' << j << " [" << ( ( nb == 3 ) ? "+" : "-" ) << "]" << endl;
	  #      endif
	  quadrant.initContext( pointel, a, b, rpr.vt[ i % n ] );
	  quadrant.initQuadrantChange( ( nb == 3 ) ? 1 : -1 );
	  it_edge = myCurve.end();
	  insertEdge( it_edge, quadrant );
	  quadrant.getPostAlphabet( a, b );
	  pointel = quadrant.postPointel();
	  break;
	case 2: // 'moins', 'moins' or 'plus', 'plus'
	  int cross = former_v.det( v );
	  bool plusplus = cnd ? ( cross < 0 ) : ( cross > 0 );
	  if ( cross == 0 ) plusplus = ! tilde;
	  #ifdef TRACE_DMLP
      cerr << i << ',' << j << " [" << ( plusplus ? "++" : "--" ) << "]" << endl;
	  #endif
	  quadrant.initContext( pointel, a, b, rpr.vt[ i % n ] );
	  quadrant.initQuadrantChange( plusplus ? 1 : -1 );
	  it_edge = myCurve.end();
	  insertEdge( it_edge, quadrant );
	  quadrant.getPostAlphabet( a, b );
	  pointel = quadrant.postPointel();
	  quadrant.initContext( pointel, a, b, rpr.vt[ i % n ] );
	  quadrant.initQuadrantChange( plusplus ? 1 : -1 );
	  it_edge = myCurve.end();
	  insertEdge( it_edge, quadrant );
	  quadrant.getPostAlphabet( a, b );
	  pointel = quadrant.postPointel();
	  break;
	}
      if ( j < n )
	{
	  edge.initContext( pointel, a, b, rpr.vt[ i ] );
	  switch (a) 
	    {
	    case 0: case 2: q = v.x() < 0 ? -v.x() : v.x(); break;
	    case 1: case 3: q = v.y() < 0 ? -v.y() : v.y(); break;
	    }
	  switch (b) 
	    {
	    case 0: case 2: p = v.x() < 0 ? -v.x() : v.x(); break;
	    case 1: case 3: p = v.y() < 0 ? -v.y() : v.y(); break;
	    }
	  edge.initEdge( tilde ? q : p,
			 tilde ? p : q,
			 tilde );
	  #ifdef TRACE_DMLP
      cerr << i << ',' << j << " [" << edge.pq.q() << "," << edge.pq.p() << "]" << endl;
	  #endif
	  it_edge = myCurve.end();
	  insertEdge( it_edge, edge );
	  edge.getPostAlphabet( a, b );
	  pointel = edge.postPointel();
	}
    } //   for ( ++i; i < n; ++i )

}

// std::string
// ImaGene::GridCurve::Edge::toFreeman() const
// {
//   ASSERT_GridCurve( false && "TODO" );
//   return "";
// }

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::GridCurve::selfDisplay( ostream& that_stream ) const
{
  that_stream << "Polygon( [ ";
  EdgeListConstIterator it = myCurve.begin();
  EdgeListConstIterator it_end = myCurve.end();
  if ( it != it_end )
    {
      that_stream << *it;
      ++it;
      while ( it != it_end )
	{
	  that_stream << ", " << *it;
	  ++it;
	}
    }
  that_stream << " ] )";
}
/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */

void 
ImaGene::GridCurve::verboseSelfDisplay( ostream& that_stream ) const
{
  that_stream << "Polygon( [ ";
  EdgeListConstIterator it = myCurve.begin();
  EdgeListConstIterator it_end = myCurve.end();
  if ( it != it_end )
    {
      that_stream << *it;
      that_stream << "(pt=" << it->pointel 
		  << ",a=" << it->a
		  << ",b=" << it->b
		  << ",ins=" << ( it->inside ? 'V' : 'F' ) << ")"
		  << std::endl;
      ++it;
      while ( it != it_end )
	{
	  that_stream << *it;
	  that_stream << "(pt=" << it->pointel 
		      << ",a=" << it->a
		      << ",b=" << it->b
		      << ",ins=" << ( it->inside ? 'V' : 'F' ) << ")"
		      << std::endl;
	  ++it;
	}
    }
  that_stream << " ] )";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::GridCurve::OK() const
{
  return true;
}

bool 
ImaGene::GridCurve::preSimplifyAt
( EdgeListIterator & it,
  EdgeListIterator & it_before_first, 
  EdgeListIterator & it_after_last,
  EdgeListIterator & min,
  EdgeListIterator & max )
{
  if ( it == endEdge() ) return false;
  // ASSERT_GridCurve( it != endEdge() );
  EdgeListIterator it_next = nextEdge( it );
  it_before_first = it;
  it_after_last = it_next;
  if ( it_next == endEdge() ) return false;
  int d;
  bool mergeable = it->isMergeable( *it_next, d );
  // First case is when two edges are direclty mergeable.
  if ( mergeable ) 
    {
      #ifdef TRACE_DMLP
        cerr << "Simple merge: " << *it << "," << *it_next;
      #endif
      Edge new_edge;
      it->getMerge( new_edge, *it_next );
      #ifdef TRACE_DMLP
        cerr << " => " << new_edge << endl;
      #endif
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      moveToNextEdgeAndPossiblyMax( it_after_last, max );
      removeEdge( it_next );
      modifyEdge( it, new_edge );
      return true;
    }
  // Second case where this edge is a quadrant change, either does
  // nothing (positive quadrant change) or call specific method.
  if ( it->isQuadrantChange() )
    return ( it->nb < 0 )
      ? preSimplifyNegativeQuadrantChangeAt( it, 
					     it_before_first, it_after_last,
					     min, max )
      : preSimplifyPositiveQuadrantChangeAt( it, 
					     it_before_first, it_after_last,
					     min, max );
  // Third case where successor is a quadrant change, not processed
  // here, but at next edge.
  if ( it_next->isQuadrantChange() ) 
    return false;
  // Last case where we have two successive non mergeable standard edges.
  if ( d > 0 ) // concave vertex 
    { // NB: the case d = 1 could be rewritten more efficiently.
      EdgeListIterator split_after_last;
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      #ifdef TRACE_DMLP
        cerr << "Concave merge: " << *it << "," << *it_next << endl;
      #endif
      splitRight( it, split_after_last );
      ASSERT_GridCurve( split_after_last == it_next );
      moveToNextEdgeAndPossiblyMax( it_after_last, max );
      splitLeft( it_next, split_after_last );
      ASSERT_GridCurve( split_after_last == it_after_last );
      ASSERT_GridCurve( it_next->isTrivial() && ( it_next->nb == 1 ) );
      ASSERT_GridCurve( nextEdge( it_next )->isTrivial() 
			&& ( nextEdge( it_next )->nb == 1 ) );
      // cerr << "[BEFORE]" << endl << "gridcurve = " << *this << endl;
      // cerr << "[MERGE " << *it_next << "," << *nextEdge( it_next ) << "]" << endl;
      bool simple_merge = merge( it_next );
      // cerr << "[AFTER]" << endl << "gridcurve = " << *this << endl;
      ASSERT_GridCurve( simple_merge );
      return true;
    }
  // d < 0 : convex vertex, nothing to do.
  return false;
}

bool
ImaGene::GridCurve::preSimplifyNegativeQuadrantChangeAt
( EdgeListIterator & it,
  EdgeListIterator & it_before_first, 
  EdgeListIterator & it_after_last,
  EdgeListIterator & min,
  EdgeListIterator & max )
{
  // Processed cases. (? is tilde or not).
  // case [-][+] => nothing
  // case [-][0^k] => [oo^k][-]
  // case [~0][-][~0] => [+]
  // case [?p/q^k][-][?p'/q'^l] => SR[?p/q^k][-]SL[?p'/q'^l]
  EdgeListIterator it_next = it_after_last; 
  if ( it_next->isQuadrantChange() )
    {
      if ( it_next->nb > 0 )
	{ // case [-][+] => nothing
	  moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
	  moveToNextEdgeAndPossiblyMax( it_after_last, max );
	  it = removeEdge( it );
	  it = removeEdge( it );
	  #ifdef TRACE_DMLP
      cerr << "[-][+] => nothing" << endl;
	  #endif
	  return true;
	}
      // case [-][-] => TODO
      return preSimplifyTwoNegativeQuadrantChangesAt( it, 
						      it_before_first, 
						      it_after_last, min, max );
      #ifdef TRACE_DMLP
        cerr << "debugcurve = " << *this << endl;
      #endif
      ASSERT_GridCurve( false && "[GridCurve::preSimplifyNegativeQuadrantChangeAt] [-][-] todo." );
      return false;
    }
  if ( it_next->isTrivialZero() && ! it_next->tilde )
    { // case [-][0^k] => [oo^k][-]
      #ifdef TRACE_DMLP
        cerr << "[-][0^k] => [oo^k][-]" << endl;
      #endif
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      moveToNextEdgeAndPossiblyMax( it_after_last, max );
      Edge sigmamoins = *it;
      Edge trivial = *it_next;
      trivial.initContext( sigmamoins.pointel, 
			   sigmamoins.a, sigmamoins.b, 
			   sigmamoins.inside );
      trivial.initTrivialInfinite( trivial.nb, false );
      sigmamoins.initContext( trivial );
      modifyEdge( it, trivial );
      modifyEdge( it_next, sigmamoins );
      return true;
    }
  EdgeListIterator it_prev = previousEdge( it );
  if ( it_prev == endEdge() ) return false;
  if ( it_prev->isQuadrantChange() )
    { // Go back if [+] or [-] before current.
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      #ifdef TRACE_DMLP
        cerr << "go back once." << endl;
      #endif
      return true;
    }
  moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
  moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
  if ( ( ! it_prev->isTrivial() ) || ( it_prev->nb > 1 ) )
    {  // [?p/q^k][-] => SR[?p/q^k][-]
      EdgeListIterator dummy_it;
      splitRight( it_prev, dummy_it );
      it_prev = previousEdge( it );
    }
  if ( it_prev->isTrivialInfinite() && it_prev->tilde )
    { // [~oo][-] => [+][~0]
      #ifdef TRACE_DMLP
        cerr << "[~oo][-] => [+][~0]" << endl;
      #endif
      ASSERT_GridCurve( it_prev->nb == 1 );
      Edge sigmaplus, newedge;
      sigmaplus.initContext( it_prev->pointel,
			     it_prev->a, it_prev->b,
			     it_prev->inside );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_prev, sigmaplus );
      newedge.initContext( sigmaplus );
      newedge.initEdge( 0, 1, 1, true );
      modifyEdge( it, newedge );
      return true;
    }
  moveToNextEdgeAndPossiblyMax( it_after_last, max );
  if ( ( ! it_next->isTrivial() ) || ( it_next->nb > 1 ) )
    {  // case [-][?p'/q'^l] => [-]SL[?p'/q'^l]
      EdgeListIterator dummy_it;
      splitLeft( it_next, dummy_it );
      it_next = nextEdge( it );
    }
  // We should be in the following cases: x,y in [0,~0,oo,~oo] 
  // configuration is [x][-][y].
  ASSERT_GridCurve( it_prev->isTrivial() && ( it_prev-> nb == 1 ) );
  ASSERT_GridCurve( it_next->isTrivial() && ( it_next-> nb == 1 ) );
  bool next_zero = it_next->isTrivialZero();
  bool next_tilde = it_next->tilde;
  bool prev_zero = it_prev->isTrivialZero();
  bool prev_tilde = it_prev->tilde;
  // extends the modified zone.
  unsigned int config = 
    ( next_tilde ? 8 : 0 ) + ( next_zero ? 0 : 4 )
    + ( prev_tilde ? 2 : 0 ) + ( prev_zero ? 0 : 1 );
  bool modified = false;
  Edge sigmaplus, sigmamoins;
  Edge newedge, newedge2;
  EdgeListIterator it_prev2, it_next2;
  switch ( config ) 
    {
    case 0: // [0][-][0]
    case 1: // [oo][-][0]
    case 2: // [~0][-][0]
    case 3: // [~oo][-][0]
      #ifdef TRACE_DMLP
        cerr << "debugcurve = " << *this << endl;
      #endif
      ASSERT_GridCurve( false && "[GridCurve::preSimplifyNegativeQuadrantChangeAt] should have been treated before." );
      break;
    case 4: // [0][-][oo]
      #ifdef TRACE_DMLP
        cerr << "debugcurve = " << *this << endl;
      #endif
      ASSERT_GridCurve( false && "[GridCurve::preSimplifyNegativeQuadrantChangeAt] geometrically invalid." );
      break;
    case 5: // [oo][-][oo] => [-][~0][~0]
      #ifdef TRACE_DMLP
        cerr << "[oo][-][oo] => [-][~0][~0]" << endl;
      #endif
      sigmamoins.initContext( it_prev->pointel,
			     it_prev->a, it_prev->b,
			     it_prev->inside );
      sigmamoins.initQuadrantChange( -1 );
      modifyEdge( it_prev, sigmamoins );
      newedge.initContext( sigmamoins );
      newedge.initEdge( 0, 1, 1, true );
      modifyEdge( it, newedge );
      newedge2.initContext( newedge );
      newedge2.initEdge( 0, 1, 1, true );
      modifyEdge( it_next, newedge2 );
      modified = true;
      break;
    case 7: // [~oo][-][oo] => [+][~1/1]
      #ifdef TRACE_DMLP
        cerr << "[~oo][-][oo] => [+][~1/1]" << endl;
      #endif
      ASSERT_GridCurve( false );
      sigmaplus.initContext( it_prev->pointel,
			     it_prev->a, it_prev->b,
			     it_prev->inside );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_prev, sigmaplus );
      newedge.initContext( sigmaplus );
      newedge.initEdge( 1, 1, 1, true );
      modifyEdge( it, newedge );
      removeEdge( it_next );
      modified = true;
      break;
    case 9: // [oo][-][~0] => [-][~0][oo]
      #ifdef TRACE_DMLP
        cerr << "[oo][-][~0] => [-][~0][oo]" << endl;
      #endif
      sigmamoins.initContext( it_prev->pointel,
			      it_prev->a, it_prev->b,
			      it_prev->inside );
      sigmamoins.initQuadrantChange( -1 );
      modifyEdge( it_prev, sigmamoins );
      newedge.initContext( sigmamoins );
      newedge.initEdge( 0, 1, 1, true );
      modifyEdge( it, newedge );
      newedge2.initContext( newedge );
      newedge2.initEdge( 1, 0, 1, false );
      modifyEdge( it_next, newedge2 );
      modified = true;
      break;
    case 10: // [~0][-][~0] => [+]
      #ifdef TRACE_DMLP
        cerr << "[~0][-][~0] => [+]" << endl;
      #endif
      sigmaplus.initContext( it_prev->pointel,
			     it_prev->a, it_prev->b,
			     it_prev->inside );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_prev, sigmaplus );
      removeEdge( it );
      removeEdge( it_next );
      modified = true;
      break;
    case 11: // [~oo][-][~0] => [+][1/1]
      #ifdef TRACE_DMLP
        cerr << "[~oo][-][~0] => [+][1/1]" << endl;
      #endif
      ASSERT_GridCurve( false );
      sigmaplus.initContext( it_prev->pointel,
			     it_prev->a, it_prev->b,
			     it_prev->inside );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_prev, sigmaplus );
      newedge.initContext( sigmaplus );
      newedge.initEdge( 1, 1, 1, false );
      modifyEdge( it, newedge );
      removeEdge( it_next );
      modified = true;
      break;
    case 12: // [0][-][~oo] => [~1/1][+]
      #ifdef TRACE_DMLP
        cerr << "[0][-][~oo] => [~1/1][+]" << endl;
      #endif
      newedge.initContext( it_prev->pointel,
			     it_prev->a, it_prev->b,
			     it_prev->inside );
      newedge.initEdge( 1, 1, 1, true );
      sigmaplus.initContext( newedge );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_prev, newedge );
      modifyEdge( it, sigmaplus );
      removeEdge( it_next );
      modified = true;
      break;
    case 13: // [oo][-][~oo] => [oo][~0][+]
      #ifdef TRACE_DMLP
        cerr << "[oo][-][~oo] => [oo][~0][+]" << endl;
      #endif
      newedge.initContext( *it_prev );
      newedge.initEdge( 0, 1, 1, true );
      sigmaplus.initContext( newedge );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it, newedge );
      modifyEdge( it_next, sigmaplus );
      modified = true;
      break;
    case 14: // [~0][-][~oo] => [1/1][+]
      #ifdef TRACE_DMLP
        cerr << "[~0][-][~oo] => [1/1][+]" << endl;
      #endif
      newedge.initContext( it_prev->pointel,
			   it_prev->a, it_prev->b,
			   it_prev->inside );
      newedge.initEdge( 1, 1, 1, false );
      sigmaplus.initContext( newedge );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_prev, newedge );
      modifyEdge( it, sigmaplus );
      removeEdge( it_next );
      modified = true;
      break;
    case 15: // [~oo][-][~oo] => [+][~0][~oo]
      #ifdef TRACE_DMLP
        cerr << "[~oo][-][~oo] => [+][~0][~oo]" << endl;
      #endif
      ASSERT_GridCurve( false );
      sigmaplus.initContext( it_prev->pointel,
			     it_prev->a, it_prev->b,
			     it_prev->inside );
      sigmaplus.initQuadrantChange( 1 );
      newedge.initContext( sigmaplus );
      newedge.initEdge( 0, 1, 1, true );
      modifyEdge( it_prev, sigmaplus );
      modifyEdge( it, newedge );
      modified = true;
      break;
    case 6: // [~0][-][oo]
      // special case, need previous edge, two possibilities :
      //  [0][~0][-][oo] => [+][~0]
      //  [~0][~0][-][oo] => [-][0]
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      it_prev2 = previousEdge( it_prev );
      if ( it_prev2 != endEdge() && ! it_prev2->isQuadrantChange() )
      {
        EdgeListIterator dummy_it;
        splitRight( it_prev2, dummy_it );
        it_prev2 = previousEdge( it_prev );
        if ( it_prev2->isTrivialZero() && ( it_prev2->nb == 1 ) 
            && ! it_prev2->tilde ) 
        {
#ifdef TRACE_DMLP
          cerr << "[0][~0][-][oo] => [+][~0]" << endl;
#endif
          sigmaplus.initContext( it_prev2->pointel,
              it_prev2->a, it_prev2->b,
              it_prev2->inside );
          sigmaplus.initQuadrantChange( 1 );
          newedge.initContext( sigmaplus );
          newedge.initEdge( 0, 1, 1, true );
          modifyEdge( it_prev2, sigmaplus );
          modifyEdge( it_prev, newedge );
          removeEdge( it );
          removeEdge( it_next );
          modified = true;
        }
        else if (it_prev2->isTrivialZero() && ( it_prev2->nb == 1 ) 
            && it_prev2->tilde ) 
        {
#ifdef TRACE_DMLP
          cerr << "[~0][~0][-][oo] => [-][0]" << endl;
#endif
          sigmamoins.initContext( it_prev2->pointel,
              it_prev2->a, it_prev2->b,
              it_prev2->inside );
          sigmamoins.initQuadrantChange( -1 );
          newedge.initContext( sigmamoins );
          newedge.initEdge( 0, 1, 1, false );
          modifyEdge( it_prev2, sigmaplus );
          modifyEdge( it_prev, newedge );
          removeEdge( it );
          removeEdge( it_next );
          modified = true;
        }
        else {
          ASSERT_GridCurve( false && "[GridCurve::preSimplifyNegativeQuadrantChangeAt] should not occur." );
          return false;
        }

      }
      break;
    case 8: // [0][-][~0]
      // special case, need next edge: [0][-][~0][oo] => [~0][+]
      moveToNextEdgeAndPossiblyMax( it_after_last, max );
      EdgeListIterator it_next2 = nextEdge( it_next );
      if ( it_next2 != endEdge() && ! it_next2->isQuadrantChange() )
	{
	  #ifdef TRACE_DMLP
      cerr << "[0][-][~0][oo] => [~0][+]" << endl;
	  #endif
	  EdgeListIterator dummy_it;
	  splitLeft( it_next2, dummy_it );
	  it_next2 = nextEdge( it_next );
    	  ASSERT_GridCurve( it_next2->isTrivialInfinite() 
			    && ( it_next2->nb == 1 ) && ! it_next2->tilde );
	  newedge.initContext( it_prev->pointel,
			       it_prev->a, it_prev->b,
			       it_prev->inside );
	  newedge.initEdge( 0, 1, 1, true );
	  sigmaplus.initContext( newedge );
	  sigmaplus.initQuadrantChange( 1 );
	  modifyEdge( it_prev, newedge );
	  modifyEdge( it, sigmaplus );
	  removeEdge( it_next );
	  removeEdge( it_next2 );
	  modified = true;
	}
      break;
    }
  return modified;
}

bool
ImaGene::GridCurve::preSimplifyPositiveQuadrantChangeAt
( EdgeListIterator & it,
  EdgeListIterator & it_before_first, 
  EdgeListIterator & it_after_last,
  EdgeListIterator & min,
  EdgeListIterator & max )
{
  EdgeListIterator it_next = it_after_last; // nextEdge( it );
  // if ( it_next == endEdge() ) return false;
  if ( it_next->isQuadrantChange() && ( it_next->nb < 0 ) )
    { // case [+][-] => nothing.
      #ifdef TRACE_DMLP
        cerr << "[+][-] => nothing." << endl;
      #endif
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      moveToNextEdgeAndPossiblyMax( it_after_last, max );
      it = removeEdge( it );
      it = removeEdge( it );
      return true;
    }
  if ( it_next->isTrivialInfinite() && ( ! it_next->tilde ) )
    { // case [+][oo^k] => [0^k][+]
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      moveToNextEdgeAndPossiblyMax( it_after_last, max );
      Edge newedge, plus;
      newedge.initContext( it->pointel, it->a, it->b, it->inside );
      newedge.initEdge( 0, 1, it_next->nb, false );
      modifyEdge( it, newedge );
      plus.initContext( newedge );
      plus.initQuadrantChange( 1 );
      modifyEdge( it_next, plus );
      return true;
    }
  return false;
}

bool
ImaGene::GridCurve::preSimplifyTwoNegativeQuadrantChangesAt
( EdgeListIterator & it,
  EdgeListIterator & it_before_first, 
  EdgeListIterator & it_after_last,
  EdgeListIterator & min,
  EdgeListIterator & max )
{
  // it -> [-], it_before_first = it, it_after_last = it_next -> [-]
  EdgeListIterator it_next = it_after_last;
  EdgeListIterator it_prev = previousEdge( it );
  if ( it_prev == endEdge() ) return false;
  if ( it_prev->isQuadrantChange() )
    { // Go back if [+] or [-] before current.
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      return true;
    }
  moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
  moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
  if ( ( ! it_prev->isTrivial() ) || ( it_prev->nb > 1 ) )
    {  // [?p/q^k][-][-] => SR[?p/q^k][-][-]
      EdgeListIterator dummy_it;
      splitRight( it_prev, dummy_it );
      it_prev = previousEdge( it );
    }
  if ( it_prev->isTrivialInfinite() && it_prev->tilde )
    { // [~oo][-][-] => [+][~0][-]
      #ifdef TRACE_DMLP
        cerr << "[~oo][-][-] => [+][~0][-]" << endl;
      #endif
      ASSERT_GridCurve( it_prev->nb == 1 );
      Edge sigmaplus, newedge;
      sigmaplus.initContext( it_prev->pointel,
			     it_prev->a, it_prev->b,
			     it_prev->inside );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_prev, sigmaplus );
      newedge.initContext( sigmaplus );
      newedge.initEdge( 0, 1, 1, true );
      modifyEdge( it, newedge );
      return true;
    }

  EdgeListIterator it_next2 = nextEdge( it_next );
  if ( it_next2 == endEdge() ) return false;
  if ( it_next2->isQuadrantChange() )
    if ( it_next2->nb > 0 )
      { // case [-][-][+] => [-]
	#ifdef TRACE_DMLP
    cerr << "[-][-][+] => [-]" << endl;
	#      endif
	moveToNextEdgeAndPossiblyMax( it_after_last, max );
	moveToNextEdgeAndPossiblyMax( it_after_last, max );
	removeEdge( it_next );
	removeEdge( it_next2 );
	return true;
      }
    else // case [p/q][-][-][-][p'/q'] => [~q/p][-][~p'/q']
      {
	#ifdef TRACE_DMLP
    cerr << "[p/q][-][-][-][p'/q'] => [~q/p][-][~p'/q']" << endl;
	#      endif
	EdgeListIterator it_next3 = nextEdge( it_next2 );
	if ( ( it_next3 == endEdge() ) || it_next3->isQuadrantChange() )
	  {
	    #ifdef TRACE_DMLP
        cerr << "debugcurve = " << *this << endl;
	    #endif
	    ASSERT_GridCurve( false && "[GridCurve::preSimplifyTwoNegativeQuadrantChangesAt] should not occur." );
	    return false;
	  }
	moveToNextEdgeAndPossiblyMax( it_after_last, max );
	moveToNextEdgeAndPossiblyMax( it_after_last, max );
	moveToNextEdgeAndPossiblyMax( it_after_last, max );
	moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
	EdgeListIterator dummy_it;
	splitRight( it_prev, dummy_it );
	it_prev = previousEdge( it );
	Edge newedge, newedge2, sigmamoins;
	newedge.initContext( it_prev->pointel,
			     it_prev->a, it_prev->b,
			     it_prev->inside );
	newedge.initEdge( it_prev->pq.q(), it_prev->pq.p(), 
			  1, ! it_prev->tilde );
	modifyEdge( it_prev, newedge );
	sigmamoins.initContext( newedge );
	sigmamoins.initQuadrantChange( -1 );
	modifyEdge( it, sigmamoins );
	newedge2.initContext( sigmamoins );
	newedge2.initEdge( it_next3->pq.p(), it_next3->pq.q(), 
			   it_next3->nb, ! it_next3->tilde );
	modifyEdge( it_next, newedge2 );
	removeEdge( it_next2 );
	removeEdge( it_next3 );
	return true;
      }
  // here, we are sure that in [x][-][-][y], [x] and [y] are true edges.
  moveToNextEdgeAndPossiblyMax( it_after_last, max );
  moveToNextEdgeAndPossiblyMax( it_after_last, max );
  if ( ( ! it_next2->isTrivial() ) || ( it_next2->nb > 1 ) )
    {  // case [-][-][?p'/q'^l] => [-][-]SL[?p'/q'^l]
      EdgeListIterator dummy_it;
      splitLeft( it_next2, dummy_it );
      it_next2 = nextEdge( it_next );
    }
  // We should be in the following cases: x,y in [0,~0,oo,~oo] 
  // configuration is [x][-][-][y].
  ASSERT_GridCurve( it_prev->isTrivial() && ( it_prev-> nb == 1 ) );
  ASSERT_GridCurve( it_next2->isTrivial() && ( it_next2-> nb == 1 ) );
  bool next2_zero = it_next2->isTrivialZero();
  bool next2_tilde = it_next2->tilde;
  bool prev_zero = it_prev->isTrivialZero();
  bool prev_tilde = it_prev->tilde;
  // extends the modified zone.
  unsigned int config = 
    ( next2_tilde ? 8 : 0 ) + ( next2_zero ? 0 : 4 )
    + ( prev_tilde ? 2 : 0 ) + ( prev_zero ? 0 : 1 );
  bool modified = false;
  Edge sigmamoins, sigmamoins2;
  Edge sigmaplus, sigmaplus2;
  Edge newedge;
  Edge newedge2;
  EdgeListIterator it_prev2, it_next3, dummy;
  switch ( config ) 
    {
    case 1: // [oo][-][-][0] => [-][~0][~0][-]
      #ifdef TRACE_DMLP
        cerr << "[oo][-][-][0] => [-][~0][~0][-]" << endl;
      #endif
      sigmamoins.initContext( it_prev->pointel,
			      it_prev->a, it_prev->b,
			      it_prev->inside );
      sigmamoins.initQuadrantChange( -1 );
      modifyEdge( it_prev, sigmamoins );
      newedge.initContext( sigmamoins );
      newedge.initEdge( 0, 1, 1, true );
      modifyEdge( it, newedge );
      newedge2.initContext( newedge );
      newedge2.initEdge( 0, 1, 1, true );
      modifyEdge( it_next, newedge2 );
      sigmamoins.initContext( newedge2 );
      modifyEdge( it_next2, sigmamoins );
      modified = true;
      break;
    case 13: // [oo][-][-][~oo] => [-][~0][oo][+]
      #ifdef TRACE_DMLP
        cerr << "[oo][-][-][~oo] => [-][~0][oo][+]" << endl;
      #endif
      sigmamoins.initContext( it_prev->pointel,
			      it_prev->a, it_prev->b,
			      it_prev->inside );
      sigmamoins.initQuadrantChange( -1 );
      modifyEdge( it_prev, sigmamoins );
      newedge.initContext( sigmamoins );
      newedge.initEdge( 0, 1, 1, true );
      modifyEdge( it, newedge );
      newedge2.initContext( newedge );
      newedge2.initEdge( 1, 0, 1, false );
      modifyEdge( it_next, newedge2 );
      sigmaplus.initContext( newedge2 );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_next2, sigmaplus );
      modified = true;
      break;
    case 10: // [~0][-][-][~0]. Must be of the form [0][~0][-][-][~0][oo]
      // => [+][+]
      #ifdef TRACE_DMLP
        cerr << "[0][~0][-][-][~0][oo] => [+][+]" << endl;
      #endif
      it_prev2 = previousEdge( it_prev );
      it_next3 = nextEdge( it_next2 );
      if ( ( it_prev2 == endEdge() ) || it_prev2->isQuadrantChange() 
	   || ( it_next3 == endEdge() ) || it_next3->isQuadrantChange() )
	{
	  #ifdef TRACE_DMLP
      cerr << "debugcurve = " << *this << endl;
	  #endif
	  ASSERT_GridCurve( false && "[GridCurve::preSimplifyTwoNegativeQuadrantChangesAt] should not occur." );
	  modified = false;
	  break;
	}
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      moveToNextEdgeAndPossiblyMax( it_after_last, max );
      splitRight( it_prev2, dummy );
      it_prev2 = previousEdge( it_prev );
      splitLeft( it_next3, dummy );
      it_next3 = nextEdge( it_next2 );
      ASSERT_GridCurve( it_prev2->isTrivialZero() && ( it_prev2->nb == 1 ) );
      ASSERT_GridCurve( ! it_prev2->tilde );
      ASSERT_GridCurve( it_next3->isTrivialInfinite() && ( it_next3->nb == 1 ) );
      ASSERT_GridCurve( ! it_next3->tilde );
      sigmaplus.initContext( it_prev2->pointel,
			     it_prev2->a, it_prev2->b,
			     it_prev2->inside );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_prev2, sigmaplus );
      sigmaplus2.initContext( sigmaplus );
      sigmaplus2.initQuadrantChange( 1 );
      modifyEdge( it_prev, sigmaplus );
      removeEdge( it );
      removeEdge( it_next );
      removeEdge( it_next2 );
      removeEdge( it_next3 );
      modified = true;
      break;
    case 2: // [~0][-][-][0]. 
      // [p/q][~0][-][-][0] => [~q/p][-][-]
      #ifdef TRACE_DMLP
        cerr << "[p/q][~0][-][-][0] => [~q/p][-][-]" << endl;
      #endif
      it_prev2 = previousEdge( it_prev );
      if ( ( it_prev2 == endEdge() ) || it_prev2->isQuadrantChange() )
	{
	  #ifdef TRACE_DMLP
      cerr << "debugcurve = " << *this << endl;
	  #endif
	  ASSERT_GridCurve( false && "[GridCurve::preSimplifyTwoNegativeQuadrantChangesAt] should not occur." );
	  modified = false;
	  break;
	}
      moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      splitRight( it_prev2, dummy );
      it_prev2 = previousEdge( it_prev );
      newedge.initContext( it_prev2->pointel,
			   it_prev2->a, it_prev2->b,
			   it_prev2->inside );
      newedge.initEdge( it_prev2->pq.q(), it_prev2->pq.p(), 1, 
			! it_prev2->tilde );
      modifyEdge( it_prev2, newedge );
      sigmamoins.initContext( newedge );
      sigmamoins.initQuadrantChange( -1 );
      modifyEdge( it_prev, sigmamoins );
      sigmamoins2.initContext( sigmamoins );
      sigmamoins2.initQuadrantChange( -1 );
      modifyEdge( it, sigmamoins2 );
      removeEdge( it_next );
      removeEdge( it_next2 );
      modified = true;

      // // Must be of the form [0][~0][-][-][0] => [+][~0][-]
      // // or (invalid !) [oo][~0][-][-][0] => [~0][-][-]
      // it_prev2 = previousEdge( it_prev );
      // if ( ( it_prev2 == endEdge() ) || it_prev2->isQuadrantChange() )
      // 	{
      // 	  cerr << "debugcurve = " << *this << endl;
      // 	  ASSERT_GridCurve( false && "[GridCurve::preSimplifyTwoNegativeQuadrantChangesAt] should not occur." );
      // 	  modified = false;
      // 	  break;
      // 	}
      // moveToPreviousEdgeAndPossiblyMin( it_before_first, min );
      // splitRight( it_prev2, dummy );
      // it_prev2 = previousEdge( it_prev );
      // if ( (it_prev2->nb != 1 ) || ( it_prev2->tilde ) )
      // 	{
      // 	  cerr << "debugcurve = Polygon( [ "
      // 	       << *it_prev2 << "," << *it_prev 
      // 	       << "," << *it << "," << *it_next
      // 	       << "," << *it_next2 << "," << *it_next3 << std::endl;
      // 	}
      // ASSERT_GridCurve( ( it_prev2->nb == 1 ) );
      // ASSERT_GridCurve( ! it_prev2->tilde );
      // if ( it_prev2->isTrivialZero() )
      // 	{
      // 	  sigmaplus.initContext( it_prev2->pointel,
      // 				 it_prev2->a, it_prev2->b,
      // 				 it_prev2->inside );
      // 	  sigmaplus.initQuadrantChange( 1 );
      // 	  modifyEdge( it_prev2, sigmaplus );
      // 	  newedge.initContext( sigmaplus );
      // 	  newedge.initEdge( 0, 1, 1, true );
      // 	  modifyEdge( it_prev, newedge );
      // 	  sigmamoins.initContext( newedge );
      // 	  sigmamoins.initQuadrantChange( - 1 );
      // 	  modifyEdge( it, sigmamoins );
      // 	  removeEdge( it_next );
      // 	  removeEdge( it_next2 );
      // 	  modified = true;
      // 	}
      // else if ( it_prev2->isTrivialInfinite() )
      // 	{
      // 	  newedge.initContext( it_prev2->pointel,
      // 			       it_prev2->a, it_prev2->b,
      // 			       it_prev2->inside );
      // 	  newedge.initEdge( 0, 1, 1, true );
      // 	  modifyEdge( it_prev2, newedge );
      // 	  sigmamoins.initContext( newedge );
      // 	  sigmamoins.initQuadrantChange( -1 );
      // 	  modifyEdge( it_prev, sigmamoins );
      // 	  sigmamoins2.initContext( sigmamoins );
      // 	  sigmamoins2.initQuadrantChange( -1 );
      // 	  modifyEdge( it, sigmamoins2 );
      // 	  removeEdge( it_next );
      // 	  removeEdge( it_next2 );
      // 	  modified = true;
      // 	}
      // else 
      // 	ASSERT_GridCurve( false );
      break;
    case 9: // [oo][-][-][~0]. Must be of the form 
      // [oo][-][-][~0][0] =>  [-][~oo][+]     or
      // [oo][-][-][~0][oo] => [-][~0][+]
      it_next3 = nextEdge( it_next2 );
      if ( ( it_next3 == endEdge() ) || it_next3->isQuadrantChange() )
	{
	  #ifdef TRACE_DMLP
      cerr << "debugcurve = " << *this << endl;
	  #endif
	  ASSERT_GridCurve( false && "[GridCurve::preSimplifyTwoNegativeQuadrantChangesAt] should not occur." );
	  modified = false;
	  break;
	}
      moveToNextEdgeAndPossiblyMax( it_after_last, max );
      splitLeft( it_next3, dummy );
      it_next3 = nextEdge( it_next2 );
      ASSERT_GridCurve( ( it_next3->nb == 1 ) );
      ASSERT_GridCurve( ! it_next3->tilde );
      sigmamoins.initContext( it_prev->pointel,
			      it_prev->a, it_prev->b,
			      it_prev->inside );
      sigmamoins.initQuadrantChange( -1 );
      modifyEdge( it_prev, sigmamoins );
      newedge.initContext( sigmamoins );
      if ( it_next3->isTrivialZero() )
	{
	  #ifdef TRACE_DMLP
      cerr << "[oo][-][-][~0][0] =>  [-][~oo][+]" << endl;
	  #endif
	  newedge.initEdge( 1, 0, 1, true ); // [~oo]
	}
      else
	{
	  #ifdef TRACE_DMLP
      cerr << "[oo][-][-][~0][oo] => [-][~0][+]" << endl;
	  #endif
	  newedge.initEdge( 0, 1, 1, true ); // [~0]
	}
      modifyEdge( it, newedge );
      sigmaplus.initContext( newedge );
      sigmaplus.initQuadrantChange( 1 );
      modifyEdge( it_next, sigmaplus );
      removeEdge( it_next2 );
      removeEdge( it_next3 );
      modified = true;
      break;
    default: // [0][-][-][0], [~oo][-][-][0], etc
      #ifdef TRACE_DMLP
        cerr << "debugcurve = " << *this << endl;
      #endif
      ASSERT_GridCurve( false && "[GridCurve::preSimplifyTwoNegativeQuadrantChangesAt] should not occur." );
      break;  
    }
  return modified;
}

bool
ImaGene::GridCurve::simplifyZone
( EdgeListIterator & min,
  EdgeListIterator & max )
{
  EdgeListIterator it = min;
  unsigned int s = 0;
  unsigned int i = 0;
  bool modif = false;
  for ( ; it != max; it = nextEdge( it ) ) ++s;
  it = min;
  while ( ( i < s ) || ( it != max ) )
    {
      #ifdef TRACE_DMLP
        cerr << "[SimplifyZone] " 
          << i 
          << " " << *it << " " << it->pointel
          << ", min=" << *min << " " << min->pointel 
          << ", max=" << *max << " " << max->pointel <<  endl;
      #endif
      EdgeListIterator it_before_first, it_after_last;
      #ifdef TRACE_DMLP
        cerr << "gridcurve = " << *this << endl;
      #endif
      bool pre_simplify = preSimplifyAt( it, 
					 it_before_first, it_after_last,
					 min, max );
      ++i;
      if ( pre_simplify ) modif = true;
      // cerr << "[SimplifyZoneAfter] "
      // 	   << ", bf=" << *it_before_first
      // 	   << ", al=" << *it_after_last
      // 	   << ", min=" << *min
      // 	   << ", max=" << *max;
      // cerr << ( pre_simplify ? " PreSimplified" : "" ) << endl;
      if ( pre_simplify )
	// checks the case where the grid curve is not a loop.
	it = it_before_first == endEdge() ? beginEdge() : it_before_first;
      else
	it = nextEdge( it );
    }
  return modif;
}

/**
   @param v (modified) simplifies inplace the Freeman chaincode
   by removing couples of opposite letters.
*/
void
ImaGene::GridCurve::simplifyFreemanCode( std::vector<unsigned char> & v )
{
  int nb = v.size();
  int j = 0;
  unsigned char precl = v[ 0 ];
  for ( int i = 1; i < nb; ++i )
    {
      unsigned char l = v[ i ];
      if ( ( j < 0 ) || ( v[ i ] != ( v[ j ] ^ 2 ) ) )
	{
	  ++j;
	  v[ j ] = v[ i ];
	}
      else
	--j;
    }
  v.resize( j+1 );
}

/**
   Splits the edge [it_edge] exactly at the position [position]. More
   precisely, there is a vertex of the grid curve at the specified
   position afterward. NB: if the edge visitor is ona quadrant edge,
   nothing is done.
   
   @param it_edge (modified) as input, an edge iterator on a "true"
   edge (not a quadrant change), as output an iterator pointing on the
   edge which starts at this vertex.

   @param position the position in number of steps where to split in
   the edge (0 is the start of the edge). Should not be equal or
   greater than the number of steps of this edge.

   @param inside when 'true', splits are done so as to be closer
   to the inside (below the edge); when 'false', splits are done
   so as to be closer to the outside (above the edge ). This is
   done with calls to splitDown and splitUp depending on the
   inside property of the edge.
*/
void
ImaGene::GridCurve::splitEdgeAt( EdgeListIterator & it_edge, 
				 unsigned int & position,
				 bool inside )
{
  if ( it_edge->isTrivialInfinite() && it_edge->tilde ) 
    { // ~oo corresponds to three freeman codes in the interpixel (a(b+2)a)
      if ( position <= 2 ) return;
      position = position - 2;
    }
  if ( position == 0 ) return;

  #ifdef TRACE_DMLP
    std::cerr << "[splitEdgeAt] edge=" << *it_edge << " pos=" << position << std::endl;
    std::cerr << "gridcurve = " << *this << std::endl;
  #endif
  unsigned int sum_pq = it_edge->pq.p() + it_edge->pq.q();
  unsigned int n = it_edge->nb;
  ASSERT_GridCurve( ( position < ( n * sum_pq ) ) );

  unsigned int k = position / sum_pq;
  Edge newedge1, newedge2, newedge3;
  if ( ( k == 0 ) && ( n == 1 ) )
    { // the edge is primitive
      EdgeListIterator it_prev = previousEdge( it_edge );
      EdgeListIterator it_dummy;
      if ( it_edge->inside == inside )
	splitDown( it_edge, it_dummy );
      else
	splitUp( it_edge, it_dummy );
      it_edge = nextEdge( it_prev );
      while ( ( sum_pq = it_edge->nb * ( it_edge->pq.p() + it_edge->pq.q() ) )
	      <= position )
	{
	  position -= sum_pq;
	  it_edge = nextEdge( it_edge );
	}
    }
  else if ( k == 0 )
    { // Make edge primitive.
      newedge2 = newedge3 = *it_edge;
      newedge2.nb = 1;
      modifyEdge( it_edge, newedge2 );
      EdgeListIterator it_next = nextEdge( it_edge );
      newedge3.initContext( newedge2 );
      newedge3.nb = n - 1;
      newedge3.tilde = false;
      insertEdge( it_next, newedge3 );
    }
  else 
    {  // Make edge primitive (general case with 2 or 3 edges).
      newedge1 = newedge2 = newedge3 = *it_edge;
      newedge1.nb = k;
      insertEdge( it_edge, newedge1 );
      newedge2.initContext( newedge1 );
      newedge2.nb = 1;
      newedge2.tilde = false;
      modifyEdge( it_edge, newedge2 );
      if ( k < ( n - 1 ) )
	{
	  EdgeListIterator it_next = nextEdge( it_edge );
	  newedge3.initContext( newedge2 );
	  newedge3.nb = n - k - 1;
	  newedge3.tilde = false;
	  insertEdge( it_next, newedge3 );
	}
      position -= k * sum_pq;
    }
  // recursive call.
  splitEdgeAt( it_edge, position, inside );
}

/**
   Flips the dynamic MLP at position specified by [it] and in the
   direction specified by 'it.inside'.
     
   @param it specifies the position where to flip the MLP.

   @param inside when 'true', flips the inside pixel associated to
   the visitor [it] (i.e., removes a pixel from the shape for a
   closed grid curve), when 'false', flips the outside pixel
   associated to the visitor [it].
     
   @param begin_mod (modified) Visitor pointing on the beginning of
   the modified part of the MLP.

   @param end_mod (modified) Visitor pointing after the end of the
   modified part of the MLP.

   @param store_undo when 'true', stores the reverse flip
   operation that will undo this change. This parameter may be
   false only if the stack 'm_undo' is empty.
     
   @see undoFlip
   @see topUndo
   @see m_undo
*/
void 
ImaGene::GridCurve::flip
( const Visitor & it, bool inside,
  Visitor & begin_mod, Visitor & end_mod,
  bool store_undo )
{
  // Taking care of possible undo(s).
  if ( store_undo )
    {
      myCurve.setStoringMode( true );
      myCurve.beginModifGroup();
      oldLengths.push(myLength);
    }
  else
    myCurve.setStoringMode( false );

  //  std::cout<<"Before flip: "<<myLength;
  
  // position iterators around the flip so as to be able to return the
  // window of modifications.
  EdgeListIterator it_edge = it.edgeIterator();
  EdgeListIterator it_before_first = previousEdge( it_edge );
  EdgeListIterator it_after_last = nextEdge( it_edge );
  EdgeListIterator it_dummy;
  unsigned int position = it.position();
  #ifdef TRACE_DMLP
    std::cerr << "[flip] before split edge " << *it_edge << " at " << position << std::endl;
  #endif

  // If it is not a quadrant change, we must make sure that edges are
  // correctly split around the position chosen for the flip.
  if ( ! it_edge->isQuadrantChange() )
    { // we must split the edge.
      splitEdgeAt( it_edge, position, inside );
      splitLeft( it_edge, it_dummy );
      it_edge = nextEdge( it_edge );
    }
  #ifdef TRACE_DMLP
    std::cerr << "[flip] after split edge " 
      << *it_edge << " at " << position << std::endl;
    std::cerr << "gridcurve = " << *this << std::endl;
  #endif

  // There are several cases for a flip, depending if we just have to
  // modify a vertex inside/outside, or if we have to add an inside or
  // outside bump. The purpose of the following tests are to determine
  // the correct case.
  if ( it_edge->isTrivialInfinite() && it_edge->tilde )
    {
      ASSERT_GridCurve( it_edge->nb == 1 );
      flipPrimitiveTildeInfinite( it_edge, position, inside,
				  it_before_first, it_after_last );
    }
  else
    {
      ASSERT_GridCurve( it_edge->isTrivial() || it_edge->isQuadrantChange() );
      if ( it_edge->inside == inside ) 
	{
	  if ( it_edge->isTrivialZero() || it_edge->isQuadrantChange() ) 
	    { // case 0, ~0, +
	      // we must swap inside / outside.
	      #ifdef TRACE_DMLP
          std::cerr << "[flip] 0, ~0, + call swapTilde. " << std::endl; 
	      #endif
	      swapTilde( it_edge, it_before_first, it_after_last );
	    }
	  else 
	    { // case oo
	      // we must swap the next one inside / outside.
	      #ifdef TRACE_DMLP
          std::cerr << "[flip] oo call swapTilde on next. " << std::endl; 
	      #endif
	      it_edge = nextEdge( it_edge );
	      swapTilde( it_edge, it_before_first, it_after_last );
	    }
	}
      else // We know it_edge->inside != inside
	{
	  if ( it_edge->isQuadrantChange() 
	       || ( it_edge->isTrivialZero() && ! it_edge->tilde ) )
	    { // case +, 0
	      insertBump( it_edge, true );
	    }
	  else if ( it_edge->isTrivialInfinite() )
	    { // case oo
	      it_edge = nextEdge( it_edge );
	      insertBump( it_edge, false );
	    }
	  else
	    { // case ~0 
	      #ifdef TRACE_DMLP
          std::cerr << "[flip] ~0 call swapTilde on next. " << std::endl; 
	      #endif
	      it_edge = nextEdge( it_edge );
	      swapTilde( it_edge, it_before_first, it_after_last );
	    }
	}
    }
  // We position the visitors so as to define the window of
  // modifications.
  Visitor b( *this, nextEdge( it_before_first ) );
  --b;
  Visitor e( *this, it_after_last );
  begin_mod = b;
  end_mod = e;
  #ifdef TRACE_DMLP
    cerr << "debugcurve = " << *this << endl;
  #endif
  // Taking care of possible undo(s).
  if ( store_undo )
    {
      myCurve.endModifGroup();
    }  
}

void
ImaGene::GridCurve::insertBump( EdgeListIterator & it_edge, bool positive )
{
  // ASSERT_GridCurve( it_edge->isTrivial() 
  // 		    || it_edge->isQuadrantChange() );
  if ( positive )
    { // positive Bump.
      // we must add [oo][+][0][-]
      #ifdef TRACE_DMLP
        std::cerr << "[insertBump] add [oo][+][0][-]. " << std::endl; 
      #endif
      Edge infinite, plus, zero, minus;
      infinite.initContext( it_edge->pointel, 
			    it_edge->a, it_edge->b, 
			    it_edge->inside );
      infinite.initEdge( 1, 0, 1, false );
      insertEdge( it_edge, infinite );
      plus.initContext( infinite );
      plus.initQuadrantChange( 1 );
      insertEdge( it_edge, plus );
      zero.initContext( plus );
      zero.initEdge( 0, 1, 1, false );
      insertEdge( it_edge, zero );
      minus.initContext( zero );
      minus.initQuadrantChange( -1 );
      insertEdge( it_edge, minus );
    }
  else
    { // creates a negative bump.
      Edge minus, infinite, plus, zero;
      minus.initContext( it_edge->pointel, 
			 it_edge->a, it_edge->b, 
			 it_edge->inside );
      minus.initQuadrantChange( -1 );
      insertEdge( it_edge, minus );
      infinite.initContext( minus );
      infinite.initEdge( 1, 0, 1, false );
      insertEdge( it_edge, infinite );
      plus.initContext( infinite );
      plus.initQuadrantChange( 1 );
      insertEdge( it_edge, plus );
      zero.initContext( plus );
      zero.initEdge( 0, 1, 1, false );
      insertEdge( it_edge, zero );
    }
}

void
ImaGene::GridCurve::flipPrimitiveTildeInfinite
( EdgeListIterator & it_edge, unsigned int position, bool inside,
  EdgeListIterator & it_before_first, EdgeListIterator & it_after_last )
{
  // We are on a [~oo^1], position is either 0, 1, 2
  ASSERT_GridCurve( it_edge->nb == 1 );
  ASSERT_GridCurve( position <= 2 );
  EdgeListIterator it_dummy;
  if ( it_edge->inside == inside )
    { // one wants the start pixel to be flipped.
      if ( position != 2 )
	{ // Easy case.
	  // we must swap inside / outside.
	  #ifdef TRACE_DMLP
      std::cerr << "[flip] [~oo] pos 0,1, call swapTilde. " << std::endl; 
	  #endif
	  swapTilde( it_edge, it_before_first, it_after_last );
	}
      else
	{ // creates a negative bump.
	  EdgeListIterator it_next = nextEdge( it_edge );
	  insertBump( it_next, false );
	}
    }
  else
    {
      if ( position != 0 )
	{ // Easy case.
	  // we must swap inside / outside the next one.
	  #ifdef TRACE_DMLP
      std::cerr << "[flip] [~oo] pos 1,2, call swapTilde on next. " << std::endl; 
	  #endif
	  it_edge = nextEdge( it_edge );
	  swapTilde( it_edge, it_before_first, it_after_last );
	}
      else
	{
	  insertBump( it_edge, true );
	}
    }
}

/**
   Put a tilde just before position [it] if possible (not a quadrant
   change) or move it backward until it finds a proper position. Then
   comes back to put a tilde afterwards.
   
   @param it (modified) any iterator on an edge, outputs an iterator
   before the first modified edge.

   @param it_after_last (returns) outputs an iterator after the last
   modified edge.
*/
void
ImaGene::GridCurve::swapTilde( EdgeListIterator & it,
			       EdgeListIterator & it_before_first,
			       EdgeListIterator & it_after_last )
{
  EdgeListIterator it_bwd = it;
  moveToPreviousEdgeAndPossiblyMin( it_bwd, it_before_first );
  while ( ( it_bwd != endEdge() ) && ( it_bwd->isQuadrantChange() ) )
    moveToPreviousEdgeAndPossiblyMin( it_bwd, it_before_first );
  if ( it_bwd == endEdge() ) 
    {
      it = endEdge();
      ASSERT_GridCurve( false );
      return;
    }
  // Modify first edge before [it].
  EdgeListIterator it_next = it_bwd;
  moveToNextEdgeAndPossiblyMax( it_next, it_after_last );
  EdgeListIterator it_after;
  splitRight( it_bwd, it_after );
  ASSERT_GridCurve( it_next == it_after );
  it = it_bwd; // it becomes it_before_first
  it_bwd = previousEdge( it_after );
  ASSERT_GridCurve( it_bwd->isTrivial() );
  ASSERT_GridCurve( it_bwd->nb == 1 );
  Edge newedge = *it_bwd;
  newedge.pq = it_bwd->pq.inverse();
  newedge.tilde = ! it_bwd->tilde;
  modifyEdge( it_bwd, newedge );
  // Take care of in-between quadrant changes.
  EdgeListIterator it_moving = it_bwd;
  moveToNextEdgeAndPossiblyMax( it_moving, it_after_last );
  while ( ( it_moving != endEdge() ) && ( it_moving->isQuadrantChange() ) )
    {
      Edge quadrant;
      quadrant.initContext( *it_bwd );
      quadrant.initQuadrantChange( - it_moving->nb );
      modifyEdge( it_moving, quadrant );
      it_bwd = it_moving;
      moveToNextEdgeAndPossiblyMax( it_moving, it_after_last );
    }
  if ( it_moving == endEdge() )
    {
      it_after_last = endEdge();
      ASSERT_GridCurve( false );
      return;
    }
  // Modify first edge after [it].
  splitLeft( it_moving, it_after ); // it_after is a dummy iterator.
  it_moving = nextEdge( it_bwd );
  ASSERT_GridCurve( it_moving->isTrivial() );
  ASSERT_GridCurve( it_moving->nb == 1 );
  newedge.initContext( *it_bwd );
  newedge.initEdge( it_moving->pq, it_moving->nb, ! it_moving->tilde );
  modifyEdge( it_moving, newedge );
}

/**
   Removes null vectors.
*/
void
ImaGene::ReversiblePolygonalRepresentation::purge()
{
  unsigned int i = 0;
  unsigned int j = 0;
  while ( i < vx.size() )
    {
      while ( ( i < vx.size() ) 
	      && ( vx[ i ] == vx[ j ] ) && ( vy[ i ] == vy[ j ] ) )
	++i;
      ++j;
      if ( ( i < vx.size() ) && ( i != j ) )
	{
	  vx[ j ] = vx[ i ];
	  vy[ j ] = vy[ i ];
	  vt[ j ] = vt[ i ];
	}
    }
  if ( j != i )
    {
      vx.resize( j );
      vy.resize( j );
      vt.resize( j );
    }
}

/**
   n twice_dv_x twice_dv_y cw
   vx[0] vy[0] vt[0]
   vx[1] vy[1] vt[1]
   ...
   vx[n-1] vy[n-1] vt[n-1]
*/
bool
ImaGene::ReversiblePolygonalRepresentation::read( std::istream & in )
{
  if ( ! in.good() ) return false;
  int n, dvx, dvy; 
  in >> n >> dvx >> dvy >> this->cw;
  if ( in.good() )
    {
      this->twice_dv = Vector2i( dvx, dvy );
      this->vx.resize( n );
      this->vy.resize( n );
      this->vt.resize( n );
      for ( int i = 0; i < n; ++i )
	{
	  int t;
	  in >> vx[ i ] >> vy[ i ] >> t;
	  vt[ i ] = t != 0;
	}
    }
  return in.good();
}



void ImaGene::GridCurve::initFromInterpixelFreemanChain 
( const FreemanChain & fc )
{
  bool cvx = true;
  OrderedAlphabet A( '0', 4 );
  FreemanChain::const_iterator it = fc.findQuadrantChange4( A );
  uint a1 = A.letter(1) - '0';
  uint a2 = A.letter(2) - '0';

  // Compute displacement vector.
  FreemanChain::const_iterator itprev = it;
  itprev.previousInLoop();
  Vector2i v_it( 0, 0 );
  Vector2i v_itprev( 0, 0 );
  v_it.move4( it.getCode() );
  v_itprev.move4( itprev.getCode() );
  myTwiceDV = v_it - v_itprev;

  // Test if cw or ccw
  uint nb_ccw = fc.isClosed();
  ASSERT_GridCurve( nb_ccw != 0 );
  myCW = ( nb_ccw < 0 );
  myIsClosed = ( nb_ccw != 0 );
  uint move = FreemanChain::movement( a1, a2, nb_ccw > 0 );
  ASSERT_GridCurve( (move == 1)  || (move == 3) );

  uint j = it.getPosition();
  uint end = j;
  uint nb_a1;
  uint nb_a2;
  string w = fc.chain;
  Vector2i xy = *it;
  GridCurve::Edge e;
  e.initContext( xy, a1, a2, move == 1 );


  
  GridCurve::Edge previous_edge;
  do 
    {
      bool previous_cvx = cvx;
      uint l = A.nextEdge( nb_a1, nb_a2, w, j, cvx );
      uint new_a1 = A.letter(1) - '0';
      uint new_a2 = A.letter(2) - '0';
      uint a3 = (a1 + 2) % 4;
      uint a0 = (a2 + 2) % 4;
      if ( ( new_a2 == a1 ) &&  (new_a1  == a0 ) ) 
        {  
          // Quadrant change, must be +
          if ( ( nb_a1  + nb_a2 ) > 0 ) 
          {
            #ifdef TRACE_DMLP
            cerr << "[NEW EDGE] " << e << endl;
            #endif
            e.initEdge( nb_a1, nb_a2, false );
            insertEdge( endEdge(), e );
            previous_edge = e;
            e.initContext( previous_edge );
          }
          e.initQuadrantChange( 1 );
        } 
      else if ((new_a1 == a1) && (new_a2 == a2 ) ) 
        {
          // Normal edge, no convexity change
          e.initEdge( nb_a2, nb_a1, false );
        }
      else if ((new_a1 == a2) && (new_a2 == a1 ) ) 
        {
          // Normal edge, no convexity change
          e.initEdge( nb_a2, nb_a1, true );
        }
      else if (( new_a1 == a3) && (new_a2 == a2) ) 
        {
          if ( ( nb_a1  + nb_a2 ) > 0 ) 
            {
              e.initEdge( nb_a1, nb_a2, true );
              insertEdge( endEdge(), e );
              #ifdef TRACE_DMLP
              cerr << "[NEW EDGE] " << e << endl;
              #endif
              previous_edge = e;
              e.initContext( previous_edge );
            }
          e.initQuadrantChange( 1 );
        }
      else 
        {
          ASSERT_GridCurve( false );
        }
      insertEdge(endEdge(), e);
      #ifdef TRACE_DMLP
      cerr << "[NEW EDGE] " << e << endl;
      #endif
      a1 = new_a1;
      a2 = new_a2;
      previous_edge = e;
      e.initContext( previous_edge );
    }
  while ( j != end );
}




/**
   Creates a RPR from an CurveCode representing an interpixel contour.
   
   @param cc any interpixel curve code (no ++ and no --).
   
   @param inner when 'true' creates the RPR on the inner side,
   otherwise creates the RPR on the outside.
*/
bool
ImaGene::ReversiblePolygonalRepresentation
::initFromInterpixelCurveCode( CurveCode & cc,
			       bool inner )
{
  vx.clear();
  vy.clear();
  vt.clear();
  Vector2i v;
  unsigned int nb_shifts = cc.shiftForInnerOrOuterCurve();
  #ifdef TRACE_DMLP
    cerr << "[initFromInterpixelCurveCode] nb_shifts=" << nb_shifts << endl;
  #endif
  FreemanChain::displacement( v.x(), v.y(), cc.startStep() );
  cw = cc.isCW();
  CurveCode tcc = cc.computeInnerOrOuterCurve( inner, twice_dv );
  //std::cerr << tcc << std::endl;
  Vector2i x = cc.startPoint();
  for ( CurveCode::ConstIterator 
	  it = tcc.begin(),
	  it_end = tcc.end(); it != it_end; ++it )
    {
      unsigned char code = *it;
      switch ( code )
	{
	case 0: // = go forward
	  vx.push_back( x.x() );
	  vy.push_back( x.y() );
	  vt.push_back( inner );
	  x += v;
	  break;
	case 1: // + convex
	  if ( cw ) // vector must turn right
	    v.turnLeft(); // reference frame is turned to the left.
	  else // vector must turn left
	    v.turnRight(); // reference frame is turned to the right.
	  break;
	case 3: // - concave
	  if ( ! cw ) // vector must turn right
	    v.turnLeft(); // reference frame is turned to the left.
	  else // vector must turn left
	    v.turnRight(); // reference frame is turned to the right.
	  break;
	case 2: 
	  ASSERT_GridCurve( false 
			    && "[ReversiblePolygonalRepresentation::initFromInterpixelCurveCode] forbidden." );
	  return false;
	}
    }
  vx.push_back( x.x() );
  vy.push_back( x.y() );
  vt.push_back( inner );
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
