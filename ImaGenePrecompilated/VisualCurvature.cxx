///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : VisualCurvature.cxx
//
// Creation : 2009/02/16
//
// Version : 2009/02/16
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
//	2009/02/16 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/SampledFunction.h"
#include "ImaGene/helper/VisualCurvature.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/VisualCurvature.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const VisualCurvature_RCS_ID = "@(#)class VisualCurvature definition.";



///////////////////////////////////////////////////////////////////////////////
// class VisualCurvature
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::VisualCurvature::~VisualCurvature()
{
}

/**
 * Constructor. The object is not valid yet.
 * @see init
 */
ImaGene::VisualCurvature::VisualCurvature()
{
}
    
/** 
 * Initialization from polygon.
 *
 * @param n the number of directions used for computing the visual
 * curvature (the more the more accurate it is, but the more
 * costly it is).
 *
 * @param x the x-coordinates of polygon vertices.
 * @param y the y-coordinates of polygon vertices.
 *
 * @param closed when 'true' the polygon is closed, otherwise it
 * is an open polygonal line.
 */
void
ImaGene::VisualCurvature::init( uint n,
				const vector<double> & x, 
				const vector<double> & y, 
				bool closed )
{
  ASSERT_VisualCurvature
    ( x.size() == y.size() 
      && "[ImaGene::VisualCurvature::init] x and y should be of the same size" );
  uint k = x.size();
  m_fcts.resize( n );
  // For all directions
  for ( uint d = 0; d < n; ++d )
    {
      double dir = direction( d );
      double cdir = cos( dir );
      double sdir = sin( dir );
      vector<double> s;
      vector<double> xs;
      vector<double> ys;
      double arclength = 0.0;
      for ( uint i = 0; i < k; ++i )
	{
	  xs.push_back(  cdir * x[ i ] + sdir * y[ i ] );
	  ys.push_back( -sdir * x[ i ] + cdir * y[ i ] );
	  s.push_back( arclength );
	  double ds = sqrt( ( x[ ( i + 1 ) % k ] - x[ i ] ) 
			    * ( x[ ( i + 1 ) % k ] - x[ i ] ) 
			    + ( y[ ( i + 1 ) % k ] - y[ i ] )  
			    * ( y[ ( i + 1 ) % k ] - y[ i ] ) );
	  arclength += ds;
	}
      // We can initialize the sampled function.
      // cerr << "[ImaGene::VisualCurvature::init] SF closed="
      // 	   << closed << " arclength=" << arclength << endl;
      m_fcts[ d ].init( s, ys, closed, closed ? arclength : 0.0 );
    }
}

/**
 * Computes the visual curvature at position s with window ds.
 *
 * @param s the arclength of the position.
 * @param ds the window for the computation.
 * @return the visual curvature.
 */
double
ImaGene::VisualCurvature::visualCurvature( double s, double ds ) const
{
  uint nb = 0;
  for ( uint k = 0; k < m_fcts.size(); ++k )
    {
      nb += m_fcts[ k ].numberExtremePoints( s - ds / 2.0, s + ds / 2.0 );
    }
  return M_PI * nb / ( m_fcts.size() * ds );
}

/**
 * Computes the multi-scale visual curvature at position s with
 * window ds.
 *
 * @param s the arclength of the position.
 * @param ds the window for the computation.
 * @param lambda the scale (between 0.0, finest, and 1.0, coarsest).
 * @return the multi-scale visual curvature.
 */
double
ImaGene::VisualCurvature::msVisualCurvature
( double s, double ds, double lambda, bool normalized ) const
{
  uint nb = 0;
  for ( uint k = 0; k < m_fcts.size(); ++k )
    {
      nb += m_fcts[ k ].numberSignificantExtremePoints( s - ds / 2.0, 
							s + ds / 2.0,
							lambda, normalized );
    }
  return M_PI * nb / ( m_fcts.size() * ds );
}



/**
 * Computes the algebraic visual curvature at position s with window
 * ds.  The difference with visual curvature is that concave and
 * convex portions cancel each other. For instance, a digital straight
 * line has much less algebraic visual curvature than visual
 * curvature.
 *
 * @param s the arclength of the position.
 * @param ds the window for the computation.
 * @return the visual curvature.
 * @see visualCurvature
 */
double
ImaGene::VisualCurvature::algebraicVisualCurvature( double s, double ds ) const
{
  int nb = 0;
  for ( uint k = 0; k < m_fcts.size(); ++k )
    {
      nb += m_fcts[ k ].algebraicNumberExtremePoints( s - ds / 2.0, 
						      s + ds / 2.0 );
    }
  return M_PI * nb / ( m_fcts.size() * ds );
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::VisualCurvature::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[VisualCurvature]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::VisualCurvature::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
