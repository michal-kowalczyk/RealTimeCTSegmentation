///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : KnTools.cxx
//
// Creation : 2003/07/24
//
// Version : 2003/07/24
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
//	2003/07/24 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnSpaceCoordScanner.h"
#include "ImaGene/digitalnD/KnTools.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/KnTools.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const KnTools_RCS_ID = "@(#)class KnTools definition.";

///////////////////////////////////////////////////////////////////////////////
// class KnTools
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static services ------------------------------

/**
 * Builds a KnCharSet equivalent to [rset].
 * @param rset any set of cells (oriented or not).
 * @return the corresponding KnCharSet.
 */
KnCharSet
KnTools::convert( const KnRCellSet & rset )
{
  return KnRCellSet::makeKnCharSet( rset );
}


/**
 * Builds a set of spels from a space [ks] and an implicit mapping [f] from
 * Rn to R. All the spels whose centroid has a value by [f] inferior or equal
 * to 0.0 are considered inside the set.
 * @param ks a digital space
 * @param f an implicit mapping from Rn to R with 'n==ks.dim()'.
 * @return the corresponding KnCharSet.
 */
KnCharSet
KnTools::negativeSpels( const KnSpace & ks,
			const RnMap & f )
{
  return negativeSpels( ks, f, 
			ks.uspel( ks.ufirst() ), 
			ks.uspel( ks.ulast() ) );
}


/**
 * Builds a set of spels from a space [ks] and an implicit mapping [f] from
 * Rn to R. All the spels whose centroid has a value by [f] inferior or equal
 * to 0.0 are considered inside the set. Restricts the computation to the
 * bounding box [low],[up].
 * @param ks a digital space
 * @param f an implicit mapping from Rn to R with 'n==ks.dim()'.
 * @param low the spel with smallest coordinates in the bounding box.
 * @param up the spel with highest coordinates in the bounding box.
 * @return the corresponding KnCharSet.
 */
KnCharSet
KnTools::negativeSpels( const KnSpace & ks,
			const RnMap & f,
			Kn_uid low,
			Kn_uid up )
{
  // Filling it by scanning the bounding cube.
  // KnCharSet s = KnCharSet::create( ks, ks.dim(), false, 0 );
  KnCharSet s = KnCharSet::ucreate( ks, low, 0 );
  KnSpaceCoordScanner scanner( ks, low, up );
  Kn_uid p = scanner.lower_bound;
  Vector vp = ks.ucentroid( p );
  do 
    {
      if ( ( f.eval( vp ) ).ro( 0 ) <= 0.0 )
	s += p;
    }
  while ( scanner.increment( p, vp ) );
  return s;
}



/**
 * In the digital space [ks], computes the set of low incident cells
 * to [c] from [min_dim] to [max_dim] and add them to [q]. Nb: if
 * 'dim(c) <= max_dim' then [c] is added to [q].
 *
 * @param q (returns) add incident cells to this deque.
 * @param ks the digital space.
 * @param c the unoriented cell of interest.
 * @param max_dim the maximum dimension of the incident cells (higher are discarded).
 * @param min_dim the minimum dimension of the incident cells (lower are discarded).
 */
void
ImaGene::KnTools::ulowIncidentCells( deque<Kn_uid> & q, 
				     const KnSpace & ks,
				     Kn_uid c,
				     uint max_dim,
				     uint min_dim )
{
  ASSERT_KnTools( max_dim >= min_dim );

  // @TODO JOL: copy principle of CubicalCell::lowIncidentCells.
  //
  //  cerr << "[KnTools::ulowIncidentCells] WARNING. Invalid method. " << endl;
  // Test if it is necessary to go on.
  if ( ks.udim( c ) < min_dim ) return;

  // Add cells to queue if within bounds.
  if ( // ( ks.udim( c ) >= min_dim ) 
      ( ks.udim( c ) <= max_dim ) )
    q.push_back( c );

  // Exit directly if min dimension is obtained.
  if ( ks.udim( c ) == min_dim ) return;

  // Extracts two first 1-lower incident cells and decompose them.
  Kn_uid d1;
  Kn_uid d2; 
  KnSpace::dir_iterator p = ks.ubegin_dirs( c ); 
  if ( ! ks.uisMax( c, *p ) ) 
    {
      d1 = ks.ulowIncident( c, *p, true );
      ulowIncidentCells( q, ks, d1, max_dim, min_dim );
    }
  if ( ! ks.uisMin( c, *p ) )
    {
      d2 = ks.ulowIncident( c, *p, false );
      ulowIncidentCells( q, ks, d2, max_dim, min_dim );
    }
  ++p;

  // Extracts other 1-lower incident cells.
  while ( ! p.end() )
    {
      if ( ! ks.uisMax( c, *p ) ) 
	{
	  d1 = ks.ulowIncident( c, *p, true );
	  q.push_back( d1 );
	}
      if ( ! ks.uisMin( c, *p ) )
	{
	  d2 = ks.ulowIncident( c, *p, false );
	  q.push_back( d2 );
	}
      
      ++p;
    }
 
}



///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
KnTools::~KnTools()
{
}

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
KnTools::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[KnTools]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
KnTools::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
