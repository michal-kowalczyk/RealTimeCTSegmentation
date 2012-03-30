///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CIteratorOnBdry.cxx
//
// Creation : 2003/02/12
//
// Version : 2003/02/12
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
//	2003/02/12 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/C4CGeometry.h"
#include "ImaGene/dgeometry2d/DLine.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/digitalnD/C4CIteratorOnBdry.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/C4CIteratorOnBdry.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const C4CIteratorOnBdry_RCS_ID = "@(#)class C4CIteratorOnBdry definition.";

///////////////////////////////////////////////////////////////////////////////
// class C4CIteratorOnBdry
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Other services ---------------------------------

  
///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
C4CIteratorOnBdry::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CIteratorOnBdry]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
C4CIteratorOnBdry::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
