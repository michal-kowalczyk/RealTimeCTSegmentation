/** @file ImageMultiscaleAnalysis.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImageMultiscaleAnalysis.h
//
// Creation : 2010/05/24
//
// Version : 2010/05/24
//
// Author : JOL
//
// Summary : Header file for files ImageMultiscaleAnalysis.ih and ImageMultiscaleAnalysis.cxx
//
// History :
//	2010/05/24 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImageMultiscaleAnalysis declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImageMultiscaleAnalysis_RECURSES)
#error Recursive header files inclusion detected in ImageMultiscaleAnalysis.h
#else // defined(ImageMultiscaleAnalysis_RECURSES)
#define ImageMultiscaleAnalysis_RECURSES

#if !defined ImageMultiscaleAnalysis_h
#define ImageMultiscaleAnalysis_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/image/Image2D.h"
#include "ImaGene/helper/DigitalElevationMapStats.h"
#include "ImaGene/helper/ScaleProfile.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class ImageMultiscaleAnalysis
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImageMultiscaleAnalysis' <p> Aim: This
   * class analyzes an image through its multiscale representation. It
   * uses a lot of discrete geometry tools to try to figure out
   * meaning scales and local noise level in the image.
   */
  class ImageMultiscaleAnalysis
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~ImageMultiscaleAnalysis();

    /**
     * Constructor. 
     */
    ImageMultiscaleAnalysis();

    /**
     * Clear object. The object is as it was just instantiated.
     */
    void clear();

    /**
     * Initializes the object with an image as well as the maximal
     * scale to consider.
     *
     * @param input any grey-level image.
     * @param max_scale the maximal scale for the analysis.
     */
    void init( const ByteImage2D & input, uint max_scale );

    /**
     * Returns the scale profile at the given cell (specified by its
     * Khalimsky coordinates in the base image.
     *
     * @param sp (returns) the scale profile.
     * @param kx the Khalimsky x-coordinate of the cell (doubled). 
     * @param ky the Khalimsky y-coordinate of the cell (doubled). 
     */
    void getScaleProfile( ScaleProfile & sp, int kx, int ky ) const;

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
     * The vector containing the different scales for the analysis.
     */
    std::vector<float>* m_scales;

    /**
     * Contains the sequences of images at different resolutions as
     * well as the associated statistics.
     * Same size as m_scales.
     */
    std::vector< DigitalElevationMapStats* >* m_emaps;

    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImageMultiscaleAnalysis( const ImageMultiscaleAnalysis & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImageMultiscaleAnalysis & operator=( const ImageMultiscaleAnalysis & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'ImageMultiscaleAnalysis'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ImageMultiscaleAnalysis' to write.
   * @return the output stream after the writing.
   */
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const ImageMultiscaleAnalysis & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/ImageMultiscaleAnalysis.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImageMultiscaleAnalysis_h

#undef ImageMultiscaleAnalysis_RECURSES
#endif // else defined(ImageMultiscaleAnalysis_RECURSES)
