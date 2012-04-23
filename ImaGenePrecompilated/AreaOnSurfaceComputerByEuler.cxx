///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : AreaOnSurfaceComputerByEuler.cxx
//
// Creation : 2003/07/30
//
// Version : 2003/07/30
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
//	2003/07/30 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/AreaOnSurfaceComputerByEuler.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/AreaOnSurfaceComputerByEuler.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const AreaOnSurfaceComputerByEuler_RCS_ID = "@(#)class AreaOnSurfaceComputerByEuler definition.";

///////////////////////////////////////////////////////////////////////////////
// class AreaOnSurfaceComputerByEuler
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
AreaOnSurfaceComputerByEuler::~AreaOnSurfaceComputerByEuler()
{
}


/**
 * Constructor. Does nothing.
 */
AreaOnSurfaceComputerByEuler::AreaOnSurfaceComputerByEuler()
{
}


// ------------------------- Geometry services ------------------------------

/**
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @return the estimated area of the surfel [b].
 */
float
AreaOnSurfaceComputerByEuler::computeArea
( DigitalSurfaceGeometry & surf_geom_ctxt, 
  Kn_sid b )
{
  uint d = surf_geom_ctxt.space()->dim();
  Vector n( d );
  surf_geom_ctxt.normal( b, n );
  return fabs( n.ro( surf_geom_ctxt.space()->sorthDir( b ) ) );
}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
AreaOnSurfaceComputerByEuler::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[AreaOnSurfaceComputerByEuler]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
AreaOnSurfaceComputerByEuler::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
