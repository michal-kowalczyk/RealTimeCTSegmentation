/** @file C4CELength.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CELength.h
//
// Creation : 2007/02/22
//
// Version : 2007/02/22
//
// Author : JOL
//
// Summary : Header file for files C4CELength.ih and C4CELength.cxx
//
// History :
//	2007/02/22 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CELength declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CELength_RECURSES)
#error Recursive header files inclusion detected in C4CELength.h
#else // defined(C4CELength_RECURSES)
#define C4CELength_RECURSES

#if !defined C4CELength_h
#define C4CELength_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/mathutils/Mathutils.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class C4CELength
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'C4CELength' <p> Aim: An abstract class to
   * encapsulate the elementary length of each surfel of a digital
   * contour.
   */
  class C4CELength
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~C4CELength();

    /**
     * Constructor.
     * 
     * @param nb_surfel the number of surfels of the contour.
     */
    C4CELength( uint nb_surfel );

    /** 
     * @param idx1 the index of a surfel between 0 and 'nb_surfel'.
     * @param idx2 the index of a surfel between 0 and 'nb_surfel'.
     * 
     * @return the length from the surfel [idx1] to the surfel [idx2]
     * excluded while increasing indices.
     */
    INLINE double length( uint idx1, uint idx2 ) const;

    /** 
     * @param idx the index of a surfel between 0 and 'nb_surfel'.
     * 
     * @return the elementary length of the surfel [idx].
     */
    INLINE double elength( uint idx ) const;

    /** 
     * @param idx the index of a surfel between 0 and 'nb_surfel'.
     * 
     * @return the elementary length of the surfel [idx].
     */
    INLINE double & elength( uint idx );

    /** 
     * @return the number of surfels.
     */
    INLINE uint nbSurfels() const;


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
     * The array of elementary lengthes.
     */
    double* m_elength;

    /**
     * Useful to make computations with a modulo 'nbSurfels'.
     */
    Mathutils::ModuloComputer m_mc;


    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE C4CELength();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE C4CELength( const C4CELength & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE C4CELength & operator=( const C4CELength & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'C4CELength'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'C4CELength' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const C4CELength & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/C4CELength.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined C4CELength_h

#undef C4CELength_RECURSES
#endif // else defined(C4CELength_RECURSES)
