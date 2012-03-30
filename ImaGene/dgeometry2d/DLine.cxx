///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : DLine.cxx
//
// Creation : 2003/02/12
//
// Version : 2003/02/12
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
//	2003/02/12 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/DLine.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/DLine.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const DLine_RCS_ID = "@(#)class DLine definition.";

///////////////////////////////////////////////////////////////////////////////
// class DLine
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static services ------------------------------


/**
 * Computes the median line of [l1] and [l2], defined by the intersection of
 * the upper and ower leaning lines of [l1] and [l2] (forms a parallelogram).
 *
 * @pre l1 and l2 concurs and are defined in the same frame.
 * @param l1 the first discrete line
 * @param l2 the second discrete line
 * @return the median discrete line (not necessarily 4-connected).
 */
DLine
DLine::medianLine( const DLine & l1, const DLine & l2 )
{

  if ( ( l1.a == 0 ) && ( l1.b == 1 ) && ( l2.a == 0 ) && ( l2.b == 1 ) )
    {
      return l1;
    }
  
//   int d = l2.a * l1.b - l1.a * l2.b;
  
//   ASSERT_DLine( d != 0 );
  
  int dmu1 = l1.mup - l1.mu;
  int dmu2 = l2.mup - l2.mu;

  return DLine( l1.a * dmu2 + l2.a * dmu1,
		l1.b * dmu2 + l2.b * dmu1,
		l1.mu * dmu2 + l2.mu * dmu1,
		l1.mup * dmu2 + l2.mup * dmu1 );
}


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
DLine::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[DLine (" << a << "," << b 
    << "," << mu << "," << mup << ") ]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
DLine::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
