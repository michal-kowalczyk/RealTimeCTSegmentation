/** @file Statistics.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Statistics.h
//
// Creation : 2007/02/19
//
// Version : 2007/02/19
//
// Author : JOL
//
// Summary : Header file for files Statistics.ih and Statistics.cxx
//
// History :
//	2007/02/19 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Statistics declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Statistics_RECURSES)
#error Recursive header files inclusion detected in Statistics.h
#else // defined(Statistics_RECURSES)
#define Statistics_RECURSES

#if !defined Statistics_h
#define Statistics_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <utility>
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
  // class Statistics
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Statistics' <p> Aim: This class stores a
   * set of sample values for several variables and can then compute
   * different statistics, like sample mean, sample variance, sample
   * unbiased variance, etc.
   * @TODO a terminer
   */
  class Statistics
  {
    // ------------------------- static services ------------------------------
  public:

    /**
     * Reads the input stream [in] line by line, excluding line beginning
     * with '#' and selects some columns according to [indices] to fill
     * the array of samples [samples]. For instance, if indices == { 3, 4,
     * 1 }, then samples( 0 ) will contain the third column, samples( 1 ),
     * the fourth column and samples( 2 ) the first column.
     *
     * @param in (modified) the input stream.
     * @param samples (updates) stores the data.
     * @param indices specifies in which columns data are read.
     */
    static void read( std::istream & in, Statistics & samples,
		      const std::vector<uint> & indices );


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~Statistics();

    /** 
     * Creates an object for computing [size] statistics.
     * 
     * @param size the number of statistical variables.
     *
     * @param store_samples when 'true', stores each given sample,
     * default is 'false'
     */    
    Statistics( uint size, bool store_samples = false );


    // ----------------------- Accessors ------------------------------
  public:

    /** 
     * @return the number of statistical variables.
     */
    INLINE uint nb() const;


    /** 
     * @param k the statistical variable.
     * 
     * @return the number of samples for this variable.
     */
    INLINE uint samples( uint k ) const;

    /** 
     * @param k the statistical variable.
     * 
     * @return the sample mean for this variable.
     * @pre method 'terminate' must have been called.
     * @see terminate
     */
    INLINE double mean( uint k ) const;

    /** 
     * @param k the statistical variable.
     * 
     * @return the sample variance for this variable.
     * @pre method 'terminate' must have been called.
     * @see terminate
     */
    INLINE double variance( uint k ) const;

    /** 
     * @param k the statistical variable.
     * 
     * @return the unbiased sample variance for this variable.
     * @pre method 'terminate' must have been called.
     * @see terminate
     */
    INLINE double unbiasedVariance( uint k ) const;

    /** 
     * @param k the statistical variable.
     * 
     * @return the maximal value for this variable.
     * @pre method 'terminate' must have been called.
     * @see terminate
     */
    INLINE double max( uint k ) const;


    /** 
     * @param k the statistical variable.
     * 
     * @return the indice of the maximal value for this variable.
     * @pre method 'terminate' must have been called.
     * @see terminate
     */
    INLINE uint maxIndice( uint k ) const;




    /** 
     * @param k the statistical variable.
     * 
     * @return the minimal value for this variable.
     * @pre method 'terminate' must have been called.
     * @see terminate
     */
    INLINE double min( uint k ) const;



    /** 
     * @param k the statistical variable.
     * 
     * @return the indice of the minimal value for this variable.
     * @pre method 'terminate' must have been called.
     * @see terminate
     */
    INLINE uint minIndice( uint k ) const;





    /** 
     * Returns a stored sample value that was added before. The object
     * should have been initialized with 'store_samples = true'.
     *
     * @param k the statistical variable.
     *
     * @param i the index of the sample value, '0<=i<samples(k)'.
     * 
     * @return the [i]-th value for this variable.
     *
     * @see Statistics, init
     */
    INLINE double value( uint k, uint i ) const;
  


    /** 
     * Adds a new sample value [v] for the variable [k].
     * 
     * @param k the statistical variable.
     * @param v the new sample value.
     */
    void addValue( uint k, double v );
  
    /**
     * Adds a sequence of sample values for the variable [k], scanning
     * a container from iterators [b] to [e].
     * 
     * Exemple: 
     @code
     vector<double> x;
     Statistics stats( 1, true );
     stats.addValue( x + 4, x + 10 );
     @endcode
     *
     * @param k the statistical variable.
     * @param b an iterator on the starting point.
     * @param e an iterator after the last point.
     */
    template <class Iter>
    void addValues( uint k, Iter b, Iter e );

    /** 
     * Once all sample values have been added to this object, computes
     * meaningful statistics like sample mean, variance and unbiased
     * variance.
     * 
     * @see mean, variance, unbiasedVariance, min, max
     */
    void terminate();


    /** 
     * Reinitializes the object to contain [size] statistical
     * variables. All preceding sample values and statistics are
     * lost. Calls 'erase()'.
     * 
     * @param size the number of statistical variables.
     *
     * @param store_samples when 'true', stores each given sample,
     *
     * @see erase
     */
    void init( uint size, bool store_samples );


    /** 
     * Clears the object from all given samples. The object still
     * represents 'nb()' statistical variables.
     */
    void clear();
  

    /** 
     * Erases the object as if it was just created. The user must call
     * 'init()' before any other method.
     * 
     * @see init
     */
    void erase();

    /**
     * Computes the covariance of the [x],[y] variables. The
     * 'Statistics' object should have stored its values and
     * 'terminate' should have been called.
     *
     * @param x the index of the x-variable (should be valid )
     * @param y the index of the y-variable (should be valid )
     * @param s the starting value index (default is 0 )
     * @param e the after-the-end value index (default is 0, meaning all )
     *
     * @return the covariance of [x],[y] from indices [s] to [e].
     */
    double covariance( uint x, uint y, 
		       uint s = 0, uint e = 0 ) const;

    /**
     * Computes the linear regression of the ([x],[y]) variables. The
     * 'Statistics' object should have stored its values and
     * 'terminate' should have been called.
     *
     * @param x the index of the x-variable (should be valid )
     * @param y the index of the y-variable (should be valid )
     *
     * @return the pair (slope, shift at origin )
     */
    std::pair<double,double> linearRegression( uint x, uint y ) const;



    /**
     * Computes the median value for a variable k. The
     * 'Statistics' object should have stored its values and 'terminate'
     * should have been called.
     *
     * @param k
     *
     * @return the median value for the variable k.
     */
    double median( uint k );



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
     * Number of statistical variables.
     */
    uint m_nb;

    /**
     * For each variable, number of samples
     */
    uint* m_samples;

    /**
     * For each variable, stores the sum of sample values for
     * computing sample mean.
     */
    double* m_exp;

    /**
     * For each variable, stores the sum of squared sample values for
     * computing sample variance.
     */
    double* m_exp2;

    /**
     * For each variable, stores the sample variance.
     */
    double* m_var;

    /**
     * For each variable, stores the unbiased sample variance.
     */
    double* m_unbiased_var;
    
    /**
     * For each variable, stores the maximal sample value.
     */
    double* m_max;

    /**
     * For each variable, stores the indice of the maximal sample value.
     */
    uint* m_indice_max;

    /**
     * For each variable, stores the minimal sample value.
     */
    double* m_min;

    /**
     * For each variable, stores the indice of the minimal sample value.
     */
    uint* m_indice_min;

    /**
     * Tells if values must be stored or not.
     */
    bool m_store_samples;

    /**
     * For each variable, stores the sample values if
     * [m_store_samples] is 'true'.
     */
    std::vector<double>* m_values;


    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE Statistics();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE Statistics( const Statistics & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE Statistics & operator=( const Statistics & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'Statistics'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Statistics' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const Statistics & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/Statistics.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Statistics_h

#undef Statistics_RECURSES
#endif // else defined(Statistics_RECURSES)
