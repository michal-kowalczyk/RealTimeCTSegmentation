///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : KnSpaceScanner.cxx
//
// Creation : 2002/07/27
//
// Version : 2002/07/27
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
//	2002/07/27 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnSpaceScanner.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/KnSpaceScanner.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const KnSpaceScanner_RCS_ID = "@(#)class KnSpaceScanner definition.";

///////////////////////////////////////////////////////////////////////////////
// class KnSpaceScanner
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
KnSpaceScanner::~KnSpaceScanner()
{
  delete[] m_gotonext;
}


/**
 * Constructor.
 * The iterator object offers a simple way to scan a parallepipedic subspace
 * of a 'KnSpace'.
 * @param sp the space where the scanning is taking place.
 * @param lower the code of an element of [sp] (of any type).
 * @param upper the code of another element of [sp] (same type as [lower]).
 */
KnSpaceScanner::KnSpaceScanner( const KnSpace & sp,
				  Kn_uid lower,
				  Kn_uid upper )
  : space( sp ), lower_bound( lower ), upper_bound( upper ),
    m_gotonext( new Kn_uid[ sp.dim() ] )
{
  uint k;
  Kn_uid accu_gotonext = m_gotonext[ 0 ] = 1;
  Kn_uid last_of_row = lower;
  for ( k = 1; k < sp.dim(); ++k )
    {
      last_of_row = sp.ugetBound( last_of_row, upper, k - 1 );
      m_gotonext[ k ] = 
	sp.ugetIncr( lower, k ) // first of next row 
	- last_of_row           // last of preceding row
	- accu_gotonext;        // iterating causes to go one cell too far. 
      accu_gotonext += m_gotonext[ k ];
    }
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
KnSpaceScanner::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[KnSpaceScanner]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
KnSpaceScanner::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
