/** @file GeometryComputerByContour4.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : GeometryComputerByContour4.h
//
// Creation : 2003/02/27
//
// Version : 2003/02/27
//
// Author : JOL
//
// Summary : Header file for files GeometryComputerByContour4.ih and GeometryComputerByContour4.cxx
//
// History :
//	2003/02/27 : ?Name? : ?What?
//
// Rcs Id : "@(#)class GeometryComputerByContour4 declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(GeometryComputerByContour4_RECURSES)
#error Recursive header files inclusion detected in GeometryComputerByContour4.h
#else // defined(GeometryComputerByContour4_RECURSES)
#define GeometryComputerByContour4_RECURSES

#if !defined GeometryComputerByContour4_h
#define GeometryComputerByContour4_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
//#include "LinAlg/LinAlg/Vector.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/mathutils/Functions.h"
#include "ImaGene/dgeometry2d/C4CSegment.h"
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/DigitalSurface.h"
#include "ImaGene/digitalnD/DigitalSurfaceGeometry.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class GeometryComputerByContour4
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'GeometryComputerByContour4' <p>
 * Aim: It represents a concrete strategy for several strategies to compute geometric measures on digital surfaces. 
 * @see DigitalSurfaceGeometry,TangentOnSurfaceComputer,NormalOnSurfaceComputer,AreaOnSurfaceComputer,MeanCurvatureOnSurfaceComputer,LengthOnSurfaceComputer,ContinuousAnalogComputer
 */
class GeometryComputerByContour4 
  : public TangentOnSurfaceComputer,
    public NormalOnSurfaceComputer,
    public AreaOnSurfaceComputer,
    public LengthOnSurfaceComputer,
    public MeanCurvatureOnSurfaceComputer,
    public ContinuousAnalogComputer
{
  // ------------------------- Public types ------------------------------
public:
  /**
   * Defines the possible discrete tangents.
   */
  enum DiscreteTangentType {
    /**
     * Longest discrete symmetric tangent.
     */
    SYMMETRIC_TANGENT,
    /**
     * Longest discrete deterministic tangent. Always includes the
     * 'SYMMETRIC_TANGENT' but may grow further on one side if there
     * is no ambiguity.
     */
    EXTENDED_TANGENT,
    /**
     * Common tangent. Always included in 'EXTENDED_TANGENT'. It is
     * the segment joining the closest pivots.
     */
    COMMON_TANGENT,
    /**
     * lambda-MST.
     */
    LAMBDA_MST
  };


  /**
   * Defines the possible discrete tangent planes definition.
   */
  enum DiscreteTangentPlanesType {
    /**
     * The upper and lower planes bound the n-1 discrete tangents.
     */
    BOUNDING_PLANES,
    /**
     * The two planes are identical and are estimated based on the
     * mean position of the surfel along each of the n-1 discrete
     * tangents.
     */
    ESTIMATED_PLANES,
    /**
     * The two planes are identical and are estimated based on the
     * mean position of the continuous analogs along each direction.
     */
    AVERAGE_CONTINUOUS_ANALOG,
  };


  /**
   * Defines the possible continuous analog definition.
   */
  enum ContinuousAnalogType {
    /**
     * The continuous analog is the projection of the centroid onto the tangent plane(s).
     */
    TANGENT_PLANE_PROJECTION,
    /**
     * The continuous analog is estimated by interpolation in the
     * pencil of maximal segments, then estimation of tangent plane
     * intersection.
     */
    PENCIL_INTERPOLATION,
    /**
     * The continuous analog is estimated by interpolation in the
     * pencil of maximal segments, then mean of extracted points.
     */
    PENCIL_INTERPOLATION2
  };
  


  // ------------------------- Public data ------------------------------
public:

  /**
   * Determines which definition of discrete tangent is used for estimating
   * tangents on digital surfaces. Used by 'computeTangent2D'.
   * Default value is 'SYMMETRIC_TANGENT'.
   * @see computeTangent2D,DiscreteTangentType::SYMMETRIC_TANGENT
   */
  DiscreteTangentType discrete_tangent_type;

  /**
   * Determines which definition of discrete tangent planes is used
   * for estimating the tangent plane on digital surfaces.
   * Default value is 'BOUNDING_PLANES'.
   * @see computeTangentPlane,DiscreteTangentPlanesType::BOUNDING_PLANES
   */
  DiscreteTangentPlanesType discrete_tangent_planes_type;
  
  /**
   * Determines which definition of continuous analog is used.
   * Default value is 'TANGENT_PLANE_PROJECTION'.
   * @see computeContinuousAnalog,ContinuousAnalogType::TANGENT_PLANE_PROJECTION
   */
  ContinuousAnalogType continuous_analog_type;
  

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. Does nothing.
   */
  virtual ~GeometryComputerByContour4();

  /**
   * Default constructor. 
   */
  GeometryComputerByContour4();

  // ------------------------- Implemented Geometry services ------------------
public:
  /**
   * Uses $n-1$ 2d tangent computations through [surf_geom_ctxt].
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
   * @see m_surface_geometry,SurfaceGeometry::tangent2D
   */
  virtual void computeNormal( DigitalSurfaceGeometry & surf_geom_ctxt,
			      Kn_sid b, Vector & vn );

  /**
   * A strong version of 'computeNormal' since it gives a tangent plane 
   * estimation. More precisely, it returns the upper and lower deviation to
   * the centroid of the surfel [b] along its orthogonal coordinate so that
   * all elements of the tangents are included in the plane. 
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
   * @param out_x_plane the deviation to the surfel centroid to get the lower leaning plane (outside the shape since normal points inside). 
   * @param in_x_plane the deviation to the surfel centroid to get the upper leaning plane (inside the shape since normal points inside). 
   * @see computeNormal
   */
  virtual void computeTangentPlane( DigitalSurfaceGeometry & surf_geom_ctxt, 
				    Kn_sid b, Vector & vn,
				    float & out_x_plane,
				    float & in_x_plane );

  /**
   * Uses [surf_geom_ctxt] to get 'C4CIterator's, then call 
   * C4CGeometry::tangentLine
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated 2d tangent vector at surfel [b] in the tracking direction [i].
   * @see DigitalSurface::newC4CIterator,C4CIterator,C4CGeometry::tangentLine
   */
  virtual Vector2i computeTangent2D( DigitalSurfaceGeometry & surf_geom_ctxt,
				      Kn_sid b, uint i );

  /**
   * Stronger version of 'computeTangent2D'. It provides also the offset 'mu'.
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated 2d tangent line at surfel [b] in the tracking direction [i].
   */
  virtual DLine computeTangentLine2D( DigitalSurfaceGeometry & surf_geom_ctxt, 
				      Kn_sid b, uint i );


  /**
   * Uses [surf_geom_ctxt] to get normals.
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @return the estimated area of the surfel [b].
   * @see m_surface_geometry,SurfaceGeometry::normal
   */
  virtual float computeArea( DigitalSurfaceGeometry & surf_geom_ctxt,
			     Kn_sid b );
  /**
   * Default: returns 1.0
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated length of the elementary displacement in the neighborhood of [b].
   */
  virtual float computeLength( DigitalSurfaceGeometry & surf_geom_ctxt,
			       Kn_sid b, uint i );
  /**
   * Uses [surf_geom_ctxt] to get 2d tangent.
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated distance to go from [b] to the next surfel in direction [i] (ie direct follower).
   * @see m_surface_geometry,SurfaceGeometry::tangent2D
   */
  virtual float computeLengthNext( DigitalSurfaceGeometry & surf_geom_ctxt,
				   Kn_sid b, uint i );
  /**
   * Uses [surf_geom_ctxt] to get 2d tangent.
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated distance to go from [b] to the previous surfel in direction [i] (ie indirect follower).
   * @see m_surface_geometry,SurfaceGeometry::tangent2D
   */
  virtual float computeLengthPrevious( DigitalSurfaceGeometry & surf_geom_ctxt,
				       Kn_sid b, uint i );
  /**
   * Uses [surf_geom_ctxt] to get normals and lengths.
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @return the estimated mean curvature at surfel [b].
   * @see m_surface_geometry,SurfaceGeometry::normal,SurfaceGeometry::lengthNext,SurfaceGeometry::lengthPrevious
   */
  virtual float computeMeanCurvature( DigitalSurfaceGeometry & surf_geom_ctxt,
				      Kn_sid b );


  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated 2d point in the Euclidean space associated to the surfel [b] using estimation along contour of direction [i].
   */
  virtual Vector2D computeContinuousAnalog( DigitalSurfaceGeometry & surf_geom_ctxt, 
					    Kn_sid b, uint i );

  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param x (returns) the estimated point in the Euclidean space associated to the surfel [b].
   */
  virtual void computeContinuousAnalog( DigitalSurfaceGeometry & surf_geom_ctxt, 
					Kn_sid b, Vector & x );


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
public:
  /**
   * If 0, indicates to extract the exact discrete tangent, otherwise defines
   * the maximum number of bels searched in one direction.
   */
  uint max_size;

private:

  /**
   * @TODO JOL: To do it nicely.
   * Array to store maximal segments.
   */
  C4CSegment m_segments[ 100 ];

  /**
   * Used by lambda-MST estimator.
   */
  TriangleFunction m_l;

  /**
   * Used by lambda-MST estimator.
   * Derivative of m_l.
   */
  DTriangleFunction m_lp;

  // ------------------------- Hidden services --------------------------------
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE GeometryComputerByContour4( const GeometryComputerByContour4 & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE GeometryComputerByContour4 & operator=( const GeometryComputerByContour4 & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'GeometryComputerByContour4'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'GeometryComputerByContour4' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const GeometryComputerByContour4 & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/GeometryComputerByContour4.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined GeometryComputerByContour4_h

#undef GeometryComputerByContour4_RECURSES
#endif // else defined(GeometryComputerByContour4_RECURSES)
