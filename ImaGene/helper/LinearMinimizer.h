/** @file LinearMinimizer.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : LinearMinimizer.h
//
// Creation : 2004/11/24
//
// Version : 2004/11/24
//
// Author : JOL
//
// Summary : Header file for files LinearMinimizer.ih and LinearMinimizer.cxx
//
// History :
//	2004/11/24 : ?Name? : ?What?
//
// Rcs Id : "@(#)class LinearMinimizer declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(LinearMinimizer_RECURSES)
#error Recursive header files inclusion detected in LinearMinimizer.h
#else // defined(LinearMinimizer_RECURSES)
#define LinearMinimizer_RECURSES

#if !defined LinearMinimizer_h
#define LinearMinimizer_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class LinearMinimizer
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'LinearMinimizer' <p> Aim: To find a set of
 * values which is as straight as possible while keeping values
 * between constraints. This set of values is associated to points on
 * a discrete contour, and is used to estimate the normal and
 * curvature of objects.
 */
class LinearMinimizer
{
public: 

  /**
   * Stores the information linked to the value to optimize.
   */
  struct ValueInfo
  {
    /**
     * The current value. Should be between [min] and [max].
     */
    double value;

    /**
     * The old value. Useful during optimization. 
     */
    double old_value; 

//     /**
//      * The absolute reference. It is the sum of the previous reference
//      * value and the previous delta.
//      */
//     double ref_value;

    /**
     * The lower bound for the value.
     */
    double min;

    /**
     * The upper bound for the value.
     */
    double max;

//     /**
//      * The variable [value] is relative. To compare [value] with the
//      * next one, [delta] should be added.
//      */
//     double delta;

    /**
     * This is the distance between this value and the next one.
     */
    double dist_to_next;

//     /**
//      * It is the sum of the distances to the first value, ie the
//      * curvilinear abscissa.
//      */
//     double curv_abscissa;

//     /**
//      * This point may also have a discrete abscissa (generally an
//      * integer). Useful to relate this structure with discrete
//      * contours.
//      */
//     double disc_abscissa; 

//     /**
//      * Initializes the structure with all necessary information.
//      *
//      * @param v the initial value. Should be between [min] and [max].
//      * @param ref_v the absolute reference value.
//      * @param min_v the lower bound for the value.
//      * @param max_v the upper bound for the value.
//      * @param d the variable [value] is relative. To compare [value] with the next one, [delta] should be added.
//      * @param dist the distance between this value and the next one.
//      * @param cabs the curvilinear abscissa.
//      * @param dabs the discrete abscissa.
//      */
//     INLINE void init( double v, double ref_v, double min_v, double max_v,
// 		      double d, double dist, double cabs, double dabs );
    
  };
  
  
  

  // ------------------------- Standard services ------------------------------
public:

  /**
   * Destructor. 
   */
  virtual ~LinearMinimizer();

  /**
   * Constructor. The object is not valid.
   */
  LinearMinimizer();

  /**
   * Reset all. The object is as if it has just been created.
   */
  void reset();
  
  /**
   * (Re-)initializes the linear minimizer.
   *
   * @param nb an upper bound for the number of values to optimize.
   */
  void init( uint nbmax );
  
  /**
   * @return a reference on the information structure of the [i]th value.
   */
  INLINE ValueInfo & rw( uint i );

  /**
   * @return a const reference on the information structure of the [i]th value.
   */
  INLINE const ValueInfo & ro( uint i ) const;
  
  /**
   * @return the maximum number of values stored in the object.
   */
  INLINE uint maxSize() const;

  /**
   * @return the number of values stored in the object.
   */
  INLINE uint size() const;

  /**
   * Specifies the exact number of valid values.
   * @param nb any number below 'maxSize()'.
   */
  INLINE void setSize( uint nb );

  /**
   * Specifies if the curve is open or not.
   * @param is_curve_open when 'true' the curve is open and the last
   * value does not depend on the first one, otherwise the curve is
   * closed and the last value is linked to the first one.
   */
  INLINE void setIsCurveOpen( bool is_curve_open = false );


  // ------------------------- Optimization services --------------------------
public:

  /**
   * @param i1 the first value to be optimized (between 0 and 'size()-1').
   * @param i2 the value after the last to be optimized (between 0 and 'size()-1') * @return the current energy of the system.
   */
  double getEnergy( uint i1, uint i2 ) const;
  
  /**
   * @param i1 the first value to be optimized (between 0 and 'size()-1').
   * @param i2 the value after the last to be optimized (between 0 and 'size()-1') * @return the former energy of the system.
   */
  double getFormerEnergy( uint i1, uint i2 ) const;

  /**
   * @return the energy gradient computed at the current position.
   */
  std::vector<double> getGradient() const;

  /**
   * @return the energy gradient computed at the former position.
   */
  std::vector<double> getFormerGradient() const;
  
  /**
   * Move each value on the straight line joining its neighboring
   * values while maintaining the value between its bounds.  The
   * optimization is performed on all values. After preparing datas,
   * calls oneStep, which may be overriden.
   *
   * @return the sum of the displacements.
   * @see oneStep
   */
  double optimize();
  
  /**
   * Move each value on the straight line joining its neighboring
   * values while maintaining the value between its bounds.  The
   * optimization is performed on values [i1] included to [i2]
   * excluded.  After preparing datas, calls oneStep, which may be
   * overriden. Then, computes max and sum of displacements.
   *
   * @param i1 the first value to be optimized (between 0 and 'size()-1').
   * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
   * @return the sum of the displacements.
   * @see oneStep
   */
  double optimize( uint i1, uint i2 );


  /**
   * Sum of all the absolute displacements of the last optimisation step.
   */
  INLINE double sum() const;

  /**
   * Max of all the absolute displacements of the last optimisation step.
   */
  INLINE double max() const;


protected:

  /**
   * The method which performs the optimization effectively. The user
   * may override it. The optimization is performed on values [i1]
   * included to [i2] excluded. Afterwards, the field ValueInfo::value
   * should contain the new value.
   *
   * @param i1 the first value to be optimized (between 0 and 'size()-1').
   * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
   */
  virtual void oneStep( uint i1, uint i2 );

public:
  /**
   * Should be used to stop the minimization process. The smaller is
   * this value, the more the optimization is at an end. May have
   * several meanings, like the infinite norm of the last displacement
   * or the infinite norm of the projected gradient.
   *
   * @return an upper bound on the norm of the last displacement. 
   */
  virtual double lastDelta() const; 


  // ------------------------- Interface --------------------------------------
public:
  /**
   * Writes/Displays the object on an output stream.
   * @param that_stream the output stream where the object is written.
   */
  virtual void selfDisplay( std::ostream & that_stream ) const;
  /**
   * Checks the validity/consistency of the object.
   * @return 'true' if the object is valid, 'false' otherwise.
   */
  bool OK() const;
  
  // ------------------------- Datas ------------------------------------------
private:

  /**
   * The dynamically allocated array of values and the associated constraints.
   */
  ValueInfo* m_values;
  
  /**
   * The size of the array [m_values], ie the maximal number of valid values.
   */
  uint m_max_size;

  /**
   * The meaningful size of the array [m_values], ie the number of
   * valid entries.
   */
  uint m_size;

protected:
  /**
   * Indicates if the curve is open.
   */
  bool m_is_curve_open;

private:
  /**
   * Sum of all the absolute displacements of the last optimisation step.
   */
  double m_sum;

  /**
   * Max of all the absolute displacements of the last optimisation step.
   */
  double m_max;

  // ------------------------- Hidden services --------------------------------
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE LinearMinimizer( const LinearMinimizer & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE LinearMinimizer & operator=( const LinearMinimizer & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

  /**
   * Specializes LinearMinimizer to optimize with a relaxation method.
   */
  class LinearMinimizerByRelaxation : public LinearMinimizer
  {
  public:
    /**
     * Default constructor. Does nothing.
     */
    INLINE LinearMinimizerByRelaxation();

    /**
     * Destructor. Does nothing.
     */
    INLINE virtual ~LinearMinimizerByRelaxation();

  protected:
    
    /**
     * The method which performs the optimization effectively. Use a
     * relaxation technique. The optimization is performed on values
     * [i1] included to [i2] excluded. 
     *
     * @param i1 the first value to be optimized (between 0 and 'size()-1').
     * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
     */
    virtual void oneStep( uint i1, uint i2 );

public:
    /**
     * Should be used to stop the minimization process. The smaller is
     * this value, the more the optimization is at an end. May have
     * several meanings, like the infinite norm of the last displacement
     * or the infinite norm of the projected gradient.
     *
     * @return an upper bound on the norm of the last displacement. 
     */
    virtual double lastDelta() const; 

    // ----------------------- Interface --------------------------------------
  public:
    /**
     * Writes/Displays the object on an output stream.
     * @param that_stream the output stream where the object is written.
     */
    virtual void selfDisplay( std::ostream & that_stream ) const;

  };
  


  /**
   * Specializes LinearMinimizer to optimize with a gradient descent method.
   */
  class LinearMinimizerByGradientDescent : public LinearMinimizer
  {
  private:

    /**
     * The step for the gradient descent.
     */
    double m_step;

  public:
    /**
     * Default constructor. Does nothing.
     */
    INLINE LinearMinimizerByGradientDescent( double step = 0.1 );

    /**
     * Destructor. Does nothing.
     */
    INLINE virtual ~LinearMinimizerByGradientDescent();

  protected:
    
    /**
     * The method which performs the optimization effectively. Use a
     * relaxation technique. The optimization is performed on values
     * [i1] included to [i2] excluded. 
     *
     * @param i1 the first value to be optimized (between 0 and 'size()-1').
     * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
     */
    virtual void oneStep( uint i1, uint i2 );

public:
    /**
     * Should be used to stop the minimization process. The smaller is
     * this value, the more the optimization is at an end. May have
     * several meanings, like the infinite norm of the last displacement
     * or the infinite norm of the projected gradient.
     *
     * @return an upper bound on the norm of the last displacement. 
     */
    virtual double lastDelta() const; 

    // ----------------------- Interface --------------------------------------
  public:
    /**
     * Writes/Displays the object on an output stream.
     * @param that_stream the output stream where the object is written.
     */
    virtual void selfDisplay( std::ostream & that_stream ) const;

  };


  /**
   * Specializes LinearMinimizer to optimize with an adaptive step
   * gradient descent method.
   */
  class LinearMinimizerByAdaptiveStepGradientDescent : public LinearMinimizer
  {
  private:

    /**
     * The current step for the gradient descent.
     */
    double m_step;

  public:
    /**
     * Default constructor. Does nothing.
     */
    INLINE LinearMinimizerByAdaptiveStepGradientDescent( double step = 0.1 );

    /**
     * Destructor. Does nothing.
     */
    INLINE virtual ~LinearMinimizerByAdaptiveStepGradientDescent();

  protected:
    
    /**
     * The method which performs the optimization effectively. Use a
     * relaxation technique. The optimization is performed on values
     * [i1] included to [i2] excluded. 
     *
     * @param i1 the first value to be optimized (between 0 and 'size()-1').
     * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
     */
    virtual void oneStep( uint i1, uint i2 );

public:
    /**
     * Should be used to stop the minimization process. The smaller is
     * this value, the more the optimization is at an end. May have
     * several meanings, like the infinite norm of the last displacement
     * or the infinite norm of the projected gradient.
     *
     * @return an upper bound on the norm of the last displacement. 
     */
    virtual double lastDelta() const; 

    // ----------------------- Interface --------------------------------------
  public:
    /**
     * Writes/Displays the object on an output stream.
     * @param that_stream the output stream where the object is written.
     */
    virtual void selfDisplay( std::ostream & that_stream ) const;

  };
  

/**
 * Overloads 'operator<<' for displaying objects of class 'LinearMinimizer'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'LinearMinimizer' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const LinearMinimizer & that_object_to_display );
 

} // namespace ImaGene



///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/LinearMinimizer.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined LinearMinimizer_h

#undef LinearMinimizer_RECURSES
#endif // else defined(LinearMinimizer_RECURSES)
