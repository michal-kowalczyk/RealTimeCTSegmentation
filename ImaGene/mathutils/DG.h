/** @file DG.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DG.h
//
// Creation : 2005/01/05
//
// Version : 2005/01/05
//
// Author : JOL
//
// Summary : Header file for files DG.ih and DG.cxx
//
// History :
//	2005/01/05 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DG declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DG_RECURSES)
#error Recursive header files inclusion detected in DG.h
#else // defined(DG_RECURSES)
#define DG_RECURSES

#if !defined DG_h
#define DG_h

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
  // class DG
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'DG' <p>
   * Aim: Implements the derivative of the Gauss function
   */
  class DG
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~DG();

    /**
     * Construct the derivative of the Gauss function with standard
     * deviation [s].
     *
     * @param s the standard deviation.
     */
    DG(double s=1.0);


    /**
     * Changes the value of the standard deviation.
     * @param s the standard deviation.
     */
    void setSigma(double s);

    /**
     * Accessor.
     * @return the standard deviation.
     */
    INLINE double sigma() const;


    /**
     * @param x any real value.
     * @return the value of the derivative of the Gauss function at [x].
     */
    INLINE double operator() (double x) const;


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
     * standard deviation.
     */
    double m_s;


    /**
     * Constant to speed up processing.
     */
    double m_c1;


    /**
     * Constant to speed up processing.
     */
    double m_c2;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE DG();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE DG( const DG & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE DG & operator=( const DG & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'DG'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'DG' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const DG & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/DG.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DG_h

#undef DG_RECURSES
#endif // else defined(DG_RECURSES)
