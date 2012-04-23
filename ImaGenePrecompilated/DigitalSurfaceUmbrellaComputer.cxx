///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : DigitalSurfaceUmbrellaComputer.cxx
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
#include "ImaGene/digitalnD/DigitalSurface.h"
#include "ImaGene/digitalnD/DigitalSurfaceUmbrellaComputer.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/DigitalSurfaceUmbrellaComputer.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const DigitalSurfaceUmbrellaComputer_RCS_ID = "@(#)class DigitalSurfaceUmbrellaComputer definition.";



///////////////////////////////////////////////////////////////////////////////
// class DigitalSurfaceUmbrellaComputer
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ----------------------- Static services ------------------------------


/**
 * Computes the complete umbrella from an initialized
 * 'DigitalSurfaceUmbrellaComputer'.
 *
 * @param umbrella an initialized umbrella (with a surface and
 * valid pivot).
 *
 * @param faces (modified) a pointer on a big enough array for
 * storing all visited faces.
 * @param nb (returns) the number of visited faces (in 3D, max is 6).
 *
 * @return 'true' if the umbrella is a loop, 'false' otherwise.
 */
bool
ImaGene::DigitalSurfaceUmbrellaComputer::computeUmbrellaFaces
( DigitalSurfaceUmbrellaComputer & umbrella,
  Kn_sid* faces,
  uint & nb )
{
  Kn_sid s1 = umbrella.face();
  Kn_sid s = s1;
  nb = 0;
  bool loop = true;
  do
    {
      faces[ nb ] = s;
      ++nb;
      if ( umbrella.next() == 0 )
	{
	  loop = false;
	  break;
	}
      s = umbrella.face();
//       if ( umbrella.pivot() != p )
// 	cerr << "[ImaGene::DigitalSurfaceUmbrellaComputer::computeUmbrellaFaces] "
// 	     << "Error: unvalid pivot." << endl;
    }
  while ( s != s1 );
  return loop;
}


/**
 * Computes the complete umbrella from an initialized
 * 'DigitalSurfaceUmbrellaComputer'.
 *
 * @param umbrella an initialized umbrella (with a surface and
 * valid pivot).
 *
 * @param states (modified) a pointer on a big enough array for
 * storing all umbrella states.
 * @param nb (returns) the number of visited faces (in 3D, max is 6).
 *
 * @return 'true' if the umbrella is a loop, 'false' otherwise.
 */
bool
ImaGene::DigitalSurfaceUmbrellaComputer::computeUmbrella
( DigitalSurfaceUmbrellaComputer & umbrella,
  State* states,
  uint & nb )
{
  Kn_sid s1 = umbrella.face();
  Kn_sid s = s1;
  nb = 0;
  bool loop = true;
  do
    {
      umbrella.getState( states[ nb ] );
      ++nb;
      if ( umbrella.next() == 0 )
	{
	  loop = false;
	  break;
	}
      s = umbrella.face();
//       if ( umbrella.pivot() != p )
// 	cerr << "[ImaGene::DigitalSurfaceUmbrellaComputer::computeUmbrella] "
// 	     << "Error: unvalid pivot." << endl;
    }
  while ( s != s1 );
  return loop;
}



/**
 * Computes the complete reversed umbrella from an initialized
 * 'DigitalSurfaceUmbrellaComputer'.
 *
 * @param umbrella an initialized umbrella (with a surface and
 * valid pivot).
 *
 * @param states (modified) a pointer on a big enough array for
 * storing all umbrella states.
 * @param nb (returns) the number of visited faces (in 3D, max is 6).
 *
 * @return 'true' if the umbrella is a loop, 'false' otherwise.
 */
bool
ImaGene::DigitalSurfaceUmbrellaComputer::computeReversedUmbrella
( DigitalSurfaceUmbrellaComputer & umbrella,
  State* states,
  uint & nb )
{
  Kn_sid s1 = umbrella.face();
  Kn_sid s = s1;
  nb = 0;
  bool loop = true;
  do
    {
      umbrella.getState( states[ nb ] );
      ++nb;
      if ( umbrella.previous() == 0 )
	{
	  loop = false;
	  break;
	}
      s = umbrella.face();
    }
  while ( s != s1 );
  return loop;
}

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::DigitalSurfaceUmbrellaComputer::~DigitalSurfaceUmbrellaComputer()
{
  if ( m_tracker != 0 )
    delete m_tracker;
}


/**
 * Initializes the umbrella with a 'DigitalSurfaceTracker'.
 *
 * @param tracker a tracker on a digital surface with a valid
 * 'current()' surfel. (cloned).
 */
void 
ImaGene::DigitalSurfaceUmbrellaComputer::init( const DigitalSurfaceTracker & tracker )
{
  if ( m_tracker != 0 )
    delete m_tracker;
  m_tracker = tracker.clone();
  m_space = tracker.surface()->space();
}


/**
 * Tells where is the positive pivot to turn around. @see
 * DigitalSurfaceUmbrellaComputer.
 *
 * @param k the separator direction different from [j] and
 *
 * @param epsilon the orientation where to find the separator
 * pointing at the pivot.
 *
 * @param j the track direction where the next surfel of the umbrella is.
 */
void 
ImaGene::DigitalSurfaceUmbrellaComputer::setPivot( uint k, bool epsilon, uint j )
{
  m_track_dir = j;
  m_separator_dir = k;
  m_separator_orientation = epsilon;
}



/**
 * Puts the umbrella in the state [state]. The umbrella must have
 * been initialized with a 'DigitalSurfaceTracker' before and the
 * state must be consistent with the tracker. In particular, the
 * face of the state must lie on the same digital surface.
 *
 * @param state a state describing the face, the separator and the pivot.
 */
void
ImaGene::DigitalSurfaceUmbrellaComputer::setState( const State & state )
{
  ASSERT_DigitalSurfaceUmbrellaComputer( m_tracker != 0 );
  m_tracker->move( state.face );
  m_separator_dir = state.k;
  m_separator_orientation = state.epsilon;
  m_track_dir = state.j;
}


/**
 * Returns the state of the umbrella in the referenced object [state]. 
 *
 * @param state (returns) the current umbrella state describing
 * the face, the separator and the pivot.
 */
void 
ImaGene::DigitalSurfaceUmbrellaComputer::getState( State & state ) const
{
  state.face = face();
  state.k = m_separator_dir;
  state.epsilon = m_separator_orientation;
  state.j = m_track_dir;
}





///////////////////////////////////////////////////////////////////////////////
// ----------------------- Pivoting services ------------------------------


/**
 * Turns around the current pivot (positive turn).
 *
 * @return 0 if the move was impossible (nothing is updated)
 * otherwise returns the move code of the face.
 */
uint
ImaGene::DigitalSurfaceUmbrellaComputer::next()
{
  ASSERT_DigitalSurfaceUmbrellaComputer( m_tracker != 0 );
  
  uint j = trackDir();
  // Delta^mu_t = +p
  bool mu = m_space->sdirect( separator(), j );
  Kn_sid sp = m_tracker->adjacent( j, mu );
  if ( sp != 0 )
    { // Then the face has a successor.
      uint i = m_tracker->orthDir();
      Kn_sid s = face();
      m_track_dir = m_separator_dir;
      m_tracker->move( sp );
      if ( m_tracker->lastCode() != 2 ) // s and sp are not aligned.
	{
	  bool track_direct_orientation 
	    = m_space->sdirect( s, j );
	  bool nu = m_space->sdirect( sp, i );
	  m_separator_dir = i;
	  m_separator_orientation = ( mu == track_direct_orientation ) ^ nu;
	  //	  cerr << " [perp]";
	}
      else
	{
	  m_separator_dir = j;
	  m_separator_orientation = ! mu;
	  //	  cerr << " [aligned]";
	}
    }
  return m_tracker->lastCode();
}


/**
 * Turns around the current pivot (negative turn).
 *
 * @return 0 if the move was impossible (nothing is updated)
 * otherwise returns the move code of the face.
 */
uint 
ImaGene::DigitalSurfaceUmbrellaComputer::previous()
{
  ASSERT_DigitalSurfaceUmbrellaComputer( m_tracker != 0 );

//   cerr << "[ImaGene::DigitalSurfaceUmbrellaComputer::previous()] begin" << endl;
  
//   cerr << "(f=" << face()
//        << ",s=" << separator()
//        << ",p=" << pivot()
//        << ",i=" << m_tracker->orthDir()
//        << ",j=" << trackDir()
//        << ",k=" << separatorDir()
//        << ",E=" << separatorOrientation() 
//        << ")";
//   cerr << endl;
  uint j = trackDir();
  // Delta^mu_t = +p
  uint i = m_tracker->orthDir();
  bool mu = m_space->sdirect( separator(), j );
  Kn_sid sp = m_tracker->adjacent( m_separator_dir, m_separator_orientation );
  //  cerr << "(" << face() << "," << sp << ")";
  if ( sp != 0 )
    { // Then the face has a successor.
      uint k = m_separator_dir;
      m_separator_dir = j; 
      m_separator_orientation = mu;
      m_tracker->move( sp );
      m_track_dir = ( m_tracker->lastCode() != 2 )
	? i  // s and sp are not aligned.
	: k; // s and sp are aligned.
    }
//   cerr << "(f=" << face()
//        << ",s=" << separator()
//        << ",p=" << pivot()
//        << ",i=" << m_tracker->orthDir()
//        << ",j=" << trackDir()
//        << ",k=" << separatorDir()
//        << ",E=" << separatorOrientation() 
//        << ")";
//   cerr << endl;
//   cerr << "[ImaGene::DigitalSurfaceUmbrellaComputer::previous()] end" << endl;
  return m_tracker->lastCode();
}

/**
 * Go the adjacent umbrella, ie the one which shares the same
 * faces around the separator but whose pivot is symmetric around
 * the separator.
 *
 * @return 'true' if their was an adjacent umbrella, 'false' otherwise
 * (the umbrella is in the same state as before the call).
 */
bool 
ImaGene::DigitalSurfaceUmbrellaComputer::adjacent()
{
  ASSERT_DigitalSurfaceUmbrellaComputer( m_tracker != 0 );

  uint old_j = m_track_dir;
  if ( previous() ==  0 ) return false;
  bool mu = m_space->sdirect( separator(), m_track_dir );
  m_separator_dir = m_track_dir;
  m_separator_orientation = mu;
  m_track_dir = old_j;
  return true;
}


 

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::DigitalSurfaceUmbrellaComputer::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[DigitalSurfaceUmbrellaComputer]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::DigitalSurfaceUmbrellaComputer::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
