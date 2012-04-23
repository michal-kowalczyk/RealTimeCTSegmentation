///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ImplicitExtension.cxx
//
// Creation : 2008/02/27
//
// Version : 2008/02/27
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
//	2008/02/27 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/3d/ImplicitExtension.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/3d/ImplicitExtension.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ImplicitExtension_RCS_ID = "@(#)class ImplicitExtension definition.";



///////////////////////////////////////////////////////////////////////////////
// class ImplicitExtension
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ImplicitExtension::~ImplicitExtension()
{
}

/**
 * Constructor. 
 */
ImaGene::ImplicitExtension::ImplicitExtension()
  : m_is( 0 ), m_d( 0 )
{
}

/**
 * Initializes the object by connecting it to an existing implicit
 * surface.
 *
 * @param is the implicit surface to cast into a higher dimensional space.
 * @param d the dimension of the implicit surface [is]
 */
void 
ImaGene::ImplicitExtension::init( ImplicitSurface* is, uint d )
{
  m_is = is;
  m_d = d;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ImplicitExtension::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ImplicitExtension]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ImplicitExtension::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
