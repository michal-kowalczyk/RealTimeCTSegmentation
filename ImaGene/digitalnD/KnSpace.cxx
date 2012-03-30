///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : KnSpace.cxx
//
// Creation : 2002/07/20
//
// Version : 2002/07/20
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
//	2002/07/20 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>
//#include "LinAlg/LinAlg/Vector.hpp"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnUtils.h"

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/KnSpace.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const KnSpace_RCS_ID = "@(#)class KnSpace definition.";

///////////////////////////////////////////////////////////////////////////////
// class KnSpace
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

KnSpace::~KnSpace()
{
  freeRestrictedCellCodeLUT();
  freeSizeAndFields();
  freeVectors();
  freeSanitizers();
}

/**
 * Constructor.
 * This constructor prepares many codes to offer an efficient implementation
 * of Kn.
 * NB: you should call 'OK()' after an instanciation to check if your space
 * can be coded with 'Kn_id's.
 * TODO: provide "big" Kn_id's for big Kn spaces.
 * @param n is the dimension of the space.
 * @param sizes is an array containing the sizes along each dimension (sizes of the digital space).
 */
KnSpace::KnSpace( uint n, const uint64* sizes )
{
  resetSizeAndFields();
  resetVectors();
  resetSanitizers();
  resetRestrictedCellCodeLUT();
  Kn_size t_sizes[ n ];
  for ( uint i = 0; i < n; ++i )
    t_sizes[ i ] = (Kn_size) sizes[ i ];
  init( n, t_sizes );
}

/**
 * Constructor.
 * This constructor prepares many codes to offer an efficient implementation
 * of Kn.
 * NB: you should call 'OK()' after an instanciation to check if your space
 * can be coded with 'Kn_id's.
 * TODO: provide "big" Kn_id's for big Kn spaces.
 * @param n is the dimension of the space.
 * @param sizes is an array containing the sizes along each dimension (sizes of the digital space).
 */
KnSpace::KnSpace( uint n, const uint32* sizes )
{
  resetSizeAndFields();
  resetVectors();
  resetSanitizers();
  resetRestrictedCellCodeLUT();
  Kn_size t_sizes[ n ];
  for ( uint i = 0; i < n; ++i )
    t_sizes[ i ] = (Kn_size) sizes[ i ];
  init( n, t_sizes );
}

///////////////////////////////////////////////////////////////////////////////
// ------------------------- Protected standard services --------------------
/**
 * Default constructor.
 * the space is empty (dim = 0).
 * @see init
 */
KnSpace::KnSpace()
{
  resetSizeAndFields();
  resetVectors();
  resetSanitizers();
  resetRestrictedCellCodeLUT();
}

/*
 * This init service is called by the constructor or derived class at
 * instanciation.
 * @param n is the dimension of the space.
 * @param sizes is an array containing the sizes along each dimension (sizes of the digital space).
 */
void
KnSpace::init( uint n, const Kn_size* sizes )
{
  if ( n == 0 ) return;

  allocSizeAndFields( n );
  if ( ! computeSizeAndFields( n, sizes ) )
    {
      cerr << "[KnSpace::init] Not enough bits." << endl;
      freeSizeAndFields();
      resetSizeAndFields();
    }
  else
    {
      allocVectors( n );
      computeVectors( n );
      computeAndAllocSanitizers( n );
      computeAndAllocRestrictedCellCodeLUT( n );
    }
}

  

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

void 
KnSpace::selfDisplay( ostream& that_stream ) const
{
  if ( ! OK() )
    {
      that_stream << "[Kn n=0 INVALID]";
      return;
    }
  that_stream << "[Kn n=" << m_dim << " ( " << size( 0 );
  uint k;
  for ( k = 1; k < dim(); ++k )
    that_stream << " x " << size( k );
  that_stream << " )]";
}

bool 
KnSpace::OK() const
{
  return m_dim != 0;
}




///////////////////////////////////////////////////////////////////////////////
// -------------------- Neighborhood services -------------------------------

  
/**
 * Computes the 1-neighborhood of the cell [cell] and add the cells
 * to the front or the back of [l].
 *
 * @param cell the unsigned cell of interest.
 * @param l (modified) the queue where neighboring cells are added.
 * @param front when 'true', add cells to the front, otherwise to the back.
 */
void
ImaGene::KnSpace::uneighborhood
( Kn_uid cell, deque_ucells & l, bool front ) const
{
  if ( front )
    for ( uint k = 0; k < m_dim; ++k )
      {
	if ( ! uisMin( cell, k ) )
	  l.push_front( ugetDecr( cell, k ) );
	if ( ! uisMax( cell, k ) )
	  l.push_front( ugetIncr( cell, k ) );
      }
  else
    for ( uint k = 0; k < m_dim; ++k )
      {
	if ( ! uisMin( cell, k ) )
	  l.push_back( ugetDecr( cell, k ) );
	if ( ! uisMax( cell, k ) )
	  l.push_back( ugetIncr( cell, k ) );
      }
}


/**
 * Computes the 1-neighborhood of the cell [cell] and add the cells
 * to the front or the back of [l].
 *
 * @param cell the signed cell of interest.
 * @param l (modified) the queue where neighboring cells are added.
 * @param front when 'true', add cells to the front, otherwise to the back.
 */
void
ImaGene::KnSpace::sneighborhood
( Kn_sid cell, deque_scells & l, bool front ) const
{
  if ( front )
    for ( uint k = 0; k < m_dim; ++k )
      {
	if ( ! sisMin( cell, k ) )
	  l.push_front( sgetDecr( cell, k ) );
	if ( ! sisMax( cell, k ) )
	  l.push_front( sgetIncr( cell, k ) );
      }
  else
    for ( uint k = 0; k < m_dim; ++k )
      {
	if ( ! sisMin( cell, k ) )
	  l.push_back( sgetDecr( cell, k ) );
	if ( ! sisMax( cell, k ) )
	  l.push_back( sgetIncr( cell, k ) );
      }
}


///////////////////////////////////////////////////////////////////////////////
// -------------------- Adjacency services ----------------------------------

/**
 * Two k-cells [p1] and [p2] with same type are low-adjacent if they have in
 * their intersection a cell of dimension l such that 0 <= k-l <= lvl.
 * NB: '[lvl] == 0' implies that p1 and p2 must be equal to be adjacent.
 * @param p1 any unsigned cell.
 * @param p2 any unsigned cell of same type as [p1].
 * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
 * @return 'true' if the two cells are [lvl]-low adjacent.
 */
bool
KnSpace::uareLowAdjacent( Kn_uid p1, Kn_uid p2, uint & lvl ) const
{
  ASSERT_KnSpace( Field_unselect( p1, m_uid_coords ) 
		  == Field_unselect( p2, m_uid_coords ) );

  uint norm = 0;
  uint k = m_dim;
  Kn_uid x;
  while ( k-- )
    {
      Kn_uid mask = Field_mask( m_uid_coord[ k ] );
      Kn_uid p1m = p1 & mask;
      Kn_uid p2m = p2 & mask;
      x = p1m >= p2m ? p1m - p2m : p2m - p1m;
      if ( x > m_uid_coded_incr[ k ] ) 
	// means > +1 in direction [k]
	return false;
      if ( x != 0 )
	// means == +1 in direction [k]
	{
	  if ( Field_select( p1, m_uid_dir[ k ] ) )
	    ++norm;
	  else
	    return false;
	}
    } // while ( k-- )
  lvl = norm;
  return true;
}

/**
 * Two k-cells [p1] and [p2] with same type are up-adjacent if they have in
 * their intersection a cell of dimension l such that 0 <= l-k <= lvl.
 * NB: '[lvl] == 0' implies that p1 and p2 must be equal to be adjacent.
 * @param p1 any unsigned cell.
 * @param p2 any unsigned cell of same type as [p1].
 * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
 * @return 'true' if the two cells are [lvl]-up adjacent.
 */
bool
KnSpace::uareUpAdjacent( Kn_uid p1, Kn_uid p2, uint & lvl ) const
{
  ASSERT_KnSpace( Field_unselect( p1, m_uid_coords ) 
		  == Field_unselect( p2, m_uid_coords ) );

  uint norm = 0;
  uint k = m_dim;
  Kn_uid x;
  while ( k-- )
    {
      Kn_uid mask = Field_mask( m_uid_coord[ k ] );
      Kn_uid p1m = p1 & mask;
      Kn_uid p2m = p2 & mask;
      x = p1m >= p2m ? p1m - p2m : p2m - p1m;
      if ( x > m_uid_coded_incr[ k ] ) 
	// means > +1 in direction [k]
	return false;
      if ( x != 0 )
	// means == +1 in direction [k]
	{
	  if ( Field_select( p1, m_uid_dir[ k ] ) )
	    return false;
	  else
	    ++norm;
	}
    } // while ( k-- )
  lvl = norm;
  return true;
}

/**
 * Two k-cells [p1] and [p2] with same type are adjacent if they have in their 
 * intersection a cell of dimension l such that -lvl <= l-k <= lvl.
 * NB: if (l-k) is positive, it is an up-adjacency, otherwise low-adjacency.
 * NB: '[lvl] == 0' implies that p1 and p2 must be equal to be adjacent.
 * @param p1 any unsigned cell.
 * @param p2 any unsigned cell of same type as [p1].
 * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
 * @return 'true' if the two cells are [lvl]-up adjacent.
 */
bool
KnSpace::uareAdjacent( Kn_uid p1, Kn_uid p2, uint & lvl ) const
{
  ASSERT_KnSpace( Field_unselect( p1, m_uid_coords ) 
		  == Field_unselect( p2, m_uid_coords ) );

  uint norm = 0;
  uint k = m_dim;
  Kn_uid x;
  while ( k-- )
    {
      Kn_uid mask = Field_mask( m_uid_coord[ k ] );
      Kn_uid p1m = p1 & mask;
      Kn_uid p2m = p2 & mask;
      x = p1m >= p2m ? p1m - p2m : p2m - p1m;
      if ( x > m_uid_coded_incr[ k ] ) 
	// means > +1 in direction [k]
	return false;
      if ( x != 0 )
	// means == +1 in direction [k]
	++norm;
    } // while ( k-- )
  lvl = norm;
  return true;
}
////////////////////////////////////////////////////////////////////////
// The version below is 25% slower
// bool
// KnSpace::areAdjacent( Kn_uid p1, Kn_uid p2, uint lvl ) const
// {
//   ASSERT_KnSpace( Field_unselect( p1, m_uid_coords ) 
// 		  == Field_unselect( p2, m_uid_coords ) );
//   uint norm = 0;
//   uint k = m_dim;
//   Kn_uid x;
//   while ( k-- )
//     {
//       Kn_uid mask = Field_mask( m_uid_coord[ k ] );
//       if ( p1 >= p2 )
// 	{
// 	  x = ( p1 & mask ) - ( p2 & mask );
// 	  if ( x > m_uid_coded_incr[ k ] ) 
// 	    // means > +1 in direction [k]
// 	    return false;
// 	  if ( x != 0 )
// 	    // means == +1 in direction [k]
// 	    {
// 	      ++norm;
// 	      p1 -= x;
// 	    }
// 	}
//       else
// 	{
// 	  x = ( p2 & mask ) - ( p1 & mask );
// 	  if ( x > m_uid_coded_incr[ k ] ) 
// 	    // means > +1 in direction [k]
// 	    return false;
// 	  if ( x != 0 )
// 	    // means == +1 in direction [k]
// 	    {
// 	      ++norm;
// 	      p2 -= x;
// 	    }
// 	}
//     } // while ( k-- )
//   lvl = norm;
//  return true;

/**
 * Two k-cells [p1] and [p2] are low-adjacent if they have in their 
 * intersection a cell of dimension l such that 0 <= k-l <= lvl.
 * NB: '[lvl] == 0' implies that p1 and p2 must be equal to be adjacent.
 * @param p1 any signed cell.
 * @param p2 any signed cell of same type and sign as [p1].
 * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
 * @return 'true' if the two cells are [lvl]-low adjacent.
 */
bool
KnSpace::sareLowAdjacent( Kn_sid p1, Kn_sid p2, uint & lvl ) const
{
  ASSERT_KnSpace( Field_unselect( p1, m_sid_coords ) 
		  == Field_unselect( p2, m_sid_coords ) );

  uint norm = 0;
  uint k = m_dim;
  Kn_sid x;
  while ( k-- )
    {
      Kn_sid mask = Field_mask( m_sid_coord[ k ] );
      Kn_sid p1m = p1 & mask;
      Kn_sid p2m = p2 & mask;
      x = p1m >= p2m ? p1m - p2m : p2m - p1m;
      if ( x > m_sid_coded_incr[ k ] ) 
	// means > +1 in direction [k]
	return false;
      if ( x != 0 )
	// means == +1 in direction [k]
	{
	  if ( Field_select( p1, m_sid_dir[ k ] ) )
	    ++norm;
	  else
	    return false;
	}
    } // while ( k-- )
  lvl = norm;
  return true;
}

/**
 * Two k-cells [p1] and [p2] are up-adjacent if they have in their 
 * intersection a cell of dimension l such that 0 <= l-k <= lvl.
 * NB: '[lvl] == 0' implies that p1 and p2 must be equal to be adjacent.
 * @param p1 any signed cell.
 * @param p2 any signed cell of same type and sign as [p1].
 * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
 * @return 'true' if the two cells are [lvl]-up adjacent.
 */
bool
KnSpace::sareUpAdjacent( Kn_sid p1, Kn_sid p2, uint & lvl ) const
{
  ASSERT_KnSpace( Field_unselect( p1, m_sid_coords ) 
		  == Field_unselect( p2, m_sid_coords ) );

  uint norm = 0;
  uint k = m_dim;
  Kn_sid x;
  while ( k-- )
    {
      Kn_sid mask = Field_mask( m_sid_coord[ k ] );
      Kn_sid p1m = p1 & mask;
      Kn_sid p2m = p2 & mask;
      x = p1m >= p2m ? p1m - p2m : p2m - p1m;
      if ( x > m_sid_coded_incr[ k ] ) 
	// means > +1 in direction [k]
	return false;
      if ( x != 0 )
	// means == +1 in direction [k]
	{
	  if ( Field_select( p1, m_sid_dir[ k ] ) )
	    return false;
	  else
	    ++norm;
	}
    } // while ( k-- )
  lvl = norm;
  return true;
}

/**
 * Two k-cells [p1] and [p2] with same type are adjacent if they have in
 * their intersection a cell of dimension l such that -lvl <= l-k <= lvl.
 * NB: if (l-k) is positive, it is an up-adjacency, otherwise low-adjacency.
 * NB: '[lvl] == 0' implies that p1 and p2 must be equal to be adjacent.
 * @param p1 any signed cell.
 * @param p2 any signed cell of same type and sign as [p1].
 * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
 * @return 'true' if the two cells are [lvl]-up adjacent.
 */
bool
KnSpace::sareAdjacent( Kn_sid p1, Kn_sid p2, uint & lvl ) const
{
  ASSERT_KnSpace( Field_unselect( p1, m_sid_coords ) 
		  == Field_unselect( p2, m_sid_coords ) );

  uint norm = 0;
  uint k = m_dim;
  Kn_sid x;
  while ( k-- )
    {
      Kn_sid mask = Field_mask( m_sid_coord[ k ] );
      Kn_sid p1m = p1 & mask;
      Kn_sid p2m = p2 & mask;
      x = p1m >= p2m ? p1m - p2m : p2m - p1m;
      if ( x > m_sid_coded_incr[ k ] ) 
	// means > +1 in direction [k]
	return false;
      if ( x != 0 )
	// means == +1 in direction [k]
	++norm;
    } // while ( k-- )
  lvl = norm;
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// -------------------- Incidence services ----------------------------------

/**
 * A k-cell [p1] and a l-cell [p2] are incident if one is a face of the
 * other.
 * NB: if (k-l) is positive, [p1] has [p2] in its lower-incidence
 * and [p2] has [p1] in its upper-incidence.
 * NB: '[lvl] == 0' implies that p1 and p2 are equal.
 * @param p1 any unsigned cell.
 * @param p2 any unsigned cell.
 * @param lvl (returns) the level of the incidency ( dim(k)-dim(l) ).
 * @return 'true' if the two cells are [lvl]-incident.
 */
bool 
KnSpace::uareIncidentComputation( Kn_uid p1, Kn_uid p2, int & lvl ) const
{
  if ( uisInLowIncidenceComputation( p1, p2, lvl ) )
    return true;
  else if ( uisInLowIncidenceComputation( p2, p1, lvl ) )
    {
      lvl = -lvl;
      return true;
    }
  return false;
}


/**
 * A l-cell [p2] is in the low-incidence of a k-cell [p1] if [p2] is a face
 * of [p1].
 * NB: '[lvl] == 0' implies that p1 and p2 are equal.
 * @param p1 any unsigned cell.
 * @param p2 any unsigned cell.
 * @param lvl (returns) the level of the incidency ( dim(k)-dim(l) ).
 * @return 'true' if [p2] is a face of [p1] ([p2] [lvl]-low-incident to [p1])
 */
bool 
KnSpace::uisInLowIncidenceComputation( Kn_uid p1, Kn_uid p2, int & lvl ) const
{
  uint norm = 0;
  uint k = dim();
  if ( p1 < p2 ) return false;
  // p2 can only be in the low-incidence of p1.
  while ( k-- )
    {
      Kn_uid mask_coord = Field_mask( m_uid_coord[ k ] );
      Kn_uid p1x = p1 & mask_coord;
      Kn_uid p2x = p2 & mask_coord;
      Kn_uid mask_dir = Field_mask( m_uid_dir[ k ] );
      if ( p1 & mask_dir ) // p1 is open in this dimension
	{
	  if ( p2 & mask_dir ) // same for p2
	    {
	      if ( p2x != p1x )
		// then they cannot be incident
		return false; 
	      // otherwise, they are identical in this direction.
	    }
	  else // p2 is closed while p1 is open in dimension [k].
	    // then p2x \in { p1x, p1x + 1 } 
	    if ( ( p2x == p1x )
		 || ( p2x == p1x + m_uid_coded_incr[ k ] ) )
	      ++norm;
	    else // coordinates donot match. 
	      return false;
	}
      else // p1 is closed in this dimension
	{
	  if ( p2 & mask_dir )
	    // p2 is open in dimension [k].
	    // impossible, since p2 was low-incident in a previous 
	    // dimension.
	    return false;
	  // both are closed in this dimension => coordinates must match.
	  if ( p2x != p1x )
	    // coordinates do not match. 
	    return false;
	}
    } // while ( k-- )
  lvl = norm;
  return true;
}


/**
 * A l-cell [p2] is in the up-incidence of a k-cell [p1] if [p1] is a face
 * of [p2].
 * NB: '[lvl] == 0' implies that p1 and p2 are equal.
 * @param p1 any unsigned cell.
 * @param p2 any unsigned cell.
 * @param lvl (returns) the level of the incidency ( dim(l)-dim(k) ).
 * @return 'true' if [p1] is a face of [p2] ([p2] [lvl]-up-incident to [p1])
 */
bool 
KnSpace::uisInUpIncidenceComputation( Kn_uid p1, Kn_uid p2, int & lvl ) const
{
  return uisInLowIncidenceComputation( p2, p1, lvl );
}



///////////////////////////////////////////////////////////////////////////////
// ----------------------- display services -----------------------------------

/**
 * Writes/Displays an element on an output stream.
 * @param p the (unsigned) element to display.
 * @param that_stream the output stream where the object is written.
 */
void 
KnSpace::displayKn_uid( Kn_uid p,
			ostream & that_stream ) const
{
  string str_dir = "[u|";
  str_dir += KnUtils::sprintBin( Field_valueOf( p, m_uid_dirs ),
				 m_dim, '0' );
  str_dir += '|';
  uint k = m_dim;
  while ( k-- ) 
    {
      str_dir += KnUtils::sprintDec( Field_valueOf( p, m_uid_coord[ k ] ),
				     4, ' ' );
      if ( k != 0 ) str_dir += ',';
      else          str_dir += ']';
    }
  str_dir += "[";
  str_dir += KnUtils::sprintBin( p, KnTypes::nbBitsKn_uid(), '0' );
  str_dir += "]";
  that_stream << str_dir;
}

/**
 * Writes/Displays an element on an output stream.
 * @param p the (signed) element to display.
 * @param that_stream the output stream where the object is written.
 */
void
KnSpace::displayKn_sid( Kn_sid p,
			ostream & that_stream ) const
{
  string str_dir = "[";
  if ( decodeSign( p ) == KnTypes::POS )  
    str_dir += "+|";
  else
    str_dir += "-|";
  str_dir += KnUtils::sprintBin( Field_valueOf( p, m_sid_dirs ),
				 m_dim, '0' );
  str_dir += '|';
  uint k = m_dim;
  while ( k-- ) 
    {
      str_dir += KnUtils::sprintDec( Field_valueOf( p, m_sid_coord[ k ] ),
				     4, ' ' );
      if ( k != 0 ) str_dir += ',';
      else          str_dir += ']';
    }
  str_dir += "[";
  str_dir += KnUtils::sprintBin( p, KnTypes::nbBitsKn_sid(), '0' );
  str_dir += "]";
  that_stream << str_dir;
}

/**
 * Writes/Displays the fields of a Kn_uid.
 * @param that_stream the output stream where the object is written.
 */
void 
KnSpace::displayKn_uid_fields( ostream & that_stream ) const
{
  static const char idirs[ 17 ] = "fedcba987654TZYX";
  static const char dirs[ 17 ] = "XYZT456789abcdef";
  
  string str_dir = "[U|";
  str_dir += idirs + ( 16 - m_dim );
  str_dir += '|';
  uint k = m_dim;
  while ( k-- ) 
    {
      str_dir += "   ";
      str_dir += dirs[ k ];
      if ( k != 0 ) str_dir += ',';
      else          str_dir += ']';
    }
  str_dir += "[";
  uint nb = KnTypes::nbBitsKn_uid();
  while ( nb > ( Field_shift( m_uid_dirs )
		 + Field_nb_bits( m_uid_dirs ) ) )
    {
      str_dir += " ";
      nb--;
    }
  k = m_dim;
  while ( k-- )
    {
      str_dir += dirs[ k ];
    }
  k = m_dim;
  while ( k-- )
    {
      uint l = Field_nb_bits( m_uid_coord[ k ] ) - 1;
      while ( l-- )
	str_dir += " ";
      str_dir += dirs[ k ];
    }
  str_dir += "]";
  that_stream << str_dir;
}

/**
 * Writes/Displays the fields of a Kn_sid.
 * @param that_stream the output stream where the object is written.
 */
void 
KnSpace::displayKn_sid_fields( ostream & that_stream ) const
{
  static const char idirs[ 17 ] = "fedcba987654TZYX";
  static const char dirs[ 17 ] = "XYZT456789abcdef";
  
  string str_dir = "[S|";
  str_dir += idirs + ( 16 - m_dim );
  str_dir += '|';
  uint k = m_dim;
  while ( k-- ) 
    {
      str_dir += "   ";
      str_dir += dirs[ k ];
      if ( k != 0 ) str_dir += ',';
      else          str_dir += ']';
    }
  str_dir += "[";
  uint nb = KnTypes::nbBitsKn_uid();
  while ( nb > ( Field_shift( m_sid_dirs )
		 + Field_nb_bits( m_sid_dirs ) ) )
    {
      str_dir += " ";
      nb--;
    }
  k = m_dim;
  while ( k-- )
    {
      str_dir += dirs[ k ];
    }
  str_dir += "S";
  k = m_dim;
  while ( k-- )
    {
      uint l = Field_nb_bits( m_sid_coord[ k ] ) - 1;
      while ( l-- )
	str_dir += " ";
      str_dir += dirs[ k ];
    }
  str_dir += "]";
  that_stream << str_dir;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

/**
 * Internal methods to compute masks from dimension and given sizes.
 * @param n is the dimension of the space.
 * @param sizes is an array containing the sizes along each dimension (sizes of the digital space).
 */
bool
KnSpace::computeSizeAndFields( uint n, const Kn_size* sizes )
{
  uint i;
  uint shift = 0;
  uint max_uid_nb_bits = KnTypes::nbBitsKn_uid();
  uint max_sid_nb_bits = KnTypes::nbBitsKn_sid();

  m_nb_bits = 0;
  m_dim = n;
  m_nb_celltypes = 1 << n;
  m_uid_coded_maxs = 0;
  m_sid_coded_maxs = 0;
  m_nb_spels = 1;
  // Computing masks for coordinates.
  for ( i = 0; i < n; i++ )
    {
      m_size[ i ] = sizes[ i ];
      m_nb_spels *= m_size[ i ];
      // nb_bits : nb of bits used to define coordinate.
      uint nb_bits = KnTypes::nbBitsCoord( sizes[ i ] );
      m_uid_coord[ i ].mask = 
	( ( KnTypes::ONE << nb_bits ) - KnTypes::ONE ) << shift;
      m_sid_coord[ i ].mask = 
	( ( KnTypes::ONE << nb_bits ) - KnTypes::ONE ) << shift;
      m_uid_coord[ i ].inv_mask	= ~m_uid_coord[ i ].mask;
      m_sid_coord[ i ].inv_mask	= ~m_sid_coord[ i ].mask;
      m_uid_coord[ i ].shift = shift;
      m_sid_coord[ i ].shift = shift;
      m_uid_coord[ i ].nb_bits = nb_bits;
      m_sid_coord[ i ].nb_bits = nb_bits;
      m_uid_coded_max[ i ] = ( sizes[ i ] - KnTypes::ONE ) << shift;
      m_uid_coded_maxs += m_uid_coded_max[ i ];
      m_uid_coded_incr[ i ] = KnTypes::ONE << shift;
      m_sid_coded_max[ i ] = ( sizes[ i ] - KnTypes::ONE ) << shift;
      m_sid_coded_maxs += m_sid_coded_max[ i ];
      m_sid_coded_incr[ i ] = KnTypes::ONE << shift;
      m_nb_bits += nb_bits;
      shift += nb_bits;
    }

  // Computing mask for all coords
  m_uid_coords.mask = ( KnTypes::ONE << shift ) - KnTypes::ONE;
  m_uid_coords.inv_mask = ~m_uid_coords.mask;
  m_uid_coords.shift = 0;
  m_uid_coords.nb_bits = shift;

  m_sid_coords.mask = ( KnTypes::ONE << shift ) - KnTypes::ONE;
  m_sid_coords.inv_mask = ~m_sid_coords.mask;
  m_sid_coords.shift = 0;
  m_sid_coords.nb_bits = shift;

  m_sid_sign_coords.mask = ( KnTypes::ONE << ( shift + 1 ) ) - KnTypes::ONE;
  m_sid_sign_coords.inv_mask = ~m_sid_sign_coords.mask;
  m_sid_sign_coords.shift = 0;
  m_sid_sign_coords.nb_bits = shift + 1;
  
  // Computing mask for sign.
  m_sid_sign.mask = KnTypes::ONE << shift;
  m_sid_sign.inv_mask = ~m_sid_sign.mask;
  m_sid_sign.shift = shift;
  m_sid_sign.nb_bits = 1;
  
  // Computing masks for directions.
  m_uid_dirs.mask = ( ( KnTypes::ONE << n ) - KnTypes::ONE ) << shift;
  m_uid_dirs.inv_mask = ~m_uid_dirs.mask;
  m_uid_dirs.shift = shift;
  m_uid_dirs.nb_bits = n;

  m_sid_dirs.mask = ( ( KnTypes::ONE << n ) - KnTypes::ONE ) << ( shift + 1 );
  m_sid_dirs.inv_mask = ~m_sid_dirs.mask;
  m_sid_dirs.shift = shift + 1;
  m_sid_dirs.nb_bits = n;

  m_sid_dirs_and_sign.mask = m_sid_dirs.mask | m_sid_sign.mask;
  m_sid_dirs_and_sign.inv_mask = ~m_sid_dirs_and_sign.mask;
  m_sid_dirs_and_sign.shift = m_sid_sign.shift;
  m_sid_dirs_and_sign.nb_bits = m_sid_sign.nb_bits + m_sid_dirs.nb_bits;
  
  m_nb_bits += n + 1;

  // Computing masks for each direction.
  m_uid_dir[ 0 ].mask = KnTypes::ONE << shift;
  m_uid_dir[ 0 ].inv_mask = ~m_uid_dir[ 0 ].mask;
  m_uid_dir[ 0 ].shift = shift;
  m_uid_dir[ 0 ].nb_bits = 1;

  m_sid_dir[ 0 ].mask = KnTypes::ONE << ( shift + 1);
  m_sid_dir[ 0 ].inv_mask = ~m_sid_dir[ 0 ].mask;
  m_sid_dir[ 0 ].shift = shift + 1;
  m_sid_dir[ 0 ].nb_bits = 1;

  for ( i = 1; i < n; i++ )
    {
      m_uid_dir[ i ].mask = m_uid_dir[ i - 1 ].mask << 1;
      m_uid_dir[ i ].inv_mask =~m_uid_dir[ i ].mask;
      m_uid_dir[ i ].shift = m_uid_dir[ i - 1 ].shift + 1;
      m_uid_dir[ i ].nb_bits = 1;

      m_sid_dir[ i ].mask = m_sid_dir[ i - 1 ].mask << 1;
      m_sid_dir[ i ].inv_mask =~m_sid_dir[ i ].mask;
      m_sid_dir[ i ].shift = m_sid_dir[ i - 1 ].shift + 1;
      m_sid_dir[ i ].nb_bits = 1;
    }

  // Computing incidence LUT.
  computeIncidenceLUT( n );

  // Computing permutation LUT.
  computePermutationLUT( n );

  // Computing coordinates for each cell type.
  uint b, j;
  for ( i = 0; i < m_nb_celltypes; ++i )
    {
      Kn_uid uid = 0;
      Kn_sid sid = 0;
      for ( b = 0; b < n; ++b )
	if ( i & ( 1 << b ) )
	  {
	    uid |= m_uid_coded_incr[ b ];
	    sid |= m_sid_coded_incr[ b ];
	  }
      m_uid_coded_celltype_incr[ i ] = uid;
      m_sid_coded_celltype_incr[ i ] = sid;
    }

  // Computing coordinates of vertices for each cell type.
  uint nb;
  for ( i = 0; i < m_nb_celltypes; ++i )
    {
      nb = 0;
      for ( j = 0; j < m_nb_celltypes; ++j )
	{
	  if ( m_low_incident[ ( i << n ) + j ] )
	    {
	      // cout << "Low-incidence: " << i << " " << j << endl;
	      m_uid_coded_coords_vtx[ ( i << n ) + nb ] 
		= m_uid_coded_celltype_incr[ i - j ];
	      nb++;
	    }
	}
      sort( m_uid_coded_coords_vtx + ( i << n ),
	    m_uid_coded_coords_vtx + ( ( i << n ) + nb ) );
    }

  // Compute direct orientation LUT.
  computeDirectOrientationLUT( n );

  // Compute binomials.
  KnUtils::binomials( n, m_binomials );
  
  // Check number of bits allowed.
  return ( m_nb_bits <= max_sid_nb_bits )
    && ( m_nb_bits - 1 <= max_uid_nb_bits );
}

/**
 * Internal methods to reset all mask attributes.
 * Should be called at construction or after freeing masks.
 */
void
KnSpace::resetSizeAndFields()
{
  m_dim = 0;
  m_size = 0;

  m_low_incident = 0;
  m_up_incident = 0;
  m_permutation = 0;
  m_sid_incident1 = 0;
  m_sid_incident2 = 0;
  m_sid_low_incidence_matrix = 0;
  m_sid_direct_orientation = 0;
  
  m_uid_coded_max = 0;
  m_uid_coded_incr = 0;
  m_uid_coded_celltype_incr = 0;
  m_uid_coded_coords_vtx = 0;
  m_uid_coord = 0;
  m_uid_dir = 0;

  m_sid_coded_max = 0;
  m_sid_coded_incr = 0;
  m_sid_coded_celltype_incr = 0;
  m_sid_coded_coords_vtx = 0;
  m_sid_coord = 0;
  m_sid_dir = 0;

  m_binomials = 0;
}

/**
 * Internal methods to alloc memory for masks.
 * @param n is the dimension of the space.
 */
void
KnSpace::allocSizeAndFields( uint n )
{
  m_size = new Kn_size[ n ];

  m_low_incident = new bool[ 1 << ( 2 * n ) ];
  m_up_incident = new bool[ 1 << ( 2 * n ) ];
  m_permutation = new bool[ ( 1 << n ) * n ];
  m_sid_incident1 = new Kn_sid[ ( 2 << n ) * n ];
  m_sid_incident2 = new Kn_sid[ ( 2 << n ) * n ];
  m_sid_low_incidence_matrix = new int8[ 1 << ( 2 * n + 2 ) ];
  m_sid_direct_orientation = new bool[ ( 2 << n ) * n ];

  m_uid_coded_max = new Kn_uid[ n ];
  m_uid_coded_incr = new Kn_uid[ n ];
  m_uid_coded_celltype_incr = new Kn_uid[ 1 << n ];
  m_uid_coded_coords_vtx = new Kn_uid[ 1 << ( 2 * n ) ];
  m_uid_coord = new Kn_uid_Field[ n ];
  m_uid_dir = new Kn_uid_Field[ n ];

  m_sid_coded_max = new Kn_sid[ n ];
  m_sid_coded_incr = new Kn_sid[ n ];
  m_sid_coded_celltype_incr = new Kn_sid[ 1 << n ];
  m_sid_coded_coords_vtx = new Kn_sid[ 1 << ( 2 * n ) ];
  m_sid_coord = new Kn_sid_Field[ n ];
  m_sid_dir = new Kn_sid_Field[ n ];

  m_binomials = new uint[ n + 1 ];
}

/**
 * Internal methods to dealloc memory for masks.
 */
void
KnSpace::freeSizeAndFields()
{
  if ( m_size != 0 )               delete[] m_size;
  
  if ( m_low_incident != 0 )       delete[] m_low_incident;
  if ( m_up_incident != 0 )        delete[] m_up_incident;
  if ( m_permutation != 0 )        delete[] m_permutation;
  if ( m_sid_incident1 != 0 )      delete[] m_sid_incident1;
  if ( m_sid_incident2 != 0 )      delete[] m_sid_incident2;
  if ( m_sid_low_incidence_matrix != 0 ) delete[] m_sid_low_incidence_matrix;
  if ( m_sid_direct_orientation != 0 ) delete[] m_sid_direct_orientation;
  
  if ( m_uid_coded_max != 0 )      delete[] m_uid_coded_max;
  if ( m_uid_coded_incr != 0 )     delete[] m_uid_coded_incr;
  if ( m_uid_coded_celltype_incr != 0 ) delete[] m_uid_coded_celltype_incr;
  if ( m_uid_coded_coords_vtx != 0 ) delete[] m_uid_coded_coords_vtx;
  if ( m_uid_coord != 0)           delete[] m_uid_coord;
  if ( m_uid_dir != 0)             delete[] m_uid_dir;

  if ( m_sid_coded_max != 0 )      delete[] m_sid_coded_max;
  if ( m_sid_coded_incr != 0 )     delete[] m_sid_coded_incr;
  if ( m_sid_coded_celltype_incr != 0 ) delete[] m_sid_coded_celltype_incr;
  if ( m_sid_coded_coords_vtx != 0 ) delete[] m_sid_coded_coords_vtx;
  if ( m_sid_coord != 0)           delete[] m_sid_coord;
  if ( m_sid_dir != 0)             delete[] m_sid_dir;

  if ( m_binomials != 0 )          delete[] m_binomials;
}

/**
 * Internal methods to reset all vector attributes.
 * Should be called at construction or after freeing vectors.
 */
void
KnSpace::resetVectors()
{
  m_basis = 0;
  m_orth_basis = 0;
}

/**
 * Internal methods to alloc memory for vectors.
 * @param n is the dimension of the space.
 */
void
KnSpace::allocVectors( uint n )
{
  m_basis = Vector::newArray( 1 << n, n );
  m_orth_basis = Vector::newArray( 1 << n, n );
}

/**
 * Internal methods to dealloc memory for masks.
 */
void
KnSpace::freeVectors()
{
  if ( m_basis != 0 ) 
    Vector::deleteArray( m_basis, m_nb_celltypes );
  if ( m_orth_basis != 0 ) 
    Vector::deleteArray( m_orth_basis, m_nb_celltypes );
}

/**
 * Internal methods to compute vectors from the space dimension.
 * @param n is the dimension of the space.
 * @return 'true'
 */
bool
KnSpace::computeVectors( uint n )
{
  uint t; // type of cell
  for ( t = 0; t < m_nb_celltypes; ++t )
    {
      uint c; // current coordinate
      uint b; // current mask of coordinate
      for ( c = 0, b = 1; b < m_nb_celltypes; b <<= 1, ++c )
	{
	  if ( t & b )
	    {
	      m_basis[ t ].rw( c ) = 1.0;
	      m_orth_basis[ t ].rw( c ) = 0.0;
	    }
	  else
	    {
	      m_basis[ t ].rw( c ) = 0.0;
	      m_orth_basis[ t ].rw( c ) = 1.0;
	    }
	}
    }
  return true;
}


/**
 * Internal methods to compute incidence LUT from dimension and given sizes.
 * NB: is called by 'computeSizeAndFields'.
 * @param n is the dimension of the space.
 */
bool
KnSpace::computeIncidenceLUT( uint n )
{
  uint k; // it is the directions of the first cell.
  uint l; // it is the directions of the second cell.
  uint b; // it is the current direction.
  uint nb_celltypes = 1 << n; // the number of possible directions.
  // We compute the low-incidence LUT (meaning [l] in the low-incidence of [k])
  // We compute the up-incidence LUT (meaning [l] in the up-incidence of [k])
  // Two nested loops to examine each case.
  for ( k = 0; k < nb_celltypes; ++k ) // loop on all directions
    for ( l = 0; l < nb_celltypes; ++l ) // loop on all directions
      {
	bool low_incident = true;
	bool up_incident = true;
	for ( b = 1; b < nb_celltypes; b <<= 1 ) // loop on all bits
	  {
	    if ( ( k & b ) && ( ! ( l & b ) ) )
	      up_incident = false;
	    else if ( ( ! ( k & b ) ) && ( l & b ) )
	      low_incident = false;
	    // proceed to next bit otherwise.
	  }
	uint offset = ( k << n ) + l;
	m_low_incident[ offset ] = low_incident;
	m_up_incident[ offset ] = up_incident;
      }
  return true;
}

/**
 * Internal methods to compute LUT for sign of permutations from dimension.
 * Useful to compute the signed incidence and the boundary operators.
 * NB: is called by 'computeSizeAndFields'.
 * @param n is the dimension of the space.
 */
bool
KnSpace::computePermutationLUT( uint n )
{
  uint nb_celltypes = 1 << n;
  uint i, j, b;
  uint offset = 0;

  // Set incidence matrix to 0.
  for ( i = 0; i < 2 * nb_celltypes; ++i )
    for ( j = 0; j < 2* nb_celltypes; ++j )
      { 
 	// low-incidence matrix is 0, except from 1-low-incident.
 	m_sid_low_incidence_matrix[ offset ] = 0;
 	++offset;
      }
  
  // Fills incidence LUTs and matrix.
  for ( i = 0; i < nb_celltypes; ++i )
    {
      bool sign = true;
      for ( b = 0; b < n; ++b )
	{
	  uint unsigned_offset = ( b << n ) + i;
	  m_permutation[ unsigned_offset ] = sign;
	  uint signed_offset = ( b << (n+1) ) + ( i << 1 );
	  m_sid_incident1[ signed_offset ] = (Kn_sid) 
	    ( ( i ^ ( 1 << b ) ) << 1 ) + ( sign ? 0 : 1 );
	  m_sid_incident2[ signed_offset ] = (Kn_sid) 
	    ( ( i ^ ( 1 << b ) ) << 1 ) + ( sign ? 1 : 0 );
	  m_sid_incident1[ signed_offset + 1 ] = (Kn_sid) 
	    ( ( i ^ ( 1 << b ) ) << 1 ) + ( sign ? 1 : 0 );
	  m_sid_incident2[ signed_offset + 1 ] = (Kn_sid) 
	    ( ( i ^ ( 1 << b ) ) << 1 ) + ( sign ? 0 : 1 );
	  if ( i & ( 1 << b ) )
	    {
 	      uint signed_i = i << 1;
 	      uint signed_j = ( i - ( 1 << b ) ) << 1;
 	      m_sid_low_incidence_matrix[ (signed_i<<(n+1)) + signed_j ]
 		= ( sign ? 1 : -1 );
 	      m_sid_low_incidence_matrix[ (signed_i<<(n+1)) + signed_j+1]
 		= ( sign ? -1 : 1 );
 	      m_sid_low_incidence_matrix[ ((signed_i+1)<<(n+1)) + signed_j ]
 		= ( sign ? -1 : 1 );
 	      m_sid_low_incidence_matrix[ ((signed_i+1)<<(n+1)) + signed_j+1]
 		= ( sign ? 1 : -1 );
	      sign = ! sign;
	    }
	}
    }
  return true;
}


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Sanitizer accessor ---------------------------

/**
 * Returns a reference to the proper bitset sanitizer depending on whether
 * or not the set is composed of oriented cells or not and on whether or not
 * the set is composed of cells with different topology.
 * @param oriented if 'true' the bitset will be associated to a set of oriented cells, otherwise it will be associated to a set of unoriented cells.
 * @param mixed if 'true' the bitset will be associated to a set of cells with different topologies, otherwise the bitset will be associated to a set of cells with the same topology.
 * @return the reference to the proper sanitizer.
 * NB: Internal use of 'KnRCellSet' and 'KnCharSet'.
 * @see Bitset1,KnRCellSet,KnCharSet
 */
const Shared<Sanitizer> & 
KnSpace::getSanitizer( bool oriented,
		       bool mixed ) const
{
  if ( mixed )
    {
      if ( oriented )
	return *m_sanitizer_set_signed_mixed_cells;
      return *m_sanitizer_set_unsigned_mixed_cells;
    }
  if ( oriented )
    return *m_sanitizer_set_signed_single_cell;
  return *m_sanitizer_set_unsigned_single_cell;
}



/**
 * Internal methods to reset all sanitizers attributes.
 * Should be called at construction or after freeing sanitizers.
 */
void
KnSpace::resetSanitizers()
{
  m_sanitizer_set_unsigned_single_cell = 0;
  m_sanitizer_set_signed_single_cell = 0;
  m_sanitizer_set_unsigned_mixed_cells = 0;
  m_sanitizer_set_signed_mixed_cells = 0;
}

/**
 * Internal methods to dealloc memory for sanitizers.
 */
void
KnSpace::freeSanitizers()
{
  if ( m_sanitizer_set_unsigned_single_cell != 0 )
    delete m_sanitizer_set_unsigned_single_cell;
  if ( m_sanitizer_set_signed_single_cell != 0 )
    delete m_sanitizer_set_signed_single_cell;
  if ( m_sanitizer_set_unsigned_mixed_cells != 0 )
    delete m_sanitizer_set_unsigned_mixed_cells;
  if ( m_sanitizer_set_signed_mixed_cells != 0 )
    delete m_sanitizer_set_signed_mixed_cells;
}


/**
 * Internal methods to compute sanitizers for all kind of sets.
 * @param n is the dimension of the space.
 * @return 'true'
 */
bool
KnSpace::computeAndAllocSanitizers( uint n )
{
  uint k;
  Kn_size* s_size = new Kn_size[ n + 1 ];
  Kn_size* s_true_size = new Kn_size[ n + 1 ];
  for ( k = 0; k < n; ++k )
    {
      s_size[ k ] = m_size[ k ];
      s_true_size[ k ] = KnTypes::ONE << m_uid_coord[ k ].nb_bits;
    }

  // Single type of unsigned cells.
  Sanitizer s1( n, s_size, s_true_size );
  m_sanitizer_set_unsigned_single_cell = new Shared<Sanitizer>( s1 );

  // Single type of signed cells.
  s_size[ n ] = 2;
  s_true_size[ n ] = 2;
  Sanitizer s2( n + 1, s_size, s_true_size );
  m_sanitizer_set_signed_single_cell = new Shared<Sanitizer>( s2 );

  // Mixed types of unsigned cells.
  s_size[ n ] = m_nb_celltypes;
  s_true_size[ n ] = m_nb_celltypes;
  Sanitizer s3( n + 1, s_size, s_true_size );
  m_sanitizer_set_unsigned_mixed_cells = new Shared<Sanitizer>( s3 );

  // Mixed types of signed cells.
  s_size[ n ] = m_nb_celltypes << 1;
  s_true_size[ n ] = m_nb_celltypes << 1;
  Sanitizer s4( n + 1, s_size, s_true_size );
  m_sanitizer_set_signed_mixed_cells = new Shared<Sanitizer>( s4 );

  delete[] s_size;
  delete[] s_true_size;
  return true;
}


/**
 * Internal methods to compute direct orientations for each cell-type.
 * Useful to digital surface tracking.
 * NB: is called by 'computeSizeAndFields'.
 * @param n is the dimension of the space.
 */
bool
KnSpace::computeDirectOrientationLUT( uint n )
{
  uint nb_celltypes = 1 << n;
  Kn_sid i;
  uint b;
  uint offset;

  // Fills direct orientations LUT.
  for ( i = 0; i < nb_celltypes; ++i )
    {
      // c is a positive cell.
      Kn_sid c = ( i << Field_shift( m_sid_dirs) );
      
      // For each direction.
      for ( b = 0; b < n; ++b )
	{
	  Kn_sid inc_c = sincident( c, b, true );
	  offset = ( b << ( n + 1 ) ) + ( i << 1 );
	  if ( decodeSign( inc_c ) == KnTypes::POS )
	    {
	      // TODO
	      m_sid_direct_orientation[ offset ] = true;       // pos
	      m_sid_direct_orientation[ offset + 1 ] = false;  // neg
	    }
	  else
	    {
	      m_sid_direct_orientation[ offset ] = false;      // pos
	      m_sid_direct_orientation[ offset + 1 ] = true;   // neg
	    }
	}
    }
  return true;
}

/**
 * Internal method to compute restricted cell codes LUT.
 * Useful to define mappings from r-cells (minimizes memory space).
 * @param n is the dimension of the space.
 */
bool
KnSpace::computeAndAllocRestrictedCellCodeLUT( uint n )
{
  ASSERT_KnSpace( m_binomials != 0 );
  
  // Computes the LUT that converts the topology of a cell into a restricted
  // cell code.
  uint nb_celltypes = 1 << n;
  m_urcode = new Kn_urid[ nb_celltypes ];
  m_srcode = new Kn_srid[ nb_celltypes ];
  m_urdecode = new Kn_uid*[ n + 1 ];
  m_srdecode = new Kn_sid*[ n + 1 ];
  uint i;
  Kn_size* acc = new Kn_size[ n + 1 ];
  for ( i = 0; i <= n; ++i ) 
    {
      acc[ i ] = 0;
      m_urdecode[ i ] = new Kn_uid[ m_binomials[ i ] ];
      m_srdecode[ i ] = new Kn_sid[ m_binomials[ i ] ];
    }
  Kn_size topo;
  for ( topo = 0; topo < nb_celltypes; ++topo )
    {
      // Count the dimension
      uint cell_dim = KnUtils::countSetBits( topo );

      // LUTs to code cells as restricted cells.
      m_urcode[ topo ] = Field_codeOf( acc[ cell_dim ], m_uid_dirs );
      m_srcode[ topo ] = Field_codeOf( acc[ cell_dim ], m_sid_dirs );
      
      // LUTs to code restricted cells as cells.
      m_urdecode[ cell_dim ][ acc[ cell_dim ] ] 
	= Field_codeOf( topo, m_uid_dirs );
      m_srdecode[ cell_dim ][ acc[ cell_dim ] ] 
	= Field_codeOf( topo, m_sid_dirs );
      acc[ cell_dim ]++;
    }
  delete[] acc;
  return true;
}

/**
 * Internal method to dealloc memory for restricted cell codes LUT.
 */
void
KnSpace::freeRestrictedCellCodeLUT()
{
  uint i;
  if ( m_urdecode != 0 )
    {
      for ( i = 0; i <= dim(); ++i )
	delete[] m_urdecode[ i ];
      delete[] m_urdecode;
    }
  if ( m_srdecode != 0 )
    {
      for ( i = 0; i <= dim(); ++i )
	delete[] m_srdecode[ i ];
      delete[] m_srdecode;
    }
  if ( m_urcode != 0 )   delete[] m_urcode;
  if ( m_srcode != 0 )   delete[] m_srcode;
}

/**
 * Internal method to reset restricted cell codes LUT.
 * Should be called at construction or after freeing sanitizers.
 */
void
KnSpace::resetRestrictedCellCodeLUT()
{
  m_urdecode = 0;
  m_srdecode = 0;
  m_urcode = 0;
  m_srcode = 0;
}

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
