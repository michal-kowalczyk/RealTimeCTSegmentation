/** @file TypedObject.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : TypedObject.h
//
// Creation : 2004/01/20
//
// Version : 2004/01/20
//
// Author : JOL
//
// Summary : Header file for files TypedObject.ih and TypedObject.cxx
//
// History :
//	2003/02/12 : ?Name? : ?What?
//
// Rcs Id : "@(#)class TypedObject declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(TypedObject_RECURSES)
#error Recursive header files inclusion detected in TypedObject.h
#else // defined(TypedObject_RECURSES)
#define TypedObject_RECURSES

#if !defined TypedObject_h
#define TypedObject_h

//////////////////////////////////////////////////////////////////////////////
// JOL removed since ImageLib-new
// #include "MiscLib/Util/TypeId.hpp"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class TypedObject
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'TypedObject' <p>
 * Aim: Interface that forces subclasses to be typed.
 */
class TypedObject
{

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  INLINE virtual ~TypedObject();

  // ------------------------- type services -----------------------
public:

  /**
   * @return the type id of the instantiated subclass.
   */
  // JOL changed since ImageLib-new
  virtual uint getTypeId() const = 0;
  // virtual misclib::TypeId getTypeId() const = 0;

};

} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/TypedObject.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined TypedObject_h

#undef TypedObject_RECURSES
#endif // else defined(TypedObject_RECURSES)
