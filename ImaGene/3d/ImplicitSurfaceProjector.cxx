///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ImplicitSurfaceProjector.cxx
//
// Creation : 2008/03/31
//
// Version : 2008/03/31
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
//	2008/03/31 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/3d/ImplicitSurfaceProjector.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/3d/ImplicitSurfaceProjector.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ImplicitSurfaceProjector_RCS_ID = "@(#)class ImplicitSurfaceProjector definition.";



///////////////////////////////////////////////////////////////////////////////
// class ImplicitSurfaceProjector
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ImplicitSurfaceProjector::~ImplicitSurfaceProjector()
{
}


/**
 * Initializes the object with the implicit surface and the
 * expected precision.
 *
 * @param dim the dimension of the domain.
 * @param is the implicit surface (the object is only referenced).
 * @param eps the required precision.
 * @param nmax the maximum number of iteration.
 */
void 
ImaGene::ImplicitSurfaceProjector::init( uint dim, 
					 const ImplicitSurface & is, 
					 float eps,
					 uint nmax )
{
  m_dim = dim;
  m_is = &is;
  m_eps = eps;
  m_nmax = nmax;
}

    
// ----------------------- Projection services ----------------------------

/**
 * Project the point [xp] onto some zero-level of this implicit
 * surface [m_is] with precision [m_eps] (meaning '|m_is( xp )| <
 * m_eps' at the end of the process.
 *
 * @param xp (returns) a point close to f=0.
 * @param x0 initial point.
 *
 * @return 'true' if value is close to zero, 'false' if the projection
 * stopped because the number of alloted iterations has been exceded.
 */
bool
ImaGene::ImplicitSurfaceProjector::project( float* xp, 
					    const float* x0 ) const
{
  ASSERT_ImplicitSurfaceProjector( m_is != 0 );

  float x[ m_dim + 1 ];
  float gx[ m_dim + 1 ];
  float f;
  float g2;
  for ( uint i = 0; i < m_dim; ++i ) 
    x[ i ] = x0[ i ];
  x[ m_dim ] = 0.0f;
  xp[ m_dim ] = 0.0f;
  for ( uint n = 0; n < m_nmax; ++n )
    {
      // One iteration, xp is x_n+1 and x is x_n.
      f = m_is->operator()( x );
//       cerr << "x=(" << x[ 0 ] << " " << x[ 1 ] << " " << x[ 2 ]
// 	   << "), f=" << f;
      if ( fabs( f ) < m_eps ) 
	{
	  for ( uint i = 0; i < m_dim; ++i ) 
	    xp[ i ] = x[ i ];
	  return true;
	}
      m_is->gradient( x, gx );
      g2 = 0.0f;
      for ( uint i = 0; i < m_dim; ++i ) 
	g2 += Mathutils::sqr( gx[ i ] );
//       cerr << " gx=(" << gx[ 0 ] << " " << gx[ 1 ] << " " << gx[ 2 ]
// 	   << "), g2=" << g2 << endl;
      for ( uint i = 0; i < m_dim; ++i ) 
	xp[ i ] = x[ i ] - 0.5f * f * gx[ i ] / g2;

      // One iteration, x is x_n+1 and xp is x_n.
      f = m_is->operator()( xp );
//       cerr << "x=(" << xp[ 0 ] << " " << xp[ 1 ] << " " << xp[ 2 ]
// 	   << "), f=" << f;
      if ( fabs( f ) < m_eps ) 
	return true;
      m_is->gradient( xp, gx );
      g2 = 0.0f;
      for ( uint i = 0; i < m_dim; ++i ) 
	g2 += Mathutils::sqr( gx[ i ] );
//       cerr << " gx=(" << gx[ 0 ] << " " << gx[ 1 ] << " " << gx[ 2 ]
// 	   << "), g2=" << g2 << endl;
      for ( uint i = 0; i < m_dim; ++i ) 
	x[ i ] = xp[ i ] - 0.5f * f * gx[ i ] / g2;
    }

  // Failure in approaching 0. Decides if projection is any better.
  if ( fabs( f ) > fabs( m_is->operator()( x0 ) ) )
    for ( uint i = 0; i < m_dim; ++i ) 
      xp[ i ] = x0[ i ];
  return false;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ImplicitSurfaceProjector::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ImplicitSurfaceProjector]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ImplicitSurfaceProjector::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
