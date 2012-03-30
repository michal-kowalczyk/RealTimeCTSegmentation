///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CGeometry.cxx
//
// Creation : 2003/02/11
//
// Version : 2003/02/11
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
//	2003/02/11 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
//#include "LinAlg/LinAlg2D/Vector2D.hpp"
#include "ImaGene/base/Vector.h"
//#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/dgeometry2d/C4CIterator.h"
#include "ImaGene/dgeometry2d/C4CSegment.h"
#include "ImaGene/dgeometry2d/C4CGeometry.h"

// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/C4CGeometry.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const C4CGeometry_RCS_ID = "@(#)class C4CGeometry definition.";

///////////////////////////////////////////////////////////////////////////////
// class C4CGeometry
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
C4CGeometry::~C4CGeometry()
{
}

/**
 * Constructor.
 */
C4CGeometry::C4CGeometry()
{
}


///////////////////////////////////////////////////////////////////////////////
// ------------------------- plane to space services ------------------------


/**
 * Projects the plane vector [v2] into the space assuming [x_coord] was the
 * x-coordinate in the plane and [y_coord] the y-coordinate.
 * @param v2 any 2D vector.
 * @param vn (returns) a n-dimensional vector.
 * @param x_coord the coordinate of the space used for defining x-axis.
 * @param y_coord the coordinate of the space used for defining y-axis.
 */
/*
void
C4CGeometry::plane2Space( const Vector2i & v2, Vector & vn, 
			uint x_coord, uint y_coord )
{
  for ( uint i = 0; i < vn.size(); ++i )
    vn.rw( i ) = 0.0;
  vn.rw( x_coord ) = v2.x();
  vn.rw( y_coord ) = v2.y();
}
*/

///////////////////////////////////////////////////////////////////////////////
// ------------------------- Tangent services ------------------------------

/**
 * Computes the tangent vector at a bel [b] along a given direction [k] over
 * the boundary of an object [obj]. The boundary of [obj] must be closed.
 * The object [obj] and the bel [b] must be defined in the same space as 
 * [badj].
 * Uses Vialard and Debled's algorithm of discrete line recognition.
 * @param badj a bel adjacency defined in a digital space.
 * @param b any bel of the object [obj].
 * @param k any direction different from 'b.orthDir()'.
 * @param obj any set of spels in [ks].
 * @return the digital line tangent to [b].
 */
/*
DLine
C4CGeometry::tangentOnBdry( const BelAdjacency & badj,
			  Kn_sid bel,
			  uint k,
			  KnCharSet obj )
{
  ContourOnBdryAdapter c4( badj, bel, k, obj );
  return C4CGeometry::tangentLine( c4 );
}
*/

///////////////////////////////////////////////////////////////////////////////
// ------------------------- Tangent services ------------------------------


/**
 * Computes the curvature at the given position on a 4-connected
 * contour. The contour is defined through a forward iterator [fwd]
 * and a backward iteraror [bwd].  Uses Vialard and Debled's
 * algorithm of discrete line recognition.
 *
 * @param curr any 4-connected contour iterator.
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the curvature at the given contour element.
 */
float
C4CGeometry::curvatureByAngleVariation( C4CIterator & fwd,
				      C4CIterator & bwd,
				      uint max_size )
{
  C4CIterator* copy_fwd = fwd.clone();
  C4CIterator* copy_bwd = bwd.clone();
  
  //DLine sym_tgt = symmetricTangent( copy_fwd, copy_bwd );
  
  // Extracts half-tangents
  C4CSegment tgt_pos;
  tgt_pos.init();
  longestPositiveSegment( tgt_pos, fwd, max_size );
  C4CSegment tgt_neg;
  tgt_neg.init();
  longestNegativeSegment( tgt_neg, bwd, max_size );

  
  // Gives tangent.
  DLine median = DLine::medianLine( tgt_neg.getLine(), tgt_pos.getLine() );

  // Estracts further half-tangents

  // (1) positive one
  Vector2i ti_pos = tgt_pos.getTangent();
  uint code_back = copy_bwd->next();
  tgt_pos.slidesForward( code_back );
  tgt_pos.retractsBack( code_back );
  longestPositiveSegment( tgt_pos, fwd, max_size );
  Vector2i tip_pos = tgt_pos.getTangent();
  // put back the vector into the same reference frame as ti_pos.
  C4CSegment::slidesBackward( code_back, tip_pos );
  //tip_pos.flipForPositiveX();

  // (2) negative one
  Vector2i tip_neg = tgt_neg.getTangent();
  uint code_front = copy_fwd->previous();
  tgt_neg.slidesBackward( code_front );
  tgt_neg.retractsFront( code_front );
  longestNegativeSegment( tgt_neg, bwd, max_size );
  Vector2i ti_neg = tgt_neg.getTangent();
  // put back the vector into the same reference frame as ti_pos.
  C4CSegment::slidesForward( code_front, ti_neg );
  //ti_neg.flipForPositiveX();

//   cout << "T+_i: " << ti_pos << endl;
//   cout << "T+_{i+1}: " << tip_pos << endl;
//   cout << "T-_i: " << ti_neg << endl;
//   cout << "T-_{i+1}: " << tip_neg << endl;
//   cout << "T_i: " << median.tangent() << endl;

  
  // Frees some stuff
  delete copy_bwd;
  delete copy_fwd;
  
  // Performs some computations.
  Vector2D ui( (float) ti_pos.x(), (float) ti_pos.y() );
  ui /= sqrt( ui.x() * ui.x() + ui.y() * ui.y() );
  Vector2D uip( (float) tip_pos.x(), (float) tip_pos.y() );
  uip /= sqrt( uip.x() * uip.x() + uip.y() * uip.y() );

  Vector2D vi( (float) ti_neg.x(), (float) ti_neg.y() );
  vi /= sqrt( vi.x() * vi.x() + vi.y() * vi.y() );
  Vector2D vip( (float) tip_neg.x(), (float) tip_neg.y() );
  vip /= sqrt( vip.x() * vip.x() + vip.y() * vip.y() );

//   cout << "ui=(" << ui.x() << " , " << ui.y() << ")" << endl;
//   cout << "uip=(" << uip.x() << " , " << uip.y() << ")" << endl;
//   cout << "vi=(" << vi.x() << " , " << vi.y() << ")" << endl;
//   cout << "vip=(" << vip.x() << " , " << vip.y() << ")" << endl;
  

  // Estimates local length.
  float dl_pos = median.tangent().averagedLength();
  //cout << "dl=" << dl_pos << endl;
  
  // Computes positive average curvature.
  double curv_square_pos = 2.0 - 2.0 * ( ui.x() * uip.x() + ui.y() * uip.y() );
  double curv_square_neg = 2.0 - 2.0 * ( vi.x() * vip.x() + vi.y() * vip.y() );
  curv_square_pos = curv_square_pos >= 0.0 ? curv_square_pos : 0.0;
  curv_square_neg = curv_square_neg >= 0.0 ? curv_square_neg : 0.0;

  //cout << "k+ = " << curv_square_pos << " k- = " << curv_square_neg << endl;
  
  return 0.5 * ( sqrt( curv_square_neg ) + sqrt( curv_square_pos ) ) / dl_pos;
}



/**
 * Computes the curvature at the given position on a 4-connected
 * contour. The contour is defined through a forward iterator [fwd]
 * and a backward iteraror [bwd].  Uses Vialard and Debled's
 * algorithm of discrete line recognition.
 *
 * @param center any 4-connected contour iterator.
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the curvature at the given contour element.
 */
float
C4CGeometry::curvatureBySymAngleVariation( C4CIterator & center,
					 uint max_size )
{
  C4CSegment segment;
  segment.init(); 
  uint start_code_back = center.previous();
  C4CIterator* copy_fwd;
  C4CIterator* copy_bwd;

  // Back symmetric tangent
  C4CSegment segment_back;
  segment_back.init();
  copy_fwd = center.clone();
  copy_bwd = center.clone();
  longestSymmetricSegment( segment_back, *copy_fwd, *copy_bwd, 0 );
  delete copy_bwd;
  delete copy_fwd;
  DLine back_sym_tgt = segment_back.getLine();
  Vector2i back_sym_tgt_vector = back_sym_tgt.tangent();
  float dl_back = back_sym_tgt_vector.averagedLength();
  C4CSegment::slidesForward( center.next(), back_sym_tgt_vector );

  // Go to next one.
  C4CSegment segment_centered;
  segment_centered.init();
  copy_fwd = center.clone();
  copy_bwd = center.clone();
  longestSymmetricSegment( segment_centered, *copy_fwd, *copy_bwd, 0 );
  delete copy_bwd;
  delete copy_fwd;
  DLine centered_sym_tgt = segment_centered.getLine();
  Vector2i centered_sym_tgt_vector = centered_sym_tgt.tangent();
  float dl_centered = centered_sym_tgt_vector.averagedLength();
  center.next();
  
  // Front symmetric tangent.
  C4CSegment segment_front;
  segment_front.init();
  copy_fwd = center.clone();
  copy_bwd = center.clone();
  longestSymmetricSegment( segment_front, *copy_fwd, *copy_bwd, 0 );
  delete copy_bwd;
  delete copy_fwd;
  DLine front_sym_tgt = segment_front.getLine();
  Vector2i front_sym_tgt_vector = front_sym_tgt.tangent();
  float dl_front = front_sym_tgt_vector.averagedLength();
  C4CSegment::slidesBackward( center.previous(), front_sym_tgt_vector );

//   // Back symmetric tangent
//   longestSymmetricSegment( segment, *copy_fwd, *copy_bwd, 0 );
//   DLine back_sym_tgt = segment.getLine();
//   Vector2i back_sym_tgt_vector = back_sym_tgt.tangent();
//   float dl_back = back_sym_tgt_vector.averagedLength();
//   C4CSegment::slidesForward( start_code_back, back_sym_tgt_vector );

//   // Go to next one.
//   nextSymmetricSegment( segment, center, *copy_fwd, *copy_bwd );
//   DLine centered_sym_tgt = segment.getLine();
//   Vector2i centered_sym_tgt_vector = centered_sym_tgt.tangent();
//   float dl_centered = centered_sym_tgt_vector.averagedLength();
  
//   // Front symmetric tangent.
//   nextSymmetricSegment( segment, center, *copy_fwd, *copy_bwd );
//   DLine front_sym_tgt = segment.getLine();
//   Vector2i front_sym_tgt_vector = front_sym_tgt.tangent();
//   float dl_front = front_sym_tgt_vector.averagedLength();
//   uint start_code_front = center.previous(); // back to centered position
//   C4CSegment::slidesBackward( start_code_front, front_sym_tgt_vector );

//   cout << "T+_i: " << ti_pos << endl;
//   cout << "T+_{i+1}: " << tip_pos << endl;
//   cout << "T-_i: " << ti_neg << endl;
//   cout << "T-_{i+1}: " << tip_neg << endl;
//   cout << "T_i: " << median.tangent() << endl;

  
  // Performs some computations.
  Vector2D uin( (float) back_sym_tgt_vector.x(), 
	       (float) back_sym_tgt_vector.y() );
  uin /= sqrt( uin.x() * uin.x() + uin.y() * uin.y() );
  Vector2D ui( (float) centered_sym_tgt_vector.x(), 
	       (float) centered_sym_tgt_vector.y() );
  ui /= sqrt( ui.x() * ui.x() + ui.y() * ui.y() );
  Vector2D uip( (float) front_sym_tgt_vector.x(),
		(float) front_sym_tgt_vector.y() );
  uip /= sqrt( uip.x() * uip.x() + uip.y() * uip.y() );

//    cout << "uin=(" << uin.x() << " , " << uin.y() << ")" << endl;
//    cout << "ui=(" << ui.x() << " , " << ui.y() << ")" << endl;
//    cout << "uip=(" << uip.x() << " , " << uip.y() << ")" << endl;

  // Estimates local length.
   //  float dl_pos = 0.5 * ( dl_front + dl_back ) + dl_centered;
  
   //cout << "dl=" << dl_pos << endl;
  
  // Computes positive average curvature.
  double curv_square1 = 2.0 - 2.0 * ( ui.x() * uip.x() + ui.y() * uip.y() );
  curv_square1 = curv_square1 >= 0.0 ? curv_square1 : 0.0;
  double curv_square2 = 2.0 - 2.0 * ( ui.x() * uin.x() + ui.y() * uin.y() );
  curv_square2 = curv_square2 >= 0.0 ? curv_square2 : 0.0;

  //cout << "k+ = " << curv_square_pos << " k- = " << curv_square_neg << endl;
  
  return sqrt( curv_square1 ) / ( dl_centered + dl_front )
    + sqrt( curv_square2 ) / ( dl_centered + dl_back );
}



/**
 * Computes the symmetric discrete tangent on a 4-connected
 * contour. The contour is defined through a forward iterator [fwd]
 * and a backward iteraror [bwd].  Uses Vialard and Debled's algorithm
 * of discrete line recognition.
 *
 * @param fwd any 4-connected contour iterator.
 * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the digital segment tangent to [b].
 */
C4CSegment
C4CGeometry::symmetricTangent( C4CIterator & fwd,
			       C4CIterator & bwd, 
			       uint max_size )
{
  C4CSegment segment;
  segment.init();
  if ( max_size == 0 ) max_size--;
  while ( max_size-- != 0 )
    {
      if ( ! segment.extendsUnsecure( bwd.previous(), fwd.next() ) )
	{
	  bwd.next();
	  fwd.previous();
	  break;
	}
    }
  return segment;
}


/**
 * Computes the extended discrete tangent on a 4-connected contour. It
 * is defined as the longest discrete segment including the symmetric
 * tangent at the given starting point, extended to the front or to
 * the back only if there is no ambiguity.
 *
 * The contour is defined through a forward iterator [fwd] and a
 * backward iteraror [bwd].  Uses Vialard and Debled's algorithm of
 * discrete line recognition.
 *
 * @param fwd any 4-connected contour iterator.
 * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the digital segment tangent to [b].
 */
C4CSegment
C4CGeometry::extendedTangent( C4CIterator & fwd,
			      C4CIterator & bwd, 
			      uint max_size )
{
  C4CSegment segment;
  segment.init();
  if ( max_size == 0 ) max_size--;
  C4CSegment copy;
  uint mode = 0; // extension mode: 0: both side, 1: front, 2: back

  // The size of the segment is the number of iteration.
  while ( max_size-- != 0 )
    {
      if ( mode == 0 )
	{
	  copy = segment;
	  // Checks extending Front.
	  if ( segment.extendsFront( fwd.next() ) == 0 )
	    {
	      uint code_back = bwd.previous();
	      // Checks extending Back.
	      if ( segment.extendsBack( code_back ) == 0 )
		// Extension on both sides succeeded.
		continue;
	      // Goes back to preceding position.
	      bwd.next();
	      // Front ok, Back no => we must check extending Back
	      // before Front.
	      if ( copy.extendsBack( code_back ) == 0 )
		// Segment can grow both sides but not at the same
		// time => stop.
		{
		  segment.retractsFront( fwd.previous() );
		  mode = 3;
		  break;
		}
	      // Segment can grow only Front. Switch to Front
	      // extension mode.
	      mode = 1; 
	    }
	  else
	    {
	      // Goes back to preceding position.
	      fwd.previous();
	      if ( segment.extendsBack( bwd.previous() ) != 0 )
		{
		  // Segment cannot grow Front or Back => stop.
		  bwd.next();
		  mode = 4;
		  break;
		}
	      // Segment can only grow Back. Switch to Back
	      // extension mode.
	      mode = 2;
	    }
	} // if ( mode == 0 )
      else if ( mode == 1 ) 
	{
	  // Checks extending Front.
	  if ( segment.extendsFront( fwd.next() ) != 0 )
	    {
	      fwd.previous();
	      break;
	    }
	}
      else if ( mode == 2 )
	{
	  // Checks extending Back.
	  if ( segment.extendsBack( bwd.previous() ) != 0 )
	    {
	      bwd.next();
	      break;
	    }
	}
    } // while ( max_size-- != 0 )
  //cerr << " " << mode;
  
//   if ( ( mode == 2 ) || ( mode == 5 ) )
//     if ( segment.isFrontRetractable() )
//       segment.retractsFront( fwd.previous() );
//   if ( ( mode == 1 ) || ( mode == 5 ) )
//       if ( segment.isBackRetractable() )
// 	segment.retractsBack( bwd.next() );
//   if ( mode != 3 )
//     {
//       if ( segment.isFrontRetractable() )
// 	segment.retractsFront( fwd.previous() );
//       if ( segment.isBackRetractable() )
// 	segment.retractsBack( bwd.next() );
//     }
  
  return segment;
}



/**
 * Computes the maximal front tangent on a 4-connected contour (ie the
 * segment [B(F(fwd)),F(fwd)]). The contour is defined through a
 * forward iterator [fwd] and a backward iteraror [bwd].  Uses Vialard
 * and Debled's algorithm of discrete line recognition.
 *
 * @param fwd any 4-connected contour iterator.
 * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the digital segment tangent to [b].
 */
C4CSegment
C4CGeometry::maximalFrontTangent( C4CIterator & fwd,
				  C4CIterator & bwd, 
				  uint max_size )
{
  C4CSegment segment;
  segment.init();
  if ( max_size == 0 ) max_size--;
  uint max_size2 = max_size;
  // Extracting F(fwd)
  while ( max_size-- != 0 )
    {
      uint code = fwd.next();
      if ( segment.extendsFront( code ) != 0 )
	{
	  if ( code != 0 ) fwd.previous();
	  break;
	}
    }
  // Extracting B(F(fwd))
  while ( max_size2-- != 0 )
    {
      uint code = bwd.previous();
      if ( segment.extendsBack( code ) != 0 )
	{
	  if ( code != 0 ) bwd.next();
	  break;
	}
    }
  return segment;
}


/**
 * Computes the maximal back tangent on a 4-connected contour (ie the
 * segment [B(bwd),F(B(bwd))]). The contour is defined through a
 * forward iterator [fwd] and a backward iteraror [bwd].  Uses Vialard
 * and Debled's algorithm of discrete line recognition.
 *
 * @param fwd any 4-connected contour iterator.
 * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the digital segment tangent to [b].
 */
C4CSegment
C4CGeometry::maximalBackTangent( C4CIterator & fwd,
				 C4CIterator & bwd, 
				 uint max_size )
{
  C4CSegment segment;
  segment.init();
  if ( max_size == 0 ) max_size--;
  uint max_size2 = max_size;
  // Extracting B(bwd)
  while ( max_size-- != 0 )
    {
      uint code = bwd.previous();
      if ( segment.extendsBack( code ) != 0 )
	{
	  if ( code != 0 ) bwd.next();
	  break;
	}
    }
  // Extracting B(F(fwd))
  while ( max_size2-- != 0 )
    {
      uint code = fwd.next();
      if ( segment.extendsFront( code ) != 0 )
	{
	  if ( code != 0 ) fwd.previous();
	  break;
	}
    }
  return segment;
}


/**
 * Computes the common tangent on a 4-connected contour (ie the
 * segment [B(F(fwd)),F(B(bwd))]). The contour is defined through a
 * forward iterator [fwd] and a backward iteraror [bwd].  Uses Vialard
 * and Debled's algorithm of discrete line recognition.
 *
 * @param fwd any 4-connected contour iterator.
 * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the digital segment tangent to [b].
 */
C4CSegment
C4CGeometry::commonTangent( C4CIterator & fwd,
			    C4CIterator & bwd, 
			    uint max_size )
{
  C4CIterator* front_fwd = fwd.clone();
  C4CIterator* front_bwd = bwd.clone();
  C4CIterator* back_fwd = fwd.clone();
  C4CIterator* back_bwd = bwd.clone();

  // Extracting maximal front and back tangent.
  maximalFrontTangent( *front_fwd, *front_bwd, max_size );
  maximalBackTangent( *back_fwd, *back_bwd, max_size );
  // The common tangent is the intersection of these two segments.
  C4CSegment segment;
  segment.init();
  while ( ! fwd.equals( *back_fwd ) )
    segment.extendsFront( fwd.next() );
  while ( ! bwd.equals( *front_bwd ) )
    segment.extendsBack( bwd.previous() );

  delete back_bwd;
  delete back_fwd;
  delete front_bwd;
  delete front_fwd;
  
  return segment;
}


/**
 * Computes the positive discrete half-tangent on a 4-connected
 * contour. The contour is defined through a forward iterator [fwd].
 * Uses Vialard and Debled's algorithm
 * of discrete line recognition.
 *
 * @param fwd any 4-connected contour iterator.
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the digital segment representing the positive half-tangent.
 */
C4CSegment
C4CGeometry::positiveHalfTangent( C4CIterator & fwd,
				uint max_size )
{
  C4CSegment segment;
  segment.init();
  longestPositiveSegment( segment, fwd, max_size );
  return segment;
}


/**
 * Computes the negative discrete half-tangent on a 4-connected
 * contour. The contour is defined through a backward iterator [bwd].
 * Uses Vialard and Debled's algorithm
 * of discrete line recognition.
 *
 * @param fwd any 4-connected contour iterator.
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the digital segment representing the negative half-tangent.
 */
C4CSegment
C4CGeometry::negativeHalfTangent( C4CIterator & bwd,
				uint max_size )
{
  C4CSegment segment;
  segment.init();
  longestNegativeSegment( segment, bwd, max_size );
  return segment;
}



/**
 * Computes the longest possible discrete segment on a 4-connected
 * contour in the positive direction (front). The contour is defined
 * through a forward iterator [fwd].  Uses Vialard and Debled's
 * algorithm of discrete line recognition.  Useful to compute
 * positive half tangents.
 *
 * @param segment a segment such that [fwd] is located on its front. Returns the longest segment obtained by extending its front.
 * @param fwd (updated) any 4-connected contour iterator.
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
 */
void
C4CGeometry::longestPositiveSegment( C4CSegment & segment,
				   C4CIterator & fwd,
				   uint max_size )
{
  if ( max_size == 0 ) max_size--;
  while ( max_size-- != 0 ) 
    {
      uint code = fwd.next();
      if ( segment.extendsFront( code ) != 0 )
	{
	  if ( code != 0 ) fwd.previous();
	  break;
	}
    }
}


/**
 * Computes the longest possible discrete segment on a 4-connected
 * contour in the negative direction (back). The contour is defined
 * through a backward iterator [bwd].  Uses Vialard and Debled's
 * algorithm of discrete line recognition.  Useful to compute
 * negative half tangents.
 *
 * @param segment a segment such that [bwd] is located on its back. Returns the longest segment obtained by extending its back.
 * @param bwd (updated) any 4-connected contour iterator.
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
 */
void 
C4CGeometry::longestNegativeSegment( C4CSegment & segment,
				   C4CIterator & bwd,
				   uint max_size )
{
  if ( max_size == 0 ) max_size--;
  while ( max_size-- != 0 )
    {
      uint code = bwd.previous();
      if ( segment.extendsBack( code ) != 0 )
	{
	  if ( code != 0 ) bwd.next();
	  break;
	}
    }
}


/**
 * Computes the longest possible symmetric discrete segment on a
 * 4-connected contour. The contour is defined through a forward
 * iterator [fwd] and a backward iterator [bwd].  Uses Vialard and
 * Debled's algorithm of discrete line recognition.  Useful to
 * compute symmetric tangents.
 *
 * @param segment a segment such that [fwd] is located on its front and [bwd] is located on its back. Returns the longest segment obtained by extending its front and back.
 * @param fwd (updated) any 4-connected contour iterator.
 * @param bwd (updated) any 4-connected contour iterator.
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
 */
void
C4CGeometry::longestSymmetricSegment( C4CSegment & segment,
				    C4CIterator & fwd,
				    C4CIterator & bwd,
				    uint max_size )
{
  if ( max_size == 0 ) max_size--;
  while ( max_size-- != 0 )
    {
      uint code_bwd_prev = bwd.previous();
      uint code_fwd_next = fwd.next();
      if ( ! segment.extends( code_bwd_prev, code_fwd_next ) )
	{
	  if ( code_bwd_prev != 0 ) bwd.next();
	  if ( code_fwd_next != 0 ) fwd.previous();
	  break;
	}
    }
}


/**
 * Computes the longest possible extended discrete segment on a
 * 4-connected contour. The contour is defined through a forward
 * iterator [fwd] and a backward iterator [bwd].  Uses Vialard and
 * Debled's algorithm of discrete line recognition.  Useful to
 * compute extended tangents (a variant of symmetric tangents that
 * can be longer in some cases).
 *
 * @param segment a segment such that [fwd] is located on its front and [bwd] is located on its back. Returns the longest extended segment obtained by extending its front and back.
 * @param fwd (updated) any 4-connected contour iterator.
 * @param bwd (updated) any 4-connected contour iterator.
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction. 
 * @deprecated Use extendedTangent instead.
 * @see extendedTangent
 */
void
C4CGeometry::longestExtendedSegment( C4CSegment & segment,
				     C4CIterator & fwd,
				     C4CIterator & bwd,
				     uint max_size )
{
  if ( max_size == 0 ) max_size--;
  uint errcode_front = 0;
  uint errcode_back = 0;
  
  C4CSegment copy;
  while ( max_size-- != 0 )
    {
      copy = segment;
      if ( errcode_front == 0 )
	{
	  errcode_front = segment.extendsFront( fwd.next() );
	  if ( errcode_front == 2 ) 
	    break;
	}
      if ( errcode_back == 0 )
	{
	  errcode_back = segment.extendsBack( bwd.previous() );
	  if ( errcode_back == 2 ) 
	    break;
	}
      if ( ( errcode_front != 0 ) && ( errcode_back != 0 ) )
	break;
    } // while ( max_size-- != 0 )
  if ( ( errcode_front != 0 ) || ( errcode_back == 2 ) )
    fwd.previous();
  if ( errcode_back != 0 ) bwd.next();
  segment = copy;
}



 



/**
 * From an already computed symmetric tangent (longest symmetric
 * tangent), computes the symmetric tangent on the next element of
 * the contour. The current element is given by [current] and the
 * extremities of the current symmetric tangent are given by
 * [forward] and [backward]. Use Debled's algorithm of discrete line
 * recognition.  Useful for incremental computation of symmetric
 * tangents.
 *
 * @param segment the symmetric tangent around [current].
 *
 * @param current (updated) a 4-connected contour iterator pointing
 * on the center element of [segment].
 *
 * @param fwd (updated) a 4-connected contour iterator pointing on
 * the last element to the front of [segment].
 *
 * @param bwd (updated) a 4-connected contour iterator pointing on
 * the last element to the back of [segment].
 */
void
C4CGeometry::nextSymmetricSegment( C4CSegment & segment,
				 C4CIterator & current,
				 C4CIterator & fwd,
				 C4CIterator & bwd )
{
  // Slides to the next one.
  if ( segment.isBackRetractable() )
    {
      segment.retractsBack( bwd.next() );
      segment.slidesForward( current.next() );
      if ( segment.extendsFront( fwd.next() ) == 0 )
	{
	  if ( ! segment.extends( bwd.previous(), fwd.next() ) )
	    {
	      fwd.previous();
	      bwd.next();
	    }
	}
      else
	{
	  fwd.previous();
	  if ( ! segment.isBackRetractable() )
	    cout << "      ERROR: " << segment << endl;
	  segment.retractsBack( bwd.next() );
	}
    }
  else // limit case where tangent is reduced to two points.
    {
      bwd.next();
      fwd.next();
      current.next();
    }
}




///////////////////////////////////////////////////////////////////////////////
// Masimal segments services
///////////////////////////////////////////////////////////////////////////////


/**
 * Given a maximal segment [max_segment], returns the next one in front of
 * it. [fwd] and [bwd] are updated to point on [next_max_segment].
 *
 * @param max_segment any maximal segment.
 * @param next_max_segment (returns) the next maximal segment.
 * @param fwd an iterator that must point to the front of [max_segment].
 * @param bwd an iterator that must point to the back of [max_segment].
 *
 * @param 'true' if there is another maximal segment before the
 * current surfel, 'false' if there is no next maximal tangent that
 * contains the current surfel (perhaps there is one after the current
 * surfel or we have reached and extremity of the contour). The next
 * maximal tangent is then non valid.
 */
bool
C4CGeometry::nextMaximalSegment( const C4CSegment & max_segment,
				 C4CSegment & next_max_segment,
				 C4CIterator & fwd,
				 C4CIterator & bwd )
{
  // Starting point is the given maximal tangent.
  next_max_segment = max_segment;
  uint code_front = fwd.next();
  // Takes care of open contours.
  if ( code_front == 0 ) return false;
  // Retracts back
  do
    {
      if ( next_max_segment.isBackRetractable() )
	next_max_segment.retractsBack( bwd.next() );
      else
	{
	  fwd.previous();
	  return false;
	}
    }
  while ( next_max_segment.extendsFront( code_front ) != 0 );
  // Till we can extend in front.
  do
    {
      code_front = fwd.next();
    }
  while ( next_max_segment.extendsFront( code_front ) == 0 );
  // Takes care of open contours.
  if ( code_front != 0 ) fwd.previous();
  return true;
}


/**
 * Computes the maximal segments around surfel pointed by [it]. These
 * segments range from F(B(it)) to B(F(it)).
 *
 * @param it any 4-connected contour iterator.
 * @param m_segments an allocated array of segments of size [m]
 * @param j the index where to compute the first segment.
 * @param k (returns) the index just after the last computed segment.
 * @param m the size of the array [m_segments].
 * @return 'false' if the array m_segments was not big enough, 'true' otherwise.
 */
bool
C4CGeometry::maximalSegments( const C4CIterator & it,
			      C4CSegment* m_segments, 
			      uint j, uint & k, uint m )
{
  C4CIterator* back_fwd = it.clone();
  C4CIterator* back_bwd = it.clone();

  // Extracting maximal back tangent.
  m_segments[ j ] = maximalBackTangent( *back_fwd, *back_bwd, 0 );
  //  cout << endl << "[Max T[" << j << "] " << m_segments[ j ] << "]" << endl;
  
  k = j;
  while ( true )
    { 
      uint kprev = k;
      k = ( k + 1 ) % m;
      
      if ( ( k == j ) ||
	   ! nextMaximalSegment( m_segments[ kprev ], 
				 m_segments[ k ],
				 *back_fwd, *back_bwd ) )
	break;
      //      cout << "[Oth T[" << k << "] " << m_segments[ k ] << "]" << endl;
    }

  delete back_bwd;
  delete back_fwd;

  if ( k == j )
      cerr << "[Oth T[" << k << "] " << m_segments[ k ] << "]" << endl;

  return k != j;
}




/**
 * Computes the maximal segments around surfel pointed by [it]. These
 * segments range from F(B(it)) to B(F(it)).
 *
 * @param it any 4-connected contour iterator.
 * @param m_segments an allocated array of segments of size [m]
 * @param j the index where to compute the first segment.
 * @param k (returns) the index just after the last computed segment.
 * @param m the size of the array [m_segments].
 * @param tabIteratorFwd the table of the foward iterator of each segment.
 * @param tabIteratorBwd the table of the backward iterator of each segment.
 * @return 'false' if the array m_segments was not big enough, 'true' otherwise.
 */
bool
C4CGeometry::maximalSegmentsWithMemory( const C4CIterator & it,
			      C4CSegment* m_segments, 
			      C4CIterator ** tabIteratorFwd,
			      C4CIterator ** tabIteratorBwd,
			      uint j, uint & k, uint m )
{
  //cout<<"begin maximalSegmentsWithMemory"<<endl;
  C4CIterator* back_fwd = it.clone();
  C4CIterator* back_bwd = it.clone();

  // Extracting maximal back tangent.
  m_segments[ j ] = maximalBackTangent( *back_fwd, *back_bwd, 0 );
 // cout<<"size of back_fwd : "<<C4CIterator::size(*back_fwd)<<endl;
 // cout<<"size of back_bwd : "<<C4CIterator::size(*back_bwd)<<endl;
  tabIteratorFwd[j] = (*back_fwd).clone();
  tabIteratorBwd[j] = (*back_bwd).clone();
  //cout<<"size of tabIteratorFwd[j] : "<<C4CIterator::size(*tabIteratorFwd[j])<<endl;
  //  cout << endl << "[Max T[" << j << "] " << m_segments[ j ] << "]" << endl;
  
  k = j;
  while ( true )
    { 
      uint kprev = k;
      k = ( k + 1 ) % m;
      
      if ( ( k == j ) ||
	   ! nextMaximalSegment( m_segments[ kprev ], 
				 m_segments[ k ],
				 *back_fwd, *back_bwd ) )
	break;
      else
      {
        tabIteratorFwd[k] = (*back_fwd).clone();
        tabIteratorBwd[k] = (*back_bwd).clone();
	//cout<<"size of tabIteratorFwd[k] : "<<C4CIterator::size(*tabIteratorFwd[k])<<endl;
      }
      //      cout << "[Oth T[" << k << "] " << m_segments[ k ] << "]" << endl;
    }

  delete back_bwd;
  delete back_fwd;
  
 // cout<<"size of tabIteratorFwd[0] après delete : "<<C4CIterator::size(*tabIteratorFwd[0])<<endl;

  if ( k == j )
      cerr << "[Oth T[" << k << "] " << m_segments[ k ] << "]" << endl;

  return k != j;
}



/**
 * Computes the discrete tangent on a 4-connected contour. The contour
 * is defined through a forward iterator [fwd] and a backward iteraror [bwd].
 * Uses Vialard and Debled's algorithm of discrete line recognition.
 * @param fwd any 4-connected contour iterator.
 * @param bwd any 4-connected contour iterator (generally a clone of [fwd]).
 * @param max_size if 0, extracts the exact discrete tangent, otherwise defines the maximum number of bels searched in one direction.
 * @return the digital line tangent to [b].
 * @deprecated
 */
DLine
C4CGeometry::tangentLine( C4CIterator & fwd,
			C4CIterator & bwd, 
			uint max_size )
{
  // initial digital line coefficients.
  int a = 0;
  int b = 1;
  int mu = 0;
  int abs_a = 0;
  Vector2i up( 1, 0 );    // U'
  Vector2i lp( up );      // L'
  Vector2i u( 0, 0 );     // U
  Vector2i l( 0, 0 );     // L
  Vector2i cp_n( up );    // C'_n
  Vector2i c_n( 0, 0 );   // C_n
  int c, lower_bound, upper_bound;
  uint8 code;
  uint8 state_cp_n = 0; // 0 : tangent, 4: turned_int, 8: turned_ext
  uint8 state_c_n = 0;  // 0 : tangent, 4: turned_int, 8: turned_ext

  int coef_a = 0;
  int coef_b = 1;
  int coef_mu = 0;

  // loop. Look in the forward direction (C'n), 
  // then in the backward direction (Cn).
  if ( max_size == 0 ) max_size--;
  while ( max_size-- != 0 )
    {
      // (I) get next c'_n.
      code = fwd.next();
      switch ( state_cp_n + code ) {
      case 0: break;
      case 1: 
	if ( a <= 0 ) // test compatibility
	  { cp_n.y()--; state_cp_n = 4; } 
	else code = 0;
	break;
      case 2: cp_n.x()++; state_cp_n = 0; break;
      case 3: 
	if ( a >= 0 ) // test compatibility
	  { cp_n.y()++; state_cp_n = 8; }
	else code = 0;
	break;
      case 4: code = 0; break;
      case 5: code = 0; break;
      case 6: cp_n.y()--; break;
      case 7: 
	if ( a > 0 ) cerr << "C4CGeometry: ERREUR" << endl;// test compatibility
	  cp_n.x()++; state_cp_n = 0;
	break;
      case 8: code = 0; break;
      case 9: 
	if ( a < 0 ) cerr << "C4CGeometry: ERREUR" << endl;// test compatibility
	cp_n.x()++; state_cp_n = 0;
	break;
      case 10: cp_n.y()++; break;
      case 11: code = 0; break;
      default: code = 0; break;
      }
      if ( code == 0 ) break;

      // compute new ax-by.
      c = a * cp_n.x() - b * cp_n.y();
      lower_bound = mu - 1;
      upper_bound = mu + abs_a + b;
      // start recognition.
      if ( ( c < lower_bound ) || ( c > upper_bound ) )
	{ // not a digital line
	  break;
	}
      else if ( c == lower_bound )
	{ // new upper critical point.
	  up = cp_n; // U'<- C'_{n+1}
	  l = lp;    // L <- L'
	  a = up.y() - u.y();
	  b = up.x() - u.x();
	  abs_a = ( a >= 0 ? a : -a );
	  mu = a * up.x() - b * up.y();
	}
      else if ( c == upper_bound )
	{ // new lower critical point.
	  u = up;    // U <- U'
	  lp = cp_n; // L'<- C'_{n+1}
	  a = lp.y() - l.y();
	  b = lp.x() - l.x();
	  abs_a = ( a >= 0 ? a : -a );
	  mu = a * lp.x() - b * lp.y() - abs_a - b + 1;
	}
      else
	{ // still the same digital line.
	  // The upper and lower critical point may change.
	  if ( c == mu ) 
	    up = cp_n; // U' <- C'_{n+1}
	  if ( c == ( upper_bound - 1 ) )
	    lp = cp_n; // L' <- C'_{n+1}
	}

      // (II) get next c'_n.
      code = bwd.previous();
      switch ( state_c_n + code ) {
      case 0: break;
      case 1: 	
	if ( a >= 0 ) // test compatibility
	  { c_n.y()--; state_c_n = 4; }
	else code = 0;
	break;
      case 2: c_n.x()--; state_c_n = 0; break;
      case 3: 
	if ( a <= 0 ) // test compatibility
	  { c_n.y()++; state_c_n = 8; }
	else code = 0;
	break;
      case 4: code = 0; break;
      case 5: code = 0; break;
      case 6: c_n.y()--; break;
      case 7: 
	if ( a < 0 ) cerr << "C4CGeometry: ERREUR" << endl;// test compatibility
	c_n.x()--; state_c_n = 0; break;
      case 8: code = 0; break;
      case 9: 
	if ( a > 0 ) cerr << "C4CGeometry: ERREUR" << endl;// test compatibility
	c_n.x()--; state_c_n = 0; break;
      case 10: c_n.y()++; break;
      case 11: code = 0; break;
      default: code = 0; break;
      }
      if ( code == 0 ) break;

      // compute new ax-by.
      c = a * c_n.x() - b * c_n.y();
      lower_bound = mu - 1;
      upper_bound = mu + abs_a + b;
      // start recognition.
      if ( ( c < lower_bound ) || ( c > upper_bound ) )
	{ // not a digital line
	  break;
	}
      else if ( c == lower_bound )
	{ // new upper critical point.
	  u = c_n;   // U <- C_{n+1}
	  lp = l;    // L'<- L
	  a = up.y() - u.y();
	  b = up.x() - u.x();
	  abs_a = ( a >= 0 ? a : -a );
	  mu = a * u.x() - b * u.y();
	}
      else if ( c == upper_bound )
	{ // new lower critical point.
	  up = u;    // U'<- U
	  l = c_n;   // L <- C_{n+1}
	  a = lp.y() - l.y();
	  b = lp.x() - l.x();
	  abs_a = ( a >= 0 ? a : -a );
	  mu = a * l.x() - b * l.y() - abs_a - b + 1;
	}
      else
	{ // still the same digital line.
	  // The upper and lower critical point may change.
	  if ( c == mu ) 
	    u = c_n; // U <- C_{n+1}
	  if ( c == ( upper_bound - 1 ) )
	    l = c_n; // L <- C_{n+1}
	}

      // Current tangent line.
      coef_a = a;
      coef_b = b;
      coef_mu = mu;
      
    } // while ( true )
  return DLine( coef_a, coef_b, coef_mu );
}

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
C4CGeometry::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
