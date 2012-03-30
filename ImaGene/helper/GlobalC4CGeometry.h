/** @file GlobalC4CGeometry.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : GlobalC4CGeometry.h
//
// Creation : 2004/11/25
//
// Version : 2004/11/25
//
// Author : JOL
//
// Summary : Header file for files GlobalC4CGeometry.ih and GlobalC4CGeometry.cxx
//
// History :
//	2004/11/25 : ?Name? : ?What?
//
// Rcs Id : "@(#)class GlobalC4CGeometry declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(GlobalC4CGeometry_RECURSES)
#error Recursive header files inclusion detected in GlobalC4CGeometry.h
#else // defined(GlobalC4CGeometry_RECURSES)
#define GlobalC4CGeometry_RECURSES

#if !defined GlobalC4CGeometry_h
#define GlobalC4CGeometry_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/dgeometry2d/C4CELength.h"
#include "ImaGene/helper/C4CTangentialCoverGeometry.h"
#include "ImaGene/helper/LinearMinimizer.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene {
  

///////////////////////////////////////////////////////////////////////////////
// class GlobalC4CGeometry
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'GlobalC4CGeometry' <p> Aim: Provides
 * geometric estimators for 4-connected contour based on a global
 * approximation of the contour. The contour is approached with circle
 * arcs, and the computation is made with a LinearMinimizer.
 */
class GlobalC4CGeometry
{
public:

  /**
   * Represents the geometry of a common part. Assumed to be a
   * circular arc. A common part corresponds to a connected piece of
   * the digital contour. From a discrete point of view, it is also a
   * digital straight segment.
   */
  struct CommonPartGeometry 
  {
    /**
     * The angle of the tangent at the beginning of the common part
     * wrt to the x-axis. Is between [min] and [max] (modulo 2pi).
     */
    double angle1;

    /**
     * The angle of the tangent at the end of the common part
     * wrt to the x-axis.
     */
    double angle2;

    /**
     * Minimum valid angle.
     */
    double min;

    /**
     * Maximum valid angle.
     */
    double max;

    /**
     * This is the length of the contour piece.
     */
    double length;

    /**
     * It is the sum of the lengths to the starting surfel of the
     * first common part, ie the curvilinear abscissa of the first
     * pointel in the common part.
     */
    double curv_abscissa;
    
    /**
     * The first pointel of the common part has a discrete abscissa
     * (an integer). Useful to relate this structure with
     * discrete contours.
     */
    uint disc_abscissa; 

    /** 
     * The number of surfels in this common part, ie the discrete length.
     */
    uint disc_length;
    
    /**
     * @param cabs a curvilinear abscissa inside the common part.
     * @return the angle wrt x-axis for this abscissa.
     */
    INLINE double angleFromCurvilinearAbscissa( double cabs ) const;

    /**
     * @param cabs a curvilinear abscissa inside the common part.
     * @return the curvature for this abscissa.
     */
    INLINE double curvatureFromCurvilinearAbscissa( double cabs ) const;

    /**
     * @param dabs a discrete abscissa inside the common part.
     * @return the angle wrt x-axis for this abscissa.
     */
    INLINE double angleFromDiscreteAbscissa( double dabs ) const;
 
    /**
     * @param dabs a discrete abscissa inside the common part.
     * @return the curvature for this abscissa.
     */
    INLINE double curvatureFromDiscreteAbscissa( double dabs ) const;
    
    /**
     * @param dabs a discrete abscissa inside the common part.
     * @return the curvilinear abscissa for this abscissa.
     */
    INLINE double curvilinearFromDiscreteAbscissa( double dabs ) const;

  };
  

  /**
   * Comparator for CommonPartGeometry based on curvilinear abscissa.
   */
  struct CPGCurvAbsCmp
  {
    /**
     * less than for CommonPartGeometry based on curvilinear abscissa.
     * @param v1 the first value.
     * @param v1 the second value.
     * @return 'v1.curv_abscissa < v2.curv_abscissa'
     */
    INLINE bool operator()( const CommonPartGeometry & v1, 
			    const CommonPartGeometry & v2 );
    
  };


  /**
   * Comparator for CommonPartGeometry based on discrete abscissa.
   */
  struct CPGDiscAbsCmp
  {
    /**
     * Constructor.  Needs the discrete length of the contour to
     * perform comparisons modulo this value.
     * @param dl the discrete length.
     */
    INLINE CPGDiscAbsCmp( uint dl );
    
    /**
     * Discrete abscissa is modulo.
     */
    Mathutils::ModuloComputer mc;
    
    /**
     * less than for CommonPartGeometry based on discrete abscissa.
     * @param v1 the first value.
     * @param v1 the second value.
     * @return 'v1.disc_abscissa < v2.disc_abscissa'
     */
    INLINE bool operator()( const CommonPartGeometry & v1,
			    const CommonPartGeometry & v2 );
    
  };
  

  
  

  /**
   * Represents the local geometry of a contour.
   */
  struct LocalGeometry
  {
    /**
     * The angle between the tangent and the x-axis.
     */
    double angle_to_x;
    /**
     * The (signed) curvature.
     */
    double curvature;
    /**
     * The geodesic distance to the surfel with index 0 of the contour.
     */
    double geo_distance;
  };
  


  // ------------------------- Static services ------------------------------
public:

  /**
   * Displays the geometry of [global_geom] on the output stream [out].
   * @param out any output stream.
   * @param global_geom any (initialized and computed) global geometry object.
   */
  static void displayGeometry( std::ostream & out,
			       const GlobalC4CGeometry & global_geom );
  
  

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~GlobalC4CGeometry();

  /**
   * Constructor. Empty object.
   */
  GlobalC4CGeometry();

  /**
   * Resets the geometry. The object is in the same state as if it was
   * just created.
   */
  void reset();

  /**
   * Initializes the object so that it can compute the geometry of the
   * contour given in parameter. The contour is represented by its
   * tangential cover [tcover] and needs also an iterator [it] to make
   * the correct change of frames.
   *
   * @param tcover the tangential cover of a digital curve.
   * @param tcover_geometry the geometry of the tangential cover [tcover].
   */
  void init( const C4CTangentialCover & tcover,
	     const C4CTangentialCoverGeometry & tcover_geometry );

  /**
   * Initializes the object so that it can compute the geometry of the
   * contour given in parameter. The contour is represented by its
   * tangential cover [tcover] and its cover geometry
   * [tcover_geometry].
   *
   * @param tcover the tangential cover of a digital curve.
   * @param tcover_geometry the geometry of the tangential cover [tcover].
   * @param elength the elementary lengthes of the surfels.
   * @param lm a specialized linear minimizer.
   */
  void init( const C4CTangentialCover & tcover,
	     const C4CTangentialCoverGeometry & tcover_geometry,
	     const C4CELength & elength,
	     LinearMinimizer* lm = 0 );



  // ------------------------- Common part services ---------------------------
public:

  /**
   * @return the number of common parts.
   */
  INLINE uint nbCommonParts() const;
  
  /**
   * @param i the index of a common part.
   * @return the common part with index [i].
   */
  INLINE const C4CTangentialCover::CommonPart & getCommonPart( uint i ) const;

  /**
   * @param i the index of a common part.
   * @return the geometry of the common part with index [i].
   */
  INLINE const CommonPartGeometry & getCommonPartGeometry( uint i ) const;

  /**
   * @param cabs a valid curvilinear abscissa on the contour. 
   * @return the index of the common part containing the point of curvilinear abscissa [cabs].
   */
  uint indexCPFromCurvilinearAbscissa( double cabs ) const;

  /**
   * @param dabs a valid discrete abscissa on the contour. 
   * @return the index of the common part containing the point of discrete abscissa [dabs].
   */
  uint indexCPFromDiscreteAbscissa( double dabs ) const;
  


  // ------------------------- Geometry services ------------------------------
public:

  
  /**
   * Computes the geometry of the whole contour. Stops when both
   * conditions are true (if they are valid).
   *
   * @param max_eps defines when to finish the optimization process
   * (ignored if negative).
   *
   * @param sum_eps defines when to finish the optimization process
   * (ignored if negative).
   */
  void computeContourGeometry( double max_eps, double sum_eps = -1.0 );

  /**
   * Returns the contour geometry at a given point specified by its
   * curvilinear abscissa.
   *
   * @param cabs a valid curvilinear abscissa on the contour. 
   * @param lgeo (returns) the local geometry at this point.
   */
  void geometryFromCurvilinearAbscissa( double cabs, 
					LocalGeometry & lgeo ) const;

  /**
   * Returns the contour geometry at a given point specified by its
   * discrete abscissa (pointel have integer abscissa while surfel
   * have half-integer abscissa).
   *
   * @param dabs a valid discrete abscissa on the contour. 
   * @param lgeo (returns) the local geometry at this point.
   */
  void geometryFromDiscreteAbscissa( double dabs, 
				     LocalGeometry & lgeo ) const;

  /**
   * Returns the contour geometry at a given point specified by its
   * discrete abscissa (pointel have integer abscissa while surfel
   * have half-integer abscissa).
   *
   * @param i1 index of the first surfel where the local geometry is computed.
   * @param i2 index of the first after the last surfel where the local geometry is computed.
   * @param lgeo_tbl (returns) an array of size at least i2 - i1 where the local geometry of the specified surfels are stored.
   *
   * @todo not done
   */
  void contourGeometry( uint i1, uint i2,
			LocalGeometry* lgeo_tbl ) const;
 

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

  /**
   * The minimizer is used to compute the geometry of the whole contour.
   */
  LinearMinimizer* m_minimizer;

  /**
   * 'true' when the minimizer has been given to the object, 'false'
   * when the object had to instantiate it.
   *
   * @see init
   */
  bool m_minimizer_owner;

  /**
   * Maximum number of common parts of the tangential cover.
   */
  uint m_max_nb_cp;

  /**
   * Number of common parts of the tangential cover.
   */
  uint m_nb_cp;

  /**
   * Number of surfels in the contour.
   */
  uint m_nb_surfels;

  /**
   * Number of maximal segments in the contour.
   */
  uint m_nb_ms;

  /**
   * Array containing all the common parts of the tangential cover.
   */
  C4CTangentialCover::CommonPart* m_cps;

  /**
   * Array containing all the geometry of the common parts of the
   * tangential cover.
   */
  CommonPartGeometry* m_cp_geoms;
  
  // ------------------------- Hidden services --------------------------------
protected:
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE GlobalC4CGeometry( const GlobalC4CGeometry & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE GlobalC4CGeometry & operator=( const GlobalC4CGeometry & other );
  
  // ------------------------- Internals --------------------------------------
private:

  /**
   * Called before any optimization is performed.
   */
  void prepareLinearMinimizer();

  /**
   * Called after an optimization to retrieve the result.
   */
  void afterLinearMinimizer();
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'GlobalC4CGeometry'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'GlobalC4CGeometry' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const GlobalC4CGeometry & that_object_to_display );
 
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/GlobalC4CGeometry.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined GlobalC4CGeometry_h

#undef GlobalC4CGeometry_RECURSES
#endif // else defined(GlobalC4CGeometry_RECURSES)
