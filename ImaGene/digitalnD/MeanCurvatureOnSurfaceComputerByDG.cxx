///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : MeanCurvatureOnSurfaceComputerByDG.cxx
//
// Creation : 2003/02/28
//
// Version : 2003/02/28
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
//	2003/02/28 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
//#include "ImageLib/Gauss/DG.hpp"
#include "ImaGene/mathutils/DG.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/MeanCurvatureOnSurfaceComputerByDG.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/MeanCurvatureOnSurfaceComputerByDG.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const MeanCurvatureOnSurfaceComputerByDG_RCS_ID = "@(#)class MeanCurvatureOnSurfaceComputerByDG definition.";

///////////////////////////////////////////////////////////////////////////////
// class MeanCurvatureOnSurfaceComputerByDG
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
MeanCurvatureOnSurfaceComputerByDG::~MeanCurvatureOnSurfaceComputerByDG()
{
  if ( m_filter != 0 )
    delete[] m_filter;
}

/**
 * Constructor.
 * @param sigma the standard deviation of the Gaussian derivative.
 * @param size the discrete corresponding has a size '2*size+1'.
 * @param delta the distance between two points of the filter.
 * @param epsilon the smallest value of the filter for which a computation is done.
 */
MeanCurvatureOnSurfaceComputerByDG::MeanCurvatureOnSurfaceComputerByDG
( float sigma,
  uint size,
  float delta,
  float epsilon )
  : m_filter( 0 )
{
  setParameters( sigma, size, delta, epsilon );
}

/**
 * Changes the parameters of the Gaussian derivative.
 * @param sigma the standard deviation of the Gaussian derivative.
 * @param size the discrete corresponding has a size '2*size+1'.
 * @param delta the distance between two points of the filter.
 * @param epsilon the smallest value of the filter for which a computation is done.
 */
void
MeanCurvatureOnSurfaceComputerByDG::setParameters
( float sigma,
  uint size,
  float delta, 
  float epsilon )
{
  if ( m_filter != 0 )
    delete[] m_filter;
  m_filter = new float[ 2*size+1 ];
  m_size = size;
  m_sigma = sigma;
  m_delta = delta;
  m_epsilon = epsilon;
  //  imagelib::DG dg( sigma );
  DG dg( sigma );
  float fx = 0.0;
  float bx = 0.0;
  m_filter[ size ] = dg( 0.0 );
  uint i;
  for ( i = 1; i <= size; ++i )
    {
      fx += delta;
      bx -= delta;
      m_filter[ size + i ] = dg( fx );
      m_filter[ size - i ] = dg( bx );
    }
  m_size_for_computation = 1;
  for ( i = 0; i < size; i++ )
    if ( fabs( m_filter[ i ] ) >= epsilon )
      {
	m_size_for_computation = size - i;
	break;
      }
  m_size_length_for_computation = m_size_for_computation * delta;
  cout << "[MeanCurvatureOnSurfaceComputerByDG size=" << m_size
       << " sizec=" << m_size_for_computation
       << " sizelc=" << m_size_length_for_computation << "]" << endl;
}


// ------------------------- Implemented Geometry services ------------------

/**
 * Uses [surf_geom_ctxt] to get normals and lengths.
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @return the estimated mean curvature at surfel [b].
 */
float
MeanCurvatureOnSurfaceComputerByDG::computeMeanCurvature
( DigitalSurfaceGeometry & surf_geom_ctxt, 
  Kn_sid b )
{
  const KnSpace* space = surf_geom_ctxt.space();
  ASSERT_MeanCurvatureOnSurfaceComputerByDG( space->sisSurfel( b ) );

  float sum = 0.0; // summation for convolution.
  // Look in each direction.
  KnSpace::dir_iterator q = space->sbegin_dirs( b );
  Vector n( space->dim() );
  uint orth_dir = space->sorthDir( b );
  while ( ! q.end() )
    {
      // Going forward.
      uint i = *q; // current direction
      Vector2i tgt2d( surf_geom_ctxt.tangent2D( b, i ) );
      float norm = tgt2d.norm();
      float t1_x = tgt2d.x() / norm;
      float t1_y = tgt2d.y() / norm;
      
      float sum_on_this_dir = 0.0;
      C4CIteratorOnSurface* pfwd = 
	surf_geom_ctxt.surface()->newC4CIterator( b, i );
      float t1 = 0.0;
      float length_b = surf_geom_ctxt.length( b, i );
      float prev_length = length_b;
      uint nb = 1;
      while ( ( pfwd->next() != 0 ) && ( nb <= 5 ) )
      // while ( pfwd->next() != 0 )
	{
	  Kn_sid bcur = pfwd->current();
	  // JOL 2003/07/18: the tangent direction changes along the contour.
	  // float next_length = surf_geom_ctxt.length( bcur, i );
	  uint current_orth_dir = space->sorthDir( bcur );
	  float next_length 
	    = surf_geom_ctxt.length( bcur, 
				     (i == current_orth_dir) ? orth_dir : i );
	  // JOL END
	  t1 += ( next_length + prev_length ) / 2.0;
	  // Checks if useless to compute this part.
	  if ( t1 > m_size_length_for_computation ) break;
	  
	  surf_geom_ctxt.normal( bcur, n );
	  sum_on_this_dir 
	    += ( n.ro( i ) * t1_x + n.ro( orth_dir ) * t1_y )
	    * gaussianDerivative( -t1 ) * next_length ;
	  prev_length = next_length;
	  ++nb;
	}
      delete pfwd;
      // Going Backward.
      C4CIteratorOnSurface* pbwd = 
	surf_geom_ctxt.surface()->newC4CIterator( b, i );
      t1 = 0.0;
      prev_length = length_b;
      nb = 1;
      while ( ( pbwd->previous() != 0 ) && ( nb <= 5 ) )
	// while ( pbwd->previous() != 0 )
	{
	  Kn_sid bcur = pbwd->current();
	  // JOL 2003/07/18: the tangent direction changes along the contour.
	  // float next_length = surf_geom_ctxt.length( bcur, i );
	  uint current_orth_dir = space->sorthDir( bcur );
	  float next_length 
	    = surf_geom_ctxt.length( bcur, 
				     (i == current_orth_dir) ? orth_dir : i );
	  // JOL END
	  t1 += ( next_length + prev_length ) / 2.0;
	  // Checks if useless to compute this part.
	  if ( t1 > m_size_length_for_computation ) break;

	  surf_geom_ctxt.normal( bcur, n );
	  sum_on_this_dir += 
	    ( n.ro( i ) * t1_x + n.ro( orth_dir ) * t1_y )
	    * gaussianDerivative( t1 ) * next_length;
	  prev_length = next_length;
	  ++nb;
	}
      delete pbwd;
      sum += space->sdirect( b, i ) ? sum_on_this_dir : -sum_on_this_dir;
      // Go to next tracking direction.
      ++q;
    }
  return sum;
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
MeanCurvatureOnSurfaceComputerByDG::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[MeanCurvatureOnSurfaceComputerByDG]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
MeanCurvatureOnSurfaceComputerByDG::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
