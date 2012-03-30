/** @file K3Isosurface_k6_l18.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : K3Isosurface_k6_l18.h
//
// Creation : 2002/08/17
//
// Version : 2002/08/17
//
// Author : JOL
//
// Summary : Header file for files K3Isosurface_k6_l18.ih and K3Isosurface_k6_l18.cxx
//
// History :
//	2002/08/17 : ?Name? : ?What?
//
// Rcs Id : "@(#)class K3Isosurface_k6_l18 declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(K3Isosurface_k6_l18_RECURSES)
#error Recursive header files inclusion detected in K3Isosurface_k6_l18.h
#else // defined(K3Isosurface_k6_l18_RECURSES)
#define K3Isosurface_k6_l18_RECURSES

#if !defined K3Isosurface_k6_l18_h
#define K3Isosurface_k6_l18_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/K3Isosurface.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

  class KnSpace;

///////////////////////////////////////////////////////////////////////////////
// class K3Isosurface_k6_l18
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'K3Isosurface_k6_l18' <p>
 * Aim: Specialization of K3Isosurface for 6-connectedness of foreground and
 * 18-connectedness of background.
 */
class K3Isosurface_k6_l18 : public K3Isosurface
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. Does nothing.
   */
  INLINE ~K3Isosurface_k6_l18();

  /**
   * Constructor.
   * Creates an isosurface calculator for 6-18 connectedness couple.
   * @param ks this is the space where the isosurface is created.
   */
  K3Isosurface_k6_l18( const KnSpace & ks );

  // ------------------------- Datas ------------------------------------------
private:

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE K3Isosurface_k6_l18();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE K3Isosurface_k6_l18( const K3Isosurface_k6_l18 & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE K3Isosurface_k6_l18 & operator=( const K3Isosurface_k6_l18 & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/K3Isosurface_k6_l18.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined K3Isosurface_k6_l18_h

#undef K3Isosurface_k6_l18_RECURSES
#endif // else defined(K3Isosurface_k6_l18_RECURSES)
