///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ObjectBoundary.cxx
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
//#include "LinAlg/LinAlg/Vector.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/dgeometry2d/DLine.h"
#include "ImaGene/dgeometry2d/C4CGeometry.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
#include "ImaGene/digitalnD/ObjectBoundary.h"
#include "ImaGene/digitalnD/C4CIteratorOnBdry.h"
#include "ImaGene/digitalnD/ObjectBoundaryTracker.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/ObjectBoundary.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


const char* const ObjectBoundary_RCS_ID = "@(#)class ObjectBoundary definition.";

///////////////////////////////////////////////////////////////////////////////
// class ObjectBoundary
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ObjectBoundary::~ObjectBoundary()
{
}

/**
 * Constructor. Represents the boundary of the set of spels [object]
 * considered with the [badj] bel adjacency.
 * @param badj any bel adjacency of the space where [object] is defined.
 * @param object the set of spels.
 */
ImaGene::ObjectBoundary::ObjectBoundary
( const BelAdjacency & badj, const KnCharSet & object )
  : m_badj( badj ), m_object( object )
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
ImaGene::ObjectBoundary::operator[]( Kn_sid surfel ) const
{
  const KnSpace & ks = *space();

  // ASSERT_ObjectBoundary( ks.sisSurfel( surfel ) );

  uint orth_dir = ks.sorthDir( surfel );
  bool orth_direct = ks.sdirect( surfel, orth_dir );
  Kn_uid vox_int = ks.unsigns( ks.sincident( surfel, orth_dir, orth_direct ) );
  Kn_uid vox_ext = ks.uadjacent( vox_int, orth_dir, ! orth_direct );

  return ( m_object[ vox_int ] && ! m_object[ vox_ext ] );
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
ImaGene::ObjectBoundary::newC4CIterator( Kn_sid surfel, uint dir ) const
{
  BelAdjacency::bel_iterator biter = m_badj.sstart( surfel, dir );
  return new C4CIteratorOnBdry( biter, m_object );
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
ImaGene::ObjectBoundary::newTracker( Kn_sid surfel ) const
{
  ObjectBoundaryTracker* obj_tracker = new ObjectBoundaryTracker();
  obj_tracker->init( this );
  obj_tracker->move( surfel );
  return obj_tracker;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ObjectBoundary::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ObjectBoundary]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ObjectBoundary::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////


