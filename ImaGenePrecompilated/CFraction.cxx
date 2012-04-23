///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : CFraction.cxx
//
// Creation : 2009/10/22
//
// Version : 2009/10/22
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
//	2009/10/22 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/CFraction.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/CFraction.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const CFraction_RCS_ID = "@(#)class CFraction definition.";



///////////////////////////////////////////////////////////////////////////////
// class CFraction
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Initializes a fraction p/q.
 *
 * @pre p <= q
 * @param p the numerator.
 * @param q the denominator.
 * @return the gcd of p and q.
 */
uint
ImaGene::CFraction::init( uint p, uint q )
{
  m_p.clear();
  m_q.clear();
  m_u.clear();
  int k = 0;
  while ( q != 0 )
    {
      uint u = p / q;
      uint r = p - q * u;
      m_u.push_back( u );
      m_p.push_back( u * pk( k - 1 ) + pk( k - 2 ) );
      m_q.push_back( u * qk( k - 1 ) + qk( k - 2 ) );
      p = q;
      q = r;
      ++k;
    }
  return p;
}


///////////////////////////////////////////////////////////////////////////////
// ----------------------- Mediant services --------------------------------

/** 
 * Splits the fraction into two fractions following Berstel's formula:
 * - E(z0) = 0
 * - E(z1) = 0^u1 1
 * - E(z2n)= E(z2n-2) E(z2n-1)^u2n
 * - E(z2n+1)= E(z2n)^u2n+1 E(z2n-1)
 *
 * @param zleft (returns) the left pattern, of the decomposition.
 * @param nleft (returns) the number of times the left pattern is repeated.
 * @param zright (returns) the right pattern, of the decomposition.
 * @param nright (returns) the number of times the right pattern is repeated.
 * @return 'true' if the decomposition was proper (non zero fraction).
 */
bool
ImaGene::CFraction::splitBerstel
( CFraction & zleft, uint & nleft, 
  CFraction & zright, uint & nright ) const
{
  int n = depth();
  if ( n <= 1 ) return false;
  if ( n & 0x1 ) // odd
    {
      zleft.init( pk( n - 1 ), qk( n - 1 ) );
      nleft = uk( n );
      zright.init( pk( n - 2 ), qk( n - 2 ) );
      nright = 1;
    }
  else // even
    {
      zleft.init( pk( n - 2 ), qk( n - 2 ) );
      nleft = 1;
      zright.init( pk( n - 1 ), qk( n - 1 ) );
      nright = uk( n );
    }
  return true;
}

/**
 * Computes the mediant of this fraction and [other]. If one of
 * the two is an ancestor of the other in the Stern-Brocot tree,
 * then the computation is O(1). Otherwise, the continued fraction
 * is recomputed in O(log (p+q)).
 *
 * @param other any other fraction.
 * @return the mediant of 'this' and [other].
 */
ImaGene::CFraction
ImaGene::CFraction::mediant( const CFraction & other ) const
{
  int d = det( other );
  if ( d == 0 ) return *this;
  if ( ( d == 1 ) || ( d == -1 ) )
    {
      if ( depth() > other.depth() )
	{
	  if ( isDirectDescendent( other ) )
	    {
	      cerr << *this << endl
		   << " ==> direct descendent of "
		   << other << endl;
	      CFraction c( *this );
	      --c.m_u.back();
	      int k = c.depth();
	      c.m_u.push_back( 2 );
	      c.m_p[ k ] -= c.m_p[ k - 1 ];
	      c.m_p.push_back( 2 * c.m_p[ k ] + c.m_p[ k - 1 ] );
	      c.m_q[ k ] -= c.m_q[ k - 1 ];
	      c.m_q.push_back( 2 * c.m_q[ k ] + c.m_q[ k - 1 ] );
	      return c;
	    }
	  else if ( isIndirectDescendent( other ) )
	    {
 	      cerr << *this << endl
		   << " ==> indirect descendent of "
		   << other << endl;
	      CFraction c( *this );
	      ++c.m_u.back();
	      int k = c.depth();
	      c.m_p[ k ] += c.m_p[ k - 1 ];
	      c.m_q[ k ] += c.m_q[ k - 1 ];
	      return c;
	    }
	  else
	    cerr << "ImaGene::CFraction::mediant: error 1." << endl;
	}
      else // if ( depth() > other.depth() )
	{
	  if ( other.isDirectDescendent( *this ) )
	    {
	      CFraction c( other );
	      cerr << other << endl
		   << " ==> direct descendent of "
		   << *this << endl;
	      --c.m_u.back();
	      int k = c.depth();
	      c.m_u.push_back( 2 );
	      c.m_p[ k ] -= c.m_p[ k - 1 ];
	      c.m_p.push_back( 2 * c.m_p[ k ] + c.m_p[ k - 1 ] );
	      c.m_q[ k ] -= c.m_q[ k - 1 ];
	      c.m_q.push_back( 2 * c.m_q[ k ] + c.m_q[ k - 1 ] );
	      return c;
	    }
	  else if ( other.isIndirectDescendent( *this ) )
	    {
	      cerr << other << endl
		   << " ==> indirect descendent of "
		   << *this << endl;
	      CFraction c( other );
	      ++c.m_u.back();
	      int k = c.depth();
	      c.m_p[ k ] += c.m_p[ k - 1 ];
	      c.m_q[ k ] += c.m_q[ k - 1 ];
	      return c;
	    }
	  else
	    cerr << "ImaGene::CFraction::mediant: error 1." << endl;
	}
    }
  return CFraction( p() + other.p(), q() + other.q() );
}

/**
 * @param other any other fraction.
 * @return the determinant of the vector (q,p) with (other.q,other.p).
 */
int
ImaGene::CFraction::det( const CFraction & other ) const
{
  return q() * other.p() - p() * other.q();
}

/**
 * @param other any other fraction.
 *
 * @return true if 'this' is the direct descendent of [other] in the
 * Stern-Brocot tree.
 */
bool
ImaGene::CFraction::isDirectDescendent( const CFraction & other ) const
{
  int d = depth() - 1;
  return ( ( p() - pk( d ) ) == other.p() )
    &&  ( ( q() - qk( d ) ) == other.q() );
}

/**
 * @param other any other fraction.
 *
 * @return true if 'this' is the indirect descendent of [other] in the
 * Stern-Brocot tree.
 */
bool
ImaGene::CFraction::isIndirectDescendent( const CFraction & other ) const
{
  int d = depth() - 1;
  return ( pk( d ) == other.p() )
    &&  ( qk( d ) == other.q() );
}


/**
 * Constructs and returns the fraction p/q= (k1*p1 +
 * k2*p2)/(k1*q1+k2*q2).  To be efficient, at least one of k1 or
 * k2 should be 1, and z1 and z2 should have determinant +1/-1.
 *
 * Note : the return by value imposes a O(prof(p/q)) complexity.
 *
 * @param z1 a fraction p1/q1.
 * @param k1 a positive number (the number of repetition of z1)
 * @param z2 a fraction p2/q2.
 * @param k2 a positive number (the number of repetition of z2)
 * @return the fraction p/q as defined above.
 */
ImaGene::CFraction
ImaGene::CFraction::compose( const CFraction & z1, uint k1, 
			     const CFraction & z2, uint k2 )
{
  CFraction z;
  z.init( k1 * z1.p() + k2 * z2.p(), 
	  k1 * z1.q() + k2 * z2.q() );
  return z;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::CFraction::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[CFraction]";
  that_stream << " " << p() << "/" << q();
  if ( depth() >= 0 )
      {
	that_stream << "=[" << m_u[ 0 ];
	for ( uint k = 1; k < m_u.size(); ++k )
	  that_stream << "," << m_u[ k ];
	that_stream << "]";
      }
    if ( depth() >= 0 )
      {
	that_stream << "={" << pk( 0 ) << "/" << qk( 0 );
	for ( uint k = 1; k < m_p.size(); ++k )
	  that_stream << " " << pk( k ) << "/" << qk( k );
	that_stream << "}";
      }
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::CFraction::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
