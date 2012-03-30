/** @file Bitset1.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Bitset1.h
//
// Creation : 2002/08/01
//
// Version : 2002/08/01
//
// Author : JOL
//
// Summary : Header file for files Bitset1.ih and Bitset1.cxx
//
// History :
//	2002/08/01 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Bitset1 declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Bitset1_RECURSES)
#error Recursive header files inclusion detected in Bitset1.h
#else // defined(Bitset1_RECURSES)
#define Bitset1_RECURSES

#if !defined Bitset1_h
#define Bitset1_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/digitalnD/KnTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class Bitset1
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'Bitset1' <p>
 * Aim: To represent a set of bits of variable size.
 * The bitset is decomposed into words.
 */
class Bitset1
{

  // ------------------------- Helper classes ---------------------------------
public:
 
  /**
   * Internal class to represent a reference to a bit in a bitset.
   */
  class reference;
  /**
   * Internal class to represent a reference to a bit in a bitset. Is a friend
   * of Bitset1.
   */
  friend class reference;

  /**
   * Internal class to represent a reference to a bit in a bitset.
   */
  class reference {
    friend class Bitset1;

  // ------------------------- Datas ------------------------------------------
  private:
    /**
     * A reference references a bit that is part of word.
     */
    _word_ptr m_word;
    /**
     * This is the position of the reference bit in its word.
     */
    uint m_pos_in_word;

    // ------------------------- Standard services ----------------------------
  private:
    /**
     * Default constructor. Left undefined.
     */
    reference();

    /**
     * Constructor.
     * Private to be used only by a 'Bitset1'.
     * @param b any bitset.
     * @param pos a position in the bitset.
     */
    reference( Bitset1 & b, Kn_size pos ) {
      m_word = & b.getWord( pos );
      m_pos_in_word = _W_WHICHBIT( pos );
    }

    // ------------------------- Standard services ----------------------------
  public:
    /**
     * Default destructor. Does nothing.
     */
    ~reference() {}

    /**
     * Assignment from boolean-value, such that <code>b[i] = __x;</code>.
     * @param x any boolean
     * @return a reference on 'this'.
     */
    reference& operator=( bool x )
    {
      if ( x ) *m_word |= _W_MASKBIT( m_pos_in_word );
      else     *m_word &= ~_W_MASKBIT( m_pos_in_word );
      return *this;
    }

    /**
     * Assignment from other reference, eg <code>b[i] = b[ j ];</code>.
     * @param j any reference to a bit (not necessarily from the same bitset).
     * @return a reference on 'this'.
     */
    reference& operator=( const reference & j )
    {
      if ( *( j.m_word ) & _W_MASKBIT( j.m_pos_in_word) )
        *m_word |= _W_MASKBIT( m_pos_in_word );
      else
        *m_word &= ~_W_MASKBIT( m_pos_in_word );
      return *this;
    }

    /**
     * Returns the flipped referenced bit.
     * @return 'true' if the bit was reset, 'false' otherwise.
     */
    bool operator~() const 
    { 
      return (*(m_word) & _W_MASKBIT( m_pos_in_word ) ) == 0;
    }
    
    /**
     * Conversion to boolean value.
     * Returns the value of the bit as a boolean, for eg <code> x = b[ i ]
     * </code>
     * @return 'true' if the bit was set, 'false' otherwise.
     */
    operator bool() const 
    { 
      return (*(m_word) & _W_MASKBIT( m_pos_in_word ) ) != 0;
    }

    /**
     * Flips the referenced bit, for eg <code> b[i].flip(); </code>
     * @return a reference to 'this'.
     */
    reference& flip()
    {
      *m_word ^= _W_MASKBIT( m_pos_in_word );
      return *this;
    }
  };



  // ------------------------- Static services ------------------------------
public:

  /**
   * Resets the words between [dst_beg] and [dst_end].
   * @param dst_beg a pointer on the first word to reset.
   * @param dst_end a pointer on the word after the last one to reset.
   */
  static INLINE void doWordReset( _word_ptr dst_beg, 
				  _word_ptr dst_end );

  /**
   * Sets the words between [dst_beg] and [dst_end].
   * @param dst_beg a pointer on the first word to set.
   * @param dst_end a pointer on the word after the last one to set.
   */
  static INLINE void doWordSet( _word_ptr dst_beg,
				_word_ptr dst_end );
  /**
   * Flips the words between [dst_beg] and [dst_end].
   * @param dst_beg a pointer on the first word to flip.
   * @param dst_end a pointer on the word after the last one to flip.
   */
  static INLINE void doWordFlip( _word_ptr dst_beg,
				 _word_ptr dst_end );
  /**
   * Performs the copy of the words between [src_beg] and [src_end] into 
   * [dst_beg] and following words.
   * @param src_beg a pointer on the first word which to perform copy with.
   * @param src_end a pointer on the word after the last one which to perform copy with.
   * @param dst_beg a pointer on the first word which holds the result of the copy operation.
   */
  static INLINE void doWordCopy( _ro_word_ptr src_beg,
				 _ro_word_ptr src_end,
				 _word_ptr dst_beg );

  /**
   * Performs the copy of the flipped words between [src_beg] and [src_end]
   * into [dst_beg] and following words.
   * @param src_beg a pointer on the first word which to perform flipped copy with.
   * @param src_end a pointer on the word after the last one which to perform flipped copy with.
   * @param dst_beg a pointer on the first word which holds the result of the flipped copy operation.
   */
  static INLINE void doWordCopyFlipped( _ro_word_ptr src_beg,
					_ro_word_ptr src_end,
					_word_ptr dst_beg );

  /**
   * Performs the bit AND operation with the words between [src_beg] and 
   * [src_end] into [dst_beg] and following words.
   * @param src_beg a pointer on the first word which to perform and with.
   * @param src_end a pointer on the word after the last one which to perform and with.
   * @param dst_beg a pointer on the first word which holds the result of the and operation.
   */
  static INLINE void doWordAnd( _ro_word_ptr src_beg,
				_ro_word_ptr src_end,
				_word_ptr dst_beg );

  /**
   * Performs the bit OR operation with the words between [src_beg] and 
   * [src_end] into [dst_beg] and following words.
   * @param src_beg a pointer on the first word which to perform or with.
   * @param src_end a pointer on the word after the last one which to perform or with.
   * @param dst_beg a pointer on the first word which holds the result of the or operation.
   */
  static INLINE void doWordOr( _ro_word_ptr src_beg,
			       _ro_word_ptr src_end,
			       _word_ptr dst_beg );

  /**
   * Performs the bit XOR operation with the words between [src_beg] and 
   * [src_end] into [dst_beg] and following words.
   * @param src_beg a pointer on the first word which to perform xor with.
   * @param src_end a pointer on the word after the last one which to perform xor with.
   * @param dst_beg a pointer on the first word which holds the result of the xor operation.
   */
  static INLINE void doWordXor( _ro_word_ptr src_beg,
				_ro_word_ptr src_end,
				_word_ptr dst_beg );


  // ------------------------- Static comparison services ---------------------
public:

  /**
   * Performs the equality comparison with the words between [src1_beg] and 
   * [src1_end] and the words starting from [src2_beg] and following words.
   * @param src1_beg a pointer on the first word of the first series of data.
   * @param src1_end a pointer on the word after the last one of the first series of data.
   * @param src2_beg a pointer on the first word of the second series of data.
   */
  static INLINE bool doWordEq( _ro_word_ptr src1_beg,
			       _ro_word_ptr src1_end,
			       _ro_word_ptr src2_beg );


  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~Bitset1();

  /**
   * Constructor. The set is left uninitalized.
   * @param size the number of bits to store in the set.
   */
  Bitset1( Kn_size size );
  /**
   * Constructor. The set is left uninitalized.
   * @param size the number of bits to store in the set.
   * @param initial_value if 0 (false) the set is filled with 0, if 1 (true) the set is filled with 1.
   */
  Bitset1( Kn_size size, bool initial_value );

  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  Bitset1( const Bitset1 & other );

  /**
   * Constructor which clones the complement of [other].
   * @param other the object to clone and flip
   * @param to_flip 'true' copy and flip, 'false' same as copy constructor.
   */
  Bitset1( const Bitset1 & other, bool to_flip );

  /**
     Resets the set to zero bits.
  */
  void clear();

  /**
   * @return the number of bits stored in the bitset.
   */
  INLINE Kn_size size() const;

  /**
   * Resizes the bitset. New elements are left uninitialized.
   * @param nsize the new size.
   */
  void resize( Kn_size nsize );

  /**
   * Resizes the bitset. New elements are initialized with [val].
   * @param nsize the new size.
   * @param val the value to fill the new elements (0 is 0, != 0 is 1).
   */
  void resize( Kn_size nsize, int val );

  /**
   * @return the number of bits set to 1 in the bitset.
   */
  Kn_size nbElements() const;


  // ------------------------- Access services -----------------------------
public:

  /**
   * @return the pointer of the first word of data.
   * NB: the data are valid from 'data()' till 'data()+nbWord()'.
   */
  INLINE _word_ptr data() const;

  /**
   * @return the number of used words.
   */
  INLINE Kn_size nbWords() const;
  
  
  // ------------------------- Bit access services ----------------------------
public:

  /**
   * Non-const access to bit, eg <code> b[ i ] = true </code>
   * @param pos a position (from 0 to size()-1)
   * @return a reference on the bit at position [pos].
   */
  INLINE reference operator[]( Kn_size pos );
  /**
   * Const access to bit, eg <code> cout << b[ i ] </code>
   * @param pos a position (from 0 to size()-1)
   * @return a boolean 'true' for 1, 'false' for 0.
   */
  INLINE bool operator[]( Kn_size pos ) const;

  /**
   * Sets all bits.
   * @return a reference to 'this'.
   */
  INLINE Bitset1 & set();

  /**
   * Sets the bit at position [pos].
   * @param pos any position (between 0 and size()-1).
   * @return a reference to 'this'.
   */
  INLINE Bitset1 & set( Kn_size pos );

  /**
   * Sets the bit to [val] at position [pos].
   * @param pos any position (between 0 and size()-1).
   * @param val a value of 0 is 0, a value != 0 is 1.
   * @return a reference to 'this'.
   */
  INLINE Bitset1 & set( Kn_size pos, int val );

  /**
   * Resets all bits.
   * @return a reference to 'this'.
   */
  INLINE Bitset1 & reset();

  /**
   * Resets the bit at position [pos].
   * @param pos any position (between 0 and size()-1).
   * @return a reference to 'this'.
   */
  INLINE Bitset1 & reset( Kn_size pos );

  /**
   * Flips all bits.
   * @return a reference to 'this'.
   */
  INLINE Bitset1 & flip();

  /**
   * Flips the bit at position [pos].
   * @param pos any position (between 0 and size()-1).
   * @return a reference to 'this'.
   */
  INLINE Bitset1 & flip( Kn_size pos );

  /**
   * Returns the value of the bit at position [pos].
   * @param pos any position (between 0 and size()-1).
   * @return 'true' if this bit was set, 'false' otherwise.
   */
  INLINE bool test( Kn_size pos ) const;


  // ------------------------- Operations -------------------------------------
public:

  /**
   * Assignment. Sizes need not to be identical.
   * @param other the object to clone.
   * @return a reference on 'this'.
   */
  INLINE Bitset1 & operator=( const Bitset1 & other );
  /**
   * Assignment with AND operation. Sizes must be identical.
   * @param other the object to do AND with.
   * @return a reference on 'this'.
   */
  INLINE Bitset1 & operator&=( const Bitset1 & other );
  /**
   * Assignment with OR operation. Sizes must be identical.
   * @param other the object to do OR with.
   * @return a reference on 'this'.
   */
  INLINE Bitset1 & operator|=( const Bitset1 & other );
  /**
   * Assignment with XOR operation. Sizes must be identical.
   * @param other the object to do XOR with.
   * @return a reference on 'this'.
   */
  INLINE Bitset1 & operator^=( const Bitset1 & other );


  // ------------------------- Comparisons ------------------------------------
public:

  /**
   * Equality comparison. Sizes must be identical.
   * @param other the object to compare with.
   * @return a reference on 'this'.
   */
  INLINE bool operator==( const Bitset1 & other ) const;


  // ------------------------- Scanning services ------------------------------
public:

  /**
   * find first "on" bit.
   * @return the position of the first "on" bit or [not_found] if no bit was "on:.
   */
  Kn_size findFirst( Kn_size not_found ) const;
  /**
   * find the next "on" bit that follows "prev"
   * @return the position of the next "on" bit or, if this was the last one, return [not_found].
   */
  Kn_size findNext( Kn_size prev, Kn_size not_found ) const;

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
  /**
   * Pointer on the first word of data.
   */
  _word_ptr m_data;
  /** 
   * Number of used words.
   */
  Kn_size m_nb_words;
  /**
   * Number of bits allocated ( m_size < ( m_nb_words / 32 )+1 )
   */
  Kn_size m_size;
  /** 
   * Number of allocated words ( m_nb_alloc_words >= m_nb_words )
   */
  Kn_size m_nb_alloc_words;

  

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE Bitset1();
private:
  
  // ------------------------- Internals --------------------------------------
private:

  /**
   * Non-const accessor to word.
   * @param pos any position (between 0 and size()-1).
   * @return the reference of the word at position [pos].
   */
  INLINE _word & getWord( Kn_size pos );
  /**
   * Const accessor to word.
   * @param pos any position (between 0 and size()-1).
   * @return the value of the word at position [pos].
   */
  INLINE _word getWord( Kn_size pos ) const;

  /**
   * Sets the bit at position [pos].
   * May cause segmentation fault if out of range.
   * @param pos any position (between 0 and size()-1).
   */
  INLINE void uncheckedSet( Kn_size pos );
  /**
   * Sets the bit to [val] at position [pos].
   * May cause segmentation fault if out of range.
   * @param pos any position (between 0 and size()-1).
   * @param val a value of 0 is 0, a value != 0 is 1.
   */
  INLINE void uncheckedSet( Kn_size pos, int val );
  /**
   * Resets the bit at position [pos].
   * May cause segmentation fault if out of range.
   * @param pos any position (between 0 and size()-1).
   */
  INLINE void uncheckedReset( Kn_size pos );
  /**
   * Flips the bit at position [pos].
   * May cause segmentation fault if out of range.
   * @param pos any position (between 0 and size()-1).
   */
  INLINE void uncheckedFlip( Kn_size pos );
  /**
   * Returns the value of the bit at position [pos].
   * May cause segmentation fault if out of range.
   * @param pos any position (between 0 and size()-1).
   * @return 'true' if this bit was set, 'false' otherwise.
   */
  INLINE bool uncheckedTest( Kn_size pos ) const;
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'Bitset1'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'Bitset1' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, 
	    const Bitset1 & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/Bitset1.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Bitset1_h

#undef Bitset1_RECURSES
#endif // else defined(Bitset1_RECURSES)
