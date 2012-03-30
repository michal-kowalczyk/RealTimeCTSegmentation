///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ImplicitSurfaceHelper.cxx
//
// Creation : 2008/03/20
//
// Version : 2008/03/20
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
//	2008/03/20 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/3d/ImplicitSurfaces.h"
#include "ImaGene/3d/ImplicitSurfaces3D.h"
#include "ImaGene/3d/ImplicitSurfaceHelper.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/3d/ImplicitSurfaceHelper.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ImplicitSurfaceHelper_RCS_ID = "@(#)class ImplicitSurfaceHelper definition.";



///////////////////////////////////////////////////////////////////////////////
// class ImplicitSurfaceHelper
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Static services - public :


/**
 * Builds an implicit surface from arguments given as follows:
 *
 * - "-icone" : ConeSurface
 * - "-whitney" : WhitneyUmbrella
 * - "-simon" : SimonUmbrella
 * - "-4lines" : FourLinesSurface
 * - "-funny" : FunnySurface
 *
 * @param args an object storing arguments.
 *
 * @return a dynamically allocated implicit surface object.
 */
ImaGene::ImplicitSurface* 
ImaGene::ImplicitSurfaceHelper::implicitSurfaceFromArgs
( const Arguments & args )
{
  ImplicitSurface* mis = 0;
  if ( args.check( "-icone" ) )
    mis = new ConeSurface;
  else if ( args.check( "-whitney" ) )
    mis = new WhitneyUmbrella;
  else if ( args.check( "-simon" ) )
    mis = new SimonUmbrella;
  else if ( args.check( "-4lines" ) )
    mis = new FourLinesSurface;
  else if ( args.check( "-funny" ) )
    mis = new FunnySurface;
  else if ( args.check( "-paunescu" ) )
    mis = new PaunescuSurface;
  else if ( args.check( "-cayley3" ) )
    mis = new CayleyCubicSurface;
  // Surfaces are taken from
  //  http://www.freigeist.cc/gallery.html
  else if ( args.check( "-durchblick" ) )
    {
      // x3y+xz3+y3z+z3+5z
      Polynomial3<float> p;
      p.add( Monomial3(3,1,0), 1.0f );
      p.add( Monomial3(1,0,3), 1.0f );
      p.add( Monomial3(0,3,1), 1.0f );
      p.add( Monomial3(0,0,3), 1.0f );
      p.add( Monomial3(0,0,1), 5.0f );
      mis = new PolynomialSurface( p );
    }
  //  http://www.freigeist.cc/gallery.html
  else if ( args.check( "-tobel" ) )
    {
      // x3 z + x2 + yz3 + z4 = 3xyz
      Polynomial3<float> p;
      p.add( Monomial3(3,0,1), 1.0f );
      p.add( Monomial3(2,0,0), 1.0f );
      p.add( Monomial3(0,1,3), 1.0f );
      p.add( Monomial3(0,0,4), 1.0f );
      p.add( Monomial3(1,1,1), -3.0f );
      mis = new PolynomialSurface( p );
    }
  //  http://www.freigeist.cc/gallery.html
  else if ( args.check( "-miau" ) )
    {
      //  x2yz + x2z2 + 2 y3z + 3 y3 = 0
      Polynomial3<float> p;
      p.add( Monomial3(2,1,1), 1.0f );
      p.add( Monomial3(2,0,2), 1.0f );
      p.add( Monomial3(0,3,1), 2.0f );
      p.add( Monomial3(0,3,0), 3.0f );
      mis = new PolynomialSurface( p );
    }
  //  http://www.freigeist.cc/gallery.html
  else if ( args.check( "-crixxi" ) )
    {
      //  (y2+z2-1)2 +(x2+y2-1)3 = 0
      // developed =
      // y4 +2y2z2+z4-2z2 -y2
      // + x6+3x4y2+3x2y4+y6-3x4-6x2y2-3y4+3x2
      Polynomial3<float> p;
      p.add( Monomial3(0,4,0), 1.0f );
      p.add( Monomial3(0,2,2), 2.0f );
      p.add( Monomial3(0,2,0), 1.0f );
      p.add( Monomial3(0,0,4), 1.0f );
      p.add( Monomial3(0,0,2), -2.0f );
      p.add( Monomial3(6,0,0), 1.0f );
      p.add( Monomial3(4,2,0), 3.0f );
      p.add( Monomial3(2,4,0), 3.0f );
      p.add( Monomial3(0,6,0), 1.0f );
      p.add( Monomial3(4,0,0), -3.0f );
      p.add( Monomial3(2,2,0), -6.0f );
      p.add( Monomial3(0,4,0), -3.0f );
      p.add( Monomial3(2,0,0), 3.0f );
      
      mis = new PolynomialSurface( p );
    }
  //  http://www.freigeist.cc/gallery.html
  else if ( args.check( "-buggle" ) )
    {
      //   x4y2+y4x2-x2y2+z6=0
      Polynomial3<float> p;
      p.add( Monomial3(4,2,0), 1.0f );
      p.add( Monomial3(2,4,0), 1.0f );
      p.add( Monomial3(2,2,0), -1.0f );
      p.add( Monomial3(0,0,6), 1.0f );
      mis = new PolynomialSurface( p );
    }
  //  http://www.freigeist.cc/gallery.html
  else if ( args.check( "-wedeln" ) )
    {
      //   x3 = y (1-z2)2 
      // x3-y+2yz2-yz4=0
      Polynomial3<float> p;
      p.add( Monomial3(3,0,0), 1.0f );
      p.add( Monomial3(0,1,0), -1.0f );
      p.add( Monomial3(0,1,2), 2.0f );
      p.add( Monomial3(0,2,4), -1.0f );
      mis = new PolynomialSurface( p );
    }
  else if ( args.check( "-simon2" ) )
    {
      // (z2-(x2+y2))^2-(xy)^2(4-z2)=0
      // x4 + y4 + z4 - 2x2y2 -2 x2z2 - 2 y2z2 + x2y2z2 = 0
      Polynomial3<float> p;
      p.add( Monomial3(4,0,0), 1.0f );
      p.add( Monomial3(0,4,0), 1.0f );
      p.add( Monomial3(0,0,4), 1.0f );
      p.add( Monomial3(2,2,0), -2.0f );
      p.add( Monomial3(2,0,2), -2.0f );
      p.add( Monomial3(0,2,2), -2.0f );
      p.add( Monomial3(2,2,2), 1.0f );
      mis = new PolynomialSurface( p );
    }
 	
  return mis;
}


/**
 * Add the standard arguments for defing implicit surfaces to argument
 * object [args].
 *
 * @param args (updated) the argument object to enrich.
 */
void
ImaGene::ImplicitSurfaceHelper::addImplicitSurfaceArgs
( Arguments & args )
{
  args.addBooleanOption( "-icone", "-icone: defines an implicit cone 4d extension." );
  args.addBooleanOption( "-whitney", "-whitney: defines a Whitney umbrella 4d extension." );
  args.addBooleanOption( "-simon", "-simon: defines a Simon umbrella 4d extension." );
  args.addBooleanOption( "-4lines", "-4lines: defines the four lines surface 4d extension." );
  args.addBooleanOption( "-funny", "-funny: defines a funny implicit surface miwing quadrics and trigonometry." );
  args.addBooleanOption( "-paunescu", "-paunescu: defines Paunescu implicit surface." );
  args.addBooleanOption( "-cayley3", "-cayley3: defines Cayley cubic implicit surface." );
  args.addBooleanOption( "-durchblick", "-durchblick: defines durchblick surface  x3y+xz3+y3z+z3+5z = 0." );
  args.addBooleanOption( "-tobel", "-tobel: defines Tobel surface x3 z + x2 + yz3 + z4 = 3xyz." );
  args.addBooleanOption( "-miau", "-miau: defines Miau surface x2yz + x2z2 + 2 y3z + 3 y3 = 0." );
  args.addBooleanOption( "-crixxi", "-crixxi: defines Crixxi surface (y2+z2-1)2 +(x2+y2-1)3 = 0." );
  args.addBooleanOption( "-buggle", "-buggle: defines Buggle surface x4y2+y4x2-x2y2+z6 = 0." );
  args.addBooleanOption( "-wedeln", "-wedeln: defines Wedeln surface x3 - y (1-z2)2= 0." );
  args.addBooleanOption( "-simon2", "-simon2: defines Simon surface (z2-(x2+y2))^2-(xy)^2(4-z2)=0." );
}


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :



///////////////////////////////////////////////////////////////////////////////
// Interface - public :




///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
