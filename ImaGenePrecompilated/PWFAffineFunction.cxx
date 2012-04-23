///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : PWFAffineFunction.cxx
//
// Creation : 2004/04/14
//
// Version : 2004/04/14
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
//	2004/04/14 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include "ImaGene/mathutils/PWFAffineFunction.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/PWFAffineFunction.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const PWFAffineFunction_RCS_ID = "@(#)class PWFAffineFunction definition.";


///////////////////////////////////////////////////////////////////////////////
// class PWFAffineFunctionData
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Standard services ------------------------------

/**
 * Destructor. 
 */
ImaGene::PWFAffineFunctionData::~PWFAffineFunctionData()
{
}


/**
 * Constructor. Empty affine function. 
 */
ImaGene::PWFAffineFunctionData::PWFAffineFunctionData()
  : m_nb_points( 1 ), m_x( 1 ), m_fx( 1 ), m_alpha( 0 ), m_beta( 0 )
{
  m_x[ 0 ] = 0.0;
  m_fx[ 0 ] = 0.0;
}


/**
 * Copy constructor.
 * @param other the object to clone.
 */
ImaGene::PWFAffineFunctionData::PWFAffineFunctionData
( const PWFAffineFunctionData & other )
  : m_nb_points( other.m_nb_points ),
    m_x( other.m_x ),
    m_fx( other.m_fx ),
    m_alpha( other.m_alpha ),
    m_beta( other.m_beta )
{
}


/**
 * Assignment.
 * @param other the object to copy.
 * @return a reference on 'this'.
 */
ImaGene::PWFAffineFunctionData & 
ImaGene::PWFAffineFunctionData::operator=
( const PWFAffineFunctionData & other )
{
  if ( this != &other )
    {
      m_nb_points = other.m_nb_points;
      m_x = other.m_x;
      m_fx = other.m_fx;
      m_alpha = other.m_alpha;
      m_beta = other.m_beta;
    }
  return *this;
}




///////////////////////////////////////////////////////////////////////////////
// ------------------------- Initializers ---------------------------------

/**
 * Initializer from points and affine pieces.
 * @param x the (sorted) positions of the samples.
 * @param fx the sample values.
 * @param alpha the slope coefficients for each piece.
 * @param beta the offset coefficients for each piece.
 */
void
ImaGene::PWFAffineFunctionData::init
( const vector<float> & x, 
  const vector<float> & fx,
  const vector<float> & alpha,
  const vector<float> & beta )
{
  ASSERT_PWFAffineFunction( ( x.size() == fx.size() )
			    && ( alpha.size() == beta.size() )
			    && ( x.size() == alpha.size() + 1 ) );

  m_x = x;
  m_fx = fx;
  m_alpha = alpha;
  m_beta = beta;
  m_nb_points = x.size();
}

/**
 * Initializer from points. The affine function is assumed continuous.
 * @param x the (sorted) positions of the samples.
 * @param fx the sample values.
 */
void
ImaGene::PWFAffineFunctionData::init
( const vector<float> & x, 
  const vector<float> & fx )
{
  ASSERT_PWFAffineFunction( x.size() == fx.size() );
  uint s = x.size();
  if ( m_x.size() != s )
    {
      m_x.resize( s );
      m_fx.resize( s );
      m_alpha.resize( s - 1 );
      m_beta.resize( s - 1 );
    }
  for ( uint i = 0; i < s; ++i )
    {
      m_x[ i ] = x[ i ];
      m_fx[ i ] = fx[ i ];
    }
  for ( uint i = 0; i < ( s - 1 ); ++i )
    {
      ASSERT_PWFAffineFunction( x[ i + 1 ] > x[ i ] );
      m_alpha[ i ] = ( fx[ i + 1 ] - fx[ i ] ) / ( x[ i + 1 ] - x[ i ] );
      m_beta[ i ] = ( - fx[ i + 1 ] * x[ i ] + fx[ i ] * x[ i + 1 ] )
	/ ( x[ i + 1 ] - x[ i ] );
    }
  m_nb_points = s;
}




/////////////////////////////////////////////////////////////////////////////
// ------------------------- Basic modification services --------------------


/**
 * Splits the function at position [x]. The new function has the same
 * graph but not the same representation. Does nothing if the affine
 * function is already split at this coordinate.
 * 
 * @param x the position where to split.
 */
void
ImaGene::PWFAffineFunctionData::split( float x )
{
  viterator p = upper_bound( m_x.begin(), m_x.end(), x );

  if ( p == m_x.end() )
    {
      m_x.push_back( x );
      m_fx.push_back( 0.0 );
      m_alpha.push_back( 0.0 );
      m_beta.push_back( 0.0 );
      ++m_nb_points;
    }
  else if ( *p != x )
    {
      float a;
      float b;
      uint at_idx = p - m_x.begin();
      m_x.insert( m_x.begin() + at_idx, x );
      a = ( at_idx >= 1 ) ? m_alpha[ at_idx - 1 ] : 0.0;
      b = ( at_idx >= 1 ) ? m_beta[ at_idx - 1 ] : 0.0;
      m_alpha.insert( m_alpha.begin() + at_idx, a );
      m_beta.insert( m_alpha.begin() + at_idx, b );
      m_fx.insert( m_fx.begin() + at_idx, a * x + b );
      ++m_nb_points;
    }
}


/**
 * Sets the value at point [x] to be [fx]. This may destroy continuity.
 * The function must be split at this point before.
 * 
 * @param x a position.
 * @param fx the sample value.
 */
void
ImaGene::PWFAffineFunctionData::setFx
( float x, float fx )
{
  viterator p = upper_bound( m_x.begin(), m_x.end(), x );
  int idx = p - m_x.begin();
  ASSERT_PWFAffineFunction( *p == x );
  m_x[ idx ] = x;
  m_fx[ idx ] = fx;
}


/**
 * Sets the values for an open interval [x1,x2] to be the affine
 * function [alpha*x+beta]. This may destroy continuity. 
 * The function must be split at [x1] and [x2].
 *
 * @param x1 the left-handside of the interval.
 * @param x2 the right-handside of the interval.
 * @param alpha the slope of the straight line.
 * @param beta the offset of the straight line.
 */
void
ImaGene::PWFAffineFunctionData::setFinterval
( float x1, float x2, float alpha, float beta )
{
  viterator p1 = upper_bound( m_x.begin(), m_x.end(), x1 );
  viterator p2 = upper_bound( m_x.begin(), m_x.end(), x2 );
  ASSERT_PWFAffineFunction( ( *p1 == x1 ) && ( *p2 == x2 ) 
			    && ( p1 < p2 ) );
  uint i1 = p1 - m_x.begin();
  uint i2 = p2 - m_x.begin();
  m_x.erase( m_x.begin() + i1 + 1, m_x.begin() + i2 );
  m_fx.erase( m_fx.begin() + i1 + 1, m_fx.begin() + i2 );
  m_alpha.erase( m_alpha.begin() + i1 + 1, m_alpha.begin() + i2 );
  m_beta.erase( m_beta.begin() + i1 + 1, m_beta.begin() + i2 );
  m_alpha[ i1 ] = alpha;
  m_beta[ i1 ] = beta;
}

/**
 * Sets the values for an open interval [x1,x2] to be the affine
 * function [alpha*x+beta]. This may destroy continuity. 
 * The function must be split at [x1] and [x2].
 *
 * @param x1 the left-handside of the interval.
 * @param x2 the right-handside of the interval.
 * @param y1 the limit value at point x1. 
 * @param y2 the limit value at point x2. 
 */
void
ImaGene::PWFAffineFunctionData::setFintervalPoints
( float x1, float x2, float y1, float y2 )
{
  setFinterval( x1, x2,
		( y2 - y1 ) / ( x2 - x1 ),
		( - y2 * x1 + y1 * x2 ) / ( x2 - x1 ) );
}


/**
 * Multiplies the whole function with value [lambda].
 * @param lambda any factor.
 */
void
ImaGene::PWFAffineFunctionData::multiply( float lambda )
{
   for ( uint i = 0; i < m_fx.size(); ++i )
     m_fx[ i ] *= lambda;
   for ( uint i = 0; i < ( m_fx.size() - 1 ); ++i )
     {
       m_alpha[ i ] *= lambda;
       m_beta[ i ] *= lambda;
     }
}


/**
 * @return the first moment of the function.
 */
float
ImaGene::PWFAffineFunctionData::centroid() const
{
  uint s = m_fx.size() - 1;
  float xc = 0.0;
  for ( uint i = 0; i < s; ++i )
    {
      float x1 = m_x[ i ];
      float x2 = m_x[ i + 1];
      xc += m_alpha[ i ] * ( x2*x2*x2 - x1*x1*x1 ) / 3.0
	+ m_beta[ i ] * ( x2*x2 - x1*x1 ) / 2.0;
    }
  return xc;
}


/**
 * @return the variance of the function.
 * @see centroid.
 */
float
ImaGene::PWFAffineFunctionData::variance() const
{
  return moment2( centroid() );
}


/**
 * @param mu a given mean (can be 'centroid()').
 * @return the second moment of the function around the given mean [mu].
 * @see centroid.
 */
float
ImaGene::PWFAffineFunctionData::moment2( float mu ) const
{
  uint s = m_fx.size() - 1;
  float xc = 0.0;
  for ( uint i = 0; i < s; ++i )
    {
      float x1 = m_x[ i ];
      float x2 = m_x[ i + 1];
      xc += m_alpha[ i ] * ( x2*x2*x2*x2 - x1*x1*x1*x1 ) / 4.0
	+ ( ( m_beta[ i ] - 2 * mu * m_alpha[ i ] )
	    * ( x2*x2*x2 - x1*x1*x1 ) / 3.0 )
	+ ( ( mu * mu * m_alpha[ i ] - 2 * mu * m_beta[ i ] )
	    * ( x2*x2 - x1*x1 ) / 2.0 )
	+ m_beta[ i ] * mu * mu * ( x2 - x1 );
    }
  return xc;
}

// ------------------------- Basic function services --------------------

/**
 * @param x any position.
 * @return the value of the function at point [x].
 */
float
ImaGene::PWFAffineFunctionData::value( float x ) const
{
  const_viterator p = lower_bound( m_x.begin(), m_x.end(), x );
  if ( p == m_x.end() ) return 0.0;
  if ( *p == x ) return m_fx[ p - m_x.begin() ];
  if ( p == m_x.begin() ) return 0.0;
  uint idx = p - m_x.begin() - 1;
  return m_alpha[ idx ] * x + m_beta[ idx ];
}


/**
 * @param x (returns) the position of the first maximal value.
 * @param limit (returns) if 0 the maximal value is at [x], if positive the maximal value is to the right of [x], if negative the maximal value is to the left of [x].
 * @return the maximal value of the function at point [x].
 */
float
ImaGene::PWFAffineFunctionData::firstMax
( float & x, int & limit ) const
{
  uint i = 0;
  x = m_x[ i ];
  limit = 0;
  float max = m_fx[ i ];
  float y;
  for ( ; i < m_x.size() - 1; ++i )
    {
      y = m_alpha[ i ] * m_x[ i ] + m_beta[ i ];
      if ( y > max ) 
	{
	  x = m_x[ i ];
	  max = y;
	  limit = 1;
	}
      y = m_alpha[ i ] * m_x[ i + 1 ] + m_beta[ i ];
      if ( y > max ) 
	{
	  x = m_x[ i + 1 ];
	  max = y;
	  limit = -1;
	}
      y = m_fx[ i + 1 ];
      if ( y > max ) 
	{
	  x = m_x[ i + 1 ];
	  max = y;
	  limit = 0;
	}
    }
  return max;
}



// /**
//  * Replace the interval at position [at_idx].
//  * 
//  * @param at_idx the index of the interval to replace.
//  * @param a the new alpha.
//  * @param b the new beta.
//  */
// float
// ImaGene::PWFAffineFunctionData::replaceInterval( uint at_idx, 
// 						 float a, float b )
// {
//    ASSERT_PWFAffineFunction( at_idx < m_nb_points - 1 );
//    m_alpha[ at_idx ] = a;
//    m_beta[ at_idx ] = b;
// }

// /**
//  * Replace the interval at position [at_idx].
//  * 
//  * @param at_idx the index of the interval to replace.
//  * @param y1 the value to the left of the interval.
//  * @param y2 the value to the right of the interval.
//  */
// float
// ImaGene::PWFAffineFunctionData::replaceInterval( uint at_idx, 
// 						 float y1, float y2 )
// {
//    ASSERT_PWFAffineFunction( at_idx < m_nb_points - 1 );

//    float x1 = m_x[ at_idx ];
//    float x2 = m_x[ at_idx + 1 ];
//    m_alpha[ at_idx ] = ( y2 - y1 ) / ( x2 - x1 );
//    m_beta[ at_idx ] = ( - y2 * x1 + y1 * x2 ) / ( x2 - x1 );
// }



///////////////////////////////////////////////////////////////////////////////
// class PWFAffineFunction
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::PWFAffineFunction::~PWFAffineFunction()
{
}


/**
 * Default Constructor. Builds the empty affine function.
 */
ImaGene::PWFAffineFunction::PWFAffineFunction()
  : m_data( PWFAffineFunctionData() )
{
}


/**
 * Constructor from data.
 * @param data any data corresponding to a piecewise affine function.
 */
ImaGene::PWFAffineFunction::PWFAffineFunction
( const PWFAffineFunctionData & data )
  : m_data( data )
{
}

/**
 * Copy constructor. 
 * @param other any piecewise affine function.
 */
ImaGene::PWFAffineFunction::PWFAffineFunction
( const PWFAffineFunction & other )
  : m_data( other.m_data )
{
}

/**
 * Assignment. 
 * @param other the object to copy.
 * @return a reference on 'this'.
 */
ImaGene::PWFAffineFunction &
ImaGene::PWFAffineFunction::operator=
( const PWFAffineFunction & other )
{
  if ( this != &other )
    m_data = other.m_data;
  return *this;
}


/**
 * Read-only accessor to data associated with affine function.
 * @return a const reference to the data of the affine function.
 */
const ImaGene::PWFAffineFunctionData & 
ImaGene::PWFAffineFunction::data() const
{
  return m_data.ro();
}


/**
 * Read-write accessor to data associated with affine function.
 * @return a reference to the data of the affine function.
 */
ImaGene::PWFAffineFunctionData & 
ImaGene::PWFAffineFunction::data()
{
  return m_data.rw();
}



///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static services ------------------------------

/**
 * @param a any real value.
 * @param b any real value greater than a.
 * @return the affine function that is the probability density of
 * the uniform law on the interval [a,b].
 */
ImaGene::PWFAffineFunction
ImaGene::PWFAffineFunction::uniformDP( float a, float b )
{
  PWFAffineFunction f;
  vector<float> x(2);
  vector<float> y(2);
  x[ 0 ] = a;
  x[ 1 ] = b;
  y[ 0 ] = 1.0 / ( b - a );
  y[ 1 ] = 1.0 / ( b - a );
  f.data().init( x, y );
  return f;
}


/**
 * @param a any real value.
 * @param b any real value greater than a.
 * @return the affine function that is the probability density of
 * the triangular law on the interval [a,b].
 */
ImaGene::PWFAffineFunction
ImaGene::PWFAffineFunction::triangularDP( float a, float b )
{
  PWFAffineFunction f;
  vector<float> x(3);
  vector<float> y(3);
  x[ 0 ] = a;
  x[ 1 ] = ( a + b ) / 2.0;
  x[ 2 ] = b;
  y[ 0 ] = 0.0;
  y[ 1 ] = 2.0 / ( b - a );
  y[ 2 ] = 0.0;
  f.data().init( x, y );
  return f;
}


/**
 * @param a any real value.
 * @param b any real value greater than a.
 * @param l a real value between 0 and 0.5 (0 gives uniform law, 0.5 gives triangular law, linear in-between).
 *
 * @return the affine function that is the probability density of
 * the symmetric trapezoidal law on the interval [a,b] with shape
 * coefficient [l].
 */
ImaGene::PWFAffineFunction
ImaGene::PWFAffineFunction::trapezoidalDP( float a, float b, float l )
{
  if ( ( l <= 0.0 ) || ( l >= 1.0 ) ) return uniformDP( a, b );
  else if ( l == 0.5 ) return triangularDP( a, b );
  else if ( l >= 0.5 ) l = 1.0 - l;
  
  PWFAffineFunction f;
  vector<float> x(4);
  vector<float> y(4);
  x[ 0 ] = a;
  x[ 1 ] = a + l * ( b - a );
  x[ 2 ] = a + ( 1 - l ) * ( b - a );
  x[ 3 ] = b;
  y[ 0 ] = 0.0;
  y[ 1 ] = 1.0 / ( ( 1.0 - l ) * ( b - a ) );
  y[ 2 ] = y[ 1 ];
  y[ 3 ] = 0.0;
  f.data().init( x, y );
  return f;
}



/**
 * Sums affine functions.
 * @param f1 any affine function.
 * @param f2 any affine function.
 * @return the sum of f1 and f2.
 */
ImaGene::PWFAffineFunction
ImaGene::PWFAffineFunction::add( const PWFAffineFunction & f1,
				 const PWFAffineFunction & f2 )
{
  const PWFAffineFunctionData & df1 = f1.m_data.ro();
  const PWFAffineFunctionData & df2 = f2.m_data.ro();
  PWFAffineFunctionData df;

  vector<float> x;
  vector<float> fx;
  vector<float> alpha;
  vector<float> beta;
  x.reserve( df1.m_x.size() + df2.m_x.size() );
  fx.reserve( df1.m_x.size() + df2.m_x.size() );
  alpha.reserve( df1.m_x.size() + df2.m_x.size() );
  beta.reserve( df1.m_x.size() + df2.m_x.size() );
  uint i1 = 0;
  uint i2 = 0;
  while ( ( i1 < df1.m_x.size() ) && ( i2 < df2.m_x.size() ) )
    {
      if ( df1.m_x[ i1 ] < df2.m_x[ i2 ] )
	{
	  x.push_back( df1.m_x[ i1 ] );
	  fx.push_back( df1.m_fx[ i1 ] );
	  if ( i2 != 0 )
	    {
	      fx.back() += df2.m_alpha[ i2 - 1 ] * df1.m_x[ i1 ] 
		+ df2.m_beta[ i2 - 1 ];
	    }
	  ++i1;
	}
      else if ( df1.m_x[ i1 ] > df2.m_x[ i2 ] )
	{
	  x.push_back( df2.m_x[ i2 ] );
	  fx.push_back( df2.m_fx[ i2 ] );
	  if ( i1 != 0 )
	    {
	      fx.back() += df1.m_alpha[ i1 - 1 ] * df2.m_x[ i2 ] 
		+ df1.m_beta[ i1 - 1 ];
	    }
	  ++i2;
	}
      else // ( df1.m_x[ i1 ] == df2.m_x[ i2 ] )
	{
	  x.push_back( df2.m_x[ i2 ] );
	  fx.push_back( df1.m_fx[ i1 ] + df2.m_fx[ i2 ] );
	  ++i1;
	  ++i2;
	}
      float a = 0.0;
      float b = 0.0;
      if ( ( i1 > 0 ) && ( i1 < df1.m_x.size() ) ) 
	{
	  a += df1.m_alpha[ i1 - 1 ];
	  b += df1.m_beta[ i1 - 1 ];
	}
      if ( ( i2 > 0 ) && ( i2 < df2.m_x.size() ) )
	{
	  a += df2.m_alpha[ i2 - 1 ];
	  b += df2.m_beta[ i2 - 1 ];
	}
      alpha.push_back( a );
      beta.push_back( b );
    } // while ( ( i1 < df1.m_x.size() ) && ( i2 < df2.m_x.size() ) )

  while ( i1 < df1.m_x.size() )
    {
      x.push_back( df1.m_x[ i1 ] );
      fx.push_back( df1.m_fx[ i1 ] );
      ++i1;
      if ( i1 < df1.m_x.size() )
	{
	  alpha.push_back( df1.m_alpha[ i1 - 1 ] );
	  beta.push_back( df1.m_beta[ i1 - 1 ] );
	}
    } // while ( i1 < df1.m_x.size() )

  while ( i2 < df2.m_x.size() )
    {
      x.push_back( df2.m_x[ i2 ] );
      fx.push_back( df2.m_fx[ i2 ] );
      ++i2;
      if ( i2 < df2.m_x.size() )
	{
	  alpha.push_back( df2.m_alpha[ i2 - 1 ] );
	  beta.push_back( df2.m_beta[ i2 - 1 ] );
	}
    } // while ( i2 < df2.m_x.size() )

  PWFAffineFunctionData affine_data;
  affine_data.init( x, fx, alpha, beta );

  return PWFAffineFunction( affine_data );
}


/**
 * Multiply an affine function by a scalar and returns the result.
 * @param f any affine function.
 * @param lambda any scalar.
 * @return the product of f by lambda.
 */
ImaGene::PWFAffineFunction 
ImaGene::PWFAffineFunction::multiply( const PWFAffineFunction & f,
				      float lambda )
{
  PWFAffineFunction g( f );
  g.m_data.rw().multiply( lambda );
  return g;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::PWFAffineFunction::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[PWFAffineFunction]";
  const vector<float> & x = data().m_x;
  for ( uint i = 0; i < x.size(); ++i )
    that_stream << " " << x[ i ];
  that_stream << endl;
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::PWFAffineFunction::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
