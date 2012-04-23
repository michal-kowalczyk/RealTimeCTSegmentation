///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ShapeHelper.cxx
//
// Creation : 2004/11/22
//
// Version : 2004/11/22
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
//	2004/11/22 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
//#include "LinAlg/LinAlg/Vector.hpp"
//#include "LinAlg/LinAlg2D/Vector2D.hpp"
#include <sstream>
#include <algorithm>

#include "ImaGene/base/Vector.h"
#include "ImaGene/base/VectorUtils.h"
#include "ImaGene/base/Proxy.h"
#include "ImaGene/base/RnMaps.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.h"
#include "ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.h"
#include "ImaGene/digitalnD/KnSpaceScanner.h"
#include "ImaGene/digitalnD/KnShapes.h"
#include "ImaGene/digitalnD/KnTools.h"
#include "ImaGene/digitalnD/KnRCellVector.h"
#include "ImaGene/helper/ShapeHelper.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/ShapeHelper.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

#include <QDebug>

using namespace std;

const char* const ShapeHelper_RCS_ID = "@(#)class ShapeHelper definition.";

///////////////////////////////////////////////////////////////////////////////
// class ShapeHelper
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ShapeHelper::~ShapeHelper()
{
}


///////////////////////////////////////////////////////////////////////////////
// Static services

/**
 * Digitized the shape [shape] wrt to the space [ks] which is
 * embedded in the nD euclidean space as specified by [embedder].
 * The digitization is the simplest one: the centroid of each spel
 * of the space is checked to be inside the given shape. The spel is
 * added to the spelset accordingly.
 *
 * @param ks any digital space.
 * @param embedder an embedding of [ks].
 * @param shape any shape in the space.
 * @return the set of spels of the space inside the shape.
 * @deprecated since v1.5 
 * @see digitizeShapeByGauss
 */
ImaGene::KnCharSet
ImaGene::ShapeHelper::makeShape( KnSpace & ks, 
				 const Embedder & embedder, 
				 const Shape & shape )
{
  return digitizeShapeByGauss( ks, embedder, shape );
}



/**
 * Digitized the shape [shape] wrt to the space [ks] which is
 * embedded in the nD euclidean space as specified by [embedder].
 * The digitization is the simplest one: the centroid of each spel
 * of the space is checked to be inside the given shape. The spel is
 * added to the spelset accordingly.
 *
 * @param ks any digital space.
 * @param embedder an embedding of [ks].
 * @param shape any shape in the space.
 * @return the set of spels of the space inside the shape.
 */
ImaGene::KnCharSet
ImaGene::ShapeHelper::digitizeShapeByGauss( KnSpace & ks, 
					    const Embedder & embedder, 
					    const Shape & shape )
{
  // Filling it by scanning the space.
  Kn_uid low = ks.uspel( ks.ufirst() );
  Kn_uid up = ks.uspel( ks.ulast() );
  KnCharSet s = KnCharSet::ucreate( ks, low, 0 );
  KnSpaceScanner scanner( ks, low, up );
  Kn_uid p = scanner.lower_bound;
  Vector vp( ks.dim() );
  do 
    {
      embedder.uembed( p, vp );
      if ( shape.isInside( vp ) )
	s += p;
    }
  while ( scanner.increment( p ) );
  return s;
}
 


/**
 * Searches for a surfel on the boundary of the 2D shape [sshape] in
 * the direction given by [dir]. Use dichotomy to speed up the
 * process.
 *
 * @param ks any 2d digital space.
 * @param embedder an embedding of [ks].
 * @param sshape any starshape in the euclidean plane.
 * @param dir the direction of the line that intersects the starshape.
 *
 * @return a bel on the shape boundary which is approximately in the
 * direction [dir] wrt to the interior point of the starshape.
 */
ImaGene::Kn_sid
ImaGene::ShapeHelper::getBelOnStarShapedBoundary
( KnSpace & ks,
  const Embedder & embedder,
  const StarShaped & sshape,
  const Vector2D & dir )
{
  Vector2D ll; 
  Vector2D ur;
  embedder.uembed( ks.uspel( ks.ufirst() ), ll );
  embedder.uembed( ks.uspel( ks.ulast() ), ur );
  Vector2D lr( ur.ro( 0 ), ll.ro( 1 ) );
  Vector2D ul( ll.ro( 0 ), ur.ro( 1 ) );
  
  Vector2D shapec = sshape.center();
  
  Vector2D rel_ur( ur ); rel_ur -= shapec;
  Vector2D rel_ul( ul ); rel_ul -= shapec;
  Vector2D rel_ll( ll ); rel_ll -= shapec;
  Vector2D rel_lr( lr ); rel_lr -= shapec;
  
  float ur_ccw_to_dir = VectorUtils::det( dir, rel_ur );
  float ul_ccw_to_dir = VectorUtils::det( dir, rel_ul );
  float ll_ccw_to_dir = VectorUtils::det( dir, rel_ll );
  float lr_ccw_to_dir = VectorUtils::det( dir, rel_lr );

  float x, y;
  int orientation = -1;
  if ( ( ur_ccw_to_dir >= 0.0 ) && ( lr_ccw_to_dir <= 0.0 ) )
    // Test with Right.
    {
      x = ur.ro( 0 );
      y = shapec.ro( 1 ) + ( x - shapec.ro( 0 ) ) * dir.ro( 1 ) / dir.ro( 0 ) ;
      orientation = 0;
    }
  else if ( ( ul_ccw_to_dir >= 0.0 ) && ( ur_ccw_to_dir <= 0.0 ) )
    // Test with Up.
    {
      y = ur.ro( 1 );
      x = shapec.ro( 0 ) + ( y - shapec.ro( 1 ) ) * dir.ro( 0 ) / dir.ro( 1 ) ;
      orientation = 1;
    }
  else if ( ( ll_ccw_to_dir >= 0.0 ) && ( ul_ccw_to_dir <= 0.0 ) )
    // Test with Left.
    {
      x = ll.ro( 0 );
      y = shapec.ro( 1 ) + ( x - shapec.ro( 0 ) ) * dir.ro( 1 ) / dir.ro( 0 ) ;
      orientation = 2;
    }
  else if ( ( lr_ccw_to_dir >= 0.0 ) && ( ll_ccw_to_dir <= 0.0 ) )
    // Test with Low.
    {
      y = ll.ro( 1 );
      x = shapec.ro( 0 ) + ( y - shapec.ro( 1 ) ) * dir.ro( 0 ) / dir.ro( 1 ) ;
      orientation = 3;
    }
  else
    // ??
    {
      cerr << "Geometric ERROR." << endl;
      x = y = 0.0;
    }

  Vector2D limit( x, y );
  // Starting dichotomy.
  Kn_uid lintv = embedder.uinverseSpel( shapec );
  Kn_uid lextv = embedder.uinverseSpel( limit );
  embedder.uembed( lintv, shapec );
  embedder.uembed( lextv, limit );
  if ( ( ! sshape.isInside( shapec ) ) 
       || ( sshape.isInside( limit ) ) )
      cerr << "Inside/Outside ERROR." << endl;
  while ( true )
    {
      Vector2D middle( shapec );
      middle += limit;
      middle *= 0.5;
      Kn_uid middlev = embedder.uinverseSpel( middle );
      embedder.uembed( middlev, middle );
      if ( ( middlev == lintv ) || ( middlev == lextv ) )
	break;
      
//       cerr << "M( " << middle.ro( 0 ) << " , " << middle.ro( 1 ) << " )" 
// 	   << middle 
//  	   << endl;
      if ( sshape.isInside( middle ) )
	{
	  shapec = middle;
	  lintv = middlev;
	}
      else
	{
	  limit = middle;
	  lextv = middlev;
	}
    }

//    cerr << "VoxC=";
//    ks.displayKn_uid( lintv, cerr );
//    cerr << endl << "VoxL=";
//    ks.displayKn_uid( lextv, cerr );
//    cerr << endl;

  // 2nd loop.
  Kn_sid bsurfel = 0;
  uint lvl;
  if ( ! ks.uareAdjacent( lintv, lextv, lvl ) )
    cerr << "Adjacency error" << endl;
  Kn_uid interm = lintv;
  Vector xinterm( 2 );
  uint i = 0;
  int dx = 0;
  for ( ; i < 2; ++i )
    {
      dx = ( (int) ks.udecodeCoord( lextv, i ) )
	- ( (int) ks.udecodeCoord( lintv, i ) );
      // cerr << "dx[" << i << "]=" << dx << endl;
      if ( dx == 0 ) continue;
      if ( dx > 0 ) interm = ks.ugetIncr( interm, i );
      else          interm = ks.ugetDecr( interm, i );
      embedder.uembed( interm, xinterm );
//       cerr << "I( " << xinterm.ro( 0 ) << " , " << xinterm.ro( 1 ) << "|"
// 	   << sshape.isInside( xinterm ) << ")" << endl;
      if ( ! sshape.isInside( xinterm ) )
	  break;
    }
  bsurfel = ks.sincident( ks.signsNeg( interm ), i, dx < 0 );
  
//   Kn_sid bdry_v = ks.signsPos( lintv );
//   Kn_sid bsurfel;
//   switch ( orientation )
//     {
//     case 0: bsurfel = ks.sincident( bdry_v, 0, true ); break;
//     case 1: bsurfel = ks.sincident( bdry_v, 1, true ); break;
//     case 2: bsurfel = ks.sincident( bdry_v, 0, false ); break;
//     case 3: bsurfel = ks.sincident( bdry_v, 1, false ); break;
//     default: bsurfel = 0;
//     }
  uint j = ks.sorthDir( bsurfel );
  Kn_uid check_int = ks.unsigns( ks.sincident( bsurfel, j,
					       ks.sdirect( bsurfel, j ) ) );
  Kn_uid check_ext = ks.unsigns( ks.sincident( bsurfel, j,
					       ! ks.sdirect( bsurfel, j ) ) );
  Vector2D check_x;
  embedder.uembed( check_int, check_x );
  if ( ! sshape.isInside( check_x ) )
    cerr << "INSIDE ERROR." << endl;
  embedder.uembed( check_ext, check_x );
  if ( sshape.isInside( check_x ) )
    cerr << "OUTSIDE ERROR." << endl;

  return bsurfel;
}


/**
 * Searches for a surfel on the boundary of the 2D shape [sshape] in
 * the direction given by [dir]. Use dichotomy to speed up the
 * process.
 *
 * @param ks any 2d digital space.
 * @param embedder an embedding of [ks].
 * @param sshape any starshape in the euclidean plane.
 * @param dir the direction of the line that intersects the starshape.
 *
 * @return a bel on the shape boundary which is approximately in the
 * direction [dir] wrt to the interior point of the starshape.
 *
 * @deprecated @since v1.5, calls @see getBelOnStarShapedBoundary
 */
ImaGene::Kn_sid
ImaGene::ShapeHelper::getBelOnStarShape2DBoundary
( KnSpace & ks,
  const Embedder & embedder,
  const StarShaped & sshape,
  const Vector2D & dir )
{
  return getBelOnStarShapedBoundary( ks, embedder, sshape, dir );
}

/**
 * Computes the face-connected set containing the spel [in] and
 * included in the set [voxset] and return a bel on its boundary.
 *
 * @param ks the digital space,
 * @param voxset the digital shape,
 * @param in a spel in [voxset],
 * @param main_inner_comp (returns) the connected set included in
 * [voxset] and containing [in],
 *
 * @return a bel on the boundary of [main_inner_comp].
 */
ImaGene::Kn_sid
ImaGene::ShapeHelper::findInnerObject
( KnSpace* ks, KnCharSet voxset, Kn_uid in,
  KnCharSet & main_inner_comp )
{
  main_inner_comp 
    = KnShapes::uexpandSeedWithinBounds( *ks, in, ~voxset );
  // cerr << main_inner_comp << endl;
  
  return KnShapes::sfindFurthestBel( *ks, in, main_inner_comp );
}

  
/**
 * Computes the face-connected set containing the spel [out] and
 * excluding the set [voxset] and return a bel on its boundary.
 *
 * @param ks the digital space,
 * @param voxset the digital shape,
 * @param out a spel outside [voxset],
 * @param main_outer_comp (returns) the connected set not intersecting
 * [voxset] and containing [out],
 * @param in a spel in [voxset],
 *
 * @return a bel on the boundary of [main_outer_comp].
 *
 * @todo Check this method (symmetry with findInnerObject)
 */
ImaGene::Kn_sid
ImaGene::ShapeHelper::findOuterObject
( KnSpace* ks, KnCharSet voxset, Kn_uid out, 
  KnCharSet & main_outer_comp, Kn_uid in  )
{
  main_outer_comp 
    = KnShapes::uexpandSeedWithinBounds( *ks, out, voxset );
  // cerr << main_outer_comp << endl;
  
  return ks->sopp( KnShapes::sfindClosestBel( *ks, in, ~main_outer_comp ) );
}



/** 
 * Constructs a star-shaped objects from command-line
 * arguments. Interpreted arguments are:
 * - "-circle <r>" : generates a disk.
 * - "-flower <R> <r> <n> <phi>" : generates a flower.
 * - "-accflower <R> <r> <n>" : generates a phase-accelerating flower.
 * - "-rsquare <R> <r>" : generates a disk with a rounded square on a quadrant.
 * - "-fuzzy_uniform" : gives a slight perturbation on the shape center.
 * - "-step" : the resolution step (used for the perturbation).
 * 
 * @param args an argument object storing options generally given on a
 * command line.
 * 
 * @return a (dyn. alloc.) starshaped object.
 */
ImaGene::StarShaped* 
ImaGene::ShapeHelper::makeStarShapedFromArgs
( const Arguments & args )
{
  bool circle = args.check( "-circle" );
  bool flower = args.check( "-flower" );
  bool accflower = args.check( "-accflower" );
  bool rsquare = args.check( "-rsquare" );
  bool regngon = args.check( "-regngon" );
  bool ellipse = args.check( "-ellipse" );
  bool fuzzy_uniform = args.check( "-fuzzy_uniform" );
  float dh = args.getOption( "-step" )->getFloatValue( 0 );
  
  float xc = args.getOption( "-center" )->getFloatValue( 0 );
  float yc = args.getOption( "-center" )->getFloatValue( 1 );
  if ( fuzzy_uniform )
    { 
      xc += ( Mathutils::random1() - 0.5 ) * dh;
      yc += ( Mathutils::random1() - 0.5 ) * dh;
    }

  StarShaped* shape = 0;
  if ( circle )
    {
      shape =
	new Circle( xc, yc, args.getOption( "-circle" )->getFloatValue( 0 ) );
    }
  else if ( flower )
    {
      shape =
	new Flower( xc, yc, 
		    args.getOption( "-flower" )->getFloatValue( 0 ),
		    args.getOption( "-flower" )->getFloatValue( 1 ),
		    args.getOption( "-flower" )->getIntValue( 2 ),
		    args.getOption( "-flower" )->getFloatValue( 3 ) );
    }
  else if ( accflower )
    {
      shape =
	new AccFlower( xc, yc, 
		       args.getOption( "-accflower" )->getFloatValue( 0 ),
		       args.getOption( "-accflower" )->getFloatValue( 1 ),
		       args.getOption( "-accflower" )->getIntValue( 2 ) );
    }
  else if ( rsquare )
    {
      shape =
	new RoundedSquare( xc, yc, 
			   args.getOption( "-rsquare" )->getFloatValue( 0 ),
			   args.getOption( "-rsquare" )->getFloatValue( 1 ) );
    }
  else if ( regngon )
    {
      shape =
	new Cngon( xc, yc, 
		   args.getOption( "-regngon" )->getFloatValue( 0 ),
		   args.getOption( "-regngon" )->getIntValue( 1 ),
		   args.getOption( "-regngon" )->getFloatValue( 2 ) );
    }
  else if ( ellipse )
    {
      shape =
	new Ellipse( xc, yc,
		     args.getOption( "-ellipse" )->getFloatValue( 0 ),
		     args.getOption( "-ellipse" )->getFloatValue( 1 ),
		     args.getOption( "-ellipse" )->getFloatValue( 2 ) );
    }		     

  return shape;
}


/**
 * Add the standard arguments for defining star-shaped objects to argument
 * object [args].
 *
 * @param args (updated) the argument object to enrich.
 */
void
ImaGene::ShapeHelper::addStarShapedArgs( Arguments & args )
{
  args.addOption( "-step", "-step <h>: the discretization step or resolution, the closer to 0, the finer.", "1.0" );
  args.addOption( "-circle", "-circle <R>: the test shape is a circle of radius R", "10.0" );
  args.addOption( "-flower", "-flower <R> <r> <k> <phi>: the test shape is a flower with k extremeties with mean radius R and variability of radius r, phi is the phase of the flower", "3", "10.0", "5.0", "0.0" );
  args.addOption( "-accflower", "-accflower <R> <r> <k>: the test shape is a phase accelerating flower with k extremeties with mean radius R and variability of radius r", "4.0", "2.0", "4" );
  args.addOption( "-rsquare", "-rsquare <R> <r>: the test shape is rounded square of big radius R and small corner radius r", "4.0", "1.0" );
  args.addOption( "-regngon", "-regngon <r> <n> <phi>: the test shape is a regular convex polygon with n sides, of radius r, phi is the phase of the polygon", "10.0", "3", "0.0" );
  args.addOption( "-ellipse", "-ellipse <a1> <b1> <theta>: the test shape is an ellipse of half big axis <a1> and half small axis <a2> with orientation <theta>", "10.0", "5.0", "0.0" );
  args.addBooleanOption( "-fuzzy_uniform", "-fuzzy_uniform: possible uniform random shift of the shape center." );
  args.addOption( "-center", "-center <x> <y>: choose the center of the shape in real coordinates", "0.0", "0.0" );
}


/**
 * Given the arguments [args] (-noisify ...), perturbate the shape
 * given by the digital set [voxset] and an optional starting bel
 * [bel]. If the shape is connected, giving or not giving [bel] does
 * not change anything, otherwise it specifies the contour to
 * perturbate.
 *
 * @param args the arguments specifying how to add noise to the shape,
 *
 * @param voxset (updates) the shape that is modified, 
 *
 * @param bel a bel on the boundary of the shape or 0 if no contour is
 * specified,
 *
 * @param in (returns) a spel deep inside the noisified object (or
 * unmodified if none exists).
 *
 * @param out (returns) a spel deep outside the noisified object (or
 * unmodified if none exists).
 */
void
ImaGene::ShapeHelper::addNoiseFromArgs
( const Arguments & args,
  KnSpace* ks, KnCharSet & voxset, Kn_sid bel, 
  Kn_uid & in, Kn_uid & out )
{
  KnRCellSet bdry = bel != 0 
    ? KnShapes::strackBoundary( *ks, voxset, bel )
    : KnShapes::smakeBoundary( *ks, voxset );
  
  KnRUCellVector<int>* dmap 
    = KnShapes::computeBdryCityBlockDistanceMap( *ks, bdry );
  int m = * ( min_element( dmap->begin(), dmap->end() ) );
  int M = * ( max_element( dmap->begin(), dmap->end() ) );

  string law = args.getOption( "-noisify" )->getValue( 0 );
  double a = (double) args.getOption( "-noisify" )->getDoubleValue( 1 );
  double b = (double) args.getOption( "-noisify" )->getDoubleValue( 2 );
  
  vector<double> p_in( M + 1 );
  vector<double> p_out( -m + 1 );
  if ( law == "POWER" )
    {
      p_in[ 0 ] = 1.0;
      p_out[ 0 ] = 1.0;
      double bb = a/b;
      for ( uint k = 1; k < p_in.size(); ++k )
	{
	  p_in[ k ] = bb;
	  bb /= b;
	}
      bb = a/b;
      for ( uint k = 1; k < p_out.size(); ++k )
	{
	  p_out[ k ] = bb;
	  bb /= b;
	}
    }

  voxset = KnShapes::noisifyObject( *ks, voxset, bdry, p_in, p_out, in, out );
  delete dmap;
}




/**
 * Add the standard arguments for defining noisy objects to argument
 * object [args].
 *
 * @param args (updated) the argument object to enrich.
 */
void
ImaGene::ShapeHelper::addNoiseArgs( Arguments & args )
{
  args.addOption( "-noisify", "-noisify <type> <a> <b>: noisify the shape within study according to the distance k to the boundary ; with the law <type>=POWER such that P_change(k)=a/b^k", "POWER", "1.0", "2.0" );
  args.addBooleanOption( "-inner_object", "-inner_object: extract inner contour (useful with noisify)." );
  args.addBooleanOption( "-outer_object", "-outer_object: extract outer contour (useful with noisify)." );
}





/**
 * Exports the 2D digital set [voxset] as a PGM file in the stream [out].
 *
 * @param out the output stream,
 * @param ks the digital space,
 * @param voxset the digital set.
 */
void
ImaGene::ShapeHelper::exportToPGM( ostream & out, 
				   KnSpace* ks, 
				   const KnCharSet & voxset )
{
  ASSERT_ShapeHelper( ks->dim() == 2 );

  out << "P5" << endl
       << "# CREATOR: test_TangentialCover " 
       << "(jacques-olivier.lachaud@univ-savoie.fr)" << endl;
  out << ks->size( 0 ) << " " << ks->size( 1 ) << endl
       << "255" << endl;
  
  KnSpaceScanner scan2( *ks, 
			ks->ufirstCell( ks->dim() ),
			ks->ulastCell( ks->dim() ) );

  Kn_uid p = scan2.begin();
  for ( Kn_uid last_y = scan2.last( p, 1 );
	p <= last_y; 
	p += scan2.gotonext( 1 ) )
    {
      for ( Kn_uid last_x = scan2.last( p, 0 ); 
	    p <= last_x; 
	    p++ ) // NB: 'scan.gotonext( 0 )' == 1;
	{ //... whatever
	  out << ( voxset[ p ] ? (char) 0 : (char) 255 );
	}
    }
  out << endl;
}


/**
 * Exports the Unsigned Cell vector  as a PGM file in the stream [out].
 *
 * @param out the output stream,
 * @param ks the digital space,
 * @param cellVector the cell vector containing image information.
 */
void
ImaGene::ShapeHelper::exportToPGM( ostream & out, 
				   KnSpace* ks, 
				   const KnRUCellVector<int> & cellVector )
{
  ASSERT_ShapeHelper( ks->dim() == 2 );

  out << "P5" << endl
       << "# CREATOR: test_TangentialCover " 
       << "(jacques-olivier.lachaud@univ-savoie.fr)" << endl;
  out << ks->size( 0 ) << " " << ks->size( 1 ) << endl
       << "255" << endl;
  
  KnSpaceScanner scan2( *ks, 
			ks->ufirstCell( ks->dim() ),
			ks->ulastCell( ks->dim() ) );

  Kn_uid p = scan2.begin();
  for ( Kn_uid last_y = scan2.last( p, 1 );
	p <= last_y; 
	p += scan2.gotonext( 1 ) )
    {
      for ( Kn_uid last_x = scan2.last( p, 0 ); 
	    p <= last_x; 
	    p++ ) // NB: 'scan.gotonext( 0 )' == 1;
	{ //... whatever
	  out << (char)(cellVector[p]);
	}
    }
  out << endl;
}




/**
 * Exports the 2D digital set [voxset] as a PGM3d file in the stream [out].
 *
 * @param out the output stream,
 * @param ks the digital space,
 * @param voxset the digital set.
 */
void
ImaGene::ShapeHelper::exportToPGM3d( ostream & out, 
				     KnSpace* ks, 
				     const KnCharSet & voxset ){
  ASSERT_ShapeHelper( ks->dim() == 3 );
  
  out << "P3d" << endl
      << "# CREATOR: ShapeHelper " << endl;
  out << ks->size( 0 ) << " " << ks->size( 1 ) <<  " " << ks->size( 2 ) << endl
      << "255" << endl;
  
  KnSpaceScanner scan2( *ks, 
			ks->ufirstCell( ks->dim() ),
			ks->ulastCell( ks->dim() ) );

  Kn_uid p = scan2.begin();
  for ( Kn_uid last_z = scan2.last( p, 2 );
	p <= last_z; 
	p += scan2.gotonext( 2 ) )
    {  
      for ( Kn_uid last_y = scan2.last( p, 1 );
	    p <= last_y; 
	    p += scan2.gotonext( 1 ) )
	{
	  for ( Kn_uid last_x = scan2.last( p, 0 ); 
		p <= last_x; 
		p++ ) // NB: 'scan.gotonext( 0 )' == 1;
	    { //... whatever
	      out << ( voxset[ p ] ? (char) 255 : (char) 0 );
	    }
	}
    }  
  out << endl;
}


/**
 * Exports the Unsigned Cell vector as a PGM3d file in the stream [out].
 *
 * @param out the output stream,
 * @param ks the digital space,
 * @param cellVector the cell vector containing image information.
 */
void
ImaGene::ShapeHelper::exportToPGM3d( ostream & out, 
				     KnSpace* ks, 
				     const KnRUCellVector<int> &cellValue ){
  ASSERT_ShapeHelper( ks->dim() == 3 );
  

  out << "P3d" << endl
      << "# CREATOR: ShapeHelper " << endl;
  out << ks->size( 0 ) << " " << ks->size( 1 ) <<  " " << ks->size( 2 ) << endl
      << "255" << endl;
  
  KnSpaceScanner scan2( *ks, 
			ks->ufirstCell( ks->dim() ),
			ks->ulastCell( ks->dim() ) );

  Kn_uid p = scan2.begin();
  for ( Kn_uid last_z = scan2.last( p, 2 );
	p <= last_z; 
	p += scan2.gotonext( 2 ) )
    {  
      for ( Kn_uid last_y = scan2.last( p, 1 );
	    p <= last_y; 
	    p += scan2.gotonext( 1 ) )
	{
	  for ( Kn_uid last_x = scan2.last( p, 0 ); 
		p <= last_x; 
		p++ ) // NB: 'scan.gotonext( 0 )' == 1;
	    { //... whatever
	      out << (char) (cellValue[p]);
	    }
	}
    }  
  out << endl;
}









/**
 * Imports a PGM file in the stream [in]. Return a vector of integer
 * representing the PGM image indexed by unsigned cell code.
 *
 * @param in the input stream,
 * @param ks (returns) a dyn. allocated digital space,
 * @param KnRUCellVector a vector indexed by unsigned cell code
 *
 * @return 'true' if everything went well, otherwise return 'false'
 * and 0 for both pointers.
 */
bool 
ImaGene::ShapeHelper::importFromPGM
( std::istream & in, 
  KnSpace* & ks,
  KnRUCellVector<int>* & vectorVal){
  
  ks = 0;
  string str;
  getline( in, str );
  if ( ! in.good() ) return false;
  if ( str != "P5" ) return false;
  do
    {
      getline( in, str );
      if ( ! in.good() ) return false;
    }
  while ( str[ 0 ] == '#' || str=="");
  istringstream str_in( str );
  Kn_size sizes[ 2 ];
  str_in >> sizes[ 0 ] >> sizes[ 1 ];

  getline( in, str );
  istringstream str2_in( str );
  int max_value;
  str2_in >> max_value;

  if ( ! in.good() ) return false;
  cerr << "# PGM " << sizes[ 0 ] << " " << sizes[ 1 ] 
       << " " << max_value << " from <" << str << ">" << endl;
  ks = new KnSpace( 2, sizes );
  if ( ks == 0 ) return false;
  vectorVal = new KnRUCellVector<int>(*ks, 2 );
  
  KnSpaceScanner scan2( *ks, 
 			ks->ufirstCell( ks->dim() ),
			ks->ulastCell( ks->dim() ) );
  
  
  uint nb_read = 0;
  Kn_uid last_y, last_x;
  Kn_uid p = scan2.begin();
  in >> noskipws;
  for ( last_y = scan2.last( p, 1 );
	p <= last_y; 
	p += scan2.gotonext( 1 ) )
    {
      for ( last_x = scan2.last( p, 0 ); 
	    p <= last_x; 
	    p++ ) // NB: 'scan.gotonext( 0 )' == 1;
	{ //... whatever
	  unsigned char c; 
	  in >> c;
	  if ( in.good() ){ 
	    ++nb_read;
	    (*vectorVal)[ p ] = c; 
	  }
	}
    }
  if ( in.fail() || in.bad() )
    {
      cerr << "# nbread=" << nb_read << endl;
      delete vectorVal;
      vectorVal=0;
      delete ks;
      ks = 0;
      return false;
    }
  in >> skipws;
  return true;
}





/**
 * Imports a 2D digital set from a PGM file in the stream
 * [in]. Creates a digital space and a digital set.
 *
 * @param out the output stream,
 * @param ks (returns) a dyn. allocated digital space,
 * @param voxset (returns) the dyn. allocated digital set (lying in [ks]).
 * @param threshold if the value>=threshold then the pixel is added to [voxset]
 *
 * @return 'true' if everything went well, otherwise return 'false'
 * and 0 for both pointers.
 */
bool 
ImaGene::ShapeHelper::importFromPGM
( std::istream & in, 
  KnSpace* & ks,
  KnCharSet* & voxset,
  uint threshold, uint bordXY, bool quiet )
{
  ks = 0;
  voxset = 0;
  string str;
  getline( in, str );
  if ( ! in.good() ){ qDebug() << "i" << 1 << (int)(in.rdstate() & std::istream::eofbit) << (int)(in.rdstate() & std::istream::failbit) << (int)(in.rdstate() & std::istream::badbit); return false;}
  if ( str != "P5" ){ qDebug() << "i" << 2; return false;}
  do
    {
      getline( in, str );
      if ( ! in.good() ){ qDebug() << "i" << 3; return false;}
    }
  while ( str[ 0 ] == '#' || str=="");
  istringstream str_in( str );
  Kn_size sizes[ 2 ];
  str_in >> sizes[ 0 ] >> sizes[ 1 ];
  uint xMaxObj=sizes[0];
  uint yMaxObj=sizes[1];


  sizes[0]+=2*bordXY;
  sizes[1]+=2*bordXY;
  
  
  getline( in, str );
  istringstream str2_in( str );
  int max_value;
  str2_in >> max_value;

  if ( ! in.good() ){ qDebug() << "i" << 4; return false;}
  if(!quiet){
      qDebug () << "# PGM " << sizes[ 0 ] << " " << sizes[ 1 ]
         << " " << max_value << " from <" << str.c_str() << ">" << endl;
  }
  ks = new KnSpace( 2, sizes );
  if ( ks == 0 ){ qDebug() << "i" << 5; return false;}
  voxset = new KnCharSet( KnCharSet::ucreate( *ks, 
					      ks->uspel( ks->ufirst() ),
                                              0 ) );
  if ( voxset == 0 )
    {
      qDebug() << "i" << 5;
      delete ks;
      ks = 0;
      return false;
    }
	
  KnSpaceScanner scan2( *ks, 
 			ks->ufirstCell( ks->dim() ),
			ks->ulastCell( ks->dim() ) );
  uint nb_read = 0;
  Kn_uid last_y, last_x;
  Kn_uid p = scan2.begin();
  uint x,y;
  
  in >> noskipws;
  for ( y=0, last_y = scan2.last( p, 1 );
	p <= last_y; y++,
	p += scan2.gotonext( 1 ) )
    {
      for ( x=0, last_x = scan2.last( p, 0 ); 
	    p <= last_x; x++,
	      p++ ) // NB: 'scan.gotonext( 0 )' == 1;
	{ //... whatever
	  if(!((x<bordXY) ||( y<bordXY)  || 
	       (x>=(xMaxObj+bordXY))|| (y>=(yMaxObj+bordXY)) )){
	    unsigned char c; 
	    in >> c;
	    if ( in.good() ) ++nb_read;
	    if ( ( (uint) c ) < threshold )
	      (*voxset)[ p ] = true; 
	  }
	}
    }
  if ( in.fail() || in.bad() )
    {
      qDebug () << "# nbread=" << nb_read << endl;
      qDebug() << "i" << 6;
      delete voxset;
      delete ks;
      voxset = 0;
      ks = 0;
      return false;
    }
  return true;
}

 


/**
 * Imports a 3D digital set from a 3dPGM file in the stream
 * [in]. Creates a digital space and a digital set.
 *
 * @param out the output stream,
 * @param ks (returns) a dyn. allocated digital space,
 * @param voxset (returns) the dyn. allocated digital set (lying in [ks]).
 * @param thresholdMin thresholdMax  if the thresholdMin<=value<=thresholdMax then the pixel is added to [voxset]
 *
 * @return 'true' if everything went well, otherwise return 'false'
 * and 0 for both pointers.
 */
bool 
ImaGene::ShapeHelper::importFromPGM3d ( std::istream & in, 
					KnSpace* & ks,
					KnCharSet* & voxset,
					uint thresholdMin, 
					uint thresholdMax, uint bordXYZ ){
  ks = 0;
  voxset = 0;
  string str;
  getline( in, str );
  if ( ! in.good() ) return false;
  if ( str != "P3d" ) return false;


  do
    {
      getline( in, str );
      if ( ! in.good() ) return false;
    }
  while ( str[ 0 ] == '#' || str=="");
  istringstream str_in( str );
  Kn_size sizes[ 3 ];
  str_in >> sizes[ 0 ] >> sizes[ 1 ] >> sizes[ 2 ] ;
  uint xMaxObj=sizes[0];
  uint yMaxObj=sizes[1];
  uint zMaxObj=sizes[2];
  
  getline( in, str );
  istringstream str2_in( str );
  int max_value;
  str2_in >> max_value;
  sizes[0]+=2*bordXYZ;
  sizes[1]+=2*bordXYZ;
  sizes[2]+=2*bordXYZ;
  
  if ( ! in.good() ) return false;
  cerr << "# PGM3d. " << sizes[ 0 ] << " " << sizes[ 1 ] << " " << sizes[ 2 ] 
       << " " << max_value << " from <" << str << ">" << endl;
  ks = new KnSpace( 3, sizes );
  if ( ks == 0 ) return false;
  voxset = new KnCharSet( KnCharSet::ucreate( *ks, 
					      ks->uspel( ks->ufirst() ),
					      0 ) );
  if ( voxset == 0 ){
    delete ks;
    ks = 0;
    return false;
  }
  
  KnSpaceScanner scan2( *ks, 
 			ks->ufirstCell( ks->dim() ),
			ks->ulastCell( ks->dim() ) );
  uint nb_read = 0;
  uint nbVox=0;
  Kn_uid last_z, last_y, last_x;
  Kn_uid p = scan2.begin();
  in >> noskipws;
  uint x,y,z;
  
  for ( z=0,last_z = scan2.last( p, 2 );
	p <= last_z; 
	z++, p += scan2.gotonext( 2 ) ){
    for (y=0, last_y = scan2.last( p, 1 );
	 p <= last_y; 
	 y++, p += scan2.gotonext( 1 ) ){
      for ( x=0,last_x = scan2.last( p, 0 ); 
	    p <= last_x; 
	    x++,p++ ) // NB: 'scan.gotonext( 0 )' == 1;
	{ //... whatever
	  if(!((x<bordXYZ) ||( y<bordXYZ) || (z<bordXYZ) || 
	       (x>=(xMaxObj+bordXYZ))|| (y>=(yMaxObj+bordXYZ)) || (z>=(zMaxObj+bordXYZ)))){
	    unsigned char c; 
	      in >> c;
	      if ( in.good() ) ++nb_read;
	      if ( (( (uint) c ) >= thresholdMin) && (( (uint) c ) <= thresholdMax) ){
		(*voxset)[ p ] = 1; 
		nbVox++;
	      }else{
		(*voxset)[ p ] = 0; 
	      }
	      }
	    }
	}
    }
  
  if ( in.fail() || in.bad() ){
      cerr << "# nbread=" << nb_read << endl;
      delete voxset;
      delete ks;
      voxset = 0;
      ks = 0;
      return false;
  }
  cerr << "# nbVox=" << nbVox << endl;
  in >> skipws;
  return true;
}




/**
 * Imports a PGM file in the stream [in]. Return a vector of integer
 * representing the PGM3D image indexed by unsigned cell code.
 *
 * @param in the input stream,
 * @param ks (returns) a dyn. allocated digital space,
 * @param KnRUCellVector (returns) a dyn. allocated vector indexed by unsigned cell code,
 * @param bordXYZ used to contruct a larger space,
 * @return 'true' if everything went well, otherwise return 'false'.
 */

bool 
ImaGene::ShapeHelper::importFromPGM3d ( std::istream & in, 
					KnSpace* & ks,
					KnRUCellVector<  int > *&vectorVal,
					uint bordXYZ ){
  ks = 0;
  
  string str;
  getline( in, str );
  if ( ! in.good() ) return false;
  if ( str != "P3d" ) return false;
  
  do
    {
      getline( in, str );
      if ( ! in.good() ) return false;
    }
  while ( str[ 0 ] == '#' || str=="");
  istringstream str_in( str );
  Kn_size sizes[ 3 ];
  str_in >> sizes[ 0 ] >> sizes[ 1 ] >> sizes[ 2 ] ;
  uint xMaxObj=sizes[0];
  uint yMaxObj=sizes[1];
  uint zMaxObj=sizes[2];
  
  getline( in, str );
  istringstream str2_in( str );
  int max_value;
  str2_in >> max_value;
  sizes[0]+=2*bordXYZ;
  sizes[1]+=2*bordXYZ;
  sizes[2]+=2*bordXYZ;
  
  if ( ! in.good() ) return false;
  cerr << "# PGM3d. " << sizes[ 0 ] << " " << sizes[ 1 ] << " " << sizes[ 2 ] 
       << " " << max_value << " from <" << str << ">" << endl;
  ks = new KnSpace( 3, sizes );
  if ( ks == 0 ) return false;
  vectorVal = new KnRUCellVector<int>(*ks, 3);
  
  KnSpaceScanner scan2( *ks, 
 			ks->ufirstCell( ks->dim() ),
			ks->ulastCell( ks->dim() ) );
  uint nb_read = 0;
  uint nbVox=0;
  Kn_uid last_z, last_y, last_x;
  Kn_uid p = scan2.begin();
  in >> noskipws;
  uint x,y,z;
  
  for ( z=0,last_z = scan2.last( p, 2 );
	p <= last_z; 
	z++, p += scan2.gotonext( 2 ) ){
    for (y=0, last_y = scan2.last( p, 1 );
	 p <= last_y; 
	 y++, p += scan2.gotonext( 1 ) ){
      for ( x=0,last_x = scan2.last( p, 0 ); 
	    p <= last_x; 
	    x++,p++ ) // NB: 'scan.gotonext( 0 )' == 1;
	{ //... whatever
	  if(!((x<bordXYZ) ||( y<bordXYZ) || (z<bordXYZ) || 
	       (x>=(xMaxObj+bordXYZ))|| (y>=(yMaxObj+bordXYZ)) || (z>=(zMaxObj+bordXYZ)))){
	    unsigned char c; 
	    in >> c;
	    if ( in.good() ) ++nb_read;
	    (*vectorVal)[p]=(int) c;
	    
	  }
	}
    }
  }
  
  if ( in.fail() || in.bad() ){
      cerr << "# nbread=" << nb_read << endl;
      delete ks;
      delete vectorVal;
      ks = 0;
      vectorVal=0;
      return false;
  }
  in >> skipws;
  return true;
}



/**
 * Builds the digital surface corresponding to the Freeman chain
 * code. The digital space \b must contain the FreemanChain.
 *
 * @param ks the digital space,
 * @param c the Freeman chaincode,
 * @param loop indicates if the Freeman chaincode loops or not (def. true).
 *
 * @return a set of oriented surfels that are the representation of
 * the Freeman chaincode in [ks].
 */
ImaGene::KnRCellSet
ImaGene::ShapeHelper::makeContourFromFreemanChain
( const KnSpace* ks,
  const FreemanChain & c,
  bool loop )
{
  KnRCellSet contour = KnRCellSet::create( *ks, 1, true, 0 );
  C4CIteratorOnFreemanChain itfc;
  itfc.init( c.begin(), loop );
  C4CIteratorOnFreemanChainSurface itfcs;
  itfcs.init( ks, itfc );
  Proxy<C4CIterator> itfcs_start( itfcs.clone() );
  do 
    {
      Kn_sid bel = itfcs.current();
      contour += bel;
      if ( itfcs.next() == 0 ) break;
    }
  while( ! itfcs.equals( *itfcs_start ) );
  return contour;
}

/**
 * Builds the digital surface corresponding to the Freeman chain
 * code. A big enough digital space for the surface is built and
 * returned.
 *
 * @param c (updates) the Freeman chaincode (may be translated),
 *
 * @return (returns) a dyn. allocated digital space,
 */
ImaGene::K2Space* 
ImaGene::ShapeHelper::makeSpaceFromFreemanChain
( FreemanChain & c )
{
  Kn_ssize min_x;
  Kn_ssize min_y;
  Kn_ssize max_x;
  Kn_ssize max_y;
  c.computeBoundingBox( min_x, min_y, max_x, max_y );
  Kn_size width = (Kn_size) ( 3 + max_x - min_x );
  Kn_size height = (Kn_size) ( 3 + max_y - min_y );
  Kn_size i = 1;
  while ( i < ( width + 2 ) ) i <<=1;
  Kn_size j = 1;
  while ( j < ( height + 2 ) ) j <<=1;
  
  c.x0 += 1+( (Kn_ssize) i - max_x - min_x ) / 2;
  c.y0 += 1+( (Kn_ssize) j - max_y - min_y ) / 2;
  Kn_size sizes[ 2 ];
  sizes[ 0 ] = i;
  sizes[ 1 ] = j;
  return new K2Space( i, j ); 
  // return new KnSpace( 2, sizes );
}

/**
 * Contructs a 3D digital shape from the arguments [args] lying in
 * the space [ks].
 *
 * @param args the arguments specifying the shape to build.
 * @param ks the digital space.
 * @return a digital shape (empty if no shape was specified).
 * @see addSimple3DShapesArgs
 */
ImaGene::KnCharSet
ImaGene::ShapeHelper::makeSimple3DShapesFromArgs
( const Arguments & args,
  KnSpace & ks )
{
  // center
  // cout << "--- Space: " << ks << endl;
  float x0 = (float) ks.size( 0 ) / 2.0;
  float y0 = (float) ks.size( 1 ) / 2.0;
  float z0 = (float) ks.size( 2 ) / 2.0;
  if ( args.check( "-center" ) )
    {
      x0 = args.getOption( "-center" )->getFloatValue( 0 );
      y0 = args.getOption( "-center" )->getFloatValue( 1 );
      z0 = args.getOption( "-center" )->getFloatValue( 2 );
    }
  RnMap f = RnMap::makeImplicitSphere( 0.0 ); 
  if ( args.check( "-ellipse" ) )
    {
      f = RnMap::makeEllipsoid3D
	( args.getOption( "-ellipse" )->getFloatValue( 0 ),
	  args.getOption( "-ellipse" )->getFloatValue( 1 ),
	  args.getOption( "-ellipse" )->getFloatValue( 2 ) );
    }
  else if ( args.check( "-sphere" ) )
    {
      f = RnMap::makeImplicitSphere
	( args.getOption( "-sphere" )->getFloatValue( 0 ) );
    }
  else if ( args.check( "-cube" ) )
    {
      float c = args.getOption( "-cube" )->getFloatValue( 0 ) / 2.0;
      RnMap h1 = RnMap::makeHalfSpace3D(   c, 0.0, 0.0, 1.0, 0.0, 0.0 );
      RnMap h2 = RnMap::makeHalfSpace3D(  -c, 0.0, 0.0,-1.0, 0.0, 0.0 );
      RnMap h3 = RnMap::makeHalfSpace3D( 0.0,   c, 0.0, 0.0, 1.0, 0.0 );
      RnMap h4 = RnMap::makeHalfSpace3D( 0.0,  -c, 0.0, 0.0,-1.0, 0.0 );
      RnMap h5 = RnMap::makeHalfSpace3D( 0.0, 0.0,   c, 0.0, 0.0, 1.0 );
      RnMap h6 = RnMap::makeHalfSpace3D( 0.0, 0.0,  -c, 0.0, 0.0,-1.0 );
      f = RnMap::max( RnMap::max( RnMap::max( h1, h2 ), 
				  RnMap::max( h3, h4 ) ),
		      RnMap::max( h5, h6 ) );
    }
  else if ( args.check( "-octahedron" ) )
    {
      float c = args.getOption( "-octahedron" )->getFloatValue( 0 );
      RnMap h1a = RnMap::makeHalfSpace3D( 0.0, 0.0,   c, 1.0, 1.0, 1.0 );
      RnMap h1b = RnMap::makeHalfSpace3D( 0.0, 0.0,  c, 1.0,-1.0, 1.0 );
      RnMap h1c = RnMap::makeHalfSpace3D( 0.0, 0.0,  c,-1.0, 1.0, 1.0 );
      RnMap h1d = RnMap::makeHalfSpace3D( 0.0, 0.0,  c,-1.0,-1.0, 1.0 );
      RnMap h2a = RnMap::makeHalfSpace3D( 0.0, 0.0, -c, 1.0, 1.0,-1.0 );
      RnMap h2b = RnMap::makeHalfSpace3D( 0.0, 0.0, -c, 1.0,-1.0,-1.0 );
      RnMap h2c = RnMap::makeHalfSpace3D( 0.0, 0.0, -c,-1.0, 1.0,-1.0 );
      RnMap h2d = RnMap::makeHalfSpace3D( 0.0, 0.0, -c,-1.0,-1.0,-1.0 );
      f = RnMap::max( RnMap::max( RnMap::max( h1a, h1b ), 
				  RnMap::max( h1c, h1d ) ),
		      RnMap::max( RnMap::max( h2a, h2b ), 
				  RnMap::max( h2c, h2d ) ) );
    }
  else if ( args.check( "-cms" ) )
    {
      float c = args.getOption( "-cms" )->getFloatValue( 0 ) / 2.0;
      float r = args.getOption( "-cms" )->getFloatValue( 1 );
      RnMap h1 = RnMap::makeHalfSpace3D(   c, 0.0, 0.0, 1.0, 0.0, 0.0 );
      RnMap h2 = RnMap::makeHalfSpace3D(  -c, 0.0, 0.0,-1.0, 0.0, 0.0 );
      RnMap h3 = RnMap::makeHalfSpace3D( 0.0,   c, 0.0, 0.0, 1.0, 0.0 );
      RnMap h4 = RnMap::makeHalfSpace3D( 0.0,  -c, 0.0, 0.0,-1.0, 0.0 );
      RnMap h5 = RnMap::makeHalfSpace3D( 0.0, 0.0,   c, 0.0, 0.0, 1.0 );
      RnMap h6 = RnMap::makeHalfSpace3D( 0.0, 0.0,  -c, 0.0, 0.0,-1.0 );
      RnMap cube = RnMap::max( RnMap::max( RnMap::max( h1, h2 ), 
					   RnMap::max( h3, h4 ) ),
			       RnMap::max( h5, h6 ) );
      f = RnMap::max( cube,
		      RnMap::neg( RnMap::makeImplicitSphere( r ) ) );
    }
   else if ( args.check( "-cps" ) )
    {
      float c = args.getOption( "-cps" )->getFloatValue( 0 ) / 2.0;
      float r = args.getOption( "-cps" )->getFloatValue( 1 );
      RnMap h1 = RnMap::makeHalfSpace3D(   c, 0.0, 0.0, 1.0, 0.0, 0.0 );
      RnMap h2 = RnMap::makeHalfSpace3D(  -c, 0.0, 0.0,-1.0, 0.0, 0.0 );
      RnMap h3 = RnMap::makeHalfSpace3D( 0.0,   c, 0.0, 0.0, 1.0, 0.0 );
      RnMap h4 = RnMap::makeHalfSpace3D( 0.0,  -c, 0.0, 0.0,-1.0, 0.0 );
      RnMap h5 = RnMap::makeHalfSpace3D( 0.0, 0.0,   c, 0.0, 0.0, 1.0 );
      RnMap h6 = RnMap::makeHalfSpace3D( 0.0, 0.0,  -c, 0.0, 0.0,-1.0 );
      RnMap cube = RnMap::max( RnMap::max( RnMap::max( h1, h2 ), 
					   RnMap::max( h3, h4 ) ),
			       RnMap::max( h5, h6 ) );
      if(args.check("-centerSphere")){
	float xS = args.getOption("-centerSphere")->getFloatValue(0); 
	float yS = args.getOption("-centerSphere")->getFloatValue(1); 
	float zS = args.getOption("-centerSphere")->getFloatValue(2); 	
	Vector sCenter(3);
	sCenter.rw(0)=xS; sCenter.rw(1)=yS; sCenter.rw(2)=zS; 
	f = RnMap::min( cube,
			RnMap::makeImplicitSphere( r,sCenter )  );
     
      }else{
	f = RnMap::min( cube,
			RnMap::makeImplicitSphere( r )  );
      }
    }
  RnMap rot = RnMap::makeTranslation3D( -x0, -y0, -z0 );
  if ( args.check( "-rz" ) )
    {
      float rz = args.getOption( "-rz" )->getFloatValue( 0 );
      rot = RnMap::make3DRotationZ( rz ) ( rot );
    }
  if ( args.check( "-ry" ) )
    {
      float ry = args.getOption( "-ry" )->getFloatValue( 0 );
      rot = RnMap::make3DRotationY( ry ) ( rot );
    }
  RnMap embedded_shape = f( rot );
  KnCharSet voxset = KnTools::negativeSpels( ks, embedded_shape );
  return voxset;
}

/**
 * Add the standard arguments for defining simple 3D shapes to argument
 * object [args].
 * -sphere <r>
 * -ellipse <a[=10]> <b[=5]> <c[=3]>
 * -cube <c>
 * -cms <c> <r>
 * -ry <radian>
 * -rz <radian>
 *
 * @param args (updated) the argument object to enrich.
 * @see makeSimple3DShapesFromArgs
 */
void
ImaGene::ShapeHelper::addSimple3DShapesArgs( Arguments & args )
{
  args.addOption( "-sphere", "-sphere <r>: the shape is a sphere of radius r.", "10" );
  args.addOption( "-ellipse", "-ellipse <a> <b> <c>: the shape is an ellipse of radii a,b,c along x,y,z.", "10", "5", "3" );
  args.addOption( "-cube", "-cube <c>: the shape is a cube of side <c> that can be rotated with -ry and -rz.", "10" );
  args.addOption( "-octahedron", "-octahedron <c>: the shape is a octahedron of side <c> that can be rotated with -ry and -rz.", "10" );
  args.addOption( "-cms", "-cms <c> <r>: the shape is a cube of side <c> minus a ball of radius <r>.", "10", "10" );
  args.addOption( "-cps", "-cps <c> <r>: the shape is a cube of side <c> plus a ball of radius <r>.", "10", "10" );
  args.addOption("-centerSphere", "-centerSphere <x> <y> <z> specifies the center of the sphere, used only in cps shapes", "0", "0","0");
  args.addOption( "-ry", "-ry <radian>: specifies the rotation to perform around the y-axis.", "0.5" );
  args.addOption( "-rz", "-rz <radian>: specifies the rotation to perform around the z-axis.", "0.3" );
  args.addOption( "-center", "-center <x0> <y0> <z0>: specifies the center of the shapes. Default is at the center of the digital space", "10", "10", "10" );
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ShapeHelper::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ShapeHelper]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ShapeHelper::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
