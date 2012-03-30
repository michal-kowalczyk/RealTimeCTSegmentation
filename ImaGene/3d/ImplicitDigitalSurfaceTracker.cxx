///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ImplicitDigitalSurfaceTracker.cxx
//
// Creation : 2005/01/18
//
// Version : 2005/01/18
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
//	2005/01/18 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/3d/ImplicitDigitalSurfaceTracker.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/3d/ImplicitDigitalSurfaceTracker.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ImplicitDigitalSurfaceTracker_RCS_ID = "@(#)class ImplicitDigitalSurfaceTracker definition.";



///////////////////////////////////////////////////////////////////////////////
// class ImplicitDigitalSurfaceTracker
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ImplicitDigitalSurfaceTracker::~ImplicitDigitalSurfaceTracker()
{
}


/**
 * Constructor. The tracker is not valid.
 * see init
 */
ImaGene::ImplicitDigitalSurfaceTracker::ImplicitDigitalSurfaceTracker()
  : m_ids( 0 )
{
}


/**
 * Initializes the implicit digital surface tracker to move on the
 * implicit digital surface [ids].
 *
 * @param ids any implicit digital surface.
 */
void
ImaGene::ImplicitDigitalSurfaceTracker::init
( const ImplicitDigitalSurface* ids )
{
  m_ids = ids;
  if ( ids != 0 )
    m_neighborhood.init( ids->space(), 
			 &( ids->belAdjacency() ) );
}



///////////////////////////////////////////////////////////////////////////////
// ------------ Implementation of abstract services -----------------------


/**
 * @return the digital surface on which the tracker is moving.
 */
const ImaGene::DigitalSurface* 
ImaGene::ImplicitDigitalSurfaceTracker::surface() const
{
  return m_ids;
}



/**
 * @return a clone of 'this'.
 */
ImaGene::DigitalSurfaceTracker* 
ImaGene::ImplicitDigitalSurfaceTracker::clone() const
{
  ImplicitDigitalSurfaceTracker* idst = new ImplicitDigitalSurfaceTracker();
  idst->init( m_ids );
  idst->move( current() );
  idst->m_last_code = m_last_code;
  return idst;
}


/**
 * @return the current surfel at which the tracker is pointing.
 */
ImaGene::Kn_sid 
ImaGene::ImplicitDigitalSurfaceTracker::current() const
{
  return m_neighborhood.current();
}

    
/**
 * @return the orthogonal direction to the current surfel.
 */
uint
ImaGene::ImplicitDigitalSurfaceTracker::orthDir() const
{
  return m_neighborhood.orthDir();
}

    
/**
 * Moves the tracker to the surfel [s].
 * @param s any surfel on the surface.
 */
void
ImaGene::ImplicitDigitalSurfaceTracker::move( Kn_sid s )
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
ImaGene::ImplicitDigitalSurfaceTracker::adjacent( uint dir, bool pos )
{
  Kn_sid adj;
  m_last_code = m_neighborhood.adjacentOnAnyBdry( *m_ids,
						  dir, 
						  pos,
						  adj );
  return (m_last_code != 0) ? adj : 0;
}

    
/**
 * @return the move code associated to the last call to
 * 'adjacent'. When '0', there was no adjacent surfel, when '=n'
 * (n=1..3), the adjacent surfel is the n-th interior surfel.
 * 
 * @see adjacent
 */
uint
ImaGene::ImplicitDigitalSurfaceTracker::lastCode() const
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
ImaGene::ImplicitDigitalSurfaceTracker::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ImplicitDigitalSurfaceTracker]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ImplicitDigitalSurfaceTracker::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
