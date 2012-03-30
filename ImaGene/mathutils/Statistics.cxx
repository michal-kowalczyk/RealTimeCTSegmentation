///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Statistics.cxx
//
// Creation : 2007/02/19
//
// Version : 2007/02/19
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
//	2007/02/19 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <algorithm>
#include "ImaGene/mathutils/Statistics.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/Statistics.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const Statistics_RCS_ID = "@(#)class Statistics definition.";



///////////////////////////////////////////////////////////////////////////////
// class Statistics
///////////////////////////////////////////////////////////////////////////////


// ------------------------- static services ------------------------------

/**
 * Reads the input stream [in] line by line, excluding line beginning
 * with '#' and selects some columns according to [indices] to fill
 * the array of samples [samples]. For instance, if indices == { 3, 4,
 * 1 }, then samples( 0 ) will contain the third column, samples( 1 ),
 * the fourth column and samples( 2 ) the first column.
 *
 * @param in (modified) the input stream.
 * @param samples (updates) stores the data.
 * @param indices specifies in which columns data are read.
 */
void
ImaGene::Statistics::read( istream & in, Statistics & samples,
			   const vector<uint> & indices )
{
  string str;
  getline( in, str );
  while ( in.good() )
    {
      if ( ( str != "" ) 
	   && ( str[ 0 ] != '#' ) )
	{
	  istringstream in_str( str );
	  uint idx = 1;
	  double val;
	  while ( in_str.good() )
	    {
	      in_str >> val;
	      for ( uint j = 0; j < indices.size(); ++j )
		if ( indices[ j ] == idx )
		  {
		    samples.addValue( j, val );
		    // cout << "Adding " << val << " to " << j << endl;
		  }
	      ++idx;
	    }
	}
      getline( in, str );
    }
}



///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::Statistics::~Statistics()
{
  erase();
}


/** 
 * Creates an object for computing [size] statistics.
 * 
 * @param size the number of statistical variables.
 *
 * @param store_samples when 'true', stores each given sample,
 * default is 'false'
 */    
ImaGene::Statistics::Statistics( uint size, 
				 bool store_samples )
{
  m_samples = 0;
  m_exp = 0;
  m_exp2 = 0;
  m_var = 0;
  m_unbiased_var = 0;
  m_max = 0;
  m_min = 0;
  m_values = 0;
  m_indice_max = 0;
  m_indice_min = 0;

  init( size, store_samples );


}



/** 
 * Adds a new sample value [v] for the variable [k].
 * 
 * @param k the statistical variable.
 * @param v the new sample value.
 */
void
ImaGene::Statistics::addValue( uint k, double v )
{
  ASSERT_Statistics( k < m_nb );
  m_samples[ k ] += 1;
  m_exp[ k ] += v;
  m_exp2[ k ] += v*v;
  if ( m_samples[ k ] == 1 )
    {
      m_max[ k ] = v;
      m_min[ k ] = v;
      m_indice_max[k]=0;
      m_indice_min[k]=0;
      
    }
  else if ( v > m_max[ k ] ){
    m_max[ k ] = v;
    m_indice_max[k]=m_samples[k]-1;
  }
  else if ( v < m_min[ k ] ){
    m_min[ k ] = v;
    m_indice_min[k]=m_samples[k]-1;
  }
  if ( m_store_samples )
    m_values[ k ].push_back( v );
}
  
/** 
 * Once all sample values have been added to this object, computes
 * meaningful statistics like sample mean, variance and unbiased
 * variance.
 * 
 * @see mean, variance, unbiasedVariance, min, max
 */
void 
ImaGene::Statistics::terminate()
{
  for ( uint k = 0; k < m_nb; ++k )
    {
      m_exp[ k ] /= m_samples[ k ];
      m_exp2[ k ] /= m_samples[ k ];
      m_var[ k ] = m_exp2[ k ] - m_exp[ k ] * m_exp[ k ];
      m_unbiased_var[ k ] = m_samples[ k ] * m_var[ k ] 
	/ ( m_samples[ k ] - 1 );
    }
}




/** 
 * Reinitializes the object to contain [size] statistical
 * variables. All preceding sample values and statistics are
 * lost. Calls 'erase()'.
 * 
 * @param size the number of statistical variables.
 *
 * @param store_samples when 'true', stores each given sample,
 *
 * @see erase
 */
void 
ImaGene::Statistics::init( uint size, bool store_samples )
{
  erase();
  if ( size == 0 ) return;
  m_nb = size;
  m_samples = new uint[ size ];
  m_exp = new double[ size ];
  m_exp2 = new double[ size ];
  m_var = new double[ size ];
  m_unbiased_var = new double[ size ];
  m_max = new double[ size ];
  m_min = new double[ size ];
  m_indice_max = new uint[ size ];
  m_indice_min = new uint[ size ];
  m_store_samples = store_samples;
  if ( m_store_samples )
    m_values = new vector<double>[ size ];
  clear();
}



/** 
 * Clears the object from all given samples. The object still
 * represents 'nb()' statistical variables.
 */
void 
ImaGene::Statistics::clear()
{
  if ( m_nb == 0 ) return;
  for ( uint i = 0; i < m_nb; ++ i )
    {
      m_samples[ i ] = 0;
      m_exp[ i ] = 0.0;
      m_exp2[ i ] = 0.0;
      m_var[ i ] = 0.0;
      m_unbiased_var[ i ] = 0.0;
      m_max[ i ] = 0.0;
      m_min[ i ] = 0.0;
      m_indice_min[ i ] = 0;
      m_indice_max[ i ] = 0;
      
      if ( m_store_samples ) {
	m_values[ i ].clear();
	m_values[ i ].reserve( 128 );
      }
    }
}
  

/** 
 * Erases the object as if it was just created. The user must call
 * 'init()' before any other method.
 * 
 * @see init
 */
void 
ImaGene::Statistics::erase() 
{
  if ( m_samples != 0 ) delete[] m_samples;
  if ( m_exp != 0 ) delete[] m_exp;
  if ( m_exp2 != 0 ) delete[] m_exp2;
  if ( m_var != 0 ) delete[] m_var;
  if ( m_unbiased_var != 0 ) delete[] m_unbiased_var;
  if ( m_max != 0 ) delete[] m_max;
  if ( m_min != 0 ) delete[] m_min;
  if ( m_indice_max != 0 ) delete[] m_indice_max;
  if ( m_indice_min != 0 ) delete[] m_indice_min;
  if ( m_values != 0 ) delete[] m_values;

  m_samples = 0;
  m_exp = 0;
  m_exp2 = 0;
  m_var = 0;
  m_unbiased_var = 0;
  m_nb = 0;
  m_min = 0;
  m_max = 0;
  m_indice_min = 0;
  m_indice_max = 0;  
  m_values = 0;
 
  m_store_samples = false;
}


/**
 * Computes the covariance of the [x],[y] variables. The
 * 'Statistics' object should have stored its values and
 * 'terminate' should have been called.
 *
 * @param x the index of the x-variable (should be valid )
 * @param y the index of the y-variable (should be valid )
 * @param s the starting value index (default is 0 )
 * @param e the after-the-end value index (default is samples( 0 ) )
 *
 * @return the covariance of [x],[y] from indices [s] to [e].
 */
double
ImaGene::Statistics::covariance( uint x, uint y,
				 uint s, uint e ) const
{
  ASSERT_Statistics( ( x < m_nb ) && ( y < m_nb ) && ( x != y )
		     && m_store_samples 
		     && ( samples( x ) == samples( y ) ) );
  uint size = samples( x );
  double coVariance = 0.0;
  double mx = 0.0;
  double my = 0.0;
  // for( uint k = 0; k < size; ++k )
  //   coVariance += ( value( x, k ) - mean( x ) )
  //     * ( value( y, k ) - mean( y ) );
  // coVariance = coVariance / size;
  if ( e == 0 ) e = size;
  ASSERT_Statistics( e > s );
  for( uint k = s; k != e; ++k )
    {
      coVariance += value( x, k ) * value( y, k );
      mx += value( x, k );
      my += value( y, k );
    }
  mx /= ( e - s );
  my /= ( e - s );
  coVariance = coVariance / ( e - s );
  coVariance = coVariance - mx * my;
  return coVariance;
}

/**
 * Computes the linear regression of the ([x],[y]) variables. The
 * 'Statistics' object should have stored its values and 'terminate'
 * should have been called.
 *
 * @param x the index of the x-variable (should be valid )
 * @param y the index of the y-variable (should be valid )
 *
 * @return the pair (slope, shift at origin )
 */
std::pair<double,double> 
ImaGene::Statistics::linearRegression( uint x, uint y ) const
{
  ASSERT_Statistics( ( x < m_nb ) && ( y < m_nb ) && ( x != y )
		     && m_store_samples 
		     && ( samples( x ) == samples( y ) ) );

  double cov = covariance( x, y );
  double slope = cov / unbiasedVariance( x );
  double b = mean( y ) - slope * mean( x );
  return std::make_pair( slope, b );
}




/**
 * Computes the median value for a variable k. The
 * 'Statistics' object should have stored its values and 'terminate'
 * should have been called.
 *
 * @param k
 *
 * @return the median value for the variable k.
 */
double
ImaGene::Statistics::median( uint k ) 
{
  ASSERT_Statistics( m_store_samples );
  
  nth_element( m_values[ k ].begin(), m_values[ k ].begin()+(m_values[k].size()/2), 
	       m_values[ k ].end());
  return *(m_values[ k ].begin()+(m_values[k].size()/2));
}





///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::Statistics::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Statistics] nb=" << nb() << endl;
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::Statistics::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
