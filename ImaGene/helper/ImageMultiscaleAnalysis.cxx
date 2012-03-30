///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ImageMultiscaleAnalysis.cxx
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
#include "ImaGene/helper/ImageMultiscaleAnalysis.h"
#include "ImaGene/image/Image2DUtils.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/ImageMultiscaleAnalysis.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ImageMultiscaleAnalysis_RCS_ID = "@(#)class ImageMultiscaleAnalysis definition.";



///////////////////////////////////////////////////////////////////////////////
// class ImageMultiscaleAnalysis
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ImageMultiscaleAnalysis::~ImageMultiscaleAnalysis()
{
  clear();
}

/**
 * Constructor. 
 */
ImaGene::ImageMultiscaleAnalysis::ImageMultiscaleAnalysis()
  : m_scales( 0 ), m_emaps( 0 )
{
}

/**
 * Clear object. The object is as it was just instantiated.
 */
void 
ImaGene::ImageMultiscaleAnalysis::clear()
{
  if ( m_scales != 0 )
    {
      delete m_scales;
      m_scales = 0;
    }
  if ( m_emaps != 0 )
    {
      for ( uint i = 0; i < m_emaps->size(); ++i )
	delete (*m_emaps)[ i ];
      delete m_emaps;
      m_emaps = 0;
    }
}

/**
 * Initializes the object with an image as well as the maximal
 * scale to consider.
 *
 * @param input any grey-level image.
 * @param max_scale the maximal scale for the analysis.
 */
void
ImaGene::ImageMultiscaleAnalysis::init
( const ByteImage2D & input, uint max_scale )
{
  clear();
  m_scales = new vector<float>( max_scale );
  m_emaps = new vector<DigitalElevationMapStats*>( max_scale );
  for ( uint i = 0; i < max_scale; ++i )
    {
      (*m_scales)[ i ] = (float)( i + 1 );
      (*m_emaps)[ i ] = new DigitalElevationMapStats;
    }
  ByteImage2D smoothed_image;
  ByteImage2D output;
  Image2DUtils<ByteImage2D> image_utils;
  for ( uint i = 0; i < max_scale; ++i )
    {
      cerr << i << "(" << flush;
      uint r = (uint) round( (*m_scales)[ i ] );
      // cut-off frequency is inverse of resolution.
      double sigma = (*m_scales)[ i ]; 
      image_utils.gaussianSmoothing( smoothed_image, input, sigma );
      cerr << "G" << flush;
      Pixel x0( 0, 0 );
      image_utils.subsample( output, smoothed_image, r, r, x0, r );
      cerr << "S" << flush;
      (*m_emaps)[ i ]->init( output );
      cerr << "E" << flush;
      (*m_emaps)[ i ]->computeStats();
      cerr << "C)." << flush;
    }
  cerr << endl;
}

/**
 * Returns the scale profile at the given cell (specified by its
 * Khalimsky coordinates in the base image.
 *
 * @param sp (returns) the scale profile.
 * @param kx the Khalimsky x-coordinate of the cell (doubled). 
 * @param ky the Khalimsky y-coordinate of the cell (doubled). 
 */
void 
ImaGene::ImageMultiscaleAnalysis::getScaleProfile
( ScaleProfile & sp, int kx, int ky ) const
{
  sp.clear();
  sp.init( m_scales->begin(), m_scales->end() );
  Kn_size sizes[ 2 ];
  for ( uint idx = 0; idx < m_scales->size(); ++idx )
    {
      int r = (int) round( (*m_scales)[ idx ] );
      int x = ( kx / 2 ) / r;
      int y = ( ky / 2 ) / r;
      sizes[ 0 ] = 2*x + ( kx & 0x1 ); 
      sizes[ 1 ] = 2*y + ( ky & 0x1 );
      const KnSpace* ks = (*m_emaps)[ idx ]->space2D();
      Kn_uid cell = ks->ukcode( sizes );
      ASSERT_ImageMultiscaleAnalysis( ks->udim( cell ) == 1 );
      sp.addStatistic( idx, (*m_emaps)[ idx ]->stat( cell ) );
    }
} 



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ImageMultiscaleAnalysis::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ImageMultiscaleAnalysis]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ImageMultiscaleAnalysis::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
