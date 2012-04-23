///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : KnSpaceCoordScanner.cxx
//
// Creation : 2003/07/03
//
// Version : 2003/07/03
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
//	2003/07/03 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnSpaceCoordScanner.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/KnSpaceCoordScanner.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const KnSpaceCoordScanner_RCS_ID = "@(#)class KnSpaceCoordScanner definition.";

///////////////////////////////////////////////////////////////////////////////
// class KnSpaceCoordScanner
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
KnSpaceCoordScanner::~KnSpaceCoordScanner()
{
}

/**
 * Constructor.
 * The iterator object offers a simple way to scan a parallepipedic subspace
 * of a 'KnSpace'.
 * @param sp the space where the scanning is taking place.
 * @param lower the code of an element of [sp] (of any type).
 * @param upper the code of another element of [sp] (same type as [lower]).
 */
KnSpaceCoordScanner::KnSpaceCoordScanner
( const KnSpace & sp, Kn_uid lower, Kn_uid upper )
  : space( sp ), lower_bound( lower ), upper_bound( upper )
{
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
KnSpaceCoordScanner::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[KnSpaceCoordScanner]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
KnSpaceCoordScanner::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
