///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : K3Shapes.cxx
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
#include <cmath>
#include "ImaGene/digitalnD/KnSpaceScanner.h"
#include "ImaGene/digitalnD/KnShapes.h"
#include "ImaGene/digitalnD/K3Shapes.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/K3Shapes.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const K3Shapes_RCS_ID = "@(#)class K3Shapes definition.";

///////////////////////////////////////////////////////////////////////////////
// class K3Shapes
///////////////////////////////////////////////////////////////////////////////


/**
 * Creates a set of unsigned volumic cells whose elements represents a
 * sphere of center [center] and radius [inclusive_radius].
 * NB: strangely not faster than generic algorithm.
 * @param ks any space (with 'dim()==3')..
 * @param center a cell that is the center of the sphere.
 * @param inclusive_radius the radius of the sphere.
 * @return a set of spel-like cells.
 * @see KnShapes::umakeVolumicSphere
 */
KnCharSet
K3Shapes::umakeVolumicSphere( const KnSpace & ks,
			      Kn_uid center,
			      float inclusive_radius )
{
  ASSERT_K3Shapes( ks.dim() == 3 );
  
  // center must be a spel.
  center = ks.uspel( center );

  // Computing sphere bounding cube.
  inclusive_radius = inclusive_radius >= 0 ? inclusive_radius : 0.0f;
  int r = (int) std::ceil( inclusive_radius );
  Kn_uid low_uid = KnShapes::ugetCubeLowerBound( ks, center, r );
  Kn_uid up_uid = KnShapes::ugetCubeUpperBound( ks, center, r );

  // Instantiating set.
  KnCharSet s = KnCharSet::ucreate( ks, center, 0 );
  Vector vcenter = ks.ucentroid( center );
  float r2 = inclusive_radius * inclusive_radius;

  // Filling it by scanning the bounding cube.
  uint k;
  KnSpaceScanner scan( ks, low_uid, up_uid );
  Kn_uid last_z, last_y, last_x;
  Kn_uid p = scan.begin();
  for ( last_z = scan.last( p, 2 );
	p <= last_z; 
	p += scan.gotonext( 2 ) )
    for ( last_y = scan.last( p, 1 );
	  p <= last_y; 
	  p += scan.gotonext( 1 ) )
      for ( last_x = scan.last( p, 0 );
	    p <= last_x; 
	    ++p ) // NB: 'scan.gotonext( 0 )' == 1;
	{
	  float dist2 = 0.0f;
	  Vector vp = ks.ucentroid( p );
	  for ( k = 0; k < 3; ++k )
	    dist2 += KnUtils::sqr( vcenter.ro( k ) - vp.ro( k ) );
	  if ( dist2 <= r2 )
	    s += p;
	}
  return s;
}


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
K3Shapes::~K3Shapes()
{
}

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
K3Shapes::selfDisplay( ostream& that_stream ) const
{
  that_stream << endl;
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
K3Shapes::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
