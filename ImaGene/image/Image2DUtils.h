/** @file Image2DUtils.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Image2DUtils.h
//
// Creation : 2010/05/20
//
// Version : 2010/05/20
//
// Author : JOL
//
// Summary : Header file for files Image2DUtils.ih and Image2DUtils.cxx
//
// History :
//	2010/05/20 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Image2DUtils declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#if defined(Image2DUtils_RECURSES)
#error Recursive header files inclusion detected in Image2DUtils.h
#else // defined(Image2DUtils_RECURSES)
#define Image2DUtils_RECURSES

#if !defined Image2DUtils_h
#define Image2DUtils_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/digitalnD/KnRCellVector.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnRCellVector.h"

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class Image2DUtils
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Image2DUtils' <p>
   * Aim: Utility class for Image2D.
   */

  template <typename IMAGE>
  class Image2DUtils
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~Image2DUtils();

    /**
     * Constructor. Does nothing.
     */
    Image2DUtils();


    /**
     * Computes the subsampled image [output] from image [input] at
     * points x0 + k*h + l*v. z gives the quantification of the
     * image values.
     *
     * @param output (returns) the subsampled image.
     * @param input the input image.
     * @param h the subsampling step along x.
     * @param v the subsampling step along y.
     * @param x0 the origin for the sampling.
     *
     * @param z the quantification (1 is no quantification, 2 divides
     * by two, etc)
     */
    static 
    void subsample( IMAGE & output, const IMAGE & input,
		    const uint h, const uint v, 
		    const typename IMAGE::pixel_type x0,
		    const uint z = 1 );

    /**
     * Convolves the input image with the gaussian kernel of variance sigma.
     *
     * @param output (returns) the convolved image.
     * @param input the input image.
     * @param sigma the variance for the Gaussian kernel.
     *
     * @param window_scale the factor for computing the size of the
     * convolution window (3.0 approximates 99.7% of the signal), the
     * greater the more precise but also the slower.
     */
    static
    void gaussianSmoothing( IMAGE & output, const IMAGE & input,
			    const double sigma = 1.0, 
			    const double window_scale = 3.0 );

    /**
     * Convolved the centered signal along x-coordinate with
     * the given input image and update the output image.
     *
     * @param output this image is modified to hold the result of the
     * convolution (output takes the size of input).
     *
     * @param input the input image.
     *
     * @param signal a functor object with f( double x ) : double.
     *
     * @param window_low the lower bound of the convolution window.
     * @param window_high the upper bound of the convolution window.
     * @param k the sampling of the window (2*k+1 points).
     *
     * @param normalize when 'true' normalizes the integral of the
     * signal to 1, 'false' takes the signal sampling as is.
     */
    template <typename SIGNAL>
    static
    void convolveX( IMAGE & output, 
		    const IMAGE & input, const SIGNAL & signal,
		    double window_low, double window_high, uint k,
		    bool normalize = false );

    /**
     * Convolved the centered signal along y-coordinate with
     * the given input image and update the output image.
     *
     * @param output this image is modified to hold the result of the
     * convolution (output takes the size of input).
     *
     * @param input the input image.
     *
     * @param signal a functor object with f( double x ) : double.
     *
     * @param window_low the lower bound of the convolution window.
     * @param window_high the upper bound of the convolution window.
     * @param k the sampling of the window (2*k+1 points).
     *
     * @param normalize when 'true' normalizes the integral of the
     * signal to 1, 'false' takes the signal sampling as is.
     */
    template <typename SIGNAL>
    static
    void convolveY( IMAGE & output, 
		    const IMAGE & input, const SIGNAL & signal,
		    double window_low, double window_high, uint k,
		    bool normalize = false );


    static 
    void
    convertToKnRUCellVector(const IMAGE & input,  KnSpace* & ks,
			    KnRUCellVector<typename IMAGE::value_type>* & vectorVal );

    

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
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE Image2DUtils( const Image2DUtils & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE Image2DUtils & operator=( const Image2DUtils & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'Image2DUtils'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Image2DUtils' to write.
   * @return the output stream after the writing.
   */
  template <typename IMAGE>
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const Image2DUtils<IMAGE> & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/image/Image2DUtils.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Image2DUtils_h

#undef Image2DUtils_RECURSES
#endif // else defined(Image2DUtils_RECURSES)
