/** @file GridCurve.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : GridCurve.h
//
// Creation : 2011/02/28
//
// Version : 2011/02/28
//
// Author : JOL
//
// Summary : Header file for files GridCurve.ih and GridCurve.cxx
//
// History :
//	2011/02/28 : ?Name? : ?What?
//
// Rcs Id : "@(#)class GridCurve declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(GridCurve_RECURSES)
#error Recursive header files inclusion detected in GridCurve.h
#else // defined(GridCurve_RECURSES)
#define GridCurve_RECURSES

#if !defined GridCurve_h
#define GridCurve_h

//////////////////////////////////////////////////////////////////////////////
#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/UndoableList.h"
#include "ImaGene/mathutils/SternBrocot.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/dgeometry2d/CurveCode.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  struct ReversiblePolygonalRepresentation {
    std::vector<int> vx; /**< the x-coordinates of the RPR vertices. */
    std::vector<int> vy; /**< the y-coordinates of the RPR vertices. */
    std::vector<bool> vt;/**< the type (inside=true, outside=false) of the
			    RPR vertices. */
    Vector2i twice_dv; /**< twice the displacement vector to go from
			the integer plane of the Freeman chain to the
			half-integer plane of the MLP. */
    bool cw;           /**< true if clockwise inside convention. */

    /**
       Removes null vectors.
    */
    void purge();

    /**
       n twice_dv_x twice_dv_y cw
       vx[0] vy[0] vt[0]
       vx[1] vy[1] vt[1]
       ...
       vx[n-1] vy[n-1] vt[n-1]
    */
    bool read( std::istream & in );

    /**
       Creates a RPR from an CurveCode representing an interpixel contour.

       @param cc (modified) any interpixel curve code (no ++ and no
       --). May be modified (shifted in fact) so as to start in a
       reasonnable position with respect to the inner or outer curve
       code.

       @param inner when 'true' creates the RPR on the inner side,
       otherwise creates the RPR on the outside.
    */
    bool initFromInterpixelCurveCode( CurveCode & cc,
				      bool inner );
  };

  /////////////////////////////////////////////////////////////////////////////
  // class GridCurve
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'GridCurve' <p> Aim: This class represents a
   * dynamic digital curve (i.e., modifiable with elementary flips)
   * which updates at each step its minimum length polygon (MLP for
   * short). It is particularly adapted to represent an open deformable
   * contour model.
   */
  class GridCurve
  {
  public:

    typedef SternBrocot::SBFraction Fraction;

    /**
	 @param v (modified) simplifies inplace the Freeman chaincode
	 by removing couples of opposite letters.
      */
    static void simplifyFreemanCode( std::vector<unsigned char> & v );


    /**
       An edge is an elementary part of a dynamic contour. It is
       defined by a displacement vector (p,q)^nb, p,q>=0, optionnaly a
       tilde (inverse) operation. Furthermore, although it could be
       recomputed by starting from the beginning of the curve, we also
       store the coordinates of the base pointel, the vector basis
       defined by two letters a,b (in 0-3), the fact (boolean inside)
       that the base pixel is inside or outside. Note that quadrant
       changes are coded with a null fraction, nb=1 codes '+' (convex
       quadrant changes), nb=-1 codes '-'.

       Edges are stored in an undoable list. Therefore, they should
       not be modified (except by the GridCurve). Merges, splits are
       thus made in GridCurve, not within the Edge class.
    */
    struct Edge {
      Fraction pq; /**< The irreducible part of the displacement vector. */
      int nb;       /**< The repetition of the displacement vector. */
      bool tilde;  /**< Indicates if there is a side change. */
      Vector2i pointel; /**< the coordinates of the base pointel. */
      unsigned int a; /**< horizontal axis given by the letter a (in 0-3). */
      unsigned int b; /**< vertical axis given by the letter b (in 0-3). */
      bool inside; /**< the fact (boolean inside) that the base pixel
       is inside or outside */

      /**
	 Default destructor. Nothing special.
       */
      ~Edge();

      /**
	 Default constructor. The object is invalid.
       */
      Edge();

      /** a,b is the pre-alphabet of the edge. */
      void initContext( const Vector2i & pointel, 
			unsigned int a, unsigned int b,
			bool inside );
      /** Init the context so as to follow consistently the given edge. */
      void initContext( const Edge & previous );
      void initQuadrantChange( int nb );
      void initTrivialZero( int nb, bool tilde );
      void initTrivialInfinite( int nb, bool tilde );
      void initEdge( unsigned int p, unsigned int q, bool tilde );
      void initEdge( unsigned int p_prime, unsigned int q_prime, int nb, bool tilde );
      void initEdge( const Fraction & other_pq, int nb, bool tilde );
      bool isQuadrantChange() const;
      bool isTrivialZero() const;
      bool isTrivialInfinite() const;
      bool isTrivial() const;

      //std::vector<unsigned char> toFreemanCode() const;
      /**
	 @param v (modified) adds the Freeman chaincode associated to
	 this edge to the back of this vector.
      */
      void pushBackFreemanCode( std::vector<unsigned char> & v ) const;

      /**
	 @return the geometric vector associated to this edge.
      */
      Vector2i toVector() const;

      /**
	 @param other any edge that is not a quadrant change.
	 
	 @return the determinant of this edge with the [other] edge
	 (to the right).

	 NB: influenced by the tilde of [other].
      */
      int det( const Edge & other ) const;

      /**
	 @param other any edge
	 
	 @return 'true' if this edge can be merged to the right with
	 the edge [other] (ie. it can be replaced by exactly one edge
	 such that it defines the same RPR).
      */
      bool isMergeable( const Edge & other ) const;

      /**
	 @param other any edge
	 @param d (returns) the determinant of this edge with the edge [other]

	 @return 'true' if this edge can be merged to the right with
	 the edge [other] (ie. it can be replaced by exactly one edge
	 such that it defines the same RPR).
      */
      bool isMergeable( const Edge & other, int & d ) const;

      /**
	 @param edge (returns) the edge that can replaced the two
	 successive edges: 'this' and [other].

	 @param other any edge that is mergeable with 'this'.
      */
      void getMerge( Edge & edge, const Edge & other ) const;

      /** Return the alphabet before this edge. */
      void getPreAlphabet( unsigned int & a, unsigned int & b ) const;
      /** Return the alphabet after this edge. */
      void getPostAlphabet( unsigned int & a, unsigned int & b ) const;
      /** Return the pointel before this edge. */
      Vector2i prePointel() const;
      /** Return the pointel after this edge. */
      Vector2i postPointel() const;

      /**
	 The current edge should be primitive and not trivial and
	 should not be a quadrant edge.

	 @param edge_list (returns) the list of edges corresponding to
	 a split down.
      */
      void splitDown( std::list<Edge> & edge_list ) const;

      /**
	 The current edge should be primitive and not trivial and
	 should not be a quadrant edge.

	 @param edge_list (returns) the list of edges corresponding to
	 a split up.
      */
      void splitUp( std::list<Edge> & edge_list ) const;

      /**
	 The current edge should not be a quadrant edge.

	 @param edge_list (returns) the list of edges corresponding to
	 a split left.
      */
      void splitLeft( std::list<Edge> & edge_list ) const;

      /**
	 The current edge should not be a quadrant edge.

	 @param edge_list (returns) the list of edges corresponding to
	 a split right.
      */
      void splitRight( std::list<Edge> & edge_list ) const;

      /**
	 @return the number of grid steps associated with this
	 edge. It is in fact this.n * |this.v|_1.
       */
      unsigned int digitalLength() const;

      /**
	 @return the Euclidean length of this edge.
       */
      double euclideanLength() const;
      void selfDisplay( std::ostream & out ) const;
    };

    /**
       The type representing the sequence of edges.
     */
    typedef UndoableList< GridCurve::Edge > EdgeList;

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
    typedef EdgeList::quasi_iterator EdgeListIterator;

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
    typedef EdgeList::const_iterator EdgeListConstIterator;

    /**
       Allows to visit step by step the interpixel contour which
       corresponds to the GridCurve. The value of the visitor is a
       pair <Vector2i, unsigned char> representing the origin pointel
       coordinates and the stepcode (0-3).
       
       The visitor is an iterator and at some point is equal to
       'end()'. It does loop around the GridCurve when it is
       closed. In this case, 'end()' is never reached.
     */
    struct Visitor {
      typedef std::pair<Vector2i,unsigned char> Value;

      GridCurve* myGC;
      EdgeListIterator myEdgeIt;
      std::vector<unsigned char> mySteps;
      /// 0 is the first step of the edge.
      unsigned int myIdx;
      Vector2i pointel;

      Visitor( GridCurve & gc, 
	       const EdgeListIterator & edge_it,
	       unsigned int idx = 0 )
	: myGC( &gc )
      {
	setEdge( edge_it, idx );
      }
      
      Visitor( const Visitor & other )
	: myGC( other.myGC ), myEdgeIt( other.myEdgeIt ),
	  mySteps( other.mySteps ), myIdx( other.myIdx ),
	  pointel( other.pointel )
      {}

      Visitor& operator=( const Visitor & other )
      {
	if ( this != &other )
	  {
	    myGC = other.myGC;
	    myEdgeIt = other.myEdgeIt;
	    mySteps = other.mySteps;
	    myIdx = other.myIdx;
	    pointel = other.pointel;
	  }
	return *this;
      }

      /**
	 @return the pixel bordering the current visitor (an oriented
	 linel) and which is inside the shape.
      */
      Vector2i insidePixel() const
      {
        unsigned int a;
        unsigned int b;
        myEdgeIt->getPostAlphabet( a, b );
        Vector2i da = FreemanChain::displacement( a );
        Vector2i db = FreemanChain::displacement( b );
        Vector2i pixel( pointel );
        unsigned int code = mySteps[ myIdx ];
        bool insideEdge = myEdgeIt->tilde ? ( ! myEdgeIt->inside ) : ( myEdgeIt->inside) ;
        Vector2i delta(-1, -1);
        if ( insideEdge ) {
          delta += (code == a) ? da - db : 
                   (code == b) ? da + db :
                   db - da; // case -a
        } else {
          delta += (code == a) ? da + db :
                   (code == b) ? db - da :
                   -db - da; // case -a
        }
        assert( ( ( delta.x() % 2 ) == 0 )
            && ( ( delta.y() % 2 ) == 0 ) );
        delta /= 2;
        return pixel + delta;
      }

      /**
	 @return the pixel bordering the current visitor (an oriented
	 linel) and which is outside the shape.
      */
      Vector2i outsidePixel() const
      {
        unsigned int a;
        unsigned int b;
        myEdgeIt->getPostAlphabet( a, b );
        Vector2i da = FreemanChain::displacement( a );
        Vector2i db = FreemanChain::displacement( b );
        Vector2i pixel( pointel );
        unsigned int code = mySteps[ myIdx ];
        bool insideEdge = myEdgeIt->tilde ? ( ! myEdgeIt->inside ) : ( myEdgeIt->inside) ;
        Vector2i delta(-1, -1);
        if ( insideEdge ) {
          delta += (code == a) ? da + db : 
                   (code == b) ? db - da :
                   -db - da; // case -a
        } else {
          delta += (code == a) ? da - db : 
                   (code == b) ? da + db :
                   db - da; // case -a
        }
        assert( ( ( delta.x() % 2 ) == 0 )
            && ( ( delta.y() % 2 ) == 0 ) );
        delta /= 2;
        return pixel + delta;
      }


      EdgeListIterator edgeIterator() const
      {
	return myEdgeIt;
      }
      
      unsigned int position() const
      {
	return myIdx;
      }

      void setEdge( const EdgeListIterator & edge_it,
		    unsigned int idx = 0 )
      {
	myEdgeIt = edge_it;
	mySteps.clear();
	if ( myEdgeIt != myGC->endEdge() )
	  {
	    myEdgeIt->pushBackFreemanCode( mySteps );
	    pointel = myEdgeIt->prePointel();
	    if ( idx >= mySteps.size() ) idx = mySteps.size() - 1;
	    myIdx = 0;
	    for ( ; idx != 0; --idx )
	      next();
	  }
	else myIdx = 0;
      }

      void setAtEndOfEdge( const EdgeListIterator & edge_it )
      {
	myEdgeIt = edge_it;
	mySteps.clear();
	if ( myEdgeIt != myGC->endEdge() )
	  {
	    myEdgeIt->pushBackFreemanCode( mySteps );
	    pointel = myEdgeIt->postPointel();
	    myIdx = mySteps.size();
	    unsigned char step = mySteps[ --myIdx ];
	    pointel -= FreemanChain::displacement( step ); 
	  }
	else myIdx = 0;
      }
      
      void next()
      {
	if ( ( myEdgeIt == myGC->endEdge() )          // invalid edge
	     || ( myIdx == mySteps.size() - 1 ) ) // or last step in edge.
	  { // move to next edge
	    // setEdge( ++myEdgeIt );
	    setEdge( myGC->nextEdge( myEdgeIt ) );
	  }
	else
	  {
	    unsigned char step = mySteps[ myIdx++ ];
	    pointel += FreemanChain::displacement( step ); 
	  }
      }

      void previous()
      {
	if ( ( myEdgeIt == myGC->endEdge() )  // invalid edge
	     || ( myIdx == 0 ) )        // or first step in edge.
	  { // move to previous edge.
	    // setAtEndOfEdge( --myEdgeIt );
	    setAtEndOfEdge( myGC->previousEdge( myEdgeIt ) );
	  }
	else
	  {
	    unsigned char step = mySteps[ --myIdx ];
	    pointel -= FreemanChain::displacement( step ); 
	  }
      }

      Value operator*() const
      {
	return std::make_pair( pointel, mySteps[ myIdx ] );
      }

      // pre-increment ++i
      Visitor& operator++()
      {
	next();
	return *this;
      }

      // post-increment i++ (more costly).
      Visitor operator++( int )
      {
	Visitor v = *this;
	next();
	return v;
      }

      // pre-decrement --i
      Visitor& operator--()
      {
	previous();
	return *this;
      }

      // post-decrement i-- (more costly).
      Visitor operator--( int )
      {
	Visitor v = *this;
	previous();
	return v;
      }
      
      bool operator==( const Visitor & other ) const
      {
	// assert same GridCurve
	return ( myEdgeIt == other.myEdgeIt )
	  && ( myIdx == other.myIdx );
      }

      bool operator!=( const Visitor & other ) const
      {
	// assert same GridCurve
	return ( myEdgeIt != other.myEdgeIt )
	  || ( myIdx != other.myIdx );
      }

    };

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~GridCurve();

    /**
     * Constructor.
     */
    GridCurve();

    /**
     * Build a GridCurve from a freeman chain.
     */
    GridCurve( const FreemanChain& achain, const std::string& contour_type )
      : myCurve(), myTwiceDV( 0, 0 ), 
	myCW( true ), myLength( 0.0 ), myUpdateNb( 0 )
	
    {
      int nb_ccw_loops = achain.isClosed();
      bool cw = nb_ccw_loops < 0;
      
      if ( contour_type == "INNER" )
	initFromFreemanChain( achain.begin(), achain.end(), true, cw );
      else if ( contour_type == "OUTER" )
	initFromFreemanChain( achain.begin(), achain.end(), false, cw );
      else if ( contour_type == "INTERPIXEL" )
	{
          initFromInterpixelFreemanChain ( achain );
	  //// ReversiblePolygonalRepresentation rpr;			
	  //// // CurveCode cc;
	  //// // cc.init( Vector2i( achain.x0, achain.y0 ), cw, achain.chain );
	  //// rpr.cw = cw;
	  //// std::vector<uint> vi;
	  //// bool closed =
	  ////   FreemanChain::computeMLP( rpr.vx, rpr.vy, vi, rpr.vt,
	  ////       		      rpr.twice_dv, achain, cw )
	  ////   != achain.end();
	  //// if ( closed )
	  ////   {
	  ////     rpr.vx.push_back( rpr.vx[ 0 ] );
	  ////     rpr.vy.push_back( rpr.vy[ 0 ] );
	  ////     rpr.vt.push_back( rpr.vt[ 0 ] );
	  ////   }
	  //// initFromReversiblePolygonalRepresentation( rpr );
	}
    }

    /*
     *  Return the freemanchain described by the GridCurve.
     */
    FreemanChain getFreemanChain()
    {
      std::string code;
      Visitor itm = begin();
      do
	{
	  code += '0'+(*itm).second;
	  ++itm;
	}
      while (itm!=begin());
      return FreemanChain(code, (*itm).first.x(), (*itm).first.y());
    }
    
    /** 
	Initializes the grid curve from an arbitrary RPR. 

	@param rpr any RPR (open or closed). The RPR is assumed closed
	if the first and last vertices coincide and if the first and
	last types coincide.
    */
    void initFromReversiblePolygonalRepresentation
    ( const ReversiblePolygonalRepresentation & rpr );


    /**
     * Initializes the gridcurve directly from the chain code using a variant of
     * CMLP
     *
     * @param fc the interpixel chain code.
     */
    void initFromInterpixelFreemanChain 
      ( const FreemanChain & fc );

    /**
       @return an iterator that points on the first edge of the grid curve.
    */
    EdgeListIterator beginEdge();

    /**
       @return an iterator that points after the last edge of the grid curve.
    */
    EdgeListIterator endEdge();

    /**
       Allows edge iterators to act as circulators.

       @param it any edge iterator.

       @return an iterator on the edge following it in this grid
       curve. If the grid curve is closed and it was pointing on the
       last edge, it returns an iterator pointing on the first edge.
       If it was endEdge(), returns beginEdge().
    */
    EdgeListIterator nextEdge( const EdgeListIterator & it );

    /**
       Allows edge iterators to act as circulators.

       @param it any edge iterator.

       @return an iterator on the edge precdeing it in this grid
       curve. If the grid curve is closed and it was pointing on the
       first edge, it returns an iterator pointing on the last edge.
       If it was endEdge(), returns the last edge.
    */
    EdgeListIterator previousEdge( const EdgeListIterator & it );

    /**
       Move [it] to next edge (as circulator if closed) and move iterator [max]
       with it if equal.
    */
    void moveToNextEdgeAndPossiblyMax( EdgeListIterator & it, 
				       EdgeListIterator & max );
    /**
       Move [it] to previous edge (as circulator if closed) and move
       iterator [min] with it if equal.
    */
    void moveToPreviousEdgeAndPossiblyMin( EdgeListIterator & it, 
					   EdgeListIterator & min );


    /**
     * This method initializes consistently a dynamic contour from a
     * Freeman chain. The chain is supposed to model either the inner
     * 4-connected 8-border of an object ([inside] is 'true') or its
     * outer 4-connected 8-border ([inside] is 'false'). If you wish to
     * initialize the dynamic contour with a digital interpixel contour,
     * you should use the method FreemanChain::innerContour beforehands.
     *
     * The iterators should have a 'int getCode()' method returning
     * between 0-3 according to the Freeman chain code and a
     * post-increment operator++() method to visit the whole Freeman
     * chain code. The iterators should also have a method 'Vector2i
     * get()' returning the coordinates of the current pointel.
     *
     * @param itb an iterator on the beginning of Freeman chain.
     * @param ite an iterator on the end of Freeman chain.
     * @param inside indicates if it is an inner or an outer border.
     * @param cw indicates whether or not we use the clockwise orientation (otherwise ccw).
     *
     * The method can handle open or closed freeman chain codes. The
     * chaincode is considered closed whenever coordinates coincide at
     * the end. Note that chaincodes containing "02" or "13" are not valid.
     *
     * @deprecated some problems since freeman chains of inner or
     * outer borders are sometimes ambiguous.
     */
    template <class FreemanChainIterator>
    void initFromFreemanChain( FreemanChainIterator itb, 
			       FreemanChainIterator ite, 
			       bool inside,
			       bool cw );

    // template <class FreemanChainIterator>
    // void initFromInterpixelChain( FreemanChainIterator itb, 
    // 				  FreemanChainIterator ite,
    // 				  bool cw );

    // -------------------- split and merge services ------------------------

    /**
       This method tries to merge the edge at [it] with the edge after
       [it]. The merge is done only If the two edges are directly
       mergeable.

       @param it an edge iterator, which is set to the new edge if the
       merge was done.

       @return 'true' if the two edges were merged, 'false' otherwise.
    */
    bool merge( EdgeListIterator & it );

    /**
       Splits down the edge pointed by [it]. The edge should be
       primitive and not trivial and should not be a quadrant edge.

       @param it (modified) as input, an edge iterator pointing on the
       edge to split, as output an edge iterator pointing on the first
       edge before the splitted part.

       @param it_after_last (returns) an edge iterator pointing on the edge
       after the last of the splitted part.
    */
    void splitDown( EdgeListIterator & it, 
		    EdgeListIterator & it_after_last );

    /**
       Splits up the edge pointed by [it]. The edge should be
       primitive and not trivial and should not be a quadrant edge.

       @param it (modified) as input, an edge iterator pointing on the
       edge to split, as output an edge iterator pointing on the first
       edge before the splitted part.

       @param it_after_last (returns) an edge iterator pointing on the edge
       after the last of the splitted part.
    */
    void splitUp( EdgeListIterator & it, 
		  EdgeListIterator & it_after_last );

    /**
       Splits left the edge pointed by [it]. The edge should not be a
       quadrant edge.

       @param it (modified) as input, an edge iterator pointing on the
       edge to split, as output an edge iterator pointing on the first
       edge before the splitted part.

       @param it_after_last (returns) an edge iterator pointing on the edge
       after the last of the splitted part.
    */
    void splitLeft( EdgeListIterator & it, 
		    EdgeListIterator & it_after_last );

    /**
       Splits right the edge pointed by [it]. The edge should not be a
       quadrant edge.

       @param it (modified) as input, an edge iterator pointing on the
       edge to split, as output an edge iterator pointing on the first
       edge before the splitted part.

       @param it_after_last (returns) an edge iterator pointing on the edge
       after the last of the splitted part.
    */
    void splitRight( EdgeListIterator & it, 
		     EdgeListIterator & it_after_last );

    /**
       Splits the edge [it_edge] exactly at the position [position]. More
       precisely, there is a vertex of the grid curve at the specified
       position afterward. NB: if the edge visitor is ona quadrant edge,
       nothing is done.
       
       @param it_edge (modified) as input, an edge iterator on a "true"
       edge (not a quadrant change), as output an iterator pointing on the
       edge which starts at this vertex.
       
       @param position (modified) the position in number of steps
       where to split in the edge (0 is the start of the edge). Should
       not be equal or greater than the number of steps of this
       edge. May be modified so as to be in the correct position on
       the created subedge.
       
       @param inside when 'true', splits are done so as to be closer
       to the inside (below the edge); when 'false', splits are done
       so as to be closer to the outside (above the edge ). This is
       done with calls to splitDown and splitUp depending on the
       inside property of the edge.
    */
    void splitEdgeAt( EdgeListIterator & it_edge, 
		      unsigned int & position,
		      bool inside );

    /**
       Tries to find the shortest grid curve around the end vertex of
       edge [it]. The iterators [it_before_first] and [it_after_last]
       specifies the modified part of the grid curve. More precisely
       they points at the two edges surrounding the modified part, but
       not modified themselves.
       
       @param it an iterator pointing on an arbitrary edge (standard
       edge or quadrant change), whose end vertex is the location of
       the simplification. (may be invalid afterwards).

       @param it_before_first (returns) the first edge before the
       modified zone of the grid curve.

       @param it_after_last (returns) the first edge after the
       modified zone of the grid curve.

       @return 'true' if some modification took place, 'false' otherwise.
    */
    bool preSimplifyAt( EdgeListIterator & it,
			EdgeListIterator & it_before_first, 
			EdgeListIterator & it_after_last,
			EdgeListIterator & min,
			EdgeListIterator & max );

    /**
       @param it an iterator pointing on an arbitrary edge (standard
       edge or quadrant change), whose end vertex is the location of
       the simplification.  (may be invalid afterwards).

       @param it_before_first (returns) the first edge before the
       modified zone of the grid curve.

       @param it_after_last (returns) the first edge after the
       modified zone of the grid curve.

       @return 'true' if some modification took place, 'false' otherwise.
    */
    bool preSimplifyNegativeQuadrantChangeAt
    ( EdgeListIterator & it,
      EdgeListIterator & it_before_first, 
      EdgeListIterator & it_after_last,
      EdgeListIterator & min,
      EdgeListIterator & max  );

    bool preSimplifyPositiveQuadrantChangeAt
    ( EdgeListIterator & it,
      EdgeListIterator & it_before_first, 
      EdgeListIterator & it_after_last,
      EdgeListIterator & min,
      EdgeListIterator & max  );

    bool preSimplifyTwoNegativeQuadrantChangesAt
    ( EdgeListIterator & it,
      EdgeListIterator & it_before_first, 
      EdgeListIterator & it_after_last,
      EdgeListIterator & min,
      EdgeListIterator & max );
    
    /**
       Tries to find the shortest grid curve around the end vertex of
       edge [it]. The iterators [it_before_first] and [it_after_last]
       specifies the modified part of the grid curve. More precisely
       they points at the two edges surrounding the modified part, but
       not modified themselves. Locally, this is the shortest path
       between the end of [it_before_first] to the beginning of
       [it_after_last].
    */
    bool simplifyZone( EdgeListIterator & min,
		       EdgeListIterator & max );

    /**
       Returns an Visitor that moves pointel by pointel along the
       MLP as it is tracing step by step the digital interpixel
       contour.
       @code
       for ( Visitor it = gc.begin(), it_end = gc.end();
             it != it_end;
             ++it )
	 { ...
         }
       @endcode

       @return an Visitor on the first pointel of the grid curve.
    */
    Visitor begin();

    /**
       Returns an Visitor that moves pointel by pointel along the
       MLP as it is tracing step by step the digital interpixel
       contour.
       @code
       for ( Visitor it = gc.begin(), it_end = gc.end();
             it != it_end;
             ++it )
	 { ...
         }
       @endcode

       @return an Visitor on the "pointel" after the last pointel of
       the grid curve.
    */
    Visitor end();

    // ----------------------- edge services ---------------------------------
  public:

    /**
     * Inserts the edges from [itbegin] to [itend] before the element
     * pointed by [it]. 
     *
     * @param it an iterator in the contour.
     * @param n a edge to insert (which is copied).
     * @return an iterator pointing on the first created edge within the contour.
     */
    template <typename EdgeIterator>
    EdgeListIterator insertEdges( const EdgeListIterator & it,
				  EdgeIterator it_begin,
				  EdgeIterator it_end );
				 

    /**
     * Inserts the edge [n] before the element pointed by [it]. 
     *
     * @param it an iterator in the contour.
     * @param n a edge to insert (which is copied).
     * @return an iterator pointing on the created edge within the contour.
     */
    EdgeListIterator insertEdge( const EdgeListIterator & it, const Edge & n );

    /**
     * The edge pointed by [it] is assigned the edge [n]. 
     *
     * @param it (modifies) an iterator in the contour, returns the
     * iterator on the modified edge (could be different from [it] in
     * storing mode).
     *
     * @param n the edge to copy.
     */
    void modifyEdge( EdgeListIterator & it, const Edge & n );

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
    EdgeListIterator removeEdge( const EdgeListIterator & it );


   // ----------------------- deformable model services -----------------------
  public:

    // /**
    //    Creates a dynamic MLP for the given interpixel contour
    //    specified as a Freeman chaincode of '0', '1', '2', '3'
    //    characters. Calling 'begin' will return a Visitor in this DMLP
    //    pointing exactly at the base of the first coded linel in [s]
    //    (i.e., corresponds to the base of the first character).
       
    //    @param s a Freeman chaincode.
       
    //    @see begin, end
    //  */
    // void initFromInterpixelContour( const FreemanChain & s );

    /**
     * Given the coordinates of a pointel (the base of a linel),
     * returns an Visitor on the corresponding point of the DMLP.
     *
     * @param x the x-coordinate of the pointel (in the original frame
     * as given in input).
     *
     * @param y the y-coordinate of the pointel (in the original frame
     * as given in input).
     *
     * @param start an iterator where to start the research. Choose
     * begin() if you do not know where to start the search.
     *
     * @return the Visitor on the corresponding point of the DMLP. If
     * it was found, it has the correct coordinates.  If not, the
     * Visitor is positionned on the end edge.
     */
    Visitor findPointel( int x, int y, const Visitor & start );
    Visitor findPointel( int x, int y )
		{ return findPointel(x,y,begin()); }

    /**
     * Given the coordinates of a pointel (the base of a linel),
     * returns an Visitor on the corresponding point of the DMLP.
     *
     * @param x the x-coordinate of the pointel (in the original frame
     * as given in input).
     *
     * @param y the y-coordinate of the pointel (in the original frame
     * as given in input).
     *
     * @param step the step at this pointel (in 0-3).
     *
     * @param start an iterator where to start the research. Choose
     * begin() if you do not know where to start the search.
     *
     * @return the Visitor on the corresponding point of the DMLP. If
     * it was found, it has the correct coordinates.  If not, the
     * Visitor is positionned on the end edge.
     */
    Visitor findPointel( int x, int y, unsigned char step,
			 const Visitor & start );

    /**
     * @return the length (i.e. perimeter) of the current MLP.
     */
    double getLength() const
		{ return myLength; }
			

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
     Flips the dynamic MLP at position specified by [it] and in the
     direction specified by 'it.inside'.
     
     @param it specifies the position where to flip the MLP.

     @param inside when 'true', flips the inside pixel associated to
     the visitor [it] (i.e., removes a pixel from the shape for a
     closed grid curve), when 'false', flips the outside pixel
     associated to the visitor [it].
     
     @param begin_mod (modified) Visitor pointing on the beginning of
     the modified part of the MLP.

     @param end_mod (modified) Visitor pointing after the end of the
     modified part of the MLP.

     @param store_undo when 'true', stores the reverse flip
     operation that will undo this change. This parameter may be
     false only if the stack 'm_undo' is empty.
     
     @see undoFlip
     @see topUndo
     @see m_undo
     */
    void flip( const Visitor & it, bool inside,
	       Visitor & begin_mod, Visitor & end_mod,
	       bool store_undo = false );

    /** 
	Flips the dynamic MLP at position specified by [it] and in the
	direction specified by 'it.inside', and simplify
	
	@see flip
	@see simplifyZone
    */
    void flipAndSimplify(const Visitor & it, bool inside,
			 bool store_undo = false )
    {
      // std::cout<<"flipAndSimplify("<<(*it).first<<") pour";
      // this->selfDisplay(std::cout); std::cout<<std::endl;
      
      Visitor vbegin=begin();
      Visitor vend=begin();
      flip(it,inside,vbegin,vend, store_undo);
      EdgeListIterator begin_mod =  vbegin.edgeIterator();
      EdgeListIterator end_mod = vend.edgeIterator();
      bool modif = true;
      while ( modif )
	{
	  modif=simplifyZone( begin_mod, end_mod );
	}
    }
		
    /**
     * Undoes the last flip operation. Can be used only if
     * 'store_undo' was set during the last flip operation.
     */
    void undoFlip();

    /**
     * @return 'true' if the stack of stored inverse flips is not empty.
     */
    bool isUndoPossible() const;

    /**
     * Clears the stack of stored inverse flips.
     */
    void clearUndo();

    /**
     * The digital contour lies in Z x Z while the MLP lies in the
     * half-integer plane (Z+1/2) x (Z+1/2). This vector gives twice
     * the displacement to go from the former to the latter.
     * @return this vector.
     */
    const Vector2i twiceDV() const;

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

    /**
     * Writes/Displays the object on an output stream.
     * @param that_stream the output stream where the object is written.
     */
    void verboseSelfDisplay( std::ostream & that_stream ) const;

    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * The dynamic contour is essentially a list of edges.
     */
    EdgeList myCurve;

    /**
     * The digital contour lies in Z x Z while the MLP lies in the
     * half-integer plane (Z+1/2) x (Z+1/2). This vector gives twice
     * the displacement to go from the former to the latter.
     */
    Vector2i myTwiceDV;

    /**
     * 'true' if contour turns around inside clockwise (INSIDE to
     * RIGHT), 'false' otherwise (INSIDE TO LEFT).
     */
    bool myCW;

    /**
     * the length (i.e. perimeter) of the current MLP.
     */
    double myLength;
    std::stack<double> oldLengths; // Stack of lengths, used for undo.
		
    /**
     * Counts the number of incremental updates of the DMLP.
     */
    unsigned int myUpdateNb;

    /**
       Tells if the GridCurve is closed (ie. GridContour) or open.
    */
    bool myIsClosed;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
       Put a tilde just before position [it] if possible (not a
       quadrant change) or move it backward until it finds a proper
       position. Then comes back to put a tilde afterwards.
       
       @param it (modified) any iterator on an edge, outputs an
       iterator before the first modified edge.
       
       @param it_after_last (returns) outputs an iterator after the last
       modified edge.
    */
    void swapTilde( EdgeListIterator & it,
		    EdgeListIterator & it_before_first,
		    EdgeListIterator & it_after_last );
    
    /**
       Handles the special case of flipping [~oo^1].

       @param it (modified) any iterator on that kind of edge.

       @param position either 0, 1 or 2
       
       @param inside when 'true', flips the inside pixel associated to
       the visitor [it] (i.e., removes a pixel from the shape for a
       closed grid curve), when 'false', flips the outside pixel
       associated to the visitor [it].
    */
    void flipPrimitiveTildeInfinite( EdgeListIterator & it_edge, 
				     unsigned int position, bool inside,
				     EdgeListIterator & it_before_first, 
				     EdgeListIterator & it_after_last);

    /**
       Inserts a positive bump ([oo][+][0][-]) or a negative bump
       ([-][oo][+][0]), depending on the boolean [primitive] just
       before the edge pointed by [it_edge].
       
       @param it_edge an iterator pointing on a valid edge.
       
       @param positive a boolean that specifies the type of inserted
       bumps.
      */
    void insertBump( EdgeListIterator & it_edge, bool positive );


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE GridCurve( const GridCurve & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE GridCurve & operator=( const GridCurve & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'Visitor'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Visitor' to write.
   * @return the output stream after the writing.
   */
  inline
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const GridCurve::Visitor & avisitor )
  {
    that_stream<<"Visitor("<<avisitor.pointel
               <<" dir="
               <<(char)('0'+avisitor.mySteps[avisitor.myIdx])
               <<")";
    return that_stream;
  }

  /**
   * Overloads 'operator<<' for displaying objects of class 'GridCurve'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'GridCurve' to write.
   * @return the output stream after the writing.
   */
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const GridCurve & that_object_to_display );

  /**
   * Overloads 'operator<<' for displaying objects of class 'GridCurve::Edge'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'GridCurve::Edge' to write.
   * @return the output stream after the writing.
   */
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const GridCurve::Edge & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/GridCurve.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined GridCurve_h

#undef GridCurve_RECURSES
#endif // else defined(GridCurve_RECURSES)
