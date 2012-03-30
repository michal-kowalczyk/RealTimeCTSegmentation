///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ExplicitDigitalSurfaceTracker.cxx
//
// Creation : 2011/05/04
//
// Version : 2011/05/04
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
//	2011/05/04 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/ExplicitDigitalSurfaceTracker.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/ExplicitDigitalSurfaceTracker.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ExplicitDigitalSurfaceTracker_RCS_ID = "@(#)class ExplicitDigitalSurfaceTracker definition.";



///////////////////////////////////////////////////////////////////////////////
// class ExplicitDigitalSurfaceTracker
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ExplicitDigitalSurfaceTracker::~ExplicitDigitalSurfaceTracker()
{
}


/**
 * Constructor. The tracker is not valid.
 * see init
 */
ImaGene::ExplicitDigitalSurfaceTracker::ExplicitDigitalSurfaceTracker()
  : m_eds( 0 )
{
}


void
ImaGene::ExplicitDigitalSurfaceTracker::init
( const ExplicitDigitalSurface* eds )
{
  m_eds = eds;
  if ( eds != 0 )
    m_neighborhood.init( eds->space(), 
			 &( eds->belAdjacency() ) );
}




///////////////////////////////////////////////////////////////////////////////
// ------------ Implementation of abstract services -----------------------


/**
 * @return the digital surface on which the tracker is moving.
 */
const ImaGene::DigitalSurface* 
ImaGene::ExplicitDigitalSurfaceTracker::surface() const
{
  return m_eds;
}



/**
 * @return a clone of 'this'.
 */
ImaGene::DigitalSurfaceTracker* 
ImaGene::ExplicitDigitalSurfaceTracker::clone() const
{
  ExplicitDigitalSurfaceTracker* eds = new ExplicitDigitalSurfaceTracker();
  eds->init( m_eds );
  eds->move( current() );
  eds->m_last_code = m_last_code;
  return eds;
}


/**
 * @return the current surfel at which the tracker is pointing.
 */
ImaGene::Kn_sid 
ImaGene::ExplicitDigitalSurfaceTracker::current() const
{
  return m_neighborhood.current();
}

    
/**
 * @return the orthogonal direction to the current surfel.
 */
uint
ImaGene::ExplicitDigitalSurfaceTracker::orthDir() const
{
  return m_neighborhood.orthDir();
}

    
/**
 * Moves the tracker to the surfel [s].
 * @param s any surfel on the surface.
 */
void
ImaGene::ExplicitDigitalSurfaceTracker::move( Kn_sid s )
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
ImaGene::ExplicitDigitalSurfaceTracker::adjacent( uint dir, bool pos )
{
  Kn_sid adj = 0;
  m_last_code = 
    m_neighborhood.adjacentOnOrientedSurface( m_eds->surface(),
					      dir, 
					      pos,
					      adj );
  return m_last_code ? adj : 0;
}

    
/**
 * @return the move code associated to the last call to
 * 'adjacent'. When '0', there was no adjacent surfel, when '=n'
 * (n=1..3), the adjacent surfel is the n-th interior surfel.
 * 
 * @see adjacent
 */
uint
ImaGene::ExplicitDigitalSurfaceTracker::lastCode() const
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
ImaGene::ExplicitDigitalSurfaceTracker::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ExplicitDigitalSurfaceTracker]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ExplicitDigitalSurfaceTracker::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
