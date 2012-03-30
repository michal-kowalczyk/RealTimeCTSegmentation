/** @file C4CIteratorOnSurface.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : C4CIteratorOnSurface.h
//
// Creation : 2003/02/12
//
// Version : 2003/02/12
//
// Author : JOL
//
// Summary : Header file for files C4CIteratorOnSurface.ih and C4CIteratorOnSurface.cxx
//
// History :
//	2003/02/12 : ?Name? : ?What?
//
// Rcs Id : "@(#)class C4CIteratorOnSurface declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(C4CIteratorOnSurface_RECURSES)
#error Recursive header files inclusion detected in C4CIteratorOnSurface.h
#else // defined(C4CIteratorOnSurface_RECURSES)
#define C4CIteratorOnSurface_RECURSES

#if !defined C4CIteratorOnSurface_h
#define C4CIteratorOnSurface_h

//////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/dgeometry2d/C4CIterator.h"
//////////////////////////////////////////////////////////////////////////////

namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class C4CIteratorOnSurface
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'C4CIteratorOnSurface' <p>
 * Aim: An interface to specializes a 'C4CIterator'. This iterator is in fact
 * moving on a digital surface.
 */
class C4CIteratorOnSurface : public C4CIterator
{
  // ------------------------- Contour services ------------------------------
public:
  /**
   * Destructor
   */
  virtual ~C4CIteratorOnSurface() {}

  /**
   * @return the current surfel the iterator is pointing at.
   */
  virtual Kn_sid current() const = 0;

  /**
   * @return the current track direction.
   */
  virtual uint trackDir() const = 0;

  /**
   * @return the current orthogonal direction.
   */
  virtual uint orthDir() const = 0;
  
};

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene

#endif // !defined C4CIteratorOnSurface_h

#undef C4CIteratorOnSurface_RECURSES
#endif // else defined(C4CIteratorOnSurface_RECURSES)
