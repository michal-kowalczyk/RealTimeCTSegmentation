/** @file DMLPContour.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DMLPContour.h
//
// Creation : 2010/01/18
//
// Version : 2010/01/18
//
// Author : JOL
//
// Summary : Header file for files DMLPContour.ih and DMLPContour.cxx
//
// History :
//	2010/01/18 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DMLPContour declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DMLPContour_RECURSES)
#error Recursive header files inclusion detected in DMLPContour.h
#else // defined(DMLPContour_RECURSES)
#define DMLPContour_RECURSES

#if !defined DMLPContour_h
#define DMLPContour_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/UndoableList.h"
#include "ImaGene/dgeometry2d/OctantChange.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/mathutils/CFraction.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class DMLPContour
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'DMLPContour' <p> Aim: This class represents
   * a dynamic digital contour (i.e., modifiable with elementary
   * flips) which updates at each step its minimum length polygon (MLP
   * for short). It is particularly adapted to represent a deformable
   * contour model.
   */
  class DMLPContour
  {


    // ----------------------- Public enum types ------------------------------	
  public:

    /**
     * Determines if the vertex lies on the pixels of the shape (INSIDE)
     * or outside.
     */
    enum TouchType { INSIDE, OUTSIDE };

    /**
     * Flips INSIDE/OUTSIDE.
     * @param t any TouchType.
     * @return the opposite of [t].
     */
    static TouchType flip( TouchType t );

    /**
     * The type of connection between two consecutive edges. For an
     * INSIDE vertex in a clockwise contour, the determinant \c d between the
     * two vectors is:
     * - d < 0 : WEDGE (/\\)
     * - d = 0 : FLAT  (---)
     * - d = 1 : AFLAT (-_-)
     * - d > 1 : VEE   (\\/)
     */
    enum ConnectionType { WEDGE, FLAT, AFLAT, VEE };

    /**
     * RIGHT means to the right of the vector (a seen from above), LEFT
     * to the left.
     */
    enum SideType { RIGHT, LEFT };


    // ----------------------- Public inner classes ----------------------------
  public:

    /**
     * An edge is an elementary part of a dynamic contour. It is
     * defined by a displacement vector [v], whose base type [base]
     * indicates whether the source of the vector lies inside or
     * outside the shape. The vector is always primitive, but may be
     * repeated [n] times. The transformation to put [v] in the first
     * octant is given by [oc]. In this first octant, the continued
     * fraction of the slope of the vector is stored in [z].
     */
    struct Edge {
      /**
       * The displacement vector.
       */
      Vector2i v;

      /**
       * The continued fraction of the slope of the vector [v] cast in the
       * first octant.
       */
      CFraction z;

      /**
       * The transformation to bring [v] in the first octant.
       */
      OctantChange oc;
    
      /**
       * The type of pixel at the source of the Edge vector.
       */
      TouchType base;

      /**
       * The number of times this vector is repeated.
       */
      uint n;

      /**
       * Default destructor. Nothing special.
       */
      ~Edge();

      /**
       * Default constructor. The object is invalid.
       */
      Edge();

      /**
       * Constructor. Other members are computed automatically.
       *
       * @param v0 an irreducible vector.
       * @param base0 the type of pixel at the source of the Edge vector.
       * @param n0 the number of times this vector is repeated.
       */
      Edge( Vector2i v0, TouchType base0, uint n0 );

      /**
       * Constructor. Other members are computed automatically.
       *
       * @param v0 an irreducible vector.
       * @param z0 the continued fraction of [v0] cast in the first octant.
       * @param oc0 the transformation to cast [v0] in the first octant.
       * @param base0 the type of pixel at the source of the Edge vector.
       * @param n0 the number of times this vector is repeated.
       */
      Edge( Vector2i v0, 
	    const CFraction & z0, 
	    const OctantChange & oc0, 
	    TouchType base0, 
	    uint n0 );

      /**
       * @return the vector representing the edge (n*v).
       */
      Vector2i vector() const;

      /**
       * Flip the source of the vector inside/out. Useful for dynamic
       * contour where the shape of interest is deformed by flipping
       * some border pixels inside/out.
       */
      void flip();

      /**
       * @return the number of grid steps associated with this
       * edge. It is in fact this.n * |this.v|_1.
       */
      uint digitalLength() const;

      /**
       * @return the Euclidean length of this edge.
       */
      double euclideanLength() const;

      /**
       * Displays the edge on the given output stream.
       *
       * @param out the output stream.
       */
      void selfDisplay( std::ostream & out ) const;

      /**
       * Displays the edge on the given output stream, with upper case letters.
       *
       * @param out the output stream.
       */
      void selfDisplayUpperCase( std::ostream & out ) const;

    };


    // ----------------------- Typedefs ------------------------------
  public:

    /**
     * The type representing the sequence of edges.
     */
    typedef UndoableList<DMLPContour::Edge> list_of_edges;

    /**
     * The type representing the iterator in the sequence of edges,
     * moving edge by edge.
     *
     * Such iterators could be used as usual:
     * @code
     for ( it = beginEdge(); it != endEdge(); ++it ) ...
     * @endcode
     * or as circulators:
     * @code
     it = firstEdge();
     do { ...
       it = dmlp.next( it );
     } while ( it != beginEdge() );
     * @endcode
     */
    typedef UndoableList<DMLPContour::Edge>::quasi_iterator edge_iterator;

    /**
     * The type representing the const_iterator in the sequence of
     * edges, moving edge by edge.
     *
     * Such iterators could be used as usual:
     * @code
     for ( it = beginEdge(); it != endEdge(); ++it ) ...
     * @endcode
     * or as circulators:
     * @code
     it = firstEdge();
     do { ...
       it = dmlp.next( it );
     } while ( it != beginEdge() );
     * @endcode
     */
    typedef UndoableList<DMLPContour::Edge>::const_iterator const_edge_iterator;


    // ----------------------- iterator class ---------------------------
  public:

    class iterator;
    class reverse_iterator;

    /** 
     * Description of class 'DMLPContour::iterator' <p> Aim:
     * Represents an iterator moving forward along the dynamic minimum
     * length polygon one linel at a time. Each iterator has a boolean
     * specifying if the flip is toward the inside or toward the
     * outside.
     */
    class iterator
    {
    public:
      friend class reverse_iterator;

      /**
       * The dynamic contour where the iterator is circulating.
       */
      DMLPContour & dmlp;

      /**
       * Indicates if a forthcoming flip is toward the inside ('true')
       * or the outside ('false') of the digital contour.
       */
      bool inside;

    public:

      /**
       * Creates an iterator circulating on a digital contour
       * [contour], positionned at the first edge in the list of edges
       * of the contour, and the first linel (second pointel) of this
       * edge. The boolean [flip_to_inside] indicates where to flip
       * (inside or outside) for a forthcoming flip. It is useful to
       * define an iterator and its inverse after flip.
       *
       * @param contour the dynamic mlp where the iterator is circulating.
       *
       * @param flip_to_inside 'true' for a flip toward the inside,
       * 'false' otherwise.
       */
      iterator( DMLPContour & contour,
		bool flip_to_inside = true );

      /**
       * Copy constructor.
       *
       * @param other any iterator.
       */
      iterator( const iterator & other );

      /**
       * Creates an iterator circulating on a digital contour
       * [contour], positionned at the same place as the given
       * reverse_iterator. More precisely, it reverts the displacement
       * vector (coded as the last_code), such that the pointed
       * pointel is the opposite extremity of the former vector.
       *
       * @param it any reverse_iterator circulating forward on a contour.
       */
      explicit iterator( const reverse_iterator & it );

      /**
       * Creates an iterator circulating on a digital contour [contour],
       * positionned at the given edge in the list of edges of the contour,
       * and at position [pos] in this edge. The boolean [flip_to_inside]
       * indicates where to flip (inside or outside) for a forthcoming
       * flip. It is useful to define an iterator and its inverse after
       * flip.
       * 
       * NB: O( pos ) operation.
       *
       * @param contour the dynamic mlp where the iterator is circulating.
       * @param flip_to_inside 'true' for a flip toward the inside,
       * 'false' otherwise.
       * @param it_edge an iterator on the given edge.
       * @param xy the coordinate of the base vertex.
       * @param pos the position of the pointel in the edge (0 : first pointel, etc).
       */
      iterator( DMLPContour & contour,
		bool flip_to_inside,
		const edge_iterator & it_edge,
		const Vector2i & xy,
		uint pos );

      /**
       * Assignment.
       *
       * @param other an iterator circulating in the same dynamic contour.
       * @return a reference to 'this'.
       */
      iterator & operator=( const iterator & other );

      /**
       * Destructor. Nothing special.
       */
      ~iterator();

      /**
       * @return an edge iterator on the edge where the iterator is
       * currently positionned.
       */
      edge_iterator edge() const;

      /**
       * @return the coordinates of the current pointel.
       */
      const Vector2i & pointel() const;

      /**
       * @return the coordinates of the base vertex of the current edge.
       */
      const Vector2i & baseVertex() const;

      /**
       * @return the coordinates of the target vertex of the current edge.
       */
      Vector2i targetVertex() const;

      /**
       * @return the position of the pointel with respect to the
       * beginning of the edge or -1 if it is a place where the
       * digital contour turns around a vertex to be in the correct
       * quadrant.
       */
      int position() const;

      /**
       * Circulates the iterator to the next linel.
       */
      void next();

      /**
       * Circulates the iterator to the previous linel.
       * Creates a reverse_iterator to backtrack.
       */
      void previous();
      
      /**
       * Circulates the iterator to the next linel.
       */
      iterator& operator++();

      /**
       * Independent from the value of [inside].
       *
       * @param other an iterator circulating in the same dynamic contour.
       * @return 'true' iff [other] points at the same place as 'this'.
       */
      bool operator==( const iterator & other ) const;

      /**
       * Independent from the value of [inside].
       * @param other an iterator circulating in the same dynamic contour.
       * @return 'true' iff [other] points at a different place as 'this'.
       */
      bool operator!=( const iterator & other ) const;

      // ----------------------- Data ----------------------------
    private:

      /**
       * An edge iterator specifying which is the current edge.
       */
      DMLPContour::edge_iterator m_it_edge;

      /**
       * The coordinates of the pointel corresponding to this iterator.
       */
      Vector2i m_xy;
      
      /**
       * The coordinates of the base vertex of the current edge.
       */
      Vector2i m_vtx_xy;

      /**
       * The last step made by the iterator. It is used to constrain
       * the future step, since the Freeman chaincode cannot
       * backtrack.
       */
      uint m_last_code;

    };



    // ----------------------- reverse_iterator class -------------------------
  public:

    /** 
     * Description of class 'DMLPContour::reverse_iterator' <p> Aim:
     * Represents an reverse_iterator moving forward along the dynamic minimum
     * length polygon one linel at a time. Each reverse_iterator has a boolean
     * specifying if the flip is toward the inside or toward the
     * outside.
     */
    class reverse_iterator
    {
    public:
      friend class iterator;

      /**
       * The dynamic contour where the reverse_iterator is circulating.
       */
      DMLPContour & dmlp;

      /**
       * Indicates if a forthcoming flip is toward the inside ('true')
       * or the outside ('false') of the digital contour.
       */
      bool inside;

    public:

      /**
       * Creates a reverse_iterator circulating on a digital contour
       * [contour], positionned at the same place as the given
       * iterator. More precisely, it reverts the displacement vector (coded
       * as the last_code), such that the pointed pointel is the opposite
       * extremity of the former vector.
       *
       * @param it any iterator circulating forward on a contour.
       */
      explicit reverse_iterator( const iterator & it );

      /**
       * Copy constructor.
       *
       * @param other any reverse_iterator.
       */
      reverse_iterator( const reverse_iterator & other );


      /**
       * Assignment.
       *
       * @param other an reverse_iterator circulating in the same
       * dynamic contour.
       *
       * @return a reference to 'this'.
       */
      reverse_iterator & operator=( const reverse_iterator & other );

      /**
       * Destructor. Nothing special.
       */
      ~reverse_iterator();

      /**
       * @return an edge iterator on the edge where the reverse_iterator is
       * currently positionned.
       */
      edge_iterator edge() const;

      /**
       * @return the coordinates of the current pointel.
       */
      const Vector2i & pointel() const;

      /**
       * @return the coordinates of the base vertex of the current edge.
       */
      const Vector2i & baseVertex() const;

      /**
       * @return the coordinates of the target vertex of the current edge.
       */
      Vector2i targetVertex() const;

 
      /**
       * @return the position of the pointel with respect to the
       * beginning of the edge or -1 if it is a place where the
       * digital contour turns around a vertex to be in the correct
       * quadrant.
       */
      int position() const;

      /**
       * Circulates the reverse_iterator to the next linel.
       */
      void next();

      /**
       * Circulates the reverse_iterator to the previous linel.
       * @todo terminate it
       */
      void previous();
      
      /**
       * Circulates the reverse_iterator to the next linel.
       */
      reverse_iterator& operator++();

      /**
       * Independent from the value of [inside].
       *
       * @param other an reverse_iterator circulating in the same dynamic contour.
       * @return 'true' iff [other] points at the same place as 'this'.
       */
      bool operator==( const reverse_iterator & other ) const;

      /**
       * Independent from the value of [inside].
       * @param other an reverse_iterator circulating in the same dynamic contour.
       * @return 'true' iff [other] points at a different place as 'this'.
       */
      bool operator!=( const reverse_iterator & other ) const;

      // ----------------------- Data ----------------------------
    private:

      /**
       * An edge iterator specifying which is the current edge.
       */
      DMLPContour::edge_iterator m_it_edge;

      /**
       * The coordinates of the pointel corresponding to this reverse_iterator.
       */
      Vector2i m_xy;
      
      /**
       * The coordinates of the base vertex of the current edge.
       */
      Vector2i m_vtx_xy;

      /**
       * The last step made by the reverse_iterator. It is used to constrain
       * the future step, since the Freeman chaincode cannot
       * backtrack.
       */
      uint m_last_code;

    };




    // ----------------------- Static services ------------------------------
  public:

    /**
     * @param inside when 'true', specifies inside, otherwise outside.
     * @return the constant enumeration INSIDE or OUTSIDE.
     */
    static TouchType getTouchType( bool inside );


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~DMLPContour();
    
    /**
     * Constructor. The contour is not valid.
     *
     * @param cw specifies if the contour turns clockwise ('true')
     * around its inside or counterclockwise. Default is 'true'
     * (clockwise).
     */
    DMLPContour( bool cw = true );

    /**
     * @return if the contour turns clockwise ('true') around its
     * inside or counterclockwise.
     */
    bool cw() const;

    /**
     * The digital contour lies in Z x Z while the MLP lies in the
     * half-integer plane (Z+1/2) x (Z+1/2). This vector gives twice
     * the displacement to go from the former to the latter.
     * 
     * @return this displacement vector.
     */
    Vector2i twiceDV() const;

    /**
     * This function is used to compute the coordinates of a pointel
     * associated with a base vertex of an edge.
     *
     * @param it an edge iterator in this contour.
     *
     * @return the displacement vector to go from the base vertex of
     * edge [e] to its associated pointel.
     */
    Vector2i basePointel( const edge_iterator & it ) const;

    /**
     * This method initializes consistently a dynamic contour from a
     * Freeman chain. The chain is supposed to model either the inner
     * 4-connected 8-border of an object ([inside] is 'true') or its
     * outer 4-connected 8-border ([inside] is 'false'). If you wish to
     * initialize the dynamic contour with a digital interpixel contour,
     * you should use the method FreemanChain::innerContour beforehands.
     *
     * The iterators should have a getCode() method returning between 0-3
     * according to the Freeman chain code and a nextInLoop() method to
     * visit the whole Freeman chain code.
     *
     * @param itb an iterator on the beginning of Freeman chain.
     * @param ite an iterator on the end of Freeman chain.
     * @param inside indicates if it is an inner or an outer border.
     */
    template <class FreemanChainIterator>
    void initFromFreemanChain( FreemanChainIterator itb, 
			       FreemanChainIterator ite, 
			       bool inside );


    /**
     * @return the number of edges of the contour.
     */
    uint size() const;

    /**
     * @return 'true' is the contour has zero edges.
     */
    bool null() const;

    /**
     * @return the position in the plane of the start Edge.
     * @see start_edge
     */
    Vector2i getStartPosition() const;

    /**
     * Given an iterator on some edge of the contour, simplifies the
     * MLP locally around till no more changes can be done.
     *
     * @param it an iterator on some edge of 'this' contour.
     * @return the number of fusion performed.
     */
    uint simplifyAt( edge_iterator & it );

    /**
     * Simplifies the MLP of the contour till no more changes are
     * possible. As a matter of fact, it computes the MLP of the whole
     * contour. Note that this is not the fastest way of doing it.
     */
    uint simplify();

    /**
     * Fills the given vectors with the list of edge vertices
     * (i.e. the base of each edge).
     *
     * @param pts (update) the coordinates of edge base are pushed
     * at the end of this vector.
     *
     * @param inside (update) the base type of each edge base is
     * pushed at the end of this vector.
     */
    void getPoints( std::vector<Vector2i> & pts,
		    std::vector<bool> & inside ) const;
		  
    // ----------------------- iterator services -----------------------
  public:

    /**
      * Returns an iterator that moves pointel by pointel along the
      * MLP as it is tracing step by step the digital interpixel
      * contour. It also stores if the future flip is inside or
      * outside. One should use it as follows:
      * @code
      iterator it1 = first();
      iterator it = it1;
      do { ...
        ++it;
      } while ( it != it1 );
      * @endcode
      *
      * @param flip_to_inside 'true' for a flip toward the inside,
      * 'false' otherwise.
      *
      * @return an iterator that should be used as a circulator.
      */
    iterator first( bool flip_to_inside = true );

    // ----------------------- edge iterator services -----------------------
  public:

    /**
     * Such iterators could be used as usual:
     * @code
     for ( it = beginEdge(); it != endEdge(); ++it ) ...
     * @endcode
     * or as circulators.
     * @see firstEdge
     *
     * @return the iterator on the first edge of the contour.
     */
    edge_iterator beginEdge();

    /**
     * @return the iterator after the last edge of the contour.
     */
    edge_iterator endEdge();
    
    /**
     * This is used to have a circulator on the contour.
     * @code
     it = firstEdge();
     do { ...
       it = dmlp.next( it );
     } while ( it != firstEdge() );
     * @endcode
     *
     * @return an iterator on the first edge of the contour (i.e., the
     * one corresponding to the starting coordinates).
     */
    edge_iterator firstEdge();

    /**
     * This is used to have a circulator with coordinates on the contour.
     * @code
     Vector2i v;
     it = firstEdge( v );
     do { ...
       it = dmlp.next( it, v );
     } while ( it != firstEdge() );
     * @endcode
     *
     * @param ref_v (returns) the coordinates of the base of the first
     * edge.
     *
     * @return an iterator on the first edge of the contour (i.e., the
     * one corresponding to the starting coordinates).
     */
    edge_iterator firstEdge( Vector2i & ref_v );

    /**
     * Given a edge circulator [it] on the contour, returns the one on the
     * next element.
     * @param it an iterator in the list of edges [contour].
     * @return the next one (assuming the contour is a loop).
     */
    edge_iterator next( const edge_iterator & it );

    /**
     * Given a edge circulator [it] on the contour, returns the one on the
     * previous element.
     * @param it an iterator in the list of edges [contour].
     * @return the previous one (assuming the contour is a loop).
     */
    edge_iterator prev( const edge_iterator & it );

    /**
     * Given a edge circulator [it] on the contour, returns the one on the
     * next element.
     *
     * @param it an iterator in the list of edges [contour].
     *
     * @param ref_v (modifies) this point is moved according to the
     * edge vector.
     *
     * @return the next one (assuming the contour is a loop).
     */
    edge_iterator next( const edge_iterator & it, Vector2i & ref_v );

    /**
     * Given a edge circulator [it] on the contour, returns the one on the
     * previous element.
     *
     * @param it an iterator in the list of edges [contour].
     *
     * @param ref_v (modifies) this point is moved according to the
     * previous edge vector.
     *
     * @return the previous one (assuming the contour is a loop).
     */
    edge_iterator prev( const edge_iterator & it, Vector2i & ref_v );



    /**
     * Such iterators could be used as usual:
     * @code
     for ( it = beginEdge(); it != endEdge(); ++it ) ...
     * @endcode
     * or as circulators.
     * @see first
     *
     * @return the iterator on the first edge of the contour.
     */
    const_edge_iterator beginEdge() const;

    /**
     * @return the iterator after the last edge of the contour.
     */
    const_edge_iterator endEdge() const;
    
    /**
     * This is used to have a circulator on the contour.
     * @code
     it = firstEdge();
     do { ...
       it = dmlp.next( it );
     } while ( it != firstEdge() );
     * @endcode
     *
     * @return an iterator on the first edge of the contour (i.e., the
     * one corresponding to the starting coordinates).
     */
    const_edge_iterator firstEdge() const;

    /**
     * This is used to have a circulator with coordinates on the contour.
     * @code
     Vector2i v;
     it = firstEdge( v );
     do { ...
       it = dmlp.next( it, v );
     } while ( it != firstEdge() );
     * @endcode
     *
     * @param ref_v (returns) the coordinates of the base of the first
     * edge.
     *
     * @return an iterator on the first edge of the contour (i.e., the
     * one corresponding to the starting coordinates).
     */
    const_edge_iterator firstEdge( Vector2i & ref_v ) const;


    /**
     * Given a edge circulator [it] on the contour, returns the one on the
     * next element.
     * @param it an iterator in the list of edges [contour].
     * @return the next one (assuming the contour is a loop).
     */
    const_edge_iterator next( const const_edge_iterator & it ) const;

    /**
     * Given a edge circulator [it] on the contour, returns the one on the
     * previous element.
     * @param it an iterator in the list of edges [contour].
     * @return the previous one (assuming the contour is a loop).
     */
    const_edge_iterator prev( const const_edge_iterator & it ) const;

    /**
     * Given a edge circulator [it] on the contour, returns the one on the
     * next element.
     *
     * @param it an iterator in the list of edges [contour].
     *
     * @param ref_v (modifies) this point is moved according to the
     * edge vector.
     *
     * @return the next one (assuming the contour is a loop).
     */
    const_edge_iterator next( const const_edge_iterator & it, 
			      Vector2i & ref_v ) const;

    /**
     * Given a edge circulator [it] on the contour, returns the one on the
     * previous element.
     *
     * @param it an iterator in the list of edges [contour].
     *
     * @param ref_v (modifies) this point is moved according to the
     * previous edge vector.
     *
     * @return the previous one (assuming the contour is a loop).
     */
    const_edge_iterator prev( const const_edge_iterator & it, 
			      Vector2i & ref_v ) const;



    // ----------------------- edge services ---------------------------------
  public:

    /**
     * Inserts the edge [n] before the element pointed by [it]. The user
     * should take care beforehands that the starting position is not
     * modified.
     *
     * @param it an iterator in the contour.
     * @param n a edge to insert (which is copied).
     * @return an iterator pointing on the created edge within the contour.
     */
    edge_iterator insertEdge( const edge_iterator & it, const Edge & n );

    /**
     * The edge pointed by [it] is assigned the edge [n].  The user
     * should take care beforehands that the starting position is not
     * modified.
     *
     * @param it (modifies) an iterator in the contour, returns the
     * iterator on the modified edge (could be different from [it] in
     * storing mode).
     *
     * @param n the edge to copy.
     */
    void modifyEdge( edge_iterator & it, const Edge & n );

    /**
     * The edge pointed by [it] is a vector v, n times. If n == 1,
     * then flip base type of next edge, otherwise subdivide edge as
     * (1,1,n-1) and change base type of second edge. The user should
     * take care beforehands that the starting position is not
     * modified.
     *
     * @param it (modifies) an iterator in the contour, returns the
     * iterator on the modified edge (different from [it]).
     */
    void flipBaseTypeOfTarget( edge_iterator & it );

    /**
     * Remove the edge pointed by [it] from the dynamic contour.  The user
     * should take care beforehands that the starting position is not
     * modified.
     *
     * @param it an iterator in the contour.
     *
     * @return an iterator pointing on the edge which follows the
     * suppressed one.
     */
    edge_iterator removeEdge( const edge_iterator & it );

    /**
     * Moves the start edge of the contour one edge forward. Useful
     * before modifying a edge which happens to be the start edge of the
     * contour.
     */
    void moveStartEdgeForward();

    /**
     * Moves the start edge of the contour one edge backward. Useful
     * before modifying a edge which happens to be the start edge of the
     * contour.
     */
    void moveStartEdgeBackward();

    /**
     * Moves the start edge of the contour one edge backward only if it
     * is equal to [it]. Useful before modifying a edge which happens to
     * be the start edge of the contour.
     *
     * @param it an iterator in the dynamic contour.
     */
    void moveStartEdgeBackwardIfEqual( const edge_iterator & it );

    /**
     * Moves the start Edge to the beginning of the contour,
     * i.e. 'contour.begin()'. Useful so that the the logical start
     * reflects the physical start.
     */
    void moveStartEdgeToBegin();

    /**
     * The start edge should not be either on [it] or on 'prev(it)'.
     * Models an elementary swap at the base vertex of [it]. It means
     * that the vectors of 'prev(it)' of [it] are exchanged. If [flip]
     * is true, then the vertex touch type is flipped INSIDE/OUTSIDE,
     * otherwise it it left unchanged.
     *
     * @param it (modifies) an iterator on a edge, whose base vertex is
     * the one being swapped, returns an iterator on the swaped edge (the
     * former place pointed by [it])
     *
     * @param flip when 'true', the touch type of [it] is reversed (no
     * modification of the underlying digital shape), otherwise it is
     * left unchanged (the digital shape has one more or one less
     * pixel).
     */
    void swapEdge( edge_iterator & it, bool flip = true );

    // ----------------------- geometric services -----------------------
  public:

    /**
     * The edge pointed by [it] has a direction vector v and a base type
     * t (an INSIDE pixel or an OUTSIDE pixel).
     *
     * @param it an iterator on a edge of the contour.
     *
     * @return RIGHT if it is to the right of v that the pixels are of
     * type t, otherwise left. Depends of course if the contour is
     * considered clockwise or counterclockwise.
     *
     * @see cw
     */
    SideType getSameSide( const edge_iterator & it ) const;

    /**
     * The edge pointed by [it] has a direction vector v and a base type
     * t (an INSIDE pixel or an OUTSIDE pixel).
     *
     * @param it an iterator on a edge of the contour.
     *
     * @return RIGHT if it is to the right of v that the pixels are \b
     * not of type t, otherwise left. Depends of course if the contour
     * is considered clockwise or counterclockwise.
     *
     * @see cw
     */
    SideType getOtherSide( const edge_iterator & it ) const;

    /**
     * Given an iterator on a edge, returns what is the shape of the
     * vertex (base of the edge) according to the directions of its
     * surrounding edges, but also to the fact it is an INSIDE or
     * OUTSIDE vertex, and according to the fact that the contour is
     * clockwise or counterclockwise.
     *
     * @param it any iterator on a edge of the contour, whose base
     * vertex is the vertex of interest.
     *
     * @return the connection type between 'prev(it)' and [it], in WEDGE
     * (i.e. convex-like), FLAT, AFLAT (i.e. almost flat), VEE
     * (concave).
     */
    ConnectionType getConnectionType( const edge_iterator & it );


    // ----------------------- split services -----------------------
  public:
    
    /**
     * The edge should be primitive non trivial.  Splits the edge E(zk)
     * into E(zk-1)E(zk-2) according to the parity of the slope. Returns
     * an iterator on the second part of the fraction.
     */
    edge_iterator splitBerstelLowerChristoffel( edge_iterator & it, 
						TouchType split_tt );

    /**
     * The edge should be primitive non trivial.  Splits the edge E(zk)
     * into E(zk-1)E(zk-2) according to the parity of the slope. Returns
     * an edge_iterator on the second part of the fraction.
     */
    edge_iterator splitBerstelUpperChristoffel( edge_iterator & it, 
						TouchType split_tt );

    /**
     * Given some iterator [it] on a edge, split it at its target
     * (front) so that there is a trivial edge at its front (in fact,
     * depth <= 1). The edge vector is split on its lower side (standard
     * Berstel/splitting decomposition) when [lower] is 'true',
     * otherwise it is split on its upper side (the reverse). [t] gives
     * the base type assigned to each created base vertex.
     *
     * @param itn an iterator on a edge of the contour.
     * @param lower the type of split.
     * @param t the base type for the created base vertices.
     * @return an iterator on the created trivial edge.
     */
    edge_iterator splitAtTargetTilQuasiTrivial( edge_iterator itn, 
						bool lower,
						TouchType t );

    /**
     * Given some iterator [it] on a edge, split it at its source/base (back)
     * so that [it] points to a trivial edge (in fact, depth <= 1). The
     * edge vector is split on its lower side (standard
     * Berstel/splitting decomposition) when [lower] is 'true',
     * otherwise it is split on its upper side (the reverse). [t] gives
     * the base type assigned to each created base vertex.
     *
     * @param itn an iterator on a edge of the contour.
     * @param lower the type of split.
     * @param t the base type for the created base vertices.
     * @return an iterator on the created trivial edge (in fact [it]).
     */
    edge_iterator splitAtBaseTilQuasiTrivial( edge_iterator itn, 
					      bool lower,
					      TouchType t );
    
    /**
     * Given some iterator [it] on a edge, split it at its target
     * (front) so that there is a trivial edge at its front (in fact,
     * vector (1,0) repeated once). The edge vector is split on its
     * lower side (standard Berstel/splitting decomposition) when
     * [lower] is 'true', otherwise it is split on its upper side (the
     * reverse). [t] gives the base type assigned to each created base
     * vertex.
     *
     * @param itn an edge_iterator on a edge of the contour.
     * @param lower the type of split.
     * @param t the base type for the created base vertices.
     * @return an iterator on the created trivial edge.
     */
    edge_iterator splitAtTargetTilTrivial( edge_iterator itn, 
					   bool lower,
					   TouchType t );
    
    /**
     * Given some iterator [it] on a edge, split it at its source/base
     * (back) so that [it] points to a trivial edge (in fact, vector
     * (1,0) repeated once). The edge vector is split on its lower side
     * (standard Berstel/splitting decomposition) when [lower] is
     * 'true', otherwise it is split on its upper side (the
     * reverse). [t] gives the base type assigned to each created base
     * vertex.
     *
     * @param itn an iterator on a edge of the contour.
     * @param lower the type of split.
     * @param t the base type for the created base vertices.
     * @return an iterator on the created trivial edge (in fact [it]).
     */
    edge_iterator splitAtBaseTilTrivial( edge_iterator itn, 
					 bool lower,
					 TouchType t );

    /**
     * This method decomposes a quasi trivial vector ( (1,x) or (x,1) )
     * into trivial vectors (1,0), (0,1) in different quadrant.
     *
     * [it] points to a primitive edge with some vector (1,x) or (x,1)
     * or (-1,x) or (x,-1).  Decomposes it into two edges in different
     * quadrant.
     *
     * The edge vector is split on its lower side (standard
     * Berstel/splitting decomposition) when [lower] is 'true',
     * otherwise it is split on its upper side (the reverse). [t] gives
     * the base type assigned to each created base vertex.
     *
     * Note also that the user should have moved away the start edge
     * from this zone.
     *
     * @param it an iterator in this contour which points to a quasi trivial edge.
     * @param lower indicates if the split must be lower or upper.
     * @param t the base type for the created base vertices.
     * @return an iterator on the created trivial edge (which follows [it]).
     */
    edge_iterator splitQuasiTrivial( edge_iterator & it, 
				     bool lower, 
				     TouchType t );


    /**
     * Given a edge pointed by [it] with n repetitions, decompose it
     * into two edges, one with n-[k] repetitions to the left, one with
     * [k] repetitions to the right. Return the iterator in the middle
     * if the edge has been split, otherwise [it] itself.
     *
     * O(1) operation.
     *
     * Note also that the user should have moved away the start edge
     * from this zone.
     *
     * @param it (modified) any iterator on the contour (points on the
     * edge that is subdivided). Although it may be modified if in
     * storing mode, it points on the same location in the list.
     * 
     * @param k indicates how it is subdivided into two aligned edges.
     *
     * @param t1 gives the new base type for the first part of the
     * subdivision (generally it->base).
     *
     * @param t2 gives the new base type for the second part of the
     * subdivision (generally it->base).
     *
     * @return an iterator on the created edge (after [it]), or [it]
     * itself if nothing was changed.
     */
    edge_iterator subdivide( edge_iterator & it, uint k,
			     TouchType t1,
			     TouchType t2 );


    /**
     * Given a edge pointed by [it] with n repetitions, decompose it
     * into two edges, one with [k] repetitions to the left, one with
     * n-[k] repetitions to the right. Return the iterator in the middle
     * if the edge has been split, otherwise [it] itself.
     *
     * O(1) operation.
     *
     * Note also that the user should have moved away the start edge
     * from this zone.
     *
     * @param it (modified) any iterator on the contour (points on the
     * edge that is subdivided). Although it may be modified if in
     * storing mode, it points on the same location in the list.
     * 
     * @param k indicates how it is subdivided into two aligned edges.
     *
     * @param t1 gives the new base type for the first part of the
     * subdivision (generally it->base).
     *
     * @param t2 gives the new base type for the second part of the
     * subdivision (generally it->base).
     *
     * @return an iterator on the created edge (after [it]), or [it]
     * itself if nothing was changed.
     */
    edge_iterator subdivideLeft( edge_iterator & it, 
				 uint k, TouchType t1, TouchType t2 );


    /**
     * This method is the base for flipping a pixel inside/out. Its
     * objective is to make the mlp trivial around the pointel pointed
     * by [it]. After that, it is easy to flip the trivial edges
     * inside/out.
     *
     * @param it an iterator in this contour (no more valid afterwards).
     * @param lower indicates if the split must be lower or upper.
     * @param t the base type for the created base vertices.
     *
     * @return an iterator on the created trivial edge (replacing
     * [it]). The preceding edge is also a trivial.
     */
    iterator splitAround( iterator it, bool lower, TouchType t );

    /**
     * This method is the base for flipping a pixel inside/out. Its
     * objective is to make the mlp trivial around the pointel pointed
     * by [it]. After that, it is easy to flip the trivial edges
     * inside/out.
     *
     * @param it an iterator in this contour (no more valid afterwards).
     * @param lower indicates if the split must be lower or upper.
     * @param t the base type for the created base vertices.
     *
     * @return an iterator on the created trivial edge (replacing
     * [it]). The preceding edge is also a trivial.
     */
    edge_iterator splitEdgeAt( edge_iterator & it, 
			       uint pos,
			       bool lower, TouchType t );

    // ----------------------- simplification services -----------------------
  public:

    /**
     * Checks if vertex is neutral, meaning that the two edges around
     * have opposite vectors and base types. 
     *
     * It may also remove an inconsistent vertex: prev( it ) and it are
     * opposite, of same base type, but next( it ) has a base type
     * different from prev( it ), although it is in the same place. For
     * now, only inconsistent vertex with trivial surrounding edges are
     * removed. They may appear in a VEE splitting with two edges in
     * different quadrants.
     *
     * @param it (modified) an iterator in this contour, which is moved
     * to the next valid edges.
     *
     * @return 'true' if the contour was changed, 'false' otherwise.
     */
    bool removeNeutral( edge_iterator & it );

    /**
     * Checks if vertex is back and forth, meaning that the two edges
     * around have opposite vectors. The new base type depends on the
     * surrounding base types. If prev( it )->base == next( it )-> base
     * then the new base type is this one, otherwise, the new base type
     * is the same as it->base.
     *       
     @verbatim
     !      would give    !
     v                    v
     it x<==x                itn x==>o==>x
     ==>(o==>)^2x             
     @endverbatim
     *
     * Contrary to removeNeutral, this method will always remove the two
     * edges surrounding the vertex (if they are opposite) and assign a
     * some base type for next(it). It this base type is changed, next(
     * it ) is subdivided. 
     * 
     * Note also that the user should have moved away the start edge
     * from this zone.
     *
     * @param it (modified) an iterator in this contour, which is moved
     * to the next valid edge.
     *
     * @return 'true' if the contour was changed, 'false' otherwise.
     */
    bool removeOpposite( edge_iterator & it );

    /**
     * This is the main simplification method when computing dynamically
     * a minimum length polygon. Its objective is to have only WEDGE or
     * FLAT vertices. If some vertex (pointed by [it]) is some VEE, then
     * it changes the vertex (and the surrounding edges) to make the
     * dynamic contour pass through the symetric vertex (diagonally
     * opposite to it in the one-wide band of the digital contour),
     * which is of course of opposite base type. After that, this vertex
     * is a WEDGE (since the base type has changed). Furthermore, this
     * method will remove neutral vertices (FLAT, but opposite and with
     * opposite base type) and AFLAT vertices.
     *
     * This is not generally a O(1) operation. Split takes O(log(n))
     * (and for now O(log^2(n))).
     *
     * @param it (modifed) an iterator on a edge of the contour
     * (specifies the base vertex). The iterator may be moved to point
     * on a valid vertex.
     *
     * @return 'true' if the contour was changed, 'false' otherwise.
     *
     * @see ConnectionType.
     */
    bool fusion( edge_iterator & it );

    /**
     * This method merges an AFLAT vertex designated by the two
     * consecutive edges [it] and [itn].
     *
     * Merge the two edges pointed by [it] and [itn], provided they are
     * mergeable. Not very efficient for now, since we do not exploit
     * the repetitions of the vectors. Assume that the vectors of the
     * two edges have determinant 1 in a clockwise contour with an
     * INSIDE join.
     *
     * Note also that the user should have moved away the start edge
     * from this zone.
     *
     * @param it (modified) an iterator in the contour (the edge is
     * subdivided if its repetition number is greater than 1).
     *
     * @param itn (modified) an iterator in the contour equal to
     * 'next( it )' (the edge is subdivided if its repetition number
     * is greater than 1).
     *
     * @return an iterator on the merged edge.
     *
     * @TODO do it fast !
     */
    edge_iterator lowerFusion( edge_iterator & it, 
			       edge_iterator & itn );

    /**
     * This method merges an AFLAT vertex designated by the two
     * consecutive edges [it] and [itn].
     *
     * Merge the two edges pointed by [it] and [itn], provided they are
     * mergeable. Not very efficient for now, since we do not exploit
     * the repetitions of the vectors. Assume that the vectors of the
     * two edges have determinant 1 in a clockwise contour with an
     * INSIDE join. For now, calls lowerFusion.
     *
     * Note also that the user should have moved away the start edge
     * from this zone.
     *
     * @param it (modified) an iterator in the contour (the edge is
     * subdivided if its repetition number is greater than 1).
     *
     * @param itn (modified) an iterator in the contour equal to
     * 'next( it )' (the edge is subdivided if its repetition number
     * is greater than 1).
     *
     * @return an iterator on the merged edge.
     *
     * @TODO do it fast !
     */
    edge_iterator upperFusion( edge_iterator & it, 
			       edge_iterator & itn );



    // ----------------------- deformable model services -----------------------
  public:


    /**
     * Creates a dynamic MLP for the given interpixel contour
     * specified as a Freeman chaincode of '0', '1', '2', '3'
     * characters. The coded contour [s] must be closed.
     *
     * @param s a Freeman chaincode.
     *
     * @return an iterator in this DMLP pointing exactly at the base
     * of the first coded linel in [s] (i.e., corresponds to the base
     * of the first character).
     */
    iterator initFromInterpixelContour( const FreemanChain & s );

    /**
     * Given the coordinates of a pointel (the base of a linel),
     * returns an iterator on the corresponding point of the DMLP.
     *
     * @param x the x-coordinate of the pointel (in the original frame
     * as given in input).
     *
     * @param y the y-coordinate of the pointel (in the original frame
     * as given in input).
     *
     * @return the iterator on the corresponding point of the DMLP. If it
     * was found, it has the correct coordinates and its member 'inside'
     * has value 'true'. If not, the iterator is positionned on the start
     * edge, with its member 'inside' equal to 'false'.
     */
    iterator findPointel( int x, int y );

    /**
     * @return the length (i.e. perimeter) of the current MLP.
     */
    double getLength() const;

    /**
     * @return the current number of updates for computing the
     * Euclidean length of the contour. Since the contour is dynamic,
     * this computation is also dynamic. Note that errors may
     * accumulate. In this case, it could be wise to call
     * updateLength() to restart from a global length recomputation.
     */
    uint nbOfUpdates() const;

    /**
     * Forces the recomputation of the length (i.e. perimeter) of the
     * current MLP. This is because the length of the DMLP is updated
     * after each flip by local computations and errors may tend to
     * accumulate. Automatically called by 'flip' after a given number
     * of calls.
     */
    void updateLength();
    
    /**
     * Computes the length of the current MLP by summing the length of
     * each edge.
     */
    double computeLength() const;

    /**
     * Flips the dynamic MLP at position specified by [it] and in the
     * direction specified by 'it.inside'.
     *
     * @param it specifies the position where to flip the MLP.
     *
     * @param store_undo when 'true', stores the reverse flip
     * operation that will undo this change. This parameter may be
     * false only if the stack 'm_undo' is empty.
     *
     * @return 'true' iff the flip was successfully performed, 'false'
     * if it was unable to do it.
     *
     * @see undoFlip
     * @see topUndo
     * @see m_undo
     */
    bool flip( const iterator & it, bool store_undo = false );

    /**
     * Undoes the last flip operation. Can be used only if
     * 'store_undo' was set during the last flip operation.
     */
    void undoFlip();

    /**
     * @return the iterator on this modified MLP which induces the
     * inverse flip to go back to the previous MLP. 
     */
    iterator topUndo() const;

    /**
     * @return 'true' if the stack of stored inverse flips is not empty.
     */
    bool isUndoPossible() const;

    /**
     * Clears the stack of stored inverse flips.
     */
    void clearUndo();

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Displays the contour on the output stream for debugging purposes.
     *
     * @param out the output stream.
     */
    void selfDisplay( std::ostream & out ) const;

    /**
     * Displays the contour on the output stream for debugging
     * purposes. Emphasizes the given iterator [it] in upper case
     * letters.
     *
     * @param out the output stream.
     * @param it an iterator in the list of edges of the contour.
     */
    void selfDisplayWithIterator( std::ostream & out, 
				  const edge_iterator & it );

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool OK() const;
  

    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * The dynamic contour is essentially a list of edges.
     */
    list_of_edges m_contour;

    /** 
     * The dynamic contour has a starting position, specified by the
     * iterator [start_edge], and with a position in the plane
     * [start_pos].
     */
    edge_iterator m_start_edge;

    /** 
     * The dynamic contour has a starting position, specified by the
     * iterator [start_edge], and with a position in the plane
     * [start_pos].
     */
    Vector2i m_start_pos;

    /**
     * The digital contour lies in Z x Z while the MLP lies in the
     * half-integer plane (Z+1/2) x (Z+1/2). This vector gives twice
     * the displacement to go from the former to the latter.
     */
    Vector2i m_twice_dv;

    /**
     * 'true' if contour turns around inside clockwise (INSIDE to
     * RIGHT), 'false' otherwise (INSIDE TO LEFT).
     */
    bool m_cw;

    /**
     * the length (i.e. perimeter) of the current MLP.
     */
    double m_length;

    /**
     * Counts the number of incremental updates of the DMLP.
     */
    uint m_nb_of_updates;
    
    /**
     * Stores reverse flip operations so that a user may undo a series
     * of changes on the MLP. Also stores length to avoid error
     * accumulations.
     */
    std::stack< std::pair< iterator, double > > m_undo;



    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE DMLPContour( const DMLPContour & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE DMLPContour & operator=( const DMLPContour & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'DMLPContour::Edge'.
   * @param that_stream the output stream where the object is written.
   * @param n the object of class 'DMLPContour::Edge' to write.
   * @return the output stream after the writing.
   */
  std::ostream &
  operator<<( std::ostream & that_stream, const DMLPContour::Edge & n );

  /**
   * Overloads 'operator<<' for displaying objects of class 'DMLPContour'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'DMLPContour' to write.
   * @return the output stream after the writing.
   */
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const DMLPContour & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/DMLPContour.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DMLPContour_h

#undef DMLPContour_RECURSES
#endif // else defined(DMLPContour_RECURSES)
