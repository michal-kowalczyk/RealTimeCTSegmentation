///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : CubicalCell.cxx
//
// Creation : 2008/03/19
//
// Version : 2008/03/19
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
//	2008/03/19 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/CubicalCell.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/CubicalCell.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const CubicalCell_RCS_ID = "@(#)class CubicalCell definition.";


/**
 * default dimension of the space.
 */
uint ImaGene::CubicalCell::default_dim = 0;


///////////////////////////////////////////////////////////////////////////////
// class CubicalCell
///////////////////////////////////////////////////////////////////////////////


void ImaGene::CubicalCell::testCubicalCell()
{
  CubicalCell::default_dim = 3;
  CubicalCell c;
  uint x[ 3 ] = { 17, 12, 5 };
  c.init( x, 3 );
  CubicalCell d( c, 3 );
  d.incident( 2, true );
  d.incident( 1, true );
  CubicalCell u( 53, 14, 13 );
  CubicalCell v( 45, 17, 33 );
  CubicalCellComparator ccc( 3 );
  map< CubicalCell, int, CubicalCellComparator > label( ccc );
  label[ c ] = 1;
  label[ d ] = 2;
  label[ u ] = 3;  
  label[ v ] = 4;
  for ( map< CubicalCell, int >::const_iterator it = label.begin();
	it != label.end();
	++it )
    {
      cout << " " << it->first.operator[]( 0 )
	   << " " << it->second;
    }
  std::deque<CubicalCell> voisins;
  //  u.lowIncidentCells1( voisins );
  CubicalCell::lowIncidentCells( voisins,
				 v, v.dim( 3 ),
				 0, 3, 3 );
  cout << endl;
  cout << "closure( 45, 17, 33 )=";
  for ( std::deque<CubicalCell>::const_iterator it2 = voisins.begin();
	it2 != voisins.end();
	++it2 )
    {
      cout << " (" << it2->operator[]( 0 )
	   << "," << it2->operator[]( 1 )
	   << "," << it2->operator[]( 2 ) << ")";
    }
  cout << endl;
}

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::CubicalCell::selfDisplay( ostream& that_stream ) const
{
  if ( m_x == 0 ) return;
  that_stream << "[CCell=("
	      << m_x[ 0 ];
  for ( uint i = 1; i < default_dim; ++i )
     that_stream << "," << m_x[ i ];
   that_stream << ")]";
}



/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::CubicalCell::OK() const
{
  return true;
}


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
