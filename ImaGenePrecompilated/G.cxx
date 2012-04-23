///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : G.cxx
//
// Creation : 2005/01/05
//
// Version : 2005/01/05
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
//	2005/01/05 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/G.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/G.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const G_RCS_ID = "@(#)class G definition.";



///////////////////////////////////////////////////////////////////////////////
// class G
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::G::~G()
{
}

/**
 * Construct a Gauss function with standard deviation [s].
 * @param s the standard deviation.
 */
ImaGene::G::G( double s )
{
  setSigma( s );
}



/**
 * Changes the value of the standard deviation.
 * @param s the standard deviation.
 */
void
ImaGene::G::setSigma(double s)
{
    m_s = s;
    m_c1 = 1.0 / (m_s * sqrt(2*M_PI));
    m_c2 = 1.0 / (2*m_s*m_s);
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::G::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[G]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::G::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
