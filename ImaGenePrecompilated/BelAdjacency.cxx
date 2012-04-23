///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : BelAdjacency.cxx
//
// Creation : 2003/02/06
//
// Version : 2003/02/06
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
//	2003/02/06 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnUtils.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/BelAdjacency.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const BelAdjacency_RCS_ID = "@(#)class BelAdjacency definition.";

///////////////////////////////////////////////////////////////////////////////
// ------------------------- BelAdjacency::bel_iterator -----------------------

/**
 * Constructor.
 * @param ks the digital space in which the bel adjacency is defined.
 * @param c the starting bel.
 * @param k the tangent direction to [c] that will be tracked.
 * @param interior when 'true', the bel iterator tracks interior adjacent bels, otherwise exterior ones.
 */
BelAdjacency::bel_iterator::bel_iterator( const KnSpace & ks, 
					  Kn_sid c, 
					  uint k, 
					  bool interior )
  : m_ks( ks ),
    m_bel( c ),
    //    m_tangent_dir( k ),
    m_interior( interior )
{
  // Do some precomputations.
  m_orth_dir = ks.sorthDir( c );

  ASSERT_BelAdjacency( ks.sisSurfel( c ) && ( m_orth_dir != k ) );

  m_orth_direct = ks.sdirect( c, m_orth_dir );
  m_vox_int = ks.unsigns( ks.sincident( c, m_orth_dir, m_orth_direct ) );
  m_vox_ext = ks.uadjacent( m_vox_int, m_orth_dir, ! m_orth_direct );
  m_track_dir = k; // tangent and track are identical at beginning.
  m_track_direct = ks.sdirect( c, m_track_dir );
}

/**
 * Initializer.
 * @param c the new starting bel.
 * @param k the new tangent direction to [c] that will be tracked.
 * @param interior when 'true', the bel iterator tracks interior adjacent bels, otherwise exterior ones.
 */
void 
BelAdjacency::bel_iterator::init( Kn_sid c, uint k, bool interior )
{
  m_bel = c;
  // m_tangent_dir = k;
  m_interior = interior;
  
  // Do some precomputations.
  m_orth_dir = m_ks.sorthDir( c );

  ASSERT_BelAdjacency( m_ks.sisSurfel( c ) && ( m_orth_dir != k ) );

  m_orth_direct = m_ks.sdirect( c, m_orth_dir );
  m_vox_int = m_ks.unsigns( m_ks.sincident( c, m_orth_dir, m_orth_direct ) );
  m_vox_ext = m_ks.uadjacent( m_vox_int, m_orth_dir, ! m_orth_direct );
  m_track_dir = k; // tangent and track are identical at beginning.
  m_track_direct = m_ks.sdirect( c, m_track_dir );
}



/**
 * Go to the next direct or indirect exterior adjacent bel on the boundary
 * of [obj].
 * @param obj a set of unoriented spels.
 * @param indirect when 'false', looks for direct followers, otherwise looks for indirect followers.
 * @return 1 if the move was the first interior, 2 if it was the second interior, 3 if it was the third interior.
 */
uint 
BelAdjacency::bel_iterator::nextExtOnBdry( const KnCharSet & obj,
					   bool indirect )
{
  // Exterior bel adjacency.
  bool pos = indirect ^ m_track_direct; // direction along tracking axis.
  
  // Check first next bel.
  Kn_uid ext_prime = m_ks.uadjacent( m_vox_ext, m_track_dir, pos );
  if ( obj[ ext_prime ] )
    {
      m_bel = follower3( pos );
      m_vox_int = ext_prime;
      KnUtils::swap( m_orth_dir, m_track_dir );
      m_orth_direct = m_ks.sdirect( m_bel, m_orth_dir );
      m_track_direct = m_ks.sdirect( m_bel, m_track_dir );

      return 3;
    }
  // Check second next bel.
  Kn_uid int_prime = m_ks.uadjacent( m_vox_int, m_track_dir, pos );
  if ( obj[ int_prime ] )
    {
      m_bel = follower2( pos );
      m_vox_int = int_prime;
      m_vox_ext = ext_prime;
      return 2;
    }
  // The third one is then the right one.
  m_bel = follower1( pos );
  m_vox_ext = int_prime;
  KnUtils::swap( m_orth_dir, m_track_dir );
  m_orth_direct = m_ks.sdirect( m_bel, m_orth_dir );
  m_track_direct = m_ks.sdirect( m_bel, m_track_dir );
  return 1;
}

/**
 * Go to the next direct or indirect interior adjacent bel on the boundary
 * of [obj].
 * @param obj a set of unoriented spels.
 * @param indirect when 'false', looks for direct followers, otherwise looks for indirect followers.
 * @return 1 if the move was the first interior, 2 if it was the second interior, 3 if it was the third interior.
 */
uint 
BelAdjacency::bel_iterator::nextIntOnBdry( const KnCharSet & obj,
					   bool indirect )
{
  // Interior bel adjacency.
  bool pos = indirect ^ m_track_direct; // direction along tracking axis.

  // Check first next bel.
  Kn_uid int_prime = m_ks.uadjacent( m_vox_int, m_track_dir, pos );
  if ( ! obj[ int_prime ] )
    {
      m_bel = follower1( pos );
      m_vox_ext = int_prime;
      KnUtils::swap( m_orth_dir, m_track_dir );
      m_orth_direct = m_ks.sdirect( m_bel, m_orth_dir );
      m_track_direct = m_ks.sdirect( m_bel, m_track_dir );
      return 1;
    }
  // Check second next bel.
  Kn_uid ext_prime = m_ks.uadjacent( m_vox_ext, m_track_dir, pos );
  if ( ! obj[ ext_prime ] )
    {
      m_bel = follower2( pos ); 
      m_vox_int = int_prime;
      m_vox_ext = ext_prime;
      return 2;
    }
  // The third one is then the right one.
  m_bel = follower3( pos ); 
  m_vox_int = ext_prime;
  KnUtils::swap( m_orth_dir, m_track_dir );
  m_orth_direct = m_ks.sdirect( m_bel, m_orth_dir );
  m_track_direct = m_ks.sdirect( m_bel, m_track_dir );
  return 3;
}



///////////////////////////////////////////////////////////////////////////////
// class BelAdjacency
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
BelAdjacency::~BelAdjacency()
{
}


/**
 * Constructor.
 * @param ks the digital space in which the bel adjacency is defined.
 * @param interior when 'true', the bel adjacency is interior for any coordinate pair, when 'false', it is exterior for any one.
 */
BelAdjacency::BelAdjacency( const KnSpace & ks, bool interior )
  : m_ks( ks ),
    m_interior( ks.dim() * ks.dim() )
{
  uint i, j;
  uint offset = 0;
  for ( j = 0; j < ks.dim(); ++j )
    for ( i = 0; i < ks.dim(); ++i )
      m_interior[ offset++ ] = interior;
}


/**
 * Set a coordinate pair as interior or exterior in the bel adjacency.
 * @param i first coordinate.
 * @param j second coordinate ('j != i').
 * @param interior when 'true', the bel adjacency is interior for the coordinate pair '(i,j)', when 'false', it is exterior.
 */
void
BelAdjacency::setAdjacency( uint i, uint j, bool interior )
{
  ASSERT_BelAdjacency( ( i < m_ks.dim() ) && ( j < m_ks.dim() ) );
  m_interior[ i * m_ks.dim() + j ] = interior;
  m_interior[ j * m_ks.dim() + i ] = interior;
}

/**
 * Returns whether a coordinate pair is interior (true) or exterior (false) in 
 * the bel adjacency.
 * @param i first coordinate.
 * @param j second coordinate ('j != i').
 * @return 'true' if the bel adjacency is interior for the coordinate pair '(i,j)', 'false' when it is exterior.
 */
bool
BelAdjacency::getAdjacency( uint i, uint j ) const
{
  ASSERT_BelAdjacency( ( i < m_ks.dim() ) && ( j < m_ks.dim() ) );
  return m_interior[ i * m_ks.dim() + j ];
}
  
/**
 * Constructs a bel iterator starting at surfel [c] with tangent 
 * direction [k].
 * @param c any oriented surfel.
 * @param k any coordinate ('k != orthDir( c )')
 * @return a bel iterator.
 */
BelAdjacency::bel_iterator
BelAdjacency::sstart( Kn_sid c, uint k ) const
{
  ASSERT_BelAdjacency( m_ks.sisSurfel( c ) );
  uint orth_dir = m_ks.sorthDir( c );
  return bel_iterator( m_ks, c, k, m_interior[ k * m_ks.dim() + orth_dir ] );
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
BelAdjacency::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[BelAdjacency]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
BelAdjacency::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
