/** @file CubicalCell.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : CubicalCell.h
//
// Creation : 2008/03/19
//
// Version : 2008/03/19
//
// Author : JOL
//
// Summary : Header file for files CubicalCell.ih and CubicalCell.cxx
//
// History :
//	2008/03/19 : ?Name? : ?What?
//
// Rcs Id : "@(#)class CubicalCell declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(CubicalCell_RECURSES)
#error Recursive header files inclusion detected in CubicalCell.h
#else // defined(CubicalCell_RECURSES)
#define CubicalCell_RECURSES

#if !defined CubicalCell_h
#define CubicalCell_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
#include <deque>
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /**
   * Coordinates of cubical cells are represented with this type.
   */
  typedef uint64 ccell_coord_t;

  /////////////////////////////////////////////////////////////////////////////
  // class CubicalCell
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CubicalCell' <p>
   * Aim: Represents an arbitrary cell in a cubical complex.
   * The cell does not know its dimension.
   */
  class CubicalCell
  {

  public:
    
    /**
     * default dimension of the space.
     */
    static uint default_dim;  

    /**
     * Test method for the class.
     */
    static void testCubicalCell();

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Creates an invalid cell.
     */
    INLINE CubicalCell();

    /**
     * Desallocates memory if necessary.
     */
    INLINE ~CubicalCell();

    /**
     * Creates a cell in one-dimensional space. 
     *
     * @param x the Khalimsky coordinate of the cell along the first axis.
     */
    INLINE explicit CubicalCell( uint32 x );

    /**
     * Creates a cell in one-dimensional space. 
     *
     * @param x the Khalimsky coordinate of the cell along the first axis.
     */
    INLINE explicit CubicalCell( uint64 x );

    /**
     * Creates a cell in two-dimensional space. 
     *
     * @param x the Khalimsky coordinate of the cell along the first axis.
     * @param y the Khalimsky coordinate of the cell along the second axis.
     */
    template <typename CoordType>
    INLINE
    CubicalCell( CoordType x, CoordType y );

    /**
     * Creates a cell in three-dimensional space. 
     *
     * @param x the Khalimsky coordinate of the cell along the first axis.
     * @param y the Khalimsky coordinate of the cell along the second axis.
     * @param z the Khalimsky coordinate of the cell along the third axis.
     */
    template <typename CoordType>
    INLINE
    CubicalCell( CoordType x, CoordType y, CoordType z );

    /**
     * "Copy" constructor.
     *
     * @param other the cell to clone.
     * @param dim the dimension of the space.
     */
    INLINE CubicalCell( const CubicalCell & other, uint dim = default_dim );

    /**
     * Initializes the cell with the given cell.
     * Allocates memory if necessary.
     *
     * @param other the cell to clone.
     * @param dim the dimension of the space.
     *
     * @pre if the cell has already been initialized, then the updated
     * cell must have the same dimension as before.
     */
    INLINE
    void init( const CubicalCell & other, uint dim = default_dim ); 

    /**
     * Initializes the cell with the given coordinates.
     * Allocates memory if necessary.
     *
     * @param coords the Khalimsky coordinates of the cell.
     * @param dim the dimension of the space.
     *
     * @pre if the cell has already been initialized, then the updated
     * cell must have the same dimension as before.
     */
    template <typename CoordType>
    INLINE
    void init( CoordType* coords, uint dim = default_dim ); 

    /**
     * Assign the cell with the given coordinates.
     * The cell must have been initialized before.
     *
     * @param coords the Khalimsky coordinates of the cell.
     * @param dim the dimension of the space.
     *
     * @pre the updated cell must have the same dimension as before.
     */
    template <typename CoordType>
    INLINE
    void assign( CoordType* coords, uint dim = default_dim ); 

    /**
     * Assign the cell with the given Khalimsky coordinates.
     * The cell must have been initialized before.
     *
     * @param other the cell to clone.
     * @param dim the dimension of the space.
     *
     * @pre the updated cell must have the same dimension as before.
     */
    INLINE
    void assign( const CubicalCell & other, uint dim = default_dim ); 


    // ----------------------- Cell services ------------------------------
  public:

    /**
     * @param dim the dimension of the space.
     * @return the dimension of this cell.
     */
    INLINE uint dim( uint dim = default_dim ) const;

    /**
     * Read/write accessor.
     *
     * @param i the coordinate axis.
     * @return the Khalimsky coordinate of the cell along axis [i].
     */
    INLINE ccell_coord_t& operator[]( uint i );

    /**
     * Read-only accessor.
     *
     * @param i the coordinate axis.
     * @return the Khalimsky coordinate of the cell along axis [i].
     */
    INLINE const ccell_coord_t& operator[]( uint i ) const;

    /**
     * The cell is transformed as its incident cell along axis [i]
     * toward the increasing coordinates if 'pos' is true, otherwise
     * toward its decreasing coordinates.
     *
     * @param i the axis along which the incident cell is computed.
     * @param pos the orientation along axis [i] (positive when true,
     * negative otherwise).
     */
    INLINE void incident( uint i, bool pos );
    
    /**
     * The cell is transformed as its adjacent cell along axis [i]
     * toward the increasing coordinates if 'pos' is true, otherwise
     * toward its decreasing coordinates.
     *
     * @param i the axis along which the adjacent cell is computed.
     * @param pos the orientation along axis [i] (positive when true,
     * negative otherwise).
     */
    INLINE void adjacent( uint i, bool pos );

    /**
     * Computes the cells that are low incident to 'this' and with a
     * dimension just inferior.
     *
     * @param q the cells are added at the back of this queue.
     * @param dim the dimension of the space.
     */
    template < typename Queue >
    INLINE
    void lowIncidentCells1( Queue & q, 
			    uint dim = default_dim ) const;

    /**
     * Computes the strict lower boundary of the cell [c].
     * Nb: all cells added to [q] are two-by-two distinct.
     *
     * @param q the cells are added at the back of this queue.
     * @param c the cell to subdivide
     * @param dim_c the dimension of the cell [c] 
     * @param dim the dimension of the space.
     */
    template < typename Queue >
    INLINE
    static void strictLowerBoundary( Queue & q,
				     const CubicalCell & c,
				     uint dim_c,
				     uint dim );


    /**
     * Computes the cells that are low incident to 'this' and with the
     * specified dimension.
     * Nb: all cells added to [q] are two-by-two distinct.
     *
     * @param q the cells are added at the back of this queue.
     * @param c the cell to subdivide
     * @param dim_c the dimension of the cell [c] 
     * @param min the minimum dimension of the cells to extract.
     * @param max the maximum dimension of the cells to extract.
     * @param dim the dimension of the space.
     */
    template < typename Queue >
    INLINE
    static void lowIncidentCells( Queue & q,
				  const CubicalCell & c,
				  uint dim_c,
				  uint min = 0,
				  uint max = default_dim,
				  uint dim = default_dim );

    /**
     * Computes the cells that are low incident to 'this' and with a
     * dimension just inferior.
     *
     * @param q the cells are added at the back of this queue.
     * @param dim the dimension of the cell.
     * @param cmin the coordinates of the lowest admissible cell.
     * @param cmax the coordinates of the highest admissible cell.
     */
    template <typename Queue>
    INLINE
    void lowIncidentCells1( Queue & q, 
			    uint dim,
			    const CubicalCell & cmin,
			    const CubicalCell & cmax ) const;

    /**
     * Computes the cells that are low incident to 'this' and with the
     * specified dimension.
     * Nb: all cells added to [q] are two-by-two distinct.
     *
     * @param q the cells are added at the back of this queue.
     * @param c the cell to subdivide
     * @param dim_c the dimension of the cell [c] 
     * @param min the minimum dimension of the cells to extract.
     * @param max the maximum dimension of the cells to extract.
     * @param dim the dimension of the space.
     * @param cmin the coordinates of the lowest admissible cell.
     * @param cmax the coordinates of the highest admissible cell.
     */
    template < typename Queue >
    INLINE
    static void lowIncidentCells( Queue & q,
				  const CubicalCell & c,
				  uint dim_c,
				  uint min, 
				  uint max,
				  uint dim,
				  const CubicalCell & cmin,
				  const CubicalCell & cmax );


    /**
     * Computes the cells that are up incident to 'this' and with a
     * dimension just superior.
     *
     * @param q the cells are added at the back of this queue.
     * @param dim the dimension of the space.
     */
    template < typename Queue >
    INLINE
    void upIncidentCells1( Queue & q, 
			   uint dim = default_dim ) const;
    

    /**
     * @param other any cell
     * @param dim the number of coordinates of the cells
     *
     * @return 'true' if the [other] cell is the same as 'this'.
     */
    INLINE bool equals( const CubicalCell & other, 
			uint dim = default_dim ) const;

    /**
     * Lexicographic ordering on cubical cells.
     *
     * @param c the other cell (of dimension [d]).
     * @param d the number of coordinates of the cells
     * @return 'true' if [c1] is before [c2] in the cell ordering.
     */
    INLINE bool less( const CubicalCell & c, uint d ) const;

    /**
     * Lexicographic ordering on cubical cells.
     *
     * @param c the other cell (of dimension [d]).
     * @param d the number of coordinates of the cells
     * @return 'true' if [c1] is after [c2] in the cell ordering.
     */
    INLINE bool more( const CubicalCell & c, uint d ) const;

    /**
     * Inverse lexicographic ordering on cubical cells.
     *
     * @param c the other cell (of dimension [d]).
     * @param d the dimension of the cells
     * @return 'true' if [c1] is before [c2] in the cell ordering.
     */
    INLINE bool invLess( const CubicalCell & c, uint d ) const;

    /**
     * Inverse lexicographic ordering on cubical cells.
     *
     * @param c the other cell (of dimension [d]).
     * @param d the dimension of the cells
     * @return 'true' if [c1] is after [c2] in the cell ordering.
     */
    INLINE bool invMore( const CubicalCell & c, uint d ) const;

    /**
     * Bounds are large (<= et >=).
     *
     * @param cmin the lowest cell in the parallepiped
     * @param cmin the most upper cell in the parallepiped
     * @param d the dimension of the space.
     *
     * @return 'true' if 'this' cell is within the specified
     * parallelepiped, 'false' otherwise.
     */
    INLINE bool isInBounds( const CubicalCell & cmin, 
			    const CubicalCell & cmax,
			    uint d ) const;


    /**
     * Adds [v] to all coordinates of this cell.
     *
     * @param v the value to add.
     * @param d the number of coordinates of the cells.
     */
    template <typename ValueType>
    INLINE void addToAll( ValueType v, uint d );

    /**
     * Subs [v] to all coordinates of this cell.
     *
     * @param v the value to add.
     * @param d the number of coordinates of the cells.
     */
    template <typename ValueType>
    INLINE void subToAll( ValueType v, uint d );

    /**
     * Adds [v] to coordinate [i] of this cell.
     *
     * @param v the value to add.
     * @param i a coordinate of the cell.
     */
    template <typename ValueType>
    INLINE void addToOne( ValueType v, uint i );

    /**
     * Subs [v] to coordinate [i] of this cell.
     *
     * @param v the value to add.
     * @param i a coordinate of the cell.
     */
    template <typename ValueType>
    INLINE void subToOne( ValueType v, uint i );

    // ----------------------- Hierarchical services --------------------------
  public:

    /**
     * @param d the dimension of the space.
     * @return 'true' if it is a boundary cell, meaning one coordinate
     * is an exact poxer of 2.
     */
    INLINE bool isBoundary( uint d ) const;

    /**
     * @param min (returns) the lowest point in the parallepiped
     * containing 'this'.
     *
     * @param max (returns) the most upper point in the parallepiped
     * containing 'this'.
     *
     * @param d the dimension of the space.
     */
    INLINE void bounds( CubicalCell & min, CubicalCell & max, uint d ) const;

    /**
     * The cell determines a subspace whose subdivision is given by
     * [k]. The lowest and most upper points are given for the k-th
     * subdivision of 'this'. [min] and [max] exclude boundaries and take
     * into account the topology of the cell.
     *
     * @param min (returns) the lowest point in the parallepiped
     * containing 'this'.
     *
     * @param max (returns) the most upper point in the parallepiped
     * containing 'this'.
     *
     * @param k the subdivision of the cell 'this'.
     *
     * @param d the dimension of the space.
     */
    INLINE void subspaceInnerBounds( CubicalCell & min, CubicalCell & max, 
				     uint k, uint d ) const;

    /**
     * The cell determines a subspace whose subdivision is given by
     * [k]. The lowest and most upper points are given for the k-th
     * subidivision of 'this'. [min] and [max] include boundaries and take
     * into account the topology of the cell.
     *
     * @param min (returns) the lowest point in the parallepiped
     * containing 'this'.
     *
     * @param max (returns) the most upper point in the parallepiped
     * containing 'this'.
     *
     * @param k the subdivision of the cell 'this'.
     *
     * @param d the dimension of the space.
     */
    INLINE void subspaceOuterBounds( CubicalCell & min, CubicalCell & max,
				     uint k, uint d ) const;

    /**
     * @param d the dimension of the space.
     *
     * @return 'true' is the cell is valid (meaning its coordinates
     * are greater than (2,2,...)).
     */
    INLINE bool isValid( uint d ) const;

    /**
     * Computes the cell that is just coarser than 'this' and that
     * includes 'this'.
     *
     * @param d the dimension of the space.
     */
    INLINE void toCoarse( uint d );

    /**
     * Computes the cell that is k-times coarser than 'this' and that
     * includes 'this'.
     *
     * @param k the number of coarser levels to ascend.
     * @param d the dimension of the space.
     */
    INLINE void toCoarse( uint k, uint d );

    /**
     * Determines the first refinement of 'this' as the cells
     * in-between [min] and [max] cells. [min] and [max] exclude
     * boundaries and take into account the topology of the cell. Same
     * as subspaceInnerBounds with k=1.
     *
     * @param min (returns) the lowest refined cell included in 'this'
     *
     * @param max (returns) the most upper refined cell included in 'this'
     *
     * @param d the dimension of the space.
     *
     * @see subspaceInnerBounds
     */
    INLINE void toFine( CubicalCell & min, CubicalCell & max, 
			uint d ) const;

    /**
     * Determines the [k]-th refinement of 'this' as the cells in-between
     * [min] and [max] cells. [min] and [max] exclude boundaries and take
     * into account the topology of the cell. Same as subspaceInnerBounds.
     *
     * @param min (returns) the lowest [k]-refined cell included in 'this'
     *
     * @param max (returns) the most upper [k]-refined cell included in 'this'
     *
     * @param k the subdivision of the cell 'this'.
     *
     * @param d the dimension of the space.
     *
     * @see subspaceInnerBounds
     */
    INLINE void toFine( CubicalCell & min, CubicalCell & max, 
			uint k, uint d ) const;

    
    /**
     * @param other any other cell.
     * @param d the dimension of the space.
     *
     * @return 'true' is 'this' cell is included in the cell other
     * (meaning having [other] as its coarser version).
     */
    INLINE bool isIncluded( const CubicalCell & other, uint d ) const;

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
     * Khalimsky coordinates of the cell.
     */
    ccell_coord_t* m_x;


    // ------------------------- Hidden services ------------------------------
  protected:


  private:


    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE CubicalCell & operator=( const CubicalCell & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };


  /////////////////////////////////////////////////////////////////////////////
  // class CubicalCellComparator
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CubicalCellComparator' <p>
   * Aim: A function object to compare cells (<, ==, >, etc)
   */
  class CubicalCellComparator
  {
  public:
    /**
     * Dimension of the space in which cells are compared.
     */
    uint dim;

    /**
     * Constructor.
     * @param d the dimension of the space in which cells are compared.
     */
    INLINE explicit CubicalCellComparator( uint d );

    /**
     * Copy constructor. 
     *
     * @param other the other cubical cell comparator.
     */
    INLINE CubicalCellComparator( const CubicalCellComparator & other );

    /**
     * Assignment. Checks that it is the same as 'this'.
     *
     * @param other the other cubical cell comparator.
     */
    INLINE CubicalCellComparator& 
    operator=( const CubicalCellComparator & other );

    /**
     * Assign an order to cubical cells.
     * @see CubicalCell::less
     *
     * @param c1 the first cell (of dimension [dim]).
     * @param c2 the second cell (of dimension [dim]).
     * @return 'true' if [c1] is before [c2] in the cell ordering.
     */
    INLINE bool operator()( const CubicalCell & c1, 
			    const CubicalCell & c2 ) const;


    /**
     * Assign an order to cubical cells iterators.
     * @see CubicalCell::less
     *
     * @param c1 an iterator on the first cell (of dimension [dim]).
     * @param c2 an iterator on the second cell (of dimension [dim]).
     * @return 'true' if [c1] is before [c2] in the cell ordering.
     */
    template <typename Tmap_iterator>
    INLINE
    bool operator() ( const Tmap_iterator & c1, 
		      const Tmap_iterator & c2 ) const;

  private:

    /**
     * Default constructor. (forbidden)
     */
    INLINE CubicalCellComparator();

  };


  /////////////////////////////////////////////////////////////////////////////
  // class CubicalCellInvComparator
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CubicalCellInvComparator' <p>
   * Aim: A function object to compare cells (<, ==, >, etc)
   */
  class CubicalCellInvComparator
  {
  public:
    /**
     * Dimension of the space in which cells are compared.
     */
    uint dim;

    /**
     * Constructor.
     * @param d the dimension of the space in which cells are compared.
     */
    INLINE explicit CubicalCellInvComparator( uint d );

    /**
     * Copy constructor. 
     *
     * @param other the other cubical cell comparator.
     */
    INLINE CubicalCellInvComparator( const CubicalCellInvComparator & other );

    /**
     * Assignment. Checks that it is the same as 'this'.
     *
     * @param other the other cubical cell comparator.
     */
    INLINE CubicalCellInvComparator& 
    operator=( const CubicalCellInvComparator & other );

    /**
     * Assign an order to cubical cells.
     * @see CubicalCell::less
     *
     * @param c1 the first cell (of dimension [dim]).
     * @param c2 the second cell (of dimension [dim]).
     * @return 'true' if [c1] is before [c2] in the cell ordering.
     */
    INLINE bool operator()( const CubicalCell & c1, 
			    const CubicalCell & c2 ) const;


    /**
     * Assign an order to cubical cells iterators.
     * @see CubicalCell::less
     *
     * @param c1 an iterator on the first cell (of dimension [dim]).
     * @param c2 an iterator on the second cell (of dimension [dim]).
     * @return 'true' if [c1] is before [c2] in the cell ordering.
     */
    template <typename Tmap_iterator>
    INLINE
    bool operator() ( const Tmap_iterator & c1, 
		      const Tmap_iterator & c2 ) const;

  private:

    /**
     * Default constructor. (forbidden)
     */
    INLINE CubicalCellInvComparator();

  };





  /////////////////////////////////////////////////////////////////////////////
  // class CubicalCellPriorityComputer
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CubicalCellPriorityComputer' <p> Aim:
   * Computes the priority of a given cubical cell. Should be tailored
   * to the usage.
   *
   * @see CubicalCellPriority
   */
  class CubicalCellPriorityComputer
  {
  public:
    
    /**
     * Destructor.
     */
    INLINE virtual ~CubicalCellPriorityComputer()
    {}

    /**
     * Constructor.
     */
    INLINE CubicalCellPriorityComputer()
    {}

    /**
     * Computes the priority of the cell [c], as specified by a
     * derived class. Default is 0.0 (no cell has a higher priority
     * than any other one).
     *
     * @param c some cell.
     * @param dim the dimension of the space in which cells are compared.
     */
    INLINE virtual float priority( const CubicalCell & c, uint dim ) const
    {
      return 0.0f;
    }
  };


  /////////////////////////////////////////////////////////////////////////////
  // class CubicalCellPriority
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CubicalCellPriority' <p> Aim: An abstract
   * class whose objective is to assign a priority to each cell, thus
   * giving an ordering on cells.
   */
  class CubicalCellPriority
  {
  public:
    typedef std::map< CubicalCell, float, CubicalCellComparator > ccp_map;
    typedef ccp_map::iterator iterator;
    typedef ccp_map::const_iterator const_iterator;


    /**
     * Dimension of the space in which cells are compared.
     */
    const uint dim;

    /**
     * Destructor.
     */
    INLINE virtual ~CubicalCellPriority();
    
    /**
     * Constructor
     *
     * @param computer the external object used to compute cell priority.
     * @param d the dimension of the space in which lies cells.
     * @param store_computation indicates whether priority computation
     * should be stored or recomputed each time.
     */
    INLINE CubicalCellPriority( CubicalCellPriorityComputer* computer,
				uint d = 1,
				bool store_computation = false );
    
    /**
     * Assign an order to cubical cells, based on priority.
     *
     * @param c1 the first cell (of dimension [dim]).
     * @param c2 the second cell (of dimension [dim]).
     * @return 'true' if [c1] is before [c2] in the cell ordering.
     */
    INLINE bool operator()( const CubicalCell & c1, 
			    const CubicalCell & c2 ) const;

    /**
     * Assign an order to cubical cells iterators, based on priority.
     * @see CubicalCell::less
     *
     * @param c1 an iterator on the first cell (of dimension [dim]).
     * @param c2 an iterator on the second cell (of dimension [dim]).
     * @return 'true' if [c1] is before [c2] in the cell ordering.
     */
    template <typename Tmap_iterator>
    INLINE
    bool operator() ( const Tmap_iterator & c1, 
		      const Tmap_iterator & c2 ) const;


  private:
  
    /**
     * A pointer on a (not-owned) object that computes on demand the
     * priority of a cubical cell.
     */
    CubicalCellPriorityComputer* m_computer;

    /**
     * When 'true', stores priority computations in the map
     * [m_priority], otherwise priorities are computed on demand.
     */
    bool m_stored;

    /**
     * Object used for ordering cells.
     */
    CubicalCellComparator m_ccc;

    /**
     * Stores priorities once they have been computed.
     */
    mutable ccp_map m_priority;

  };






  /**
   * Overloads 'operator<<' for displaying objects of class 'CubicalCell'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'CubicalCell' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const CubicalCell & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/CubicalCell.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CubicalCell_h

#undef CubicalCell_RECURSES
#endif // else defined(CubicalCell_RECURSES)
