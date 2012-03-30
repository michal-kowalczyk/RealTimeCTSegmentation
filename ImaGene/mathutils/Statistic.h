/** @file Statistic.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Statistic.h
//
// Creation : 2010/05/24
//
// Version : 2010/05/24
//
// Author : JOL
//
// Summary : Header file for files Statistic.ih and Statistic.cxx
//
// History :
//	2010/05/24 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Statistic declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Statistic_RECURSES)
#error Recursive header files inclusion detected in Statistic.h
#else // defined(Statistic_RECURSES)
#define Statistic_RECURSES

#if !defined Statistic_h
#define Statistic_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class Statistic
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Statistic' <p> Aim: This class processes a
   * set of sample values for one variable and can then compute
   * different statistics, like sample mean, sample variance, sample
   * unbiased variance, etc. It is minimalistic for space
   * efficiency. For multiple variables, sample storage and others,
   * see Statistics class.
   *
   * @see Statistics
   */
  template <typename RealNumberType>
  class Statistic
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~Statistic();

    /**
     * Constructor.
     */
    Statistic(bool storeSample=false);

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    Statistic( const Statistic & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    Statistic & operator=( const Statistic & other );

    /**
     * Adds to self another set of statistics (should be of the same
     * variable).
     *
     * @param other the object to add.
     * @return a reference on 'this'.
     */
    Statistic & operator+=( const Statistic & other );

    /**
     * Adds two set of statistics (should be of the same variable).
     *
     * @param other the object to add.
     * @return a new object that is the union of these statistics.
     */
    Statistic operator+( const Statistic & other ) const;
   
 
    // ----------------------- Accessors ------------------------------
  public:


    /**
     * Used by boost to Serialize. 
     *
     **/

    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version){
      this->terminate();
      ar & m_samples & m_exp & m_exp2 & m_max & m_min & m_median &m_isTerminated;
    }

    /** 
     * @return the number of samples.
     */
    uint samples() const;

    /** 
     * @return the sample mean.
     */
    RealNumberType mean() const;

    /** 
     * @return the sample variance.
     */
    RealNumberType variance() const;

    /** 
     * @return the unbiased sample variance.
     */
    RealNumberType unbiasedVariance() const;

    /** 
     * @return the maximal value.
     */
    RealNumberType max() const;

    /** 
     * @return the minimal value.
     */
    RealNumberType min() const;
    

    /**
     * Return the median value of the Statistic values. It can be given in two possible cases:
     * - if the the values are stored in the 'Statistic' objects (not always a good solution). (complexity: linear on average)
     * - if the values were first stored and computed by the function @ref terminate(). 
     *  @return the median value.
     * 
     * @see terminate, Statistic 
     */
    
    RealNumberType median() ;

    
    /** 
     * Adds a new sample value [v].
     * 
     * @param v the new sample value.
     */
    void addValue( RealNumberType v );
  
    /**
     * Adds a sequence of sample values, scanning a container from
     * iterators [b] to [e].
     * 
     * Exemple: 
     @code
     vector<RealNumberType> x;
     Statistic stats;
     stats.addValue( x + 4, x + 10 );
     @endcode
     *
     * @param b an iterator on the starting point.
     * @param e an iterator after the last point.
     */
    template <class Iter>
    void addValues( Iter b, Iter e );

    /** 
     * Clears the object. As if it has just been created.
     */
    void clear();
 
    /**
     * Computes the median value of the statistics and switch to mode
     * which does not save the statistics samples (@ref
     * m_store_samples = false). Usefull only if the values are stored
     * (specified in the the constructor) else it doest nothing.
     *
     * @see median, Statistic, m_store_samples
     */
    void terminate();
    

    
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


    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * number of samples
     */
    uint m_samples;

    /**
     * stores the sum of sample values for computing sample mean.
     */
    RealNumberType m_exp;

    /**
     * stores the sum of squared sample values for
     * computing sample variance.
     */
    RealNumberType m_exp2;

    /**
     * stores the maximal sample value.
     */
    RealNumberType m_max;

    /**
     * stores the minimal sample value.
     */
    RealNumberType m_min;


    /**
     * stores the median sample value.
     */
    RealNumberType m_median;
    
    
    /**
     * stores the sample to determine the median value 
     * 
     **/
    std::vector < RealNumberType > m_values;


    /**
     * Tells if values must be stored or not. 
     */
    bool m_store_samples;
     
    
    /**
     * Tells if terminate was called. Usefull to return median value
     * even if the m_values are no more stored (possible after
     * serialisation which doest store the values.).
     *
     */
    bool m_isTerminated;


    // ------------------------- Hidden services ------------------------------
  protected:

 
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'Statistic'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Statistic' to write.
   * @return the output stream after the writing.
   */
  template <typename RealNumberType>
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const Statistic<RealNumberType> & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/Statistic.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Statistic_h

#undef Statistic_RECURSES
#endif // else defined(Statistic_RECURSES)
