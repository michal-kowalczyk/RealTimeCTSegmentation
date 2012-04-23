///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : SternBrocot.cxx
//
// Creation : 2011/02/28
//
// Version : 2011/02/28
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
//	2011/02/28 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/SternBrocot.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/SternBrocot.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const SternBrocot_RCS_ID = "@(#)class SternBrocot definition.";



///////////////////////////////////////////////////////////////////////////////
// class SternBrocot
///////////////////////////////////////////////////////////////////////////////
ImaGene::SternBrocot::Node 
ImaGene::SternBrocot::myVirtualZeroOverOne( 0, 1, 0, -2, 
					    0, 0, 0, &myOneOverZero,
					    &myOneOverZero );
ImaGene::SternBrocot::Node 
ImaGene::SternBrocot::myOneOverZero( 1, 0, 0, -1, 
				     // &myVirtualZeroOverOne, 0, &myOneOverOne, 0,
				     &myZeroOverOne, 0, &myOneOverOne, 0,
				     &myZeroOverOne );
ImaGene::SternBrocot::Node 
ImaGene::SternBrocot::myZeroOverOne( 0, 1, 0, 0, 
				     // &myVirtualZeroOverOne, &myOneOverZero, 
				     &myZeroOverOne, &myOneOverZero, 
				     0, &myOneOverOne,
				     &myOneOverZero );
ImaGene::SternBrocot::Node 
ImaGene::SternBrocot::myOneOverOne( 1, 1, 1, 0, 
				    &myZeroOverOne, &myOneOverZero, 0, 0,
				    &myOneOverOne );

ImaGene::SternBrocot::SBFraction
ImaGene::SternBrocot::fraction
( unsigned int p, unsigned int q,
  SBFraction ancestor )
{
  // special case 1/0
  if ( ( p == 1 ) && ( q == 0 ) ) return oneOverZero();
  // other positive fractions
  while ( ! ancestor.equals( p, q ) )
    {
      ASSERT_SternBrocot( ( p + q ) >= ( ancestor.p() + ancestor.q() )
			  && "[ImaGene::SternBrocot::fraction] bad ancestor." );
      ancestor = ancestor.lessThan( p, q ) 
	? ancestor.right()
	: ancestor.left();
    }
  return ancestor;
}


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::SternBrocot::~SternBrocot()
{
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::SternBrocot::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[SternBrocot]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::SternBrocot::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
