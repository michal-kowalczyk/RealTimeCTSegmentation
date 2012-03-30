/** @file KnSpaceCoordScanner.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnSpaceCoordScanner.h
//
// Creation : 2003/07/03
//
// Version : 2003/07/03
//
// Author : JOL
//
// Summary : Header file for files KnSpaceCoordScanner.ih and KnSpaceCoordScanner.cxx
//
// History :
//	2003/07/03 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnSpaceCoordScanner declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnSpaceCoordScanner_RECURSES)
#error Recursive header files inclusion detected in KnSpaceCoordScanner.h
#else // defined(KnSpaceCoordScanner_RECURSES)
#define KnSpaceCoordScanner_RECURSES

#if !defined KnSpaceCoordScanner_h
#define KnSpaceCoordScanner_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
//#include "LinAlg/LinAlg/Vector.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/digitalnD/KnTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

  class KnSpace;

///////////////////////////////////////////////////////////////////////////////
// class KnSpaceCoordScanner
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnSpaceCoordScanner' <p>
 * Aim: Object to scan a portion of a space and which updates the centroid
 * of the current element. Useful if you often need the coordinates of the
 * current element.
 * <p>
 * Use as follows: <pre>
 * KnSpace kn( ... );
 * Kn_uid first, last; // lower and upper bounds 
 * KnSpaceCoordScanner scanner( kn, first, last );
 * Kn_uid p = scanner.lower_bound;
 * Vector vp = ks.ucentroid( p );
 * do 
 *   { // ... whatever [p] is the current cell
 *     // ... and [vp] its centroid
 *   }
 * while ( scanner.increment( p, vp ) ); 
 * </pre>
 */
class KnSpaceCoordScanner
{

  // ------------------------- public attributes ------------------------------
public:
  /** 
   * A const reference to the space the iterator is scanning.
   */
  const KnSpace & space;
  /**
   * This is the lower bound (in all dimensions) of the scanning. It is also
   * the starting point.
   */
  const Kn_uid lower_bound;
  /**
   * This is the upper bound (in all dimensions) of the scanning. It is also
   * the ending point.
   */
  const Kn_uid upper_bound;

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~KnSpaceCoordScanner();

  /**
   * Constructor.
   * The iterator object offers a simple way to scan a parallepipedic subspace
   * of a 'KnSpace'.
   * @param sp the space where the scanning is taking place.
   * @param lower the code of an element of [sp] (of any type).
   * @param upper the code of another element of [sp] (same type as [lower]).
   */
  KnSpaceCoordScanner( const KnSpace & sp, Kn_uid lower, Kn_uid upper );

  // ------------------------- Generic iteration services ---------------------
public:

  /**
   * Generic increment the cell [p] to its next position (as classically done 
   * in a scanning).
   * Exemple: <pre> 
   * KnSpace kn( ... );
   * Kn_uid first, last; // lower and upper bounds 
   * KnSpaceCoordScanner scanner( kn, first, last );
   * Kn_uid p = scanner..lower_bound;
   * Vector vp = ks.ucentroid( p );
   * do 
   *   { // ... whatever [p] is the current cell
   *     // ... and [vp] its centroid
   *   }
   * while ( scanner.increment( p, vp ) ); 
   * </pre>
   * @param p any cell (modified).
   * @param c the centroid of [p] (modified).
   * @return true if p is still within the bounds, false if the scanning is finished.
   */
  INLINE bool increment( Kn_uid & p, Vector & c ) const;

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
   * Forbidden by default (protected to avoid g++ warnings).
   */
  INLINE KnSpaceCoordScanner();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE KnSpaceCoordScanner( const KnSpaceCoordScanner & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE KnSpaceCoordScanner & operator=( const KnSpaceCoordScanner & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'KnSpaceCoordScanner'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'KnSpaceCoordScanner' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const KnSpaceCoordScanner & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "KnSpaceCoordScanner.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnSpaceCoordScanner_h

#undef KnSpaceCoordScanner_RECURSES
#endif // else defined(KnSpaceCoordScanner_RECURSES)
