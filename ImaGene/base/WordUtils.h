/** @file WordUtils.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : WordUtils.h
//
// Creation : 2008/06/27
//
// Version : 2008/06/27
//
// Author : JOL
//
// Summary : Header file for files WordUtils.ih and WordUtils.cxx
//
// History :
//	2008/06/27 : ?Name? : ?What?
//
// Rcs Id : "@(#)class WordUtils declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(WordUtils_RECURSES)
#error Recursive header files inclusion detected in WordUtils.h
#else // defined(WordUtils_RECURSES)
#define WordUtils_RECURSES

#if !defined WordUtils_h
#define WordUtils_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class WordUtils
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'WordUtils' <p> Aim: Contains a set of
   * functions to process binary words (ie. over a 2-letter alphabet).
   */
  class WordUtils
  {

    // ----------------------- Standard services ------------------------------
  public:
    /**
     * The index datatype.
     */
    typedef unsigned int index_t;

    /**
     * The size datatype.
     */
    typedef unsigned int size_t;

    /**
     * Destructor. 
     */
    ~WordUtils();

    /**
     * Constructor.
     * The alphabet defines the lexicographic order.
     *
     * @param zero the first letter of the alphabet.
     * @param one the second letter of the alphabet.
     */
    WordUtils( char zero, char one );

    /**
     * @param c1 a letter in the alphabet
     * @param c2 another letter in the same alphabet.
     * @return 'true' iff c1 < c2
     */
    INLINE bool less( char c1, char c2 ) const;

    /**
     * @param c1 a letter in the alphabet
     * @param c2 another letter in the same alphabet.
     * @return 'true' iff c1 <= c2
     */
    INLINE bool lessOrEqual( char c1, char c2 ) const;

    /**
     * @param c1 a letter in the alphabet
     * @param c2 another letter in the same alphabet.
     * @return 'true' iff c1 == c2
     */
    INLINE bool equal( char c1, char c2 ) const;

    // ----------------------- word services ----------------------------------
  public:

    /**
     * @param w a word
     * @param s the starting index in [w].
     * @param e the index after the end in [w].
     * @return 'true' if the [w] is in the alphabet.
     */
    bool isInAlphabet( const std::string & w, index_t s, index_t e ) const;

    /**
     * Gives the first lyndon factor of the binary word [w]
     * starting at position [s].
     *
     * @param len (returns) the length of the primitive Lyndon factor
     * (which starts at position s).
     *
     * @param nb (returns) the number of times the Lyndon factor appears.
     * 
     * @param w a word
     * @param s the starting index in [w].
     * @param e the index after the end in [w] (s<e).
     */
    void firstLyndonFactor( size_t & len, size_t & nb,
			    const std::string & w, 
			    index_t s, index_t e ) const;

    /**
     * Gives the first lyndon factor of the binary cyclic word [w]
     * starting at position [s].
     *
     * @param len (returns) the length of the primitive Lyndon factor
     * (which starts at position s).
     *
     * @param nb (returns) the number of times the Lyndon factor appears.
     * 
     * @param w a word
     * @param s the starting index in [w].
     * @param e the index after the end in [w] (s and e arbitrary).
     */
    void firstLyndonFactorMod( size_t & len, size_t & nb,
			       const std::string & w, 
			       index_t s, index_t e ) const;


    /**
     * Counts the number of letters [c] in [w] from position [s] to
     * [e] excluded.
     *
     * @param w a word
     * @param c the letter [c] for which the number of occurences is searched.
     * @param s the starting index in [w].
     * @param e the index after the end in [w].
     *
     * @return the number of letters [c] in [w] from position [s] to
     * [e] excluded.
     */
    size_t count( const std::string & w, char c, 
		  index_t s, index_t e ) const;

    /**
     * Tells if a subword is Christoffel (upper-Christoffel).
     * An horizontal or vertical word is not an upper Christoffel word.
     *
     * @param w a word
     * @param s the starting index in [w].
     * @param e the index after the end in [w].
     * @return 'true' if w[s:e-1] is a Christoffel word.
     */
    bool isChristoffel( const std::string & w,
			index_t s, index_t e ) const;

    /**
     * Tells if a subword is Christoffel (upper-Christoffel) in a cyclic word.
     * An horizontal or vertical word is not an upper Christoffel word.
     *
     * @param w a word
     * @param s the starting index in [w].
     * @param e the index after the end in [w] (e and s are arbitrary).
     * @return 'true' if w[s:e-1] is a Christoffel word.
     */
    bool isChristoffelMod( const std::string & w,
			   index_t s, index_t e ) const;

    /**
     * Tells if a subword is upper-left convex. For instance, the path
     * '10111011010010001000' is upper-left convex. As a Freeman chain
     * code, it would represent a path in the first quadrant going
     * clockwise around a convex set.
     *
     * @param w a word
     * @param s the starting index in [w].
     * @param e the index after the end in [w].
     * @return 'true' if w[s:e-1] is a Christoffel word.
     */
    bool isULConvex( const std::string & w,
		     index_t s, index_t e ) const;

    /**
     * Tells if a subword is upper-left convex. For instance, the path
     * '10111011010010001000' is upper-left convex. As a Freeman chain
     * code, it would represent a path in the first quadrant going
     * clockwise around a convex set.
     *
     * @param w a cyclic word
     * @param s the starting index in [w].
     * @param e the index after the end in [w] (s and e arbitrary).
     * @return 'true' if w[s:e-1] is a Christoffel word.
     */
    bool isULConvexMod( const std::string & w,
			index_t s, index_t e ) const;


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
  public:

    /**
     * first and second letters of the alphabet.
     */
    char alph[ 2 ];

  private:


    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE WordUtils();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE WordUtils( const WordUtils & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE WordUtils & operator=( const WordUtils & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'WordUtils'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'WordUtils' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const WordUtils & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/WordUtils.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined WordUtils_h

#undef WordUtils_RECURSES
#endif // else defined(WordUtils_RECURSES)
