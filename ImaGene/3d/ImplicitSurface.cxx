///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ImplicitSurface.cxx
//
// Creation : 2005/01/17
//
// Version : 2005/01/17
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
//	2005/01/17 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/3d/ImplicitSurface.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/3d/ImplicitSurface.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ImplicitSurface_RCS_ID = "@(#)class ImplicitSurface definition.";



///////////////////////////////////////////////////////////////////////////////
// class ImplicitSurface
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ImplicitSurface::~ImplicitSurface()
{
}


/**
 * Returns the bounds of the implicit surface (isovalue 0), if known.
 * @param lower the lower bounds, when finite.
 * @param has_lower the type for lower bounds.
 * @param upper the upper bounds, when finite.
 * @param has_upper the type for upper bounds.
 * @return 'true' if every bound type is finite, 'false' otherwise.
 */
bool
ImaGene::ImplicitSurface::bounds
( float lower[], BoundType has_lower[],
  float upper[], BoundType has_upper[] ) const
{
  for ( uint i = 0; i < 3; ++i )
    {
      has_lower[ i ] = Unknown;
      has_upper[ i ] = Unknown;
    }
  return false;
}






///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ImplicitSurface::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ImplicitSurface]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ImplicitSurface::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
