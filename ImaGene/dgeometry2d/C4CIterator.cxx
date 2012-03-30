///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CIterator.cxx
//
// Creation : 2004/12/01
//
// Version : 2004/12/01
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
//	2004/12/01 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/C4CIterator.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/C4CIterator.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const C4CIterator_RCS_ID = "@(#)class C4CIterator definition.";

///////////////////////////////////////////////////////////////////////////////
// class C4CIterator
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static services ------------------------------


/**
 * Counts the number of elements of the digital contour implicitly
 * defined by the iterator. Works also if the contour is open, but
 * in this case, the iterator is moved to the back of the contour.
 *
 * @param it any iterator on a digital contour (modified only if the contour is open).
 * @param is_open (returns) 'true' if the digital contour is open, 'false' otherwise.
 * @return the number of elements of the digital contour.
 */
uint
ImaGene::C4CIterator::size( C4CIterator & it, bool & is_open )
{
  uint n = 0;
  C4CIterator* it2 = it.clone();
  is_open = false;
  do
    {
      if ( it2->next() == 0 )
	{
	  is_open = true;
	  break;
	}
      ++n;
    }
  while ( ! it.equals( *it2 ) );
  delete it2;
  if ( is_open )
    {
      while ( it.previous() != 0 ) ++n;
      ++n;
    }
  return n;
}



/**
 * Counts the number of elements of the digital contour implicitly
 * defined by the iterator. Works also if the contour is open, but
 * in this case, the iterator is moved to the back of the contour.
 *
 * @param it any iterator on a digital contour (modified only if the contour is open).
 * @return the number of elements of the digital contour.
 */
uint
ImaGene::C4CIterator::size( C4CIterator & it )
{
  bool open;
  return size( it, open );
}




///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
