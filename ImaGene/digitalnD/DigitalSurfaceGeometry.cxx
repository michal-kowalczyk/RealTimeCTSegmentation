///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : DigitalSurfaceGeometry.cxx
//
// Creation : 2003/02/27
//
// Version : 2003/02/27
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
//	2003/02/27 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/DigitalSurfaceGeometry.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/DigitalSurfaceGeometry.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


const char* const DigitalSurfaceGeometry_RCS_ID = "@(#)class DigitalSurfaceGeometry definition.";

///////////////////////////////////////////////////////////////////////////////
// class DigitalSurfaceGeometry
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// class NormalOnSurfaceComputer
///////////////////////////////////////////////////////////////////////////////

void
ImaGene::NormalOnSurfaceComputer::computeTangentPlane
( DigitalSurfaceGeometry & surf_geom_ctxt, 
  Kn_sid b, Vector & vn,
  float & min_x_orth,
  float & max_x_orth )
{
  this->computeNormal( surf_geom_ctxt, b, vn );
  min_x_orth = max_x_orth = 0.0;
}


///////////////////////////////////////////////////////////////////////////////
// class TrivialNormalOnSurfaceComputer
///////////////////////////////////////////////////////////////////////////////

/**
 * Returns the trivial normal associated to the surfel [b] and
 * pointing inside.
 *
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
 */
void
ImaGene::TrivialNormalOnSurfaceComputer::computeNormal
( DigitalSurfaceGeometry & surf_geom_ctxt, 
  Kn_sid b, Vector & vn )
{
  surf_geom_ctxt.space()->snormalInside( b, vn );
}




///////////////////////////////////////////////////////////////////////////////
// Standard services - public :


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

