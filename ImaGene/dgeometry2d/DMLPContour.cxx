///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : DMLPContour.cxx
//
// Creation : 2010/01/18
//
// Version : 2010/01/18
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
//	2010/01/18 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/dgeometry2d/DMLPContour.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/DMLPContour.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const DMLPContour_RCS_ID = "@(#)class DMLPContour definition.";



///////////////////////////////////////////////////////////////////////////////
// class DMLPContour
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::DMLPContour::~DMLPContour()
{
}

/**
 * Constructor. The contour is not valid.
 *
 * @param cw specifies if the contour turns clockwise ('true')
 * around its inside or counterclockwise. Default is 'true'
 * (clockwise).
 */
ImaGene::DMLPContour::DMLPContour( bool cw )
  : m_contour( true ), m_cw( cw ), 
    m_length( 0.0 ), m_nb_of_updates( 0 )
{
}



/**
 * Simplifies the MLP of the contour till no more changes are
 * possible. As a matter of fact, it computes the MLP of the whole
 * contour. Note that this is not the fastest way of doing it.
 */
uint 
ImaGene::DMLPContour::simplify()
{
  uint nb = 0;
  uint nb0;
  do 
    {
      nb0 = 0;
      edge_iterator itb = m_start_edge;
      if ( ! null() )
	nb0 += simplifyAt( itb );
      edge_iterator it = next( m_start_edge );
      while ( ( ! null() ) && ( it != m_start_edge ) )
	{
	  nb0 += simplifyAt( it );
	  it = next( it );
	}
      nb += nb0;
    }
  while ( nb0 != 0 );

  return nb;
}


/**
 * Fills the given vectors with the list of edge vertices
 * (i.e. the base of each edge).
 *
 * @param pts (update) the coordinates of edge base are pushed
 * at the end of this vector.
 *
 * @param inside (update) the base type of each edge base is
 * pushed at the end of this vector.
 */
void
ImaGene::DMLPContour::getPoints
( vector<Vector2i> & pts,
  vector<bool> & inside ) const
{
  if ( ! null() ) 
    {
      Vector2i start_pt( m_start_pos );
      const_edge_iterator it = m_start_edge;
      do
	{ 
	  pts.push_back( start_pt );
	  inside.push_back( it->base );
	  start_pt += it->n * it->v;
	  it = next( it );
	}
      while ( it != m_start_edge );
    }
} 


///////////////////////////////////////////////////////////////////////////////
// ----------------------- split services -----------------------
    
/**
 * The edge should be primitive non trivial.  Splits the edge E(zk)
 * into E(zk-1)E(zk-2) according to the parity of the slope. Returns
 * an iterator on the second part of the fraction.
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::splitBerstelLowerChristoffel
( edge_iterator & it, TouchType split_tt )
{
  if ( it->n != 1 ) 
    cerr << "[DynamicContour::splitBerstelLowerChristoffel] Edge should be primitive."
	 << endl;
  edge_iterator itn = next( it );
  //    TouchType split_tt = getTouchTypeForSplit( true );
  Edge nr;
  // JOL: the following lines are necessary since elements of list may
  // not be modified.
  // Edge & nl = *it;
  Edge nl = *it;
  CFraction z( nl.z );
  if ( ! z.splitBerstel( nl.z, nl.n, nr.z, nr.n ) ) 
    {
      // cerr << "[splitLC] " << z << " no split" << endl;
      return it;
    }
  nr.base = split_tt;
  nr.oc = nl.oc;
  nl.v = nl.oc.castBack( Vector2i( nl.z.q(), nl.z.p() ) );
  nr.v = nr.oc.castBack( Vector2i( nr.z.q(), nr.z.p() ) );
  // cerr << "[splitLC] " << z << " = (" << nl.z << ")^" << nl.n
  // 	 << " + " << " (" << nr.z << ")^" << nr.n << endl;
  modifyEdge( it, nl );
  if ( ( split_tt != it->base )
       && ( it->n > 1 ) ) 
    {
      // Should subdivide first edge when the aligned inner vertices
      // have not the same touch type as the base of the vector.
      subdivide( it, it->n - 1,
		 it->base, split_tt );
      // it_inter->base = split_tt;
    }
  return insertEdge( itn, nr );
}

/**
 * The edge should be primitive non trivial.  Splits the edge E(zk)
 * into E(zk-1)E(zk-2) according to the parity of the slope. Returns
 * an edge_iterator on the second part of the fraction.
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::splitBerstelUpperChristoffel
( edge_iterator & it, TouchType split_tt )
{
  if ( it->n != 1 ) 
    cerr << "[DynamicContour::splitBerstelUpperChristoffel] Edge should be primitive."
	 << endl;
  edge_iterator itn = next( it );
  //    TouchType split_tt = getTouchTypeForSplit( false );
  Edge nr;
  // JOL: the following lines are necessary since elements of list may
  // not be modified.
  // Edge & nl = *it;
  Edge nl = *it;
  CFraction z( it->z );
  if ( ! z.splitBerstel( nr.z, nr.n, nl.z, nl.n ) )
    {
      // cerr << "[splitUC] " << z << " no split" << endl;
      return it;
    }
  nr.base = split_tt;
  nr.oc = nl.oc;
  nl.v = nl.oc.castBack( Vector2i( nl.z.q(), nl.z.p() ) );
  nr.v = nr.oc.castBack( Vector2i( nr.z.q(), nr.z.p() ) );
  // cerr << "[splitUC] " << z << " = (" << nl.z << ")^" << nl.n
  // 	 << " + " << " (" << nr.z << ")^" << nr.n << endl;
  modifyEdge( it, nl );
  if ( ( split_tt != it->base )
       && ( it->n > 1 ) )
    {
      // Should subdivide first edge when the aligned inner vertices
      // have not the same touch type as the base of the vector.
      subdivide( it, it->n - 1,
		 it->base, split_tt );
      // it_inter->base = split_tt;
    }
  return insertEdge( itn, nr );
}

/**
 * Given some iterator [it] on a edge, split it at its target
 * (front) so that there is a trivial edge at its front (in fact,
 * depth <= 1). The edge vector is split on its lower side (standard
 * Berstel/splitting decomposition) when [lower] is 'true',
 * otherwise it is split on its upper side (the reverse). [t] gives
 * the base type assigned to each created base vertex.
 *
 * @param itn an iterator on a edge of the contour.
 * @param lower the type of split.
 * @param t the base type for the created base vertices.
 * @return an iterator on the created trivial edge.
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::splitAtTargetTilQuasiTrivial
( edge_iterator itn, bool lower,
  TouchType t )
{
  // edge_iterator itn = prev( it );
  while ( true )
    {
      if ( itn->n > 1 ) 
	itn = subdivide( itn, 1, itn->base, itn->base );
      edge_iterator itr = 
	( itn->oc.isIndirect() ? ! lower : lower )
	? splitBerstelLowerChristoffel( itn, t )
	: splitBerstelUpperChristoffel( itn, t );
      if ( itr == itn ) break;
      itn = itr;
    }
  return itn;
}

/**
 * Given some iterator [it] on a edge, split it at its source/base (back)
 * so that [it] points to a trivial edge (in fact, depth <= 1). The
 * edge vector is split on its lower side (standard
 * Berstel/splitting decomposition) when [lower] is 'true',
 * otherwise it is split on its upper side (the reverse). [t] gives
 * the base type assigned to each created base vertex.
 *
 * @param itn an iterator on a edge of the contour.
 * @param lower the type of split.
 * @param t the base type for the created base vertices.
 * @return an iterator on the created trivial edge (in fact [it]).
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::splitAtBaseTilQuasiTrivial
( edge_iterator itn, bool lower,
  TouchType t )
{
  // edge_iterator itn = it;
  while ( true )
    {
      if ( itn->n > 1 ) 
	subdivide( itn, itn->n - 1, itn->base, itn->base );
      edge_iterator itr = 
	( itn->oc.isIndirect() ? ! lower : lower )
	? splitBerstelLowerChristoffel( itn, t )
	: splitBerstelUpperChristoffel( itn, t );
      if ( itr == itn ) break;
    }
  return itn;
}

/**
 * Given some iterator [it] on a edge, split it at its target
 * (front) so that there is a trivial edge at its front (in fact,
 * vector (1,0) repeated once). The edge vector is split on its
 * lower side (standard Berstel/splitting decomposition) when
 * [lower] is 'true', otherwise it is split on its upper side (the
 * reverse). [t] gives the base type assigned to each created base
 * vertex.
 *
 * @param itn an edge_iterator on a edge of the contour.
 * @param lower the type of split.
 * @param t the base type for the created base vertices.
 * @return an iterator on the created trivial edge.
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::splitAtTargetTilTrivial
( edge_iterator itn, bool lower,
  TouchType t )
{
  edge_iterator itt = splitAtTargetTilQuasiTrivial( itn, lower, t );
  edge_iterator itt2 = splitQuasiTrivial( itt, lower, t );
  return subdivide( itt2, 1, itt2->base, itt2->base );
}

/**
 * Given some iterator [it] on a edge, split it at its source/base
 * (back) so that [it] points to a trivial edge (in fact, vector
 * (1,0) repeated once). The edge vector is split on its lower side
 * (standard Berstel/splitting decomposition) when [lower] is
 * 'true', otherwise it is split on its upper side (the
 * reverse). [t] gives the base type assigned to each created base
 * vertex.
 *
 * @param itn an iterator on a edge of the contour.
 * @param lower the type of split.
 * @param t the base type for the created base vertices.
 * @return an iterator on the created trivial edge (in fact [it]).
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::splitAtBaseTilTrivial
( edge_iterator itn, bool lower,
  TouchType t )
{
  edge_iterator itb = splitAtBaseTilQuasiTrivial( itn, lower, t );
  splitQuasiTrivial( itb, lower, t );
  subdivide( itb, itb->n - 1, itb->base, itb->base );
  return itb;
}

/**
 * This method decomposes a quasi trivial vector ( (1,x) or (x,1) )
 * into trivial vectors (1,0), (0,1) in different quadrant.
 *
 * [it] points to a primitive edge with some vector (1,x) or (x,1)
 * or (-1,x) or (x,-1).  Decomposes it into two edges in different
 * quadrant.
 *
 * The edge vector is split on its lower side (standard
 * Berstel/splitting decomposition) when [lower] is 'true',
 * otherwise it is split on its upper side (the reverse). [t] gives
 * the base type assigned to each created base vertex.
 *
 * Note also that the user should have moved away the start edge
 * from this zone.
 *
 * @param it an iterator in this contour which points to a quasi trivial edge.
 * @param lower indicates if the split must be lower or upper.
 * @param t the base type for the created base vertices.
 * @return an iterator on the created trivial edge (which follows [it]).
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::splitQuasiTrivial
( edge_iterator & it, bool lower, TouchType t ) 
{
  ASSERT_DMLPContour( ( it->n == 1 ) &&
		      "[DynamicContour::splitQuasiTrivial] edge should be primitive." );
  ASSERT_DMLPContour( ( it->z.depth() <= 1 ) &&
		      "[DynamicContour::splitQuasiTrivial] vector is too complex." );
  if ( it->z.p() == 0 ) return it;
  ASSERT_DMLPContour( ( it->z.p() == 1 ) &&
		      "[DynamicContour::splitQuasiTrivial] vector should be some 1/k." );
  lower = ( it->oc.isIndirect() ? ! lower : lower );
  if ( lower )
    {
      uint repetition = it->z.q();
      Edge nv( it->oc.castBack( Vector2i( 0, 1 ) ), t, 1 );
      Edge nh( it->oc.castBack( Vector2i( 1, 0 ) ), it->base, repetition );
      // JOL: Check this.
      modifyEdge( it, nh );
      edge_iterator itn = insertEdge( next( it ), nv ); 
      if ( ( it->base != t ) && ( repetition > 1 ) )
	{
	  // takes care that new edges have the correct TouchType. 
	  edge_iterator itn2 = subdivide( it, repetition - 1,
					  it->base, t );
	  // JOL: Check this.
	  // itn2->base = t;
	  return itn2;
	}
      return itn;
    }
  else
    {
      Edge nv( it->oc.castBack( Vector2i( 0, 1 ) ), it->base, 1 );
      Edge nh( it->oc.castBack( Vector2i( 1, 0 ) ), t, it->z.q() );
      // JOL: Check this.
      modifyEdge( it, nv );
      return insertEdge( next( it ), nh ); 
    }
}



/**
 * Given a edge pointed by [it] with n repetitions, decompose it
 * into two edges, one with n-[k] repetitions to the left, one with
 * [k] repetitions to the right. Return the iterator in the middle
 * if the edge has been split, otherwise [it] itself.
 *
 * O(1) operation.
 *
 * Note also that the user should have moved away the start edge
 * from this zone.
 *
 * @param it (modified) any iterator on the contour (points on the edge that is
 * subdivided). Although it may be modified if in storing mode, it
 * points on the same location in the list.
 * 
 * @param k indicates how it is subdivided into two aligned edges.
 *
 * @param t1 gives the new base type for the first part of the
 * subdivision (generally it->base).
 *
 * @param t2 gives the new base type for the second part of the
 * subdivision (generally it->base).
 *
 * @return an iterator on the created edge (after [it]), or [it]
 * itself if nothing was changed.
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::subdivide
( edge_iterator & it, uint k, TouchType t1, TouchType t2 )
{
  // edge_iterator it2 = it;
  // if ( ( k != 0 ) && ( k < it->n ) )
  //   {
  //     it2 = next( it );
  //     it2 = insertEdge( it2, *it );
  //     it->n -= k;
  //     it2->n = k;
  //   }
  // return it2;
  if ( ( k != 0 ) && ( k < it->n ) )
    {
      Edge e1 = *it;
      Edge e2 = e1;
      e1.n -= k;
      e1.base = t1;
      e2.n = k;
      e2.base = t2;
      modifyEdge( it, e1 );
      return insertEdge( next( it ), e2 );
    }
  else
    return it;
}

/**
 * Given a edge pointed by [it] with n repetitions, decompose it
 * into two edges, one with [k] repetitions to the left, one with
 * n-[k] repetitions to the right. Return the iterator in the middle
 * if the edge has been split, otherwise [it] itself.
 *
 * O(1) operation.
 *
 * Note also that the user should have moved away the start edge
 * from this zone.
 *
 * @param it (modified) any iterator on the contour (points on the edge that is
 * subdivided). Although it may be modified if in storing mode, it
 * points on the same location in the list.
 * 
 * @param k indicates how it is subdivided into two aligned edges.
 *
 * @param t1 gives the new base type for the first part of the
 * subdivision (generally it->base).
 *
 * @param t2 gives the new base type for the second part of the
 * subdivision (generally it->base).
 *
 * @return an iterator on the created edge (after [it]), or [it]
 * itself if nothing was changed.
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::subdivideLeft
( edge_iterator & it, uint k, TouchType t1, TouchType t2 )
{
  if ( ( k != 0 ) && ( k < it->n ) )
    {
      Edge e1 = *it;
      Edge e2 = e1;
      e1.base = t1;
      e2.n = e1.n - k;
      e1.n = k;
      e2.base = t2;
      modifyEdge( it, e1 );
      return insertEdge( next( it ), e2 );
    }
  else if ( k == 0 )
    {
      if ( it->base != t2 )
	{
	  Edge e = *it;
	  e.base = t2;
	  modifyEdge( it, e );
	}
    }
  else
    {
      if ( it->base != t1 )
	{
	  Edge e = *it;
	  e.base = t1;
	  modifyEdge( it, e );
	}
    }
  return it;
}



/**
 * This method is the base for flipping a pixel inside/out. Its
 * objective is to make the mlp trivial around the pointel pointed
 * by [it]. After that, it is easy to flip the trivial edges
 * inside/out.
 *
 * @param it an iterator in this contour (no more valid afterwards).
 * @param lower indicates if the split must be lower or upper.
 * @param t the base type for the created base vertices.
 *
 * @return an iterator on the created trivial edge (replacing
 * [it]). The preceding edge is also a trivial.
 */
ImaGene::DMLPContour::iterator
ImaGene::DMLPContour::splitAround( iterator it, bool lower, TouchType t )
{
  int pos = it.position();
  if ( pos < 0 )
    {
      cerr << "[ImaGene::DMLPContour::splitAround] Not yet implemented: do not know how to split around corners." << endl;
    }
  else
    {
      cerr << "[ImaGene::DMLPContour::splitAround] position = " << pos << endl;
      Vector2i vtx = it.baseVertex();
      edge_iterator edge_it = it.edge();
      edge_iterator prev_edge_it = prev( edge_it );
      edge_iterator new_edge_it = splitEdgeAt( edge_it,
					       (uint) pos, lower, t );
      edge_it = next( prev_edge_it );
      while ( edge_it != new_edge_it )
	{
	  vtx += edge_it->vector();
	  edge_it = next( edge_it );
	}
      return iterator( it.dmlp, it.inside, new_edge_it, vtx, 0 );
    }
  return it;
}

/**
 * This method is the base for flipping a pixel inside/out. Its
 * objective is to make the mlp trivial around the pointel pointed
 * by [it]. After that, it is easy to flip the trivial edges
 * inside/out.
 *
 * @param it an iterator in this contour (no more valid afterwards).
 * @param lower indicates if the split must be lower or upper.
 * @param t the base type for the created base vertices.
 *
 * @return an iterator on the created trivial edge (replacing
 * [it]). The preceding edge is also a trivial.
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::splitEdgeAt( edge_iterator & it, 
				   uint pos,
				   bool lower, TouchType t )
{
  // edge_iterator itn = it;
  uint shift = 0;
  while ( true )
    {
      const Edge & e = *it;
      // Display for debug
      cerr << "  ";
      for ( uint i = 0; i < shift; ++i ) cerr << "  ";
      cerr << e << endl;
      ++shift;
      TouchType tbase = e.base;
      uint size_primitive = e.v.norm1();
      uint k = pos / size_primitive;
      it = subdivideLeft( it, k, tbase, tbase );
      subdivideLeft( it, 1, tbase, tbase );
      pos = pos % size_primitive;
      // The edge pointed by [it] is primitive
      edge_iterator itr = 
	( it->oc.isIndirect() ? ! lower : lower )
	? splitBerstelLowerChristoffel( it, t )
	: splitBerstelUpperChristoffel( it, t );
      // it points on the left part of the edge, itr on the right part.
      if ( itr == it ) 
	{
	  // There was no split, ie the edge is trivial or quasitrivial.
	  itr = splitQuasiTrivial( it, lower, t );
	}
      uint dlen = it->digitalLength();
      if ( pos >= dlen ) 
	{ // right part of splitted edge.
	  it = itr;
	  pos = pos - dlen;
	}
      // We are now on the correct part of the splitted edge.
      // We check if the edge is correctly simplified. 
      if ( ( pos == 0 ) && ( it->digitalLength() == 1 ) )
	break;
    }
  return it;
}



/**
 * Checks if vertex is neutral, meaning that the two edges around
 * have opposite vectors and base types. 
 *
 * It may also remove an inconsistent vertex: prev( it ) and it are
 * opposite, of same base type, but next( it ) has a base type
 * different from prev( it ), although it is in the same place. For
 * now, only inconsistent vertex with trivial surrounding edges are
 * removed. They may appear in a VEE splitting with two edges in
 * different quadrants.
 *
 * @param it (modified) an iterator in this contour, which is moved
 * to the next valid edges.
 *
 * @return 'true' if the contour was changed, 'false' otherwise.
 */
bool 
ImaGene::DMLPContour::removeNeutral( edge_iterator & it )
{
  edge_iterator itp = prev( it );
  if ( ( it->v.x() == -itp->v.x() )
       && ( it->v.y() == -itp->v.y() )
       )
    if ( it->base != itp->base )
      {	
	moveStartEdgeBackwardIfEqual( it );
	moveStartEdgeBackwardIfEqual( itp );
	edge_iterator itn = next( it );
	if ( itp->n == it->n )
	  {
	    removeEdge( itp );
	    removeEdge( it );
	  }
	else if ( itp->n > it->n )
	  {
	    // JOL: Check this.
	    // itp->n -= it->n;
	    Edge ep = *itp;
	    ep.n -= it->n;
	    modifyEdge( itp, ep );
	    removeEdge( it );
	  }
	else
	  {
	    // it->base = itp->base;
	    // JOL: Check this.
	    // it->n -= itp->n;
	    Edge e = *it;
	    e.n -= itp->n;
	    modifyEdge( it, e );
	    removeEdge( itp );
	  }
	it = itn;
	return true;
      }
    else 
      {
	edge_iterator itn = next( it );
	if ( ( itn->base != itp->base ) 
	     && ( it->n == 1 ) && ( itp->n == 1 ) )
	  {
	    // cerr << "[removeNeutral]: remove inconsistency." << endl;
	    moveStartEdgeBackwardIfEqual( it );
	    moveStartEdgeBackwardIfEqual( itp );
	    TouchType t = it->base;
	    subdivide( itn, itn->n - 1,
		       t, itn->base );
	    removeEdge( itp );
	    removeEdge( it );
	    // JOL: Check this.
	    // itn->base = t;
	    it = itn;
	    return true;
	  }
      }
  return false;
}

/**
 * Checks if vertex is back and forth, meaning that the two edges
 * around have opposite vectors. The new base type depends on the
 * surrounding base types. If prev( it )->base == next( it )-> base
 * then the new base type is this one, otherwise, the new base type
 * is the same as it->base.
 *       
 @verbatim
 !      would give    !
 v                    v
 it x<==x                itn x==>o==>x
 ==>(o==>)^2x             
 @endverbatim
 *
 * Contrary to removeNeutral, this method will always remove the two
 * edges surrounding the vertex (if they are opposite) and assign a
 * some base type for next(it). It this base type is changed, next(
 * it ) is subdivided. 
 * 
 * Note also that the user should have moved away the start edge
 * from this zone.
 *
 * @param it (modified) an iterator in this contour, which is moved
 * to the next valid edge.
 *
 * @return 'true' if the contour was changed, 'false' otherwise.
 */
bool 
ImaGene::DMLPContour::removeOpposite( edge_iterator & it )
{
  edge_iterator itp = prev( it );
  if ( ( it->v.x() == -itp->v.x() )
       && ( it->v.y() == -itp->v.y() ) )
    {	
      edge_iterator itn = next( it );
      TouchType t = ( itn->base == itp->base )
	? itp->base : it->base;
      removeEdge( itp );
      removeEdge( it );
      it = itn;
      subdivide( it, it->n-1,
		 t, it->base );
      // it->base = t;
      return true;
    }
  return false;
}


/**
 * This is the main simplification method when computing dynamically
 * a minimum length polygon. Its objective is to have only WEDGE or
 * FLAT vertices. If some vertex (pointed by [it]) is some VEE, then
 * it changes the vertex (and the surrounding edges) to make the
 * dynamic contour pass through the symetric vertex (diagonally
 * opposite to it in the one-wide band of the digital contour),
 * which is of course of opposite base type. After that, this vertex
 * is a WEDGE (since the base type has changed). Furthermore, this
 * method will remove neutral vertices (FLAT, but opposite and with
 * opposite base type) and AFLAT vertices.
 *
 * This is not generally a O(1) operation. Split takes O(log(n))
 * (and for now O(log^2(n))).
 *
 * @param it (modifed) an iterator on a edge of the contour
 * (specifies the base vertex). The iterator may be moved to point
 * on a valid vertex.
 *
 * @return 'true' if the contour was changed, 'false' otherwise.
 *
 * @see ConnectionType.
 */
bool 
ImaGene::DMLPContour::fusion( edge_iterator & it )
{
  edge_iterator itp = prev( it ); 
  ConnectionType vtype = getConnectionType( it );
  bool change = false;
  // cerr << "[fusion] before:";
  // selfDisplayWithIterator( cerr, it );

  switch ( vtype ) 
    {
    case FLAT:
      // vectors are aligned. Merge is O(1).
      // cerr << "[fusion] FLAT " << (*itp) << " - " << (*it) << endl; 
      if ( ( itp->v.x() == it->v.x() ) 
	   && ( itp->v.y() == it->v.y() )
	   && ( itp->base == it->base ) )
	{
	  // TODO ! changes things
	  moveStartEdgeBackwardIfEqual( it );
	  moveStartEdgeBackwardIfEqual( itp );
	  Edge e = *it;
	  e.n += itp->n;
	  modifyEdge( it, e );
	  // it->n += itp->n;
	  removeEdge( itp ); //contour.erase( itp ); //removeEdge( itp );
	  change = true;
	}
      else 
	{ // vectors are opposite. Check if they can be removed.
	  // cerr << "[fusion] NON STD FLAT : ISNEUTRAL " << (*itp) << " - " << (*it) << endl; 
	  if ( removeNeutral( it ) )
	    change = true;
	}
      break;
    case AFLAT:
      // cerr << "[fusion] AFLAT " << (*itp) << " - " << (*it) << endl; 
      moveStartEdgeBackwardIfEqual( it );
      moveStartEdgeBackwardIfEqual( itp );
      if ( it->base == INSIDE )
	it = lowerFusion( itp, it );
      else
	it = upperFusion( itp, it );
      change = true;
      break;
    case WEDGE:
      // nothing to do.
      // cerr << "[fusion] WEDGE " << (*itp) << " - " << (*it) << endl; 
      break;
    case VEE:
      // In this case, we have to flip INSIDE/OUTSIDE at this position.
      // cerr << "[fusion] VEE " << (*itp) << " - " << (*it) << endl; 
      bool lower = getOtherSide( it ) == RIGHT;
      // The created vertices are on the other side of the shape, so
      // they should lie on OUTSIDE pixels if [it] was lying on an
      // INSIDE pixel, and inversely.
      TouchType t = it->base == INSIDE ? OUTSIDE : INSIDE;
      moveStartEdgeBackwardIfEqual( it );
      moveStartEdgeBackwardIfEqual( itp );

      edge_iterator itl = splitAtTargetTilTrivial( prev( it ), lower, t ); 
      edge_iterator itr = splitAtBaseTilTrivial( it, lower, t );
      // in case the edges where not in the same quadrant.
      if ( removeOpposite( itr ) )
	it = itr;
      else 
	{ // standard case.
	  swapEdge( itr, true );
	  // it = itl;
	  it = prev( itr );
	}
      change = true;
      break;

    }
  // if ( change )
  //   {
  //     // cerr << "[fusion] post:  ";
  //     // selfDisplayWithIterator( cerr, it );
  //   }
  // else
  //   cerr << "[fusion] post: no change." << endl;
  return change;
}



/**
 * This method merges an AFLAT vertex designated by the two
 * consecutive edges [it] and [itn].
 *
 * Merge the two edges pointed by [it] and [itn], provided they are
 * mergeable. Not very efficient for now, since we do not exploit
 * the repetitions of the vectors. Assume that the vectors of the
 * two edges have determinant 1 in a clockwise contour with an
 * INSIDE join.
 *
 * Note also that the user should have moved away the start edge
 * from this zone.
 *
 * @param it (modified) an iterator in the contour (the edge is
 * subdivided if its repetition number is greater than 1).
 *
 * @param itn (modified) an iterator in the contour equal to
 * 'next( it )' (the edge is subdivided if its repetition number
 * is greater than 1).
 *
 * @return an iterator on the merged edge.
 *
 * @TODO do it fast !
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::lowerFusion
( edge_iterator & it, 
  edge_iterator & itn )
{
  // we already know that these two consecutive edges (with an
  // INSIDE between) are mergeable (det=1).
  edge_iterator itn_left = subdivide( itn, itn->n - 1, itn->base, itn->base );
  edge_iterator it_right = subdivide( it, 1, it->base, it->base );
  Edge n( it_right->v + itn_left->v, 
	  it_right->base, 
	  1 );
  removeEdge( itn );
  // JOL: Check this.
  modifyEdge( it_right, n );
  return it_right;
}

/**
 * This method merges an AFLAT vertex designated by the two
 * consecutive edges [it] and [itn].
 *
 * Merge the two edges pointed by [it] and [itn], provided they are
 * mergeable. Not very efficient for now, since we do not exploit
 * the repetitions of the vectors. Assume that the vectors of the
 * two edges have determinant 1 in a clockwise contour with an
 * INSIDE join. For now, calls lowerFusion.
 *
 * Note also that the user should have moved away the start edge
 * from this zone.
 *
 * @param it an iterator in the contour (the edge is subdivided if
 * its repetition number is greater than 1).
 *
 * @param itn an iterator in the contour equal to 'next( it )' (the
 * edge is subdivided if its repetition number is greater than 1).
 *
 * @return an iterator on the merged edge.
 *
 * @TODO do it fast !
 */
ImaGene::DMLPContour::edge_iterator
ImaGene::DMLPContour::upperFusion
( edge_iterator & it, 
  edge_iterator & itn )
{
  return lowerFusion( it, itn );
}



///////////////////////////////////////////////////////////////////////////////
// ----------------------- deformable model services -----------------------


/**
 * Creates a dynamic MLP for the given interpixel contour
 * specified as a Freeman chaincode of '0', '1', '2', '3'
 * characters. The coded contour [s] must be closed.
 *
 * @param s a Freeman chaincode.
 *
 * @return an iterator in this DMLP pointing exactly at the base
 * of the first coded linel in [s] (i.e., corresponds to the base
 * of the first character).
 */
ImaGene::DMLPContour::iterator
ImaGene::DMLPContour::initFromInterpixelContour( const FreemanChain & s )
{
  std::vector<int> vx;
  std::vector<int> vy;
  std::vector<uint> vi;
  std::vector<bool> vt;
  Vector2i twice_dv;
  FreemanChain::const_iterator it_s =
    FreemanChain::computeMLP( vx, vy, vi, vt, m_twice_dv, s, m_cw );
  ASSERT_DMLPContour( ( it_s != s.end() ) 
		      && "[ImaGene::DMLPContour::initFromInterpixelContour(s)] Error computing combinatorial MLP of given FreemanChain." );
  
  m_contour.clear();
  m_nb_of_updates = 0;
  m_length = 0.0;
  edge_iterator it = endEdge();
  uint n = vx.size();
  for ( uint i = 0; i < n; ++i )
    cerr << ( vx[ i ]+0.5*m_twice_dv.x() ) 
	 << " " << ( vy[ i ]+0.5*m_twice_dv.y() )
	 << " " << vt[ i ] << " " << vi[ i ] << endl;
  for ( uint i = 0; i < n; ++i )
    {
      Vector2i v( vx[ ( i+1 ) % n ] - vx[ i ], 
		  vy[ ( i+1 ) % n ] - vy[ i ] );
      if ( ( v.x() != 0 ) || ( v.y() != 0 ) )
	{
	  int d = Mathutils::gcd( v.x(), v.y() );
	  // The convention for the combinatorial MLP is different
	  // from the dynamic MLP. In fact, in the dynamic MLP, when
	  // an edge is composed of several patterns, the base type of
	  // each pattern is the one of the edge. In the combinatorial
	  // MLP, the base type of the patterns but the first is the
	  // base type of the next edge !
	  // This is why we must convert these edges.
	  if ( ( d > 1 ) && ( vt[ i ] != vt[ ( i+1 ) % n ] ) )
	    {
	      Edge n1( v / d, getTouchType( vt[ i ] ), 1 );
	      insertEdge( it, n1 );
	      Edge n2( v / d, getTouchType( vt[ ( i+1 ) % n ] ), d - 1 );
	      insertEdge( it, n2 );
	    }
	  else
	    {
	      Edge n( v / d, getTouchType( vt[ i ] ), d );
	      insertEdge( it, n );
	    }
	}
    }
  m_start_pos = Vector2i( vx[ 0 ], vy[ 0 ] );
  m_start_edge = beginEdge();
}

/**
 * Given the coordinates of a pointel (the base of a linel),
 * returns an iterator on the corresponding point of the DMLP.
 *
 * @param x the x-coordinate of the pointel (in the original frame
 * as given in input).
 *
 * @param y the y-coordinate of the pointel (in the original frame
 * as given in input).
 *
 * @return the iterator on the corresponding point of the DMLP. If it
 * was found, it has the correct coordinates and its member 'inside'
 * has value 'true'. If not, the iterator is positionned on the start
 * edge, with its member 'inside' equal to 'false'.
 */
ImaGene::DMLPContour::iterator
ImaGene::DMLPContour::findPointel( int x, int y )
{
  Vector2i P( x, y );
  iterator it1 = this->first( true );
  iterator it = it1;
  do {
    if ( P == it.pointel() )
      return it;
    ++it;
  } while ( it != it1 );
  it.inside = false;
  return it;
}

/**
 * @return the length (i.e. perimeter) of the current MLP.
 */
double 
ImaGene::DMLPContour::getLength() const
{
  return m_length;
}

/**
 * Forces the recomputation of the length (i.e. perimeter) of the
 * current MLP. This is because the length of the DMLP is updated
 * after each flip by local computations and errors may tend to
 * accumulate. Automatically called by 'flip' after a given number
 * of calls.
 */
void 
ImaGene::DMLPContour::updateLength()
{
  m_length = computeLength();
  m_nb_of_updates = 0;
}
    
/**
 * Computes the length of the current MLP by summing the length of
 * each edge.
 */
double 
ImaGene::DMLPContour::computeLength() const
{
  double l = 0.0;
  for ( const_edge_iterator it = beginEdge(); it != endEdge(); ++it )
    l += it->euclideanLength();
  return l;
}

/**
 * Flips the dynamic MLP at position specified by [it] and in the
 * direction specified by 'it.inside'.
 *
 * @param it specifies the position where to flip the MLP.
 *
 * @param store_undo when 'true', stores the reverse flip
 * operation that will undo this change. This parameter may be
 * false only if the stack 'm_undo' is empty.
 * 
 * @return 'true' iff the flip was successfully performed, 'false'
 * if it was unable to do it.
 *
 * @see undoFlip
 * @see topUndo
 * @see m_undo
 */
bool
ImaGene::DMLPContour::flip( const iterator & it, bool store_undo )
{
  // Moves start edge outside flip zone just in case.
  moveStartEdgeBackwardIfEqual( it.edge() );
  edge_iterator it_edge_prev = prev( it.edge() );
  edge_iterator it_edge_next = next( it.edge() );
  int pos = it.position();
  if ( pos < 0 )
    {
      cerr << "[ImaGene::DMLPContour::flip] flip around corners is not implemented yet." << endl;
    }
  else
    {
      bool lower = getOtherSide( it.edge() ) == DMLPContour::RIGHT;
      // The created vertices are on the other side of the shape, so
      // they should lie on OUTSIDE pixels if [it] was lying on an
      // INSIDE pixel, and inversely.
      DMLPContour::TouchType t;
      DMLPContour::TouchType opp_t;
      if ( it.edge()->base == DMLPContour::INSIDE )
	{
	  t = DMLPContour::OUTSIDE;
	  opp_t = DMLPContour::INSIDE;
	}
      else
	{
	  t = DMLPContour::INSIDE;
	  opp_t = DMLPContour::OUTSIDE;
	}
      iterator new_it = splitAround( it, lower, t );
      edge_iterator new_it_edge = new_it.edge();
      edge_iterator new_it_edge_prev = prev( new_it_edge );
      edge_iterator new_it_edge_next = next( new_it_edge );
      cerr << "[ImaGene::DMLPContour::flip] in flip." << endl;
      cerr << "  new_it_edge      = " << *new_it_edge << endl;
      cerr << "  new_it_edge_prev = " << *new_it_edge_prev << endl;
      cerr << "  new_it_edge_next = " << *new_it_edge_next << endl;
      cerr << "  " << *this << endl;
      //ASSERT_DMLPContour( new_it_edge->v.norm1() == 1 );
      // ASSERT_DMLPContour( new_it_prev.edge()->v.norm1() == 1 );
      ASSERT_DMLPContour( it_edge_prev != it_edge_next );
      if ( new_it_edge->v != new_it_edge_prev->v )
	{
	  flipBaseTypeOfTarget( new_it_edge );
	  // Edge e( *new_it_edge_next );
	  // e.flip();
	  // modifyEdge( new_it_edge_next, e );
	  simplify();
	  // while ( ! null() )
	  //   {
	  //     //edge_iterator itb = new_it_edge;
	  //     unsigned int nb0 = simplifyAt( new_it_edge );
	  //     if ( nb0 == 0 ) // go to next edge
	  // 	break;
	  //   }
	  return true;
	}
    }
  return false;
}

/**
 * Undoes the last flip operation. Can be used only if
 * 'store_undo' was set during the last flip operation.
 */
void 
ImaGene::DMLPContour::undoFlip()
{
}

/**
 * @return the iterator on this modified MLP which induces the
 * inverse flip to go back to the previous MLP. 
 */
ImaGene::DMLPContour::iterator 
ImaGene::DMLPContour::topUndo() const
{
}

/**
 * @return 'true' if the stack of stored inverse flips is not empty.
 */
bool 
ImaGene::DMLPContour::isUndoPossible() const
{
}

/**
 * Clears the stack of stored inverse flips.
 */
void 
ImaGene::DMLPContour::clearUndo()
{
}








///////////////////////////////////////////////////////////////////////////////
// Interface - public :


/**
 * Displays the contour on the output stream for debugging purposes.
 *
 * @param out the output stream.
 */
void
ImaGene::DMLPContour::selfDisplay( ostream & out ) const
{
  for ( const_edge_iterator it = m_contour.begin();
	it != m_contour.end();
	++it )
    {
      if ( it == m_start_edge )
	out << "{S" << m_start_pos.x() << "," << m_start_pos.y() << "}";
      (*it).selfDisplay( out );
    }
  out << endl;
}

/**
 * Displays the contour on the output stream for debugging
 * purposes. Emphasizes the given iterator [it] in upper case
 * letters.
 *
 * @param out the output stream.
 * @param it an iterator in the list of edges of the contour.
 */
void 
ImaGene::DMLPContour::selfDisplayWithIterator
( ostream & out, const edge_iterator & it )
{
  for ( edge_iterator itc = m_contour.begin();
	itc != m_contour.end();
	++itc )
    {
      if ( itc == m_start_edge )
	out << "{S" << m_start_pos.x() << "," << m_start_pos.y() << "}";
      if ( it == itc) (*itc).selfDisplayUpperCase( out );
      else            (*itc).selfDisplay( out );
    }
  out << endl;
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::DMLPContour::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
