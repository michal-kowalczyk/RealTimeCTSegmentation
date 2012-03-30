/** @file KnRCellSet.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnRCellSet.h
//
// Creation : 2002/08/02
//
// Version : 2002/08/02
//
// Author : JOL
//
// Summary : Header file for files KnRCellSet.ih and KnRCellSet.cxx
//
// History :
//	2002/08/02 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnRCellSet declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnRCellSet_RECURSES)
#error Recursive header files inclusion detected in KnRCellSet.h
#else // defined(KnRCellSet_RECURSES)
#define KnRCellSet_RECURSES

#if !defined KnRCellSet_h
#define KnRCellSet_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/Bitset1.h"
#include "ImaGene/base/Shared.h"
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/Sanitizer.h"
#include "ImaGene/digitalnD/KnSpace.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

  class KnCharSet;

///////////////////////////////////////////////////////////////////////////////
// class KnRCellSet
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnRCellSet' <p>
 * Aim: To represent any set of Kn cells (signed, unsigned, single type of
 * cells, mixed types of cells) with a characteristic function (0 not present,
 * 1 present).
 * More compact than KnCharSet. Slightly slower for per element operation,
 * faster for global operations.
 */
class KnRCellSet : protected Shared<Bitset1>
{

  // ------------------------- static public services -------------------------
public:

  /**
   * Returns a set (defined with a characteristic function) which may contain
   * any oriented or unoriented cell of dimension [d]. The set is empty or 
   * filled depending on [val].
   * @param ks the space in which the set is defined.
   * @param d the dimension of the cells hold in the returned set.
   * @param oriented when 'true' the set contains oriented cells otherwise the set contains unoriented cells.
   * @param val 0=fills with zeros, otherwise fills with ones.
   * @return a (modifiable) set of [d]-cells (oriented or unoriented).
   */
  static KnRCellSet create( const KnSpace & ks,
			 uint d, 
			 bool oriented = false,
			 int val = 0 );
  
  /**
   * Builds a KnCharSet equivalent to [s].
   * @param s any set of cells (oriented or not).
   * @return the corresponding KnCharSet.
   * NB: do not abuse of conversion between types of sets in Kn, since often
   * one representation is better than the other ones.
   */
  static KnCharSet makeKnCharSet( const KnRCellSet & s );
  


  // ------------------------- public local types -----------------------------
public:

  /**
   * Reference on an element of the set.
   */
  typedef Bitset1::reference reference;

  /**
   * Forward iterator on the elements of a 'KnRCellSet'. Can be used indifferently
   * to scan elements of an oriented or unoriented set.
   */
  class cell_iterator
  {
    /**
     * Associated set.
     */
    const KnRCellSet* m_set;
    /**
     * Current position in set.
     */
    Kn_grid m_current;

    // ------------------------- Standard services ----------------------------
  public:
    /**
     * Constructor.
     * @param ptr_s a pointer on a set or '0'.
     * @param c the reduced code of the starting cell or 'INVALID_RCELL'
     */
    INLINE cell_iterator( const KnRCellSet* ptr_s = 0,
			  Kn_grid c = INVALID_RCELL );
    /**
     * Copy constructor.
     * @param other the cell iterator to clone.
     */
    INLINE cell_iterator( const cell_iterator & other );
    
    /**
     * Assignment.
     * @param other the cell iterator to copy.
     * @return a reference on 'this'.
     */
    INLINE cell_iterator& operator=( const cell_iterator & other );
    
    /**
     * Destructor. Does nothing.
     */
    INLINE ~cell_iterator();
    
    // ------------------------- iteration services ---------------------------
  public:

    /**
     * @return the current cell (oriented or not depending on the set).
     * @see KnRCellSet::cell_iterator::get
     */
    INLINE Kn_gid operator*() const;

    /**
     * @return the current cell (oriented or not depending on the set).
     * @see KnRCellSet::cell_iterator::operator*
     */
    INLINE Kn_gid get() const;

    /**
     * Pre-increment.
     * Goes to the next cell in the set.
     * @see KnRCellSet::cell_iterator::next
     */
    INLINE cell_iterator& operator++();
    /**
     * Goes to the next cell in the set.
     * @see KnRCellSet::cell_iterator::operator++
     */
    INLINE void next();

    /**
     * Equality operator.
     * @param other the cell iterator to compare with (must be defined in the same set).
     * @return 'true' if their current positions coincide.
     */
    INLINE bool operator==( const cell_iterator & other ) const;

    /**
     * Inequality operator.
     * @param other the cell iterator to compare with (must be defined in the same set).
     * @return 'true' if their current positions differs.
     */
    INLINE bool operator!=( const cell_iterator & other ) const;

    /**
     * Inferior operator.
     * @param other the cell iterator to compare with (must be defined in the same set).
     * @return 'true' if the current position of 'this' is before the current position of [other].
     */
    INLINE bool operator<( const cell_iterator & other ) const;

  };

  /**
   * The inner class 'cell_iterator' is a friend of 'KnRCellSet'.
   */
  friend class KnRCellSet::cell_iterator;
  
  typedef cell_iterator iterator;
  typedef cell_iterator const_iterator;

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  INLINE ~KnRCellSet();

  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  INLINE KnRCellSet( const KnRCellSet & other );

  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   */
  INLINE KnRCellSet & operator=( const KnRCellSet & other );

  /**
     Clears the set.
  */
  void clear();

  /**
   * @return other any set.
   * @return 'true' if 'this' is compatible with [other] for set operations.
   */
  INLINE bool isCompatibleWith( const KnRCellSet & other ) const;

  /**
   * @return the number of elements of the set.
   */
  INLINE Kn_size nbElements() const;

  /**
   * @return the number of elements of the set.
   */
  INLINE Kn_size size() const;

  /**
     @return the dimension of the cells within this set.
  */
  uint dim() const;

  // ------------------------- Set comparison services ------------------------
public:

  /**
   * @param other the object to compare with.
   * @return 'true' if the two sets contain the same elements.
   */
  INLINE bool operator==( const KnRCellSet & other ) const;

  /**
   * @param other the object to compare with.
   * @return 'true' if the two sets are different.
   */
  INLINE bool operator!=( const KnRCellSet & other ) const;
  
  
  // ------------------------- Set services -----------------------------------
public:

  /**
   * Non-const access to element, eg <code> b[ i ] = true </code>
   * @param elem the code of a cell.
   * @return a reference on the element.
   * NB: if [elem] is outside the set, may cause trouble.
   */
  INLINE reference operator[]( Kn_gid elem );

  /**
   * Const access to element, eg <code> cout << b[ i ] </code>
   * @param elem the code of a cell.
   * @return a boolean 'true' for 1, 'false' for 0.
   * NB: if [elem] is outside the set, may cause trouble.
   */
  INLINE bool operator[]( Kn_gid elem ) const;

  /**
   * Read/write accessor.
   * [rc_offset] must be a valid index.
   * @param rc_offset any restricted code.
   * @return the value in the vector at index [rc_offset].
   */
  INLINE reference rAt( Kn_grid rc_offset );
  /**
   * Read-only accessor.
   * [rc_offset] must be a valid index.
   * @param rc_offset any restricted code.
   * @return the value in the vector at index [rc_offset].
   */
  INLINE bool rAt( Kn_grid rc_offset ) const;

  /**
   * Non-const access to element when using unoriented cells. Valid if it is
   * a set of unoriented cells.
   * @param elem the code of a cell.
   * @return a reference on the element.
   * NB: if [elem] is outside the set, may cause trouble.
   */
  INLINE reference uAt( Kn_uid elem );

  /**
   * Const access to element when using unoriented cells. Valid if it is
   * a set of unoriented cells.
   * @param elem the code of a cell.
   * @return a boolean 'true' for 1, 'false' for 0.
   * NB: if [elem] is outside the set, may cause trouble.
   */
  INLINE bool uAt( Kn_uid elem ) const;

  /**
   * Non-const access to element when using oriented cells. Valid if it is
   * a set of oriented cells.
   * @param elem the code of a cell.
   * @return a reference on the element.
   * NB: if [elem] is outside the set, may cause trouble.
   */
  INLINE reference sAt( Kn_uid elem );

  /**
   * Const access to element when using soriented cells. Valid if it is
   * a set of unoriented cells.
   * @param elem the code of a cell.
   * @return a boolean 'true' for 1, 'false' for 0.
   * NB: if [elem] is outside the set, may cause trouble.
   */
  INLINE bool sAt( Kn_uid elem ) const;


  // ------------------------- index services --------------------------------
public:

  /**
   * @param c any valid cell (ie. a 'cellDim()' cell).
   * @return the corresponding reduced code.
   */
  INLINE Kn_grid index( Kn_gid c ) const;
  /**
   * @param rc any restricted cell code (ie offset in the set).
   * @return the corresponding cell in the space.
   */
  INLINE Kn_gid cell( Kn_grid rc ) const;


  // ------------------------- Set ops services -------------------------------
public:

  /**
   * Creates the set union of 'this' and [other].
   * @param other the object with which to merge.
   * @return the set union of 'this' and [other].
   */
  INLINE KnRCellSet operator+( const KnRCellSet & other ) const;

  /**
   * Union of the set [other] with 'this'.
   * @param other the object with which to merge.
   * @return a reference on 'this'.
   */
  INLINE KnRCellSet& operator+=( const KnRCellSet & other );

  /**
   * The element [elem] is added to 'this'.
   * @param elem the element to add to 'this'
   * @return a reference on 'this'.
   */
  INLINE KnRCellSet& operator+=( Kn_gid elem );

  /**
   * Creates the set substraction of 'this' and [other].
   * @param other the object to substract.
   * @return the set substraction of 'this' and [other].
   */
  INLINE KnRCellSet operator-( const KnRCellSet & other ) const;

  /**
   * Substraction of [other] to 'this'.
   * @param other the object to substract.
   * @return a reference on 'this'.
   */
  INLINE KnRCellSet& operator-=( const KnRCellSet & other );

  /**
   * The element [elem] is removed from 'this'.
   * @param elem the element to remove from 'this'
   * @return a reference on 'this'.
   */
  INLINE KnRCellSet& operator-=( Kn_gid elem );

  /**
   * Creates the set symmetric difference of 'this' and [other].
   * @param other the object to substract symmetrically.
   * @return the set symmetric difference of 'this' and [other].
   */
  INLINE KnRCellSet operator^( const KnRCellSet & other ) const;

  /**
   * Symmetric difference of [other] to 'this'.
   * @param other the object to substract symmetrically.
   * @return a reference on 'this'.
   */
  INLINE KnRCellSet& operator^=( const KnRCellSet & other );

  /**
   * The element [elem] is removed from 'this' if it was in 'this'. Otherwise
   * the element is added to 'this'. Ie symmetric difference.
   * @param elem the element to remove/add from 'this'
   * @return a reference on 'this'.
   */
  INLINE KnRCellSet& operator^=( Kn_gid elem );

  /**
   * Creates the set intersection of 'this' and [other].
   * @param other the object with which to create an intersection.
   * @return the set union of 'this' and [other].
   */
  INLINE KnRCellSet operator*( const KnRCellSet & other ) const;

  /**
   * Intersection of the set [other] with 'this'.
   * @param other the object with which to intersect.
   * @return a reference on 'this'.
   */
  INLINE KnRCellSet& operator*=( const KnRCellSet & other );

  /**
   * Complement of the set.
   * @return the complement of 'this'.
   */
  INLINE KnRCellSet operator~() const;


  // ------------------------- Scanning services ------------------------------
public:

  /**
   * To scan a set do:
   * <pre> 
   * KnRCellSet s1 = ...
   * for ( KnRCellSet::cell_iterator p = s1.begin(); p != s1.end(); ++p )
   *   cout << " " << *p;
   * cout << endl;
   * </pre>
   * @return the iterator pointing on the first cell belonging to 'this' or the iterator equal to 'end()' if the set is empty.
   * @see KnRCellSet::cell_iterator,end
   * @TODO Create a 'const_cell_iterator'.
   */
  INLINE cell_iterator begin() const;

  /**
   * @return the iterator pointing on the element after the last cell of the set.
   * @see KnRCellSet::cell_iterator,begin
   * @TODO Create a 'const_cell_iterator'.
   */
  INLINE cell_iterator end() const;

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


  // ------------------------- Protected services -----------------------------
protected:

  /**
   * Constructor.
   * Builds a set of [cell_dimension]-cells, given the space [ks], the 
   * dimension of each cell [cell_dimension]. The cells are oriented depending
   * on [oriented].
   * The set is empty at initialization if [val] is 0, otherwise the set
   * contains all the [cell_dimension]-cells of the space.
   * A sanitizer clears up unused bits and words of the bitset, which 
   * are present when the sizes of the coordinates are not powers of 2.
   * @param ks a reference to a digital space.
   * @param sanitizer the sanitizer used to clear unused bits and words of the bitset
   * @param the dimension of each cell.
   * @param oriented indicates whether it is a set of oriented cells or unoriented cells.
   * @param val 0=fills with zeros, otherwise fills with ones.
   */
  KnRCellSet( const KnSpace & ks,
	   const Shared<Sanitizer> & sanitizer,
	   uint cell_dimension,
	   bool oriented,
	   int val = 0 );

  /**
   * Clears up all unused bits and words of the bitset. Generally called after
   * a flipping operation on the whole set.
   */
  INLINE void doSanitize();

  
  // ------------------------- Datas ------------------------------------------
private:

  /**
   * The digital space where the set is defined.
   */
  const KnSpace & m_space;
  
  /**
   * It is used for clearing unused bits and words of the bitset (this is due
   * to the specific alignment of coordinates with powers of 2).
   */
  Shared<Sanitizer> m_sanitizer;

  /**
   * Dimension of the cells stored in this set.
   */
  uint m_cell_dimension;
  
  /**
   * Tells whether the cells are oriented or not in the cell.
   */
  bool m_oriented;
  

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE KnRCellSet();
private:

  
  // ------------------------- Internals --------------------------------------
private:

  /**
   * Constructor from 'Bitset1'.
   * Builds a set of [cell_dimension]-cells, given the space [ks], the 
   * dimension of each cell [cell_dimension]. The cells are oriented depending
   * on [oriented].
   * The set is created with the elements specified in [bitset].
   * A sanitizer clears up unused bits and words of the bitset, which 
   * are present when the sizes of the coordinates are not powers of 2.
   * @param ks a reference to a digital space.
   * @param sanitizer the sanitizer used to clear unused bits and words of the bitset
   * @param the dimension of each cell.
   * @param oriented indicates whether it is a set of oriented cells or unoriented cells.
   * @param bitset the bitset to copy
   * @param to_sanitize when 'true', the set must be sanitized.
   */
  KnRCellSet( const KnSpace & ks,
	   const Shared<Sanitizer> & sanitizer,
	   uint cell_dimension,
	   bool oriented,
	   const Bitset1 & bitset,
	   bool to_sanitize );

  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'KnRCellSet'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'KnRCellSet' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const KnRCellSet & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/KnRCellSet.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnRCellSet_h

#undef KnRCellSet_RECURSES
#endif // else defined(KnRCellSet_RECURSES)
