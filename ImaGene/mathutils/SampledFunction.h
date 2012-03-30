/** @file SampledFunction.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : SampledFunction.h
//
// Creation : 2009/02/13
//
// Version : 2009/02/13
//
// Author : JOL
//
// Summary : Header file for files SampledFunction.ih and SampledFunction.cxx
//
// History :
//	2009/02/13 : ?Name? : ?What?
//
// Rcs Id : "@(#)class SampledFunction declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(SampledFunction_RECURSES)
#error Recursive header files inclusion detected in SampledFunction.h
#else // defined(SampledFunction_RECURSES)
#define SampledFunction_RECURSES

#if !defined SampledFunction_h
#define SampledFunction_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/mathutils/Functions.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class SampledFunction
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'SampledFunction' <p> Aim: Represents a
   * function that is known at given points. Generally, a continuous
   * version of this function is obtained by connecting by straight
   * segments these samples. This function implements R2R function,
   * and allows periodicity.
   */
  class SampledFunction : public R2RFunction
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    virtual ~SampledFunction();
    
    /**
     * Creates a non-valid sampled function.
     * @see init
     */
    SampledFunction();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    INLINE SampledFunction( const SampledFunction & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    INLINE SampledFunction & operator=( const SampledFunction & other );

    /**
     * Create a sampled function from a vector of positions, a vector
     * of values, and periodicity information.
     *
     * @param x the sample positions (sorted).
     * @param fx the values at each sample position.
     *
     * @param periodic when 'true', the function is considered
     * periodic, with periodicity [periodicity].
     *
     * @param periodicity should be greater than the last x value.
     */
    void init( const std::vector<double> & x, 
	       const std::vector<double> & fx,
	       bool periodic = false, 
	       double periodicity = 0.0 );

    /**
     * Evaluates the function at [x].
     * complexity is O(n).
     *
     * @param x any value in the range.
     * @return the value of the function at [x].
     */
    virtual float value( float x ) const;

    /**
     * Evaluates the function at [x].
     * Complexity is O(log(n)).
     *
     * @param x any value in the range.
     * @return the value of the function at [x].
     */
    virtual double value( double x ) const;

    /**
     * Finds the index i of the sample such that x[i] <= x < x[i+1],
     * whether it is periodic or not.
     * Complexity is O(log(n)).
     *
     * @param x a real number describing a position.
     *
     * @param lambda (returns) a coefficient between 0.0 and 1.0
     * (excluded) which tells where x is between x[i] and x[i+1].
     *
     * @return the index in the array of sample positions x.
     */
    uint find( double x, double & lambda ) const;

    /**
     * @param i a sample index 
     *
     * @return 1 if it is maxima, -1 if it is a minima, 2 if it is a
     * starting or ending point and the funtion is not periodic, 0 in
     * the other cases.
     */
    int type( uint i ) const;

    /**
     * Given an interval, returns the number of extreme points within.
     * Complexity is O(log(n)).
     *
     * @param x1 the first abscissa.
     * @param x2 the last abscissa.
     *
     * @return the number of extreme points on the sampled function
     * between positions [x1,x2].
     */
    uint numberExtremePoints( double x1, double x2 ) const;

    /**
     * Given an interval, returns the number of extreme points within,
     * such that their scale measure is no smaller than lambda.
     * Complexity is O(log(n)).
     *
     * @param x1 the first abscissa.
     * @param x2 the last abscissa.
     *
     * @param lambda the minimal scale measure to take into account
     * the extreme points
     *
     * @param normalized: set to false for skipping normalization.
     *
     * @return the number of extreme points on the sampled function
     * between positions [x1,x2].
     */
    uint numberSignificantExtremePoints( double x1, 
					 double x2, 
					 double lambda, bool normalized=true ) const;

    /**
     * Given an interval, returns the algebraic number of extreme
     * points within (meaning concave and convex portions cancel each
     * other).  Complexity is O(log(n)).
     *
     * @param x1 the first abscissa.
     * @param x2 the last abscissa.
     *
     * @return the algebraic number of extreme points on the sampled function
     * between positions [x1,x2].
     */
    int algebraicNumberExtremePoints( double x1, double x2 ) const;

    /**
     * @return the height of the function (i.e. max-min)
     */
    double height() const;

    /**
     * Given a starting point given by an index, return the height of the
     * function to the left of the starting point, i.e. the lowest (or
     * highest) deviation of the function wrt to the starting value up to
     * the moment where the function meets again the starting value.
     *
     * @param idx the starting position
     * @return the depth (or H function) to the left of position idx. 
     */
    double heightNegativePeak( uint idx ) const;

    /**
     * Given a starting point given by an index, return the depth of the
     * function to the right of the starting point, i.e. the lowest (or
     * highest) deviation of the function wrt to the starting value up to
     * the moment where the function meets again the starting value.
     *
     * @param idx the starting position
     * @return the depth (or H function) to the right of position idx. 
     */
    double heightPositivePeak( uint idx ) const;

    /**
     * Given an extreme point specified by its index [idx], return its
     * scale measure (@see VisualCurvature). It is defined as the
     * minimum of the heights of the positive and negative peaks
     * around the point of interest, divided by the height of the
     * function.
     *
     * @param the given extreme point index.
     * @param normalized: set to false for skipping normalization.
     *
     * @return its scale measure between 0.0 and 1.0 (the larger, the
     * more significant is the extreme point).
     */
    double scaleMeasure( uint idx, bool normalized=true ) const;

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
  public:
    /**
     * sample positions.
     */
    std::vector<double> x;
    /**
     * sample values.
     */
    std::vector<double> fx;
    /**
     * Periodicity (meaningful only if > 0.0).
     */
    double periodicity;

    /**
     * Height of the function or -1 if it has not been already computed.
     */
    mutable double m_height;

    // ------------------------- Hidden services ------------------------------
  private:

  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'SampledFunction'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'SampledFunction' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const SampledFunction & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/SampledFunction.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined SampledFunction_h

#undef SampledFunction_RECURSES
#endif // else defined(SampledFunction_RECURSES)
