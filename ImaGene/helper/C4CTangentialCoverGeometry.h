/** @file C4CTangentialCoverGeometry.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CTangentialCoverGeometry.h
//
// Creation : 2004/11/29
//
// Version : 2004/11/29
//
// Author : JOL
//
// Summary : Header file for files C4CTangentialCoverGeometry.ih and C4CTangentialCoverGeometry.cxx
//
// History :
//	2004/11/29 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CTangentialCoverGeometry declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CTangentialCoverGeometry_RECURSES)
#error Recursive header files inclusion detected in C4CTangentialCoverGeometry.h
#else // defined(C4CTangentialCoverGeometry_RECURSES)
#define C4CTangentialCoverGeometry_RECURSES

#if !defined C4CTangentialCoverGeometry_h
#define C4CTangentialCoverGeometry_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/mathutils/Functions.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/Frame2D.h"
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/dgeometry2d/C4CELength.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {


///////////////////////////////////////////////////////////////////////////////
// class C4CTangentialCoverGeometry
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'C4CTangentialCoverGeometry' <p> Aim: Defines
 * a geometry on a digital contour represented by its tangential
 * cover.
 *
 * @see C4CTangentialCover
 */
  class C4CTangentialCoverGeometry
    {

      // ------------------------- static services ----------------------------
    public:

      /**
       * Given a digital curve specified by its 'C4CTangentialCover'
       * [tcover] and its 'C4CTangentialCoverGeometry' [tcover_geometry],
       * and given an interpolation function [lambda], computes the
       * lambda-MS tangent estimation of a surfel specified by its
       * 'SurfelMaximalSegments' [sms].
       *
       * @param tcover the tangential cover of the digital curve.
       * @param tcover_geometry the geometry of the tangential cover [tcover].
       * @param lambda an interpolation function.
       * @param sms the local maximal segment structure around the surfel of interest.
       * @return the angle of the curve wrt x-axis.
       */
      static double angleByLambdaMS
	( const C4CTangentialCover & tcover,
	  const C4CTangentialCoverGeometry & tcover_geometry,
	  const R2RFunction & lambda,
	  const C4CTangentialCover::SurfelMaximalSegments & sms );
  


      /**
       * Given a digital curve specified by its 'C4CTangentialCover'
       * [tcover] and its 'C4CTangentialCoverGeometry' [tcover_geometry],
       * and given an interpolation function [lambda], computes the length
       * of a subset of the digital curve. This subset is located on a
       * common part of the tangential cover, specified by a
       * 'SurfelMaximalSegments' [sms] and two surfel indices [s1] and
       * [s2]. The length is estimated by integrated the averaged length
       * of the lambda-MS tangent direction.
       *
       * @param tcover the tangential cover of the digital curve.
       * @param tcover_geometry the geometry of the tangential cover [tcover].
       * @param lambda an interpolation function.
       * @param sms the local maximal segment structure around the surfels of interest.
       * @param s1 the first surfel index.
       * @param s2 the surfel index after the last.
       * @return the length of the digital curve from [s1] to [s2] (excluded).
       */
      static double lengthByLambdaMS
	( const C4CTangentialCover & tcover,
	  const C4CTangentialCoverGeometry & tcover_geometry,
	  const R2RFunction & lambda,
	  const C4CTangentialCover::SurfelMaximalSegments & sms,
	  uint s1, uint s2 );


      /**
       * Given a digital curve specified by its 'C4CTangentialCoverGeometry'
       * [tcover_geometry], computes the length of a subset of the digital
       * curve. This subset is located on a common part of the tangential
       * cover, specified by a 'SurfelMaximalSegments' [sms] and two surfel
       * indices [s1] and [s2]. The length is estimated by averaging the
       * length of the portion of the maximal segments visiting these
       * surfels.
       *
       * @param tcover the tangential cover of the digital curve.
       * @param tcover_geometry the geometry of the tangential cover [tcover].
       * @param sms the local maximal segment structure around the surfels of interest.
       * @param s1 the first surfel index.
       * @param s2 the surfel index after the last.
       * @return the length of the digital curve from [s1] to [s2] (excluded).
       */
      static double lengthByAverage
	( const C4CTangentialCover & tcover,
	  const C4CTangentialCoverGeometry & tcover_geometry,
	  const C4CTangentialCover::SurfelMaximalSegments & sms,
	  uint s1, uint s2 );


      /**
       * Given a digital curve specified by its 'C4CTangentialCover'
       * [tcover] and its 'C4CTangentialCoverGeometry' [tcover_geometry],
       * and given an interpolation function [lambda], computes the
       * elementary lengthes of all surfels of the contour.
       *
       * @param tcover the tangential cover of the digital curve.
       * @param tcover_geometry the geometry of the tangential cover [tcover].
       * @param lambda an interpolation function.
       *
       * @return the elementary lengthes.
       */
      static C4CELength* computeELengthByLambdaMS
	( const C4CTangentialCover & tcover,
	  const C4CTangentialCoverGeometry & tcover_geometry,
	  const R2RFunction & lambda );


      /**
       * Display the geometry of a tangential cover as a function of the
       * maximal segement.
       *
       * @param out any output stream.
       * @param tcover the tangential cover of the digital curve.
       * @param tcover_geometry the geometry of the tangential cover [tcover].
       */
      static void displayGeometry
	( std::ostream & out, 
	  const C4CTangentialCover & tcover,
	  const C4CTangentialCoverGeometry & tcover_geometry );
  


      // ------------------------- public structures --------------------------
    public:

      /**
       * Represents the geometry of a surfel in a contour.
       */
      struct SurfelGeometry
      {
	/**
	 * The vector aligned with the surfel.
	 */
	Vector2i track;

	/**
	 * L'angle entre le surfel et l'axe des x.
	 */
	double track_angle_to_x;

	/**
	 * The local reference frame for this surfel.
	 */
	Frame2D frame;
      };

      /**
       * Represents the geometry of a maximal segment in a contour.
       */
      struct MaximalSegmentGeometry
      {

	/**
	 * The (most probable) angle between the maximal segment and the x-axis (between [0:2pi[).
	 */
	double angle_to_x;

	/**
	 * The minimum angle between the maximal segment and the x-axis (between [0:2pi[).
	 */
	double min_angle;

	/**
	 * The maximum angle between the maximal segment and the x-axis (between [0:2pi[).
	 */
	double max_angle;
    
      };

      /** 
       * A abstract action class used for computing the maximal segment
       * geometry. Computation are made in the local frame of the
       * maximal segment.
       */
      class MSGeometryComputer
	{
	public:
	  /** 
	   * Destructor. 
	   */
	  INLINE MSGeometryComputer();

	  /** 
	   * Destructor. 
	   */
	  INLINE virtual ~MSGeometryComputer();

	  /** 
	   * @return the (most probable) angle between the maximal segment and
	   * its local x-axis.
	   */
	  virtual double angle( const C4CSegment & ms ) = 0;

	  /** 
	   * @return the minimum angle between the maximal segment and
	   * its local x-axis.
	   */
	  virtual double minAngle( const C4CSegment & ms ) = 0;

	  /** 
	   * @return the maximum angle between the maximal segment and
	   * its local x-axis.
	   */
	  virtual double maxAngle( const C4CSegment & ms ) = 0;

	};  


      /** 
       * A concrete action class used for computing the maximal
       * segment geometry. Computation are made in the local frame of
       * the maximal segment. Uses leaning points to compute min and
       * max angle.
       *
       * @see MSGeometryComputerByDual
       */
      class MSGeometryComputerByLP : public MSGeometryComputer
	{
	public:
	  /** 
	   * Destructor. 
	   */
	  INLINE MSGeometryComputerByLP() {}

	  /** 
	   * Destructor. 
	   */
	  INLINE virtual ~MSGeometryComputerByLP() {}

	  /** 
	   * @return the (most probable) angle between the maximal segment and
	   * its local x-axis.
	   */
	  virtual double angle( const C4CSegment & ms );

	  /** 
	   * @return the minimum angle between the maximal segment and
	   * its local x-axis.
	   */
	  virtual double minAngle( const C4CSegment & ms );

	  /** 
	   * @return the maximum angle between the maximal segment and
	   * its local x-axis.
	   */
	  virtual double maxAngle( const C4CSegment & ms );

	};


      /** 
       * A concrete action class used for computing the maximal
       * segment geometry. Computation are made in the local frame of
       * the maximal segment. Uses dual space to compute min and
       * max angle.
       *
       * NB: more restrictive than 'MSGeometryComputerByLP'.
       *
       * @see MSGeometryComputerByLP
       */
      class MSGeometryComputerByDual : public MSGeometryComputer
	{
	public:
	  /** 
	   * Destructor. 
	   */
	  INLINE MSGeometryComputerByDual() {}

	  /** 
	   * Destructor. 
	   */
	  INLINE virtual ~MSGeometryComputerByDual() {}

	  /** 
	   * @return the (most probable) angle between the maximal segment and
	   * its local x-axis.
	   */
	  virtual double angle( const C4CSegment & ms );

	  /** 
	   * @return the minimum angle between the maximal segment and
	   * its local x-axis.
	   */
	  virtual double minAngle( const C4CSegment & ms );

	  /** 
	   * @return the maximum angle between the maximal segment and
	   * its local x-axis.
	   */
	  virtual double maxAngle( const C4CSegment & ms );

	};


      // ------------------------- Standard services --------------------------
    public:
      /**
       * Destructor. 
       */
      ~C4CTangentialCoverGeometry();

      /**
       * Constructor. Empty object.
       */
      C4CTangentialCoverGeometry();

      /**
       * Resets the object. As if it was just created.
       */
      void reset();
  
      /**
       * (Re-) Initializes the object so that it can compute the
       * geometry of the contour given in parameter. The contour is
       * represented by its tangential cover [tcover] and needs also
       * an iterator [it] to make the correct change of frames.
       *
       * @param ks the digital space.
       * @param x_axis the first axis of the plane where the geometry is computed.
       * @param y_axis the second axis of the plane where the geometry is computed.
       * @param tcover the tangential cover of a digital curve.
       * @param it an iterator on the digital curve, pointing on the surfel of index 0 in the tangential cover.
       * @deprecated Do not compute SurfelGeometry. Use other init instead.
       */
      void init( KnSpace* ks, 
		 uint x_axis, uint y_axis,
		 const C4CTangentialCover & tcover,
		 const C4CIteratorOnSurface & it );


      /**
       * (Re-) Initializes the object so that it can compute the
       * geometry of the contour given in parameter. The contour is
       * represented by its tangential cover [tcover] and needs also
       * an iterator [it] to make the correct change of frames.
       *
       * @param tcover the tangential cover of a digital curve.
       *
       * @param it an iterator on the digital curve, pointing on the
       * surfel of index 0 in the tangential cover.
       *
       * @param msgeom an action object to compute maximal segment geometry.
       * 
       * @param frame the reference frame centered on the first surfel
       * ('*it'), may be midified.
       */
      void init( const C4CTangentialCover & tcover,
		 const C4CIteratorOnSurface & it,
		 MSGeometryComputer & msgeom,
		 Frame2D & frame );

      /**
       * @return the number of maximal segments of the tangential cover.
       */
      INLINE uint nbMaximalSegments() const;

      /**
       * @return the number of surfels of the tangential cover.
       */
      INLINE uint nbSurfels() const;
      
      /**
       * @param i the index of the maximal segment.
       * @return the maximal segment of index [i].
       */
      INLINE const MaximalSegmentGeometry & geometry( uint i ) const;

      /**
       * @param i the index of the surfel.
       * @return the geometry of the surfel [i].
       */
      INLINE const SurfelGeometry & sgeometry( uint idx ) const;


  
      // ------------------------- Interface ----------------------------------
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
  
      // ------------------------- Datas --------------------------------------
    private:

      /**
       * Number of maximal segments.
       */
      uint m_nb_maximal_segments;
  
      /**
       * Geometry of the maximal segments
       */
      MaximalSegmentGeometry* m_ms_geometry;

      /**
       * Number of surfels.
       */
      uint m_nb_surfels;
  
      /**
       * Geometry of the maximal segments
       */
      SurfelGeometry* m_surfel_geometry;


      // ------------------------- Hidden services ----------------------------
    protected:
    private:
      /**
       * Copy constructor.
       * @param other the object to clone.
       * Forbidden by default.
       */
      INLINE C4CTangentialCoverGeometry( const C4CTangentialCoverGeometry & other );
      /**
       * Assignment.
       * @param other the object to copy.
       * @return a reference on 'this'.
       * Forbidden by default.
       */
      INLINE C4CTangentialCoverGeometry & operator=( const C4CTangentialCoverGeometry & other );
  
      // ------------------------- Internals ----------------------------------
    private:
  
    };

  /**
   * Overloads 'operator<<' for displaying objects of class
   * 'C4CTangentialCoverGeometry'.
   *
   * @param that_stream the output stream where the object is written.
   *
   * @param that_object_to_display the object of class
   * 'C4CTangentialCoverGeometry' to write.
   *
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
    operator<<( std::ostream & that_stream, 
		const C4CTangentialCoverGeometry & that_object_to_display );


}  // namespace ImaGene



///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/C4CTangentialCoverGeometry.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CTangentialCoverGeometry_h

#undef C4CTangentialCoverGeometry_RECURSES
#endif // else defined(C4CTangentialCoverGeometry_RECURSES)
