///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : SurfelNeighborhood.cxx
//
// Creation : 2005/01/10
//
// Version : 2005/01/10
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
//	2005/01/10 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/SurfelNeighborhood.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/SurfelNeighborhood.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const SurfelNeighborhood_RCS_ID = "@(#)class SurfelNeighborhood definition.";



///////////////////////////////////////////////////////////////////////////////
// class SurfelNeighborhood
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::SurfelNeighborhood::~SurfelNeighborhood()
{
}



/**
 * Constructor. The object is not valid.
 * @see setContext, init
 */
ImaGene::SurfelNeighborhood::SurfelNeighborhood()
  : m_ks( 0 )
{
}





///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::SurfelNeighborhood::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[SurfelNeighborhood]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::SurfelNeighborhood::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
