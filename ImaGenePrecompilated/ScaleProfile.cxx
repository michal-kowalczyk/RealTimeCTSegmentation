///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ScaleProfile.cxx
//
// Creation : 2010/05/24
//
// Version : 2010/05/24
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
//	2010/05/24 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/mathutils/SimpleLinearRegression.h"
#include "ImaGene/helper/ScaleProfile.h"
#include "ImaGene/base/Vector.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/ScaleProfile.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ScaleProfile_RCS_ID = "@(#)class ScaleProfile definition.";



///////////////////////////////////////////////////////////////////////////////
// class ScaleProfile
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ScaleProfile::~ScaleProfile()
{
  if ( m_scales != 0 ) delete m_scales;
  if ( m_stats != 0 ) delete m_stats; 
}

/**
 * Constructor. The object is not valid.
 */
ImaGene::ScaleProfile::ScaleProfile()
  : m_scales( 0 ), m_stats( 0 ), m_profileDef(MEAN), m_storeValInStats(false)
{
}


/**
 * Constructor. The object is not valid.
 * @param type allows to specify the used to computes the profile points from the added samples.
 */
ImaGene::ScaleProfile::ScaleProfile(ProfileComputingType type)
  : m_scales( 0 ), m_stats( 0 ), m_profileDef(type), m_storeValInStats(false)
{
}

/**
 * Clears the object as if it has been just created.
 */
void 
ImaGene::ScaleProfile::clear()
{
  if ( m_scales != 0 ) 
    {
      delete m_scales;
      m_scales = 0;
    }
  if ( m_stats != 0 ) 
    {
      delete m_stats;
      m_stats = 0;
    }
  
}



/**
 * Initializer. Must be called before adding datas. Specifies the
 * scales of the profile as the sequence (1,2,3,4,...,nb).
 *
 * @param nb an integer number strictly positive.
 */
void 
ImaGene::ScaleProfile::init( uint nb, bool storeValsInStats )
{
  clear();
  m_scales = new vector<float>( nb );
  m_stats = new vector< Statistic<float> >;
  m_storeValInStats= storeValsInStats;
  for ( uint i = 0; i < nb; ++i ){
    (*m_scales)[ i ] = (float) ( i + 1 );
    m_stats->push_back(Statistic<float>(storeValsInStats));
  }
}



/**
 * Adds some sample value at the given scale.
 *
 * @param idx_scale some valid index (according to init).
 * @param value any value.
 */
void
ImaGene::ScaleProfile::addValue( uint idx_scale, float value )
{
  ASSERT_ScaleProfile( OK() && ( idx_scale < m_scales->size() ) );
  (*m_stats)[ idx_scale ].addValue( value );

}

/**
 * Adds some statistic at the given scale.
 *
 * @param idx_scale some valid index (according to init).
 *
 * @param stat any statistic (which is added to the current
 * statistic object).
 */
void
ImaGene::ScaleProfile::addStatistic
( uint idx_scale, const Statistic<float> & stat )
{
  ASSERT_ScaleProfile( OK() && ( idx_scale < m_scales->size() ) );
  (*m_stats)[ idx_scale ] += stat;
}


///////////////////////////////////////////////////////////////////////////////
// ----------------------- Profile services --------------------------------

/**
 * @param x (modified) adds the x-value of the profile (log(scale))
 * to the back of the vector.
 *
 * @param y (modified) adds the y-value of the profile
 * (log(Exp(samples))) to the back of the vector.
 * 
 */
void
ImaGene::ScaleProfile::getProfile
( vector<double> & x, vector<double> & y ) const
{
  ASSERT_ScaleProfile( OK() );
  for ( uint i = 0; i < m_scales->size(); ++i) 
    {
      x.push_back( log( (*m_scales)[ i ] ) );
      switch (m_profileDef){
      case MEAN:
	y.push_back( log( (*m_stats)[ i ].mean() ) );
	break;
      case MAX:
	y.push_back( log( (*m_stats)[ i ].max() ) );
	break;
      case MIN:
	y.push_back( log( (*m_stats)[ i ].min() ) );
	break;
      case MEDIAN:
	y.push_back (log( (*m_stats)[ i ].median() ));
	break;
      }
    }
}

/**
 * A meaningful scale is an interval of scales of length no
 * smaller than [min_width] and in which the profile has slopes
 * below [max_slope] and above [min_slope]. This method returns
 * the sequence of meaningful scales for surfel [idx].
 *
 * @param intervals (returns) a list of meaningful scales.
 * @param min_width the minimum length for the meaningful scales.
 * @param max_slope the maximum allowed slope for length evolution.
 * @param min_slope the minimum allowed slope for length evolution.
 * @param method the method which is used to obtain the multiscale profile (MEAN (default), MAX or MIN (not very efficient)) 
 */
void 
ImaGene::ScaleProfile::meaningfulScales
( std::vector< std::pair< uint, uint > > & intervals,
  uint min_width,
  double max_slope,
  double min_slope ) const
{
  vector<double> x;
  vector<double> y;
  this->getProfile( x, y );
  uint l = 0;
  for ( uint k = 1; k < x.size(); ++k )
    {
      double slope = ( y[ k ] - y[ k - 1 ] ) / ( x[ k ] - x[ k - 1 ] );
      if ( ( slope > max_slope ) 
	   || ( slope < min_slope ) 
	   || ( (k+1) == x.size() ) )
	{
	  if ( ( k - 1 - l ) >= min_width )
	    intervals.push_back( std::make_pair( l+1, k ) );
	  l = k;
	}
    }
}


/**
 *  Compute the profile slope of the first meaningful scale
 *  interval computed by a simple linear regression model.
 *
 * @return a pair<bool, double> giving the slope and indicating if the
 * a meaningful scale was well found or not.
 *
 * @param min_slope the  minimum allowed slope for length evolution.  
 * @param max_slope the  maximum allowed slope for length evolution.
 * 
 **/


pair<bool, double> 
ImaGene::ScaleProfile::getSlopeFromMeaningfulScales(double max_slope, double min_slope, uint min_size) const {
  pair<bool, double> resuSlope(false, 0.0);
  vector< std::pair< uint, uint > > vectIntervals;
  this->meaningfulScales(vectIntervals, min_size,max_slope, min_slope);
  vector<double> vx;
  vector<double> vy;

  this->getProfile( vx, vy );
  // no meaningfulScale interval: 
  
  if(vectIntervals.size()!=0 && vectIntervals.at(0).first<vectIntervals.at(0).second ){
    resuSlope.first=true;
  }else{
    pair<uint, uint> interAll(1, vx.size());
    vectIntervals.push_back(interAll); 
  }
  SimpleLinearRegression SLR;


  uint mfsBeg = vectIntervals.at(0).first-1;
  uint mfsEnd = vectIntervals.at(0).second-1;


// Only two sampled points
  if ( mfsEnd-mfsBeg == 1 ){
    Vector2D pt1(vx.at(mfsBeg),vy.at(mfsBeg));
    Vector2D pt2(vx.at(mfsEnd),vy.at(mfsEnd));
    double slope = (pt2.y() - pt1.y())/(pt2.x() -pt1.x());
    resuSlope.second = slope;
    return resuSlope;
  }
  for(int i=mfsBeg; i<=mfsEnd; i++){
    SLR.addSample(vx.at(i), vy.at(i));
  }
  SLR.computeRegression();
  resuSlope.second = SLR.slope();

  return resuSlope;
}






/**
 * The noise level is the first scale of the first meaningful
 * scale. A meaningful scale is an interval of scales of length no
 * smaller than [min_width] and in which the profile has slopes
 * below [max_slope]. 
 *
 * @param min_width the minimum length for the meaningful scales.
 * @param max_slope the maximum allowed slope for length evolution.
 * @param min_slope the minimum allowed slope for length evolution.
 * @return the noise level or zero is none was found.
 * @see meaningfulScales
 */
uint
ImaGene::ScaleProfile::noiseLevel
( uint min_width,
  double max_slope,
  double min_slope ) const
{
  std::vector< std::pair< uint, uint > > intervals;
  this->meaningfulScales( intervals, min_width, max_slope, min_slope );
  return ( intervals.size() != 0 )
    ? intervals[ 0 ].first
    : 0;
}

/**
 * The noise level is the first scale of the first meaningful
 * scale. A meaningful scale is an interval of scales of length no
 * smaller than [min_width] and in which the profile has slopes
 * below [max_slope]. The lower bounded noise level also requires
 * minimum lenghs for different scales. Therefore the profile must
 * be greater that
 * [lower_bound_at_scale_1]+[lower_bound_slope]*scale.
 *
 * @param min_width the minimum length for the meaningful scales.
 * @param max_slope the maximum allowed slope for length evolution.
 * @param min_slope the minimum allowed slope for length evolution.
 * @param lower_bound_at_scale_1 the lower bound for the profile at scale 1.
 * @param lower_bound_slope the slope of the lower bound for the profile (for instance -1 for digital contours, -3 for digital image graphs since area values are divided by (scale)^3.
 * @return the noise level or zero is none was found.
 * @see meaningfulScales
 */
uint
ImaGene::ScaleProfile::lowerBoundedNoiseLevel
( uint min_width,
  double max_slope,
  double min_slope,
  double lower_bound_at_scale_1,
  double lower_bound_slope ) const
{
  std::vector< std::pair< uint, uint > > intervals;
  this->meaningfulScales( intervals, min_width, max_slope, min_slope );
  vector<double> x;
  vector<double> y;
  this->getProfile( x, y );
  for ( uint i = 0; i < intervals.size(); ++i )
    {
      bool above = true;
      for ( uint k = intervals[ i ].first; ( k <= intervals[ i ].second ); ++k )
	{
	  if ( y[ k ] < ( lower_bound_at_scale_1 + lower_bound_slope*x[ k ] ) )
	    {
	      above = false;
	      break;
	    }
	}
      if ( above ) return intervals[ i ].first;
    }
  // Failure. No detected noise level.
  return 0;
}





/**
 * It stops (and Erase) the stats saved values. It must be called to
 * avoid to store all statistics values when we have to acess to
 * the median value.  Typically if you nedd to access to the
 * median value of the profile, you need to follow this example:
 * @code
 * ScaleProfile sp;
 * // the value are now stored and you can access to the median value of the profile.
 * sp.init(true);
 * sp.addValue(0, 10.5);
 * sp.addValue(0, 9.2);
 *  ...
 * // When all values have been added you can stop to store them again
 * sp.stopAndEraseStatsSaving();
 * // before to erase the statistics sample values the median is computed and stored.
 * @endcode
 */

void 
ImaGene::ScaleProfile::stopStatsSaving()
{
  for ( uint i = 0; i < m_stats->size(); ++i ){
    ((*m_stats).at( i )).terminate();
  } 

}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ScaleProfile::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ScaleProfile]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ScaleProfile::OK() const
{
  return m_scales != 0;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////



