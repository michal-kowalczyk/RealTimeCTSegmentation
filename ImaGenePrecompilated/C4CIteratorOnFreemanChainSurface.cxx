///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CIteratorOnFreemanChainSurface.cxx
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
#include "ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const C4CIteratorOnFreemanChainSurface_RCS_ID = "@(#)class C4CIteratorOnFreemanChainSurface definition.";



///////////////////////////////////////////////////////////////////////////////
// class C4CIteratorOnFreemanChainSurface
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
ImaGene::C4CIteratorOnFreemanChainSurface::next()
{
  return m_it.next();
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
ImaGene::C4CIteratorOnFreemanChainSurface::previous()
{
  return m_it.previous();
}


/**
 * @return the current surfel the iterator is pointing at.
 */
ImaGene::Kn_sid 
ImaGene::C4CIteratorOnFreemanChainSurface::current() const
{
  Kn_size xy[ 2 ];
  // Khalimsky coordinates of pixel.
  Vector2i base_pt( *( m_it.getIterator() ) );
  xy[ 0 ] = 2 * base_pt.x();
  xy[ 1 ] = 2 * base_pt.y();
//   xy[ 0 ] = 2 * base_pt.x() + 1;
//   xy[ 1 ] = 2 * base_pt.y() + 1;
  Kn_sid pointel = m_ks->skcode( xy, KnTypes::NEG );
  //Kn_sid pixel;
  Kn_sid bel;
  switch ( m_it.getIterator().getCode() )
    {
    case 0: 
      bel = m_ks->sincident( pointel, 0, true );
      break;
    case 1:
      bel = m_ks->sincident( pointel, 1, true );
      break;
    case 2:
      bel = m_ks->sincident( pointel, 0, false );
      break;
    case 3:
      bel = m_ks->sincident( pointel, 1, false );
      break;
    default:
      bel = 0;
//     case 0: 
//       pixel = m_ks->skcode( xy, KnTypes::POS );
//       bel = m_ks->sincident( pixel, 1, false );
//       break;
//     case 1:
//       xy[ 0 ] -= 2;
//       pixel = m_ks->skcode( xy, KnTypes::POS );
//       bel = m_ks->sincident( pixel, 0, true );
//       break;
//     case 2:
//       xy[ 0 ] -= 2;
//       xy[ 1 ] -= 2;
//       pixel = m_ks->skcode( xy, KnTypes::POS );
//       bel = m_ks->sincident( pixel, 1, true );
//       break;
//     case 3:
//       xy[ 1 ] -= 2;
//       pixel = m_ks->skcode( xy, KnTypes::POS );
//       bel = m_ks->sincident( pixel, 0, false );
//       break;
//     default:
//       bel = 0;
    }
  return bel;
}
    
/**
 * @return the current track direction.
 */
uint
ImaGene::C4CIteratorOnFreemanChainSurface::trackDir() const
{
  return m_it.getIterator().getCode() & 1;
}
    
/**
 * @return the current orthogonal direction.
 */
uint
ImaGene::C4CIteratorOnFreemanChainSurface::orthDir() const
{
  return ( m_it.getIterator().getCode() + 1 ) & 1;
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::C4CIteratorOnFreemanChainSurface::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CIteratorOnFreemanChainSurface]"
	      << "(" << m_it.getIterator().getCode()
	      << ":t" << trackDir()
	      << ",o" << orthDir()
	      << ":" << m_it.getIterator().get().x()
	      << "," << m_it.getIterator().get().y()
	      << ")" << endl;
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::C4CIteratorOnFreemanChainSurface::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
