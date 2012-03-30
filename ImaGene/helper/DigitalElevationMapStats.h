/** @file DigitalElevationMapStats.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DigitalElevationMapStats.h
//
// Creation : 2010/05/21
//
// Version : 2010/05/21
//
// Author : JOL
//
// Summary : Header file for files DigitalElevationMapStats.ih and DigitalElevationMapStats.cxx
//
// History :
//	2010/05/21 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DigitalElevationMapStats declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DigitalElevationMapStats_RECURSES)
#error Recursive header files inclusion detected in DigitalElevationMapStats.h
#else // defined(DigitalElevationMapStats_RECURSES)
#define DigitalElevationMapStats_RECURSES

#if !defined DigitalElevationMapStats_h
#define DigitalElevationMapStats_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/mathutils/Statistic.h"
#include "ImaGene/digitalnD/KnRCellVector.h"
#include "ImaGene/helper/DigitalElevationMap.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class DigitalElevationMapStats
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'DigitalElevationMapStats' <p>
   * Aim: 
   */
  class DigitalElevationMapStats : public DigitalElevationMap
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~DigitalElevationMapStats();

    /**
     * Constructor. The object is not valid.
     */
    DigitalElevationMapStats();

    /**
     * Clear all stats.
     */
    void clearStats();

    /**
     * Compute all statistics (digital lengths of maximal segments).
     */
    void computeStats();

    /**
     * @return the statistic of all samples (lengths) at the given linel.
     */
    const Statistic<float> & stat( Kn_uid linel ) const;

 
  private:

    /**
     * Compute statistics (digital lengths of maximal segments) along
     * [i]Z paths.
     *
     * @param i a coordinate index (0 or 1).
     */
    void computeStatsAlongIZ( uint i );

    /**
     * Compute statistics (digital lengths of maximal segments) along XY paths.
     */
    void computeStatsAlongXY();

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
     * Stores the sum of the samples for each cell.
     */
    KnRUCellVector< Statistic< float> >* m_stats;


    // ------------------------- Hidden services ------------------------------
  protected:

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE DigitalElevationMapStats( const DigitalElevationMapStats & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE DigitalElevationMapStats & operator=( const DigitalElevationMapStats & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'DigitalElevationMapStats'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'DigitalElevationMapStats' to write.
   * @return the output stream after the writing.
   */
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const DigitalElevationMapStats & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/DigitalElevationMapStats.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DigitalElevationMapStats_h

#undef DigitalElevationMapStats_RECURSES
#endif // else defined(DigitalElevationMapStats_RECURSES)
