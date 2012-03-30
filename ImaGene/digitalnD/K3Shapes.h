/** @file K3Shapes.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : K3Shapes.h
//
// Creation : 2002/08/11
//
// Version : 2002/08/11
//
// Author : JOL
//
// Summary : Header file for files K3Shapes.ih and K3Shapes.cxx
//
// History :
//	2002/08/11 : ?Name? : ?What?
//
// Rcs Id : "@(#)class K3Shapes declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(K3Shapes_RECURSES)
#error Recursive header files inclusion detected in K3Shapes.h
#else // defined(K3Shapes_RECURSES)
#define K3Shapes_RECURSES

#if !defined K3Shapes_h
#define K3Shapes_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnCharSet.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class K3Shapes
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'K3Shapes' <p>
 * Aim: Defines classical algorithm for Z3.
 */
class K3Shapes
{

  // ------------------------- Static services --------------------------------
public:

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
  static KnCharSet umakeVolumicSphere( const KnSpace & ks,
				       Kn_uid center,
				       float inclusive_radius );



  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~K3Shapes();

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
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE K3Shapes();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE K3Shapes( const K3Shapes & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE K3Shapes & operator=( const K3Shapes & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'K3Shapes'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'K3Shapes' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const K3Shapes & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/K3Shapes.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined K3Shapes_h

#undef K3Shapes_RECURSES
#endif // else defined(K3Shapes_RECURSES)
