///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : KnShapes.cxx
//
// Creation : 2002/08/05
//
// Version : 2002/08/05
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
//	2002/08/05 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <queue>
// #include <priority_queue>
#include <set>
#include <list>
#include <algorithm>
#include<fstream>
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/digitalnD/KnShapes.h"
#include "ImaGene/digitalnD/KnSpaceScanner.h"
#include "ImaGene/digitalnD/KnSpaceCoordScanner.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
#include "ImaGene/digitalnD/ObjectBoundary.h"
#include "ImaGene/digitalnD/SurfelNeighborhood.h"

#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/dgeometry2d/C4CGeometry.h"
#include "ImaGene/digitalnD/C4CIteratorOnBdry.h"
#include "ImaGene/digitalnD/Frame2D.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/VectorUtils.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/KnShapes.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


//const char* const KnShapes_RCS_ID = "@(#)class KnShapes definition.";

///////////////////////////////////////////////////////////////////////////////
// class KnShapes
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static services --------------------------------

/**
 * Given a cell [center] and a integer number [r], returns a cell
 * of same type as [center] but whose coordinates are each decreased by [r]
 * (or 0 if the coordinate - r was negative).
 * @param ks the digital space
 * @param center any cell.
 * @return the lower bound of the hypercube of center [center] and half-edge [r].
 */
ImaGene::Kn_uid
ImaGene::KnShapes::ugetCubeLowerBound( const KnSpace & ks,
				       Kn_uid center,
				       Kn_size r )
{
  uint k;
  Kn_size* low = new Kn_size[ ks.dim() ];
  ks.udecodeCoords( center, low );
  for ( k = 0; k < ks.dim(); ++k )
    {
      low[ k ] = ( ((Kn_ssize)low[ k ]) - (Kn_ssize) r ) > 0 
	? (Kn_size) ( low[ k ] - r ) 
	: KnTypes::ZERO;
    }
  Kn_uid p = ks.ucode( low, center );
  delete[] low;
  return p;
}

/**
 * Given a cell [center] and a integer number [r], returns a cell
 * of same type as [center] but whose coordinates are each increased by [r]
 * (or size[ k ] - 1 if the coordinate + r was outside the space bounds).
 * @param ks the digital space
 * @param center any cell.
 * @return the upper bound of the hypercube of center [center] and half-edge [r].
 */
ImaGene::Kn_uid
ImaGene::KnShapes::ugetCubeUpperBound( const KnSpace & ks,
				       Kn_uid center,
				       Kn_size r )
{
  Kn_size* up = new Kn_size[ ks.dim() ];
  ks.udecodeCoords( center, up );
  uint k;
  for ( k = 0; k < ks.dim(); ++k )
    {
      up[ k ] = ( up[ k ] + r ) < ks.size( k ) 
	? up[ k ] + r
	: ks.size( k ) - 1;
    }
  Kn_uid p = ks.ucode( up, center );
  delete[] up;
  return p;
}


/**
 * Creates a set of unsigned surfel cells whose elements represents the 
 * boundary of the set of spels [spelset].
 * @param ks any space.
 * @param spelset any set of spel-like cells.
 * @param lower_bound only the cells with greater coordinates than [lower_bound] are scanned.
 * @param upper_bound only the cells with lower coordinates than [upper_bound] are scanned.
 * @return a set of cells (which are all surfels), the boundary of [spelset].
 */
ImaGene::KnRCellSet
ImaGene::KnShapes::umakeBoundary( const KnSpace & ks,
				  const KnCharSet & spelset,
				  Kn_uid lower_bound,
				  Kn_uid upper_bound )
{
  // this is the surfel set that we intend to create.
  KnRCellSet s = KnRCellSet::create( ks, ks.dim() - 1, false, 0 );

  // Filling it by scanning the bounding cube.
  uint k;
  bool in_here, in_further;
  for ( k = 0; k < ks.dim(); ++k )
    {
      Kn_uid dir_low_uid = lower_bound;
      Kn_uid dir_up_uid = ks.ugetDecr( upper_bound, k );
      
      Kn_uid p = dir_low_uid;
      do 
	{
	  in_here = spelset[ p ];
	  in_further = spelset[ ks.ugetIncr( p, k ) ];
	  if ( in_here != in_further ) // boundary element
	    { // add it to the set.
	      s += ks.uincident( p, k, true );
	    }
	}
      while ( ks.uincrInBounds( p, dir_low_uid, dir_up_uid ) );
    }
  return s;
}


/**
 * Creates a set of signed surfel cells whose elements represents the 
 * boundary of the set of spels [spelset]. The cells are within the limits
 * given by [lower_bound] and [upper_bound].
 * The spel set is considered as positively oriented and the returned set is
 * exactly its oriented lower boundary.
 * @param ks any space.
 * @param spelset any set of spel-like cells.
 * @param lower_bound only the cells with greater coordinates than [lower_bound] are scanned.
 * @param upper_bound only the cells with lower coordinates than [upper_bound] are scanned.
 * @return a set of cells (which are all surfels), the boundary of [spelset].
 */
ImaGene::KnRCellSet
ImaGene::KnShapes::smakeBoundary( const KnSpace & ks,
				  const KnCharSet & spelset,
				  Kn_uid lower_bound,
				  Kn_uid upper_bound )
{
  // this is the surfel set that we intend to create.
  KnRCellSet s = KnRCellSet::create( ks, ks.dim() - 1, true, 0 );

  // Filling it by scanning the bounding cube.
  uint k;
  bool in_here, in_further;
  for ( k = 0; k < ks.dim(); ++k )
    {
      if ( ks.udecodeCoord( lower_bound, k )
	   == ks.udecodeCoord( upper_bound, k ) )
	continue;

      Kn_uid dir_low_uid = lower_bound;
      Kn_uid dir_up_uid = ks.ugetDecr( upper_bound, k );
      
      KnSpaceScanner scanner( ks, dir_low_uid, dir_up_uid );
      Kn_uid p = dir_low_uid;
      do 
	{
	  in_here = spelset[ p ];
	  in_further = spelset[ ks.ugetIncr( p, k ) ];
	  if ( in_here != in_further ) // boundary element
	    { // add it to the set.
	      if ( in_here == true )
		s += ks.sincident( ks.signsPos( p ), k, true );
	      else
		s += ks.sincident( ks.signsNeg( p ), k, true );
	    }
	}
      while ( scanner.increment( p ) );
    }
  return s;
}



/**
 * Creates a set of unsigned volumic cells whose elements represents a
 * parallelepiped of lower vertex [low] and upper vertex [up].
 * @param ks any space.
 * @param low a cell that is the lower bound of the box.
 * @param up a cell that is the upper bound of the box.
 * @return a set of spel-like cells.
 */
ImaGene::KnCharSet
ImaGene::KnShapes::umakeVolumicParallelepiped( const KnSpace & ks,
					       Kn_uid low,
					       Kn_uid up )
{
  low = ks.uspel( low );
  up = ks.uspel( up );
  
  if ( ( low == ks.ufirst( low ) )
       && ( up == ks.ulast( low ) ) )
    // Case where the parallelepiped is whole set.
    return KnCharSet::ucreate( ks, low, 1 );

  // Otherwise, fills the set.
  KnCharSet s = KnCharSet::ucreate( ks, low, 0 );
  Kn_uid p = low;
  do
    {
      s += p;
    }
  while ( ks.uincrInBounds( p, low, up ) );
  return s;
}


/**
 * Creates a set of unsigned volumic cells whose elements represents a
 * sphere of center [center] and radius [inclusive_radius].
 * @param ks any space.
 * @param center a cell that is the center of the sphere.
 * @param inclusive_radius the radius of the sphere.
 * @return a set of spel-like cells.
 */
ImaGene::KnCharSet
ImaGene::KnShapes::umakeVolumicSphere( const KnSpace & ks,
				       Kn_uid center,
				       float inclusive_radius )
{
  // center must be a spel.
  center = ks.uspel( center );

  // Computing sphere bounding cube.
  inclusive_radius = inclusive_radius >= 0 ? inclusive_radius : 0.0f;
  int r = (int) std::ceil( inclusive_radius );
  Kn_uid low_uid = ugetCubeLowerBound( ks, center, r );
  Kn_uid up_uid = ugetCubeUpperBound( ks, center, r );

  // Instantiating set.
  KnCharSet s = KnCharSet::ucreate( ks, center, 0 );
  Vector vcenter = ks.ucentroid( center );
  float r2 = inclusive_radius * inclusive_radius;

  // Filling it by scanning the bounding cube.
  KnSpaceCoordScanner scanner( ks, low_uid, up_uid );
  uint k;
  Kn_uid p = scanner.lower_bound;
  Vector vp = ks.ucentroid( p );
  do 
    {
      float dist2 = 0.0f;
      for ( k = 0; k < ks.dim(); ++k )
	dist2 += KnUtils::sqr( vcenter.ro( k ) - vp.ro( k ) );
      if ( dist2 <= r2 )
	s += p;
    }
  while ( scanner.increment( p, vp ) );
  return s;
}


  
/**
 * Creates a set of unsigned surfel cells whose elements represents a
 * sphere of center [center] and radius [inclusive_radius].
 * @param ks any space.
 * @param center a cell that is the center of the sphere (only coords are significant).
 * @param inclusive_radius the radius of the sphere.
 * @return a set of cells (which are all surfels).
 */
ImaGene::KnRCellSet
ImaGene::KnShapes::umakeSurfacicSphere( const KnSpace & ks,
					Kn_uid center,
					float inclusive_radius )
{
  // center must be a spel.
  center = ks.uspel( center );
  // Computing sphere bounding cube.
  inclusive_radius = inclusive_radius >= 0.0f ? inclusive_radius : 0.0f;
  int r = (int) std::ceil( inclusive_radius );
  Kn_uid low_uid = ugetCubeLowerBound( ks, center, r + 1 );
  Kn_uid up_uid = ugetCubeUpperBound( ks, center, r + 1 );

  // Instantiating set.
  // this is the sphere as a set of spels.
  KnCharSet filled_sphere = umakeVolumicSphere( ks, center, inclusive_radius );
  // returns its boundary.
  return umakeBoundary( ks, filled_sphere, low_uid, up_uid );
}



/**
 * Creates a set by expanding a seed element [p] within the limits fixed by
 * the blocking set [bset]. The blocking set is composed of cells that are
 * incident (up or low) (depending on the seed) to cells like [p].
 * For instance, you can expand a spel-like cell within the bounds of a 
 * set of surfel-like cells.
 * @param ks the space where the expansion took place
 * @param seed an unsigned cell.
 * @param bset the blocking set.
 * @return the set after expansion.
 */
ImaGene::KnCharSet
ImaGene::KnShapes::uexpandSeedWithinIncidentBounds( const KnSpace & ks,
					   Kn_uid seed,
					   const KnCharSet & bset )
{
  KnCharSet expset = KnCharSet::ucreate( ks, seed, 0 );
  queue<Kn_uid> elems;
  elems.push( seed );
  expset += seed;
  uint k;
  
  // For each border element
  while ( ! elems.empty() )
    {
      Kn_uid c = elems.front();
      elems.pop();
      // Look in all directions.
      for ( k = 0; k < ks.dim(); ++k )
	{
	  // Look negatively along the axis [k].
	  if ( ! ks.uisMin( c, k ) )
	    {
	      Kn_uid clow = ks.ugetDecr( c, k );
	      if ( ! expset[ clow ] && ! bset[ ks.uincident( c, k, false ) ] )
		{
		  expset += clow;
		  elems.push( clow );
		}
	    }
	  // Look positively along the axis [k].
	  if ( ! ks.uisMax( c, k ) )
	    {
	      Kn_uid chi = ks.ugetIncr( c, k );
	      if ( ! expset[ chi ] && ! bset[ ks.uincident( c, k, true ) ] )
		{
		  expset += chi;
		  elems.push( chi );
		}
	    }
	} // for ( k = 0; k < dim(); ++k )
    } // while ( ! elems.empty() )
  return expset;
}

/**
 * Creates a set by expanding a seed element [p] within the limits fixed by
 * the blocking set [bset]. The blocking set is composed of cells that are
 * of the same kind as the seed.
 * For instance, you can expand a spel-like cell within the limit bounds of a 
 * set of spel-like cells.
 * @param ks the space where the expansion took place
 * @param seed an unsigned cell.
 * @param bset the blocking set.
 * @return the set after expansion.
 */
ImaGene::KnCharSet
ImaGene::KnShapes::uexpandSeedWithinBounds( const KnSpace & ks,
					    Kn_uid seed,
					    const KnCharSet & bset )
{
  KnCharSet expset = KnCharSet::ucreate( ks, seed, 0 );
  queue<Kn_uid> elems;
  elems.push( seed );
  expset += seed;
  uint k;
  // For each border element
  while ( ! elems.empty() )
    {
      Kn_uid c = elems.front();
      elems.pop();
      // cout << "(" << elems.size() << " " << c << ") ";
      // cout.flush();
      
      // Look in all directions.
      for ( k = 0; k < ks.dim(); ++k )
	{
	  // Look negatively along the axis [k].
	  if ( ! ks.uisMin( c, k ) )
	    {
	      Kn_uid clow = ks.ugetDecr( c, k );
	      if ( ( ! expset[ clow ] ) && ( ! bset[ clow ] ) )
		{
		  expset += clow;
		  elems.push( clow );
		}
	      
	    }
	  // Look positively along the axis [k].
	  if ( ! ks.uisMax( c, k ) )
	    {
	      Kn_uid chi = ks.ugetIncr( c, k );
	      if ( ( ! expset[ chi ] ) && ( ! bset[ chi ] ) )
		{
		  expset += chi;
		  elems.push( chi );
		}
	    }
	} // for ( k = 0; k < dim(); ++k )
    } // while ( ! elems.empty() )
  return expset;
}


/**
 * Find a bel on the boundary of the set of spels [voxelset]. Extracts
 * the bel of [voxelset] that is furthest from [inner_spel] along some
 * axis.
 *
 * @param ks the space where the expansion took place
 * @param seed an unsigned cell.
 * @param voxelset the blocking set.
 * @return a bel on the boundary of [voxelset], 0 if not found.
 */
ImaGene::Kn_sid
ImaGene::KnShapes::sfindFurthestBel( const KnSpace & ks,
				     Kn_uid inner_spel,
				     const KnCharSet & voxelset )
{
  ASSERT_KnShapes( ( ks.udim( inner_spel ) == ks.dim() )
		   && voxelset[ inner_spel ] );
  
  // Try to find one spel that is outside the shape of interest.
  Kn_uid outer_spel;
  bool descent = false;
  // Look in all directions.
  uint k;
  for ( k = 0; k < ks.dim(); ++k )
    {
      outer_spel = ks.ugetMin( inner_spel, k );
      if ( ! voxelset[ outer_spel ] ) break;
      outer_spel = ks.ugetMax( inner_spel, k );
      if ( ! voxelset[ outer_spel ] ) 
	{
	  descent = true;
	  break;
	}
    }
  if ( k == ks.dim() ) 
    return 0;

  // Found one inside, one outside.
  // Now move inner spel til it reaches the object boundary.
  Kn_uid moving_spel;
  if ( descent ) 
    {
      moving_spel = ks.ugetDecr( outer_spel, k );
      while ( ! voxelset[ moving_spel ] ) 
	{
	  outer_spel = moving_spel;
	  moving_spel = ks.ugetDecr( outer_spel, k );
	}
      return ks.sincident( ks.signsNeg( outer_spel ), k, false );
    }
  else 
    { // ascent
      moving_spel = ks.ugetIncr( outer_spel, k );
      while ( ! voxelset[ moving_spel ] ) 
	{
	  outer_spel = moving_spel;
	  moving_spel = ks.ugetIncr( outer_spel, k );
	}
      return ks.sincident( ks.signsNeg( outer_spel ), k, true );
    }
}


/**
 * Find a bel on the boundary of the set of spels [voxelset]. Extracts
 * the bel of [voxelset] that is closest from [inner_spel] along some
 * axis.
 *
 * @param ks the space where the expansion took place
 * @param seed an unsigned cell.
 * @param voxelset the blocking set.
 * @return a bel on the boundary of [voxelset], 0 if not found.
 */
ImaGene::Kn_sid
ImaGene::KnShapes::sfindClosestBel( const KnSpace & ks,
				    Kn_uid inner_spel,
				    const KnCharSet & voxelset )
{
  ASSERT_KnShapes( ( ks.udim( inner_spel ) == ks.dim() )
		   && voxelset[ inner_spel ] );
  
  // Try to find one spel that is outside the shape of interest.
  Kn_uid outer_spel;
  bool descent = true;
  // Look in all directions.
  uint k;
  for ( k = 0; k < ks.dim(); ++k )
    {
      outer_spel = ks.ugetMin( inner_spel, k );
      if ( ! voxelset[ outer_spel ] ) break;
      outer_spel = ks.ugetMax( inner_spel, k );
      if ( ! voxelset[ outer_spel ] ) 
	{
	  descent = false;
	  break;
	}
    }
  if ( k == ks.dim() ) 
    return 0;

  // Found one inside, one outside.
  // Now move inner spel til it reaches the object boundary.
  Kn_uid moving_spel;
  if ( descent ) 
    {
      moving_spel = ks.ugetDecr( inner_spel, k );
      while ( voxelset[ moving_spel ] ) 
	{
	  inner_spel = moving_spel;
	  moving_spel = ks.ugetDecr( inner_spel, k );
	}
      return ks.sincident( ks.signsPos( inner_spel ), k, false );
    }
  else 
    { // ascent
      moving_spel = ks.ugetIncr( inner_spel, k );
      while ( voxelset[ moving_spel ] ) 
	{
	  inner_spel = moving_spel;
	  moving_spel = ks.ugetIncr( inner_spel, k );
	}
      return ks.sincident( ks.signsPos( inner_spel ), k, true );
    }

}






/**
 * Creates the city-block distance map to the given closed
 * boundary. Inside the shape, 1 is the smallest possible distance,
 * while outside -1 is the greatest possible distance.
 *
 * @param ks the space where the distance map is computed.
 *
 * @param bdry the (non-empty) oriented boundary of some object (ie
 * a closed surface).
 *
 * @return a dyn. alloc map that stores the city-block distance map.
 */
ImaGene::KnRUCellVector<int>*
ImaGene::KnShapes::computeBdryCityBlockDistanceMap( const KnSpace & ks,
						    const KnRCellSet & bdry )
{
  KnRUCellVector<int>* distance = new KnRUCellVector<int>( ks, ks.dim() );
  KnCharSet core = KnCharSet::ucreate( ks, ks.ufirstCell( ks.dim() ), 0 );
  deque<Kn_uid> F;
  for ( KnRCellSet::cell_iterator p = bdry.begin();
	p != bdry.end();
	++p )
    {
      Kn_sid s = *p;
      uint i = ks.sorthDir( s );
      bool d = ks.sdirect( s, i );
      Kn_uid inner_spel = ks.unsigns( ks.sincident( s, i, d ) );
      Kn_uid outer_spel = ks.unsigns( ks.sincident( s, i, !d ) );
      core += inner_spel;
      core += outer_spel;
      (*distance)[ inner_spel ] = 1;
      (*distance)[ outer_spel ] = -1;
      F.push_back( inner_spel ); 
      F.push_back( outer_spel ); 
    }

  // Will store neighborhoods.
  KnSpace::deque_ucells N;

  // expand seeds.
  while ( ! F.empty() ) 
    {
      Kn_uid v = F.front();
      F.pop_front();
      int d = (*distance)[ v ];
      int next_d = d > 0 ? d+1 : d-1;
      ks.uneighborhood( v, N, false );
      while ( ! N.empty() ) 
	{
	  Kn_uid w = N.back();
	  N.pop_back();
	  if ( ! core[ w ] )
	    {
	      (*distance)[ w ] = next_d;
	      core += w;
	      F.push_back( w );
	    }
	}
    }
  return distance;
}


  
/**
 * Creates a set of oriented surfel cells whose elements represents a 
 * boundary component of the set of spels [spelset]. 
 * @param ks any space.
 * @param spelset any set of spel-like cells.
 * @param bel an oriented surfel which should be between an element of [spelset] and an element not in [spelset].
 * @return a set of cells (which are all surfels), the boundary component of [spelset] which touches [p].
 */
ImaGene::KnRCellSet
ImaGene::KnShapes::strackDirectClosedBoundary( const KnSpace & ks,
					       const KnCharSet & spelset,
					       Kn_sid bel )
{
  // boundary being extracted.
  KnRCellSet bdry = KnRCellSet::create( ks, ks.dim() - 1, true, 0 );

  ASSERT_KnShapes( ks.sisSurfel( bel ) );

  Kn_sid b;       // current tracked bel 
  Kn_sid nsurf;   // next tracked bel 
  Kn_uid vox_int; // interior voxel to current bel [b]
  Kn_uid vox_ext; // exterior voxel to current bel [b]
  uint orth_dir;  // direction orthogonal to current bel [b]
  uint orth_direct; // direct orthogonal orientation (vox_int in that direction
  uint track_dir;  // current tracked direction (one spanned by [b]).
  uint track_direct; // current direct tracked orientation.
  
  bdry += bel;

  queue<Kn_sid> qbels;
  qbels.push( bel );
  
  // For all pending bels
  while ( ! qbels.empty() )
    {
      b = qbels.front();
      qbels.pop();

      // Track in direct orientation. Get orthogonal direction.
      orth_dir = ks.sorthDir( b );
      orth_direct = ks.sdirect( b, orth_dir );
      vox_int = ks.unsigns( ks.sincident( b, orth_dir, orth_direct ) );
      vox_ext = ks.uadjacent( vox_int, orth_dir, ! orth_direct );

      // Check that [b] is a bel.
      ASSERT_KnShapes( ( spelset[ vox_int ] ) && ( ! spelset[ vox_ext ] ) );

      // Loop in all tracking directions. (Generic tracking).
      for ( KnSpace::dir_iterator q = ks.sbegin_dirs( b ); ! q.end(); ++q )
	{
	  track_dir = *q;
	  track_direct = ks.sdirect( b, track_dir );

	  // Check first next bel.
	  if ( spelset[ ks.uadjacent( vox_ext, track_dir, track_direct ) ] )
	    {
	      nsurf = ks.sincident( ks.signsNeg( vox_ext ), // is ext
				    track_dir, track_direct );
	    }
	  // Check second next bel.
	  else if ( spelset[ ks.uadjacent( vox_int, track_dir,
					   track_direct ) ] )
	    {
	      nsurf = ks.sadjacent( b, track_dir, track_direct );
	    }
	  // The third one is then the right one.
	  else
	    {
	      nsurf = ks.sincident( ks.signsPos( vox_int ),
				    track_dir, track_direct );
	    }
	  // If not already in boundary, add it to boundary and to the bels
	  // left for tracking.
	  if ( ! bdry[ nsurf ] )
	    {
	      bdry += nsurf;
	      qbels.push( nsurf );
	    }
	} // for ( q = ks.sbegin_dirs( b ); ! q.end(); ++q )
    } // while ( ! qbels.empty() )
  return bdry;
}

  
/**
 * Creates a set of oriented surfel cells whose elements represents a 
 * boundary component of the set of spels [spelset]. 
 * @param ks any space.
 * @param spelset any set of spel-like cells.
 * @param bel an oriented surfel which should be between an element of [spelset] and an element not in [spelset].
 * @return a set of cells (which are all surfels), the boundary component of [spelset] which touches [p].
 */
ImaGene::KnRCellSet
ImaGene::KnShapes::strackBoundary( const KnSpace & ks,
				   const KnCharSet & spelset,
				   Kn_sid bel )
{
  // boundary being extracted.
  KnRCellSet bdry = KnRCellSet::create( ks, ks.dim() - 1, true, 0 );

  ASSERT_KnShapes( ks.sisSurfel( bel ) );

  Kn_sid b;       // current tracked bel 
  Kn_sid nsurf;   // next tracked bel 
  Kn_uid vox_int; // interior voxel to current bel [b]
  Kn_uid vox_ext; // exterior voxel to current bel [b]
  uint orth_dir;  // direction orthogonal to current bel [b]
  uint orth_direct; // direct orthogonal orientation (vox_int in that direction
  uint track_dir;  // current tracked direction (one spanned by [b]).
  
  bdry += bel;

  queue<Kn_sid> qbels;
  qbels.push( bel );
  
  // For all pending bels
  while ( ! qbels.empty() )
    {
      b = qbels.front();
      qbels.pop();

      // Track in direct orientation. Get orthogonal direction.
      orth_dir = ks.sorthDir( b );
      orth_direct = ks.sdirect( b, orth_dir );
      vox_int = ks.unsigns( ks.sincident( b, orth_dir, orth_direct ) );
      vox_ext = ks.uadjacent( vox_int, orth_dir, ! orth_direct );

      // Check that [b] is a bel.
      ASSERT_KnShapes( ( spelset[ vox_int ] ) && ( ! spelset[ vox_ext ] ) );

      // Loop in all tracking directions. (Generic tracking).
      for ( KnSpace::dir_iterator q = ks.sbegin_dirs( b ); ! q.end(); ++q )
	{
	  track_dir = *q;

	  // ----- 1st pass with positive orientation ------

	  // Check if it goes outside the space.
	  if ( ! ks.uisMax( vox_int, track_dir ) )
	    {
	      // Check first next bel.
	      if ( spelset[ ks.uadjacent( vox_ext, track_dir, true ) ] )
		{
		  nsurf = ks.sincident( ks.signsNeg( vox_ext ), // is ext
					track_dir, true );
		}
	      // Check second next bel.
	      else if ( spelset[ ks.uadjacent( vox_int, track_dir, true ) ] )
		{
		  nsurf = ks.sadjacent( b, track_dir, true );
		}
	      // The third one is then the right one.
	      else
		{
		  nsurf = ks.sincident( ks.signsPos( vox_int ),
					track_dir, true );
		}
	      // If not already in boundary, add it to boundary and to the bels
	      // left for tracking.
	      if ( ! bdry[ nsurf ] )
		{
		  bdry += nsurf;
		  qbels.push( nsurf );
		}
	    }

	  // ----- 2nd pass with negative orientation ------

	  // Check if it goes outside the space.
	  if ( ! ks.uisMin( vox_int, track_dir ) )
	    {
	      // Check first next bel.
	      if ( spelset[ ks.uadjacent( vox_ext, track_dir, false ) ] )
		{
		  nsurf = ks.sincident( ks.signsNeg( vox_ext ), // is ext
					track_dir, false );
		}
	      // Check second next bel.
	      else if ( spelset[ ks.uadjacent( vox_int, track_dir, false ) ] )
		{
		  nsurf = ks.sadjacent( b, track_dir, false );
		}
	      // The third one is then the right one.
	      else
		{
		  nsurf = ks.sincident( ks.signsPos( vox_int ),
					track_dir, false );
		}
	      // If not already in boundary, add it to boundary and to the bels
	      // left for tracking.
	      if ( ! bdry[ nsurf ] )
		{
		  bdry += nsurf;
		  qbels.push( nsurf );
		}
	    }
	  
	} // for ( q = ks.sbegin_dirs( b ); ! q.end(); ++q )
    } // while ( ! qbels.empty() )
  return bdry;
}



/**
 * Creates a set of oriented surfel cells whose elements represents a
 * boundary component of the set of spels [spelset].  Use
 * 'SurfelNeighborhood's.  NB: only_direct_orientation should be set
 * to 'true' when the boundary is known to be closed.
 *
 * @param ks any space.
 * @param beladj a bel adjacency to track surfels.
 * @param spelset any set of spel-like cells.
 * @param bel an oriented surfel which should be between an element of [spelset] and an element not in [spelset].
 * @param only_direct_orientation when 'true', tracks only along direct orientations for each direction, otherwise tracks in both orientations.
 *
 * @return a set of cells (which are all surfels), the boundary component of [spelset] which touches [p].
 */
ImaGene::KnRCellSet
ImaGene::KnShapes::strackBoundary( const KnSpace & ks,
				   const BelAdjacency & beladj,
				   const KnCharSet & spelset,
				   Kn_sid bel,
				   bool only_direct_orientation )
{
  // boundary being extracted.
  KnRCellSet bdry = KnRCellSet::create( ks, ks.dim() - 1, true, 0 );

  ASSERT_KnShapes( ks.sisSurfel( bel ) );

  SurfelNeighborhood neighborhood;
  neighborhood.init( &ks, &beladj );

  Kn_sid b;       // current tracked bel 
  Kn_sid nsurf;   // next tracked bel 
  uint track_dir;  // current tracked direction (one spanned by [b]).
  
  bdry += bel;

  queue<Kn_sid> qbels;
  qbels.push( bel );
  
  // For all pending bels
  while ( ! qbels.empty() )
    {
      b = qbels.front();
      qbels.pop();
      neighborhood.move( b );

      // Track in direct orientation. Get orthogonal direction.

      // Loop in all tracking directions. (Generic tracking).
      for ( KnSpace::dir_iterator q = ks.sbegin_dirs( b ); ! q.end(); ++q )
	{
	  track_dir = *q;
	  bool orientation = false;
	  do 
	    {
	      if ( neighborhood.adjacentOnAnyBdry( spelset, 
						   track_dir,
						   orientation,
						   nsurf ) != 0 )
		{
		  // If not already in boundary, add it to boundary
		  // and to the bels left for tracking.
		  if ( ! bdry[ nsurf ] )
		    {
		      bdry += nsurf;
		      qbels.push( nsurf );
		    }
		}
	      orientation = ! orientation;
	    }
	  while ( orientation != only_direct_orientation );
	  	  
	} // for ( KnSpace::dir_iterator q = ks.sbegin_dirs( b ); ! q.end(); ++q )
    } // while ( ! qbels.empty() )
  return bdry;
}

/**
 * From a seed 'DigitalSurfaceTracker', extracts the whole component.
 *
 * @param ks any space.
 * @param tracker a 'DigitalSurfaceTracker' in the same space as [ks].
 *
 * @return a set of cells (which are all surfels), the boundary component of [spelset] which touches [p].
 */
ImaGene::KnRCellSet 
ImaGene::KnShapes::strack( const KnSpace & ks,
			   const DigitalSurfaceTracker & tracker )
{
  // boundary being extracted.
  KnRCellSet bdry = KnRCellSet::create( ks, ks.dim() - 1, true, 0 );
  DigitalSurfaceTracker* p = tracker.clone();
  Kn_sid b = p->current();       // current tracked bel 
  Kn_sid nsurf;   // next tracked bel 
  uint track_dir;  // current tracked direction (one spanned by [b]).
  
  bdry += b;

  queue<Kn_sid> qbels;
  qbels.push( b );
  
  // For all pending bels
  while( ! qbels.empty() )
    {
      b = qbels.front();
      qbels.pop();
      p->move( b );
      // Loop in all tracking directions. (Generic tracking).
      for ( KnSpace::dir_iterator q = ks.sbegin_dirs( b ); ! q.end(); ++q )
	{
	  track_dir = *q;
	  nsurf = p->adjacent( track_dir, false );
	  // If the surfel exists and is not not already in boundary,
	  // add it to boundary and to the bels left for tracking.
	  if ( ( p->lastCode() != 0 ) && ( ! bdry[ nsurf ] ) )
	    {
	      bdry += nsurf;
	      qbels.push( nsurf );
	    }
 	  nsurf = p->adjacent( track_dir, true );
 	  // If the surfel exists and is not not already in boundary,
 	  // add it to boundary and to the bels left for tracking.
 	  if ( ( p->lastCode() != 0 ) && ( ! bdry[ nsurf ] ) )
 	    {
 	      bdry += nsurf;
 	      qbels.push( nsurf );
 	    }
	} // for ( KnSpace::dir_iterator q = ks.sbegin_dirs( b ); ! q.end(); ++q )
    } // while ( ! qbels.empty() )

  delete p;
  return bdry;
}


/**
 * Implementation of General Bel-Tracking Algorithm [Herman98].
 * Creates a set of oriented surfel cells whose elements represents a 
 * boundary component of the set of spels [spelset]. 
 * @param ks any space.
 * @param spelset any set of spel-like cells.
 * @param bel an oriented surfel which should be between an element of [spelset] and an element not in [spelset].
 * @return a set of cells (which are all surfels), the boundary component of [spelset] which touches [p].
 */
ImaGene::KnRCellSet
ImaGene::KnShapes::strackGenDirectClosedBoundary( const KnSpace & ks,
						  const KnCharSet & spelset,
						  Kn_sid bel )
{
  // boundary being extracted.
  KnRCellSet bdry = KnRCellSet::create( ks, ks.dim() - 1, true, 0 );

  ASSERT_KnShapes( ks.sisSurfel( bel ) );

  Kn_sid b;       // current tracked bel 
  Kn_sid nsurf;   // next tracked bel 
  Kn_uid vox_int; // interior voxel to current bel [b]
  Kn_uid vox_ext; // exterior voxel to current bel [b]
  uint orth_dir;  // direction orthogonal to current bel [b]
  uint orth_direct; // direct orthogonal orientation (vox_int in that direction
  uint track_dir;  // current tracked direction (one spanned by [b]).
  uint track_direct; // current direct tracked orientation.
  
  uint i;
  uint dim_minus_2 = ks.dim() - 2;
  queue<Kn_sid> qbels;      // queue of bels being processed.
  list<Kn_sid> T;       // set of bels that may already be in boundary
  bdry += bel;
  qbels.push( bel );
  // dim() copies of bel in T.
  for ( i = 0; i <= dim_minus_2; ++i )
    T.insert( T.begin(), bel );
  
  // For all pending bels
  while ( ! qbels.empty() )
    {
      b = qbels.front();
      qbels.pop();

      // Track in direct orientation. Get orthogonal direction.
      orth_dir = ks.sorthDir( b );
      orth_direct = ks.sdirect( b, orth_dir );
      vox_int = ks.unsigns( ks.sincident( b, orth_dir, orth_direct ) );
      vox_ext = ks.uadjacent( vox_int, orth_dir, ! orth_direct );

      // Check that [b] is a bel.
      ASSERT_KnShapes( ( spelset[ vox_int ] ) && ( ! spelset[ vox_ext ] ) );

      // Loop in all tracking directions. (Generic tracking).
      for ( KnSpace::dir_iterator q = ks.sbegin_dirs( b ); ! q.end(); ++q )
	{
	  track_dir = *q;
	  track_direct = ks.sdirect( b, track_dir );

	  // Check first next bel.
	  if ( spelset[ ks.uadjacent( vox_ext, track_dir, track_direct ) ] )
	    {
	      nsurf = ks.sincident( ks.signsNeg( vox_ext ), // is ext
				    track_dir, track_direct );
	    }
	  // Check second next bel.
	  else if ( spelset[ ks.uadjacent( vox_int, track_dir,
					   track_direct ) ] )
	    {
	      nsurf = ks.sadjacent( b, track_dir, track_direct );
	    }
	  // The third one is then the right one.
	  else
	    {
	      nsurf = ks.sincident( ks.signsPos( vox_int ),
				    track_dir, track_direct );
	    }
	  // Check if it is one the boundary by searching in T.
	  list<Kn_sid>::iterator p = find( T.begin(), T.end(), nsurf );
	  if ( p != T.end() )
	    T.erase( p );
	  else
	    {
	      bdry += nsurf;
	      qbels.push( nsurf );
	      for ( i = 0; i < dim_minus_2; ++i )
		T.insert( T.begin(), nsurf );
	    }
	} // for ( q = ks.sbegin_dirs( b ); ! q.end(); ++q )
    } // while ( ! qbels.empty() )
  return bdry;
}


/**
 * Given the space [ks], a set of spels [spelset], a starting bel
 * [bel] and a direction of tracking [dir], returns the number of
 * bels of the contour, assumed to be closed.
 *
 * @param ks any space.
 * @param spelset any set of spel-like cells.
 * @param bel an oriented surfel which should be between an element of [spelset] and an element not in [spelset].
 * @param dir a tracking direction which is not the orthogonal direction to [bel].
 * @return the number of bels along the specified contour, or 0 if the input was invalid.
 */
uint
ImaGene::KnShapes::sgetContourSize( KnSpace & ks, 
				    const KnCharSet & spelset, 
				    Kn_sid bel,
				    uint dir )
{
  BelAdjacency badj( ks, true );
  ObjectBoundary bdry( badj, spelset );
  C4CIteratorOnSurface* cp = bdry.newC4CIterator( bel, dir );
  Kn_sid b = bel;
  uint nb = 0;
  do 
    {
      // Go to next one.
      cp->next();
      b = cp->current();
      ++nb;
    }
  while ( b != bel );
  delete cp;
  return nb;
}


/**
 * Given a boundary surface [bdry] in [ks], compute its interior as a
 * set of unoriented spels.  The technique is to fill line by line and
 * tests the intersection with the surface.
 *
 * @param ks the digital space.
 * @param bdry the digital Jordan surface
 *
 * @param empty_is_inside when 'true', an empty line is considered
 * interior, otherwise exterior.
 *
 * @return its set of interior spels
 */
ImaGene::KnCharSet
ImaGene::KnShapes::ucomputeInterior( const KnSpace & ks, 
				     const KnRCellSet & bdry,
				     bool empty_is_inside )
{
  KnCharSet image = KnCharSet::ucreate( ks, ks.uspel( ks.ufirst() ), 0 );
  // Scan image to fill pixels.
  std::deque<Kn_uid> pixels;

  KnSpaceScanner scan2( ks, 
			ks.ufirstCell( ks.dim() ),
			ks.ulastCell( ks.dim() ) );

  bool in_mode = empty_is_inside; 
  Kn_uid p = scan2.begin();
  do 
    {
      if ( p != ks.ugetMax( p, 0 ) )
	{ 
	  pixels.push_front( p );
	  Kn_sid b = ks.sincident( ks.signsPos( p ), 0, true );
	  if ( bdry[ b ] )
	    {
	      while ( ! pixels.empty() )
		{
		  image += pixels.front();
		  pixels.pop_front();
		}
	      in_mode = false;
	    }
	  else if ( bdry[ ks.sopp( b ) ] )
	    {
	      pixels.clear();
	      in_mode = true;
	    }
	}
      else
	{
	  pixels.push_front( p );
	  if ( in_mode )
	    {
	      while ( ! pixels.empty() )
		{
		  image += pixels.front();
		  pixels.pop_front();
		}
	    }
	  else
	    pixels.clear();
	  in_mode = empty_is_inside;
	}
    }
  while ( scan2.increment( p ) ); 
  return image;
}





///////////////////////////////////////////////////////////////////////////////
// ------------------------- Noise services ---------------------------------

/**
 * Given the space [ks], a set of spels [spelset], its boundary
 * [bdry], some probability for spels to change state according to
 * laws [p_in] and [p_out], computes a noisy version of the given set
 * of spels.
 * 
 * @param ks any space.
 *
 * @param spelset any set of spel-like cells.
 *
 * @param bdry the oriented boundary of [spelset]
 *
 * @param p_in an array storing the probability of changing an inside
 * spel to an outside spel according to the city-block distance to the
 * boundary.
 *
 * @param p_out an array storing the probability of changing an
 * outside spel to an inside spel according to the city-block distance
 * to the boundary.
 *
 * @param inner_spel (returns) a spel deep inside the noisified
 * object (or unmodified if none exists).
 *
 * @param outer_spel (returns) a spel deep outside the noisified
 * object (or unmodified if none exists).
 *
 * @return the noisified object.
 */
ImaGene::KnCharSet
ImaGene::KnShapes::noisifyObject( KnSpace & ks,
				  const KnCharSet & spelset, 
				  const KnRCellSet & bdry,
				  const std::vector<double> & p_in,
				  const std::vector<double> & p_out,
				  Kn_uid & inner_spel,
				  Kn_uid & outer_spel )
{
  KnCharSet noisy_object( spelset );
  KnRUCellVector<int>* dmap = computeBdryCityBlockDistanceMap( ks, bdry );
  Kn_uid in_spel = 0;
  int in_d = 0; 
  Kn_uid out_spel = 0;
  int out_d = 0;

  KnSpaceScanner scanner( ks, 
			  ks.ufirstCell( ks.dim() ),
			  ks.ulastCell( ks.dim() ) );
  Kn_uid p = scanner.begin();
  do 
    { // ... whatever [p] is the current cell
      int d = (*dmap)[ p ];
      if ( d < 0 )
	{ // outside
	  if ( ( -d < (int) p_out.size() )
	       && ( Mathutils::random1() < p_out[ -d ] ) )
	    noisy_object += p;
	  else if ( d < out_d ) 
	    { // new best outer spel.
	      out_spel = p;
	      out_d = d;
	    }
	}
      else 
	{ // inside
	  if ( ( d < (int) p_in.size() )
	       && ( Mathutils::random1() < p_in[ d ] ) )
	    noisy_object -= p;
	  else if ( d > in_d ) 
	    { // new best outer spel.
	      in_spel = p;
	      in_d = d;
	    }
	}
    }
  while ( scanner.increment( p ) ); 
  
  if ( in_spel != 0 ) inner_spel = in_spel;
  if ( out_spel != 0 ) outer_spel = out_spel;

  delete dmap;

  return noisy_object;
}
  

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::KnShapes::~KnShapes()
{
}

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::KnShapes::selfDisplay( ostream& that_stream ) const
{
  that_stream << endl;
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::KnShapes::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

