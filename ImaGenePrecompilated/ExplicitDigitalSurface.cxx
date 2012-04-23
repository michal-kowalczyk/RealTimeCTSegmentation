///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ExplicitDigitalSurface.cxx
//
// Creation : 2003/02/13
//
// Version : 2003/02/13
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
//	2003/02/13 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/ExplicitDigitalSurface.h"
#include "ImaGene/digitalnD/C4CIteratorOnExplicitDigitalSurface.h"
#include "ImaGene/digitalnD/ExplicitDigitalSurfaceTracker.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/ExplicitDigitalSurface.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


const char* const ExplicitDigitalSurface_RCS_ID = "@(#)class ExplicitDigitalSurface definition.";

///////////////////////////////////////////////////////////////////////////////
// class ExplicitDigitalSurface
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ExplicitDigitalSurface::~ExplicitDigitalSurface()
{
}

ImaGene::ExplicitDigitalSurface::ExplicitDigitalSurface
( const BelAdjacency & badj, const KnRCellSet & surfels )
  : m_badj( badj ), m_surfels( surfels )
{
}



///////////////////////////////////////////////////////////////////////////////
// ------------------------- set services -------------------------------
/**
 * Useful for 'SurfelNeighborhood::adjacentOnOrientedSurfaces'.
 *
 * @param surfel any surfel of the digital space.
 * @return 'true' if the surfel belongs to the boundary of the object, 'false' otherwise.
 */
bool
ImaGene::ExplicitDigitalSurface::operator[]( Kn_sid surfel ) const
{
  return m_surfels[ surfel ];
}



///////////////////////////////////////////////////////////////////////////////
// ------------------------- Factory services ------------------------------

/**
 * Returns an iterator pointing on [surfel] and moving along direction [dir]
 * in the direct tracking direction. Each movement is given as a direction
 * on a 4-connected contour.
 * @param surfel any surfel.
 * @param dir a tracking direction (different from the orthogonal direction to [surfel]).
 * @return a dyn. alloc. contour iterator (a 'C4CIteratorOnBdry').
 * @see C4CIteratorOnBdry
 */
ImaGene::C4CIteratorOnSurface* 
ImaGene::ExplicitDigitalSurface::newC4CIterator( Kn_sid surfel, uint dir ) const
{
  return new C4CIteratorOnExplicitDigitalSurface( *this, surfel, dir );
}


/**
 * Returns a surface tracker pointing on [surfel].
 *
 * @param surfel any surfel.
 *
 * @return a dyn. alloc. surface tracker (must be desallocated by
 * the caller).
 */
ImaGene::DigitalSurfaceTracker* 
ImaGene::ExplicitDigitalSurface::newTracker( Kn_sid surfel ) const
{
  ExplicitDigitalSurfaceTracker* eds_tracker
    = new ExplicitDigitalSurfaceTracker();
  eds_tracker->init( this );
  eds_tracker->move( surfel );
  return eds_tracker;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ExplicitDigitalSurface::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ExplicitDigitalSurface]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ExplicitDigitalSurface::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////


