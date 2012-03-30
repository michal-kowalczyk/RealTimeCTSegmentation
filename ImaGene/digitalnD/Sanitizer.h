/** @file Sanitizer.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Sanitizer.h
//
// Creation : 2002/08/03
//
// Version : 2002/08/03
//
// Author : JOL
//
// Summary : Header file for files Sanitizer.ih and Sanitizer.cxx
//
// History :
//	2002/08/03 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Sanitizer declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Sanitizer_RECURSES)
#error Recursive header files inclusion detected in Sanitizer.h
#else // defined(Sanitizer_RECURSES)
#define Sanitizer_RECURSES

#if !defined Sanitizer_h
#define Sanitizer_h

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
// class Sanitizer
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'Sanitizer' <p>
 * Aim: To fill with zero unused parts of 'KnCharSet' or 'KnRCellSet'.
 * NB: For internal use only.
 * TODO: not restrict sanitizer to lower coordinate size > _W_NBBITSPERWORD/2.
 * @see KnCharSet,KnRCellSet
 */
class Sanitizer
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~Sanitizer();

  /**
   * Constructor.
   * @param n the dimension of the space.
   * @param size the size for each dimension.
   * @param true_size the power of 2 greater or equal to the size of the same dimension.
   */
  Sanitizer( uint n,
	     const Kn_size* size,
	     const Kn_size* true_size );

  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  INLINE Sanitizer( const Sanitizer & other );

  /**
   * cloning services.
   * @return a copy of 'this'.
   */
  INLINE Sanitizer* clone() const;
  
  // ------------------------- Sanitization services --------------------------
public:

  /**
   * Sanitizes the data [data] given the sizes and true sizes at
   * initialization.
   * @param data the starting word.
   * @param data_hiword the last word.
   */
  void sanitize( _word_ptr data,
		 _word_ptr data_hiword ) const;
  
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
   * Dimension of the space.
   */
  uint m_dim;
  /**
   * Size of the space in each dimension.
   */
  Kn_size* m_size;
  /**
   * Array used when sanitizing a set.
   */
  Kn_size* m_x;
  /**
   * Mask used to set to 0 the words that are partially used.
   */
  _word m_mask_lower_bits;
  /**
   * Equal to_W_WHICHWORD( m_size[ 0 ] );
   */
  Kn_size m_offset_size_x;
  /**
   * Equal to _W_WHICHWORD( m_true_size[ 0 ] );
   */
  Kn_size m_offset_true_size_x;
  /**
   * Array[ k ] equal to _W_WHICHWORD( ( m_true_size[ k ] - m_size[ k ] )
   *                                   << m_shift[ k ] );
   */
  Kn_size* m_gotonext;
  /**
   * When 'true', all lower sizes are powers of 2, and there is no need for 
   * sanitization.
   */
  bool m_need_sanitization;
  
  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE Sanitizer();
private:
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE Sanitizer & operator=( const Sanitizer & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'Sanitizer'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'Sanitizer' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream,
	    const Sanitizer & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE) 
#include "ImaGene/digitalnD/Sanitizer.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Sanitizer_h

#undef Sanitizer_RECURSES
#endif // else defined(Sanitizer_RECURSES)
