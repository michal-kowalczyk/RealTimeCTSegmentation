/** @file KnSpaceScanner.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnSpaceScanner.h
//
// Creation : 2002/07/27
//
// Version : 2002/07/27
//
// Author : JOL
//
// Summary : Header file for files KnSpaceScanner.ih and KnSpaceScanner.cxx
//
// History :
//	2002/07/27 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnSpaceScanner declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnSpaceScanner_RECURSES)
#error Recursive header files inclusion detected in KnSpaceScanner.h
#else // defined(KnSpaceScanner_RECURSES)
#define KnSpaceScanner_RECURSES

#if !defined KnSpaceScanner_h
#define KnSpaceScanner_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnTypes.h"
//#include "ImaGene/digitalnD/KnSpace.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

  class KnSpace;

///////////////////////////////////////////////////////////////////////////////
// class KnSpaceScanner
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnSpaceScanner' <p>
 * Aim: Object to scan a portion of a space. 
 * <p>
 * Use as follows (here in K4): <pre>
 * KnSpace k4( 4, ... );
 * KnSpaceScanner scan( k4, lower_bd, upper_bd );
 * Kn_uid last_t, last_z, last_y, last_x;
 * Kn_uid p = scan.begin();
 * for ( last_t = scan.last( p, 3 );
 *	p <= last_t; 
 *	p += scan.gotonext( 3 ) )
 *   for ( last_z = scan.last( p, 2 );
 *	  p <= last_z; 
 *	  p += scan.gotonext( 2 ) )
 *     for ( last_y = scan.last( p, 1 );
 *	    p <= last_y; 
 *	    p += scan.gotonext( 1 ) )
 *	for ( last_x = scan.last( p, 0 ); 
 *	      p <= last_x; 
 *	      p++ ) // NB: 'scan.gotonext( 0 )' == 1;
 *        { //... whatever
 *        }
 * </pre>
 */
class KnSpaceScanner
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
  ~KnSpaceScanner();
  /**
   * Constructor.
   * The iterator object offers a simple way to scan a parallepipedic subspace
   * of a 'KnSpace'.
   * @param sp the space where the scanning is taking place.
   * @param lower the code of an element of [sp] (of any type).
   * @param upper the code of another element of [sp] (same type as [lower]).
   */
  KnSpaceScanner( const KnSpace & sp, Kn_uid lower, Kn_uid upper );

  // ------------------------- Iteration services -----------------------------
public:
  /**
   * @return the code of the first element that will be scanned (ie the lower bound).
   * @see lower_bound
   */
  INLINE Kn_uid begin() const;
  /**
   * Given an element [c] of the space and a dimension [k], returns the element
   * that is the projection of [c] on the lower_bound in the direction of the
   * dimension [k].
   * @param c a cell.
   * @param k any dimension.
   * @return the cell with the same coordinates as [c] except in the dimension [k] where the coordinate is identical to the one of the lower bound.
   */
  INLINE Kn_uid first( Kn_uid c, uint k ) const;
  /**
   * Given an element [c] of the space and a dimension [k], returns the element
   * that is the projection of [c] on the upper_bound in the direction of the
   * dimension [k].
   * @param c a cell.
   * @param k any dimension.
   * @return the cell with the same coordinates as [c] except in the dimension [k] where the coordinate is identical to the one of the upper bound.
   */
  INLINE Kn_uid last( Kn_uid c, uint k ) const;
  /**
   * This code should be added to the iterating cell when a [k]-dimensional
   * row is finished (see class definition). In fact it is the difference 
   * between the code of the next [k]-dimensional row and the code of the
   * element just after the last of the preceding row.
   * @param k any dimension.
   * @return the code to add to the iterating cell at the end of a [k]-dimensional row.
   */
  INLINE Kn_uid gotonext( uint k ) const;
  

  // ------------------------- Generic iteration services ---------------------
public:

  /**
   * Generic increment the cell [p] to its next position (as classically done 
   * in a scanning).
   * Exemple: <pre> 
   * KnSpace kn( ... );
   * Kn_uid first, last; // lower and upper bounds 
   * KnSpaceScanner scanner( kn, first, last );
   * Kn_uid p = scanner.begin();
   * do 
   *   { // ... whatever [p] is the current cell
   *   }
   * while ( scanner.increment( p ) ); 
   * </pre>
   * @param p any cell.
   * @return true if p is still within the bounds, false if the scanning is finished.
   */
  INLINE bool increment( Kn_uid & p ) const;


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
  /**
   * An array of offsets where each element is the increment to add to a 
   * cellcode to move to the next element. The 0th is always 1, the 1st is
   * added when at the end of a row, the 2nd is added at the end of a plane,
   * etc.
   */
  Kn_uid* m_gotonext;

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  inline KnSpaceScanner();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  inline KnSpaceScanner( const KnSpaceScanner & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  inline KnSpaceScanner & operator=( const KnSpaceScanner & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'KnSpaceScanner'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'KnSpaceScanner' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const KnSpaceScanner & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/KnSpaceScanner.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnSpaceScanner_h

#undef KnSpaceScanner_RECURSES
#endif // else defined(KnSpaceScanner_RECURSES)
