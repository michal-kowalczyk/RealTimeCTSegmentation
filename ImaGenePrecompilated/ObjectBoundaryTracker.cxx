///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ObjectBoundaryTracker.cxx
//
// Creation : 2005/01/11
//
// Version : 2005/01/11
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
//	2005/01/11 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/ObjectBoundaryTracker.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/ObjectBoundaryTracker.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ObjectBoundaryTracker_RCS_ID = "@(#)class ObjectBoundaryTracker definition.";



///////////////////////////////////////////////////////////////////////////////
// class ObjectBoundaryTracker
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ObjectBoundaryTracker::~ObjectBoundaryTracker()
{
}


/**
 * Constructor. The tracker is not valid.
 * see init
 */
ImaGene::ObjectBoundaryTracker::ObjectBoundaryTracker()
  : m_objectbdry( 0 )
{
}


/**
 * Initializes the object boundary tracker to move on the object boundary
 * [objectbdry].
 *
 * @param objectbdry any object boundary.
 */
void
ImaGene::ObjectBoundaryTracker::init( const ObjectBoundary* objectbdry )
{
  m_objectbdry = objectbdry;
  if ( objectbdry != 0 )
    m_neighborhood.init( objectbdry->space(), 
			 &( objectbdry->belAdjacency() ) );
}




///////////////////////////////////////////////////////////////////////////////
// ------------ Implementation of abstract services -----------------------


/**
 * @return the digital surface on which the tracker is moving.
 */
const ImaGene::DigitalSurface* 
ImaGene::ObjectBoundaryTracker::surface() const
{
  return m_objectbdry;
}



/**
 * @return a clone of 'this'.
 */
ImaGene::DigitalSurfaceTracker* 
ImaGene::ObjectBoundaryTracker::clone() const
{
  ObjectBoundaryTracker* obt = new ObjectBoundaryTracker();
  obt->init( m_objectbdry );
  obt->move( current() );
  obt->m_last_code = m_last_code;
  return obt;
}


/**
 * @return the current surfel at which the tracker is pointing.
 */
ImaGene::Kn_sid 
ImaGene::ObjectBoundaryTracker::current() const
{
  return m_neighborhood.current();
}

    
/**
 * @return the orthogonal direction to the current surfel.
 */
uint
ImaGene::ObjectBoundaryTracker::orthDir() const
{
  return m_neighborhood.orthDir();
}

    
/**
 * Moves the tracker to the surfel [s].
 * @param s any surfel on the surface.
 */
void
ImaGene::ObjectBoundaryTracker::move( Kn_sid s )
{
  m_neighborhood.move( s );
}


/**
 * Computes the surfel adjacent to 'current()' in the direction
 * [dir] along orientation [pos]. The move code (0-3) is stored
 * for possible later use.
 *
 * @param dir any direction different from 'orthDir()'.
 * @param pos when 'true' look in positive direction along [track_dir] axis, 'false' look in negative direction.
 * @return the corresponding adjacent surfel or 0 if there was none.
 * @see lastCode
 */
ImaGene::Kn_sid 
ImaGene::ObjectBoundaryTracker::adjacent( uint dir, bool pos )
{
  Kn_sid adj;
  m_last_code = m_neighborhood.adjacentOnAnyBdry( m_objectbdry->object(),
						  dir, 
						  pos,
						  adj );
  return adj;
}

    
/**
 * @return the move code associated to the last call to
 * 'adjacent'. When '0', there was no adjacent surfel, when '=n'
 * (n=1..3), the adjacent surfel is the n-th interior surfel.
 * 
 * @see adjacent
 */
uint
ImaGene::ObjectBoundaryTracker::lastCode() const
{
  return m_last_code;
}





///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ObjectBoundaryTracker::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ObjectBoundaryTracker]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ObjectBoundaryTracker::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
