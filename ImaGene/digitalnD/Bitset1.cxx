///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Bitset1.cxx
//
// Creation : 2002/08/01
//
// Version : 2002/08/01
//
// Author : Jacques-Olivier Lachaud
//
// email : lachaud@labri.fr
//
// Purpose : ??
//
// Distribution :
//
// Use :
//	??
//
// Todo :
//	O ??
//
// History :
//	2002/08/01 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnUtils.h"
#include "ImaGene/digitalnD/Bitset1.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/Bitset1.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const Bitset1_RCS_ID = "@(#)class Bitset1 definition.";

///////////////////////////////////////////////////////////////////////////////
// class Bitset1
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
Bitset1::~Bitset1()
{
#ifdef DEBUG_Bitset1
  cout << "Bitset1::~Bitset1() size was " << m_size << endl;
#endif // DEBUG_Bitset1

  delete[] m_data;
}

/**
 * Constructor. The set is left uninitalized.
 * @param size the number of bits to store in the set.
 */
Bitset1::Bitset1( Kn_size size )
  : m_data( new _word[ _W_NBWORDS( size ) ] ),
    m_nb_words( _W_NBWORDS( size ) ),
    m_size( size ),
    m_nb_alloc_words( _W_NBWORDS( size ) )
{
  ASSERT_Bitset1( size > 0 );
}


/**
 * Constructor. The set is left uninitalized.
 * @param size the number of bits to store in the set.
 * @param initial_value if 0 (false) the set is filled with 0, if 1 (true) the set is filled with 1.
 */
Bitset1::Bitset1( Kn_size size, bool initial_value )
  : m_data( new _word[ _W_NBWORDS( size ) ] ),
    m_nb_words( _W_NBWORDS( size ) ),
    m_size( size ),
    m_nb_alloc_words( _W_NBWORDS( size ) )
{
  ASSERT_Bitset1( size > 0 );
  if ( initial_value )
    set();
  else
    reset();
}

/**
 * Copy constructor.
 * @param other the object to clone.
 */
Bitset1::Bitset1( const Bitset1 & other )
  : m_data( new _word[ other.m_nb_words ] ),
    m_nb_words( other.m_nb_words ),
    m_size( other.m_size ),
    m_nb_alloc_words( other.m_nb_words )
{
#ifdef DEBUG_Bitset1
  cout << "Bitset1::Bitset1( const Bitset1 & ) size is " << m_size << endl;
#endif //DEBUG_Bitset1

  doWordCopy( other.m_data, other.m_data + m_nb_words, m_data );
}

/**
 * Constructor which clones the complement of [other].
 * @param other the object to clone and flip
 * @param to_flip 'true' copy and flip, 'false' same as copy constructor.
 */
Bitset1::Bitset1( const Bitset1 & other, 
		  bool to_flip )
  : m_data( new _word[ other.m_nb_words ] ),
    m_nb_words( other.m_nb_words ),
    m_size( other.m_size ),
    m_nb_alloc_words( other.m_nb_words )
{
#ifdef DEBUG_Bitset1
  cout << "Bitset1::Bitset1( const Bitset1 &, bool ) size is "
       << m_size << endl;
#endif // DEBUG_Bitset1

  if ( to_flip )
    doWordCopyFlipped( other.m_data, other.m_data + m_nb_words, m_data );
  else
    doWordCopy( other.m_data, other.m_data + m_nb_words, m_data );
}

/**
 * Resizes the bitset. New elements are left uninitialized.
 * @param nsize the new size.
 */
void 
Bitset1::resize( Kn_size nsize )
{
  ASSERT_Bitset1( nsize > 0 );

  Kn_size nnb_words = _W_NBWORDS( nsize );
  if ( nnb_words > m_nb_alloc_words )
    {
      // new space must be allocated.
      _word_ptr ndata = new _word[ nnb_words ];
      doWordCopy( m_data, m_data + m_nb_words, ndata );
      delete[] m_data;
      m_data = ndata;
      m_nb_alloc_words = nnb_words;
    }
  m_size = nsize;
  m_nb_words = nnb_words;
}


/**
 * Resizes the bitset. New elements are initialized with [val].
 * @param nsize the new size.
 * @param val the value to fill the new elements (0 is 0, != 0 is 1).
 */
void
Bitset1::resize( Kn_size nsize, int val )
{
  ASSERT_Bitset1( nsize > 0 );
  
  Kn_size nnb_words = _W_NBWORDS( nsize );
  if ( nnb_words > m_nb_alloc_words )
    {
      // new space must be allocated.
      _word_ptr ndata = new _word[ nnb_words ];
      // and initialized.
      doWordCopy( m_data, m_data + m_nb_words, ndata );
      if ( val )
	doWordSet( ndata + m_nb_words, ndata + nnb_words );
      else
	doWordReset( ndata + m_nb_words, ndata + nnb_words );
      // Old stuff is removed.
      delete[] m_data;
      m_data = ndata;
      m_nb_alloc_words = nnb_words;
    }
  // Now we must be careful on the former last word, which must be initialized
  // correctly.
  _word m = ( 1 << _W_WHICHBIT( m_size ) ) - 1;
  if ( val )
    m_data[ m_nb_words - 1 ] |= ~m;
  else
    m_data[ m_nb_words - 1 ] &= m;
  m_size = nsize;
  m_nb_words = nnb_words;
}


/**
 * @return the number of bits set to 1 in the bitset.
 */
Kn_size 
Bitset1::nbElements() const
{
  Kn_size nb = 0;
  const uint8* byte_ptr = (const uint8*) data();
  const uint8* end_ptr = (const uint8*) ( data() + nbWords() );

  while ( byte_ptr < end_ptr )
    {
      nb += KnUtils::countSetBits( *byte_ptr );
      ++byte_ptr;
    }
  return nb;
}


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Scanning services ------------------------------

/**
 * find first "on" bit.
 * @return the position of the first "on" bit or [not_found] if no bit was "on:.
 */
Kn_size 
Bitset1::findFirst( Kn_size not_found ) const
{
  for ( Kn_size i = 0; i < m_nb_words; ++i )
    {
      _word w = m_data[ i ];
      if ( w != _W_ZERO )
	{
	  // find byte within word
	  for ( uint j = 0; j < sizeof( _word ); ++j )
	    {
	      uint8 b = static_cast<uint8>( w & (~(uint8)0));
	      if ( b )
		return _W_POSFROMWORD( i ) + ( j << 3 )
		  + KnUtils::getLSB( b );
	      w >>= 8;
	    }
	}
    }
  // not found, so return an indication of failure.
  return not_found;
}

/**
 * find the next "on" bit that follows "prev"
 * @return the position of the next "on" bit or, if this was the last one, return [not_found].
 */
Kn_size 
Bitset1::findNext( Kn_size prev, Kn_size not_found ) const
{
  // make bound inclusive
  ++prev;

  // check out of bounds
  if ( prev >= m_size ) return not_found;

  // search first word
  Kn_size i = _W_WHICHWORD( prev );
  _word w = m_data[ i ];

  // mask off bits below bound
  w &= (~ _W_ZERO ) << _W_WHICHBIT( prev );

  if ( w != _W_ZERO ) 
    {
      // find byte within word
      // get first byte into place
      w >>= _W_WHICHBYTE( prev ) * 8;
      for ( uint j = _W_WHICHBYTE( prev ); j < sizeof(_word ); ++j )
	{
	  uint8 b = static_cast<uint8>( w & (~(uint8)0));
	  if ( b )
	    return _W_POSFROMWORD( i ) + ( j << 3 )
	      + KnUtils::getLSB( b );
	  w >>= 8;
	}
    }

  // check subsequent words
  ++i;
  for ( ; i < m_nb_words; ++i )
    {
      _word w = m_data[ i ];
      if ( w != _W_ZERO )
	{
	  // find byte within word
	  for ( uint j = 0; j < sizeof( _word ); ++j )
	    {
	      uint8 b = static_cast<uint8>( w & (~(uint8)0));
	      if ( b )
		return _W_POSFROMWORD( i ) + ( j << 3 )
		  + KnUtils::getLSB( b );
	      w >>= 8;
	    }
	}
    }
  // not found, so return an indication of failure.
  return not_found;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
Bitset1::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Bitset1 size=" << size() 
	      << " words=" << m_nb_words
	      << " alloc=" << m_nb_alloc_words
	      << "]" << endl;
  Kn_size i;
  for ( i = 0; i < m_nb_words; ++i )
    {
      that_stream << KnUtils::sprintDec( i * _W_NBBITSPERWORD, 6, ' ' )
		  << ":"
		  << KnUtils::sprintBin( m_data[ i ], _W_NBBITSPERWORD, '0' )
		  << endl;
    }
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
Bitset1::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

}
