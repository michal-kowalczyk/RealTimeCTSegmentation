/** @file CubicalComplex.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : CubicalComplex.h
//
// Creation : 2008/03/13
//
// Version : 2008/03/13
//
// Author : JOL
//
// Summary : Header file for files CubicalComplex.ih and CubicalComplex.cxx
//
// History :
//	2008/03/13 : ?Name? : ?What?
//
// Rcs Id : "@(#)class CubicalComplex declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(CubicalComplex_RECURSES)
#error Recursive header files inclusion detected in CubicalComplex.h
#else // defined(CubicalComplex_RECURSES)
#define CubicalComplex_RECURSES

#if !defined CubicalComplex_h
#define CubicalComplex_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
#include <deque>
#include <set>
#include <list>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/digitalnD/CubicalCell.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  

  /////////////////////////////////////////////////////////////////////////////
  // class CubicalCellData
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CubicalCellData' <p> Aim: This class holds
   * the data associated with each cell of a cubical complex.
   */
  class CubicalCellData
  {
  public:

    /**
     * Type chosen for the data.
     */
    typedef uint32 data_t;

    /**
     * Number of bits hold in this.
     * @see data_t
     */
    static const int NBBITS;

    /**
     * Number of bits reserved for flags.
     * @see data_t
     */
    static const int FLAGBITS;

    /**
     * Number of bits reserved for value.
     * Equal to NBBITS-FLAGBITS.
     * @see data_t
     */
    static const int VALBITS;

    /**
     * REMOVED flag.
     */
    static const uint REMOVED;

    /**
     * COLLAPSIBLE flag.
     */
    static const uint COLLAPSIBLE;

    /**
     * FIXED flag.
     */
    static const uint FIXED;

    /**
     * User Flag 1.
     */
    static const uint USER1;


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Constructor.
     */
    INLINE CubicalCellData();

    /**
     * Copy constructor.
     * 
     * @param other the data to clone.
     */
    INLINE CubicalCellData( const CubicalCellData & other );

    /**
     * Destructor.
     */
    INLINE ~CubicalCellData();

    /**
     * Assignment.
     *
     * @param other the data to clone.
     * @return a reference on this.
     */
    INLINE CubicalCellData & operator=( const CubicalCellData & other );

    // ----------------------- Cubical cell data services ---------------------
  public:

    /**
     * @param flag a valid flag for cubical cell.
     * @return 'true' if the data is marked.
     * @see REMOVED,COLLAPSABLE
     */
    INLINE bool isFlagged( uint flag ) const;

    /**
     * Sets the flag [flag].
     *
     * @param flag a valid flag for cubical cell.
     * @see REMOVED,COLLAPSABLE
     */
    INLINE void setFlag( uint flag );

    /**
     * Resets the flag [flag].
     *
     * @param flag a valid flag for cubical cell.
     * @see REMOVED,COLLAPSABLE
     */
    INLINE void resetFlag( uint flag );

    /**
     * Assign the value [v] to this data.
     *
     * @param v an integer number (may be slightly truncated).
     * @see VALBITS
     */
    INLINE void setValue( int v );

    /**
     * Access the value [v] stored in this data.
     *
     * @return an integer number (with VALBITS significant bits)
     * @see VALBITS
     */
    INLINE int getValue() const;

    // ----------------------- Data ------------------------------
  private:

    /**
     * data holder.
     */
    uint32 m_data;

    
  };
  



  /////////////////////////////////////////////////////////////////////////////
  // class CubicalComplex
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CubicalComplex' <p>
   * Aim: Represent a cubical complex, ie a complex made of cubical cells.
   */
  class CubicalComplex
  {

  public:

    /**
     * map for cubical cells.
     */
    typedef std::map< CubicalCell, CubicalCellData, 
		      CubicalCellInvComparator > cc_map;



    /**
     * Allows the visit of all cells of the complex. 
     */
    typedef cc_map::iterator iterator;

    /**
     * Allows the visit of all cells of the complex. 
     */
    typedef cc_map::const_iterator const_iterator;

    /**
     * The type of a set of cells lying in the cubical complex.
     */
    typedef std::set< iterator, CubicalCellInvComparator > cc_cell_set;


    /**
     * The type of a list of cells lying in the cubical complex.
     */
    typedef std::deque< iterator > cc_cell_list;


    // ----------------------- Static services ------------------------------
  public:

    /**
     * Collapse a subcomplex of the closed cubical complex [K],
     * collapsing cells following priority [priority], in a decreasing
     * sequence. [S] provides the starting cells, generally (but not
     * compulsory) maximal cells.  Note: cells that have been marked
     * as fixed are not removed.  only cells that are in the closure
     * of [S] may be removed, and only if they are not marked as
     * fixed.
     *
     * @param K the closed cubical complex to collapse.
     * @param S a subset of cells of [K], as iterators in [K]
     * @param S_closed indicates if [S] is a closed set (faster in this case)
     * @param priority the object that assign a priority to each cell.
     */
    static void collapse( CubicalComplex & K,
			  cc_cell_set & S,
			  bool S_closed,
			  const CubicalCellPriority & priority );

    /**
     * Collapse a subcomplex of the closed cubical complex [K],
     * collapsing cells following priority [priority], in a decreasing
     * sequence. [S] provides the starting cells, generally (but not
     * compulsory) maximal cells.  Note: cells that have been marked
     * as fixed are not removed.  only cells that are in the closure
     * of [S] may be removed, and only if they are not marked as
     * fixed.
     *
     * @param K the closed cubical complex to collapse.
     * @param S a subset of cells of [K], as iterators in [K]
     * @param S_closed indicates if [S] is a closed set (faster in this case)
     * @param priority the object that assign a priority to each cell.
     * @deprecated 
     */
    static void collapse2( CubicalComplex & K,
			   cc_cell_set & S,
			   bool S_closed,
			   const CubicalCellPriority & priority );


    /**
     * Builds a coarse version of [Kfine] whose cells are within
     * [cmin]-[cmax] such that cells outside these bounds are
     * discarded, and cells within these bounds have their coarser
     * bounding cell included.
     *
     * @param Kcoarse (returns) a coarse version of [Kfine]
     * @param Kfine the fine complex to project.
     * @param cmin the lowest cell that can be in [Kcoarse].
     * @param cmin the greatest cell that can be in [Kcoarse].
     * @param data each cell in [Kcoarse] has this data.
     */
    static void project( CubicalComplex & Kcoarse,
			 const CubicalComplex & Kfine,
			 const CubicalCell & cmin,
			 const CubicalCell & cmax,
			 const CubicalCellData & data );


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~CubicalComplex();

    /**
     * Creates an empty cubical complex lying in a space of dimension [dim].
     *
     * @param dim the dimension of the space.
     */
    CubicalComplex( uint dim );

    /**
     * @return the dimension of the space in which lies this cubical complex.
     */
    INLINE uint dim() const;

    // ----------------------- Complex services ------------------------------
  public:

    /**
     * Close the complex.
     */
    void close();

    /**
     * Clear the complex.
     */
    void clear();

    /**
     * Extracts the cells of 'this' that belongs to the parallepipedic
     * subspace [c1, c2]. Cells in ]c1,c2| are added to [inner_S],
     * others are added to [bdry_S]. Do not take care of hierarchical
     * complexes.
     *
     * @param inner_S (returns) the set of inner cells is added to it.
     * @param bdry_S (returns) the set of boundary cells is added to it.
     * @param c1 the lower bound of the subspace.
     * @param c2 the upper bound of the subspace.
     */
    void getCellsInSubspace( cc_cell_set & inner_S, 
			     cc_cell_set & bdry_S, 
			     const CubicalCell & c1,
			     const CubicalCell & c2 );
    
    /**
     * Extracts the cells of 'this' that belongs to the parallepipedic
     * subspace [c1, c2]. Cells in ]c1,c2| are added to [inner_S], others
     * are added to [bdry_S]. Pick only cells of maximal dimension (ie n
     * for inner_S and n-1 for bdry_S). Do not take care of hierarchical
     * complexes.
     *
     * @param inner_S (returns) the set of inner cells is added to it.
     * @param bdry_S (returns) the set of boundary cells is added to it.
     * @param c1 the lower bound of the subspace.
     * @param c2 the upper bound of the subspace.
     */
    void getMaximalCellsInSubspace( cc_cell_set & inner_S, 
				    cc_cell_set & bdry_S, 
				    const CubicalCell & c1,
				    const CubicalCell & c2 );


    // ----------------------- Cell services ------------------------------
  public:

    /**
     * @return an iterator on the first cell.
     */
    INLINE iterator begin();

    /**
     * @return an iterator pointing just after the last cell.
     */
    INLINE iterator end();

    /**
     * @return an iterator on the first cell.
     */
    INLINE const_iterator begin() const;

    /**
     * @return an iterator pointing just after the last cell.
     */
    INLINE const_iterator end() const;
    
    /**
     * Adds the cell [c] to the complex.
     *
     * @param c a cell lying in the adequate space.
     * @param d an optional data to associate with [c].
     * @return an iterator on the cell in the subcomplex.
     */
    INLINE iterator add( const CubicalCell & c,
			 const CubicalCellData & d = CubicalCellData() );

    /**
     * Adds the cell [c] and its closure to the complex.
     *
     * @param c a cell lying in the adequate space.
     * @param d an optional data to associate with all cells in closure([c]).
     * @return an iterator on the cell in the subcomplex.
     */
    iterator addClosure( const CubicalCell & c,
			 const CubicalCellData & d = CubicalCellData() );

    /**
     * Adds the cell [c] and its closure to the complex. Stays within bounds.
     *
     * @param c a cell lying in the adequate space.
     * @param d an optional data to associate with all cells in closure([c]).
     * @param cmin the coordinates of the lowest admissible cell.
     * @param cmax the coordinates of the highest admissible cell.
     * @return an iterator on the cell in the subcomplex.
     */
    iterator addClosure( const CubicalCell & c,
			 const CubicalCell & cmin,
			 const CubicalCell & cmax,
			 const CubicalCellData & d = CubicalCellData() );

    /**
     * Finds the iterator in the complex that points on the cell [c].
     *
     * @param c a cell lying in the adequate space.
     *
     * @return an iterator on the cell in the subcomplex or 'end()' if
     * not found.
     */
    INLINE iterator find( const CubicalCell & c );

    /**
     * Removes from 'this' complex the cell defined by iterator [it].
     *
     * @param it an iterator on a cell of 'this'.
     */
    INLINE void erase( iterator it );

    /**
     * Computes cells 1-low incident to [c] in the cubical complex and
     * adds them to the back of [low_cells].
     */
    void lowIncidentCells1( const CubicalCell & c, cc_cell_list & low_cells );

    /**
     * Computes cells 1-up incident to [c] in the cubical complex and
     * adds them to the back of [up_cells].
     */
    void upIncidentCells1( const CubicalCell & c, cc_cell_list & up_cells );

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param that_stream the output stream where the object is written.
     * @param detail when 'true', displays cells, default is 'false'.
     */
    void selfDisplay( std::ostream & that_stream, bool detail = false ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool OK() const;
  
    /**
     * Writes the list of cells of the complex on [that_stream].
     * @param that_stream the output stream where the object is written.
     */
    void outputAsListOfCells( std::ostream & that_stream );

    /**
     * Test method for the class.
     */
    static void testCubicalComplex();

    // ------------------------- Datas ----------------------------------------
  protected:

    /**
     * Dimension of the space in which lies the cubical complex.
     */
    const uint m_dim;

    /**
     * Stores the cells of the cubical complex and associates some data
     * to each. 
     */
    std::map< CubicalCell, CubicalCellData, CubicalCellInvComparator > m_cells;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE CubicalComplex();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE CubicalComplex( const CubicalComplex & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE CubicalComplex & operator=( const CubicalComplex & other );
  
    // ------------------------- Internals ------------------------------------
  private:

    /**
     * Given a cell [c], tells if it is a maximal cell in the complex
     * (return 0), or if it is a free face of the cell pointed by
     * [it_cell_up] (return 1) or if it is not a free face.
     *
     * The complex must be closed. In computing the 1-up-incident
     * cells, this method ignores cell marked as REMOVED. Furthermore,
     * if one 1-up-incident cell is not marked as COLLAPSIBLE, the
     * method returns 2.
     *
     * @param c a cubical cell (belonging to 'this')
     *
     * @param it_cell_up (returns) a pointer on a cell d if c is a
     * free face of d.
     *
     * @return 0 if the cell is maximal, 1 if the cell is a free face,
     * 2 otherwise.
     */
    uint computeCellType( const CubicalCell & c, 
			  iterator & it_cell_up ); 
  
  };


  /////////////////////////////////////////////////////////////////////////////
  // class CubicalComplex
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'HierarchicalCubicalComplex' <p> Aim:
   * Represent a hierarchical cubical complex, ie a cubical complex
   * whose cells may not have the same dimension.  The complex is
   * built such that at any time, there are no two cells that have an
   * non-empty intersection. 
   */
  class HierarchicalCubicalComplex : public CubicalComplex
  {
    // ----------------------- public types ------------------------------
  public:
    
    /**
     * Defines the type for representing incidences between cells of
     * different sizes in a hierarchical complex, as a map from a
     * given iterator on a cell toward a list of iterators on a cell.
     */
    typedef std::map< CubicalComplex::iterator, 
		      std::list<CubicalComplex::iterator>,
		      CubicalCellInvComparator > cc_incidence_map_t;


    /**
     * Collapse a subcomplex of the closed cubical complex [K],
     * collapsing cells following priority [priority], in a decreasing
     * sequence. [S] provides the starting cells, generally (but not
     * compulsory) maximal cells.  Note: cells that have been marked
     * as fixed are not removed.  only cells that are in the closure
     * of [S] may be removed, and only if they are not marked as
     * fixed.
     *
     * @param K the closed cubical complex to collapse.
     * @param S a subset of cells of [K], as iterators in [K]
     * @param S_closed indicates if [S] is a closed set (faster in this case)
     * @param priority the object that assign a priority to each cell.
     */
    static void collapse( HierarchicalCubicalComplex & K,
			  CubicalComplex::cc_cell_set & S,
			  bool S_closed,
			  const CubicalCellPriority & priority );


    /**
     * Test method for hierarchical cubical complex.
     */
    static void test();


  public:

    /**
     * Destructor. 
     */
    ~HierarchicalCubicalComplex();

    /**
     * Creates an empty hierarchical cubical complex lying in a space
     * of dimension [dim].
     *
     * @param dim the dimension of the space.
     */
    HierarchicalCubicalComplex( uint dim );


    /**
     * Close the complex.
     */
    void close();

    /**
     * Computes the direct incidence maps between cells of different
     * sizes. The principle is to find for each cell if it has coarser
     * incident cells and store them both in maps [coarse_*] and [fine_*].
     *
     * @param coarse_low the mapping associating to a cell the coarser direct
     * low incident cells.
     *
     * @param coarse_up the mapping associating to a cell the coarser direct
     * up incident cells.
     *
     * @param fine_low the mapping associating to a cell the finer direct
     * low incident cells.
     *
     * @param fine_up the mapping associating to a cell the finer direct
     * up incident cells.
     *
     */
    void computeDirectIncidences( cc_incidence_map_t & coarse_low,
				  cc_incidence_map_t & coarse_up,
				  cc_incidence_map_t & fine_low,
				  cc_incidence_map_t & fine_up );


    /**
     * Extracts the cells of 'this' that belongs to the parallepipedic
     * subspace [c1, c2]. Cells in ]c1,c2[ are added to [inner_S],
     * others are added to [bdry_S]. Take care of hierarchical
     * complexes, meaning that all cells within [c1, c2], even finer
     * ones, are selected.
     *
     * @param inner_S (returns) the set of inner cells is added to it.
     * @param bdry_S (returns) the set of boundary cells is added to it.
     * @param c1 the lower bound of the subspace.
     * @param c2 the upper bound of the subspace.
     */
    void getCellsInSubspace( CubicalComplex::cc_cell_set & inner_S, 
			     CubicalComplex::cc_cell_set & bdry_S, 
			     const CubicalCell & c1,
			     const CubicalCell & c2 );
  
    
    // ----------------------- Cell services ------------------------------
  public:

    /**
     * Finds the cell [c] in the complex or the first coarser version
     * of [c].
     *
     * @param it (returns) an iterator on the cell [c] or the first of its
     * coarser version in the subcomplex, .
     * 
     * @param c a cell lying in the adequate space.
     *
     * @return 0 if [c] was found ([it] points to [c]), a positive
     * number (the size difference) if a coarser version of [c] was
     * found (pointed by [it]), -1 if not found ([it] points to the
     * insertion point for [c] in the complex).
     */
    int findCoarser( CubicalComplex::iterator & it, 
		     const CubicalCell & c );

    /**
     * Finds the first coarser version of [c].
     *
     * @param it (returns) an iterator on the first of its coarser version
     * in the subcomplex.
     * 
     * @param c a cell lying in the adequate space.
     *
     * @return a positive number (the size difference) if a coarser
     * version of [c] was found (pointed by [it]), -1 if not found ([it]
     * points to the insertion point for [c] in the complex).
     */
    int findStrictCoarser( CubicalComplex::iterator & it,
			   const CubicalCell & c );


    /**
     * Adds the cell [c] to the complex.  Overriden. Note that the
     * cell is \b not added if it is included in some other cell. In
     * this case, the returned iterator points on this container cell.
     *
     * @param c a cell lying in the adequate space.
     * @param d an optional data to associate with [c].
     * @return an iterator on the cell in the subcomplex.
     */
    iterator add( const CubicalCell & c,
		  const CubicalCellData & d = CubicalCellData() );

    /**
     * Adds the cell [c] and its closure to the
     * complex. Overriden. Note that the cell is \b not added if it is
     * included in some other cell. In this case, the returned
     * iterator points on this container cell.
     *
     * @param c a cell lying in the adequate space.
     * @param d an optional data to associate with all cells in closure([c]).
     * @return an iterator on the cell in the subcomplex.
     */
    iterator addClosure( const CubicalCell & c,
			 const CubicalCellData & d = CubicalCellData() );

    /**
     * Adds the cell [c] and its closure to the complex. Stays within
     * bounds. Overriden. Note that the cell is \b not added if it is
     * included in some other cell. In this case, the returned
     * iterator points on this container cell.
     *
     * @param c a cell lying in the adequate space.
     * @param d an optional data to associate with all cells in closure([c]).
     * @param cmin the coordinates of the lowest admissible cell.
     * @param cmax the coordinates of the highest admissible cell.
     * @return an iterator on the cell in the subcomplex.
     */
    iterator addClosure( const CubicalCell & c,
			 const CubicalCell & cmin,
			 const CubicalCell & cmax,
			 const CubicalCellData & d = CubicalCellData() );

    /**
     * Computes cells 1-low incident to the cell pointed by [itc] in the
     * cubical complex and adds them to the back of [low_cells]. All
     * direct low incident cells of any sizes are extracted, using the
     * mapping [coarse_low] and [fine_low], which should have been
     * computed beforehand.
     *
     * @param low_cells (returns) the list of low incident cells.
     *
     * @param itc an iterator on a given cell of 'this'.
     *
     * @param coarse_low the mapping associating to a cell the coarser direct
     * low incident cells.
     *
     * @param fine_low the mapping associating to a cell the finer direct
     * low incident cells.
     *
     * @see computeDirectIncidences
     */
    void lowIncidentCells1( cc_cell_list & low_cells, 
			    const CubicalComplex::iterator & itc, 
			    const cc_incidence_map_t & coarse_low,
			    const cc_incidence_map_t & fine_low );

    /**
     * Computes cells 1-up incident to the cell pointed by [itc] in the
     * cubical complex and adds them to the back of [low_cells]. All
     * direct up incident cells of any sizes are extracted, using the
     * mapping [coarse_up] and [fine_up], which should have been
     * computed beforehand.
     *
     * @param up_cells (returns) the list of up incident cells.
     *
     * @param itc an iterator on a given cell of 'this'.
     *
     * @param coarse_up the mapping associating to a cell the coarser direct
     * up incident cells.
     *
     * @param fine_up the mapping associating to a cell the finer direct
     * up incident cells.
     *
     * @see computeDirectIncidences
     */
    void upIncidentCells1( cc_cell_list & up_cells, 
			   const CubicalComplex::iterator & itc, 
			   const cc_incidence_map_t & coarse_up,
			   const cc_incidence_map_t & fine_up );


    /**
     * Given a cell [c], tells if it is a maximal cell in the complex
     * (return 0), or if it is a free face of the cell pointed by
     * [it_cell_up] (return 1) or if it is not a free face.
     *
     * The complex must be closed. In computing the 1-up-incident
     * cells, this method ignores cell marked as REMOVED. Furthermore,
     * if one 1-up-incident cell is not marked as COLLAPSIBLE, the
     * method returns 2.
     *
     * @param it_cell_up (returns) a pointer on a cell d if c is a
     * free face of d.
     *
     * @param c a cubical cell (belonging to 'this')
     *
     * @param coarse_up the mapping associating to a cell the coarser direct
     * up incident cells.
     *
     * @param fine_up the mapping associating to a cell the finer direct
     * up incident cells.
     *
     * @return 0 if the cell is maximal, 1 if the cell is a free face,
     * 2 otherwise.
     */
    uint computeCellType( CubicalComplex::iterator & it_cell_up,
			  const CubicalComplex::iterator & itc,
			  const cc_incidence_map_t & coarse_up,
			  const cc_incidence_map_t & fine_up  );

  

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE HierarchicalCubicalComplex();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE HierarchicalCubicalComplex
    ( const HierarchicalCubicalComplex & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE HierarchicalCubicalComplex & operator=
    ( const HierarchicalCubicalComplex & other );
 
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'CubicalComplex'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'CubicalComplex' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const CubicalComplex & that_object_to_display );

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
#include "ImaGene/digitalnD/CubicalComplex.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CubicalComplex_h

#undef CubicalComplex_RECURSES
#endif // else defined(CubicalComplex_RECURSES)
