///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : OctantChange.cxx
//
// Creation : 2010/02/11
//
// Version : 2010/02/11
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
//	2010/02/11 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/OctantChange.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/OctantChange.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const OctantChange_RCS_ID = "@(#)class OctantChange definition.";



///////////////////////////////////////////////////////////////////////////////
// class OctantChange
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::OctantChange::~OctantChange()
{
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::OctantChange::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[OctantChange "
	      << "{" << isSymXeq0() << isSymYeq0() << isSymXeqY() << "}]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::OctantChange::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
