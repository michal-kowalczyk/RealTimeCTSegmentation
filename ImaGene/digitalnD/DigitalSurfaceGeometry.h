/** @file DigitalSurfaceGeometry.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DigitalSurfaceGeometry.h
//
// Creation : 2003/02/27
//
// Version : 2003/02/27
//
// Author : JOL
//
// Summary : Header file for files DigitalSurfaceGeometry.ih and DigitalSurfaceGeometry.cxx
//
// History :
//	2003/02/27 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DigitalSurfaceGeometry declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DigitalSurfaceGeometry_RECURSES)
#error Recursive header files inclusion detected in DigitalSurfaceGeometry.h
#else // defined(DigitalSurfaceGeometry_RECURSES)
#define DigitalSurfaceGeometry_RECURSES

#if !defined DigitalSurfaceGeometry_h
#define DigitalSurfaceGeometry_h

//////////////////////////////////////////////////////////////////////////////
//#include "LinAlg/LinAlg/Vector.hpp"
//#include "LinAlg/LinAlg2D/Vector2D.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/dgeometry2d/DLine.h"
#include "ImaGene/digitalnD/DigitalSurface.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene { 

  class DigitalSurfaceGeometry;
  
///////////////////////////////////////////////////////////////////////////////
// class TangentOnSurfaceComputer
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'TangentOnSurfaceComputer' <p>
 * Aim: A "strategy" interface to represent a 2d discrete tangent estimator over a
 * digital surface.
 */
class TangentOnSurfaceComputer
{
  // ------------------------- Geometry services ------------------------------
public:

  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated 2d tangent vector at surfel [b] in the tracking direction [i].
   */
  virtual Vector2i computeTangent2D( DigitalSurfaceGeometry & surf_geom_ctxt, 
				      Kn_sid b, uint i ) = 0;

  /**
   * Stronger version of 'computeTangent2D'. It provides also the offset 'mu'.
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated 2d tangent line at surfel [b] in the tracking direction [i].
   */
  virtual DLine computeTangentLine2D( DigitalSurfaceGeometry & surf_geom_ctxt, 
				      Kn_sid b, uint i ) = 0;

};


///////////////////////////////////////////////////////////////////////////////
// class NormalOnSurfaceComputer
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'NormalOnSurfaceComputer' <p>
 * Aim: A "strategy" interface to represent a normal vector estimator on a
 * digital surface.
 */
class NormalOnSurfaceComputer
{
  // ------------------------- Geometry services ------------------------------
public:

  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
   */
  virtual void computeNormal( DigitalSurfaceGeometry & surf_geom_ctxt, 
			      Kn_sid b, Vector & vn ) = 0;

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

};


///////////////////////////////////////////////////////////////////////////////
// class AreaOnSurfaceComputer
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'AreaOnSurfaceComputer' <p>
 * Aim: A "strategy" interface to represent an area estimator on a
 * digital surface.
 */
class AreaOnSurfaceComputer
{
  // ------------------------- Geometry services ------------------------------
public:
  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @return the estimated area of the surfel [b].
   */
  virtual float computeArea( DigitalSurfaceGeometry & surf_geom_ctxt, 
			     Kn_sid b ) = 0;

};


///////////////////////////////////////////////////////////////////////////////
// class MeanCurvatureOnSurfaceComputer
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'MeanCurvatureOnSurfaceComputer' <p>
 * Aim: A "strategy" interface to represent a mean curvature estimator on a
 * digital surface.
 */
class MeanCurvatureOnSurfaceComputer
{
  // ------------------------- Geometry services ------------------------------
public:
  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @return the estimated mean curvature at surfel [b].
   */
  virtual float computeMeanCurvature( DigitalSurfaceGeometry & surf_geom_ctxt, 
				      Kn_sid b ) = 0;

};


///////////////////////////////////////////////////////////////////////////////
// class LengthOnSurfaceComputer
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'LengthOnSurfaceComputer' <p>
 * Aim: A "strategy" interface to represent a length estimator on a
 * digital surface.
 */
class LengthOnSurfaceComputer
{
  // ------------------------- Geometry services ------------------------------
public:
  /**
   * Default: returns 1.0
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated length of the elementary displacement in the neighborhood of [b].
   */
  virtual float computeLength( DigitalSurfaceGeometry & surf_geom_ctxt, 
			       Kn_sid b, uint i ) = 0;
  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated distance to go from [b] to the next surfel in direction [i] (ie direct follower).
   */
  virtual float computeLengthNext( DigitalSurfaceGeometry & surf_geom_ctxt, 
				   Kn_sid b, uint i ) = 0;
  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated distance to go from [b] to the previous surfel in direction [i] (ie indirect follower).
   */
  virtual float computeLengthPrevious( DigitalSurfaceGeometry & surf_geom_ctxt, 
				       Kn_sid b, uint i ) = 0;

};


///////////////////////////////////////////////////////////////////////////////
// class ContinuousAnalogComputer
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'ContinuousAnalogComputer' <p> Aim: A
 * "strategy" interface to represent a 2d discrete continuous analog
 * estimator over a digital surface.
 */
class ContinuousAnalogComputer
{
  // ------------------------- Geometry services ------------------------------
public:

  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated 2d point in the Euclidean space associated to the surfel [b] using estimation along contour of direction [i].
   */
  virtual Vector2D computeContinuousAnalog( DigitalSurfaceGeometry & surf_geom_ctxt, 
					    Kn_sid b, uint i ) = 0;

  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param x (returns) the estimated point in the Euclidean space associated to the surfel [b].
   */
  virtual void computeContinuousAnalog( DigitalSurfaceGeometry & surf_geom_ctxt, 
					Kn_sid b, Vector & x ) = 0;

};



///////////////////////////////////////////////////////////////////////////////
// class DigitalSurfaceGeometry
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'DigitalSurfaceGeometry' <p>
 * Aim: A "context" class with default behavior to represent the geometry of a
 * digital surface. This context class references several strategies to
 * authorize several implementations.
 * @see TangentOnSurfaceComputer,NormalOnSurfaceComputer,AreaOnSurfaceComputer,MeanCurvatureOnSurfaceComputer,LengthOnSurfaceComputer,ContinuousAnalogComputer
 */
class DigitalSurfaceGeometry
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. Does nothing.
   */
  INLINE ~DigitalSurfaceGeometry();

  /**
   * Constructor. Does nothing.
   */
  INLINE DigitalSurfaceGeometry();

  /**
   * Sets the associated digital surface.
   * @param dig_surf the digital surface to associate to this geometry.
   */
  INLINE void setSurface( DigitalSurface* dig_surf );

  /**
   * @return the associated digital surface.
   */
  INLINE DigitalSurface* surface() const;

  /**
   * @return the associated digital space in which the surface is embedded.
   */
  INLINE const KnSpace* space() const;
  

  // ------------------------- Geometry services ------------------------------
public:
  /**
   * Default: does nothing.
   * @param b a signed surfel.
   * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
   */
  INLINE void normal( Kn_sid b, Vector & vn );

  /**
   * A strong version of 'normal' since it gives a tangent plane 
   * estimation. More precisely, it returns the upper and lower deviation to
   * the centroid of the surfel [b] along its orthogonal coordinate so that
   * all elements of the tangents are included in the plane. 
   * @param b a signed surfel.
   * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
   * @param min_x_orth the deviation to the surfel centroid to get the lower leaning plane. 
   * @param max_x_orth the deviation to the surfel centroid to get the upper leaning plane. 
   * @see computeNormal
   */
  INLINE void tangentPlane( Kn_sid b, Vector & vn,
			    float & min_x_orth,
			    float & max_x_orth );

  /**
   * Default: returns a (1,0) vector.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated 2d tangent vector at surfel [b] in the tracking direction [i].
   */
  INLINE Vector2i tangent2D( Kn_sid b, uint i );
  /**
   * Default: returns a (1,0,0) discrete line.
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated 2d tangent line at surfel [b] in the tracking direction [i].
   */
  INLINE DLine tangentLine2D( Kn_sid b, uint i );
  /**
   * Default: returns 1.0
   * @param b a signed surfel.
   * @return the estimated area of the surfel [b].
   */
  INLINE float area( Kn_sid b );
  /**
   * Default: returns 1.0
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated length of the elementary displacement in the neighborhood of [b].
   */
  INLINE float length( Kn_sid b, uint i );
  /**
   * Default: returns 1.0
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated distance to go from [b] to the next surfel in direction [i] (ie direct follower).
   */
  INLINE float lengthNext( Kn_sid b, uint i );
  /**
   * Default: returns 1.0
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated distance to go from [b] to the previous surfel in direction [i] (ie indirect follower).
   */
  INLINE float lengthPrevious( Kn_sid b, uint i );
  /**
   * Default: returns 0.0
   * @param b a signed surfel.
   * @return the estimated mean curvature at surfel [b].
   */
  INLINE float meanCurvature( Kn_sid b );


  /**
   * Default (0.5, 0.0).
   * @param b a signed surfel.
   * @param i any tracking direction from [b].
   * @return the estimated 2d point in the Euclidean space associated to the surfel [b] using estimation along contour of direction [i].
   */
  INLINE Vector2D continuousAnalog( Kn_sid b, uint i );

  /**
   * Default centroid of [b].
   * @param b a signed surfel.
   * @param x (returns) the estimated point in the Euclidean space associated to the surfel [b].
   */
  INLINE void continuousAnalog( Kn_sid b, Vector & x );



  // ----------------------------- data -------------------------------------
private:
  /**
   * Associated digital surface.
   */
  DigitalSurface* m_digital_surface;
  
public:
  /**
   * Strategy to compute tangent estimation.
   */
  TangentOnSurfaceComputer* tangent_computer;
  /**
   * Strategy to compute normal estimation.
   */
  NormalOnSurfaceComputer* normal_computer;
  /**
   * Strategy to compute area estimation.
   */
  AreaOnSurfaceComputer* area_computer;
  /**
   * Strategy to compute mean curvature estimation.
   */
  MeanCurvatureOnSurfaceComputer* mean_curvature_computer;
  /**
   * Strategy to compute length estimation.
   */
  LengthOnSurfaceComputer* length_computer;
  /**
   * Strategy to compute continuous analog estimation.
   */
  ContinuousAnalogComputer* continuous_analog_computer;

};


///////////////////////////////////////////////////////////////////////////////
// class TrivialNormalOnSurfaceComputer
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'NormalOnSurfaceComputer' <p>
 * Aim: A "strategy" interface to represent a normal vector estimator on a
 * digital surface.
 */
class TrivialNormalOnSurfaceComputer : public NormalOnSurfaceComputer
{
  // ------------------------- Standard services ------------------------------
public:

  /**
   * Destructor. Does nothing.
   */
  INLINE virtual ~TrivialNormalOnSurfaceComputer();

  /**
   * Constructor. Does nothing.
   */
  INLINE TrivialNormalOnSurfaceComputer();


  // ------------------------- Geometry services ------------------------------
public:
  
  /**
   * Returns the trivial normal associated to the surfel [b] and
   * pointing inside.
   *
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
   */
  virtual void computeNormal( DigitalSurfaceGeometry & surf_geom_ctxt, 
			      Kn_sid b, Vector & vn );

};



} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/DigitalSurfaceGeometry.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DigitalSurfaceGeometry_h

#undef DigitalSurfaceGeometry_RECURSES
#endif // else defined(DigitalSurfaceGeometry_RECURSES)
