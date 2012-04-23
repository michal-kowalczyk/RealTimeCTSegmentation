///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : FreemanChainTransform.cxx
//
// Creation : 2009/07/08
//
// Version : 2009/07/08
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
//	2009/07/08 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/FreemanChainTransform.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/FreemanChainTransform.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const FreemanChainTransform_RCS_ID = "@(#)class FreemanChainTransform definition.";



///////////////////////////////////////////////////////////////////////////////
// class FreemanChainTransform
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::FreemanChainTransform::~FreemanChainTransform()
{
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::FreemanChainTransform::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[FreemanChainTransform]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::FreemanChainTransform::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
