/** @file KnTools.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnTools.h
//
// Creation : 2003/07/24
//
// Version : 2003/07/24
//
// Author : JOL
//
// Summary : Header file for files KnTools.ih and KnTools.cxx
//
// History :
//	2003/07/24 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnTools declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnTools_RECURSES)
#error Recursive header files inclusion detected in KnTools.h
#else // defined(KnTools_RECURSES)
#define KnTools_RECURSES

#if !defined KnTools_h
#define KnTools_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <deque>
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnRCellSet.h"
#include "ImaGene/base/RnMaps.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class KnTools
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnTools' <p>
 * Aim: Provides several tools for objects in 'KnSpace'. For instance,
 * conversion between sets.
 */
class KnTools
{
  // ------------------------- Static services ------------------------------
public:

  /**
   * Builds a KnCharSet equivalent to [rset].
   * @param rset any set of cells (oriented or not).
   * @return the corresponding KnCharSet.
   */
  static KnCharSet convert( const KnRCellSet & rset );
  
  /**
   * Builds a set of spels from a space [ks] and an implicit mapping [f] from
   * Rn to R. All the spels whose centroid has a value by [f] inferior or equal
   * to 0.0 are considered inside the set.
   * @param ks a digital space
   * @param f an implicit mapping from Rn to R with 'n==ks.dim()'.
   * @return the corresponding KnCharSet.
   */
  static KnCharSet negativeSpels( const KnSpace & ks,
				  const RnMap & f );

  /**
   * Builds a set of spels from a space [ks] and an implicit mapping [f] from
   * Rn to R. All the spels whose centroid has a value by [f] inferior or equal
   * to 0.0 are considered inside the set. Restricts the computation to the
   * bounding box [low],[up].
   * @param ks a digital space
   * @param f an implicit mapping from Rn to R with 'n==ks.dim()'.
   * @param low the spel with smallest coordinates in the bounding box.
   * @param up the spel with highest coordinates in the bounding box.
   * @return the corresponding KnCharSet.
   */
  static KnCharSet negativeSpels( const KnSpace & ks,
				  const RnMap & f,
				  Kn_uid low,
				  Kn_uid up );

  /**
   * In the digital space [ks], computes the set of low incident cells
   * to [c] from [min_dim] to [max_dim] and add them to [q]. Nb: if
   * 'dim(c) <= max_dim' then [c] is added to [q].
   *
   * @param q (returns) add incident cells to this deque.
   * @param ks the digital space.
   * @param c the unoriented cell of interest.
   *
   * @param max_dim the maximum dimension of the incident cells
   * (higher are discarded).
   *
   * @param min_dim the minimum dimension of the incident cells (lower
   * are discarded).
   */
  static void ulowIncidentCells( std::deque<Kn_uid> & q, 
				 const KnSpace & ks,
				 Kn_uid c,
				 uint max_dim,
				 uint min_dim = 0 );

			     

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~KnTools();

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
  INLINE KnTools();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE KnTools( const KnTools & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE KnTools & operator=( const KnTools & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'KnTools'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'KnTools' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const KnTools & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/KnTools.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnTools_h

#undef KnTools_RECURSES
#endif // else defined(KnTools_RECURSES)
