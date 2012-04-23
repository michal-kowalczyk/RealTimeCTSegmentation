///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CELength.cxx
//
// Creation : 2007/02/22
//
// Version : 2007/02/22
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
//	2007/02/22 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/C4CELength.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/C4CELength.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const C4CELength_RCS_ID = "@(#)class C4CELength definition.";



///////////////////////////////////////////////////////////////////////////////
// class C4CELength
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::C4CELength::~C4CELength()
{
  if ( m_elength != 0 ) delete[] m_elength;
}


/**
 * Constructor.
 * 
 * @param nb_surfel the number of surfels of the contour.
 */
ImaGene::C4CELength::C4CELength( uint nb_surfel )
  : m_mc( nb_surfel )
{
  m_elength = new double[ nb_surfel ];
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::C4CELength::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CELength]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::C4CELength::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
