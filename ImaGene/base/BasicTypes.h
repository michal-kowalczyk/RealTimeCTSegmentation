/** @file BasicTypes.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : BasicTypes.h
//
// Creation : 2002/07/20
//
// Version : 2002/07/20
//
// Author : JOL
//
// Summary :
//
// History :
//	2002/07/20 : ?Name? : ?What?
//
// Rcs Id : "@(#)class BasicTypes declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(BasicTypes_RECURSES)
#error Recursive header files inclusion detected in BasicTypes.h
#else // defined(BasicTypes_RECURSES)
/**
 * Useful to avoid recursive inclusion.
 */
#define BasicTypes_RECURSES

#if !defined BasicTypes_h
#define BasicTypes_h

/**
 * Defines the signed integer number represented with 64 bits.
 * @todo the following definition is valid only on Linux/i686 platforms.
 */
typedef long long int64;
/**
 * Defines the signed integer number represented with 32 bits.
 * @todo the following definition is valid only on Linux/i686 platforms.
 */
typedef int int32;
/**
 * Defines the signed integer number represented with 16 bits.
 * @todo the following definition is valid only on Linux/i686 platforms.
 */
typedef short int16;
/**
 * Defines the signed integer number represented with 8 bits.
 * @todo the following definition is valid only on Linux/i686 platforms.
 */
typedef char int8;
/**
 * Defines the generic signed integer number (here 32 bits).
 * @todo the following definition is valid only on Linux/i686 platforms.
 */
typedef unsigned int uint;
/**
 * Defines the unsigned integer number represented with 64 bits.
 * @todo the following definition is valid only on Linux/i686 platforms.
 */
typedef unsigned long long uint64;
/**
 * Defines the unsigned integer number represented with 32 bits.
 * @todo the following definition is valid only on Linux/i686 platforms.
 */
typedef unsigned int uint32;
/**
 * Defines the unsigned integer number represented with 16 bits.
 * @todo the following definition is valid only on Linux/i686 platforms.
 */
typedef unsigned short uint16;
/**
 * Defines the unsigned integer number represented with 8 bits.
 * @todo the following definition is valid only on Linux/i686 platforms.
 */
typedef unsigned char uint8;


/**
 * Words should be of the size of the processor data bus. Useful to speed-up
 * memory access for shorter types (we get several consecutive ones at once).
 */
typedef uint32 _word;
/**
 * Read-write pointer on a word (data can be changed).
 * @see _word
 */
typedef _word* _word_ptr;
/**
 * Read-only pointer on a word (data are constant).
 * @see _word_ptr
 */
typedef const _word* _ro_word_ptr;

/**
 * Defines a bit set to 0
 */
#define _W_ZERO 0
/**
 * Defines a bit set to 1
 */
#define _W_ONE static_cast<_word>(1)
/**
 * Defines the number of bits per word.
 * @see _word
 */
#define _W_NBBITSPERWORD 32
/**
 * Given a number of bits [SIZE], returns the number of words necessary to 
 * store them.
 * @see _word
 */
#define _W_NBWORDS(SIZE) ( ( ( SIZE - 1 ) >> 5 ) + 1 )
/**
 * Given a bit index [POS], returns the bit index location in the corresponding
 * word.
 * @see _word
 */
#define _W_WHICHBIT(POS) ( POS & 0x1f )
/**
 * Given a bit index [POS], returns the byte index where this bit is located.
 * @see _word
 */
#define _W_WHICHBYTE(POS) ( ( POS >> 3 ) & 0x3 )
/**
 * Given a bit index [POS], returns the word index where this bit is located.
 * @see _word
 */
#define _W_WHICHWORD(POS) ( POS >> 5 )
/**
 * Given a word index [WORD], returns the index of the first bit of the word.
 * @see _word
 */
#define _W_POSFROMWORD(WORD) ( WORD << 5 )
/**
 * Given a bit index [POS], returns the mask that selects it in the word.
 * @see _word
 */
#define _W_MASKBIT(POS) ( _W_ONE <<_W_WHICHBIT(POS) )
/**
 * Given a bit index [POS], returns the mask that selects all the bits 
 * strictly inferior to it in the word.
 * @see _word
 */
#define _W_MASKLTBITS(POS) ( _W_MASKBIT(POS) - 1 )
/**
 * Given a bit index [POS], returns the mask that selects all the bits 
 * inferior to it in the word.
 * @see _word
 */
#define _W_MASKLEBITS(POS) ( ( _W_MASKBIT(POS) - 1 ) | _W_MASKBIT(POS) )
/**
 * Given a bit index [POS], returns the mask that selects all the bits 
 * superior to it in the word.
 * @see _word
 */
#define _W_MASKGEBITS(POS) ( ~( _W_MASKBIT(POS) - 1 ) )
/**
 * Given a bit index [POS], returns the mask that selects all the bits 
 * strictly superior to it in the word.
 * @see _word
 */
#define _W_MASKGTBITS(POS) ( ~( _W_MASKBIT(POS) - 1 ) ^ _W_MASKBIT(POS) )


//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined BasicTypes_h

#undef BasicTypes_RECURSES
#endif // else defined(BasicTypes_RECURSES)
