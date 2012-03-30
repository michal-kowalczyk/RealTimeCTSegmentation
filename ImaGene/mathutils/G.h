/** @file G.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : G.h
//
// Creation : 2005/01/05
//
// Version : 2005/01/05
//
// Author : JOL
//
// Summary : Header file for files G.ih and G.cxx
//
// History :
//	2005/01/05 : ?Name? : ?What?
//
// Rcs Id : "@(#)class G declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(G_RECURSES)
#error Recursive header files inclusion detected in G.h
#else // defined(G_RECURSES)
#define G_RECURSES

#if !defined G_h
#define G_h

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
  // class G
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'G' <p>
   * Aim: Implements the Gauss function x->exp(-x^2/s^2)/(s*sqrt(pi))
   */
  class G
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~G();

    /**
     * Construct a Gauss function with standard deviation [s].
     * @param s the standard deviation.
     */
    G(double s=1.0);

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE G( const G & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE G & operator=( const G & other );


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
     * @return the value of the Gauss function at [x].
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

  private:

  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'G'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'G' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const G & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/G.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined G_h

#undef G_RECURSES
#endif // else defined(G_RECURSES)
