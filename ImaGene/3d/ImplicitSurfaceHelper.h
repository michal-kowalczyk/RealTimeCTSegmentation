/** @file ImplicitSurfaceHelper.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImplicitSurfaceHelper.h
//
// Creation : 2008/03/20
//
// Version : 2008/03/20
//
// Author : JOL
//
// Summary : Header file for files ImplicitSurfaceHelper.ih and ImplicitSurfaceHelper.cxx
//
// History :
//	2008/03/20 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImplicitSurfaceHelper declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImplicitSurfaceHelper_RECURSES)
#error Recursive header files inclusion detected in ImplicitSurfaceHelper.h
#else // defined(ImplicitSurfaceHelper_RECURSES)
#define ImplicitSurfaceHelper_RECURSES

#if !defined ImplicitSurfaceHelper_h
#define ImplicitSurfaceHelper_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/Arguments.h"
#include "ImaGene/3d/ImplicitSurface.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class ImplicitSurfaceHelper
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitSurfaceHelper' <p>
   * Aim: A utility class to build implicit surfaces.
   */
  class ImplicitSurfaceHelper
  {

    // ----------------------- Static services ------------------------------
  public:

    /**
     * Builds an implicit surface from arguments given as follows:
     *
     * - "-iconex" : ConeSurfaceExt
     * - "-whitneyx" : WhitneyUmbrellaExt
     * - "-simonx" : SimonUmbrellaExt
     * - "-4linesx" : FourLinesSurfaceExt
     * - "-funnyx" : FunnySurfaceExt
     *
     * @param args an object storing arguments.
     *
     * @return a dynamically allocated implicit surface object.
     */
    static ImplicitSurface* implicitSurfaceFromArgs( const Arguments & args );
 
    /**
     * Add the standard arguments for defing implicit surfaces to argument
     * object [args].
     *
     * @param args (updated) the argument object to enrich.
     */
    static void addImplicitSurfaceArgs( Arguments & args );
 
  };

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/ImplicitSurfaceHelper.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImplicitSurfaceHelper_h

#undef ImplicitSurfaceHelper_RECURSES
#endif // else defined(ImplicitSurfaceHelper_RECURSES)
