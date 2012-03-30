///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : NormalOnSurfaceComputerByUmbrellas.cxx
//
// Creation : 2009/05/18
//
// Version : 2009/05/18
//
// Author : Laurent Provot
//
// email : provot@loria.fr
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
#include "ImaGene/digitalnD/KnRCellSet.h"
#include "ImaGene/digitalnD/SurfelNeighborhood.h"
#include "ImaGene/digitalnD/DigitalSurfaceUmbrellaComputer.h"
#include "ImaGene/digitalnD/ObjectBoundary.h"
#include "ImaGene/digitalnD/NormalOnSurfaceComputerByUmbrellas.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/NormalOnSurfaceComputerByUmbrellas.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const NormalOnSurfaceComputerByUmbrellas_RCS_ID = "@(#)class NormalOnSurfaceComputerByUmbrellas definition.";

///////////////////////////////////////////////////////////////////////////////
// class NormalOnSurfaceComputerByUmbrellas
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
NormalOnSurfaceComputerByUmbrellas::~NormalOnSurfaceComputerByUmbrellas()
{
}

/**
 * Constructor.
 * @param order the number of "rings" of umbrellas taken into account to estimate the normal vector
 * @param weighted whether or not the estimation is done using a weighted summation.
 */
NormalOnSurfaceComputerByUmbrellas::NormalOnSurfaceComputerByUmbrellas
( uint order,
  bool weighted )
  : m_order(order), m_weighted(weighted)
{
}

// ------------------------- Geometry services ------------------------------

/**
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
 */
void 
NormalOnSurfaceComputerByUmbrellas::computeNormal
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b, Vector & vn )
{
  if (m_weighted)
    computeWeightedNormal(surf_geom_ctxt, b, vn);
  else
    computeSimpleNormal(surf_geom_ctxt, b, vn);
}

// ------------------------ Normal vector services --------------------------

/**
 * Computes normals on [surf_geom_ctxt] using a bel neighborhood depending on the current order.
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
 * @see m_order
 */
void
NormalOnSurfaceComputerByUmbrellas::computeSimpleNormal
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b,
  Vector & vn )
{
  const KnSpace & ks = *surf_geom_ctxt.space();
  KnRCellSet surfels = KnRCellSet::create( ks, ks.dim() - 1, true );
  surfels += b;
  uint i;
  for ( i = 0; i < m_order; ++i )
    extendNeighborhoodWithUmbrellas( surf_geom_ctxt, surfels );

  vn.zero();
  for ( KnRCellSet::cell_iterator c_it = surfels.begin();
        c_it != surfels.end();
        ++c_it )
    vn += ks.snormalInside( *c_it );

  float norm = 0.0;
  for ( i = 0; i < ks.dim(); ++i )
    norm += vn.ro( i ) * vn.ro( i );
  vn /= sqrt( norm );
}

/**
 * Computes normals on [surf_geom_ctxt] using a summation of simple normals in the order-zero umbrella of [b].
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param b a signed surfel.
 * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
 * @see comupteSimpleNormal, m_order
 */
void
NormalOnSurfaceComputerByUmbrellas::computeWeightedNormal
( DigitalSurfaceGeometry & surf_geom_ctxt,
  Kn_sid b,
  Vector & vn )
{
  const KnSpace & ks = *surf_geom_ctxt.space();
  KnRCellSet surfels = KnRCellSet::create( ks, ks.dim() - 1, true );
  surfels += b;
  ks.snormalInside( b, vn );
  uint i;
  for ( i = 0; i < m_order; ++i )
  {
    extendNeighborhoodWithUmbrellas( surf_geom_ctxt, surfels );

    for ( KnRCellSet::cell_iterator c_it = surfels.begin();
          c_it != surfels.end();
          ++c_it )
      vn += ks.snormalInside( *c_it );
  }

  float norm = 0.0;
  for ( i = 0; i < ks.dim(); ++i )
    norm += vn.ro( i ) * vn.ro( i );
  vn /= sqrt( norm );
}

/**
 * Extends a set of surfels [surfels_set] by adding the surfels not in [surfels_set] that belong to umbrellas of pointels of [surfels_set]. [surfels_set] must contain at least one surfel.
 * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
 * @param surfels_set (returns) the connected set of surfels to extend.
 */
void
NormalOnSurfaceComputerByUmbrellas::extendNeighborhoodWithUmbrellas
( DigitalSurfaceGeometry & surf_geom_ctxt,
  KnRCellSet & surfels_set )
{
  // ASSERT_NormalOnSurfaceComputerByUmbrellas(surfels_set.nbElements() != 0);

  KnRCellSet original_set = surfels_set;

  const KnSpace & ks = *surf_geom_ctxt.space();
  DigitalSurface & surf = *surf_geom_ctxt.surface();

  BelAdjacency adjacency( ks );
  SurfelNeighborhood neighborhood;
  neighborhood.init( &ks, &adjacency );

  // the set of umbrellas to compute to extend the surfels_set
  std::vector<DigitalSurfaceUmbrellaComputer::State> umbrellas_to_compute;
  DigitalSurfaceUmbrellaComputer::State umbrella_state;
  Kn_sid s;
  KnRCellSet::cell_iterator c_it = original_set.begin();
  DigitalSurfaceTracker * tracker = surf.newTracker( *c_it );

  // find all the surfels 's1' of surfels_set that have an adjacent
  // surfel 's2' on the surface that doesn't belong to surfels_set,
  // and save the state of an umbrella to compute, that starts
  // from 's1' and has a tracking direction from 's1' to 's2'
  for ( ; c_it != original_set.end(); ++c_it )
  {
    neighborhood.move( *c_it );
    for ( KnSpace::dir_iterator dir = ks.sbegin_dirs( *c_it );
          ! dir.end();
          ++dir )
    {
      // we first check in the direct orientation
      if ( neighborhood.adjacentOnOrientedSurfaceDirected(
             dynamic_cast<ObjectBoundary &>( surf ), *dir, false, s ) &&
           ! original_set[ s ] )
      {
        umbrella_state.face = *c_it;
        umbrella_state.epsilon = true;
        umbrella_state.j = *dir;
        for ( KnSpace::dir_iterator ortho_dir = ks.sbegin_dirs( *c_it );
              ! ortho_dir.end();
              ++ortho_dir )
          if ( *ortho_dir != *dir )
          {
            umbrella_state.k = *ortho_dir;
            umbrellas_to_compute.push_back( umbrella_state );
          }
      }
      // and we then check in the indirect orientation
      if ( neighborhood.adjacentOnOrientedSurfaceDirected(
             dynamic_cast<ObjectBoundary &>( surf ), *dir, true, s ) &&
           ! original_set[ s ] )
      {
        umbrella_state.face = *c_it;
        umbrella_state.epsilon = false;
        umbrella_state.j = *dir;
        for ( KnSpace::dir_iterator ortho_dir = ks.sbegin_dirs( *c_it );
              ! ortho_dir.end();
              ++ortho_dir )
          if ( *ortho_dir != *dir )
          {
            umbrella_state.k = *ortho_dir;
            umbrellas_to_compute.push_back( umbrella_state );
          }
      }
    }
  }

  // compute the set of umbrellas stored in 'umbrellas_to_compute'
  // and add to surfels_set the surfels of thoses umbrellas that
  // didn't belong to the set before
  uint n = ks.dim();
  Kn_sid faces[ n * ( 1 << ( n - 1 ) ) ];
  DigitalSurfaceUmbrellaComputer umbrella;
  umbrella.init( *tracker );
  delete tracker;

  for ( uint i = 0; i < umbrellas_to_compute.size(); ++i )
  {
    umbrella.setState( umbrellas_to_compute [ i ] );
    DigitalSurfaceUmbrellaComputer::computeUmbrellaFaces( umbrella, faces, n );
    for ( uint j = 0; j < n; ++j )
      if ( ! surfels_set[ faces[ j ] ] )
        surfels_set += faces[ j ];
  }
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
NormalOnSurfaceComputerByUmbrellas::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[NormalOnSurfaceComputerByUmbrellas]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
NormalOnSurfaceComputerByUmbrellas::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
