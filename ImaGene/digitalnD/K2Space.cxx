///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : K2Space.cxx
//
// Creation : 2008/12/16
//
// Version : 2008/12/16
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
//	2008/12/16 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/K2Space.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/K2Space.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const K2Space_RCS_ID = "@(#)class K2Space definition.";



///////////////////////////////////////////////////////////////////////////////
// class K2Space
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::K2Space::~K2Space()
{
}

/**
 * Constructor.
 * @param width the size along the x-axis (or 0-axis)
 * @param height the size along the y-axis (or 1-axis)
 */
ImaGene::K2Space::K2Space( uint width, uint height )
{
  Kn_size sizes[ 2 ] = { width, height };
  // Initialize KnSpace.
  KnSpace::init( 2, sizes );
  // Initialize K2Space.
  init( width, height );
}

/**
 * Initializes some tables specific to KnSpace in 2D.
 * @param width the size along the x-axis (or 0-axis)
 * @param height the size along the y-axis (or 1-axis)
 * @see m_fc_lut
 */
void 
ImaGene::K2Space::init( uint width, uint height )
{
    Kn_sid pointel = spointel( 0, 0, KnTypes::NEG );
    m_fc_lut[ 0 ].slinel = sincident( pointel, 0, true );
    m_fc_lut[ 1 ].slinel = sincident( pointel, 1, true );
    pointel = sopp( pointel );
    m_fc_lut[ 2 ].slinel = sincident( pointel, 0, true );
    m_fc_lut[ 3 ].slinel = sincident( pointel, 1, true );
    for ( uint i = 0; i < 4; ++i )
      {
	// displayKn_sid( m_fc_lut[ i ].slinel, cerr );
	// cerr << endl;
	m_fc_lut[ i ].ulinel = unsigns( m_fc_lut[ i ].slinel );
	// displayKn_uid( m_fc_lut[ i ].ulinel, cerr );
	// cerr << endl;
      }
}

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::K2Space::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[K2Space]";
} 

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::K2Space::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
