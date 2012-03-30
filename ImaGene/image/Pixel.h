/** @file Pixel.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Pixel.h
//
// Creation : 2010/05/19
//
// Version : 2010/05/19
//
// Author : JOL
//
// Summary : Header file for files Pixel.ih and Pixel.cxx
//
// History :
//	2010/05/19 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Pixel declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Pixel_RECURSES)
#error Recursive header files inclusion detected in Pixel.h
#else // defined(Pixel_RECURSES)
#define Pixel_RECURSES

#if !defined Pixel_h
#define Pixel_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class Pixel
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Pixel' <p>
   * Aim: A simple class to represent a pixel element.
   */
  struct Pixel
  {
    typedef int integer_t;

    // ----------------------- public data ------------------------------
  public:

    /**
     * the x-coordinate
     */
    integer_t x;

    /**
     * the y-coordinate
     */
    integer_t y;

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~Pixel();

    /**
     * Constructor. Default position is arbitrary.
     */
    Pixel();

    /**
     * Constructor from position.
     */
    Pixel( integer_t nx, integer_t ny );

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    Pixel( const Pixel & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    Pixel & operator=( const Pixel & other );

    /**
     * Equality operator.
     * @param other the other pixel.
     * @return 'true' if this and [other] have the same coordinates.
     */
    bool operator==( const Pixel & other ) const;

    /**
     * Less than operator (lexicographic ordering).
     * @param other the other pixel.
     * @return 'true' if this.y < other.y or if equal, this.x < other.x.
     */
    bool operator<( const Pixel & other ) const;

    /**
     * Read only accessor.
     * @param i the coordinate index (0 or 1 ).
     * @return the coordinate.
     */
    integer_t operator[]( size_t i ) const;

    /**
     * Read-write only accessor.
     * @param i the coordinate index (0 or 1 ).
     * @return the coordinate.
     */
    integer_t & operator[]( size_t i );
  

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


    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'Pixel'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Pixel' to write.
   * @return the output stream after the writing.
   */
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const Pixel & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/image/Pixel.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Pixel_h

#undef Pixel_RECURSES
#endif // else defined(Pixel_RECURSES)
