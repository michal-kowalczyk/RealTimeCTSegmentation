/** @file KnUtils.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnUtils.h
//
// Creation : 2002/07/22
//
// Version : 2002/07/22
//
// Author : JOL
//
// Summary : Header file for files KnUtils.ih and KnUtils.cxx
//
// History :
//	2002/07/22 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnUtils declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnUtils_RECURSES)
#error Recursive header files inclusion detected in KnUtils.h
#else // defined(KnUtils_RECURSES)
/**
 * Useful to avoid recursive inclusion.
 */
#define KnUtils_RECURSES

#if !defined KnUtils_h
#define KnUtils_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "ImaGene/digitalnD/KnTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class KnUtils
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnUtils' <p>
 * Aim: To provide some basic tools for basic 'Kn' types.
 */
class KnUtils
{
  // ------------------------- Static services ------------------------------
public:
  /**
   * Builds a string displaying the value [val] as a decimal representation.
   * @param val the value to display
   * @param size if >0, the value will take [size] chars.
   * @param fill_with when [size]>0, the missing chars are filled with it.
   * @return the decimal representation of [val].
   */
  static std::string
  sprintDec( uint val, uint size = 0, char fill_with = ' ' );
  /**
   * Builds a string displaying the value [val] as an hexadecimal
   * representation.
   * @param val the value to display
   * @param size if >0, the value will take [size] chars.
   * @param fill_with when [size]>0, the missing chars are filled with it.
   * @return the hexadecimal representation of [val].
   */
  static std::string
  sprintHex( uint val, uint size = 0, char fill_with = ' ' );
  /**
   * Builds a string displaying the value [val] as a binary representation.
   * @param val the value to display
   * @param size if >0, the value will take [size] chars.
   * @param fill_with when [size]>0, the missing chars are filled with it.
   * @return the binary representation of [val].
   */
  static std::string
  sprintBin( uint val, uint size = 0, char fill_with = ' ' );

  /**
   * @param v any value.
   * @return the number of bits set to 1 in [v].
   */
  static INLINE uint countSetBits( uint8 v );
  /**
   * @param v any value.
   * @return the number of bits set to 1 in [v].
   */
  static INLINE uint countSetBits( uint16 v );
  /**
   * @param v any value.
   * @return the number of bits set to 1 in [v].
   */
  static INLINE uint countSetBits( uint32 v );
  /**
   * @param v any value.
   * @return the number of bits set to 1 in [v].
   */
  static INLINE uint countSetBits( uint64 v );
  /**
   * @param v any value.
   * @return the least-significant bit set to 1 in [v].
   */
  static INLINE uint getLSB( uint8 v );
  /**
   * @param v any value.
   * @return the least-significant bit set to 1 in [v].
   */
  static INLINE uint getLSB( uint16 v );
  /**
   * @param v any value.
   * @return the least-significant bit set to 1 in [v].
   */
  static INLINE uint getLSB( uint32 v );
  /**
   * @param v any value.
   * @return the least-significant bit set to 1 in [v].
   */
  static INLINE uint getLSB( uint64 v );
  
  /**
   * NB: Faster if most-significant bit is high.
   * @param v any value.
   * @return the most-significant bit set to 1 in [v], (-1) if 0
   */
  static INLINE int getMSBbySup( uint32 v );

  /**
   * NB: Faster if most-significant bit is low.
   * @param v any value.
   * @return the most-significant bit set to 1 in [v], (-1) if 0
   */
  static INLINE int getMSBbyInf( uint32 v );

  /**
   * NB: Relatively constant time and generally faster than the others.
   * @param v any value.
   * @return the most-significant bit set to 1 in [v], (0) if 0
   */
  static INLINE int getMSBbyLog( uint32 v );

  /**
   * Calls 'getMSBbyLog'.
   * @param v any value.
   * @return the most-significant bit set to 1 in [v], (0) if 0
   */
  static INLINE int getMSB( uint32 v );

  /**
   * NB: Relatively constant time and generally faster than the others.
   * @param v any value.
   * @return the most-significant bit set to 1 in [v], (0) if 0
   */
  static INLINE int getMSBbyLog( uint64 v );

  /**
   * Calls 'getMSBbyLog'.
   * @param v any value.
   * @return the most-significant bit set to 1 in [v], (0) if 0
   */
  static INLINE int getMSB( uint64 v );

  // ------------------------- General services ------------------------------
public:
  /**
   * @param x any floating point number.
   * @return the square of [x] ('x*x').
   */
  static INLINE float sqr( float x );

  /**
   * Swaps the value of [x] and [y]
   * @param x any unsigned int.
   * @param y any unsigned int.
   */
  static INLINE void swap( uint & x, uint & y );
  /**
   * Swaps the value of [x] and [y]
   * @param x any bool.
   * @param y any bool.
   */
  static INLINE void swap( bool & x, bool & y );

  /**
   * Computes the binomials (n r) in the array [t] using Pascal triangle.
   * then t[ r ] = (n r).
   * @param n one less than the dimension of the array [t].
   * @param t the array that will store each (n r), r=0..n
   */
  static void binomials( uint n, uint* t );
  
  
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~KnUtils();

  // ------------------------- Interface --------------------------------------
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
  
  // ------------------------- Datas ------------------------------------------
private:

  // ------------------------- Static datas -----------------------------------
private:
  static uint8 m_bit_count[ 256 ];
  static uint8 m_lsb[ 256 ];

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  inline KnUtils();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  inline KnUtils( const KnUtils & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  inline KnUtils & operator=( const KnUtils & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Macro that returns the least significant bit of [WORD].
 * @param WORD any unsigned integer value (uint8, uint16, uint32, uint64).
 */
#define LSB( WORD ) ( WORD & (unsigned) -WORD )

/**
 * Overloads 'operator<<' for displaying objects of class 'KnUtils'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'KnUtils' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream,
	    const KnUtils & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/KnUtils.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnUtils_h

#undef KnUtils_RECURSES
#endif // else defined(KnUtils_RECURSES)
