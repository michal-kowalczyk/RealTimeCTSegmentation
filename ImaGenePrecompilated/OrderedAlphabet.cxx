///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : OrderedAlphabet.cxx
//
// Creation : 2009/06/24
//
// Version : 2009/06/24
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
//	2009/06/24 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <sstream>
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/base/OrderedAlphabet.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/base/OrderedAlphabet.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const OrderedAlphabet_RCS_ID = "@(#)class OrderedAlphabet definition.";



///////////////////////////////////////////////////////////////////////////////
// class OrderedAlphabet
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::OrderedAlphabet::~OrderedAlphabet()
{
  if ( m_order != 0 )
    delete[ ] m_order;
}


/**
 * @return the current ordered alphabet.
 */
string
ImaGene::OrderedAlphabet::orderedAlphabet() const
{
  char tbl[ m_nb + 1];
  for ( uint i = 0; i < m_nb; ++i )
    {
      tbl[ m_order[ i ] ] = m_first + i;
    }
  tbl[ m_nb ] = '\0';
  string s( tbl );
  return s;
}

/**
 * Shift a0 < a1 < ... < an to a1 < ... < an < a0
 */
void
ImaGene::OrderedAlphabet::shiftLeft()
{
  uint* p = m_order;
  uint* q = m_order + m_nb;
  while ( p != q )
    {
      uint k = *p;
      *p = ( k == 0 ) ? m_nb - 1 : k - 1; 
      ++p;
    }
}

/**
 * Shift a0 < a1 < ... < an to an < a0 < ... < an-1
 */
void 
ImaGene::OrderedAlphabet::shiftRight()
{
  uint* p = m_order;
  uint* q = m_order + m_nb;
  while ( p != q )
    {
      uint k = *p + 1;
      *p = ( k == m_nb ) ? 0 : k; 
      ++p;
    }
}

/**
 * Reverse the order a0 < a1 < ... < an to an < ... < a1 < a0
 */
void 
ImaGene::OrderedAlphabet::reverse()
{
  uint* p = m_order;
  uint* q = m_order + m_nb;
  while ( p != q )
    {
      *p = m_nb - 1 - *p;
      ++p;
    }
}

/**
 * Reverse the order a0 < a1 < ... < an to a3 < a2 < a1 < a0 < an < ... 
 */
void 
ImaGene::OrderedAlphabet::reverseAround12()
{
  uint* p = m_order;
  uint* q = m_order + m_nb;
  while ( p != q )
    {
      *p = ( m_nb + 3 - *p ) % m_nb;
      ++p;
    }
}




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
void 
ImaGene::OrderedAlphabet::firstLyndonFactor
( size_t & len, size_t & nb,
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
void
ImaGene::OrderedAlphabet::firstLyndonFactorMod
( size_t & len, size_t & nb,
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
bool
ImaGene::OrderedAlphabet::duvalPP
( size_t & len, size_t & nb,
  const std::string & w, 
  index_t s, index_t e ) const
{
  ASSERT_OrderedAlphabet( ( order( w[ s ] ) == 1 )
			  || ( order( w[ s ] ) == 2 ) );
  index_t i = s;
  index_t j = s+1;
  uint p = 1;
  uint q = 2;
  while ( ( j < e ) && ( lessOrEqual( w[ i ], w[ j ] ) ) )
    {
      // cerr << "i=" << i << " j=" << j << " p=" << p << " q=" << q << endl;
      if ( equal( w[ i ], w[ j ] ) )
	{
	  if ( j + 1 == s + q )
	    q += p;
	  ++i;
	}
      else
	{
	  if ( ( j + 1 != s + q ) || ( order ( w[ j ] ) != 2 ) )
	    {
	      len = j; nb = 0;
	      return false;
	    }
	  uint tmp = p; 
	  p = q;
	  q += q - tmp;
	  i = s;
	}
      ++j;
    }
  len = (size_t) j - i;
  nb = ( (size_t) ( j - s ) ) / len;
  return true;
}


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
 *
 * @param s the starting index in [w].
 * @param e the index after the end in [w] (s and e arbitrary).
 */
bool 
ImaGene::OrderedAlphabet::duvalPPMod( size_t & len, size_t & nb,
				       const std::string & w, 
				       index_t s, index_t e ) const
{
  ASSERT_OrderedAlphabet( ( order( w[ s ] ) == 1 )
			  || ( order( w[ s ] ) == 2 ) );
  size_t modulo = w.size();
  Mathutils::ModuloComputer mc( modulo );
  index_t i = s;
  index_t j = mc.next( s );
  uint p = 1;
  uint q = 2;
  while ( ( j != e ) && ( lessOrEqual( w[ i ], w[ j ] ) ) )
    {
      // cerr << "i=" << i << " j=" << j << " p=" << p << " q=" << q << endl;
      if ( equal( w[ i ], w[ j ] ) )
	{
	  if ( j == mc.cast( s + q - 1 ) )
	    q += p;
	  mc.increment( i );
	}
      else 
	{
	  if ( ( j != mc.cast( s + q - 1 ) ) || ( order ( w[ j ] ) != 2 ) )
	    {
	      len = j; nb = 0;
	      return false;
	    }
	  uint tmp = p; 
	  p = q;
	  q += q - tmp;
	  i = s;
	}
      mc.increment( j );
    }
  len = j >= i ? (size_t) ( j - i )
    : (size_t) ( j + modulo - i );
  nb = ( (size_t) ( ( j + modulo - s ) % modulo ) ) / len;
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::OrderedAlphabet::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[OrderedAlphabet]";
  that_stream << " " << orderedAlphabet() << endl;
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::OrderedAlphabet::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------- Static services ------------------------------

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
ImaGene::OrderedAlphabet::size_t
ImaGene::OrderedAlphabet::nextEdge( size_t & nb_a1,
				    size_t & nb_a2,
				    std::string & w,
				    index_t & s,
				    bool & cvx )
{
  Mathutils::ModuloComputer mc( w.size() );
  size_t l;
  size_t len;
  size_t nb;
  bool inC = duvalPPMod( len, nb, w, s, s );
  if ( ! inC ) 
    // case : change of convexity
    {
//       cerr << "Convexity change" <<  orderedAlphabet() ;
//       cerr << " (" << w[ len ] << "==" << letter( 2 ) << ")";
      // JOL : temporary change of letter w[ s ]
      char tmp = w[ s ];
      index_t tmp_s = s;
      w[ s ] = letter( 2 ); // a3
      this->reverseAround12();
      cvx = ! cvx;
//       cerr << " => " << orderedAlphabet() << endl;
      l = nextEdge( nb_a1, nb_a2, w, s, cvx );
      // JOL : former letter is put back in string.
      w[ tmp_s ] = tmp;
    }
  else if ( ( len == 1 ) && ( order( w[ s ] ) == 1 ) ) 
    // case u=a1 => Quadrant change
    {
//       cerr << "Quadrant change " << orderedAlphabet() ;
      this->shiftRight();
//       cerr << " => " << orderedAlphabet() << endl;
      s = mc.cast( s + nb );
      nb_a1 = 0;
      nb_a2 = nb - 1;
      l = nb;
    }
  else 
    { // standard (convex) case.
//       cerr << "standard (convex) case " << orderedAlphabet() << endl;
      l = len * nb;
      char a2 = letter( 2 ); 
      nb_a1 = len;
      nb_a2 = 0;
      index_t ss = s;
      s = mc.cast( s + l );
      while ( len != 0 )
	{
	  if ( w[ ss ] == a2 ) ++nb_a2;
	  mc.increment( ss );
	  --len;
	}
      nb_a1 -= nb_a2;
      nb_a1 *= nb;
      nb_a2 *= nb;
    }
  return l;
}
				    

/**
 * Test the class. Useful for non-regression tests.
 *
 * @param args may give some contextual arguments (like trace, debugging)
 * @return the number of failed tests.
 */
uint
ImaGene::OrderedAlphabet::testClass( const Arguments & args )
{
  uint trace = args.getOption( "-trace" )->getIntValue( 0 );
  uint nb_ok = 0;
  uint nb_ko = 0;
  OrderedAlphabet A( '0', 4 );
  string w1 = "01101010100101001010001";
  string a1= "(011)^1(01)^3(00101)^2(0001)^1";
  if ( A.checkFLF( a1, w1, trace ) ) nb_ok++;
  else                               nb_ko++;
  w1 = "01232112232232103220123210120";
  a1 = "(0123211223223210322)^1(012321)^1(012)^1(0)^1";
  if ( A.checkFLF( a1, w1, trace ) ) nb_ok++;
  else                               nb_ko++;
  w1 = "1112111211211121121112111211211121121111";
  a1 = "C(111211121121112112)^2";
  if ( A.checkDuvalPP( a1, w1, trace ) ) nb_ok++;
  else                                   nb_ko++;
  w1 = "1112111211211121121121111211211121121111";
  a1 = "NC(20)";
  if ( A.checkDuvalPP( a1, w1, trace ) ) nb_ok++;
  else                                   nb_ko++;
  w1 = "2111211211121121111111211121121112112111";
  a1 = "C(111211121121112112)^2";
  if ( A.checkDuvalPPMod( a1, w1, 19, trace ) ) nb_ok++;
  else                                          nb_ko++;
  if ( trace >= 1 )
    {
      cerr << "[test_OrderedAlphabet::testClass] passed " << nb_ok << "/" 
	   << ( nb_ok + nb_ko ) << endl;
    }
  return nb_ko;
}

/**
 * Checks if the Lyndon factorization works.
 *
 * @param input the input word.
 * @param output the expected output word as (w1)^l1...(wn)^ln.
 * @param trace the trace level (0:silent, 3:verbose)
 * @return 'true' if the test was ok, 'false' otherwise.
 */
bool
ImaGene::OrderedAlphabet::checkFLF( const string & output, 
				    const string & input,
				    uint trace ) const
{
  string w1 = input;
  string a1 = output;
  stringstream s1;
  uint len;
  uint nb;
  uint s = 0;
  uint e = w1.size();
  do 
    {
      firstLyndonFactor( len, nb, w1, s, e );
      s1 << "(" << w1.substr( s, len ) << ")^" << nb;
      s += len*nb;
    }
  while ( s < e );
  if ( trace >= 3 )
    {
      cerr << "[OrderedAlphabet::checkFLF] input =" << input << endl;
      cerr << "[OrderedAlphabet::checkFLF] expect=" << output << endl;
      cerr << "[OrderedAlphabet::checkFLF] comput=" << s1.str() << endl;
    }
  return s1.str() == a1;
}

/**
 * Checks if Duval++ works.
 *
 * @param input the input word.
 * @param output the expected output word as "NC" or "C(w1)^l1"
 * @param trace the trace level (0:silent, 3:verbose)
 * @return 'true' if the test was ok, 'false' otherwise.
 */
bool
ImaGene::OrderedAlphabet::checkDuvalPP( const string & output, 
					const string & input,
					uint trace ) const
{
  size_t len;
  size_t nb;
  bool christoffel = duvalPP( len, nb, input, 0, input.size() );
  stringstream s1;
  if ( christoffel ) 
    s1 << "C(" << input.substr( 0, len ) << ")^" << nb;
  else s1 << "NC(" << len << ")";
  if ( trace >= 3 )
    {
      cerr << "[OrderedAlphabet::checkDuvalPP] input =" << input << endl;
      cerr << "[OrderedAlphabet::checkDuvalPP] expect=" << output << endl;
      cerr << "[OrderedAlphabet::checkDuvalPP] comput=" << s1.str() << endl;
    }
  return s1.str() == output;
}

/**
 * Checks if Duval++ modulo works.
 *
 * @param input the input word.
 * @param output the expected output word as "NC" or "C(w1)^l1"
 * @param s starting index.
 * @param trace the trace level (0:silent, 3:verbose)
 * @return 'true' if the test was ok, 'false' otherwise.
 */
bool
ImaGene::OrderedAlphabet::checkDuvalPPMod( const string & output, 
					   const string & input,
					   index_t s,
					   uint trace ) const
{
  size_t len;
  size_t nb;
  bool christoffel = duvalPPMod( len, nb, input, s, s );
  stringstream s1;
  if ( christoffel ) 
    {
      s1 << "C(";
      for ( uint i = 0; i < len; ++i )
	{
	  s1 << input[ s ];
	  s = s + 1 % input.size();
	}
      s1  << ")^" << nb;
    }
  else s1 << "NC(" << len << ")";
  if ( trace >= 3 )
    {
      cerr << "[OrderedAlphabet::checkDuvalPPMod] input =" << input << endl;
      cerr << "[OrderedAlphabet::checkDuvalPPMod] expect=" << output << endl;
      cerr << "[OrderedAlphabet::checkDuvalPPMod] comput=" << s1.str() << endl;
    }
  return s1.str() == output;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
