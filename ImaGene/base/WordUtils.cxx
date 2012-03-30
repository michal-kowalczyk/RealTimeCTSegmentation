///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : WordUtils.cxx
//
// Creation : 2008/06/27
//
// Version : 2008/06/27
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
//	2008/06/27 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/base/WordUtils.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/base/WordUtils.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const WordUtils_RCS_ID = "@(#)class WordUtils definition.";



///////////////////////////////////////////////////////////////////////////////
// class WordUtils
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::WordUtils::~WordUtils()
{
}


/**
 * Constructor.
 * The alphabet defines the lexicographic order.
 *
 * @param zero the first letter of the alphabet.
 * @param one the second letter of the alphabet.
 */
ImaGene::WordUtils::WordUtils( char zero, char one )
{
  alph[ 0 ] = zero;
  alph[ 1 ] = one;
}


///////////////////////////////////////////////////////////////////////////////
// ----------------------- word services ----------------------------------


/**
 * @param w a word
 * @param s the starting index in [w].
 * @param e the index after the end in [w].
 * @return 'true' if the [w] is in the alphabet.
 */
bool 
ImaGene::WordUtils::isInAlphabet( const std::string & w, 
				  index_t s, index_t e ) const
{
  for ( ; s < e; ++s )
    if ( ( w[ s ] != alph[ 0 ] ) || ( w[ s ] != alph[ 1 ] ) )
      return false;
  return true;
}

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
 * @param e the index after the end in [w].
 */
void 
ImaGene::WordUtils::firstLyndonFactor( size_t & len, size_t & nb,
				       const std::string & w, 
				       index_t s, index_t e ) const
{
  index_t i = s;
  index_t j = s+1;
  while ( ( j < e ) && ( lessOrEqual( w[ i ], w[ j ] ) ) )
    {
      if ( equal( w[ i ], w[ j ] ) )
	++i;
      else 
	i = s;
      ++j;
    }
  len = (size_t) j - i;
  nb = ( (size_t) ( j - s ) ) / len;
}


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
 * @param e the index after the end in [w].
 */
void 
ImaGene::WordUtils::firstLyndonFactorMod( size_t & len, size_t & nb,
					  const std::string & w, 
					  index_t s, index_t e ) const
{
  size_t modulo = w.size();
  Mathutils::ModuloComputer mc( modulo );
  index_t i = s;
  index_t j = mc.next( s );
  while ( ( j != e ) && ( lessOrEqual( w[ i ], w[ j ] ) ) )
    {
      if ( equal( w[ i ], w[ j ] ) )
	mc.increment( i );
      else 
	i = s;
      mc.increment( j );
    }
  len = j >= i ? (size_t) ( j - i )
    : (size_t) ( j + modulo - i );
  nb = ( (size_t) ( ( j + modulo - s ) % modulo ) ) / len;
}

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
ImaGene::WordUtils::size_t 
ImaGene::WordUtils::count( const std::string & w, 
			   char c, 
			   index_t s, index_t e ) const
{
  size_t nb = 0;
  for ( ; s < e; ++s )
    if ( w[ s ] == c ) ++nb;
  return nb;
}

/**
 * Tells if a subword is Christoffel (upper-Christoffel).
 * An horizontal or vertical word is not an upper Christoffel word.
 *
 * @param w a word
 * @param s the starting index in [w].
 * @param e the index after the end in [w].
 * @return 'true' if w[s:e-1] is a Christoffel word.
 */
bool 
ImaGene::WordUtils::isChristoffel( const std::string & w,
				   index_t s, index_t e ) const
{
  size_t a = count( w, alph[ 1 ], s, e );
  index_t i = s;
  size_t r = a;
  size_t n = e - s;
  size_t m = ( s + e ) / 2;
//   cout << endl << "-----------------------------------------" << endl;
//   cout << "SW=" << w.substr( s, e-s ) << " e=" << e << ", s=" << s << endl;
//   cout << "[a/n=" << a << "/" << n << "] ";
  --e;
  bool rejected = ! ( ( w[s] == alph[ 0 ] ) && (w[e] == alph[ 1 ] ) );
  ++i; --e;
  while ( ( ! rejected ) && ( i < m ) ) {
    if ( w[ i ] != w[ e ] ) rejected = true;
    else {
      size_t rp = ( r + a ) % n;
      //      cout << "[" << r << "," << rp << "]";
      if ( r < rp ) 
	rejected = w[ i ] != alph[ 0 ];
      else
	rejected = w[ i ] != alph[ 1 ];
      r = rp;
    }
    ++i; --e;
  }
  return ! rejected;
}


/**
 * Tells if a subword is Christoffel (upper-Christoffel) in a cyclic word.
 * An horizontal or vertical word is not an upper Christoffel word.
 *
 * @param w a word
 * @param s the starting index in [w].
 * @param e the index after the end in [w] (e and s are arbitrary).
 * @return 'true' if w[s:e-1] is a Christoffel word.
 */
bool 
ImaGene::WordUtils::isChristoffelMod( const std::string & w,
				      index_t s, index_t e ) const
{
  size_t modulo = w.size();
  Mathutils::ModuloComputer mc( modulo );

  size_t a = s <= e ? count( w, alph[ 1 ], s, e )
    : count( w, alph[ 1 ], s, modulo ) + count( w, alph[ 1 ], 0, e );
  index_t i = s;
  size_t r = a;
  size_t n = s <= e ? e - s
    : e + modulo - s;
  size_t m = ( s + n / 2 ) % modulo;
//   cout << endl << "-----------------------------------------" << endl;
//   cout << "SW=" << w.substr( s, e-s ) << " e=" << e << ", s=" << s << endl;
//   cout << "[a/n=" << a << "/" << n << "] ";
  mc.decrement( e );
  bool rejected = ! ( ( w[s] == alph[ 0 ] ) && (w[e] == alph[ 1 ] ) );
  mc.increment( i );
  mc.decrement( e );
  while ( ( ! rejected ) && ( i != m ) ) {
    if ( w[ i ] != w[ e ] ) rejected = true;
    else {
      size_t rp = ( r + a ) % n;
      //      cout << "[" << r << "," << rp << "]";
      if ( r < rp ) 
	rejected = w[ i ] != alph[ 0 ];
      else
	rejected = w[ i ] != alph[ 1 ];
      r = rp;
    }
    mc.increment( i );
    mc.decrement( e );
  }
  return ! rejected;
}

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
bool 
ImaGene::WordUtils::isULConvex( const std::string & w,
				index_t s, index_t e ) const
{
  index_t index = s;
  bool rejected = false;
  size_t l1; 
  size_t n1;
  while ( ( ! rejected ) && ( index < e ) ) {
    firstLyndonFactor( l1, n1, w, index, e );
    // cout << endl << "Lyndon=(" << l1 << "," << n1 << ")";
    rejected = ! isChristoffel( w, index, index + l1 );
    index += n1*l1;
  }
  if ( rejected )
    cerr << "Rejected between " << index - n1*l1 << " " << index << endl;
  return ! rejected;

}


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
bool 
ImaGene::WordUtils::isULConvexMod( const std::string & w,
				   index_t s, index_t e ) const
{
  index_t index = s;
  bool rejected = false;
  size_t l1; 
  size_t n1;
  while ( ( ! rejected ) && ( index != e ) ) {
    firstLyndonFactorMod( l1, n1, w, index, e );
    // cout << endl << "Lyndon=(" << l1 << "," << n1 << ")";
    rejected = ! isChristoffelMod( w, index, index + l1 );
    index = ( index + n1*l1 ) % w.size();
  }
  if ( rejected )
    cerr << "Rejected between " << index - n1*l1 << " " << index << endl;
  return ! rejected;

}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::WordUtils::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[WordUtils]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::WordUtils::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
