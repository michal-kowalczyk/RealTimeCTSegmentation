///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Sanitizer.cxx
//
// Creation : 2002/08/03
//
// Version : 2002/08/03
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
//	2002/08/03 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/Sanitizer.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/Sanitizer.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const Sanitizer_RCS_ID = "@(#)class Sanitizer definition.";

///////////////////////////////////////////////////////////////////////////////
// class Sanitizer
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
Sanitizer::~Sanitizer()
{
  delete[] m_gotonext;
  delete[] m_x;
  delete[] m_size;
}

/**
 * Constructor.
 * @param n the dimension of the space.
 * @param size the size for each dimension.
 * @param true_size the power of 2 greater or equal to the size of the same dimension.
 */
Sanitizer::Sanitizer( uint n,
		      const Kn_size* size,
		      const Kn_size* true_size )
  : m_dim( n ),
    m_size( new Kn_size[ n ] ),
    m_x( new Kn_size[ n ] ),
    m_gotonext( new Kn_size[ n ] )
{
  uint i;
  m_mask_lower_bits = _W_MASKLTBITS( size[ 0 ] );
  m_offset_size_x = _W_WHICHWORD( size[ 0 ] );
  m_offset_true_size_x = _W_WHICHWORD( true_size[ 0 ] );
  Kn_size acc = true_size[ 0 ];
  m_size[ 0 ] = size[ 0 ];
  m_need_sanitization = false;
  for ( i = 1; i < n; ++i )
    {
      m_need_sanitization = m_need_sanitization 
	|| ( size[ i - 1 ] != true_size[ i - 1 ] );
      m_size[ i ] = size[ i ];
      m_gotonext[ i ] = 
	_W_WHICHWORD( ( true_size[ i ] - size[ i ] ) * acc );
      acc *= true_size[ i ];
    }
}



///////////////////////////////////////////////////////////////////////////////
// ------------------------- Sanitization services --------------------------


/**
 * Sanitizes the data [data] given the sizes and true sizes at initialization.
 * @param data the starting word.
 * @param data_hiword the last word.
 */
void
Sanitizer::sanitize( _word_ptr data,
		     _word_ptr data_hiword ) const
{
  if ( ! m_need_sanitization ) return;

  ASSERT_Sanitizer( m_size[ 0 ] > ( _W_NBBITSPERWORD >> 1 ) );
  
  _word_ptr data_next_row;  // next bound.
  uint k;                   // dimension or coordinate.
  uint dm1 = m_dim - 1;
  
  // Set coordinates to 0. Starts from coord 1 since 0 is done implicitely.
  for ( k = 1; k < dm1; ++k )
    m_x[ k ] = 0;

  // For all coordinates
  while ( true ) 
    {
      // take care of x-coordinate (or 0).
      data_next_row = data + m_offset_true_size_x;
      data += m_offset_size_x;
      if ( m_mask_lower_bits )  // partially used word to clear.
	{
	  *(data++) &= m_mask_lower_bits; // partially used word.
	  if (data > data_hiword ) break; // If these are the last cells
	}
      else // If these are the last cells
	if ( data >= data_hiword )
	  break;
      
      while ( data < data_next_row ) // remaining words, if any.
	*(data++) = 0;
      
      for ( k = 1; k < dm1; ++k )
	{
	  m_x[ k ]++;
	  if ( m_x[ k ] != m_size[ k ] )
	      break;
	  else
	    {
	      // Fills with zero.
	      data_next_row = data + m_gotonext[ k ];
	      while ( data < data_next_row ) *(data++) = 0;
	      m_x[ k ] = 0;
	      // go to next coordinate.
	    }
	}
    }
  // while ( true )
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
Sanitizer::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Sanitizer]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
Sanitizer::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
