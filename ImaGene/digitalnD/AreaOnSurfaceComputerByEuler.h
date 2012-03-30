/** @file AreaOnSurfaceComputerByEuler.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : AreaOnSurfaceComputerByEuler.h
//
// Creation : 2003/07/30
//
// Version : 2003/07/30
//
// Author : JOL
//
// Summary : Header file for files AreaOnSurfaceComputerByEuler.ih and AreaOnSurfaceComputerByEuler.cxx
//
// History :
//	2003/07/30 : ?Name? : ?What?
//
// Rcs Id : "@(#)class AreaOnSurfaceComputerByEuler declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(AreaOnSurfaceComputerByEuler_RECURSES)
#error Recursive header files inclusion detected in AreaOnSurfaceComputerByEuler.h
#else // defined(AreaOnSurfaceComputerByEuler_RECURSES)
#define AreaOnSurfaceComputerByEuler_RECURSES

#if !defined AreaOnSurfaceComputerByEuler_h
#define AreaOnSurfaceComputerByEuler_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/DigitalSurfaceGeometry.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class AreaOnSurfaceComputerByEuler
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'AreaOnSurfaceComputerByEuler' <p>
 * Aim: Another technique to computer the elementary area of a surfel.
 * It is just the scalar product of the estimated normal and the trivial 
 * normal.
 */
class AreaOnSurfaceComputerByEuler : public AreaOnSurfaceComputer
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~AreaOnSurfaceComputerByEuler();

  /**
   * Constructor. Does nothing.
   */
  AreaOnSurfaceComputerByEuler();

  // ------------------------- Geometry services ------------------------------
public:
  /**
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @return the estimated area of the surfel [b].
   */
  virtual float computeArea( DigitalSurfaceGeometry & surf_geom_ctxt, 
			     Kn_sid b );


  // ------------------------- Interface --------------------------------------
public:
  /**
   * Writes/Displays the object on an output stream.
   * @param that_stream the output stream where the object is written.
   */
  void selfDisplay( std::ostream & that_stream ) const;
  /**
   * Checks the validity/consistency of the object.
   * @return 'true' if the object is valid, 'false' otherwise.
   */
  bool OK() const;
  
  // ------------------------- Datas ------------------------------------------
private:

  // ------------------------- Hidden services --------------------------------
protected:
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE AreaOnSurfaceComputerByEuler( const AreaOnSurfaceComputerByEuler & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE AreaOnSurfaceComputerByEuler & operator=( const AreaOnSurfaceComputerByEuler & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'AreaOnSurfaceComputerByEuler'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'AreaOnSurfaceComputerByEuler' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const AreaOnSurfaceComputerByEuler & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "AreaOnSurfaceComputerByEuler.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined AreaOnSurfaceComputerByEuler_h

#undef AreaOnSurfaceComputerByEuler_RECURSES
#endif // else defined(AreaOnSurfaceComputerByEuler_RECURSES)
