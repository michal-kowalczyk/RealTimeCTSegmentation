///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CIteratorOnExplicitDigitalSurface.cxx
//
// Creation : 2011/05/04
//
// Version : 2011/05/04
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
//	2011/05/04 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/C4CIteratorOnExplicitDigitalSurface.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/C4CIteratorOnExplicitDigitalSurface.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const C4CIteratorOnExplicitDigitalSurface_RCS_ID = "@(#)class C4CIteratorOnExplicitDigitalSurface definition.";

///////////////////////////////////////////////////////////////////////////////
// class C4CIteratorOnExplicitDigitalSurface
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
C4CIteratorOnExplicitDigitalSurface::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CIteratorOnExplicitDigitalSurface]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
C4CIteratorOnExplicitDigitalSurface::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
