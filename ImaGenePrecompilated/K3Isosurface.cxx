///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : K3Isosurface.cxx
//
// Creation : 2002/08/11
//
// Version : 2002/08/11
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
//	2002/08/11 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnRCellSet.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnShapes.h"
#include "ImaGene/digitalnD/K3Isosurface.h"

// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/K3Isosurface.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const K3Isosurface_RCS_ID = "@(#)class K3Isosurface definition.";

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// /*----------------------------------------------------------------*/
// /* declaration de la table des configurations des cubes de voxels */
// /*----------------------------------------------------------------*/
// typedef int sommet[2];		/* le sommet d'une facette est    *
// 				 * situee sur l'arete dont les    *
// 				 * indices sont les 2 valeurs     *
// 				 * contenu dans sommet.		  */

// typedef sommet facette[3];	/* une facette est constituee de  *
// 				 * 3 sommets de facettes	  */


// typedef facette surfaceinter[6];/* la surface d'intersection entre *
// 				 * le cube de voxels et 	   *
// 				 * l'iso-surface est constituee au *
// 				 * maxi de 4 facettes triangulaires*/

// typedef struct general2		/* structure des cas generaux */
// {
//   int nb_facettes;		/* nombre de facettes dans *
// 				 * l'intersection	   */
//   surfaceinter tab_facettes;	/* tableau des facettes de *
// 				 * l'intersection	   */
// } tous_general2;

/*--------------------------------------------------------------------------*
 * declaration et initialisation de la table permettant -- a partir d'un    *
 * index -- d'acceder a la surface engendree par la configuration traduite  *
 * par l'index. Cette surface est une liste de facettes (au maximum 4). Une *
 * facette est une liste de 3 sommets connus par leur appartenance a une    *
 * arete du cube. Un sommet est un couple de numeros de sommet du cube.	    *
 *--------------------------------------------------------------------------*/
static K3Isosurface::tous_general2 cas_general[256] = 
  {  /* tables des config des cubes */
#include "ImaGene/digitalnD/cas.h" 
  };

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// class K3Isosurface
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
K3Isosurface::~K3Isosurface()
{
}

/**
 * Constructor.
 * @param ks this is the space where the isosurface is created.
 * @param lut this is the look-up table giving the triangles for each configuration (0 use default).
 */
K3Isosurface::K3Isosurface( const KnSpace & ks, const tous_general2* lut )
  : m_ks( ks )
{
  if ( lut == 0 ) lut = cas_general;
  init( lut, m_cfg_vtx, m_cfg_tris );
}


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Isosurface services ----------------------------

/**
 * Builds and returns the isosurface between "on" spels and "off" spels by
 * marching over the space.
 */
void
K3Isosurface::extract( const KnCharSet & spels,
		       KnRCellSet & vertices,
		       triangle_set & all_tris,
		       Kn_uid lower_bound,
		       Kn_uid upper_bound ) const
{

  // New version is faster than old one: old (~700), new (~400) for
  // 574104 vertices (surfels) and 1148224 triangles.
  uint i;
  uint x1 = m_ks.udecodeCoord( lower_bound, 0 );
  uint x2 = m_ks.udecodeCoord( upper_bound, 0 );
  uint y1 = m_ks.udecodeCoord( lower_bound, 1 );
  uint y2 = m_ks.udecodeCoord( upper_bound, 1 );
  uint z1 = m_ks.udecodeCoord( lower_bound, 2 );
  uint z2 = m_ks.udecodeCoord( upper_bound, 2 );
  Kn_uid dy = m_ks.ugetIncr( lower_bound, 1 ) - lower_bound;
  Kn_uid dz = m_ks.ugetIncr( lower_bound, 2 ) - lower_bound;
  Kn_uid start_z = lower_bound;
  for ( uint z = z1; z < z2; ++z )
    {
      Kn_uid start_y = start_z;
      for ( uint y = y1; y < y2; ++y )
      {
	Kn_uid p = start_y;
	uint cfg = spels[ p ] ? 2 : 0;
	cfg += spels[ p + dy ] ? 8 : 0;
	cfg += spels[ p + dz ] ? 32 : 0;
	cfg += spels[ p + dz + dy ] ? 128 : 0;
	for ( uint x = x1; x < x2; ++x )
	  {
	    // use the fact that half of the voxels were in the previous 
	    // configuration.
	    cfg = ( cfg & 0xaa ) >> 1 ;
	    cfg += spels[ p + 1 ] ? 2 : 0;
	    cfg += spels[ p + dy + 1 ] ? 8 : 0;
	    cfg += spels[ p + dz + 1 ] ? 32 : 0;
	    cfg += spels[ p + dz + dy + 1 ] ? 128 : 0;
	    Kn_uid base = m_ks.upointel( p );
	    for ( i = 0; i < m_cfg_vtx[ cfg ].nb_vtx; ++i )
	      vertices += base + m_cfg_vtx[ cfg ].vtx[ i ];
	    for ( i = 0; i < m_cfg_tris[ cfg ].nb_tris; ++i )
	      {
		all_tris.push_back( m_cfg_tris[ cfg ].tris[ i ] );
		all_tris.back().v[ 0 ] += base;
		all_tris.back().v[ 1 ] += base;
		all_tris.back().v[ 2 ] += base;
	      }
	    ++p;
	  }
	start_y += dy;
      }
      start_z += dz;
    }
  

//   // A liitle bit slower (~440) for 574104 vertices (surfels) and
//   // 1148224 triangles.
//   uint i;
//   Kn_uid p = lower_bound;
//   Kn_uid upper_bound2 = KnShapes::ugetCubeLowerBound( m_ks, upper_bound, 1 );
//   uint x1 = m_ks.udecodeCoord( p, 0 );
//   uint x2 = m_ks.udecodeCoord( upper_bound2, 0 );
//   Kn_uid dy = m_ks.ugetIncr( p, 1 ) - p;
//   Kn_uid dz = m_ks.ugetIncr( p, 2 ) - p;
//   do
//     {
//       uint cfg = spels[ p ] ? 2 : 0;
//       cfg += spels[ p + dy ] ? 8 : 0;
//       cfg += spels[ p + dz ] ? 32 : 0;
//       cfg += spels[ p + dz + dy ] ? 128 : 0;
//       for ( uint x = x1; x < x2; ++x )
// 	{
// 	  // use the fact that half of the voxels were in the previous 
// 	  // configuration.
// 	  cfg = ( cfg & 0xaa ) >> 1 ;
// 	  cfg += spels[ p + 1 ] ? 2 : 0;
// 	  cfg += spels[ p + dy + 1 ] ? 8 : 0;
// 	  cfg += spels[ p + dz + 1 ] ? 32 : 0;
// 	  cfg += spels[ p + dz + dy + 1 ] ? 128 : 0;
// 	  Kn_uid base = m_ks.upointel( p );
// 	  for ( i = 0; i < m_cfg_vtx[ cfg ].nb_vtx; ++i )
// 	    vertices += base + m_cfg_vtx[ cfg ].vtx[ i ];
// 	  for ( i = 0; i < m_cfg_tris[ cfg ].nb_tris; ++i )
// 	    {
// 	      all_tris.push_back( m_cfg_tris[ cfg ].tris[ i ] );
// 	      all_tris.back().v[ 0 ] += base;
// 	      all_tris.back().v[ 1 ] += base;
// 	      all_tris.back().v[ 2 ] += base;
// 	    }
// 	  ++p;
// 	}
//     }
//   while ( m_ks.uincrInBounds( p, lower_bound, upper_bound2 ) );

  
	
//   // Much slower (~700) for 574104 vertices (surfels) and
//   // 1148224 triangles.

//   uint i;
//   Kn_uid p = lower_bound;
//   Kn_uid upper_bound2 = KnShapes::ugetCubeLowerBound( m_ks, upper_bound, 1 );
//   Kn_uid dy = m_ks.ugetIncr( p, 1 ) - p;
//   Kn_uid dz = m_ks.ugetIncr( p, 2 ) - p;
//   do
//     {
//       // First compute configuration.
//       uint cfg = spels[ p ] ? 1 : 0;
//       cfg += spels[ p + 1 ] ? 2 : 0;
//       cfg += spels[ p + dy ] ? 4 : 0;
//       cfg += spels[ p + dy + 1 ] ? 8 : 0;
//       Kn_uid pz = p + dz;
//       cfg += spels[ pz ] ? 16 : 0;
//       cfg += spels[ pz + 1 ] ? 32 : 0;
//       cfg += spels[ pz + dy ] ? 64 : 0;
//       cfg += spels[ pz + dy + 1 ] ? 128 : 0;
//       Kn_uid base = m_ks.upointel( p );
//       for ( i = 0; i < m_cfg_vtx[ cfg ].nb_vtx; ++i )
// 	vertices += base + m_cfg_vtx[ cfg ].vtx[ i ];
//       for ( i = 0; i < m_cfg_tris[ cfg ].nb_tris; ++i )
// 	{
//  	  all_tris.push_back( m_cfg_tris[ cfg ].tris[ i ] );
//  	  all_tris.back().v[ 0 ] += base;
//  	  all_tris.back().v[ 1 ] += base;
//  	  all_tris.back().v[ 2 ] += base;
// 	}
      
//     }
//   while ( m_ks.uincrInBounds( p, lower_bound, upper_bound2 ) );
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
K3Isosurface::selfDisplay( ostream& that_stream ) const
{
  that_stream << endl;
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
K3Isosurface::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :


void
K3Isosurface::init( const tous_general2* lut, 
		    cfg_vertices* c_vtcs, cfg_triangles* c_tris )
{
  uint cfg, idx_facet, idx_vtx;
  // For each config, interpret "cas_general" to compute vertices and triangles
  for ( cfg = 0; cfg < 256; ++cfg )
    {
      const tous_general2* the_cfg = lut + cfg;
      // Copy all facets.
      for ( idx_facet = 0; 
	    idx_facet < (unsigned int) the_cfg->nb_facettes;
	    ++idx_facet )
	{
	  const facette* the_facet = the_cfg->tab_facettes + idx_facet;
	  triangle t;
	  for ( idx_vtx = 0; idx_vtx < 3; ++idx_vtx )
	    {
	      const sommet* the_vtx = (*the_facet) + idx_vtx ;
	      Kn_sid v = codeIsoVertexAsSurfel( cfg, 
						(*the_vtx)[ 0 ], 
						(*the_vtx)[ 1 ] );
	      t.v[ idx_vtx ] = v;
	      c_vtcs[ cfg ].add( v );
	    }
	  c_tris[ cfg ].add( t );
	}
    }
}

Kn_sid 
K3Isosurface::codeIsoVertexAsSurfel( uint cfg, uint v1, uint v2 ) const
{
  bool v1_int;
  uint k;
  uint code = ( v1 >= v2 ) ? v1 - v2 : v2 - v1;
  // check if v1 is int.
  if ( cfg & ( 1 << v1 ) )
    {
      v1_int = true;
      ASSERT_K3Isosurface( ! ( cfg & ( 1 << v2 ) ) );
    }
  else
    {
      v1_int = false;
      ASSERT_K3Isosurface( ! ( cfg & ( 1 << v1 ) ) );
    }
  Kn_size x[ 3 ];
  x[ 0 ] = ( v1 & 1 ) ? 3 : 1;
  x[ 1 ] = ( v1 & 2 ) ? 3 : 1;
  x[ 2 ] = ( v1 & 4 ) ? 3 : 1;
  switch( code ) 
    {
    case 1: k = 0; break;
    case 2: k = 1; break;
    case 4: k = 2; break;
    default:
      ASSERT_K3Isosurface( false );
      k = 0;
    }
  Kn_sid voxel = m_ks.skcode( x, v1_int ? KnTypes::POS : KnTypes::NEG );
  Kn_sid surfel = m_ks.sincident( voxel, k, true );
  return surfel;
}


//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
