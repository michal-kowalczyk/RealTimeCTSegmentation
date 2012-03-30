/** @file ShapeHelper.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ShapeHelper.h
//
// Creation : 2004/11/22
//
// Version : 2004/11/22
//
// Author : JOL
//
// Summary : Header file for files ShapeHelper.ih and ShapeHelper.cxx
//
// History :
//	2004/11/22 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ShapeHelper declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ShapeHelper_RECURSES)
#error Recursive header files inclusion detected in ShapeHelper.h
#else // defined(ShapeHelper_RECURSES)
#define ShapeHelper_RECURSES

#if !defined ShapeHelper_h
#define ShapeHelper_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/Shapes.h"
#include "ImaGene/base/Arguments.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/K2Space.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnRCellSet.h"
#include "ImaGene/digitalnD/KnRCellVector.h"
#include "ImaGene/digitalnD/Embedder.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene
{

///////////////////////////////////////////////////////////////////////////////
// class ShapeHelper
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'ShapeHelper' <p> Aim: Helper for classes for
 * defining shapes and discretization of shapes. Not instantiable.
 */
class ShapeHelper
{
  // ------------------------- Static services ------------------------------
public:

  /**
   * Digitized the shape [shape] wrt to the space [ks] which is
   * embedded in the nD euclidean space as specified by [embedder].
   * The digitization is the simplest one: the centroid of each spel
   * of the space is checked to be inside the given shape. The spel is
   * added to the spelset accordingly.
   *
   * @param ks any digital space.
   * @param embedder an embedding of [ks].
   * @param shape any shape in the space.
   * @return the set of spels of the space inside the shape.
   * @deprecated since v1.5 
   * @see digitizeShapeByGauss
   */
  static KnCharSet makeShape( KnSpace & ks, 
			      const Embedder & embedder, 
			      const Shape & shape );
  /**
   * Digitized the shape [shape] wrt to the space [ks] which is
   * embedded in the nD euclidean space as specified by [embedder].
   * The digitization is the simplest one: the centroid of each spel
   * of the space is checked to be inside the given shape. The spel is
   * added to the spelset accordingly.
   *
   * @param ks any digital space.
   * @param embedder an embedding of [ks].
   * @param shape any shape in the space.
   * @return the set of spels of the space inside the shape.
   */
  static KnCharSet digitizeShapeByGauss( KnSpace & ks, 
					 const Embedder & embedder, 
					 const Shape & shape );
 

  /**
   * Searches for a surfel on the boundary of the 2D shape [sshape] in
   * the direction given by [dir]. Use dichotomy to speed up the
   * process.
   *
   * @param ks any 2d digital space.
   * @param embedder an embedding of [ks].
   * @param sshape any starshape in the euclidean plane.
   * @param dir the direction of the line that intersects the starshape.
   *
   * @return a bel on the shape boundary which is approximately in the
   * direction [dir] wrt to the interior point of the starshape.
   */
  static Kn_sid getBelOnStarShapedBoundary( KnSpace & ks,
					    const Embedder & embedder,
					    const StarShaped & sshape,
					    const Vector2D & dir );
  /**
   * Searches for a surfel on the boundary of the 2D shape [sshape] in
   * the direction given by [dir]. Use dichotomy to speed up the
   * process.
   *
   * @param ks any 2d digital space.
   * @param embedder an embedding of [ks].
   * @param sshape any starshape in the euclidean plane.
   * @param dir the direction of the line that intersects the starshape.
   *
   * @return a bel on the shape boundary which is approximately in the
   * direction [dir] wrt to the interior point of the starshape.
   *
   * @deprecated @since v1.5, calls @see getBelOnStarShapedBoundary
   */
  static Kn_sid getBelOnStarShape2DBoundary( KnSpace & ks,
					    const Embedder & embedder,
					    const StarShaped & sshape,
					    const Vector2D & dir );

  /**
   * Computes the face-connected set containing the spel [in] and
   * included in the set [voxset] and return a bel on its boundary.
   *
   * @param ks the digital space,
   * @param voxset the digital shape,
   * @param in a spel in [voxset],
   * @param main_inner_comp (returns) the connected set included in
   * [voxset] and containing [in],
   *
   * @return a bel on the boundary of [main_inner_comp].
   */
  static Kn_sid findInnerObject( KnSpace* ks, KnCharSet voxset, 
				 Kn_uid in,
				 KnCharSet & main_inner_comp );
  /**
   * Computes the face-connected set containing the spel [out] and
   * excluding the set [voxset] and return a bel on its boundary.
   *
   * @param ks the digital space,
   * @param voxset the digital shape,
   * @param out a spel outside [voxset],
   * @param main_outer_comp (returns) the connected set not intersecting
   * [voxset] and containing [out],
   * @param in a spel in [voxset],
   *
   * @return a bel on the boundary of [main_outer_comp].
   *
   * @todo Check this method (symmetry with findInnerObject)
   */
  static Kn_sid findOuterObject( KnSpace* ks, KnCharSet voxset, Kn_uid out, 
				 KnCharSet & main_outer_comp, Kn_uid in  );

  /**
   * Exports the 2D digital set [voxset] as a PGM file in the stream [out].
   *
   * @param out the output stream,
   * @param ks the digital space,
   * @param voxset the digital set.
   */
  static void exportToPGM( std::ostream & out, 
			   KnSpace* ks, const KnCharSet & voxset );


  
  /**
   * Exports the Unsigned Cell vector  as a PGM file in the stream [out].
   *
   * @param out the output stream,
   * @param ks the digital space,
   * @param cellVector the cell vector containing image information.
   */
  static void exportToPGM(std::ostream & out, KnSpace* ks, 
			  const KnRUCellVector<int> & cellVector );
  
  

  /**
   * Exports the 2D digital set [voxset] as a PGM file in the stream [out].
   *
   * @param out the output stream,
   * @param ks the digital space,
   * @param voxset the digital set.
   */
  static void exportToPGM3d( std::ostream & out, 
			   KnSpace* ks, const KnCharSet & voxset );




/**
 * Exports the Unsigned Cell vector as a PGM3d file in the stream [out].
 *
 * @param out the output stream,
 * @param ks the digital space,
 * @param cellVector the cell vector containing image information.
 */
  static void exportToPGM3d( std::ostream & out, 
			     KnSpace* ks, 
			     const KnRUCellVector<int> &cellValue );


  /**
   * Imports a 2D digital set from a PGM file in the stream
   * [in]. Creates a digital space and a digital set.
   *
   * @param out the output stream,
   * @param ks (returns) a dyn. allocated digital space,
   * @param voxset (returns) the dyn. allocated digital set (lying in [ks]).
   *
   * @param threshold if the value>=threshold then the pixel is added
   * to [voxset]
   *
   * @return 'true' if everything went well, otherwise return 'false'
   * and 0 for both pointers.
   */
  static bool importFromPGM( std::istream & in, 
			     KnSpace* & ks,
			     KnCharSet* & voxset,
			     uint threshold = 128,uint bordXY=0, bool quiet=false  );



/**
 * Imports a PGM file in the stream [in]. Return a vector of integer
 * representing the PGM image indexed by unsigned cell code.
 *
 * @param in the input stream,
 * @param ks (returns) a dyn. allocated digital space,
 * @param KnRUCellVector a vector indexed by unsigned cell code
 *
 * @return 'true' if everything went well, otherwise return 'false'
 * and 0 for both pointers.
 */
static bool importFromPGM( std::istream & in, 
			   KnSpace* & ks,
			   KnRUCellVector<int>* & vectorVal);





/**
 * Imports a 3D digital set from a 3dPGM file in the stream
 * [in]. Creates a digital space and a digital set.
 *
 * @param out the output stream,
 * @param ks (returns) a dyn. allocated digital space,
 * @param voxset (returns) the dyn. allocated digital set (lying in [ks]).
 * @param thresholdMin thresholdMax  if the thresholdMin<=value<=thresholdMax then the pixel is added to [voxset]
 *
 * @return 'true' if everything went well, otherwise return 'false'
 * and 0 for both pointers.
 */
static bool  importFromPGM3d ( std::istream & in, 
			       KnSpace* & ks,
			       KnCharSet* & voxset,
			       uint thresholdMin=0, uint thresholdMax=128, uint bordXYZ=0 );





/**
 * Imports a PGM file in the stream [in]. Return a vector of integer
 * representing the PGM3D image indexed by unsigned cell code.
 *
 * @param in the input stream,
 * @param ks (returns) a dyn. allocated digital space,
 * @param KnRUCellVector (returns) a dyn. allocated vector indexed by unsigned cell code,
 * @param bordXYZ used to contruct a larger space,
 * @return 'true' if everything went well, otherwise return 'false'.
 */

static bool  importFromPGM3d ( std::istream & in, 
			       KnSpace* & ks,
			       KnRUCellVector<  int > *&vectorVal,
			       uint bordXYZ=0 );

  /**
   * Builds the digital surface corresponding to the Freeman chain
   * code. The digital space \b must contain the FreemanChain.
   *
   * @param ks the digital space,
   * @param c the Freeman chaincode,
   * @param loop indicates if the Freeman chaincode loops or not (def. true).
   *
   * @return a set of oriented surfels that are the representation of
   * the Freeman chaincode in [ks].
   */
  static KnRCellSet makeContourFromFreemanChain( const KnSpace* ks,
						 const FreemanChain & c,
						 bool loop = true );

  /**
   * Builds the digital surface corresponding to the Freeman chain
   * code. A big enough digital space for the surface is built and
   * returned.
   *
   * @param c (updates) the Freeman chaincode (may be translated),
   *
   * @return (returns) a dyn. allocated digital space,
   */
  static K2Space* makeSpaceFromFreemanChain( FreemanChain & c );


  /** 
   * Constructs a star-shaped objects from command-line
   * arguments. Interpreted arguments are:
   * - "-circle <r>" : generates a disk.
   * - "-flower <R> <r> <n> <phi>" : generates a flower.
   * - "-accflower <R> <r> <n>" : generates a phase-accelerating flower.
   * - "-rsquare <R> <r>" : generates a disk with a rounded square on a quadrant.
   * - "-fuzzy_uniform" : gives a slight perturbation on the shape center.
   * - "-center <x> <y>" : choose the center of the shape in real coordinates (0.0 0.0).
   * - "-step" : the resolution step (used for the perturbation).
   * 
   * @param args an argument object storing options generally given on a
   * command line.
   * 
   * @return a (dyn. alloc.) starshaped object.
   */
  static StarShaped* makeStarShapedFromArgs( const Arguments & args );

  /**
   * Add the standard arguments for defining star-shaped objects to argument
   * object [args].
   *
   * @param args (updated) the argument object to enrich.
   */
  static void addStarShapedArgs( Arguments & args );


  /**
   * Given the arguments [args] (-noisify ...), perturbate the shape
   * given by the digital set [voxset] and an optional starting bel
   * [bel]. If the shape is connected, giving or not giving [bel] does
   * not change anything, otherwise it specifies the contour to
   * perturbate.
   *
   * @param args the arguments specifying how to add noise to the shape,
   *
   * @param voxset (updates) the shape that is modified, 
   *
   * @param bel a bel on the boundary of the shape or 0 if no contour is
   * specified,
   *
   * @param in (returns) a spel deep inside the noisified object (or
   * unmodified if none exists).
   *
   * @param out (returns) a spel deep outside the noisified object (or
   * unmodified if none exists).
   */
  static void addNoiseFromArgs( const Arguments & args,
				KnSpace* ks, 
				KnCharSet & voxset, 
				Kn_sid bel, 
				Kn_uid & in, Kn_uid & out );

  /**
   * Add the standard arguments for defining noisy objects to argument
   * object [args].
   *
   * @param args the arguments specifying how to apply noise.
   */
  static void addNoiseArgs( Arguments & args );

  /**
   * Contructs a 3D digital shape from the arguments [args] lying in
   * the space [ks].
   *
   * @param args the arguments specifying the shape to build.
   * @param ks the digital space.
   * @return a digital shape (empty if no shape was specified).
   * @see addSimple3DShapesArgs
   */
  static KnCharSet makeSimple3DShapesFromArgs( const Arguments & args,
					       KnSpace & ks );

  /**
   * Add the standard arguments for defining simple 3D shapes to argument
   * object [args].
   * -sphere <r>
   * -ellipse <a[=10]> <b[=5]> <c[=3]>
   * -cube <c>
   * -octahedron <c>
   * -cms <c> <r>
   * -ry <radian>
   * -rz <radian>
   *
   * @param args (updated) the argument object to enrich.
   * @see makeSimple3DShapesFromArgs
   */
  static void addSimple3DShapesArgs( Arguments & args );



  

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~ShapeHelper();

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
   * Forbidden by default (protected to avoid g++ warnings).
   */
  INLINE ShapeHelper();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE ShapeHelper( const ShapeHelper & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE ShapeHelper & operator=( const ShapeHelper & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'ShapeHelper'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'ShapeHelper' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const ShapeHelper & that_object_to_display );


} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/ShapeHelper.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ShapeHelper_h

#undef ShapeHelper_RECURSES
#endif // else defined(ShapeHelper_RECURSES)
