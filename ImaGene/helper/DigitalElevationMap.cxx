///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : DigitalElevationMap.cxx
//
// Creation : 2010/05/21
//
// Version : 2010/05/21
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
//	2010/05/21 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/helper/DigitalElevationMap.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/DigitalElevationMap.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const DigitalElevationMap_RCS_ID = "@(#)class DigitalElevationMap definition.";



///////////////////////////////////////////////////////////////////////////////
// class DigitalElevationMap
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
ImaGene::DigitalElevationMap::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[DigitalElevationMap]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::DigitalElevationMap::OK() const
{
  return m_space != 0;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
