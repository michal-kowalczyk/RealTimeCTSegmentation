///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CIteratorOnFreemanChain.cxx
//
// Creation : 2008/05/22
//
// Version : 2008/05/22
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
//	2008/05/22 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const C4CIteratorOnFreemanChain_RCS_ID = "@(#)class C4CIteratorOnFreemanChain definition.";



///////////////////////////////////////////////////////////////////////////////
// class C4CIteratorOnFreemanChain
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :


/**
 * Moves the iterator on the 4-connected contour to the next
 * position.  @return 0 if the move was impossible, 1 if it was a
 * move toward the interior, 2 if it was a straight movement, 3 if
 * it was a move toward the exterior.  NB: If the C4CIterator is
 * moving over a digital surface, then 'next' means moving along a
 * direct tracking direction.
 */
uint
ImaGene::C4CIteratorOnFreemanChain::next()
{
  uint ccur = m_it.getCode();
  m_it.next();
  if ( m_it == m_it.getChain()->end() )
    if ( m_loop )
      {
	m_it = m_it.getChain()->begin();
      }
    else
      {
	m_it.previous();
	return 0;
      }
  uint cnext = m_it.getCode();
  return ( 6 + ccur - cnext ) % 4; // cf FFMTable
}  
  
/**
 * Moves the iterator on the 4-connected contour to the previous
 * position.  @return 0 if the move was impossible, 1 if it was a
 * move toward the interior, 2 if it was a straight movement, 3 if
 * it was a move toward the exterior.  NB: If the C4CIterator is
 * moving over a digital surface, then 'previous' means moving
 * along an indirect tracking direction.
 */
uint 
ImaGene::C4CIteratorOnFreemanChain::previous()
{
  if ( ( ! m_loop ) && ( m_it == m_it.getChain()->begin() ) )
    return 0;
  if ( ( m_loop ) && ( m_it == m_it.getChain()->end() ) )
    m_it = m_it.getChain()->begin(); // JOL
  uint ccur = m_it.getCode();
  m_it.previousInLoop();
  uint cprev = m_it.getCode();
  return ( 6 - ccur + cprev ) % 4; // cf FBMTable
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::C4CIteratorOnFreemanChain::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CIteratorOnFreemanChain]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::C4CIteratorOnFreemanChain::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
