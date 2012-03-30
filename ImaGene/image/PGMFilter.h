/** @file PGMFilter.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : PGMFilter.h
//
// Creation : 2010/05/19
//
// Version : 2010/05/19
//
// Author : JOL
//
// Summary : Header file for files PGMFilter.ih and PGMFilter.cxx
//
// History :
//	2010/05/19 : ?Name? : ?What?
//
// Rcs Id : "@(#)class PGMFilter declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(PGMFilter_RECURSES)
#error Recursive header files inclusion detected in PGMFilter.h
#else // defined(PGMFilter_RECURSES)
#define PGMFilter_RECURSES

#if !defined PGMFilter_h
#define PGMFilter_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "ImaGene/image/Image2D.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class PGMFilter
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'PGMFilter' <p>
   * Aim: Used to read and write images with PGM format.
   */
  template <typename IMAGE>
  class PGMFilter
  {
  public:
    enum ErrorType { Ok, IOError, ImageError, NotImplementedPGMFormatError };

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~PGMFilter();

    /**
     * Constructor. Does nothing.
     */
    PGMFilter();

    /**
     * Reads an input stream (which should follow the PGM format) to
     * create the image [ptr_image].
     *
     * @param ptr_image if 0, it points on the dyn. alloc. created
     * image, otherwise, the pointed image is modified.
     * @param in the input stream (coded in PGM).
     *
     * @return 'true' if everyhing went well, otherwise 'false'.
     * @see errorCode()
     */
    bool genericRead( IMAGE* & ptr_image, std::istream & in );

    /**
     * Reads an input stream (which should follow the PGM format) to
     * modify the image [ref_image].
     *
     * @param ref_image a reference to the image is updated from the stream.
     * @param in the input stream (coded in PGM).
     *
     * @return 'true' if everyhing went well, otherwise 'false'. In
     * @see errorCode()
     */
    bool read( IMAGE & ref_image, std::istream & in );

    /**
     * Reads an input stream (which should follow the PGM format) to
     * create the returned dynamically allocated image.
     *
     * @param in the input stream (coded in PGM).
     *
     * @return the pointer on the allocated image (or 0 if an error occured).
     * @see errorCode()
     */
    IMAGE* read( std::istream & in );

    
    /**
     * Writes on an output stream (in PGM format) the image [ptr_image].
     *
     * @param image a const reference on an image.
     * @param in the input stream (coded in PGM).
     *
     * @return 'true' if everyhing went well, otherwise 'false'. In
     * this case, errorCode()
     */
    bool write( const IMAGE & image, std::ostream & out );
 
    /**
     * @return the code of the last error or OK if the last operation went ok.
     */
    ErrorType errorCode() const;

    /**
     * @return the message corresponding to the last error.
     */
    std::string errorMessage() const;

    
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
     * The last error code.
     */
    ErrorType m_error;

    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE PGMFilter( const PGMFilter & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE PGMFilter & operator=( const PGMFilter & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'PGMFilter'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'PGMFilter' to write.
   * @return the output stream after the writing.
   */
  template <typename IMAGE>
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const PGMFilter<IMAGE> & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/image/PGMFilter.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined PGMFilter_h

#undef PGMFilter_RECURSES
#endif // else defined(PGMFilter_RECURSES)
