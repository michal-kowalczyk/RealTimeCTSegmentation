/** @file Image2D.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Image2D.h
//
// Creation : 2010/05/19
//
// Version : 2010/05/19
//
// Author : JOL
//
// Summary : Header file for files Image2D.ih and Image2D.cxx
//
// History :
//	2010/05/19 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Image2D declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Image2D_RECURSES)
#error Recursive header files inclusion detected in Image2D.h
#else // defined(Image2D_RECURSES)
#define Image2D_RECURSES

#if !defined Image2D_h
#define Image2D_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/image/Pixel.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{

  /**
   * Checks class 'Image2D'.
   * @return 'true' if test went ok.
   */
  static bool checkImage2D();

  /////////////////////////////////////////////////////////////////////////////
  // class Image2D
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Image2D' <p>
   * Aim: A simple class to model a 2D image of value_type ELEMENT.
   *
   @code
   Image2D<uint8> I( 100, 100, 17 );
   Pixel p = I.lowest();
   Pixel q = I.highest();
   double average = 0.0;
   for ( Pixel z = p; z.y < q.y; z.y++ )
     for ( z.x = p.x; z.x < q.x; z.x++ )
       average += (double) I.at( z );
   average /= I.size();
   @endcode
   */
  template <typename ELEMENT, typename PIXELTYPE = ImaGene::Pixel>
  class Image2D
  {
  public:
    typedef ELEMENT value_type;
    typedef ELEMENT returned_type;
    typedef const ELEMENT & const_reference;
    typedef ELEMENT & reference;
    typedef ELEMENT* pointer;
    typedef PIXELTYPE pixel_type;

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~Image2D();

    /**
     * Constructor.
     * The image has size 0x0.
     */
    Image2D();

    /**
     * Constructor from sizes. The image is filled with [fill_value].
     *
     * @param width the width of the image (in pixels).
     * @param height the height of the image (in pixels).
     * @param fill_value The image is filled with [fill_value].
     */
    Image2D( uint width, uint height, value_type fill_value = ELEMENT() );

    /**
     * Reinitializer from sizes.
     *
     * @param width the width of the image (in pixels).
     * @param height the height of the image (in pixels).
     */
    void init( uint width, uint height );

    /**
     * @param clone (returned) a copy of 'this' after processing.
     */
    void copy( Image2D<ELEMENT,PIXELTYPE> & clone ) const;
    
    /**
     * Minimum possible value for ELEMENT.
     */
    static returned_type minElementValue();

    /**
     * Maximum possible value for ELEMENT.
     */
    static returned_type maxElementValue();

    /**
     * @return the pixel (0,0) of the image.
     */
    PIXELTYPE lowest() const;

    /**
     * @return the pixel (width,height) of the image.
     */
    PIXELTYPE highest() const;
    
    /**
     * @return the maximal value minus the minimal value.
     */
    returned_type valueRange() const;

    /**
     * @return the width of the image.
     */
    uint width() const;

    /**
     * @return the height of the image.
     */
    uint height() const;

    /**
     * @return the number of pixels of the image.
     */
    uint size() const;

    /**
     * @param x the x-coordinate of the pixel.
     * @param y the y-coordinate of the pixel.
     * @param def_val the default value.
     *
     * @return the value of the image at (x,y) if it is a valid
     * position otherwise the given default value.
     */
    returned_type at( int x, int y, 
		      const_reference def_val = ELEMENT() ) const;

    /**
     * @param x the x-coordinate of the pixel.
     * @param y the y-coordinate of the pixel.
     *
     * @param the new value of the image at (x,y).
     */
    void set( int x, int y, const_reference value );

    /**
     * @param p the pixel.
     * @param def_val the default value.
     *
     * @return the value of the image at (x,y) if it is a valid
     * position otherwise the given default value.
     */
    returned_type at( const PIXELTYPE & p,
		      const_reference def_val = ELEMENT() ) const;

    /**
     * @param p the pixel.
     *
     * @param the new value of the image at (x,y).
     */
    void set( const PIXELTYPE & p, const_reference value );

    /**
     * Fills the image with [fill_value].
     * @param any value.
     */
    void fill( value_type fill_value );

    /**
     * @return the most centered pixel of the image, and if several
     * are possible, the smallest one.
     */
    PIXELTYPE center() const;

    /**
     * Free the image buffer. The image has now size (0, 0).
     * Sets m_highest accordingly.
     */
    void free();
    
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
     * Stores image data as a linear array.
     */
    ELEMENT* m_data;

    /**
     * Lowest pixel (0,0).
     */
    PIXELTYPE m_lowest;

    /**
     * highes pixel (width,height).
     */
    PIXELTYPE m_highest;

    // ------------------------- Hidden services ------------------------------
  protected:


  private:
    

    /**
     * @param x the x-coordinate of the pixel.
     * @param y the y-coordinate of the pixel.
     *
     * @return the offset of (x,y) in the linear array m_data.
     */
    uint offset( uint x, uint y ) const;


    /**
     * Reallocate a big enough buffer for the given width and height.
     * Sets m_highest accordingly.
     */
    void realloc( uint width, uint height );

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE Image2D( const Image2D & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE Image2D & operator=( const Image2D & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'Image2D'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Image2D' to write.
   * @return the output stream after the writing.
   */
  
  template <typename ELEMENT, typename PIXELTYPE>
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const Image2D<ELEMENT,PIXELTYPE> & that_object_to_display );


  // ------------------- Definition of usual specialization -------------

  typedef Image2D<uint8,ImaGene::Pixel> ByteImage2D;
  typedef Image2D<uint16,ImaGene::Pixel> ShortImage2D;
  typedef Image2D<float,ImaGene::Pixel> FloatImage2D;

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/image/Image2D.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Image2D_h

#undef Image2D_RECURSES
#endif // else defined(Image2D_RECURSES)
