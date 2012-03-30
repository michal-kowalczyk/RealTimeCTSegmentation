///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : DigitalSurface.cxx
//
// Creation : 2003/07/08
//
// Version : 2003/07/08
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
//	2003/07/08 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/DigitalSurface.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/DigitalSurface.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const DigitalSurface_RCS_ID = "@(#)class DigitalSurface definition.";

///////////////////////////////////////////////////////////////////////////////
// class DigitalSurface
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
DigitalSurface::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[DigitalSurface]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
DigitalSurface::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
