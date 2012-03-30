///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CTangentialCover.cxx
//
// Creation : 2004/11/10
//
// Version : 2004/11/10
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
//	2004/11/10 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <climits>
#include "ImaGene/base/Proxy.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/dgeometry2d/C4CGeometry.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/C4CTangentialCover.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const C4CTangentialCover_RCS_ID = "@(#)class C4CTangentialCover definition.";

///////////////////////////////////////////////////////////////////////////////
// class C4CTangentialCover
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static services ------------------------------

/** 
 * Given an iterator [it] on a digital contour, computes the number of
 * maximal segments covering the contour. Complexity is O(nbs) if nbs
 * is the number of surfels/points of the contour.
 * 
 * @param it an iterator on the contour. (returned as the first
 * surfel of the first maximal segment).
 * 
 * @return the number of maximal segments.
 */
int
ImaGene::C4CTangentialCover::computeNumberMS( C4CIterator & it )
{
  bool is_curve_open = false;

  // Computes the number of surfels.  Automatically rewinds the
  // iterator to the beginning of the contour if it is open.
  int nb_surfels = C4CIterator::size( it, is_curve_open );

  // Useful when the contour is closed.
  Mathutils::ModuloComputer mc( nb_surfels );

  // Init process. Find maximum back tangent. Compute the first maximal
  // segment at position given by [it].
  Proxy<C4CIterator> itcur( it.clone() );
  Proxy<C4CIterator> itfwd( it.clone() );
  Proxy<C4CIterator> itbwd( it.clone() );
  C4CSegment dsscur = C4CGeometry::maximalBackTangent( *itfwd, *itbwd, 0 );
  // Memorize the forward extremity to know when to finish the loop.
  Proxy<C4CIterator> itloop( itfwd->clone() );

  uint ms = 0;
  do
    {
      // Since we cannot retract a dss more than the starting surfel
      // at (0,0)-(1,0), it is necessary to slide this starting surfel
      // at most. This guarantees that the next maximal segment also
      // includes this surfel. 
      //
      // First: slide dss till it reaches the front.
      while ( ! itcur->equals( *itfwd ) )
	dsscur.slidesForward( itcur->next() );
      ++ms;
      
      // Computing next maximal segment.
      // Starting point is the given maximal tangent.
      uint code_front = itfwd->next();
      if ( code_front != 0 ) // Check if curve open.
	{
	  // Retracts back till we can extend in front.
	  do
	      dsscur.retractsBack( itbwd->next() );
	  while ( dsscur.extendsFront( code_front ) != 0 );

	  // Extends in front till limit.
 	  while ( ( code_front = itfwd->next() ) != 0 )
	    if ( dsscur.extendsFront( code_front ) != 0 )
	      break;
	  if ( code_front == 0 )
	    { // curve is open
	      ++ms;
	      break; 
	    }

	  // must go back one step.
	  itfwd->previous();
	}
      else // Curve is open. Quit loop.
	break;
    }
  while ( ! itloop->equals( *itfwd ) );

  return ms;
}

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::C4CTangentialCover::~C4CTangentialCover()
{
  if ( m_segments != 0 )
    delete[] m_segments;
  if ( m_begin_it != 0 )
    delete m_begin_it;
  if ( m_end_it != 0 )
    delete m_end_it;
}


/**
 * Constructor. Does nothing. The object is not valid.
 */
ImaGene::C4CTangentialCover::C4CTangentialCover()
{
  m_segments = 0;
  m_begin_it = 0;
  m_end_it = 0;
}


/**
 * Resets the tangential cover as if it was just instantiated.
 */
void
ImaGene::C4CTangentialCover::reset()
{
  if ( m_segments != 0 )
    delete[] m_segments;
  if ( m_begin_it != 0 )
    delete m_begin_it;
  if ( m_end_it != 0 )
    delete m_end_it;
  m_segments = 0;
  m_begin_it = 0;
  m_end_it = 0;
}



/**
 * Computes the tangential cover.
 *
 * @param it an iterator on the digital curve. (returns) It points on
 * the surfel with index 0 in the tangential cover.
 *
 * @param nbmax_ms the maximal number of maximal segments on this
 * curve. If 0, computes the number of maximal segments before
 * proceeding, defaut is 0.
 *
 * @return 'true' if the initialization went ok, 'false' otherwise
 * (e.g., nbmax_ms is too small).
 */
bool 
ImaGene::C4CTangentialCover::init( C4CIterator & it, 
				   uint nbmax_ms )
{
  reset();
  if ( nbmax_ms == 0 )
    nbmax_ms = computeNumberMS( it );
  m_segments = new MaximalSegment[ nbmax_ms ];
  m_nb_surfels = C4CIterator::size( it, m_is_curve_open );
  m_begin_it = it.clone();
  Mathutils::ModuloComputer mc( m_nb_surfels );

  // Init process. Find maximum back tangent
  Proxy<C4CIterator> itcur( it.clone() );
  Proxy<C4CIterator> itfwd( it.clone() );
  Proxy<C4CIterator> itbwd( it.clone() );
  C4CSegment dsscur = C4CGeometry::maximalBackTangent( *itfwd, *itbwd, 0 );
  Proxy<C4CIterator> itloop( itfwd->clone() );

  // indices will be useful to have a direct access to maximal
  // segments, common parts and so on.
  int idxcur = 0; // index of frame surfel.
  int idxback = dsscur.relIndex( dsscur.c_n() );
  int idxfront = dsscur.relIndex( dsscur.cp_n() ) - 1;
  uint ms = 0;
  bool error = false;
  do
    {
      // Since we cannot retract a dss more than the starting surfel
      // at (0,0)-(1,0), it is necessary to slide this starting surfel
      // at most. This guarantees that the next maximal segment also
      // includes this surfel. 
      //
      // First: slide dss till it reaches the front.
      while ( idxcur != idxfront )
	{
	  dsscur.slidesForward( itcur->next() );
	  ++idxcur;
	}
      // The maximal segment is expressed in the frame of the front surfel.
      if ( ms >= nbmax_ms )
	{
	  error = true;
	  break;
	}
      m_segments[ ms ].dss = dsscur;
      m_segments[ ms ].back_surfel_idx = mc.cast( idxback );
      m_segments[ ms ].front_surfel_idx = mc.cast( idxfront );
      ++ms;
      
      // Computing next maximal segment.
      // Starting point is the given maximal tangent.
      uint code_front = itfwd->next();
      if ( code_front != 0 ) // Check if curve open.
	{
	  // Retracts back till we can extend in front.
	  do
	    {
	      dsscur.retractsBack( itbwd->next() );
	      ++idxback;
	    }
	  while ( dsscur.extendsFront( code_front ) != 0 );
	  ++idxfront;
	  // Extends in front till limit.
	  while ( ( code_front = itfwd->next() ) != 0 )
	    if ( dsscur.extendsFront( code_front ) == 0 )
	      ++idxfront;
	    else
	      break;
	  if ( code_front == 0 )
	    {  // curve is open
	      // continue to store last maximal segment and to put it
	      // in the reference frame of its front surfel.
	      continue;
	    }
	  else 
	    {
	      itfwd->previous();
	      ASSERT_C4CTangentialCover( ( idxback <= idxcur ) &&
					 ( idxcur <= idxfront ) );
	    }
	}
      else // Curve is open. Quit loop.
	break;
    }
  while ( ! itloop->equals( *itfwd ) );

  // Fill in information.
  m_nb_max_segments = ms;
  m_end_it = itloop->equals( *itfwd ) 
    ? m_begin_it-> clone() 
    : itfwd->clone();
  return error;
}

/**
 * @param i the index of the maximal segment.
 *
 * @return a dyn. alloc. iterator lying on the front surfel of the
 * [i]-th maximal segment.
 *
 * Nb: linear time algorithm.
 */
ImaGene::C4CIterator* 
ImaGene::C4CTangentialCover::getSurfelFront( uint i ) const
{
  Mathutils::ModuloComputer mc( nbSurfels() );
  C4CIterator* it = this->beginSurfel();
  uint front = this->getMaximalSegment( i ).front_surfel_idx;
  uint idx_bel = 0;
  while ( idx_bel != front )
    {
      mc.increment( idx_bel );
      it->next();
    }
  return it;
}

/**
 * @param i the index of the maximal segment.
 *
 * @return a dyn. alloc. iterator lying on the front surfel of the
 * [i]-th maximal segment.
 *
 * Nb: linear time algorithm.
 */
ImaGene::C4CIterator* 
ImaGene::C4CTangentialCover::getSurfelBack( uint i ) const
{
  Mathutils::ModuloComputer mc( nbSurfels() );
  C4CIterator* it = this->beginSurfel();
  uint back = this->getMaximalSegment( i ).back_surfel_idx;
  uint idx_bel = 0;
  while ( idx_bel != back )
    {
      mc.increment( idx_bel );
      it->next();
    }
  return it;
}


///////////////////////////////////////////////////////////////////////////////
// -------------------- Tangential cover services ---------------------------

/**
 * @param idx a surfel index.
 * @return the maximal segments of the surfel of index [idx].
 */
ImaGene::C4CTangentialCover::SurfelMaximalSegments
ImaGene::C4CTangentialCover::beginSMS( uint idx ) const
{
  ASSERT_C4CTangentialCover( idx < nbSurfels() );

  Mathutils::ModuloComputer mc( this->isContourOpen() ? UINT_MAX : nbSurfels() );
  SurfelMaximalSegments sms;
  sms.idx_surfel = idx;
  uint min = 0;
  uint max = nbMaximalSegments() - 1;
  uint cur = ( idx * nbMaximalSegments() ) / nbSurfels();

  // Dichotomy to find a englobing maximal segment.
  while ( true )
    {
      const MaximalSegment & ms = getMaximalSegment( cur );
      if ( mc.less( idx, ms.back_surfel_idx ) )
	{ // Visit left side.
	  max = cur - 1;
	  cur = ( min + max ) / 2;
	}
      // else if ( ms.front_surfel_idx < (int) idx )
      else if ( mc.less( ms.front_surfel_idx, idx ) )
	{ // Visit Right side.
	  min = cur + 1;
	  cur = ( min + max ) / 2;
	}
      else break;
    }

  // Shift left to find first maximal segment.
  min = cur;
  if ( min != 0 )
    {
      // while ( getMaximalSegment( --min ).front_surfel_idx >= (int) idx ) ;
      while ( ! mc.less( getMaximalSegment( --min ).front_surfel_idx, idx ) )
	;
      sms.begin_ms = min + 1;
    }
  else
    sms.begin_ms = 0;

  // Shift right to find last maximal segment.
  max = cur;
  if ( isContourOpen() )
    { // Contour is open.
      do
	{
	  ++max;
	  if ( max == nbMaximalSegments() ) // end of contour.
	    {
	      --max;
	      break;
	    }
	}
      // while ( getMaximalSegment( max ).back_surfel_idx <= (int) idx ) ;
      while ( ! mc.less( idx, getMaximalSegment( max ).back_surfel_idx ) );
      sms.end_ms = max;
    }
  else 
    { // Contour is closed.
      do
	{
	  ++max;
	  if ( max == nbMaximalSegments() ) // may loop
	    {
	      max = 0;
	      idx -= nbSurfels();
	    }
	}
      // while ( getMaximalSegment( max ).back_surfel_idx <= (int) idx ) ;
      while ( ! mc.less( idx, getMaximalSegment( max ).back_surfel_idx ) );
      sms.end_ms = max;
    }

  return sms;
}


/**
 * Given the maximal segments of a surfel given by [sms], computes
 * the maximal segments of the next surfel on the contour.
 *
 * @param sms the maximal segments of a surfel, (returns) the maximal segments of the surfel with an incremented index.
 * @return 'true' if there was a next surfel, false otherwise.
 */
bool 
ImaGene::C4CTangentialCover::nextSMS( SurfelMaximalSegments & sms ) const
{
  uint idx = sms.idx_surfel;
  if ( ( idx == nbSurfels() - 1 ) && ( m_is_curve_open ) )
    return false;
  
  Mathutils::ModuloComputer mcsu( nbSurfels() );
  Mathutils::ModuloComputer mcms( nbMaximalSegments() );

  uint front_idx_begin_ms = getMaximalSegment( sms.begin_ms ).front_surfel_idx;
  if ( front_idx_begin_ms == idx ) 
    mcms.increment( sms.begin_ms );
  
  mcsu.increment( idx );

  uint back_idx_end_ms = getMaximalSegment( sms.end_ms ).back_surfel_idx;
  if ( back_idx_end_ms == idx ) 
    mcms.increment( sms.end_ms );
  
  sms.idx_surfel = idx;
  return true;
}


/**
 * Given the maximal segments of a surfel given by [sms], computes
 * the maximal segments of the previous surfel on the contour.
 *
 * @param sms the maximal segments of a surfel, (returns) the maximal segments of the surfel with a decremented index.
 * @return 'true' if there was a previous surfel, false otherwise.
 */
bool 
ImaGene::C4CTangentialCover::previousSMS( SurfelMaximalSegments & sms ) const
{
  uint idx = sms.idx_surfel;
  if ( ( idx == 0 ) && ( m_is_curve_open ) )
    return false;

  Mathutils::ModuloComputer mcsu( nbSurfels() );
  Mathutils::ModuloComputer mcms( nbMaximalSegments() );
  
  uint p_end_ms = mcms.previous( sms.end_ms );
  uint back_idx_end_ms = getMaximalSegment( p_end_ms ).back_surfel_idx;
  if ( back_idx_end_ms == idx ) 
    sms.end_ms = p_end_ms;
  mcsu.decrement( idx );

  uint p_begin_ms = mcms.previous( sms.begin_ms );
  uint front_idx_begin_ms = getMaximalSegment( p_begin_ms ).front_surfel_idx;
  if ( front_idx_begin_ms == idx )
    sms.begin_ms = p_begin_ms;
  sms.idx_surfel = idx;
  return true;
}



/**
 * @return the first common part of the tangential cover..
 */
ImaGene::C4CTangentialCover::CommonPart
ImaGene::C4CTangentialCover::beginCP() const
{
  CommonPart cp;
  uint idx = 0;
  uint ms1 = 0;

  Mathutils::ModuloComputer mcsu( nbSurfels() );
  Mathutils::ModuloComputer mcms( nbMaximalSegments() );

  // Finding front of common part.
  uint ms1_front = mcsu.next( getMaximalSegment( ms1 ).front_surfel_idx );
  uint ms2 = mcms.next( ms1 );
  while ( ! mcsu.less( idx, getMaximalSegment( ms2 ).back_surfel_idx ) )
    mcms.increment( ms2 );
  uint ms2_back = getMaximalSegment( ms2 ).back_surfel_idx;
  cp.after_front_surfel_idx = 
    mcsu.less( ms2_back, ms1_front ) ? ms2_back : ms1_front;
  // Finding back of common part.
  uint ms2_prev_back = 
    getMaximalSegment( mcms.previous( ms2 ) ).back_surfel_idx;
  uint ms1_prev_front = 
    mcsu.next( getMaximalSegment( mcms.previous( ms1 ) ).front_surfel_idx );
  cp.back_surfel_idx = 
    mcsu.less( ms2_prev_back, ms1_prev_front ) 
    ? ms1_prev_front : ms2_prev_back;
  
  cp.begin_ms = ms1;
  cp.end_ms = ms2;
  return cp;
}


/**
 * Given a common part specified by [cp], computes
 * the next common part of the tangential cover.
 *
 * @param cp a common part of the tangential cover, (returns) the one
 * adjacent to it at the front.
 */
void
ImaGene::C4CTangentialCover::nextCP( CommonPart & cp ) const
{
  Mathutils::ModuloComputer mcsu( nbSurfels() );
  Mathutils::ModuloComputer mcms( nbMaximalSegments() );

  if ( mcsu.next( getMaximalSegment( cp.begin_ms ).front_surfel_idx )
       == cp.after_front_surfel_idx )
    // go to next back maximal segment.
    mcms.increment( cp.begin_ms );

  // The next common part starts where the previous finishes.
  cp.back_surfel_idx = cp.after_front_surfel_idx;
  
  if ( getMaximalSegment( cp.end_ms ).back_surfel_idx 
       == cp.back_surfel_idx )
    // go to next front maximal segment.
    mcms.increment( cp.end_ms );
  
  // Find new front surfel idx
  uint ms1_front 
    = mcsu.next( getMaximalSegment( cp.begin_ms ).front_surfel_idx );
  uint ms2_back 
    = getMaximalSegment( cp.end_ms ).back_surfel_idx;
  cp.after_front_surfel_idx 
    = mcsu.less( ms2_back, ms1_front ) ? ms2_back : ms1_front;

}





///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::C4CTangentialCover::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CTangentialCover]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::C4CTangentialCover::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
