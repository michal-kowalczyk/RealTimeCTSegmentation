/** @file KnShapes.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnShapes.h
//
// Creation : 2002/08/05
//
// Version : 2002/08/05
//
// Author : JOL
//
// Summary : Header file for files KnShapes.ih and KnShapes.cxx
//
// History :
//	2002/08/05 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnShapes declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnShapes_RECURSES)
#error Recursive header files inclusion detected in KnShapes.h
#else // defined(KnShapes_RECURSES)
#define KnShapes_RECURSES

#if !defined KnShapes_h
#define KnShapes_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnRCellSet.h"
#include "ImaGene/digitalnD/KnRCellVector.h"
#include "ImaGene/digitalnD/DigitalSurfaceTracker.h"

#include <cmath>
#include <queue>
#include <set>
#include <list>
#include <algorithm>
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/digitalnD/KnSpaceScanner.h"
#include "ImaGene/digitalnD/KnSpaceCoordScanner.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
#include "ImaGene/digitalnD/ObjectBoundary.h"
#include "ImaGene/digitalnD/SurfelNeighborhood.h"

#include "ImaGene/dgeometry2d/C4CGeometry.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/KnShapes.ih"
#endif
//////////////////////////////////////////////////////////////////////////////

// #include "ImaGene/planeRecognition/Z3.h"
// #include "ImaGene/planeRecognition/utils.h"
// #include "ImaGene/planeRecognition/COBA.h"

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

using namespace std;


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class KnShapes
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnShapes' <p>
 * Aim: To provide functions that builds n-dimensional shapes.
 */
class KnShapes
{
  // ------------------------- Static services --------------------------------
public:

  /**
   * Given a cell [center] and a integer number [r], returns a cell
   * of same type as [center] but whose coordinates are each decreased by [r]
   * (or 0 if the coordinate - r was negative).
   * @param ks the digital space
   * @param center any cell.
   * @param r half the size of the cube edge.
   * @return the lower bound of the hypercube of center [center] and half-edge [r].
   */
  static Kn_uid ugetCubeLowerBound( const KnSpace & ks,
				    Kn_uid center,
				    Kn_size r );

  /**
   * Given a cell [center] and a integer number [r], returns a cell
   * of same type as [center] but whose coordinates are each increased by [r]
   * (or size[ k ] - 1 if the coordinate + r was outside the space bounds).
   * @param ks the digital space
   * @param center any cell.
   * @param r half the size of the cube edge.
   * @return the upper bound of the hypercube of center [center] and half-edge [r].
   */
  static Kn_uid ugetCubeUpperBound( const KnSpace & ks,
				    Kn_uid center,
				    Kn_size r );

  /**
   * Creates a set of unsigned volumic cells whose elements represents a
   * parallelepiped of lower vertex [low] and upper vertex [up].
   * @param ks any space.
   * @param low a cell that is the lower bound of the box.
   * @param up a cell that is the upper bound of the box.
   * @return a set of spel-like cells.
   */
  static KnCharSet umakeVolumicParallelepiped( const KnSpace & ks,
					       Kn_uid low,
					       Kn_uid up );

  /**
   * Creates a set of unsigned volumic cells whose elements represents a
   * sphere of center [center] and radius [inclusive_radius].
   * @param ks any space.
   * @param center a cell that is the center of the sphere.
   * @param inclusive_radius the radius of the sphere.
   * @return a set of spel-like cells.
   */
  static KnCharSet umakeVolumicSphere( const KnSpace & ks,
				       Kn_uid center,
				       float inclusive_radius );
  
  /**
   * Creates a set of unsigned surfel cells whose elements represents a
   * sphere of center [center] and radius [inclusive_radius].
   * @param ks any space.
   * @param center a cell that is the center of the sphere (only coords are significant).
   * @param inclusive_radius the radius of the sphere.
   * @return a set of cells (which are all surfels).
   */
  static KnRCellSet umakeSurfacicSphere( const KnSpace & ks,
				      Kn_uid center,
				      float inclusive_radius );

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
  static KnCharSet uexpandSeedWithinIncidentBounds( const KnSpace & ks,
						    Kn_uid seed,
						    const KnCharSet & bset );

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
  static KnCharSet uexpandSeedWithinBounds( const KnSpace & ks,
					    Kn_uid seed,
					    const KnCharSet & bset );

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
  static Kn_sid sfindFurthestBel( const KnSpace & ks,
				  Kn_uid inner_spel,
				  const KnCharSet & voxelset );

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
  static Kn_sid sfindClosestBel( const KnSpace & ks,
				 Kn_uid inner_spel,
				 const KnCharSet & voxelset );


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
  static KnRUCellVector<int>* computeBdryCityBlockDistanceMap
    ( const KnSpace & ks,
      const KnRCellSet & bdry );


  /**
   * Creates a set of unsigned surfel cells whose elements represents the 
   * boundary of the set of spels [spelset]. The cells are within the limits
   * given by [lower_bound] and [upper_bound].
   * @param ks any space.
   * @param spelset any set of spel-like cells.
   * @param lower_bound only the cells with greater coordinates than [lower_bound] are scanned.
   * @param upper_bound only the cells with lower coordinates than [upper_bound] are scanned.
   * @return a set of cells (which are all surfels), the boundary of [spelset].
   */
  static KnRCellSet umakeBoundary( const KnSpace & ks,
				const KnCharSet & spelset,
				Kn_uid lower_bound,
				Kn_uid upper_bound );
  
  /**
   * Creates a set of signed surfel cells whose elements represents the 
   * boundary of the set of spels [spelset].
   * The spel set is considered as positively oriented and the returned set is
   * exactly its oriented lower boundary.
   * @param ks any space.
   * @param spelset any set of spel-like cells.
   * @return a set of cells (which are all surfels), the boundary of [spelset].
   */
  INLINE static KnRCellSet smakeBoundary( const KnSpace & ks,
				       const KnCharSet & spelset );
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
  static KnRCellSet smakeBoundary( const KnSpace & ks,
				const KnCharSet & spelset,
				Kn_uid lower_bound,
				Kn_uid upper_bound );
  
  /**
   * Creates a set of unsigned surfel cells whose elements represents the 
   * boundary of the set of spels [spelset]. 
   * @param ks any space.
   * @param spelset any set of spel-like cells.
   * @return a set of cells (which are all surfels), the boundary of [spelset].
   */
  INLINE static KnRCellSet umakeBoundary( const KnSpace & ks,
				       const KnCharSet & spelset );
  
  /**
   * Creates a set of oriented surfel cells whose elements represents a 
   * boundary component of the set of spels [spelset]. Tracks only in the
   * direction orientation of bels. The boundary should thus be closed.
   * @param ks any space.
   * @param spelset any set of spel-like cells.
   * @param bel an oriented surfel which should be between an element of [spelset] and an element not in [spelset].
   * @return a set of cells (which are all surfels), the boundary component of [spelset] which touches [p].
   */
  static KnRCellSet strackDirectClosedBoundary( const KnSpace & ks,
					     const KnCharSet & spelset,
					     Kn_sid bel );
  
  /**
   * Creates a set of oriented surfel cells whose elements represents a 
   * boundary component of the set of spels [spelset]. 
   * @param ks any space.
   * @param spelset any set of spel-like cells.
   * @param bel an oriented surfel which should be between an element of [spelset] and an element not in [spelset].
   * @return a set of cells (which are all surfels), the boundary component of [spelset] which touches [p].
   */
  static KnRCellSet strackBoundary( const KnSpace & ks,
				 const KnCharSet & spelset,
				 Kn_sid bel );

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
  static KnRCellSet strackBoundary( const KnSpace & ks,
				    const BelAdjacency & beladj,
				    const KnCharSet & spelset,
				    Kn_sid bel,
				    bool only_direct_orientation = true );

  /**
   * From a seed 'DigitalSurfaceTracker', extracts the whole component.
   *
   * @param ks any space.
   * @param tracker a 'DigitalSurfaceTracker' in the same space as [ks].
   *
   * @return a set of cells (which are all surfels), the boundary component of [spelset] which touches [p].
   */
  static KnRCellSet strack( const KnSpace & ks,
			    const DigitalSurfaceTracker & tracker );
  
  
  /**
   * Implementation of General Bel-Tracking Algorithm [Herman98].
   * Creates a set of oriented surfel cells whose elements represents a 
   * boundary component of the set of spels [spelset]. 
   * @param ks any space.
   * @param spelset any set of spel-like cells.
   * @param bel an oriented surfel which should be between an element of [spelset] and an element not in [spelset].
   * @return a set of cells (which are all surfels), the boundary component of [spelset] which touches [p].
   */
  static KnRCellSet strackGenDirectClosedBoundary( const KnSpace & ks,
						const KnCharSet & spelset,
						Kn_sid bel );


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
  static uint sgetContourSize( KnSpace & ks, 
			       const KnCharSet & spelset, 
			       Kn_sid starting_bel,
			       uint dir );


  /**
   * Given a boundary surface [bdry] in [ks], compute its interior as
   * a set of unoriented spels. The technique is to fill line by line and
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
  static KnCharSet ucomputeInterior( const KnSpace & ks, 
				     const KnRCellSet & bdry,
				     bool empty_is_inside = false );


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
  static KnCharSet noisifyObject( KnSpace & ks,
				  const KnCharSet & spelset, 
				  const KnRCellSet & bdry,
				  const std::vector<double> & p_in,
				  const std::vector<double> & p_out,
				  Kn_uid & inner_spel,
				  Kn_uid & outer_spel );
				  

  
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~KnShapes();

  // ------------------------- Interface --------------------------------------
public:
  /**
   * Writes/Displays the object on an output stream.
   * @param that_stream the output stream where the object is written.
   */
  void selfDisplay( std::ostream & that_stream ) const;
  /**
   * Checks the validity/consistency of the object.
   * @return 'true' if the object is valid, 'false' otherwise.
   */
  bool OK() const;
  
  // ------------------------- Datas ------------------------------------------
private:

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE KnShapes();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE KnShapes( const KnShapes & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE KnShapes & operator=( const KnShapes & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'KnShapes'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'KnShapes' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const KnShapes & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/KnShapes.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnShapes_h

#undef KnShapes_RECURSES
#endif // else defined(KnShapes_RECURSES)
