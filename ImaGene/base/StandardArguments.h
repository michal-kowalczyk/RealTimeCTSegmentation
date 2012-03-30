/** @file StandardArguments.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : StandardArguments.h
//
// Creation : 2008/12/17
//
// Version : 2008/12/17
//
// Author : JOL
//
// Summary : Header file for files StandardArguments.ih and StandardArguments.cxx
//
// History :
//	2008/12/17 : ?Name? : ?What?
//
// Rcs Id : "@(#)class StandardArguments declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(StandardArguments_RECURSES)
#error Recursive header files inclusion detected in StandardArguments.h
#else // defined(StandardArguments_RECURSES)
#define StandardArguments_RECURSES

#if !defined StandardArguments_h
#define StandardArguments_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "ImaGene/base/Arguments.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class StandardArguments
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'StandardArguments' <p>
   * Aim: A utility class to set up standard arguments.
   */
  class StandardArguments
  {

    // ----------------------- Static services ------------------------------
  public:

    /**
     * Add the standard arguments for debug and trace to argument
     * object [args].
     * -debug: debug mode (force some tests).
     * -trace <level>: trace information level (0: no trace, 3: very verbose)
     * -timing <level>: displays some timings (0: no timings, 3: lots).
     *
     * @param args (updated) the argument object to enrich.
     * @param debug if 'true' add "-debug"
     * @param trace if 'true' add "-trace"
     * @param timings if 'true' add "-timing"
     */
    static void addDebugArgs( Arguments & args, 
			      bool debug = true, bool trace = true, 
			      bool timings = false );

    /**
     * Add the standard arguments for I/O and trace to argument
     * object [args].
     * -input <file_in>: name of input file.
     * -output <file_out>: name of output file.
     *
     * @param args (updated) the argument object to enrich.
     * @param in if 'true' add "-input"
     * @param out if 'true' add "-output"
     */
    static void addIOArgs( Arguments & args, bool in = true, bool out = true );

    /**
     * Add the standard arguments for digital spaces to argument
     * object [args].
     * -d <dim>: dimension of the space
     * -x <sx>: size of the space along x, 0-axis
     * -y <sy>: size of the space along y, 1-axis
     * -z <sz>: size of the space along z, 2-axis
     * -t <st>: size of the space along t, 3-axis
     * -4567 <s4> <s5> <s6> <s7>: sizes of the space along axes 4 to 7.
     * -r <radius>: define a radius.
     * -ri <first> <last> <incr>: define a sequence of radii.
     * -digital: tells that the surface should be digital.
     *
     * @param args (updated) the argument object to enrich.
     * @param dim the maximum meaningful dimension.
     * @param radius if 'true' add radius parameters
     * @param digital if 'true' add digital parameters
     */
    static void addDigitalArgs( Arguments & args, 
				uint dim = 4, 
				bool radius = false, 
				bool digital = false );

    /**
     * Open input file or standard input depending on arguments. This
     * method is not reentrant.
     *
     * @param args the argument object.
     * @return an input stream (cin or file)
     * @see addIOArgs
     */
    static std::istream & openInput( const Arguments & args );

    /**
     * Open output file or standard output depending on
     * arguments. This method is not reentrant.
     *
     * @param args the argument object.
     * @return an output stream (cout or file)
     * @see addIOArgs
     */
    static std::ostream & openOutput( const Arguments & args );

    /**
     * @param args the argument object.
     *
     * @return the dimension of the digital space.
     *
     * @see addDigitalArgs
     */
    static unsigned int dim( const Arguments & args );

    /**
     * Fills the sizes of the digital space as specified in the
     * argument object.
     *
     * @param args the argument object.
     *
     * @param sizes (modified) a array of size at least the dimension
     * of the space.
     *
     * @see addDigitalArgs
     */
    static void fillSizes( const Arguments & args, unsigned int* sizes );

    /**
     * Fills the sizes of the digital space as specified in the
     * argument object.
     *
     * @param args the argument object.
     *
     * @param sizes (modified) a array of size at least the dimension
     * of the space.
     *
     * @see addDigitalArgs
     */
    static void fillSizes( const Arguments & args, unsigned long long* sizes );

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~StandardArguments();

    

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
     * The default input stream.
     */
    static std::ifstream m_input_stream;

    /**
     * The default output stream.
     */
    static std::ofstream m_output_stream;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE StandardArguments();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE StandardArguments( const StandardArguments & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE StandardArguments & operator=( const StandardArguments & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'StandardArguments'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'StandardArguments' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const StandardArguments & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/StandardArguments.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined StandardArguments_h

#undef StandardArguments_RECURSES
#endif // else defined(StandardArguments_RECURSES)
