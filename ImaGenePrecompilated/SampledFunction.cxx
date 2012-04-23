///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : SampledFunction.cxx
//
// Creation : 2009/02/13
//
// Version : 2009/02/13
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
//	2009/02/13 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <algorithm>
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/mathutils/SampledFunction.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/SampledFunction.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const SampledFunction_RCS_ID = "@(#)class SampledFunction definition.";



///////////////////////////////////////////////////////////////////////////////
// class SampledFunction
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::SampledFunction::~SampledFunction()
{
}


/**
 * Creates a non-valid sampled function.
 * @see init
 */
ImaGene::SampledFunction::SampledFunction()
  : m_height( -1.0 )
{
  periodicity = 0.0;
}

/**
 * Create a sampled function from a vector of positions, a vector
 * of values, and periodicity information.
 *
 * @param x the sample positions (sorted).
 * @param fx the values at each sample position.
 *
 * @param periodic when 'true', the function is considered
 * periodic, with periodicity [periodicity].
 *
 * @param periodicity should be greater than the last x value.
 */
void 
ImaGene::SampledFunction::init( const vector<double> & x, 
				const vector<double> & fx,
				bool periodic,
				double periodicity )
{
  this->x = x;
  this->fx = fx;
  this->periodicity = periodic ? periodicity : 0.0;
  if ( periodicity < this->x.back() ) 
    {
      cerr << "[ImaGene::SampledFunction::init] invalid periodicity" << endl;
      periodicity = 0.0;
    }
  m_height = -1.0;
}

/**
 * Evaluates the function at [x].
 * Complexity is O(log(n)).
 *
 * @param x any value in the range.
 * @return the value of the function at [x].
 */
float 
ImaGene::SampledFunction::value( float x ) const
{
  return (double) this->value( (double) x );
}


/**
 * Evaluates the function at [x].
 * Complexity is O(log(n)).
 *
 * @param x any value in the range.
 * @return the value of the function at [x].
 */
double
ImaGene::SampledFunction::value( double x ) const
{
  double lambda;
  uint i = this->find( x, lambda );
  return ( 1.0 - lambda ) * fx[ i ] + lambda * fx[ ( i + 1 ) % fx.size() ];
}

/**
 * Finds the index i of the sample such that x[i] <= s < x[i+1],
 * whether it is periodic or not.
 * Complexity is O(log(n)).
 *
 * @param s a real number describing a position.
 *
 * @param lambda (returns) a coefficient between 0.0 and 1.0
 * (excluded) which tells where s is between x[i] and x[i+1].
 *
 * @return the index in the array of sample positions x.
 */
uint
ImaGene::SampledFunction::find( double s, double & lambda ) const
{
  // cerr << "[find] s=" << s;
  if ( periodicity > 0.0 ) 
    {
      int n = floor( s / periodicity );
      s = s - n * periodicity;
    }
  // cerr << " (p=" << periodicity << ") after s=" << s;
  vector<double>::const_iterator it = upper_bound( x.begin(), x.end(), s );
  int idx = ( (int) ( it - x.begin() ) ) - 1;
  if ( ( idx >= 0 ) && ( idx < ( x.size() - 1 ) ) )
    lambda = ( s - x[ idx ] ) / ( x[ idx + 1 ] - x[ idx ] );
  else if ( periodicity > 0.0 )
    {
      if ( idx == ( x.size() - 1 ) )
	lambda = ( s - x[ idx ] ) / ( periodicity - x[ idx ] );
      else 
	{
	  idx = x.size() - 1;
	  lambda = ( s + periodicity - x[ idx ] ) 
	    / ( periodicity - x[ idx ] + x[ 0 ] );
	}
    }
  else
    {
      if ( idx == ( x.size() - 1 ) )
	{
	  lambda = 0.0;
	}
      else
	{
	  idx = 0; lambda = 0.0;
	}
    }
  // cerr << " lambda=" << lambda << " idx=" << idx << endl;
  return idx;
}

/**
 * @param i a sample index 
 *
 * @return 1 if it is maxima, -1 if it is a minima, 2 if it is a
 * starting or ending point and the funtion is not periodic, 0 in
 * the other cases.
 */
int
ImaGene::SampledFunction::type( uint i ) const
{
  uint n = x.size();
  if ( ( i == 0 ) || ( i == ( n - 1 ) ) )
    if ( periodicity <= 0.0 ) return 2;
  if ( ( fx[ ( i + n - 1 ) % n ] <= fx[ i ] )
       && ( fx[ ( i + 1 ) % n ] <= fx[ i ] ) )
    return 1;
  else if ( ( fx[ ( i + n - 1 ) % n ] >= fx[ i ] )
       && ( fx[ ( i + 1 ) % n ] >= fx[ i ] ) )
    return -1;
  return 0;
}

/**
 * Given an interval, returns the number of extreme points within.
 * Complexity is O(log(n)).
 *
 * @param x1 the first abscissa.
 * @param x2 the last abscissa.
 *
 * @return the number of extreme points on the sampled function
 * between positions [x1,x2].
 */
uint 
ImaGene::SampledFunction::numberExtremePoints( double x1, double x2 ) const
{
  double l1;
  double l2;
  Mathutils::ModuloComputer mc( x.size() );
  uint i1 = this->find( x1, l1 );
  uint i2 = this->find( x2, l2 );
  mc.increment( i2 );
  if ( l1 > 0.0 ) mc.increment( i1 );
  // cerr << "(" << x1 << "," << x2 << ") =>" 
  //      << "(" << i1 << "," << i2 << ")";
  uint nb = 0;
  while ( i1 != i2 )
    {
      uint t = this->type( i1 );
      if ( ( t == 1 ) || ( t == -1 ) )
	++nb;
      mc.increment( i1 );
    }
  // cerr << " with #nb=" << nb << endl;
  return nb;
}

/**
 * Given an interval, returns the number of extreme points within,
 * such that their scale measure is no smaller than lambda.
 * Complexity is O(log(n)).
 *
 * @param x1 the first abscissa.
 * @param x2 the last abscissa.
 *
 * @param lambda the minimal scale measure to take into account
 * the extreme points
 *
 * @return the number of extreme points on the sampled function
 * between positions [x1,x2].
 */
uint
ImaGene::SampledFunction::numberSignificantExtremePoints
( double x1, 
  double x2, 
  double lambda, bool normalized ) const
{
  double l1;
  double l2;
  Mathutils::ModuloComputer mc( x.size() );
  uint i1 = this->find( x1, l1 );
  uint i2 = this->find( x2, l2 );
  mc.increment( i2 );
  if ( l1 > 0.0 ) mc.increment( i1 );
  // cerr << "(" << x1 << "," << x2 << ") =>" 
  //      << "(" << i1 << "," << i2 << ")";
  uint nb = 0;
  while ( i1 != i2 )
    {
      uint t = this->type( i1 );
      if ( ( ( t == 1 ) || ( t == -1 ) )
	   && ( scaleMeasure( i1, normalized ) >= lambda ) )
	++nb;
      mc.increment( i1 );
    }
  // cerr << " with #nb=" << nb << endl;
  return nb;
}



/**
 * Given an interval, returns the algebraic number of extreme
 * points within (meaning concave and convex portions cancel each
 * other).  Complexity is O(log(n)).
 *
 * @param x1 the first abscissa.
 * @param x2 the last abscissa.
 *
 * @return the algebraic number of extreme points on the sampled function
 * between positions [x1,x2].
 */
int
ImaGene::SampledFunction::algebraicNumberExtremePoints
( double x1, double x2 ) const
{
  double l1;
  double l2;
  Mathutils::ModuloComputer mc( x.size() );
  uint i1 = this->find( x1, l1 );
  uint i2 = this->find( x2, l2 );
  mc.increment( i2 );
  if ( l1 > 0.0 ) mc.increment( i1 );
  // cerr << "(" << x1 << "," << x2 << ") =>" 
  //      << "(" << i1 << "," << i2 << ")";
  int nb = 0;
  while ( i1 != i2 )
    {
      uint t = this->type( i1 );
      if ( t == 1 ) ++nb;
      else if ( t == -1 ) --nb;
      mc.increment( i1 );
    }
  // cerr << " with #nb=" << nb << endl;
  return nb;
}


/**
 * @return the height of the function (i.e. max-min)
 */
double
ImaGene::SampledFunction::height() const
{
  if ( m_height < 0.0 ) 
    {
      double min = fx[ 0 ]; 
      double max = min;
      for ( uint idx = 1; idx != x.size(); ++idx )
	{
	  if ( fx[ idx ] > max ) max = fx[ idx ];
	  else if ( fx[ idx ] < min ) min = fx[ idx ];
	}
      m_height = max - min;
    }
  return m_height;
}


/**
 * Given a starting point given by an index, return the depth of the
 * function to the left of the starting point, i.e. the lowest (or
 * highest) deviation of the function wrt to the starting value up to
 * the moment where the function meets again the starting value.
 *
 * @param idx the starting position
 * @return the depth (or H function) to the left of position idx. 
 */
double 
ImaGene::SampledFunction::heightNegativePeak( uint idx ) const
{
  uint n = this->x.size();
  uint i = idx;
  double fx1 = this->fx[ i ];
  do {
    i = ( i + n - 1 ) % n;
  } while ( ( i != idx ) && ( this->fx[ i ] == fx1 ) );
  bool above = this->fx[ i ] > fx1;
  if ( above )
    {
      double max = this->fx[ i ];
      while ( ( i != idx ) && ( this->fx[ i ] >= fx1 ) )
	{
	  if ( this->fx[ i ] > max ) 
	    max = this->fx[ i ];
	  i = ( i + n - 1 ) % n;
	}
      return max - fx1;
    }
  else
    {
      double min = this->fx[ i ];
      while ( ( i != idx ) && ( this->fx[ i ] <= fx1 ) )
	{
	  if ( this->fx[ i ] < min ) 
	    min = this->fx[ i ];
	  i = ( i + n - 1 ) % n;
	}
      return fx1 - min;
    }
}

/**
 * Given a starting point given by an index, return the depth of the
 * function to the right of the starting point, i.e. the lowest (or
 * highest) deviation of the function wrt to the starting value up to
 * the moment where the function meets again the starting value.
 *
 * @param idx the starting position
 * @return the depth (or H function) to the right of position idx. 
 */
double
ImaGene::SampledFunction::heightPositivePeak( uint idx ) const
{
  uint n = this->x.size();
  uint i = idx;
  double fx1 = this->fx[ i ];
  do {
    i = ( i + 1 ) % n;
  } while ( ( i != idx ) && ( this->fx[ i ] == fx1 ) );
  bool above = this->fx[ i ] > fx1;
  if ( above )
    {
      double max = this->fx[ i ];
      while ( ( i != idx ) && ( this->fx[ i ] >= fx1 ) )
	{
	  if ( this->fx[ i ] > max ) 
	    max = this->fx[ i ];
	  i = ( i + 1 ) % n;
	}
      return max - fx1;
    }
  else
    {
      double min = this->fx[ i ];
      while ( ( i != idx ) && ( this->fx[ i ] <= fx1 ) )
	{
	  if ( this->fx[ i ] < min ) 
	    min = this->fx[ i ];
	  i = ( i + 1 ) % n;
	}
      return fx1 - min;
    }
}


/**
 * Given an extreme point specified by its index [idx], return its
 * scale measure (@see VisualCurvature). It is defined as the
 * minimum of the heights of the positive and negative peaks
 * around the point of interest, divided by the height of the
 * function.
 *
 * @param the given extreme point index.
 *
 * @return its scale measure between 0.0 and 1.0 (the larger, the
 * more significant is the extreme point).
 */
double
ImaGene::SampledFunction::scaleMeasure( uint idx, bool normalized ) const
{
  double r_minus = heightNegativePeak( idx );
  double r_plus = heightPositivePeak( idx );
 
 return ( r_minus <= r_plus ) 
   ? r_minus / (normalized? height():1.0)
   : r_plus /(normalized? height():1.0);
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::SampledFunction::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[SampledFunction]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::SampledFunction::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
