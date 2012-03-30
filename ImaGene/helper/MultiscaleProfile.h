/** @file MultiscaleProfile.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : MultiscaleProfile.h
//
// Creation : 2009/07/10
//
// Version : 2009/07/10
//
// Author : JOL
//
// Summary : Header file for files MultiscaleProfile.ih and MultiscaleProfile.cxx
//
// History :
//	2009/07/10 : ?Name? : ?What?
//
// Rcs Id : "@(#)class MultiscaleProfile declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(MultiscaleProfile_RECURSES)
#error Recursive header files inclusion detected in MultiscaleProfile.h
#else // defined(MultiscaleProfile_RECURSES)
#define MultiscaleProfile_RECURSES

#if !defined MultiscaleProfile_h
#define MultiscaleProfile_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <utility>
#include "ImaGene/dgeometry2d/MultiscaleFreemanChain.h"
#include "ImaGene/base/Vector.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  class Statistics;

  /////////////////////////////////////////////////////////////////////////////
  // class MultiscaleProfile
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'MultiscaleProfile' <p> Aim: Useful to
   * detect noise on Freeman chains based on multiscale decomposition.
   */
  class MultiscaleProfile : public MultiscaleFreemanChain
  {

    // ----------------------- Standard services ------------------------------
  public:
    struct LengthStatsAtScale
    {
      uint scale;
      Statistics* stats;
      std::vector< std::pair<SubsampledChainKey,double> > longest_ms; 
      std::vector< std::pair<SubsampledChainKey,double> > longest_mean; 
      LengthStatsAtScale()
	: stats( 0 )
      {}
      ~LengthStatsAtScale()
      {
	if ( stats != 0 ) delete stats;
      }
    };

    std::vector<LengthStatsAtScale> all_stats;

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~MultiscaleProfile();

    /**
     * Constructor.
     */
    MultiscaleProfile();
    
    /**
     * Computes the multiresolutions of the given Freeman chain [src]
     * from (1,1) up to (h,v)=(r,r) with shifts. Starts profile
     * analysis.
     *
     * @param src the source Freeman chain.
     * @param r the maximal resolution.
     */
    void init( const FreemanChain & src, uint r );

    /**
     * @param x (returns) the x-value of the profile (log(scale+1)).
     * @param y (returns) the y-value of the profile (log(length of ms)).
     * @param idx the index of the surfel.
     */
    void profile( std::vector<double> & x, 
		  std::vector<double> & y,  
		  uint idx ) const;


    /**
     * @param x (returns) the x-value of the profile (log(scale+1)).
     * @param y (returns) the y-value of the profile (log(length of ms)).
     * @param idx the index of the surfel.
     */
    void profileFromMedian( std::vector<double> & x, 
			    std::vector<double> & y,  
			    uint idx ) const;
    
    /**
     * Computes the detailed profile for the surfel of index [idx]. All
     * values (scale, length of ms) are inserted in [x] and [y],
     * contrarily to 'profile' which inserts only the mean values. The
     * vector [nb] contains the number of samples for each scale.
     * 
     * @param x (returns) the x-value of the profile (log(scale+1)).
     * @param y (returns) the y-value of the profile (log(length of ms)).
     * @param nb (returns) the number of samples for each scale.
     * @param idx the index of the surfel.
     */
    void detailedProfile( std::vector<double> & x, 
			  std::vector<double> & y,  
			  std::vector<uint> & nb,  
			  uint idx ) const;


    /**
     * A meaningful scale is an interval of scales of length no
     * smaller than [min_width] and in which the profile has slopes
     * below [max_slope]. This method returns the sequence of
     * meaningful scales for surfel [idx].
     *
     * @param intervals (returns) a list of meaningful scales.
     * @param idx the surfel of interest.
     * @param min_width the minimum length for the meaningful scales.
     * @param max_slope the maximum allowed slope for length evolution.
     */
    void meaningfulScales( std::vector< std::pair< uint, uint > > & intervals,
			   uint idx,
			   uint min_width = 1,
			   double max_slope = -0.2 ) const;

    /**
     * The noise level is the first scale of the first meaningful
     * scale. A meaningful scale is an interval of scales of length no
     * smaller than [min_width] and in which the profile has slopes
     * below [max_slope]. 
     *
     * @param idx the surfel of interest.
     * @param min_width the minimum length for the meaningful scales.
     * @param max_slope the maximum allowed slope for length evolution.
     * @return the noise level or zero is none was found.
     * @see meaningfulScales
     */
    uint noiseLevel( uint idx,
		     uint min_width = 1,
		     double max_slope = -0.2 ) const;


    /**
     * Test BK 21/09/09
     *
     * The maximal valid scale is the longest interval presenting
     * slopes less than maxSlope. 
     * 
     *  @return the first scale of the longest interval at point [idx].
     */
    uint maximalValidScale ( uint idx, double maxSlope ) const;




    /**
     * Test BK 22/09/09
     *
     * The maxMeaningfulScale is a meaningfulscale with the maximal length of maximal segments.
     *
     *  @return the first scale of the interval with longest maximal segment at point [idx].
     */
    
    uint maxMeaningfulScale ( uint idx, uint minSize,  double maxSlope ) const;
    
    


    
    /**
     * Returns the key specifying the subsampled chain where this
     * surfel has the longest maximal segment in its surrounding.
     *
     * @param idx a valid surfel index.
     * @param scale the desired scale.
     * @return the specified key.
     */
    SubsampledChainKey longestMaxAtScale( uint idx, uint scale ) const;

    /**
     * Returns the key specifying the subsampled chain where this
     * surfel has the greatest mean for the lengths of maximal
     * segments in its surrounding.
     *
     * @param idx a valid surfel index.
     * @param scale the desired scale.
     * @return the specified key.
     */
    SubsampledChainKey longestMeanAtScale( uint idx, uint scale ) const;

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


    // ------------------------- Hidden services ------------------------------
    //protected:
  public:

    
    
  private:
    
    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE MultiscaleProfile( const MultiscaleProfile & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE MultiscaleProfile & operator=( const MultiscaleProfile & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'MultiscaleProfile'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'MultiscaleProfile' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const MultiscaleProfile & that_object_to_display );


  



  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/MultiscaleProfile.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined MultiscaleProfile_h

#undef MultiscaleProfile_RECURSES
#endif // else defined(MultiscaleProfile_RECURSES)
