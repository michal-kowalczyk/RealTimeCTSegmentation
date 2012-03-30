/** @file CharacteristicPolygon.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : CharacteristicPolygon.h
//
// Creation : 2008/06/20
//
// Version : 2008/06/20
//
// Author : JOL
//
// Summary : Header file for files CharacteristicPolygon.ih and CharacteristicPolygon.cxx
//
// History :
//	2008/06/20 : ?Name? : ?What?
//
// Rcs Id : "@(#)class CharacteristicPolygon declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(CharacteristicPolygon_RECURSES)
#error Recursive header files inclusion detected in CharacteristicPolygon.h
#else // defined(CharacteristicPolygon_RECURSES)
#define CharacteristicPolygon_RECURSES

#if !defined CharacteristicPolygon_h
#define CharacteristicPolygon_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/Vector.h"
#include "ImaGene/mathutils/Line2D.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/digitalnD/Frame2D.h"
#include "ImaGene/helper/C4CTangentialCoverGeometry.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class CharacteristicPolygon
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CharacteristicPolygon' <p>
   * Aim: Encodes the characteristic polygon of a digital contour.
   */
  class CharacteristicPolygon
  {
  public:


    /** 
     * Description of struct 'CharacteristicPolygon::Edge' <p>
     * Aim: Encodes each edge of the characteristic polygon.
     */
    struct Edge
    {
    public:
      uint idx1;
      uint idx2;
      uint n;
      uint quadrant;
      uint dir;
      bool convex_back;
      bool convex_front;
      C4CSegment dss;
      Frame2D frame_back;

    public:

      /**
       * @return the point of the characteristic polygon corresponding
       * to the first pointel of the edge.
       *
       * NB: should be equal to 'getP2()' of the previous edge.
       */
      INLINE Vector2D getP1() const;

      /**
       * @return the point of the characteristic polygon corresponding
       * to the last pointel of the edge.
       *
       * NB: should be equal to 'getP1()' of the next edge.
       */
      INLINE Vector2D getP2() const;

      /**
       * @return the median line between interior and exterior.
       */
      INLINE Line2D medianLine() const;

      /**
       * @param alpha the coefficient determining the distance to the MLP. 0
       * is the MLP, 0.5 is the median polygon. 1 is furthest polygon away
       * from the MLP while satisfying interior/exterior separation.
       *
       * @return some intermediate line between interior and exterior.
       */
      INLINE Line2D intermediateLine( float alpha ) const;

    };


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~CharacteristicPolygon();

    /**
     * Constructor. The object is not valid.
     */
    INLINE CharacteristicPolygon();

    /**
     * Initializes the characteristic polygon with the structure and
     * geometry of a digital contour.
     *
     * @param tcover the tangential cover of a digital contour.
     * @param tcover_geometry the geometry of [tcover].
     */
    void init( const C4CTangentialCover & tcover,
	       const C4CTangentialCoverGeometry & tcover_geometry );

    /**
     * @return the shape of each surfel as characters '0', '1', '2'.
     */ 
    INLINE const std::string & shape() const;

    /**
     * @return the edges of the polygon.
     */ 
    INLINE const std::vector<Edge> & edges() const;

    /**
     * @return an iterator on the first surfel of the characteristic
     * polygon (lower-left corner).
     */
    INLINE const C4CIterator* startSurfel() const;

    /**
     * @return the index of the maximal segment containing the first
     * surfel (lower-left corner) at its front.
     */
    INLINE uint startMS() const;

    // ----------------------- Interface --------------------------------------
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
  

    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * Starting surfel (lower left corner of the shape). Orientation
     * is 3. Next surfel has orientation 0.
     */
    uint m_idx_ll;

    /**
     * Iterator on starting surfel.
     */
    C4CIterator* m_it_ll;

    /**
     * Starting maximal segment. Starting surfel is at front of this
     * maximal segment. (lower left corner of the shape). Orientation
     * is 3. Next surfel has orientation 0.
     */
    uint m_idx_ms_ll;

    /**
     * Reconstructed Freeman chain code.
     */
    std::string m_chain;

    /**
     * Quadrant for each linel of the Freeman chain code.
     */
    std::string m_qchain;

    /**
     * Shape for each linel: 0: inflexion, 1: convex, 2: concave
     */
    std::string m_shape;

    /**
     * Stores the set of edges of the characteristic polygon.
     */
    std::vector<Edge> m_edges;

    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE CharacteristicPolygon( const CharacteristicPolygon & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE CharacteristicPolygon & operator=( const CharacteristicPolygon & other );
  
    // ------------------------- Internals ------------------------------------
  private:

    /**
     * After Initialization, extract all the edges from the computed
     * strings (Freeman chaincode [m_chain], quadrant info [m_qchain],
     * convexity info [m_shape].
     */
    void extractEdges();


    /**
     * After initialization, extract an edge in a consistent zone (as
     * given by cutting points).
     *
     * @param i the start point (the corresponding surfel is included)
     * @param j the end point  (the corresponding surfel is excluded)
     */
    void extractEdgesInZone( std::vector<Edge> & edges, uint i, uint j );

    /** 
     * After extracting edges, compute all the associated information.
     *
     * @param tcover the tangential cover of a digital contour.
     * @param tcover_geometry the geometry of [tcover].
     */
    void computeEdgesInformation
    ( const C4CTangentialCover & tcover,
      const C4CTangentialCoverGeometry & tcover_geometry );



  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'CharacteristicPolygon'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'CharacteristicPolygon' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const CharacteristicPolygon & that_object_to_display );


  /**
   * Overloads 'operator<<' for displaying objects of class
   * 'CharacteristicPolygon::Edge'.
   *
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'CharacteristicPolygon' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const CharacteristicPolygon::Edge & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/CharacteristicPolygon.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CharacteristicPolygon_h

#undef CharacteristicPolygon_RECURSES
#endif // else defined(CharacteristicPolygon_RECURSES)
