///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CIteratorOnImplicitDigitalSurface.cxx
//
// Creation : 2005/01/18
//
// Version : 2005/01/18
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
//	2005/01/18 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/3d/C4CIteratorOnImplicitDigitalSurface.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/3d/C4CIteratorOnImplicitDigitalSurface.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const C4CIteratorOnImplicitDigitalSurface_RCS_ID = "@(#)class C4CIteratorOnImplicitDigitalSurface definition.";



///////////////////////////////////////////////////////////////////////////////
// class C4CIteratorOnImplicitDigitalSurface
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::C4CIteratorOnImplicitDigitalSurface::~C4CIteratorOnImplicitDigitalSurface()
{
}

/**
 * Constructor. The iterator is not valid.
 * @see init, start
 */
ImaGene::C4CIteratorOnImplicitDigitalSurface::C4CIteratorOnImplicitDigitalSurface()
  : m_ids( 0 )
{
}



/**
 * Initializes the iterator on implicit digital surface to move on the
 * implicit digital surface [ids].
 *
 * @param ids any implicit digital surface.
 */
void
ImaGene::C4CIteratorOnImplicitDigitalSurface::init
( const ImplicitDigitalSurface* ids )
{
  m_ids = ids;
  if ( ids != 0 )
    m_neighborhood.init( ids->space(),
			 &( ids->belAdjacency() ) );
}

      
/**
 * Starts the iterator at surfel [s] in track direction [track_dir].
 *
 * @param s any surfel of the implicit digital surface.
 * @param track_dir any direction different from 'orthDir( s )'.
 */
void
ImaGene::C4CIteratorOnImplicitDigitalSurface::start
( Kn_sid s, uint track_dir )
{
  ASSERT_C4CIteratorOnImplicitDigitalSurface( m_ids != 0 );
  m_neighborhood.move( s );
  m_track_dir = track_dir;
}




// ----------------------- C4CIteratorOnSurface services ----------------

/**
 * @return a clone of the current iterator.
 */
ImaGene::C4CIterator* 
ImaGene::C4CIteratorOnImplicitDigitalSurface::clone() const
{
  C4CIteratorOnImplicitDigitalSurface* it =
    new C4CIteratorOnImplicitDigitalSurface();
  it->init( m_ids );
  if ( it->m_ids != 0 )
    it->start( current(), trackDir() );
  return it;
}


/**
 * @param other any other iterator.
 * @return 'true' if [other] points to the same location as 'this'.
 */
bool 
ImaGene::C4CIteratorOnImplicitDigitalSurface::equals
( const C4CIterator & other ) const
{
  const C4CIteratorOnImplicitDigitalSurface & it =
    dynamic_cast<const C4CIteratorOnImplicitDigitalSurface &>( other );
  ASSERT_C4CIteratorOnImplicitDigitalSurface( m_ids == it.m_ids );
  return ( current() == it.current() ) && ( trackDir() == it.trackDir() );
}

    
/**
 * Moves the iterator on the 4-connected contour to the next position.
 * @return 0 if the move was impossible, 1 if it was a move toward the interior, 2 if it was a straight movement, 3 if it was a move toward the exterior.
 * NB: If the C4CIterator is moving over a digital surface, then 'next'
 * means moving along a direct tracking direction.
 */
uint 
ImaGene::C4CIteratorOnImplicitDigitalSurface::next()
{
  Kn_sid adj;
  uint i = orthDir();
  uint j = trackDir();
  bool direct_j = m_ids->space()->sdirect( current(), j );
  uint code = m_neighborhood.adjacentOnAnyBdry( *m_ids,
						j,
						direct_j,
						adj );
  if ( code != 0 )
    {
      m_neighborhood.move( adj );
      if ( orthDir() == j )
	m_track_dir = i;
    }
  return code;
}

    
/**
 * Moves the iterator on the 4-connected contour to the previous position.
 * @return 0 if the move was impossible, 1 if it was a move toward the interior, 2 if it was a straight movement, 3 if it was a move toward the exterior.
 * NB: If the C4CIterator is moving over a digital surface, then 
 * 'previous' means moving along an indirect tracking direction.
 */
uint 
ImaGene::C4CIteratorOnImplicitDigitalSurface::previous()
{
  Kn_sid adj;
  uint i = orthDir();
  uint j = trackDir();
  bool direct_j = m_ids->space()->sdirect( current(), j );
  uint code = m_neighborhood.adjacentOnAnyBdry( *m_ids,
						j,
						! direct_j,
						adj );
  if ( code != 0 )
    {
      m_neighborhood.move( adj );
      if ( orthDir() == j )
	m_track_dir = i;
    }
  return code;
}

    
    
///////////////////////////////////////////////////////////////////////////////
// ----------------------- C4CIteratorOnSurface services ----------------

    
/**
 * @return the current surfel the iterator is pointing at.
 */
ImaGene::Kn_sid 
ImaGene::C4CIteratorOnImplicitDigitalSurface::current() const
{
  return m_neighborhood.current();
}

    
/**
 * @return the current track direction.
 */
uint 
ImaGene::C4CIteratorOnImplicitDigitalSurface::trackDir() const
{
  return m_track_dir;
}

    
/**
 * @return the current orthogonal direction.
 */
uint 
ImaGene::C4CIteratorOnImplicitDigitalSurface::orthDir() const
{
  return m_neighborhood.orthDir();
}

    



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::C4CIteratorOnImplicitDigitalSurface::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CIteratorOnImplicitDigitalSurface]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::C4CIteratorOnImplicitDigitalSurface::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
