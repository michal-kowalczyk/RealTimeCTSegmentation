/** @file ExplicitDigitalSurface.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ExplicitDigitalSurface.h
//
// Creation : 2011/05/04
//
// Version : 2011/05/04
//
// Author : JOL
//
// Summary : Header file for files ExplicitDigitalSurface.ih and ExplicitDigitalSurface.cxx
//
// History :
//	2011/05/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ExplicitDigitalSurface declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ExplicitDigitalSurface_RECURSES)
#error Recursive header files inclusion detected in ExplicitDigitalSurface.h
#else // defined(ExplicitDigitalSurface_RECURSES)
#define ExplicitDigitalSurface_RECURSES

#if !defined ExplicitDigitalSurface_h
#define ExplicitDigitalSurface_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/KnRCellSet.h"
#include "ImaGene/digitalnD/DigitalSurface.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class ExplicitDigitalSurface
///////////////////////////////////////////////////////////////////////////////
/** 
    Description of class 'ExplicitDigitalSurface' <p>
    Aim: To represent a digital surface as a set of signed surfels.
 */
class ExplicitDigitalSurface : public DigitalSurface
{
  // ------------------------- Standard services ------------------------------
public:
  /**
     Destructor. 
   */
  virtual ~ExplicitDigitalSurface();

  /**
     Constructor. Represents the digital surface as the set of signed
     surfels [surfels].

     @param badj any bel adjacency of the space where [object] is defined.
     @param surfels the set of signed surfels.
   */
  ExplicitDigitalSurface( const BelAdjacency & badj, 
			  const KnRCellSet & surfels );


  /**
   * @return a pointer on the digital space in which the factory is embedded.
   */
  virtual const KnSpace* space() const;

  /**
   * @return the bel adjacency chosen for this surface.
   */
  const BelAdjacency & belAdjacency() const;
  
  /**
   * @return the set of surfels of the surface.
   */
  const KnRCellSet & surface() const;

  // ------------------------- set services -------------------------------
public:
  /**
     Useful for 'SurfelNeighborhood::adjacentOnOrientedSurfaces'.
     
     @param surfel any surfel of the digital space.
     @return 'true' if the surfel belongs to the digital surface, 'false' otherwise.
  */
  bool operator[]( Kn_sid surfel ) const;

  // ------------------------- Factory services ------------------------------
public:

  /**
     Returns a surface tracker pointing on [surfel].

     @param surfel any surfel.
   
     @return a dyn. alloc. surface tracker (must be desallocated by
     the caller).
  */
  virtual DigitalSurfaceTracker* newTracker( Kn_sid surfel ) const;


  /**
     Returns an iterator pointing on [surfel] and moving along direction [dir]
     in the direct tracking direction. Each movement is given as a direction
     on a 4-connected contour.
     
     @param surfel any surfel.
     @param dir a tracking direction (different from the orthogonal direction to [surfel]).
     @return a dyn. alloc. contour iterator (in fact, a 'C4CIteratorOnBdry').
     @see C4CIterator,C4CIteratorOnBdry
  */
  virtual C4CIteratorOnSurface* newC4CIterator( Kn_sid surfel, uint dir ) const;

  
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
   * Reference on a bel adjacency relation, which indicates the topology of 
   * the bels.
   */
  const BelAdjacency & m_badj;

  /**
   * The set of surfels forming the digital surface.
   */
  KnRCellSet m_surfels;

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE ExplicitDigitalSurface();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE ExplicitDigitalSurface( const ExplicitDigitalSurface & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE ExplicitDigitalSurface & operator=( const ExplicitDigitalSurface & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'ExplicitDigitalSurface'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'ExplicitDigitalSurface' to write.
 * @return the output stream after the writing.
 */
std::ostream&
operator<<( std::ostream & that_stream, 
	    const ExplicitDigitalSurface & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/ExplicitDigitalSurface.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ExplicitDigitalSurface_h

#undef ExplicitDigitalSurface_RECURSES
#endif // else defined(ExplicitDigitalSurface_RECURSES)
