/** @file OrderedAlphabet.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : OrderedAlphabet.h
//
// Creation : 2009/06/24
//
// Version : 2009/06/24
//
// Author : JOL
//
// Summary : Header file for files OrderedAlphabet.ih and OrderedAlphabet.cxx
//
// History :
//	2009/06/24 : ?Name? : ?What?
//
// Rcs Id : "@(#)class OrderedAlphabet declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(OrderedAlphabet_RECURSES)
#error Recursive header files inclusion detected in OrderedAlphabet.h
#else // defined(OrderedAlphabet_RECURSES)
#define OrderedAlphabet_RECURSES

#if !defined OrderedAlphabet_h
#define OrderedAlphabet_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "ImaGene/base/Arguments.h"
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
  // class OrderedAlphabet
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'OrderedAlphabet' <p> Aim: Describes an
   * alphabet over an interval of (ascii) letters, where the
   * lexicographic order can be changed (shifted, reversed,
   * ...). Useful for the arithmetic minimum length polygon (AMLP).
   */
  class OrderedAlphabet
  {
    // ----------------------- Standard types ------------------------------
  public:
    /**
     * The index datatype.
     */
    typedef unsigned int index_t;

    /**
     * The size datatype.
     */
    typedef unsigned int size_t;

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~OrderedAlphabet();

    /**
     * Constructor from letters
     *
     * @param first the first letter of the alphabet.
     * @param nb the number of letters of the alphabet.
     *
     * Exemple: OrderedAlphabet( '0', 4 ) defines the alphabet for
     * 4-connected freeman chains.
     */
    INLINE OrderedAlphabet( char first, uint nb );

    /**
     * @return the current ordered alphabet.
     */
    std::string orderedAlphabet() const;

    /**
     * Shift a0 < a1 < ... < an to a1 < ... < an < a0
     */
    void shiftLeft();

    /**
     * Shift a0 < a1 < ... < an to an < a0 < ... < an-1
     */
    void shiftRight();

    /**
     * Reverse the order a0 < a1 < ... < an to an < ... < a1 < a0
     */
    void reverse();

    /**
     * Reverse the order a0 < a1 < ... < an to a3 < a2 < a1 < a0 < an < ... 
     */
    void reverseAround12();
    
    /**
     * @param c any valid letter in this alphabet.
     *
     * @return the index of the letter [c] in the order relation,
     * starting from 0 to m_nb-1.
     */
    INLINE uint order( char c ) const;

    /**
     * @param i the index of some letter in the order relation,
     * between 0 and m_nb-1.
     *
     * @return c the corresponding letter in this alphabet.
     *
     * NB: O(nb of letters in the alphabet).
     */
    INLINE char letter( uint i ) const;

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

    // ----------------------- word services ------------------------------
  public:

    /**
     * Gives the first lyndon factor of the word [w] starting at
     * position [s] in this alphabet.
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
     * Gives the first lyndon factor of the cyclic word [w]
     * starting at position [s] in this alphabet.
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
     * Adaptation of Duval's algorithm to extract the first Lyndon factor
     * (FLF). Whilst scanning the Lyndon factor, it also checks whether it
     * is a Christoffel word or not. It returns 'true' if the FLF is
     * indeed a Christoffel word, otherwise returns false. It starts the
     * extraction at position [s] in the word [w].
     *
     * The alphabet takes the form a0 < a1 < a2 < ... < an-1. [w] starts
     * with a1 or a2 at position s.
     *
     * See [Provencal, Lachaud 2009].
     *
     * @param len (returns) the length of the primitive Lyndon factor
     * (which starts at position s).
     *
     * @param nb (returns) the number of times the Lyndon factor appears.
     * 
     * @param w a word which starts with a1 or a2 at position s.
     * @param s the starting index in [w].
     * @param e the index after the end in [w] (s<e).
     */
    bool duvalPP( size_t & len, size_t & nb,
		  const std::string & w, 
		  index_t s, index_t e ) const;

    /**
     * Adaptation of Duval's algorithm to extract the first Lyndon factor
     * (FLF). Whilst scanning the Lyndon factor, it also checks whether it
     * is a Christoffel word or not. It returns 'true' if the FLF is
     * indeed a Christoffel word, otherwise returns false. It starts the
     * extraction at position [s] in the cyclic word [w].
     *
     * The alphabet takes the form a0 < a1 < a2 < ... < an-1. [w] starts
     * with a1 or a2 at position s.
     *
     * See [Provencal, Lachaud 2009].
     *
     * @param len (returns) the length of the primitive Lyndon factor
     * (which starts at position s).
     *
     * @param nb (returns) the number of times the Lyndon factor appears.
     * 
     * @param w a (cyclic) word which starts with a1 or a2 at position s.
     * @param s the starting index in [w].
     * @param e the index after the end in [w] (s and e arbitrary).
     */
    bool duvalPPMod( size_t & len, size_t & nb,
		     const std::string & w, 
		     index_t s, index_t e ) const;


    // ----------------------- MLP services -----------------------------------
  public:

    /**
     * Extracts the next edge of the minimum length polygon starting from
     * position [s] on the word [w]. The alphabet may be modified
     * (reversed or shifted). The output alphabet is some a0 < a1 < a2 < ...
     *
     * @param (returns) the number of letters a1 in the extracted edge (a1
     * in the output alphabet)
     *
     * @param (returns) the number of letters a2 in the extracted edge (a2
     * in the output alphabet)
     *
     * @param w the input (cyclic) word (may be modified in the process).
     *
     * @param s the starting point in the word (updated).
     *
     * @param cvx (updates) this boolean is flipped only if a change of
     * convexity is detected.
     *
     * @return the number of letters of the extracted edge.
     */ 
    size_t nextEdge( size_t & nb_a1, size_t & nb_a2,
		     std::string & w, index_t & s, bool & cvx );


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
  
    // ----------------------- Static services ------------------------------
  public:

    /**
     * Test the class. Useful for non-regression tests.
     *
     * @param args may give some contextual arguments (like trace,
     * debugging) 
     * @return the number of failed tests.
     */
    static uint testClass( const Arguments & args );

    /**
     * Checks if the Lyndon factorization works.
     *
     * @param input the input word.
     * @param output the expected output word as (w1)^l1...(wn)^ln.
     * @param trace the trace level (0:silent, 3:verbose)
     * @return 'true' if the test was ok, 'false' otherwise.
     */
    bool checkFLF( const std::string & output, 
		   const std::string & input,
		   uint trace ) const;

    /**
     * Checks if Duval++ works.
     *
     * @param input the input word.
     * @param output the expected output word as "NC" or "C(w1)^l1"
     * @param trace the trace level (0:silent, 3:verbose)
     * @return 'true' if the test was ok, 'false' otherwise.
     */
    bool checkDuvalPP( const std::string & output, 
		       const std::string & input,
		       uint trace ) const;

    /**
     * Checks if Duval++ modulo works.
     *
     * @param input the input word.
     * @param output the expected output word as "NC" or "C(w1)^l1"
     * @param s starting index.
     * @param trace the trace level (0:silent, 3:verbose)
     * @return 'true' if the test was ok, 'false' otherwise.
     */
    bool checkDuvalPPMod( const std::string & output, 
			  const std::string & input,
			  index_t s,
			  uint trace ) const;

    // ------------------------- Datas ----------------------------------------
  private:
    /**
     * the first character.
     */
    char m_first;

    /**
     * the number of letters.
     */
    uint m_nb;

    /**
     * the order relation, given by the isomorphism with 0..m_nb-1.
     */
    uint* m_order;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE OrderedAlphabet();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE OrderedAlphabet( const OrderedAlphabet & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE OrderedAlphabet & operator=( const OrderedAlphabet & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'OrderedAlphabet'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'OrderedAlphabet' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const OrderedAlphabet & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/OrderedAlphabet.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined OrderedAlphabet_h

#undef OrderedAlphabet_RECURSES
#endif // else defined(OrderedAlphabet_RECURSES)
