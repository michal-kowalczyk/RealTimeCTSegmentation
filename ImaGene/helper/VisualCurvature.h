/** @file VisualCurvature.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : VisualCurvature.h
//
// Creation : 2009/02/16
//
// Version : 2009/02/16
//
// Author : JOL
//
// Summary : Header file for files VisualCurvature.ih and VisualCurvature.cxx
//
// History :
//	2009/02/16 : ?Name? : ?What?
//
// Rcs Id : "@(#)class VisualCurvature declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(VisualCurvature_RECURSES)
#error Recursive header files inclusion detected in VisualCurvature.h
#else // defined(VisualCurvature_RECURSES)
#define VisualCurvature_RECURSES

#if !defined VisualCurvature_h
#define VisualCurvature_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/mathutils/SampledFunction.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class VisualCurvature
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'VisualCurvature' <p> Aim: This class
   * computes and stores all information related to the visual
   * curvature of a given polygon or polygonal line. See reference
   * [Liu2008].

   * [Liu2008] H. Liu, L. J. Latecki, W. Liu, A unified Curvature
   * Definition for Regular, Polygonal, and Digital Planar curves,
   * International Journal of Computer Vision, 80: 104-124, 2008.
   */
  class VisualCurvature
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~VisualCurvature();

    /**
     * Constructor. The object is not valid yet.
     * @see init
     */
    VisualCurvature();
    
    /** 
     * Initialization from polygon.
     *
     * @param n the number of directions used for computing the visual
     * curvature (the more the more accurate it is, but the more
     * costly it is).
     *
     * @param x the x-coordinates of polygon vertices.
     * @param y the y-coordinates of polygon vertices.
     *
     * @param closed when 'true' the polygon is closed, otherwise it
     * is an open polygonal line.
     */
    void init( uint n,
	       const std::vector<double> & x, 
	       const std::vector<double> & y, 
	       bool closed );

    // ----------------------- Simple services --------------------------------
  public:
    
    /**
     * @param k an integer between 0 (included) and 'm_fcts.size()'
     * (excluded).  
     * 
     * @return the corresponding direction.
     */
    INLINE double direction( uint k ) const;

    /**
     * Computes the visual curvature at position s with window ds.
     *
     * @param s the arclength of the position.
     * @param ds the window for the computation.
     * @return the visual curvature.
     */
    double visualCurvature( double s, double ds ) const;

    /**
     * Computes the multi-scale visual curvature at position s with
     * window ds.
     *
     * @param s the arclength of the position.
     * @param ds the window for the computation.
     * @param lambda the scale (between 0.0, finest, and 1.0, coarsest).
     * @return the multi-scale visual curvature.
     */
    double msVisualCurvature( double s, double ds, double lambda, bool normalized=true ) const;

    /**
     * Computes the algebraic visual curvature at position s with
     * window ds.  The difference with visual curvature is that
     * concave and convex portions cancel each other. For instance, a
     * digital straight line has much less algebraic visual curvature
     * than visual curvature.
     *
     * @param s the arclength of the position.
     * @param ds the window for the computation.
     * @return the visual curvature.
     * @see visualCurvature
     * @deprecated 
     */
    double algebraicVisualCurvature( double s, double ds ) const;

    // ----------------------- Interface --------------------------------------
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
  

    // ------------------------- Datas ----------------------------------------
  private:

    /** 
     * Represents the distance functions according to all the
     * specified directions.
     */
    std::vector<SampledFunction> m_fcts;

    // ------------------------- Hidden services ------------------------------
  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE VisualCurvature( const VisualCurvature & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE VisualCurvature & operator=( const VisualCurvature & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'VisualCurvature'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'VisualCurvature' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const VisualCurvature & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/VisualCurvature.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined VisualCurvature_h

#undef VisualCurvature_RECURSES
#endif // else defined(VisualCurvature_RECURSES)
