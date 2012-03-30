/** @file MeanCurvatureOnSurfaceComputerByDG.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : MeanCurvatureOnSurfaceComputerByDG.h
//
// Creation : 2003/02/28
//
// Version : 2003/02/28
//
// Author : JOL
//
// Summary : Header file for files MeanCurvatureOnSurfaceComputerByDG.ih and MeanCurvatureOnSurfaceComputerByDG.cxx
//
// History :
//	2003/02/28 : ?Name? : ?What?
//
// Rcs Id : "@(#)class MeanCurvatureOnSurfaceComputerByDG declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(MeanCurvatureOnSurfaceComputerByDG_RECURSES)
#error Recursive header files inclusion detected in MeanCurvatureOnSurfaceComputerByDG.h
#else // defined(MeanCurvatureOnSurfaceComputerByDG_RECURSES)
#define MeanCurvatureOnSurfaceComputerByDG_RECURSES

#if !defined MeanCurvatureOnSurfaceComputerByDG_h
#define MeanCurvatureOnSurfaceComputerByDG_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/DigitalSurfaceGeometry.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class MeanCurvatureOnSurfaceComputerByDG
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'MeanCurvatureOnSurfaceComputerByDG' <p>
 * Aim: It represents a concrete strategy for the geometric curvature strategy.
 * The method is based on the application of a derivative Gaussian filter.
 * @see DigitalSurfaceGeometry,TangentOnSurfaceComputer,NormalOnSurfaceComputer,AreaOnSurfaceComputer,MeanCurvatureOnSurfaceComputer,LengthOnSurfaceComputer
 */
class MeanCurvatureOnSurfaceComputerByDG : public MeanCurvatureOnSurfaceComputer
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~MeanCurvatureOnSurfaceComputerByDG();
  /**
   * Constructor.
   * @param sigma the standard deviation of the Gaussian derivative.
   * @param size the discrete corresponding has a size '2*size+1'.
   * @param delta the distance between two points of the filter.
   * @param epsilon the smallest value of the filter for which a computation is done.
   */
  MeanCurvatureOnSurfaceComputerByDG( float sigma = 1.0, 
				      uint size = 1000, 
				      float delta = 0.01,
				      float epsilon = 0.02 );

  /**
   * Changes the parameters of the Gaussian derivative.
   * @param sigma the standard deviation of the Gaussian derivative.
   * @param size the discrete corresponding has a size '2*size+1'.
   * @param delta the distance between two points of the filter.
   * @param epsilon the smallest value of the filter for which a computation is done.
   */
  void setParameters( float sigma, uint size, float delta, float epsilon );

  // ------------------------- Implemented Geometry services ------------------
public:

  /**
   * Uses [surf_geom_ctxt] to get normals and lengths.
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @return the estimated mean curvature at surfel [b].
   */
  virtual float computeMeanCurvature( DigitalSurfaceGeometry & surf_geom_ctxt, 
				      Kn_sid b );


  // --------------------- Gaussian derivative services -----------------------
public:
  /**
   * @param x any point.
   * @return the gaussian derivative at point [x] based on the current parameters.
   */
  INLINE float gaussianDerivative( float x ) const;

  /**
   * @return the size of the filter to consider for computation.
   * @see m_size_for_computation
   */
  INLINE uint sizeForFiltering() const;

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
   * Array of size '2*m_size+1' that stores the values of the Gaussian 
   * derivative.
   * @see m_size
   */
  float* m_filter;

  /**
   * The standard deviation of the Gaussian derivative.
   */
  float m_sigma;

  /**
   * The size of one-side of the filter 'm_filter'.
   * @see m_filter
   */
  uint m_size;
  
  /**
   * The delta between two points of the filter.
   */
  float m_delta;

  /**
   * The smallest value of the filter for which a computation is done.
   */
  float m_epsilon;
  
  /**
   * The size of the filter to consider for computation (based on 'm_epsilon')
   * Always below 'm_size'.
   * @see m_epsilon,m_size
   */
  uint m_size_for_computation;

  /**
   * The size of the filter to consider for computation (based on 'm_epsilon')
   * Equals to 'm_delta*m_size_for_computation'
   * @see m_epsilon,m_size
   */
  float m_size_length_for_computation;

  // ------------------------- Hidden services --------------------------------
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE MeanCurvatureOnSurfaceComputerByDG( const MeanCurvatureOnSurfaceComputerByDG & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE MeanCurvatureOnSurfaceComputerByDG & operator=( const MeanCurvatureOnSurfaceComputerByDG & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'MeanCurvatureOnSurfaceComputerByDG'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'MeanCurvatureOnSurfaceComputerByDG' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const MeanCurvatureOnSurfaceComputerByDG & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/MeanCurvatureOnSurfaceComputerByDG.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined MeanCurvatureOnSurfaceComputerByDG_h

#undef MeanCurvatureOnSurfaceComputerByDG_RECURSES
#endif // else defined(MeanCurvatureOnSurfaceComputerByDG_RECURSES)
