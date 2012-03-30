///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : CubicalComplex.cxx
//
// Creation : 2008/03/13
//
// Version : 2008/03/13
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
//	2008/03/13 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <map>
#include <set>
#include <deque>
#include <queue>
#include "ImaGene/digitalnD/CubicalComplex.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/CubicalComplex.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace ImaGene;

const char* const CubicalComplex_RCS_ID = "@(#)class CubicalComplex definition.";

/**
 * Number of bits hold in this.
 * @see data_t
 */
const int ImaGene::CubicalCellData::NBBITS = 32;

/**
 * Number of bits reserved for flags.
 * @see data_t
 */
const int ImaGene::CubicalCellData::FLAGBITS = 4;

/**
 * Number of bits reserved for value.
 * Equal to NBBITS-FLAGBITS.
 * @see data_t
 */
const int ImaGene::CubicalCellData::VALBITS = 
ImaGene::CubicalCellData::NBBITS - ImaGene::CubicalCellData::FLAGBITS;

/**
 * REMOVED flag.
 */
const uint ImaGene::CubicalCellData::REMOVED = 0x10000000;

/**
 * COLLAPSIBLE flag.
 */
const uint ImaGene::CubicalCellData::COLLAPSIBLE = 0x20000000;

/**
 * FIXED flag.
 */
const uint ImaGene::CubicalCellData::FIXED = 0x40000000;

/**
 * User Flag 1.
 */
const uint ImaGene::CubicalCellData::USER1 = 0x80000000;





///////////////////////////////////////////////////////////////////////////////
// class CubicalComplex
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Static services - public :

/**
 * Collapse a subcomplex of the closed cubical complex [K], collapsing
 * cells following priority [priority], in a decreasing sequence. [S]
 * provides the starting cells, generally (but not compulsory) maximal
 * cells.  Note: cells that have been marked as fixed are not removed.
 * only cells that are in the closure [S] may be removed, and only if
 * they are not marked as fixed.
 *
 * @param K the closed cubical complex to collapse.
 * @param S the set of cells of [K], as iterators in [K]
 * @param S_closed indicates if [S] is a closed set (faster in this case)
 * @param priority the object that assign a priority to each cell.
 */
void
ImaGene::CubicalComplex::collapse( CubicalComplex & K,
				   cc_cell_set & S,
				   bool S_closed,
				   const CubicalCellPriority & priority )
{
  // NB : a maximal k-cell is collapsible if it has a free incident k-1-cell.
  uint dim_s = K.dim();

  deque< CubicalCell > Q;
  //deque< iterator > Q_up;
  deque< iterator > Q_low;
  deque< iterator > Q_collapsible;
  iterator it_c;
  deque< iterator >::const_iterator itlow;
  deque< iterator >::const_iterator itqup;

  cout << "[CC::collapse]-+ tag collapsible elements... " << flush;
  // Restricts the set of elements that are collapsible.
  if ( S_closed )
    for ( cc_cell_set::const_iterator it = S.begin();
	  it != S.end();
	  ++it )
      {
	it_c = *it;
	CubicalCellData & ccdata = it_c->second;
	if ( ! ( ccdata.isFlagged( CubicalCellData::FIXED ) 
		 || ccdata.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
	  {
	    ccdata.setFlag( CubicalCellData::COLLAPSIBLE );
	    Q_collapsible.push_back( it_c );
	  }
      }
  else // not ( S_closed )
    for ( cc_cell_set::const_iterator it = S.begin();
	  it != S.end();
	  ++it )
      {
	CubicalCell::lowIncidentCells( Q, (*it)->first, 
				       (*it)->first.dim( dim_s ), 
				       0, dim_s, dim_s );
	while ( ! Q.empty() )
	  {
	    it_c = K.find( Q.front() );
	    CubicalCellData & ccdata = it_c->second;
	    if ( ! ( ccdata.isFlagged( CubicalCellData::FIXED ) 
		     || ccdata.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
	      {
		ccdata.setFlag( CubicalCellData::COLLAPSIBLE );
		Q_collapsible.push_back( it_c );
	      }
	    Q.pop_front();
	  }
      }
  cout << " " << Q_collapsible.size() << " found." << endl;

  // Fill queue
  priority_queue
    < iterator, vector<iterator>, CubicalCellPriority > PQ( priority );

  
  cout << "[CC::collapse]-+ entering collapsing loop. " << endl;
  uint nb_pass = 0;
  uint nb_examined = 0;
  while ( ! S.empty() ) 
    {
      cout << "[CC::collapse]---+ Pass " << ++nb_pass
	   << ", Card(PQ)=";
      // cout << "[CC::collapse]---+ filling priority queue with ";
      // cout << endl;
      // push elements in the priority queue.
      for ( cc_cell_set::iterator it = S.begin();
	    it != S.end();
	    ++it )
	{
	  PQ.push( *it );
// 	  cout << "[CC::collapse]-----+ " << (*it)->first
// 	       << endl;
	  (*it)->second.setFlag( CubicalCellData::USER1 );
	}
      S.clear();
      // cout << "[CC::collapse]-----+ =";
      cout << PQ.size() << " elements, "
	   << "nb_exam=" << nb_examined << endl;
      
      // Try to collapse elements according to priority queue.
      while ( ! PQ.empty() )
	{
	  // Get top element.
	  iterator itcur = PQ.top();
	  PQ.pop();
	  ++nb_examined;
	  CubicalCellData & cur_data = itcur->second;
	  if ( ! cur_data.isFlagged( CubicalCellData::USER1 ) )
	    cout << "ERROR1";
	  cur_data.resetFlag( CubicalCellData::USER1 );

// 	  cout << "[CC::collapse]-----+ /" << nb_examined << "/ "
// 	       << itcur->first;

	  // Check if the cell is removable
	  if ( cur_data.isFlagged( CubicalCellData::REMOVED ) 
	       || ( ! cur_data.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
	    {
// 	      cout << " not removable." << endl;
	      continue;
	    }

	  // Cell may be removable.

	  // Check if it is a maximal cell
	  const CubicalCell & cur_c = itcur->first;
	  iterator itup;
	  uint cur_c_type = K.computeCellType( cur_c, itup );
	  bool found_pair = false;
	  iterator it_cell_c;
	  iterator it_cell_d;
	  if ( cur_c_type == 0 )
	    { // maximal cell... must find a free face
	      // cout << ", maximal." << endl;

	      // check faces to find a free face.
	      K.lowIncidentCells1( cur_c, Q_low );
	      iterator low_ic;
	      iterator best_free_face_it = K.end();
	      for ( itlow = Q_low.begin(); itlow != Q_low.end(); ++itlow )
		{
		  low_ic = *itlow;
		  if ( low_ic->second.isFlagged( CubicalCellData::REMOVED ) 
		       || ( ! low_ic->second.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
		    continue;
		  const CubicalCell & cur_d = low_ic->first;
		  uint cur_d_type = K.computeCellType( cur_d, itup );

		  if ( cur_d_type == 1 )
		    { // found a free n-1-face ic
		      if ( ( best_free_face_it == K.end() )
			   // JOL 2008/04/07 : should choose greatest priority
			   // for removal.
			   || ( ! priority( low_ic, best_free_face_it ) ) )
			best_free_face_it = low_ic;
		    }
		}
	      if ( best_free_face_it != K.end() )
		{
		  // delete c and ic.
		  // cout << "[CC::collapse]------> found free n-1-face." << endl;
		  found_pair = true;
		  it_cell_c = itcur;
		  it_cell_d = best_free_face_it;
		  // Q_low already contains cells that should be
		  // checked again
		}
// 	      // check faces to find a free face.
// 	      K.lowIncidentCells1( cur_c, Q_low );
// 	      iterator low_ic;
// 	      for ( itlow = Q_low.begin(); itlow != Q_low.end(); ++itlow )
// 		{
// 		  low_ic = *itlow;
// 		  if ( low_ic->second.isFlagged( CubicalCellData::REMOVED ) 
// 		       || ( ! low_ic->second.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
// 		    continue;
// 		  const CubicalCell & cur_d = low_ic->first;
// 		  uint cur_d_type = K.computeCellType( cur_d, itup );

// 		  if ( cur_d_type == 1 )
// 		    { // found a free n-1-face ic
// 		      // delete c and ic.
// // 		      cout << "[CC::collapse]------> found free n-1-face." << endl;
// 		      found_pair = true;
// 		      it_cell_c = itcur;
// 		      it_cell_d = low_ic;
// 		      // Q_low already contains cells that should be
// 		      // checked again
// 		      break;
// 		    }
// 		}
	    }
	  else if ( cur_c_type == 1 )
	    { // free face... check that its 1-up-incident face is maximal.
	      const CubicalCell & cur_d = itup->first;
	      iterator it_up_up;
	      uint cur_d_type = K.computeCellType( cur_d, it_up_up );
	      if ( cur_d_type == 0 )
		{ // found a maximal face.
		  // cout << "[CC::collapse]-------> its n+1-face is maximal." << endl;
// 		  cout << ", free face with max n+1-face." << endl;
		  found_pair = true;
		  it_cell_c = itup;
		  it_cell_d = itcur;
		  // Q_low will contain cells that should be checked
		  // again
		  K.lowIncidentCells1( it_cell_c->first, Q_low );
		}
// 	      else
// 		cout << ", free face, no max n+1-face." << endl;
	    }
// 	  else
// 	    cout << ", not a free face." << endl;
	  if ( found_pair )
	    {
// 	      cout << "[CC::collapse]-------> removing " << it_cell_c->first
// 		   << " " << it_cell_d->first
// 		   << endl;
	      it_cell_c->second.setFlag( CubicalCellData::REMOVED );
	      it_cell_d->second.setFlag( CubicalCellData::REMOVED );

	      // Incident cells have to be checked again.
// 	      uint nb_put_in_S = 0;
// 	      uint nb_low_1 = Q_low.size();
	      while ( ! Q_low.empty() )
		{
		  const CubicalCellData & data_qlow = 
		    Q_low.front()->second;
		  if ( ( ! data_qlow.isFlagged( CubicalCellData::REMOVED ) )
		       && data_qlow.isFlagged( CubicalCellData::COLLAPSIBLE )
		       && ( ! data_qlow.isFlagged( CubicalCellData::USER1 ) ) )
		    {
// 		      cout << "[CC::collapse]-------+ S += " 
// 			   << Q_low.front()->first << endl;
		      S.insert( Q_low.front() );
// 		      ++nb_put_in_S;
		    }
		  Q_low.pop_front();
		}
// 	      cout << "[CC::collapse]-------> " << nb_put_in_S << "/"
// 		   << nb_low_1 << " cells added to S, "
// 		   << "Card(S)=" << S.size() << endl;
	    }
	  else
	    Q_low.clear();
	  
	} // while ( ! PQ.empty() )
    } // while ( ! S.empty() ) 

  // Now clean the complex so that removed cells are effectively
  // removed and no more cell is tagged as collapsible.
  for ( deque< iterator >::iterator itc = Q_collapsible.begin();
	itc != Q_collapsible.end();
	++itc )
    {
      if ( (*itc)->second.isFlagged( CubicalCellData::REMOVED ) )
	K.m_cells.erase( *itc );
      else
	(*itc)->second.resetFlag( CubicalCellData::COLLAPSIBLE );
    }
}


/**
 * Collapse a subcomplex of the closed cubical complex [K], collapsing
 * cells following priority [priority], in a decreasing sequence. [S]
 * provides the starting cells, generally (but not compulsory) maximal
 * cells.  Note: cells that have been marked as fixed are not removed.
 * only cells that are in the closure [S] may be removed, and only if
 * they are not marked as fixed.
 *
 * @param K the closed cubical complex to collapse.
 * @param S the set of cells of [K], as iterators in [K]
 * @param S_closed indicates if [S] is a closed set (faster in this case)
 * @param priority the object that assign a priority to each cell.
 */
void
ImaGene::CubicalComplex::collapse2( CubicalComplex & K,
				   cc_cell_set & S,
				   bool S_closed,
				   const CubicalCellPriority & priority )
{
  // NB : a maximal k-cell is collapsible if it has a free incident k-1-cell.
  uint dim_s = K.dim();

  deque< CubicalCell > Q;
  deque< iterator > Q_up;
  deque< iterator > Q_low;
  deque< iterator > Q_collapsible;
  iterator it_c;
  deque< iterator >::const_iterator itlow;
  deque< iterator >::const_iterator itqup;

  cout << "[CC::collapse]-+ tag collapsible elements... " << flush;
  // Restricts the set of elements that are collapsible.
  if ( S_closed )
    for ( cc_cell_set::const_iterator it = S.begin();
	  it != S.end();
	  ++it )
      {
	it_c = *it;
	CubicalCellData & ccdata = it_c->second;
	if ( ! ( ccdata.isFlagged( CubicalCellData::FIXED ) 
		 || ccdata.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
	  {
	    ccdata.setFlag( CubicalCellData::COLLAPSIBLE );
	    Q_collapsible.push_back( it_c );
	  }
      }
  else // not ( S_closed )
    for ( cc_cell_set::const_iterator it = S.begin();
	  it != S.end();
	  ++it )
      {
	CubicalCell::lowIncidentCells( Q, (*it)->first, 
				       (*it)->first.dim( dim_s ), 
				       0, dim_s, dim_s );
	while ( ! Q.empty() )
	  {
	    it_c = K.find( Q.front() );
	    CubicalCellData & ccdata = it_c->second;
	    if ( ! ( ccdata.isFlagged( CubicalCellData::FIXED ) 
		     || ccdata.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
	      {
		ccdata.setFlag( CubicalCellData::COLLAPSIBLE );
		Q_collapsible.push_back( it_c );
	      }
	    Q.pop_front();
	  }
      }
  cout << " " << Q_collapsible.size() << " found." << endl;

  // Fill queue
  priority_queue
    < iterator, vector<iterator>, CubicalCellPriority > PQ( priority );

  
  cout << "[CC::collapse]-+ entering collapsing loop. " << endl;
  while ( ! S.empty() ) 
    {
      cout << "[CC::collapse]---+ filling priority queue with " << endl;
      // push elements in the priority queue.
      for ( cc_cell_set::iterator it = S.begin();
	    it != S.end();
	    ++it )
	{
	  PQ.push( *it );
	  cout << "[CC::collapse]-----+ " << (*it)->first
	       << endl;
	}
      cout << "[CC::collapse]-----+ =" << PQ.size() << " elements." << endl;
      S.clear();

      cout << "[CC::collapse]---+ collapsing according to priority queue."
	   << endl;
      // Try to collapse elements.
      while ( ! PQ.empty() )
	{
	  // Get top element.
	  iterator itcur = PQ.top();
	  PQ.pop();
	  const CubicalCellData & cur_data = itcur->second;
	  cout << "[CC::collapse]-----+ " << itcur->first << endl;

	  // Check if the cell is removable
	  if ( cur_data.isFlagged( CubicalCellData::REMOVED ) 
	       || ( ! cur_data.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
	    continue;

	  cout << "[CC::collapse]-----+ cell is removable." << endl;

	  // Check if it is a maximal cell
	  const CubicalCell & cur_c = itcur->first;
	  uint d = cur_c.dim( dim_s );
	  if ( d < dim_s )
	    K.upIncidentCells1( cur_c, Q_up );

	  // Filtering unwanted up-incident cells.
	  uint nb = 0;
	  iterator itup;
	  for ( itqup = Q_up.begin();
		itqup != Q_up.end();
		++itqup )
	    {
	      if ( ! (*itqup)->second.isFlagged( CubicalCellData::COLLAPSIBLE ) ) 
		break;
	      if ( ! (*itqup)->second.isFlagged( CubicalCellData::REMOVED ) )
		{
		  ++nb;
		  if ( nb > 1 ) break;
		  itup = *itqup;
		}
	    }
	  
	  if ( itqup == Q_up.end() )
	    {
	      if ( nb == 0 )
		{ // maximal cell
		  cout << "[CC::collapse]-----+ cell is maximal." << endl;
		  K.lowIncidentCells1( cur_c, Q_low );
		  iterator low_ic;
		  for ( itlow = Q_low.begin();
			itlow != Q_low.end();
			++itlow )
		    {
		      low_ic = *itlow;
		      if ( low_ic->second.isFlagged( CubicalCellData::REMOVED ) 
			   || ( ! low_ic->second.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
			continue;
		      K.upIncidentCells1( low_ic->first, Q_up );
		      uint nb = 0;
		      for ( ; ! Q_up.empty(); Q_up.pop_front() )
			if ( ! Q_up.front()->second.isFlagged( CubicalCellData::REMOVED ) )
			  ++nb;
		      if ( nb == 1 )
			{ // found a free n-1-face ic
			  // delete c and ic.
			  cout << "[CC::collapse]-----+ found free n-1-face." << endl;
			  cout << "[CC::collapse]-----+ removing " << itcur->first
			       << endl;
			  cout << "[CC::collapse]-----+ removing " << low_ic->first
			       << endl;
			  itcur->second.setFlag( CubicalCellData::REMOVED );
			  low_ic->second.setFlag( CubicalCellData::REMOVED );
			  break;
			}
		    }
		  uint nb_put_in_S = 0;
		  uint nb_low_1 = Q_low.size();
		  if ( itlow != Q_low.end() )
		    { // the face was deleted. 
		      // Incident cells have to be checked again.
		      // Cells 1-low-incident to free face should be
		      // checked again.
		      K.lowIncidentCells1( low_ic->first, Q_low );
		      nb_low_1 = Q_low.size();
		      while ( ! Q_low.empty() )
			{
			  const CubicalCellData & data_qlow = 
			    Q_low.front()->second;
			  if ( ( ! data_qlow.isFlagged( CubicalCellData::REMOVED ) )
			       && data_qlow.isFlagged( CubicalCellData::COLLAPSIBLE ) )
			    {
			      cout << "[CC::collapse]-------+ S += " 
				   << Q_low.front()->first << endl;
			      S.insert( Q_low.front() );
			      ++nb_put_in_S;
			    }
			  Q_low.pop_front();
			}
		      // cout << "[CC::collapse]-------+ TODO, should add incident cells to free face." << endl;
		    }
		  else
		    Q_low.clear();
		  cout << "[CC::collapse]-----+ " << nb_put_in_S << "/"
		       << nb_low_1 << " cells put again in S." << endl;
		  cout << "[CC::collapse]-----+ Card(S)=" << S.size() << endl;
		}
	      else if ( nb == 1 )
		{ // check if is the face of a maximal cell.
		  cout << "[CC::collapse]-----+ cell is a face of a maximal cell."
		       << endl;
// 		  if ( itqup == Q_up.end() )
// 		    { // found a free n-1-face c
// 		      // delete c and itup_cell.
// 		      itcur->second.setFlag( CubicalCellData::REMOVED );
// 		      itup->second.setFlag( CubicalCellData::REMOVED );
// 		      //  Incident cells have to be checked again.
// 		      K.lowIncidentCells1( itup->first, Q_low );
// 		      while ( ! Q_low.empty() )
// 			{
// 			  const CubicalCellData & data_qlow = 
// 			    Q_low.front()->second;
// 			  if ( ( ! data_qlow.isFlagged( CubicalCellData::REMOVED ) )
// 			       && ( data_qlow.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
// 			    S.insert( Q_low.front() );
// 			  Q_low.pop_front();
// 			}
// 		    }
		}
	      else
		cout << "[CC::collapse]-----+ cell has more than " << nb << " 1-up-incident cells." << endl;
	    } // if 
	  else // not ( itqup == Q_up.end() )
	    {
	      cout << "[CC::collapse]-----+ cell touches non collapsible cell "
		   << (*itqup)->first << endl;
	    }
	  Q_up.clear();

	} // while ( ! PQ.empty() )
    } // while ( ! S.empty() ) 

  // Now clean the complex so that removed cells are effectively
  // removed and no more cell is tagged as collapsible.
  for ( deque< iterator >::iterator itc = Q_collapsible.begin();
	itc != Q_collapsible.end();
	++itc )
    {
      if ( (*itc)->second.isFlagged( CubicalCellData::REMOVED ) )
	K.m_cells.erase( *itc );
      else
	(*itc)->second.resetFlag( CubicalCellData::COLLAPSIBLE );
    }
}



/**
 * Builds a coarse version of [Kfine] whose cells are within
 * [cmin]-[cmax] such that cells outside these bounds are
 * discarded, and cells within these bounds have their coarser
 * bounding cell included.
 *
 * @param Kcoarse (returns) a coarse version of [Kfine]
 * @param Kfine the fine complex to project.
 * @param cmin the lowest cell that can be in [Kcoarse].
 * @param cmin the greatest cell that can be in [Kcoarse].
 * @param data each cell in [Kcoarse] has this data.
 */
void
ImaGene::CubicalComplex::project( CubicalComplex & Kcoarse,
				  const CubicalComplex & Kfine,
				  const CubicalCell & cmin,
				  const CubicalCell & cmax, 
				  const CubicalCellData & data )
{
  uint i;
  uint D = Kcoarse.dim();
  ASSERT_CubicalComplex( Kcoarse.dim() == Kfine.dim() );
  for ( CubicalComplex::const_iterator it = Kfine.begin();
	it != Kfine.end();
	++it )
    {
      CubicalCell c( it->first );
      for ( i = 0; i < D; ++i )
	{
	  while ( c[ i ] > cmax[ i ] )
	    c[ i ] >>= 1;
	  if ( c[ i ] < cmin[ i ] )
	    break;
	}
      if ( i == D ) // ok, within bounds
	{
	  CubicalComplex::iterator itcc = Kcoarse.find( c );
	  if ( itcc != Kcoarse.end() )
	    itcc->second = data;
	  //Kcoarse.add( c, data );
	}
    }
}




///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::CubicalComplex::~CubicalComplex()
{
}

/**
 * Creates an empty cubical complex lying in a space of dimension [dim].
 *
 * @param dim the dimension of the space.
 */
ImaGene::CubicalComplex::CubicalComplex( uint dim )
  : m_dim( dim ),
    m_cells( CubicalCellInvComparator( dim ) )
{
}


/**
 * Adds the cell [c] and its closure to the complex.
 *
 * @param c a cell lying in the adequate space.
 * @param d an optional data to associate with all cells in closure([c]).
 * @return an iterator on the cell in the subcomplex.
 */
ImaGene::CubicalComplex::iterator
ImaGene::CubicalComplex::addClosure( const CubicalCell & c,
				     const CubicalCellData & d )
{
  deque<CubicalCell> q;
  uint dim_c = c.dim();
  if ( dim_c > 0 )
    CubicalCell::lowIncidentCells( q, c, dim_c, 0, dim_c-1, this->dim() );

  iterator it = m_cells.lower_bound( c );
  if ( ( it == end() ) || ( c.invLess( it->first, m_dim ) ) )
    // it->first is greater than or equivalent to c.
    it = m_cells.insert( it, std::make_pair( c, d ) );
  else
    it->second = d;

  for ( deque<CubicalCell>::const_iterator it_q = q.begin();
	it_q != q.end();
	++it_q )
    {
      this->add( *it_q, d );
    }
  return it;

}

/**
 * Adds the cell [c] and its closure to the complex. Stays within bounds.
 *
 * @param c a cell lying in the adequate space.
 * @param d an optional data to associate with all cells in closure([c]).
 * @param cmin the coordinates of the lowest admissible cell.
 * @param cmax the coordinates of the highest admissible cell.
 * @return an iterator on the cell in the subcomplex.
 */
ImaGene::CubicalComplex::iterator
ImaGene::CubicalComplex::addClosure( const CubicalCell & c,
				     const CubicalCell & cmin,
				     const CubicalCell & cmax,
				     const CubicalCellData & d )
{
  deque<CubicalCell> q;
  uint dim_c = c.dim();
  if ( dim_c > 0 )
    CubicalCell::lowIncidentCells( q, c, dim_c, 0, dim_c-1, this->dim(),
				   cmin, cmax );

  iterator it = m_cells.lower_bound( c );
  if ( ( it == end() ) || ( c.invLess( it->first, m_dim ) ) )
    // it->first is greater than or equivalent to c.
    it = m_cells.insert( it, std::make_pair( c, d ) );
  else
    it->second = d;

  for ( deque<CubicalCell>::const_iterator it_q = q.begin();
	it_q != q.end();
	++it_q )
    {
      this->add( *it_q, d );
    }
  return it;

}



///////////////////////////////////////////////////////////////////////////////
// ----------------------- Complex services ------------------------------

/**
 * Clear the complex.
 */
void
ImaGene::CubicalComplex::clear()
{
  m_cells.clear();
}


/**
 * Close the complex.
 */
void
ImaGene::CubicalComplex::close()
{
  deque<CubicalCell> closure_c;
  cc_cell_list cells_K;
  for ( iterator it = this->begin(); it != this->end(); ++it )
    cells_K.push_back( it );

  for ( cc_cell_list::const_iterator itc = cells_K.begin();
	itc != cells_K.end();
	++itc )
    {
      const CubicalCell & c = (*itc)->first;
      const CubicalCellData & d = (*itc)->second;
      uint dim_c = c.dim();
      if ( dim_c > 0 )
	CubicalCell::lowIncidentCells( closure_c, c, dim_c, 
				       0, dim_c - 1, this->dim() );
      for ( deque<CubicalCell>::const_iterator it_closure_c 
	      = closure_c.begin();
	    it_closure_c != closure_c.end();
	    ++it_closure_c )
	{
	  // the iterator on [c] is used as a hint to find where to insert.
	  iterator new_it = m_cells.lower_bound( *it_closure_c );
	  if ( ( new_it == this->end() ) 
	       || ( it_closure_c->invLess( new_it->first, this->dim() ) ) )
	    m_cells.insert( new_it, std::make_pair( *it_closure_c, d ) );
	}
      closure_c.clear();
    }

}


/**
 * Extracts the cells of 'this' that belongs to the parallepipedic
 * subspace [c1, c2]. Cells in ]c1,c2| are added to [inner_S],
 * others are added to [bdry_S]. Do not take care of hierarchical
 * complexes.
 *
 * @param inner_S (returns) the set of inner cells is added to it.
 * @param bdry_S (returns) the set of boundary cells is added to it.
 * @param c1 the lower bound of the subspace.
 * @param c2 the upper bound of the subspace.
 */
void
ImaGene::CubicalComplex::getCellsInSubspace( cc_cell_set & inner_S, 
					     cc_cell_set & bdry_S, 
					     const CubicalCell & c1,
					     const CubicalCell & c2 )
{
  uint d = this->dim();
  uint i;
  uint nb_equal;
  CubicalCell min( c1, d );
  CubicalCell max( c2, d );
  for ( i = 0; i < d; ++i )
    if ( min[ i ] > max[ i ] ) 
      std::swap( min[ i ], max[ i ] );
  iterator it = m_cells.lower_bound( min );
  iterator it_end = m_cells.upper_bound( max );

  cc_cell_set::iterator it_inner = inner_S.end();
  cc_cell_set::iterator it_bdry = bdry_S.end();
  if ( it == m_cells.end() ) return;
  for ( ; it != it_end; ++it )
    {
      const CubicalCell & c = it->first;
      nb_equal = 0;
      for ( i = 0; i < d; ++i )
	{
	  if ( ( min[ i ] > c[ i ] ) || ( c[ i ] > max[ i ] ) )
	    break;
	  else if ( ( min[ i ] == c[ i ] ) || ( max[ i ] == c[ i ] ) )
	    ++nb_equal;
	}
      if ( i == d )
	if ( nb_equal != 0 )
	  it_bdry = bdry_S.insert( it_bdry, it );
	else
	  it_inner = inner_S.insert( it_inner, it );
    }
}

/**
 * Extracts the cells of 'this' that belongs to the parallepipedic
 * subspace [c1, c2]. Cells in ]c1,c2| are added to [inner_S], others
 * are added to [bdry_S]. Pick only cells of maximal dimension (ie n
 * for inner_S and n-1 for bdry_S). Do not take care of hierarchical
 * complexes.
 *
 * @param inner_S (returns) the set of inner cells is added to it.
 * @param bdry_S (returns) the set of boundary cells is added to it.
 * @param c1 the lower bound of the subspace.
 * @param c2 the upper bound of the subspace.
 */
void
ImaGene::CubicalComplex::getMaximalCellsInSubspace( cc_cell_set & inner_S, 
						    cc_cell_set & bdry_S, 
						    const CubicalCell & c1,
						    const CubicalCell & c2 )
{
  uint d = this->dim();
  uint i;
  uint nb_equal;
  CubicalCell min( c1, d );
  CubicalCell max( c2, d );
  for ( i = 0; i < d; ++i )
    if ( min[ i ] > max[ i ] ) 
      std::swap( min[ i ], max[ i ] );
  iterator it = m_cells.lower_bound( min );
  iterator it_end = m_cells.upper_bound( max );

  cc_cell_set::iterator it_inner = inner_S.end();
  cc_cell_set::iterator it_bdry = bdry_S.end();
  if ( it == m_cells.end() ) return;
  for ( ; it != it_end; ++it )
    {
      const CubicalCell & c = it->first;
      nb_equal = 0;
      for ( i = 0; i < d; ++i )
	{
	  if ( ( min[ i ] > c[ i ] ) || ( c[ i ] > max[ i ] ) )
	    break;
	  else if ( ( min[ i ] == c[ i ] ) || ( max[ i ] == c[ i ] ) )
	    ++nb_equal;
	}
      if ( i == d )
	if ( ( nb_equal != 0 ) && ( c.dim( d ) == d - 1 ) )
	  it_bdry = bdry_S.insert( it_bdry, it );
	else if ( c.dim( d ) == d )
	  it_inner = inner_S.insert( it_inner, it );
    }
}




/**
 * Computes cells 1-low incident to [c] in the cubical complex and
 * adds them to the back of [low_cells].
 */
void 
ImaGene::CubicalComplex::lowIncidentCells1
( const CubicalCell & c, cc_cell_list & low_cells )
{
  deque<CubicalCell> q;
  c.lowIncidentCells1( q, this->dim() );
  for ( deque<CubicalCell>::const_iterator itq = q.begin();
	itq != q.end();
	++itq )
    {
      iterator it = this->find( *itq );
      if ( it != this->end() )
	low_cells.push_back( it );
    }
}

/**
 * Computes cells 1-up incident to [c] in the cubical complex and
 * adds them to the back of [up_cells].
 */
void
ImaGene::CubicalComplex::upIncidentCells1
( const CubicalCell & c, cc_cell_list & up_cells )
{
  deque<CubicalCell> q;
  c.upIncidentCells1( q, this->dim() );
  for ( deque<CubicalCell>::const_iterator itq = q.begin();
	itq != q.end();
	++itq )
    {
      iterator it = this->find( *itq );
      if ( it != this->end() )
	up_cells.push_back( it );
    }
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 * @param detail when 'true', displays cells, default is 'false'.
 */
void 
ImaGene::CubicalComplex::selfDisplay( ostream& that_stream, bool detail ) const
{
  that_stream << "[CubicalComplex, nb=" << m_cells.size();
  if ( detail )
    {
      cout << " cells=";
      for ( CubicalComplex::const_iterator itc = begin();
	    itc != end();
	    ++itc )
	{
	  that_stream << "{ " << itc->first << "->";
	  const CubicalCellData & ccd =  itc->second;
	  if ( ccd.isFlagged( CubicalCellData::FIXED ) )
	    that_stream << "F,";
	  if ( ccd.isFlagged( CubicalCellData::REMOVED ) )
	    that_stream << "R,";
	  if ( ccd.isFlagged( CubicalCellData::COLLAPSIBLE ) )
	    that_stream << "C,";
	  if ( ccd.isFlagged( CubicalCellData::USER1 ) )
	    that_stream << "1,";
	  that_stream << ccd.getValue() << " } ";
	}
    }
  that_stream << " ]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::CubicalComplex::OK() const
{
  return true;
}

/**
 * Writes the list of cells of the complex on [that_stream].
 * @param that_stream the output stream where the object is written.
 */
void
ImaGene::CubicalComplex::outputAsListOfCells( ostream & that_stream )
{
  that_stream << "# Cubical Complex" << endl
	      << "# dim nbcells" << endl
	      << "# <Khalimsky coordinates per cell> ..." << endl;
  uint d = dim();
  that_stream << d << " " << m_cells.size() << endl;
  for ( CubicalComplex::const_iterator itc = begin();
	itc != end();
	++itc )
    {
      const CubicalCell & c = itc->first;
      that_stream << c[ 0 ]; 
      for ( uint i = 1; i < d; ++i )
	{
	  that_stream << " " << c[ i ];
	}
      that_stream << endl;
    }
}



class MyCubicalCellPriorityComputer : public CubicalCellPriorityComputer
{
public:
  /**
   * Computes the priority of the cell [c], as specified by a
   * derived class. Default is 0.0 (no cell has a higher priority
   * than any other one).
   *
   * @param c some cell.
   * @param dim the dimension of the space in which cells are compared.
   */
  INLINE virtual float priority( const CubicalCell & c, uint dim ) const
  {
    float p = 0.0f;
    for ( uint i = 0; i < dim; ++i )
      if ( c[ i ] > p ) p = c[ i ];
    return -p;
  }
  
};


/**
 * Test method for the class.
 */
void
ImaGene::CubicalComplex::testCubicalComplex()
{
  CubicalComplex K( 3 );

  CubicalCell::default_dim = 3;
  CubicalCell c;
  uint x[ 3 ] = { 17, 12, 5 };
  c.init( x, 3 );
  CubicalCell d( c, 3 );
  d.incident( 2, true );
  d.incident( 1, true );
  CubicalCell u( 3, 3, 1 );
  CubicalCell u1( 1, 3, 3 );
  CubicalCell u2( 3, 1, 3 );
  CubicalCell u3( 5, 3, 3 );
  CubicalCell u4( 3, 5, 3 );
  CubicalCell u5( 3, 3, 5 );
  CubicalCell uc( 3, 3, 3 );
  CubicalCell ue1( 2, 2, 0 );
  CubicalCell ue2( 2, 2, 6 );
  CubicalCell v( 1, 1, 1 );  
  CubicalCell v1( 1, 0, 1 );
  CubicalCell v2( 2, 2, 1 );
  CubicalCell v3( 0, 2, 0 );
  CubicalCellInvComparator ccc( 3 );

  iterator it_u = K.add( u );
  K.add( u1 );
  K.add( u2 );
  // K.addClosure( u3 );
  K.add( u4 );
  K.add( u5 );
  K.close();

  std::deque<CubicalCell> voisins;
  //  CubicalCell::lowIncidentCells( voisins, v, v.dim( 3 ), 0, 3, 3 );
//   iterator it_u = K.addClosure( u );
//   K.addClosure( u1 );
//   K.addClosure( u2 );
//   // K.addClosure( u3 );
//   K.addClosure( u4 );
//   K.addClosure( u5 );
//   // K.addClosure( uc );
//   CubicalCell::lowIncidentCells( voisins, u, u.dim( 3 ), 0, 3, 3 );
//   CubicalCell::lowIncidentCells( voisins, u1, u1.dim( 3 ), 0, 3, 3 );
//   CubicalCell::lowIncidentCells( voisins, u2, u2.dim( 3 ), 0, 3, 3 );
//   CubicalCell::lowIncidentCells( voisins, u3, u3.dim( 3 ), 0, 3, 3 );
//   CubicalCell::lowIncidentCells( voisins, u4, u4.dim( 3 ), 0, 3, 3 );
//   CubicalCell::lowIncidentCells( voisins, u5, u5.dim( 3 ), 0, 3, 3 );
  //CubicalCell::lowIncidentCells( voisins, uc, uc.dim( 3 ), 0, 3, 3 );
  cout << endl;
//   for ( std::deque<CubicalCell>::const_iterator it2 = voisins.begin();
// 	it2 != voisins.end();
// 	++it2 )
//     {
//       K.add( *it2 );
//     }
  
  cout << "K=" << K << endl;

  // test collapse
  CubicalComplex::cc_cell_set S( ccc );
  S.insert( it_u ); // K.find( u ) ); // it_u
  S.insert( K.find( u1 ) );
  S.insert( K.find( u2 ) );
  // S.insert( K.find( u3 ) );
  S.insert( K.find( u4 ) );
  S.insert( K.find( u5 ) );
  //S.insert( K.find( uc ) );
  //  S.insert( K.find( v ) );

  // Fix some cells.
  //(K.find( v1 ))->second.setFlag( CubicalCellData::FIXED );
  //(K.find( v2 ))->second.setFlag( CubicalCellData::FIXED );
  //(K.find( v3 ))->second.setFlag( CubicalCellData::FIXED );
  (K.find( ue1 ))->second.setFlag( CubicalCellData::FIXED );
  (K.find( ue2 ))->second.setFlag( CubicalCellData::FIXED );
//   (K.find( u1 ))->second.setFlag( CubicalCellData::FIXED );
//   (K.find( u2 ))->second.setFlag( CubicalCellData::FIXED );

  MyCubicalCellPriorityComputer ccp_computer;
  CubicalCellPriority ccp( &ccp_computer, 3, false );
  CubicalComplex::collapse( K, S, false, ccp );

  cout << "----------------- after collapse --------------------" << endl;
  cout << "K'=";
  K.selfDisplay( cout, true );
  cout << endl;

  cout << "----------------- test subspace --------------------" << endl;
  CubicalComplex::cc_cell_set iS( ccc );
  CubicalComplex::cc_cell_set bS( ccc );

  CubicalCell x1( 1, 1, 1 );
  CubicalCell x2( 5, 5, 5 );
  K.getCellsInSubspace( iS, bS, x1, x2 );
  cout << "---- cells inside " << x1 << " - " << x2 << endl;
  for ( cc_cell_set::const_iterator it = iS.begin(); it != iS.end(); ++it )
    cout << " " << (*it)->first;
  cout << endl;
  cout << "---- cells on bdry " << x1 << " - " << x2 << endl;
  for ( cc_cell_set::const_iterator it = bS.begin(); it != bS.end(); ++it )
    cout << " " << (*it)->first;
  cout << endl;
}


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

/**
 * Given a cell [c], tells if it is a maximal cell in the complex
 * (return 0), or if it is a free face of the cell pointed by
 * [it_cell_up] (return 1) or if it is not a free face.
 *
 * The complex must be closed. In computing the 1-up-incident
 * cells, this method ignores cell marked as REMOVED. Furthermore,
 * if one 1-up-incident cell is not marked as COLLAPSIBLE, the
 * method returns 2.
 *
 * @param c a cubical cell (belonging to 'this')
 *
 * @param it_cell_up (returns) a pointer on a cell d if c is a
 * free face of d.
 *
 * @return 0 if the cell is maximal, 1 if the cell is a free face,
 * 2 otherwise.
 */
uint
ImaGene::CubicalComplex::computeCellType
( const CubicalCell & c, 
  iterator & it_cell_up )
{
  // Check if it is a maximal cell
  uint d = c.dim( this->dim() );
  if ( d == this->dim() )
    return 0;
  
  deque< iterator > Q_up;
  this->upIncidentCells1( c, Q_up );
  
  // Filtering unwanted up-incident cells.
  uint nb = 0;
  deque< iterator >::const_iterator itqup;
  for ( itqup = Q_up.begin();
	itqup != Q_up.end();
	++itqup )
    {
      if ( ! (*itqup)->second.isFlagged( CubicalCellData::COLLAPSIBLE ) )
	return 2;
      if ( ! (*itqup)->second.isFlagged( CubicalCellData::REMOVED ) )
	{
	  ++nb;
	  if ( nb > 1 ) return 2;
	  it_cell_up = *itqup;
	}
    }
  return nb;
}

//                                                                           //
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// class HierarchicalCubicalComplex
///////////////////////////////////////////////////////////////////////////////


/**
 * Destructor. 
 */
ImaGene::HierarchicalCubicalComplex::~HierarchicalCubicalComplex()
{
}

/**
 * Creates an empty hierarchical cubical complex lying in a space of
 * dimension [dim].
 *
 * @param dim the dimension of the space.
 */
ImaGene::HierarchicalCubicalComplex::HierarchicalCubicalComplex( uint dim )
  : CubicalComplex( dim )
{
}

/**
 * Close the complex.
 */
void
ImaGene::HierarchicalCubicalComplex::close()
{
  cout << "+- closing complex... " << endl;
  deque<CubicalCell> closure_c;
  cc_cell_list cells_K;
  for ( iterator it = this->begin(); it != this->end(); ++it )
    cells_K.push_back( it );

  CubicalComplex::iterator itcoarser;
  for ( cc_cell_list::const_iterator itc = cells_K.begin();
	itc != cells_K.end();
	++itc )
    {
      const CubicalCell & c = (*itc)->first;
      // cout << " " << c;
      if ( this->findStrictCoarser( itcoarser, c ) > 0 )
	{
	  // Is already in hierarchical complex, remove it.
	  cout << " (" << c << " already in K)";
	  m_cells.erase( *itc );
	  continue;
	}
      const CubicalCellData & d = (*itc)->second;
      uint dim_c = c.dim( this->dim() );
      if ( dim_c > 0 )
	CubicalCell::strictLowerBoundary( closure_c, c, dim_c, this->dim() );
      for ( deque<CubicalCell>::const_iterator it_closure_c 
	      = closure_c.begin();
	    it_closure_c != closure_c.end();
	    ++it_closure_c )
	{
	  this->add( *it_closure_c, d );
	}
      closure_c.clear();
    }
  cout << "ended" << endl;
}

/**
 * Computes the direct incidence maps between cells of different
 * sizes. The principle is to find for each cell if it has coarser
 * incident cells and store them both in maps [coarse_*] and [fine_*].
 *
 * @param coarse_low the mapping associating to a cell the coarser direct
 * low incident cells.
 *
 * @param coarse_up the mapping associating to a cell the coarser direct
 * up incident cells.
 *
 * @param fine_low the mapping associating to a cell the finer direct
 * low incident cells.
 *
 * @param fine_up the mapping associating to a cell the finer direct
 * up incident cells.
 *
 */
void
ImaGene::HierarchicalCubicalComplex::computeDirectIncidences
( cc_incidence_map_t & coarse_low,
  cc_incidence_map_t & coarse_up,
  cc_incidence_map_t & fine_low,
  cc_incidence_map_t & fine_up )
{
  CubicalComplex::iterator it_coarse;
  deque<CubicalCell> Q_inc_cells;
  for ( CubicalComplex::iterator it_cur = this->begin(); 
	it_cur != this->end(); 
	++it_cur )
    {
      const CubicalCell & c_cur = it_cur->first;
      uint dim_c = c_cur.dim();
      // Take care of low incidence.
      if ( dim_c > 0 )
	c_cur.lowIncidentCells1( Q_inc_cells, this->dim() );
      for ( deque<CubicalCell>::const_iterator it_inc_cells 
	      = Q_inc_cells.begin();
	    it_inc_cells != Q_inc_cells.end();
	    ++it_inc_cells )
	{
	  // test if their is a coarser version of this incident cell
	  if ( findStrictCoarser( it_coarse, *it_inc_cells ) > 0 )
	    {
	      coarse_low[ it_cur ].push_back( it_coarse );
	      fine_up[ it_coarse ].push_back( it_cur );
	    }
	}
      Q_inc_cells.clear();
      // Take care of up incidence.
      if ( dim_c < this->dim() )
	c_cur.upIncidentCells1( Q_inc_cells, this->dim() );
      for ( deque<CubicalCell>::const_iterator it_inc_cells 
	      = Q_inc_cells.begin();
	    it_inc_cells != Q_inc_cells.end();
	    ++it_inc_cells )
	{
	  // test if their is a coarser version of this incident cell
	  if ( findStrictCoarser( it_coarse, *it_inc_cells ) > 0 )
	    {
	      coarse_up[ it_cur ].push_back( it_coarse );
	      fine_low[ it_coarse ].push_back( it_cur );
	    }
	}
      Q_inc_cells.clear();

    }

}


/**
 * Finds the cell [c] in the complex or the first coarser version of [c].
 *
 * @param it (returns) an iterator on the cell [c] or the first of its
 * coarser version in the subcomplex, .
 * 
 * @param c a cell lying in the adequate space.
 *
 * @return 0 if [c] was found ([it] points to [c]), a positive number
 * (the size difference) if a coarser version of [c] was found
 * (pointed by [it]), -1 if not found ([it] points to the insertion
 * point for [c] in the complex).
 */
int
ImaGene::HierarchicalCubicalComplex::findCoarser
( CubicalComplex::iterator & it, const CubicalCell & c )
{
  
  it = m_cells.lower_bound( c );

  // cout << "+- [findCoarse of " << c;
  if ( ( it == end() ) || ( c.invLess( it->first, m_dim ) ) )
    // it->first is greater than or equivalent to c.
    {
      // checks that no cell contains c
      CubicalCell c_coarse( c, m_dim );
      c_coarse.toCoarse( m_dim );
      iterator it2;
      uint k = 1;
      while ( c_coarse.isValid( m_dim ) )
	{
	  // cout << ", " << c_coarse;
	  it2 = m_cells.lower_bound( c_coarse );
	  if ( ( it2 != end() ) 
	       && ( ! c_coarse.invLess( it2->first, m_dim ) ) )
	    { // found
	      it = it2;
	      // cout << " found k=" << k << "]" << endl;
	      return k;
	    }
	  if ( it2 == begin() ) break;
	  c_coarse.toCoarse( m_dim );
	  ++k;
	}
      // cout << " not found]" << endl;
      return -1;
    }
  // cout << " already in K]" << endl;
  return 0;
}

/**
 * Finds the first coarser version of [c].
 *
 * @param it (returns) an iterator on the first of its coarser version
 * in the subcomplex.
 * 
 * @param c a cell lying in the adequate space.
 *
 * @return a positive number (the size difference) if a coarser
 * version of [c] was found (pointed by [it]), -1 if not found ([it]
 * points to the insertion point for [c] in the complex).
 */
int
ImaGene::HierarchicalCubicalComplex::findStrictCoarser
( CubicalComplex::iterator & it, const CubicalCell & c )
{
  // checks that no cell contains c
  CubicalCell c_coarse( c, m_dim );
  c_coarse.toCoarse( m_dim );
  iterator it2;
  uint k = 1;
  // cout << "+- [findStrictCoarser of " << c;
  while ( c_coarse.isValid( m_dim ) )
    {
      // cout << ", " << c_coarse;
      it2 = m_cells.lower_bound( c_coarse );
      if ( ( it2 != end() ) 
	   && ( ! c_coarse.invLess( it2->first, m_dim ) ) )
	{ // found
	  it = it2;
	  // cout << " found k=" << k << "]" << endl;
	  return k;
	}
      if ( it2 == begin() ) break;
      c_coarse.toCoarse( m_dim );
      ++k;
    }
  // cout << " not found]" << endl;
  return -1;
}


/**
 * Adds the cell [c] to the complex.  Overriden. Note that the
 * cell is \b not added if it is included in some other cell. In
 * this case, the returned iterator points on this container cell.
 *
 * @param c a cell lying in the adequate space.
 * @param d an optional data to associate with [c].
 * @return an iterator on the cell in the subcomplex.
 */
ImaGene::CubicalComplex::iterator
ImaGene::HierarchicalCubicalComplex::add
( const CubicalCell & c,
  const CubicalCellData & d )
{
  CubicalComplex::iterator it;
  int res = findCoarser( it, c );
  if ( res > 0 ) return it;
  else if ( res < 0 ) 
    return m_cells.insert( it, std::make_pair( c, d ) );
  else 
    {
      it->second = d;
      return it;
    }
}


/**
 * Adds the cell [c] and its closure to the
 * complex. Overriden. Note that the cell is \b not added if it is
 * included in some other cell. In this case, the returned
 * iterator points on this container cell.
 *
 * @param c a cell lying in the adequate space.
 * @param d an optional data to associate with all cells in closure([c]).
 * @return an iterator on the cell in the subcomplex.
 */
ImaGene::CubicalComplex::iterator 
ImaGene::HierarchicalCubicalComplex::addClosure
( const CubicalCell & c,
  const CubicalCellData & d )
{
  deque<CubicalCell> q;
  uint dim_c = c.dim();
  if ( dim_c > 0 )
    CubicalCell::lowIncidentCells( q, c, dim_c, 0, dim_c-1, this->dim() );

  iterator it = this->add( c, d );
  for ( deque<CubicalCell>::const_iterator it_q = q.begin();
	it_q != q.end();
	++it_q )
    {
      this->add( *it_q, d );
    }
  return it;

}

/**
 * Adds the cell [c] and its closure to the complex. Stays within
 * bounds. Overriden. Note that the cell is \b not added if it is
 * included in some other cell. In this case, the returned
 * iterator points on this container cell.
 *
 * @param c a cell lying in the adequate space.
 * @param d an optional data to associate with all cells in closure([c]).
 * @param cmin the coordinates of the lowest admissible cell.
 * @param cmax the coordinates of the highest admissible cell.
 * @return an iterator on the cell in the subcomplex.
 */
ImaGene::CubicalComplex::iterator
ImaGene::HierarchicalCubicalComplex::addClosure
( const CubicalCell & c,
  const CubicalCell & cmin,
  const CubicalCell & cmax,
  const CubicalCellData & d )
{
  deque<CubicalCell> q;
  uint dim_c = c.dim();
  if ( dim_c > 0 )
    CubicalCell::lowIncidentCells( q, c, dim_c, 0, dim_c-1, this->dim(),
				   cmin, cmax );

  iterator it = this->add( c, d );
  for ( deque<CubicalCell>::const_iterator it_q = q.begin();
	it_q != q.end();
	++it_q )
    {
      this->add( *it_q, d );
    }
  return it;

}


// ------------------------- Internals ------------------------------------

/**
 * Given a cell pointed by [itc], tells if it is a maximal cell in the complex
 * (return 0), or if it is a free face of the cell pointed by
 * [it_cell_up] (return 1) or if it is not a free face.
 *
 * The complex must be closed. In computing the 1-up-incident
 * cells, this method ignores cell marked as REMOVED. Furthermore,
 * if one 1-up-incident cell is not marked as COLLAPSIBLE, the
 * method returns 2.
 *
 * @todo Prove that this algorithm is correct.
 *
 * @param it_cell_up (returns) a pointer on a cell d if c is a
 * free face of d.
 *
 * @param itc an iterator on a cubical cell (belonging to 'this')
 *
 * @param coarse_up the mapping associating to a cell the coarser direct
 * up incident cells.
 *
 * @param fine_up the mapping associating to a cell the finer direct
 * up incident cells.
 *
 * @return 0 if the cell is maximal, 1 if the cell is a free face,
 * 2 otherwise.
 */
uint
ImaGene::HierarchicalCubicalComplex::computeCellType
( CubicalComplex::iterator & it_cell_up,
  const CubicalComplex::iterator & itc,
  const cc_incidence_map_t & coarse_up,
  const cc_incidence_map_t & fine_up  )
{
  // Check if it is a maximal cell
  const CubicalCell & c = itc->first;
  uint d = c.dim( this->dim() );
  if ( d == this->dim() )
    return 0;
  
  deque< iterator > Q_up;
  this->upIncidentCells1( Q_up, itc, coarse_up, fine_up );
  
  // Filtering unwanted up-incident cells.
  uint nb = 0;
  deque< iterator >::const_iterator itqup;
  for ( itqup = Q_up.begin();
	itqup != Q_up.end();
	++itqup )
    {
      if ( ! (*itqup)->second.isFlagged( CubicalCellData::COLLAPSIBLE ) )
	return 2;
      if ( ! (*itqup)->second.isFlagged( CubicalCellData::REMOVED ) )
	{
	  ++nb;
	  if ( nb > 1 ) return 2;
	  it_cell_up = *itqup;
	}
    }
  // JOL : TODO prove that it is correct.
  if ( ( nb != 0 ) && ( it_cell_up->first.dim( this->dim() ) 
			!= ( itc->first.dim( this->dim() ) + 1 ) ) )
    return 2;
  return nb;
}


/**
 * Computes cells 1-low incident to the cell pointed by [itc] in the
 * cubical complex and adds them to the back of [low_cells]. All
 * direct low incident cells of any sizes are extracted, using the
 * mapping [coarse_low] and [fine_low], which should have been
 * computed beforehand.
 *
 * @param low_cells (returns) the list of low incident cells.
 *
 * @param itc an iterator on a given cell of 'this'.
 *
 * @param coarse_low the mapping associating to a cell the coarser direct
 * low incident cells.
 *
 * @param fine_low the mapping associating to a cell the finer direct
 * low incident cells.
 *
 * @see computeDirectLowIncidences
 */
void 
ImaGene::HierarchicalCubicalComplex::lowIncidentCells1
( cc_cell_list & low_cells, 
  const CubicalComplex::iterator & itc, 
  const cc_incidence_map_t & coarse_low,
  const cc_incidence_map_t & fine_low )
{
  cc_incidence_map_t::const_iterator it_low_cells;
  const CubicalCell & c = itc->first;
  // Check for low incident cell with same size.
  deque<CubicalCell> q;
  c.lowIncidentCells1( q, this->dim() );
  for ( deque<CubicalCell>::const_iterator itq = q.begin();
	itq != q.end();
	++itq )
    {
      CubicalComplex::iterator it = this->find( *itq );
      if ( it != this->end() )
	low_cells.push_back( it );
    }
  // Check for coarse low incident cells.
  it_low_cells = coarse_low.find( itc );
  if ( it_low_cells != coarse_low.end() )
    {
      const list<CubicalComplex::iterator> & list_cells = it_low_cells->second;
      for ( list<CubicalComplex::iterator>::const_iterator it 
	      = list_cells.begin();
	    it != list_cells.end();
	    ++it )
	{
	  low_cells.push_back( *it );
	}
    }
  // Check for fine low incident cells.
  it_low_cells = fine_low.find( itc );
  if ( it_low_cells != fine_low.end() )
    {
      const list<CubicalComplex::iterator> & list_cells = it_low_cells->second;
      for ( list<CubicalComplex::iterator>::const_iterator it 
	      = list_cells.begin();
	    it != list_cells.end();
	    ++it )
	{
	  low_cells.push_back( *it );
	}
    }

}

/**
 * Computes cells 1-up incident to the cell pointed by [itc] in the
 * cubical complex and adds them to the back of [low_cells]. All
 * direct up incident cells of any sizes are extracted, using the
 * mapping [coarse_up] and [fine_up], which should have been
 * computed beforehand.
 *
 * @param up_cells (returns) the list of up incident cells.
 *
 * @param itc an iterator on a given cell of 'this'.
 *
 * @param coarse_up the mapping associating to a cell the coarser direct
 * up incident cells.
 *
 * @param fine_up the mapping associating to a cell the finer direct
 * up incident cells.
 *
 * @see computeDirectIncidences
 */
void
ImaGene::HierarchicalCubicalComplex::upIncidentCells1
( cc_cell_list & up_cells, 
  const CubicalComplex::iterator & itc, 
  const cc_incidence_map_t & coarse_up,
  const cc_incidence_map_t & fine_up )
{
  cc_incidence_map_t::const_iterator it_up_cells;
  const CubicalCell & c = itc->first;
  // Check for up incident cell with same size.
  deque<CubicalCell> q;
  c.upIncidentCells1( q, this->dim() );
  for ( deque<CubicalCell>::const_iterator itq = q.begin();
	itq != q.end();
	++itq )
    {
      CubicalComplex::iterator it = this->find( *itq );
      if ( it != this->end() )
	up_cells.push_back( it );
    }
  // Check for coarse up incident cells.
  it_up_cells = coarse_up.find( itc );
  if ( it_up_cells != coarse_up.end() )
    {
      const list<CubicalComplex::iterator> & list_cells = it_up_cells->second;
      for ( list<CubicalComplex::iterator>::const_iterator it 
	      = list_cells.begin();
	    it != list_cells.end();
	    ++it )
	{
	  up_cells.push_back( *it );
	}
    }
  // Check for fine up incident cells.
  it_up_cells = fine_up.find( itc );
  if ( it_up_cells != fine_up.end() )
    {
      const list<CubicalComplex::iterator> & list_cells = it_up_cells->second;
      for ( list<CubicalComplex::iterator>::const_iterator it 
	      = list_cells.begin();
	    it != list_cells.end();
	    ++it )
	{
	  up_cells.push_back( *it );
	}
    }

}



/**
 * Collapse a subcomplex of the closed cubical complex [K],
 * collapsing cells following priority [priority], in a decreasing
 * sequence. [S] provides the starting cells, generally (but not
 * compulsory) maximal cells.  Note: cells that have been marked
 * as fixed are not removed.  only cells that are in the closure
 * of [S] may be removed, and only if they are not marked as
 * fixed.
 *
 * @param K the closed cubical complex to collapse.
 * @param S a subset of cells of [K], as iterators in [K]
 * @param S_closed indicates if [S] is a closed set (faster in this case)
 * @param priority the object that assign a priority to each cell.
 */
void
ImaGene::HierarchicalCubicalComplex::collapse
( HierarchicalCubicalComplex & K,
  CubicalComplex::cc_cell_set & S,
  bool S_closed,
  const CubicalCellPriority & priority )
{
  // NB : a maximal k-cell is collapsible if it has a free incident k-1-cell.
  uint dim_s = K.dim();

  deque< CubicalCell > Q;
  //deque< iterator > Q_up;
  deque< iterator > Q_low;
  deque< iterator > Q_collapsible;
  iterator it_c;
  deque< iterator >::const_iterator itlow;
  deque< iterator >::const_iterator itqup;

  cout << "[CC::collapse]-+ extract incidence maps... " << flush;
  CubicalCellInvComparator ccic( K.dim() );
  cc_incidence_map_t coarse_low( ccic );
  cc_incidence_map_t coarse_up( ccic );
  cc_incidence_map_t fine_low( ccic );
  cc_incidence_map_t fine_up( ccic );
  K.computeDirectIncidences( coarse_low, coarse_up,
			     fine_low, fine_up );
  cout << endl
       << "                 |clow|=" << coarse_low.size()
       << " |cup|=" << coarse_up.size() << endl;
  cout << "                 |flow|=" << fine_low.size()
       << " |fup|=" << fine_up.size() << endl;
    //cout << "ended." << endl;

  cout << "[CC::collapse]-+ tag collapsible elements... " << flush;
  // Restricts the set of elements that are collapsible.
  if ( S_closed )
    for ( cc_cell_set::const_iterator it = S.begin();
	  it != S.end();
	  ++it )
      {
	it_c = *it;
	CubicalCellData & ccdata = it_c->second;
	if ( ! ( ccdata.isFlagged( CubicalCellData::FIXED ) 
		 || ccdata.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
	  {
	    ccdata.setFlag( CubicalCellData::COLLAPSIBLE );
	    Q_collapsible.push_back( it_c );
	  }
      }
  else // not ( S_closed )
    for ( cc_cell_set::const_iterator it = S.begin();
	  it != S.end();
	  ++it )
      {
	CubicalCell::lowIncidentCells( Q, (*it)->first, 
				       (*it)->first.dim( dim_s ), 
				       0, dim_s, dim_s );
	while ( ! Q.empty() )
	  {
	    it_c = K.find( Q.front() );
	    CubicalCellData & ccdata = it_c->second;
	    if ( ! ( ccdata.isFlagged( CubicalCellData::FIXED ) 
		     || ccdata.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
	      {
		ccdata.setFlag( CubicalCellData::COLLAPSIBLE );
		Q_collapsible.push_back( it_c );
	      }
	    Q.pop_front();
	  }
      }
  cout << " " << Q_collapsible.size() << " found." << endl;

  // Fill queue
  priority_queue
    < iterator, vector<iterator>, CubicalCellPriority > PQ( priority );

  
  cout << "[CC::collapse]-+ entering collapsing loop. " << endl;
  uint nb_pass = 0;
  uint nb_examined = 0;
  while ( ! S.empty() ) 
    {
      cout << "[CC::collapse]---+ Pass " << ++nb_pass
	   << ", Card(PQ)=";
      // cout << "[CC::collapse]---+ filling priority queue with ";
      // cout << endl;
      // push elements in the priority queue.
      for ( cc_cell_set::iterator it = S.begin();
	    it != S.end();
	    ++it )
	{
	  PQ.push( *it );
// 	  cout << "[CC::collapse]-----+ " << (*it)->first
// 	       << endl;
	  (*it)->second.setFlag( CubicalCellData::USER1 );
	}
      S.clear();
      // cout << "[CC::collapse]-----+ =";
      cout << PQ.size() << " elements, "
	   << "nb_exam=" << nb_examined << endl;
      
      // Try to collapse elements according to priority queue.
      while ( ! PQ.empty() )
	{
	  // Get top element.
	  iterator itcur = PQ.top();
	  PQ.pop();
	  ++nb_examined;
	  CubicalCellData & cur_data = itcur->second;
	  if ( ! cur_data.isFlagged( CubicalCellData::USER1 ) )
	    cout << "ERROR1";
	  cur_data.resetFlag( CubicalCellData::USER1 );

// 	  cout << "[CC::collapse]-----+ /" << nb_examined << "/ "
// 	       << itcur->first;

	  // Check if the cell is removable
	  if ( cur_data.isFlagged( CubicalCellData::REMOVED ) 
	       || ( ! cur_data.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
	    {
// 	      cout << " not removable." << endl;
	      continue;
	    }

	  // Cell may be removable.

	  // Check if it is a maximal cell
	  // const CubicalCell & cur_c = itcur->first;
	  iterator itup;
	  uint cur_c_type = K.computeCellType( itup, itcur,
					       coarse_up, fine_up );
	  bool found_pair = false;
	  iterator it_cell_c;
	  iterator it_cell_d;
	  if ( cur_c_type == 0 )
	    { // maximal cell... must find a free face
	      // cout << ", maximal." << endl;

	      // check faces to find a free face.
	      K.lowIncidentCells1( Q_low, itcur, 
				   coarse_low, fine_low );
	      iterator low_ic;
	      iterator best_free_face_it = K.end();
	      for ( itlow = Q_low.begin(); itlow != Q_low.end(); ++itlow )
		{
		  low_ic = *itlow;
		  if ( low_ic->second.isFlagged( CubicalCellData::REMOVED ) 
		       || ( ! low_ic->second.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
		    continue;
		  // const CubicalCell & cur_d = low_ic->first;
		  uint cur_d_type = K.computeCellType( itup, low_ic,
						       coarse_up, fine_up );

		  if ( cur_d_type == 1 )
		    { // found a free n-1-face ic
		      if ( ( best_free_face_it == K.end() )
			   // JOL 2008/04/07 : should choose greatest priority
			   // for removal.
			   || ( ! priority( low_ic, best_free_face_it ) ) )
			best_free_face_it = low_ic;
		    }
		}
	      if ( best_free_face_it != K.end() )
		{
		  // delete c and ic.
		  // cout << "[CC::collapse]------> found free n-1-face." << endl;
		  found_pair = true;
		  it_cell_c = itcur;
		  it_cell_d = best_free_face_it;
		  // Q_low already contains cells that should be
		  // checked again
		}
// 	      // check faces to find a free face.
// 	      K.lowIncidentCells1( cur_c, Q_low );
// 	      iterator low_ic;
// 	      for ( itlow = Q_low.begin(); itlow != Q_low.end(); ++itlow )
// 		{
// 		  low_ic = *itlow;
// 		  if ( low_ic->second.isFlagged( CubicalCellData::REMOVED ) 
// 		       || ( ! low_ic->second.isFlagged( CubicalCellData::COLLAPSIBLE ) ) )
// 		    continue;
// 		  const CubicalCell & cur_d = low_ic->first;
// 		  uint cur_d_type = K.computeCellType( cur_d, itup );

// 		  if ( cur_d_type == 1 )
// 		    { // found a free n-1-face ic
// 		      // delete c and ic.
// // 		      cout << "[CC::collapse]------> found free n-1-face." << endl;
// 		      found_pair = true;
// 		      it_cell_c = itcur;
// 		      it_cell_d = low_ic;
// 		      // Q_low already contains cells that should be
// 		      // checked again
// 		      break;
// 		    }
// 		}
	    }
	  else if ( cur_c_type == 1 )
	    { // free face... check that its 1-up-incident face is maximal.
	      // const CubicalCell & cur_d = itup->first;
	      iterator it_up_up;
	      uint cur_d_type = K.computeCellType( it_up_up, itup,
						   coarse_up, fine_up );
	      if ( cur_d_type == 0 )
		{ // found a maximal face.
		  // cout << "[CC::collapse]-------> its n+1-face is maximal." << endl;
// 		  cout << ", free face with max n+1-face." << endl;
		  found_pair = true;
		  it_cell_c = itup;
		  it_cell_d = itcur;
		  // Q_low will contain cells that should be checked
		  // again
		  K.lowIncidentCells1( Q_low, it_cell_c,
				       coarse_low, fine_low );
		}
// 	      else
// 		cout << ", free face, no max n+1-face." << endl;
	    }
// 	  else
// 	    cout << ", not a free face." << endl;
	  if ( found_pair )
	    {
//  	      cout << "[CC::collapse]-------> removing " << it_cell_c->first
//  		   << " " << it_cell_d->first
//  		   << endl;
	      it_cell_c->second.setFlag( CubicalCellData::REMOVED );
	      it_cell_d->second.setFlag( CubicalCellData::REMOVED );

	      // Incident cells have to be checked again.
// 	      uint nb_put_in_S = 0;
// 	      uint nb_low_1 = Q_low.size();
	      while ( ! Q_low.empty() )
		{
		  const CubicalCellData & data_qlow = 
		    Q_low.front()->second;
		  if ( ( ! data_qlow.isFlagged( CubicalCellData::REMOVED ) )
		       && data_qlow.isFlagged( CubicalCellData::COLLAPSIBLE )
		       && ( ! data_qlow.isFlagged( CubicalCellData::USER1 ) ) )
		    {
// 		      cout << "[CC::collapse]-------+ S += " 
// 			   << Q_low.front()->first << endl;
		      S.insert( Q_low.front() );
// 		      ++nb_put_in_S;
		    }
		  Q_low.pop_front();
		}
// 	      cout << "[CC::collapse]-------> " << nb_put_in_S << "/"
// 		   << nb_low_1 << " cells added to S, "
// 		   << "Card(S)=" << S.size() << endl;
	    }
	  else
	    Q_low.clear();
	  
	} // while ( ! PQ.empty() )
    } // while ( ! S.empty() ) 

  // Now clean the complex so that removed cells are effectively
  // removed and no more cell is tagged as collapsible.
  for ( deque< iterator >::iterator itc = Q_collapsible.begin();
	itc != Q_collapsible.end();
	++itc )
    {
      if ( (*itc)->second.isFlagged( CubicalCellData::REMOVED ) )
	K.m_cells.erase( *itc );
      else
	(*itc)->second.resetFlag( CubicalCellData::COLLAPSIBLE );
    }
}


/**
 * Extracts the cells of 'this' that belongs to the parallepipedic
 * subspace [c1, c2]. Cells in ]c1,c2| are added to [inner_S],
 * others are added to [bdry_S]. Take care of hierarchical
 * complexes, meaning that all cells within [c1, c2], even finer
 * ones, are selected.
 *
 * @param inner_S (returns) the set of inner cells is added to it.
 * @param bdry_S (returns) the set of boundary cells is added to it.
 * @param c1 the lower bound of the subspace.
 * @param c2 the upper bound of the subspace.
 */
void 
ImaGene::HierarchicalCubicalComplex::getCellsInSubspace
( CubicalComplex::cc_cell_set & inner_S, 
  CubicalComplex::cc_cell_set & bdry_S, 
  const CubicalCell & c1,
  const CubicalCell & c2 )
{
  uint i;
  uint nb_equal;
  uint d = this->dim();
  CubicalCell min( c1, d );
  CubicalCell max( c2, d );
  for ( i = 0; i < d; ++i )
    if ( min[ i ] > max[ i ] ) 
      std::swap( min[ i ], max[ i ] );
  CubicalCell tmp1( min, d );
  CubicalCell tmp2( min, d );

  // Start loop. Extract coarse cells, then tries finer ones.
  while ( true )
    {
      iterator it = m_cells.lower_bound( min );
      iterator it_end = m_cells.upper_bound( max );
      if ( it == m_cells.end() ) return;
      
      cc_cell_set::iterator it_inner = inner_S.end();
      cc_cell_set::iterator it_bdry = bdry_S.end();
      for ( ; it != it_end; ++it )
	{
	  const CubicalCell & c = it->first;
	  nb_equal = 0;
	  for ( i = 0; i < d; ++i )
	    {
	      if ( ( min[ i ] > c[ i ] ) || ( c[ i ] > max[ i ] ) )
		break;
	      else if ( ( min[ i ] == c[ i ] ) || ( max[ i ] == c[ i ] ) )
		++nb_equal;
	    }
	  if ( i == d )
	    if ( nb_equal != 0 )
	      it_bdry = bdry_S.insert( it_bdry, it );
	    else
	      it_inner = inner_S.insert( it_inner, it );
	}
      min.toFine( tmp1, tmp2, d );
      min.assign( tmp1, d );
      max.toFine( tmp1, tmp2, d );
      max.assign( tmp2, d );
    } // while ( true )
}

  
/**
 * Test method for hierarchical cubical complex.
 */
void
ImaGene::HierarchicalCubicalComplex::test()
{
  cout << "########### HierarchicalCubicalComplex ############" << endl;
  CubicalCell::default_dim = 3;
  HierarchicalCubicalComplex K( 3 );
  CubicalCellInvComparator ccc( 3 );
  CubicalCell cmin( 3, 3, 3 );
  CubicalCell cmax( 3, 3, 3 );

  CubicalCell c1( 11, 11, 10 );
  CubicalCell c2( 23, 25, 20 );
  CubicalCell c3( 13, 13, 10 );
  CubicalCell c4( 27, 23, 20 );
  CubicalCell c5( 25, 21, 20 );
  CubicalCell c6( 25, 23, 20 );
  K.add( c1 );
  K.add( c2 );
  K.add( c3 );
  K.add( c4 );
  K.add( c5 );
  K.add( c6 );

  cout << "----------------- before close --------------------" << endl;
  cout << "+- K=";
  K.selfDisplay( cout, true );
  cout << endl;

  K.close();

  cout << "----------------- after close --------------------" << endl;
  cout << "+- K=";
  K.selfDisplay( cout, true );
  cout << endl;
  
  cout << "----------------- collapsing K  ------------------" << endl;
  CubicalComplex::cc_cell_set bdry_S( ccc );
  CubicalComplex::cc_cell_set inner_S( ccc );
  K.getCellsInSubspace( inner_S, bdry_S, cmin, cmax ); 
  cout << "+- |bdry_S|=" << bdry_S.size() << endl;
  cout << "+- |inner_S|=" << inner_S.size() << endl;
  CubicalCellPriorityComputer ccpc;
  CubicalCellPriority ccp( &ccpc, 3, false );
  cout << "+- collapsing boundary..." << endl;
  HierarchicalCubicalComplex::collapse( K, bdry_S, true, ccp );
  cout << "+- K=";
  K.selfDisplay( cout, true );
  cout << endl;

  cout << "+- collapsing interior..." << endl;
  HierarchicalCubicalComplex::collapse( K, inner_S, true, ccp );

  cout << "----------------- after collapse------------------" << endl;
  cout << "+- K=";
  K.selfDisplay( cout, true );
  cout << endl;
  
}

