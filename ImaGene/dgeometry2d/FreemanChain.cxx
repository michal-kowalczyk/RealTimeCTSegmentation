///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : FreemanChain.cxx
//
// Creation : 2008/05/20
//
// Version : 2008/05/20
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
//	2008/05/20 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include "ImaGene/base/Proxy.h"
#include "ImaGene/base/OrderedAlphabet.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/FreemanChain.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const FreemanChain_RCS_ID = "@(#)class FreemanChain definition.";



///////////////////////////////////////////////////////////////////////////////
// class FreemanChain
///////////////////////////////////////////////////////////////////////////////


/**
 * Outputs the chain [c] to the stream [out].
 * @param out any output stream,
 * @param c a Freeman chain.
 */
void
ImaGene::FreemanChain::write( std::ostream & out, const FreemanChain & c )
{
  out << c.x0 << " " << c.y0 << " " << c.chain << endl;
}


/**
 * Reads a chain from the stream [in] and updates [c].
 * @param in any input stream,
 * @param c (returns) the Freeman chain.
 */
void 
ImaGene::FreemanChain::read( std::istream & in, FreemanChain & c )
{
  string str;
  while ( true )
    {
      getline( in, str );
      if ( ! in.good() ) return;
      if ( ( str.size() > 0 ) && ( str[ 0 ] != '#' ) )
	{
	  istringstream str_in( str );
	  str_in >> c.x0 >> c.y0 >> c.chain;
	  return;
	}
    }
}

/**
 * Creates a Freeman chaincode [chain] and a chain coding the
 * quadrant of each step [qchain], given an iterator on a
 * 4-connected path [it], a number of step [nb], the first step
 * [freeman_code] and the first quadrant [quadrant].
 *
 * @param chain (returns) a string of '0', '1', '2', or '3'
 * (Freeman chaincode)
 *
 * @param qchain (returns) a string of '0', '1', '2', or '3'
 * giving the quadrants.
 *
 * @param it any iterator 
 *
 * @param nb the number of 'next' performed on a copy of [it].
 *
 * @param freeman the first code or step.
 *
 * @param quadrant the first quadrant (equal to freeman_code or one below).
 *
 * @param start_index the starting index in [chain] and [qchain], default is 0.
 */
void
ImaGene::FreemanChain::create( std::string & chain,
			       std::string & qchain,
			       const C4CIterator & it, 
			       uint nb, uint freeman, uint quadrant,
			       uint start_index )
{
  // Computes Freeman chain code and quadrants.
  Mathutils::ModuloComputer mf( 4 );
  Mathutils::ModuloComputer msu( nb );
  chain.resize( nb );
  qchain.resize( nb );
  Proxy<C4CIterator> it2( it.clone() );

  for ( uint i = 0; i != nb; ++i )
    {
      chain[ start_index ] = '0' + freeman;
      qchain[ start_index ] = '0' + quadrant;
      uint code = it2->next();
      switch ( code )
	{
	case 0: cerr << "[ImaGene::FreemanChain::create] "
		     << " Contour is too short." << endl; 
	  break;
	case 1: mf.increment( freeman );
	  break;
	case 2: break;
	case 3: mf.decrement( freeman );
	  break;
	}
      if ( ( quadrant != freeman ) 
	   && ( mf.next( quadrant ) != freeman ) )
	{ // Change of quadrant.
	  if ( mf.previous( quadrant ) == freeman )
	    mf.decrement( quadrant );
	  else if ( mf.next( quadrant ) == mf.previous( freeman ) )
	    mf.increment( quadrant );
	}
      msu.increment( start_index );
    }
}


/**
 * @param zero (returns) the '0' or 'x' letter for quadrant [quadrant].
 * @param one (returns) the '1' or 'y' letter for quadrant [quadrant].
 * @param quadrant the quadrant as any of '0', '1', '2', or '3'.
 */
void
ImaGene::FreemanChain::alphabet( char & zero, char & one, 
				 char quadrant )
{
  switch ( quadrant )
    {
    case '0': zero = '0'; one = '1'; break;
    case '1': zero = '1'; one = '2'; break;
    case '2': zero = '2'; one = '3'; break;
    case '3': zero = '3'; one = '0'; break;
    }
}


/**
 * Given two consecutive moves on a Freeman chain code, this
 * method returns the type of movement: 0: return move, 1: turning
 * toward the interior, 2: going straight, 3: turning toward
 * exterior. Interior/exterior is specified by [ccw].
 *
 * @param code1 the code of the first step as an integer in 0..3.
 * @param code2 the code of the second step as an integer in 0..3.
 * @param ccw 'true' if the contour is seen counterclockwise with
 * its inside to the left.
 */
uint
ImaGene::FreemanChain::movement( uint code1, uint code2, bool ccw )
{
  uint cfg = ( ccw ? 0 : 16 ) + ( code1 << 2 ) + code2;
  static const uint tbl[ 32 ] = { 
    2, 1, 0, 3, 3, 2, 1, 0,
    0, 3, 2, 1, 1, 0, 3, 2, 
    2, 3, 0, 1, 1, 2, 3, 0,
    0, 1, 2, 3, 3, 0, 1, 2 };
  return tbl[ cfg ];
}


/**
 * From the Freeman chain [pl_chain] representing a pointel
 * 4-connected contour, constructs the Freeman chain [pix_chain]
 * that represents its inner 4-connected border of pixels. The
 * Freeman chain [pl_chain] has its inside to the left (ie. ccw).
 * 
 * Note that chain codes going back and forth are @b never considered
 * useless: it means that the chain is always supposed to have its
 * interior to the left (ccw) or right (cw) even at configurations
 * "02", "13", "20", "31".
 * 
 * @param pix_chain (output) the code of the 4-connected inner border. 
 *
 * @param pl2pix (output) the mapping associating pointels to
 * pixels as indices in their respective Freeman chain.
 *
 * @param pix2pl (output) the inverse mapping associating pixels to
 * pointels as indices in their respective Freeman chain.
 *
 * @param pl_chain the input code of the 4-connected pointel contour.
 */
void
ImaGene::FreemanChain::pointel2pixel( FreemanChain & pix_chain,
				      vector<uint> & pl2pix,
				      vector<uint> & pix2pl,
				      const FreemanChain & pl_chain )
{
  innerContour( pix_chain, pl2pix, pix2pl, pl_chain, true );
}

/**
 * From the Freeman chain [outer_chain] representing a 4-connected
 * contour, constructs the Freeman chain [inner_chain] that
 * represents its inner 4-connected contour (which lies in its
 * interpixel space). The boolean [ccw] specifies if the inside is
 * to the left (ccw) or to the right (cw).
 *
 * Note that chain codes going back and forth are @b never considered
 * useless: it means that the chain is always supposed to have its
 * interior to the left (ccw) or right (cw) even at configurations
 * "02", "13", "20", "31".
 * 
 * @param inner_chain (output) the code of the 4-connected inner
 * border, with starting coordinates that are floored to the closest
 * integer.
 *
 * @param outer2inner (output) the mapping associating outer to
 * inner elements as indices in their respective Freeman chain.
 *
 * @param inner2outer (output) the mapping associating inner to
 * outer elements as indices in their respective Freeman chain.
 *
 * @param outer_chain the input code of the 4-connected contour.
 *
 * @param ccw 'true' if the contour is seen counterclockwise with its
 * inside to the left.
 */
void 
ImaGene::FreemanChain::innerContour
( FreemanChain & inner_chain,
  vector<uint> & outer2inner,
  vector<uint> & inner2outer,
  const FreemanChain & outer_chain,
  bool ccw )
{
  uint nb = outer_chain.chain.size();
  uint j = 0;
  outer2inner.clear();
  outer2inner.reserve( nb );
  // inner_chain.chain.reserve( nb + 4 );
  inner_chain.chain = "";
  inner2outer.clear();
  inner2outer.reserve( nb + ( ccw ? 4 : -4 ) );
  int dx0, dy0;
  int dx1, dy1;
  FreemanChain::displacement( dx0, dy0, outer_chain.code( 0 ) );
  int turn = ccw ? 1 : 3;
  FreemanChain::displacement( dx1, dy1, ( outer_chain.code( 0 ) + turn ) % 4 );
  dx0 += dx1;
  dy0 += dy1;
  inner_chain.x0 = dx0 > 0 ? outer_chain.x0 : outer_chain.x0 - 1;
  inner_chain.y0 = dy0 > 0 ? outer_chain.y0 : outer_chain.y0 - 1;

  FreemanChain::const_iterator it_begin = outer_chain.begin();
  FreemanChain::const_iterator it = it_begin;
  it.next();
  for ( uint i = 0; i < nb; ++i )
    {
      // Check if contour is open.
      // cerr << "i=" << i << " code=" << outer_chain.code( i ) << endl;
      switch ( movement( outer_chain.code( i ), 
			 outer_chain.code( ( i + 1 ) % nb ),
			 ccw ) ) 
	{
	case 0:
	// contour going in then out.
	  inner_chain.chain += outer_chain.chain[ i ];
	  inner_chain.chain += ( ( ( (uint) ( outer_chain.chain[ i ] - '0' ) 
				     + ( ccw ? 3 : 1 ) ) )
				 % 4 ) + '0';
	  inner_chain.chain += outer_chain.chain[ ( i + 1 ) % nb ];
	  outer2inner.push_back( j );
	  inner2outer.push_back( i );
	  inner2outer.push_back( i + 1 );
	  inner2outer.push_back( i + 1 );
	  j += 3;
	  break;

	case 1:
	  // contour turning toward its inside.
	  outer2inner.push_back( j );
	  break;

	case 2:
	  // contour going straight ahead
	  inner_chain.chain += outer_chain.chain[ i ];
	  outer2inner.push_back( j );
	  inner2outer.push_back( i );
	  ++j;
	  break;

	case 3:
	  // contour turning toward its outside.
	  inner_chain.chain += outer_chain.chain[ i ];
	  inner_chain.chain += outer_chain.chain[ ( i + 1 ) % nb ];
	  outer2inner.push_back( j );
	  inner2outer.push_back( i );
	  inner2outer.push_back( i + 1 );
	  j += 2;
	  break;
	}	
      

      // Advances along contour and check if it is a closed contour.
      it.next();
      if ( ( i == nb - 1 )
	   && ( *it_begin != *it ) ) 
	// freeman chain is *not* a closed loop.
	{
	  inner_chain.chain += outer_chain.chain[ i ];
	  outer2inner.push_back( j );
	  inner2outer.push_back( i );
	  ++i; ++j;
	  break;
	}
    }
}
    

/**
 * Reads the 4-connected contour [c] so that meaningless back and
 * forth steps are removed. These operations may create one or
 * several 4-connected contours (stored in [clean_cs]), whether
 * these removals cuts the contour in several loops. Because of
 * that, the mappings are more complex.

 * @param clean_cs (output) the array of cleaned 4-connected contours.
 *
 * @param c2clean (output) the mapping associating an element to
 * its clean element as a pair (n,i) where n is the index of the
 * cleaned contour and i the indice of the element in this Freeman
 * chain.
 *
 * @param clean2c (output) the array of mapping associating a
 * clean element to its non-clean element. clean2c[n][j] gives the
 * index of the non-clean element on c corresponding to the clean
 * element of index j in the n-th contour.
 *
 * @param c the input code of the 4-connected contour.
 *
 * @param ccw 'true' if the contour is seen counterclockwise with
 * its inside to the left.
 */
void
ImaGene::FreemanChain::cleanContour( vector<FreemanChain> & clean_cs,
				     vector< pair<uint,uint> > & c2clean,
				     vector< vector<uint> > & clean2c,
				     const FreemanChain & c,
				     bool ccw )
{
  cerr << "NOT IMPLEMENTED." << endl;
}


/**
 * Removes outer spikes along a 4-connected contour, meaning steps
 * "02", "13", "20" or "31", which point outside the shape. The
 * inside is given by parameter [ccw]. Note that 4-connected
 * pointel contours should not have any outer spikes, while
 * 4-connected pixel contours should not have any inner spikes.
 *
 * @param clean_c (output) the cleaned 4-connected contour.
 *
 * @param c2clean (output) the mapping associating an element to
 * its clean element.
 *
 * @param clean2c (output) the inverse mapping associating a
 * clean element to its non-clean element. 
 *
 * @param c the input code of the 4-connected contour.
 *
 * @param ccw 'true' if the contour is seen counterclockwise with
 * its inside to the left.
 *
 * @return 'true' if the contour add an interior, 'false' otherwise.
 */
bool
ImaGene::FreemanChain::cleanOuterSpikes( FreemanChain & clean_c,
					 std::vector<uint> & c2clean,
					 std::vector<uint> & clean2c,
					 const FreemanChain & c,
					 bool ccw )
{
  uint nb = c.chain.size();
  if ( nb == 0 ) 
    {
      cerr << "[ImaGene::FreemanChain::cleanOuterSpikes]"
	   << " cleanOuterSpikes: Empty input chain"
	   << endl;
      return false;
    }
  Mathutils::ModuloComputer mc( nb );
  uint i = 0;
  uint j = 0;
  vector<uint> c2cleanTMP;
  clean_c.chain.reserve( nb );
  clean_c.chain = "";
  c2clean.clear();
  clean2c.clear();
  c2clean.reserve( nb );
  clean2c.reserve( nb );
  c2cleanTMP.reserve( nb );
  FreemanChain::const_iterator it = c.begin();
  FreemanChain::const_iterator itn = c.begin(); itn.nextInLoop();
  // Find a consistent starting point.
  uint n;
  uint size_spike = 0;
  for ( n = 0; n < nb; ++n )
    {
      size_spike = 0;
      while ( movement( it.getCode(), itn.getCode(), ccw ) == 0 )
	{
	  it.previousInLoop(); 
	  itn.nextInLoop(); 
	  mc.increment( i );
	  size_spike += 2;
	  if ( size_spike >= nb )
	    {
	      cerr << "[ImaGene::FreemanChain::cleanOuterSpikes]"
		   << " Spike is longer than contour !" 
		   << " size_spike=" << size_spike
		   << " nb=" << nb
		   << endl;
	      return false;
	    }
	}
      mc.increment( i );
      it = itn;
      itn.nextInLoop();
      if ( size_spike > 0 ) break;
    }
  uint start_idx = it.getPosition();
  i = start_idx;
  // JOL : 2009/07/7, added starting coordinates
  Vector2i P = *it;
  clean_c.x0 = P.x();
  clean_c.y0 = P.y();

  // cerr << "Starting point is " << i << endl;
  // ASSERT_FreemanChain( ( n < nb ) || ( i == 0 ) );
  if ( ( n == nb ) )
    { // do nothing
      clean_c.chain = c.chain;
      for ( uint n = 0; n < nb; ++n )
	{
	  c2clean.push_back( n );
	  clean2c.push_back( n );
	}
      if ( size_spike != 0 )
	cerr << "[ImaGene::FreemanChain::cleanOuterSpikes]"
	     << "No starting point found (only spikes !)" << endl;
      
      return size_spike == 0;
    }
  // Loops over all letters.
  FreemanChain::const_iterator it_begin = it;
  deque<uint> clean_code;
  deque<uint> clean_idx;
  vector<uint> begin_outer_spike;
  vector<uint> end_outer_spike;
  // i follows iterator it.
  do
    {
      clean_code.push_back( it.getCode() );
      clean_idx.push_back( i );
      itn = it;
      it.nextInLoop();
      mc.increment( i );
      // cerr << "- i=" << i << " (" << clean_code.back() 
      // 	   << it.getCode() << ") ";
      uint size_spike = 0;
      uint last_spike_idx = end_outer_spike.empty() ?
	start_idx :
	end_outer_spike.back();
      j = i;
      while ( ( ! clean_code.empty() )
	      && ( j != last_spike_idx ) 
	      && ( movement( clean_code.back(), it.getCode(), ccw ) == 0 )
	      && ( it != it_begin ) )
	{
	  clean_code.pop_back(); 
	  clean_idx.pop_back();
	  mc.increment( i );
	  mc.decrement( j );
	  it.nextInLoop();
	  itn.previousInLoop();
	  size_spike += 2;
	}
      // cerr << "i=" << i << " size_spike=" << size_spike
      // 	   << " last_spike_idx=" << last_spike_idx
      // 	   << endl;
      if ( size_spike != 0 )
	{
	  // There is a spike. Is it an outer one ?  
	  uint previous_code = itn.getCode();
	  uint previous_idx = itn.getPosition();
	  // JOL : do not
	  // consider any more "cleaned contour" since we cannot go
	  // further than the last spike.
	  // uint previous_code = 
	  //   clean_code.empty() ? itn.getCode() : clean_code.back();
	  // uint previous_idx = 
	  //   clean_code.empty() ? itn.getPosition() : clean_idx.back();
	  itn = it; 
	  itn.previousInLoop();
	  uint move1 = movement( previous_code,
				 ( itn.getCode() + 2 ) % 4, ccw );
	  uint move2 = movement( itn.getCode(), it.getCode() , ccw );
	  bool return_spike = ( move1 == 0 ) || ( move2 == 0 );
	  bool outer_spike = ( move1 == 3 ) || ( move2 == 3 );
// 	  if ( return_spike )
// 	    cerr << "[ImaGene::FreemanChain::cleanOuterSpikes] return spike."
// 		 << endl;
// 	  if ( ! ( ( outer_spike && ( move1 != 1 ) && ( move2 != 1 ) )
// 		   || ( ! outer_spike && ( move1 != 3 ) && ( move2 != 3 ) ) ) )
// 	    cerr << "[ImaGene::FreemanChain::cleanOuterSpikes] "
// 		 << "Weird spike. Invalid contour (expected 3 3) ?" 
// 		 << " move1=" << move1
// 		 << " move2=" << move2
// 		 << " ccw=" << ccw
// 		 << " start_idx=" << start_idx
// 		 << " size_spike=" << size_spike
// 		 << " it=" << it.getPosition()
// 		 << " itp=" << previous_idx
// 		 << endl
// 		 << c.chain << endl;
	  // Process waiting steps.
	  if ( outer_spike || return_spike ) 
	    {
	      begin_outer_spike.push_back( mc.next( previous_idx ) );
	      end_outer_spike.push_back( i );
	      // cout << " outer spike [" << begin_outer_spike.back()
	      // 	   << "," << end_outer_spike.back() << "[  " << endl;
	    }
	}
    }
  while ( it != it_begin );

  // Once outer spikes are known, we can create the new contour.
  c2clean.resize( nb );
  i = start_idx % nb;
  j = 0;
  uint nb_spikes = begin_outer_spike.size();
  uint k = 0;
  n = 0;
  while ( n < nb )
    {
      if ( ( k == nb_spikes ) || ( i != begin_outer_spike[ k ] ) )
	{
	  clean_c.chain.push_back( c.chain[ i ] );
	  c2clean[ i ] = j;
	  clean2c.push_back( i );
	  mc.increment( i );
	  ++j;
	  ++n;
	}
      else 
	{
	  while ( i != end_outer_spike[ k ] )
	    {
	      c2clean[ i ] = j;
	      mc.increment( i );
	      ++n;
	    }
	  ++k;
	}
    }
  for ( uint i = 0; i < nb; ++i )
    if ( c2clean[ i ] >= clean_c.chain.size() )
      if ( c2clean[ i ] == clean_c.chain.size() )
	c2clean[ i ] = 0;
      else 
	{
	  cerr << "[ImaGene::FreemanChain::cleanOuterSpikes]"
	       << "Bad correspondence for c2clean[" << i<< "]"
	       << " = " << c2clean[ i ] << " >= " << clean_c.chain.size()
	       << endl;
	  c2clean[ i ] = c2clean[ i ] % clean_c.chain.size();
	}

  for ( uint j = 0; j < clean_c.chain.size(); ++j )
    if ( clean2c[ j ] >= nb )
      {
	cerr << "[ImaGene::FreemanChain::cleanOuterSpikes]"
	     << "Bad correspondence for clean2c[" << j << "]"
	     << " = " << clean2c[ j ] << " >= " << nb
	     << endl;
	clean2c[ j ] = clean2c[ j ] % nb;
      }

  // for( int i= 0; i < c2cleanTMP.size(); i++ ) {
  //   c2clean.push_back
  //     ( c2cleanTMP.at( ( i + (c2cleanTMP.size()-start_idx) ) 
  // 		       % c2cleanTMP.size() ) );
  // }

  // i = start_idx;
  // j = 0;
  // uint nb_spikes = begin_outer_spike.size();
  // uint k = 0;
  // n = 0;
  // while ( n < nb )
  //   {
  //     if ( ( k == nb_spikes ) || ( i != begin_outer_spike[ k ] ) )
  // 	{
  // 	  clean_c.chain.push_back( c.chain[ i ] );
  // 	  c2cleanTMP.push_back( j );
  // 	  clean2c.push_back( i );
  // 	  mc.increment( i );
  // 	  ++j;
  // 	  ++n;
  // 	}
  //     else 
  // 	{
  // 	  while ( i != end_outer_spike[ k ] )
  // 	    {
  // 	      c2cleanTMP.push_back( j );
  // 	      mc.increment( i );
  // 	      ++n;
  // 	    }
  // 	  ++k;
  // 	}
  //   }

  // for( int i= 0; i < c2cleanTMP.size(); i++ ) {
  //   c2clean.push_back
  //     ( c2cleanTMP.at( ( i + (c2cleanTMP.size()-start_idx) ) 
  // 		       % c2cleanTMP.size() ) );
  // }

  // while ( true )
  //   {
  //     cout << "- i=" << i;
  //     uint size_spike = 0;
  //     while ( movement( it.getCode(), itn.getCode(), ccw ) == 0 )
  // 	{
  // 	  it.previousInLoop(); 
  // 	  itn.nextInLoop(); 
  // 	  mc.decrement( i );
  // 	  size_spike += 2;
  // 	}
  //     cout << " with spike of size " << size_spike << endl;
  //     if ( size_spike == 0 )
  // 	{
  // 	  fc_to_process.push_back( c.chain[ i ] );
  // 	  idx_to_process.push_back( i );
  // 	  it.nextInLoop();
  // 	  itn.nextInLoop();
  // 	  mc.increment( i );
  // 	}
  //     else
  // 	{
  // 	  // There is a spike. Is it an outer one ?
  // 	  uint code11 = it.getCode();
  // 	  it.nextInLoop();
  // 	  uint code12 = it.getCode();
  // 	  mc.increment( i );
  // 	  uint move1 = movement( code11, code12, ccw );
  // 	  uint code22 = itn.getCode();
  // 	  itn.previousInLoop();
  // 	  uint code21 = itn.getCode();
  // 	  uint move2 = movement( code21, code22, ccw );
  // 	  bool outer_spike = ( move1 == 3 ) || ( move2 == 3 );
  // 	  ASSERT_FreemanChain
  // 	    ( ( ( outer_spike && ( move1 != 1 ) && ( move2 != 1 ) )
  // 		|| ( ! outer_spike && ( move1 != 3 ) && ( move2 != 3 ) ) )
  // 	      && "[ImaGene::FreemanChain::cleanOuterSpikes] Weird spike. Invalid contour ?" );
  // 	  // Process waiting steps.
  // 	  while ( ! fc_to_process.empty() )
  // 	    {
  // 	      clean_c.chain += fc_to_process.front();
  // 	      fc_to_process.pop_front();
  // 	      c2clean.push_back( j ) ; 
  // 	      clean2c.push_back( idx_to_process.front() );
  // 	      // c2clean[ idx_to_process.front() ] = j;
  // 	      // clean2c[ j ] = idx_to_process.front();
  // 	      idx_to_process.pop_front();
  // 	      ++j;
  // 	    }
  // 	  if ( outer_spike ) 
  // 	    {
  // 	      // Outer spike. Remove it.
  // 	      for ( uint k = 0; k < size_spike; ++k )
  // 		{
  // 		  // c2clean[ i ] = j;
  // 		  c2clean.push_back( j ) ; 
  // 		  mc.increment( i );
  // 		  it.nextInLoop();
  // 		}
  // 	    }
  // 	  else
  // 	    {
  // 	      // Inner spike. Keep it.
  // 	      for ( uint k = 0; k < size_spike; ++k )
  // 		{
  // 		  c2clean.push_back( j ) ; 
  // 		  clean2c.push_back( i );
  // 		  // c2clean[ i ] = j;
  // 		  // clean2c[ j ] = i;
  // 		  clean_c.chain += c.chain[ i ];
  // 		  ++j;
  // 		  mc.increment( i );
  // 		  it.nextInLoop();
  // 		}
  // 	    }
  // 	  itn = it;
  // 	  itn.nextInLoop();
  // 	}
  //     if ( it == it_begin ) break;
  //   } //   while ( true )
  // // Process waiting steps.
  // while ( ! fc_to_process.empty() )
  //   {
  //     cout << fc_to_process.front() << " at " << idx_to_process.front() << endl;
  //     clean_c.chain += fc_to_process.front();
  //     fc_to_process.pop_front();
  //     c2clean.push_back( j ) ; 
  //     clean2c.push_back( idx_to_process.front() );
  //     // c2clean[ idx_to_process.front() ] = j;
  //     // clean2c[ j ] = idx_to_process.front();
  //     idx_to_process.pop_front();
  //     ++j;
  //   }


  return true;
}

/**
 * Given a Freeman chain [c] coding a 4-connected pixel loop, computes
 * its subsampling by the transformation:
 * X = ( x - x0 ) div h, 
 * Y = ( y - y0 ) div v.
 *
 * @param subc (output) the subsampled Freeman chain code (may contain spikes)
 * 
 * @param c2subc (output) the mapping associating an element to
 * its subsampled element.
 *
 * @param subc2c (output) the inverse mapping associating a
 * subsampled element to its element.
 *
 * @param c the input chain code.
 *
 * @param h the subsampling along x
 * @param v the subsampling along y
 * @param x0 the x-origin of the frame (X,Y) in (x,y)
 * @param y0 the y-origin of the frame (X,Y) in (x,y)
 *
 * @return 'false' if initial contour was empty or if [subc] is empty,
 * 'true' otherwise.
 */
bool
ImaGene::FreemanChain::subsample( FreemanChain & subc,
				  std::vector<uint> & c2subc,
				  std::vector<uint> & subc2c,
				  const FreemanChain & c,
				  uint h, uint v,
				  int x0, int y0 )
{
  if ( ( h == 0 ) || ( v == 0 ) ) return false;
  FreemanChain::const_iterator it = c.begin();
  uint j = 0;
  uint nb = c.chain.size();
  if ( nb == 0 ) return false;

  Vector2i fxy( it.get() );
  Vector2i fXY( ( fxy.x() - x0 ) / (int)h, ( fxy.y() - y0 ) / (int)v );
  subc.x0 = fXY.x();
  subc.y0 = fXY.y();
  c2subc.clear();
  c2subc.reserve( nb );
  subc2c.clear();
  subc2c.reserve( nb );

  for ( uint i = 0; i < nb; ++i )
    {
      c2subc.push_back( j );
      it.nextInLoop();
      Vector2i nxy( it.get() );
      Vector2i nXY( ( nxy.x() - x0 ) / (int)h, ( nxy.y() - y0 ) / (int)v );
      if ( nXY != fXY )
	{
	  subc2c.push_back( i );
	  char code;
	  if ( nXY.x() > fXY.x() )       code = '0';
	  else if ( nXY.x() < fXY.x() )  code = '2';
	  else if ( nXY.y() > fXY.y() )  code = '1';
	  else                           code = '3';
	  subc.chain += code;
	  ++j;
	  fXY = nXY;
	}
    }
//   c2subc.push_back( j );
//   it.nextInLoop();
//   for ( uint i = 1; i <= nb; ++i )
//     {
//       // JOL 
//       //c2subc.push_back( j );
//       Vector2i nxy( it.get() );
//       Vector2i nXY( ( nxy.x() - x0 ) / h, ( nxy.y() - y0 ) / v );
//       if ( nXY != fXY )
// 	{
// 	  char code;
// 	  if ( nXY.x() > fXY.x() )       code = '0';
// 	  else if ( nXY.x() < fXY.x() )  code = '2';
// 	  else if ( nXY.y() > fXY.y() )  code = '1';
// 	  else                           code = '3';
// 	  subc.chain += code;
// 	  subc2c.push_back( i - 1 );
// 	  ++j;
// 	  fXY = nXY;
// 	}
//       if ( i != nb ) c2subc.push_back( j );
//       it.nextInLoop();
//     }
//   // TODO : enhance this.
  uint nbsub =  subc.chain.size();
  // Last correspondence may be in fact to 0 instead of nbsub.
  if ( nbsub != 0 )
    for ( uint i = 0; i < nb; ++i )
      if ( c2subc[ i ] >= nbsub ) c2subc[ i ] -= nbsub;

  ASSERT_FreemanChain( c2subc.size() == nb );
  return nbsub != 0;
}


/**
 * Computes the Minimum Length Polygon (MLP) of the Freeman chain [fc].
 *
 * @param vx (returns) the x-coordinates of the MLP vertices.
 * @param vy (returns) the y-coordinates of the MLP vertices.
 * @param vi (returns) the indices of the MLP vertices in [fc].
 * @param fc the input Freeman chain.
 *
 * @return the length of the MLP, -1 if MLP has not correct loop (error ?).
 */
double
ImaGene::FreemanChain::computeMLP( std::vector<int> & vx,
				   std::vector<int> & vy,
				   std::vector<uint> & vi,
				   const FreemanChain & fc )
{
  double length=0;
  bool cvx = true;
  OrderedAlphabet A( '0', 4 );
  FreemanChain::const_iterator it = fc.findQuadrantChange4( A );
  // char start_char = fc.chain[ it.getPosition() ];

  uint j = it.getPosition();
  uint end = j;
  // cerr << "end=" << end << endl; 
  uint nb_a1;
  uint nb_a2;
  string w = fc.chain;
  // cerr << w << endl;
  Vector2i xy = *it;
  do 
    {
      vx.push_back( xy.x() );
      vy.push_back( xy.y() );  
      vi.push_back( j );
//       cerr << "- (" << xy.x() << "," << xy.y() << ") at " << j << endl;
      uint l = A.nextEdge( nb_a1, nb_a2, w, j, cvx );
      if ( l != 0 )
	{
	  double dx = (double) nb_a1;
	  double dy = (double) nb_a2;
	  length += sqrt( dx*dx + dy*dy );
	}

//       cerr << "  nb_a1=" << nb_a1 << " nb_a2=" << nb_a2 << endl;
      int dx;
      int dy;
      displacement( dx, dy, A.letter( 1 ) - '0' );
      if ( dx > 0 )      xy.x() += nb_a1;
      else if ( dx < 0 ) xy.x() -= nb_a1;
      else if ( dy > 0 ) xy.y() += nb_a1;
      else if ( dy < 0 ) xy.y() -= nb_a1;
      displacement( dx, dy, A.letter( 2 ) - '0' );
      if ( dx > 0 )      xy.x() += nb_a2;
      else if ( dx < 0 ) xy.x() -= nb_a2;
      else if ( dy > 0 ) xy.y() += nb_a2;
      else if ( dy < 0 ) xy.y() -= nb_a2;
    }
  while ( j != end );
  // cerr << w << endl;
  bool loop_ok = ( ( xy.x() == vx[ 0 ] ) && ( xy.y() == vy[ 0 ] ) );
//   if ( loop_ok )
//     cerr << "Loop OK" << endl;
//   else 
//     cerr << "Loop ERROR" << endl;

  if ( !loop_ok ) length = -1;
  return length;
}

/**
 * Computes the Minimum Length Polygon (MLP) of the Freeman chain
 * [fc]. Tells also if vertices are inside or outside.
 *
 * @param vx (returns) the x-coordinates of the MLP vertices.
 * @param vy (returns) the y-coordinates of the MLP vertices.
 * @param vi (returns) the indices of the MLP vertices in [fc].
 *
 * @param vt (returns) the type (inside=true, outside=false) of the
 * MLP vertices in [fc].
 *
 * @param fc the input Freeman chain.
 *
 * @param cw when 'true', the inside is considered to be to the right
 * of the contour (the contour turns clockwise around the inside),
 * when 'false' the inside is considered to be to the left of the
 * contour (the contour turns counterclockwise around the inside).
 *
 * @return 'true' if the MLP has made a correct loop, 'false'
 * otherwise (error ?).
 */
bool
ImaGene::FreemanChain::computeMLP( std::vector<int> & vx,
				   std::vector<int> & vy,
				   std::vector<uint> & vi,
				   std::vector<bool> & vt,
				   const FreemanChain & fc,
				   bool cw )
{
  OrderedAlphabet A( '0', 4 );
  if ( cw ) A.reverseAround12();
  FreemanChain::const_iterator it = fc.findQuadrantChange4( A );
  FreemanChain::const_iterator itprev = it;
  itprev.previousInLoop();
  uint mvt = FreemanChain::movement( itprev.getCode(), it.getCode(), ! cw );
  ASSERT_FreemanChain( ( ( mvt == 1 ) || ( mvt == 3 ) )
		       && "[ImaGene::FreemanChain::computeMLP] Invalid start point." );
  bool cvx = mvt == 1;

  uint j = it.getPosition();
  uint end = j;
  // cerr << "end=" << end << endl; 
  uint nb_a1;
  uint nb_a2;
  string w = fc.chain;
  // cerr << w << endl;
  Vector2i xy = *it;
  do 
    {
      vx.push_back( xy.x() );
      vy.push_back( xy.y() );  
      vi.push_back( j );
      vt.push_back( cvx );
      // cerr << "(" << xy.x() << "," << xy.y() << ") at " << j;
      uint l = A.nextEdge( nb_a1, nb_a2, w, j, cvx );
      // cerr << " nb_a1=" << nb_a1 << " nb_a2=" << nb_a2 << endl;
      int dx;
      int dy;
      displacement( dx, dy, A.letter( 1 ) - '0' );
      if ( dx > 0 )      xy.x() += nb_a1;
      else if ( dx < 0 ) xy.x() -= nb_a1;
      else if ( dy > 0 ) xy.y() += nb_a1;
      else if ( dy < 0 ) xy.y() -= nb_a1;
      displacement( dx, dy, A.letter( 2 ) - '0' );
      if ( dx > 0 )      xy.x() += nb_a2;
      else if ( dx < 0 ) xy.x() -= nb_a2;
      else if ( dy > 0 ) xy.y() += nb_a2;
      else if ( dy < 0 ) xy.y() -= nb_a2;
    }
  while ( j != end );
  // cerr << w << endl;
  bool loop_ok = ( ( xy.x() == vx[ 0 ] ) && ( xy.y() == vy[ 0 ] ) );
  // if ( loop_ok )
  //   cerr << "Loop OK" << endl;
  // else 
  //   cerr << "Loop ERROR" << endl;
  return loop_ok;
}


/**
 * Computes the Minimum Length Polygon (MLP) of the Freeman chain
 * [fc]. Tells also if vertices are inside or outside. The MLP lies in
 * the half-integer plane compared with the Freeman Chain. Vertex
 * coordinates are given as integers. This function gives the
 * displacement vector to go from the digital contour to the MLP.
 *
 * @param vx (returns) the x-coordinates of the MLP vertices.
 * @param vy (returns) the y-coordinates of the MLP vertices.
 * @param vi (returns) the indices of the MLP vertices in [fc].
 *
 * @param vt (returns) the type (inside=true, outside=false) of the
 * MLP vertices in [fc].
 *
 * @param twice_dv (returns) twice the displacement vector to go from
 * the integer plane of the Freeman chain to the half-integer plane of
 * the MLP.
 *
 * @param fc the input Freeman chain.
 *
 * @param cw when 'true', the inside is considered to be to the right
 * of the contour (the contour turns clockwise around the inside),
 * when 'false' the inside is considered to be to the left of the
 * contour (the contour turns counterclockwise around the inside).
 *
 * @return the iterator on the starting step of the MLP or 'end()' if
 * there was an error (incorrect loop ?).
 */
ImaGene::FreemanChain::const_iterator
ImaGene::FreemanChain::computeMLP( std::vector<int> & vx,
				   std::vector<int> & vy,
				   std::vector<uint> & vi,
				   std::vector<bool> & vt,
				   Vector2i & twice_dv,
				   const FreemanChain & fc,
				   bool cw )
{
  OrderedAlphabet A( '0', 4 );
  if ( cw ) A.reverseAround12();
  FreemanChain::const_iterator it = fc.findQuadrantChange4( A );

  // Compute displacement vector.
  FreemanChain::const_iterator itprev = it;
  itprev.previousInLoop();
  Vector2i v_it( 0, 0 );
  Vector2i v_itprev( 0, 0 );
  v_it.move4( it.getCode() );
  v_itprev.move4( itprev.getCode() );
  twice_dv = v_it - v_itprev;

  uint mvt = FreemanChain::movement( itprev.getCode(), it.getCode(), ! cw );
  ASSERT_FreemanChain( ( ( mvt == 1 ) || ( mvt == 3 ) )
		       && "[ImaGene::FreemanChain::computeMLP] Invalid start point." );
  bool cvx = mvt == 1;
  

  uint j = it.getPosition();
  uint end = j;
  // cerr << "end=" << end << endl; 
  uint nb_a1;
  uint nb_a2;
  string w = fc.chain;
  // cerr << w << endl;
  Vector2i xy = *it;
  do 
    {
      vx.push_back( xy.x() );
      vy.push_back( xy.y() );  
      vi.push_back( j );
      vt.push_back( cvx );
      // cerr << "(" << xy.x() << "," << xy.y() << ") at " << j;
      uint l = A.nextEdge( nb_a1, nb_a2, w, j, cvx );
      // cerr << " nb_a1=" << nb_a1 << " nb_a2=" << nb_a2 << endl;
      int dx;
      int dy;
      displacement( dx, dy, A.letter( 1 ) - '0' );
      if ( dx > 0 )      xy.x() += nb_a1;
      else if ( dx < 0 ) xy.x() -= nb_a1;
      else if ( dy > 0 ) xy.y() += nb_a1;
      else if ( dy < 0 ) xy.y() -= nb_a1;
      displacement( dx, dy, A.letter( 2 ) - '0' );
      if ( dx > 0 )      xy.x() += nb_a2;
      else if ( dx < 0 ) xy.x() -= nb_a2;
      else if ( dy > 0 ) xy.y() += nb_a2;
      else if ( dy < 0 ) xy.y() -= nb_a2;
    }
  while ( j != end );
  // cerr << w << endl;
  bool loop_ok = ( ( xy.x() == vx[ 0 ] ) && ( xy.y() == vy[ 0 ] ) );
  // if ( loop_ok )
  //   cerr << "Loop OK" << endl;
  // else 
  //   cerr << "Loop ERROR" << endl;
  return loop_ok ? it : fc.end();
}


/**
 * Computes the Minimum Length Polygon (MLP) of the Freeman chain [fc]
 * and return its length.
 *
 * @param fc the input Freeman chain.
 *
 * @return the Euclidean length of the MLP.
 */
double
ImaGene::FreemanChain::lengthMLP( const FreemanChain & fc )
{
  OrderedAlphabet A( '0', 4 );
  FreemanChain::const_iterator it = fc.findQuadrantChange4( A );
  uint j = it.getPosition();
  uint end = j;
  uint nb_a1;
  uint nb_a2;
  string w = fc.chain;
  double length = 0.0;
  bool cvx = true;
  do 
  {
    uint l = A.nextEdge( nb_a1, nb_a2, w, j, cvx );
    if ( l != 0 )
    {
      double dx = (double) nb_a1;
      double dy = (double) nb_a2;
      length += sqrt( dx*dx + dy*dy );
    }
  }
  while ( j != end );
  return length;
}

/**
 * Comnputes du Combinatorial Minumum Length Polygon (CMLP)
 * of the Freeman chain [fc] and returns the norm 1 of all 
 * edges. This is used only to test the computation time.
 */
double
ImaGene::FreemanChain::testCMLP( const FreemanChain & fc )
{
  OrderedAlphabet A( '0', 4 );
  FreemanChain::const_iterator it = fc.findQuadrantChange4( A );
  uint j = it.getPosition();
  uint end = j;
  uint nb_a1;
  uint nb_a2;
  string w = fc.chain;
  double length = 0.0;
  bool cvx = true;
  do 
  {
    uint l = A.nextEdge( nb_a1, nb_a2, w, j, cvx );
    if ( l != 0 )
    {
      double dx = (double) nb_a1;
      double dy = (double) nb_a2;
      length += dx + dy;
    }
  }
  while ( j != end );
  return length;
}

/**
 * Finds a quadrant change in 'this' Freeman chain and returns the
 * position as an iterator. A quadrant change is some
 <code>
 abb..bc
  |
 iterator
 <endcode>
 *
 * The alphabet is possibly re-ordered so that a > b > c.
 *
 * @param A (possibly updated) a Freeman chain alphabet, possibly
 * re-ordered so that a > b > c.
 *
 * @return an iterator on 'this' that points on the first letter b.
 */
ImaGene::FreemanChain::const_iterator
ImaGene::FreemanChain::findQuadrantChange( OrderedAlphabet & A ) const
{
  FreemanChain::const_iterator it = begin();
  FreemanChain::const_iterator it_end = end();
  // find first letters a and b.
  uint code1 = it.getCode();
  it.next();
  while ( ( it != it_end ) && ( it.getCode() == code1 ) )
    it.next();
  ASSERT_FreemanChain( ( it != it_end ) 
		       && "[ImaGene::FreemanChain::findQuadrantChange( OrderedAlphabet & A ) const] 1-letter freeman chain." );
  uint code2 = it.getCode();
  // find third letter c.
  while ( ( it != it_end ) && ( ( it.getCode() == code1 )
				|| ( it.getCode() == code2 ) ) )
    it.next();
  ASSERT_FreemanChain( ( it != it_end ) 
		       && "[ImaGene::FreemanChain::findQuadrantChange( OrderedAlphabet & A ) const] 2-letters Freeman chain." );
  uint code3 = it.getCode();
  // reorder a and b.
  it.previous();
  if ( it.getCode() != code2 ) swap( code1, code2 );
  // find first a.
  do 
    { 
      it.previous();
    }
  while ( it.getCode() == code2 );
  char a_char = chain[ it.getPosition() ];
  // the next is the first b.
  it.next();
  char b_char = chain[ it.getPosition() ];
  // Reorder the alphabet to match the quadrant change.
  while ( A.order( b_char ) != 1 )
    A.shiftLeft();
  if ( A.order( a_char ) == 0 )
    {
      A.reverse();
      while ( A.order( b_char ) != 1 )
	A.shiftLeft();
    }
  ASSERT_FreemanChain( ( A.order( b_char ) == 1 )
		       && ( A.order( a_char ) == 2 )
		       && "[ImaGene::FreemanChain::findQuadrantChange( OrderedAlphabet & A ) const] Internal error: invalid Quadrant change found." );
  return it;
}


/**
 * Finds a quadrant change in 'this' Freeman chain and returns the
 * position as an iterator. A quadrant change is some
 <code>
 (abc)*bc...cd
        |
 iterator
 <endcode>
 *
 * This quadrant change also guarantees that is not a place where a
 * convexity change occurs in the combinatorial MLP algorithm.
 *
 * The alphabet is possibly re-ordered so that b > c > d > a.
 *
 * @param A (possibly updated) a Freeman chain alphabet, possibly
 * re-ordered so that b > c > d > a.
 *
 * @return an iterator on 'this' that points on the first letter c.
 */
ImaGene::FreemanChain::const_iterator
ImaGene::FreemanChain::findQuadrantChange4( OrderedAlphabet & A ) const
{
  FreemanChain::const_iterator it = begin();
  FreemanChain::const_iterator it_end = end();
  // find first letters a and b.
  uint code1 = it.getCode();
  it.next();
  while ( ( it != it_end ) && ( it.getCode() == code1 ) )
    it.next();
  ASSERT_FreemanChain( ( it != it_end ) 
		       && "[ImaGene::FreemanChain::findQuadrantChange( OrderedAlphabet & A ) const] 1-letter freeman chain." );
  uint code2 = it.getCode();
  // find third letter c.
  while ( ( it != it_end ) && ( ( it.getCode() == code1 )
				|| ( it.getCode() == code2 ) ) )
    it.next();
  ASSERT_FreemanChain( ( it != it_end ) 
		       && "[ImaGene::FreemanChain::findQuadrantChange( OrderedAlphabet & A ) const] 2-letters Freeman chain." );
  uint code3 = it.getCode();
  // find fourth letter d.
  while ( ( it != it_end ) && ( ( it.getCode() == code1 )
				|| ( it.getCode() == code2 )
				|| ( it.getCode() == code3 ) ) )
    it.next();
  ASSERT_FreemanChain( ( it != it_end ) 
		       && "[ImaGene::FreemanChain::findQuadrantChange( OrderedAlphabet & A ) const] 3-letters Freeman chain." );
  uint code4 = it.getCode();
  // define true c.
  it.previous();
  code3 = it.getCode();
  // find first b.
  do 
    { 
      it.previous();
    }
  while ( it.getCode() == code3 );
  char a_char = chain[ it.getPosition() ];
  // the next is the first c.
  it.next();
  char b_char = chain[ it.getPosition() ];
  // Reorder the alphabet to match the quadrant change.
  while ( A.order( b_char ) != 1 )
    A.shiftLeft();
  if ( A.order( a_char ) == 0 )
    {
      A.reverse();
      while ( A.order( b_char ) != 1 )
	A.shiftLeft();
    }
  ASSERT_FreemanChain( ( A.order( b_char ) == 1 )
		       && ( A.order( a_char ) == 2 )
		       && "[ImaGene::FreemanChain::findQuadrantChange( OrderedAlphabet & A ) const] Internal error: invalid Quadrant change found." );
  return it;
}


/**
 * This method takes O(n) operations and works only for Freeman
 * chains whose successive codes are between +1/-1. It determines
 * if the FreemanChain corresponds to a closed contour, and if
 * this is the case, determines how many counterclockwise loops the
 * contour has done. Of course, it the contour has done
 * clockwise loops, then the given number is accordingly
 * negative.
 *
 * @return the number of counterclockwise loops, or '0' is the contour
 * is open or invalid.
 */
int
ImaGene::FreemanChain::isClosed() const
{
  FreemanChain::const_iterator it = this->begin();
  FreemanChain::const_iterator it_end = this->end();
  FreemanChain::const_iterator it_suiv = it;
  Vector2i spos = *it;
  int nb_ccw_turns = 0;
  while ( it != it_end )
    {
      int code1 = it.getCode();
      it_suiv.nextInLoop();
      int code2 = it_suiv.getCode();
      uint diff = ( code2 - code1 + 4 ) % 4;
      if ( diff == 1 )      ++nb_ccw_turns;
      else if ( diff == 3 ) --nb_ccw_turns;
      else if ( diff == 2 ) return 0;
      ++it;
    }
  if ( spos == *it_suiv )
    return nb_ccw_turns / 4;
  else return 0;
}








/**
 * Return a vector containing all the interger points of the freemanchain.
 *
 * @param fc the FreemanChain
 * @param vContour (returns) the vector containing all the integer contour points.
  */
void
ImaGene::FreemanChain::getContourPoints(const FreemanChain & fc, vector<Vector2i> & vContour) 
{
  vContour.clear();
  for ( FreemanChain::const_iterator it = fc.begin();
	it != fc.end();
	++it )
    {
      vContour.push_back(*it);
    }
}



///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::FreemanChain::~FreemanChain()
{
}

/**
 * Computes a bounding box for the Freeman chain code.
 *
 * @param min_x (returns) the minimal x-coordinate.
 * @param min_y (returns) the minimal y-coordinate.
 * @param max_x (returns) the maximal x-coordinate.
 * @param max_y (returns) the maximal y-coordinate.
 */
void
ImaGene::FreemanChain::computeBoundingBox( int32 & min_x, int32 & min_y, 
					   int32 & max_x, int32 & max_y ) const
{
  min_x = max_x = x0;
  min_y = max_y = y0;
  for ( FreemanChain::const_iterator it = begin();
	it != end();
	++it )
    {
      Vector2i c( *it );
      if ( c.x() < min_x ) min_x = c.x();
      else if ( c.x() > max_x ) max_x = c.x();
      if ( c.y() < min_y ) min_y = c.y();
      else if ( c.y() > max_y ) max_y = c.y();
    }
}

/**
 * Computes a bounding box for the Freeman chain code.
 *
 * @param min_x (returns) the minimal x-coordinate.
 * @param min_y (returns) the minimal y-coordinate.
 * @param max_x (returns) the maximal x-coordinate.
 * @param max_y (returns) the maximal y-coordinate.
 */
void
ImaGene::FreemanChain::computeBoundingBox( int64 & min_x, int64 & min_y, 
					   int64 & max_x, int64 & max_y ) const
{
  min_x = max_x = x0;
  min_y = max_y = y0;
  for ( FreemanChain::const_iterator it = begin();
	it != end();
	++it )
    {
      Vector2i c( *it );
      if ( c.x() < min_x ) min_x = c.x();
      else if ( c.x() > max_x ) max_x = c.x();
      if ( c.y() < min_y ) min_y = c.y();
      else if ( c.y() > max_y ) max_y = c.y();
    }
}


/**
   Given a 2D vector, return its basis.
   The basis is given by the returned two letters a<b.
   @param a (modified) the small letter in 0-3 (horizontal axis)
   @param b (modified) the big letter in 0-3 (vertical axis)
   @param v any non-null 2D vector.
   @param inner 'true' if the basis is related to the inner contour. 
   @param cw 'true' if the basis is related to a clockwise contour (inside to the right). 
*/
void
ImaGene::FreemanChain::
basis( uint & a, uint & b, 
       const Vector2i & v, bool inner, bool cw  )
{
  uint q = v.quadrant();
  bool cnd = ( inner && cw ) || ( !inner && !cw );
  if ( v.x() * v.y() != 0 ) // non trivial vector.
    {
      a = cnd ? q : ( q + 1 ) % 4; 
      b = cnd ? ( q + 1 ) % 4  : q; 
    }
  else
    {
      a = q;
      b = cnd ? ( q + 1 ) % 4  : ( q + 3 ) % 4; 
    }

}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::FreemanChain::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[FreemanChain]";
  for ( FreemanChain::const_iterator it = begin();
	it != end();
	++it )
    {
      that_stream << "("<<(*it).x() << "," << (*it).y() << ") ";
    }
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::FreemanChain::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
