///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : DigitalSurfaceUmbrella.cxx
//
// Creation : 2011/04/29
//
// Version : 2011/04/29
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
//	2011/04/29 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/DigitalSurface.h"
#include "ImaGene/digitalnD/DigitalSurfaceUmbrella.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/DigitalSurfaceUmbrella.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const DigitalSurfaceUmbrella_RCS_ID = "@(#)class DigitalSurfaceUmbrella definition.";



///////////////////////////////////////////////////////////////////////////////
// class DigitalSurfaceUmbrella
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
   Initializes the umbrella with a 'DigitalSurfaceTracker'.  Tells
   where is the positive pivot to turn around. @see
   DigitalSurfaceUmbrellaComputer.
   
   @param tracker a tracker on a digital surface with a valid
   'current()' surfel. (cloned).
   
   @param k the separator direction different from [j] and
   
   @param epsilon the orientation where to find the separator
   pointing at the pivot.
   
   @param j the track direction where the next surfel of the umbrella is.
   
   @return 'true' if this is a valid umbrella, or 'false' if the
   umbrella is open (and thus not valid).
*/
bool
ImaGene::DigitalSurfaceUmbrella::
init( const DigitalSurfaceTracker & tracker,
      uint k, bool epsilon, uint j )
{
  DigitalSurfaceUmbrellaComputer & computer = m_start.m_computer;
  computer.init( tracker );
  computer.setPivot( k, epsilon, j );
  // Determine is the umbrella is valid.
  Kn_sid s1 = computer.face();
  Kn_sid s = s1;
  Kn_sid min = s1;
  uint nb = 0;
  do
    {
      ASSERT_DigitalSurfaceUmbrella( tracker.surface()->space()->sisSurfel( s ) );
      if ( computer.next() == 0 )
	return false; // open umbrella.
      ++nb;
      s = computer.face();
      if ( s < min ) min = s;
    }
  while ( ( s != s1 ) && ( nb <= 6 ) );
  if ( nb > 6 ) 
    cerr << "[DigitalSurfaceUmbrella::init] error umbrella > 6." << endl;
  // Place the computer at the minimal position.
  else 
    while ( computer.face() != min ) computer.next();
  return true;
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::DigitalSurfaceUmbrella::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[DigitalSurfaceUmbrella]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::DigitalSurfaceUmbrella::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
