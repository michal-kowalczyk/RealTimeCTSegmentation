/** @file KnCharSet.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnCharSet.h
//
// Creation : 2002/08/02
//
// Version : 2002/08/02
//
// Author : JOL
//
// Summary : Header file for files KnCharSet.ih and KnCharSet.cxx
//
// History :
//	2002/08/02 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnCharSet declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnCharSet_RECURSES)
#error Recursive header files inclusion detected in KnCharSet.h
#else // defined(KnCharSet_RECURSES)
#define KnCharSet_RECURSES

#if !defined KnCharSet_h
#define KnCharSet_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/Bitset1.h"
#include "ImaGene/base/Shared.h"
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/Sanitizer.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

  class KnSpace;

///////////////////////////////////////////////////////////////////////////////
// class KnCharSet
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnCharSet' <p>
 * Aim: To represent any set of Kn cells (signed, unsigned, single type of
 * cells, mixed types of cells) with a characteristic function (0 not present,
 * 1 present).
 */
class KnCharSet : protected Shared<Bitset1>
{
  /**
   * Reference on an element of the set.
   */
  typedef Bitset1::reference reference;


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
  static KnCharSet create( const KnSpace & ks,
			   uint d, 
			   bool oriented = false,
			   int val = 0 );

  /**
   * Returns a set (defined with a characteristic function) which may contain
   * any oriented or unoriented cell of any dimension. The set is empty or 
   * filled depending on [val].
   * @param ks the space in which the set is defined.
   * @param oriented when 'true' the set contains oriented cells otherwise the set contains unoriented cells.
   * @param val 0=fills with zeros, otherwise fills with ones.
   * @return a (modifiable) set of cells (oriented or unoriented).
   */
  static KnCharSet create( const KnSpace & ks,
			   bool oriented = false,
			   int val = 0 );

  /**
   * Returns a set (defined with a characteristic function) which may contain
   * any cell of same topology as [c]. The set is empty or 
   * filled depending on [val].
   * @param ks the space in which the set is defined.
   * @param c all cells in the set will have the same topology as [c].
   * @param val 0=fills with zeros, otherwise fills with ones.
   * @return a (modifiable) set of unoriented cells.
   */
  static KnCharSet ucreate( const KnSpace & ks,
			    Kn_uid c,
			    int val = 0 );
  
  /**
   * Returns a set (defined with a characteristic function) which may contain
   * any cell of same topology as [c]. The set is empty or 
   * filled depending on [val].
   * @param ks the space in which the set is defined.
   * @param c all cells in the set will have the same topology as [c].
   * @param val 0=fills with zeros, otherwise fills with ones.
   * @return a (modifiable) set of oriented cells.
   */
  static KnCharSet screate( const KnSpace & ks,
			    Kn_sid c,
			    int val = 0 );
 

  // ------------------------- public types ----------------------------
public:

  
  /**
   * Forward iterator on the elements of a 'KnCharSet'. Can be used
   * indifferently to scan elements of an oriented or unoriented set.
   */
  class cell_iterator
  {
    /**
     * Associated set.
     */
    const KnCharSet* m_set;
    /**
     * Current position in set.
     */
    Kn_gid m_current;

    // ------------------------- Standard services ----------------------------
  public:
    /**
     * Constructor.
     * @param ptr_s a pointer on a set or '0'.
     * @param c the offset in [ptr_s] of the starting cell or 'INVALID_CELL'
     */
    INLINE cell_iterator( const KnCharSet* ptr_s = 0,
			  Kn_gid c = INVALID_CELL );
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
     * @see KnCharSet::cell_iterator::get
     */
    INLINE Kn_gid operator*() const;

    /**
     * @return the current cell (oriented or not depending on the set).
     * @see KnCharSet::cell_iterator::operator*
     */
    INLINE Kn_gid get() const;

    /**
     * Pre-increment.
     * Goes to the next cell in the set.
     * @see KnCharSet::cell_iterator::next
     */
    INLINE cell_iterator& operator++();
    /**
     * Goes to the next cell in the set.
     * @see KnCharSet::cell_iterator::operator++
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
   * The inner class 'cell_iterator' is a friend of 'KnCharSet'.
   */
  friend class KnCharSet::cell_iterator;
  
  
  
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  INLINE ~KnCharSet();

  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  INLINE KnCharSet( const KnCharSet & other );

  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   */
  INLINE KnCharSet & operator=( const KnCharSet & other );

  /**
     Clears the set.
  */
  void clear();

  /**
   * @return other any set.
   * @return 'true' if 'this' is compatible with [other] for set operations.
   */
  INLINE bool isCompatibleWith( const KnCharSet & other ) const;

  /**
   * @return the number of elements of the set.
   */
  INLINE Kn_size nbElements() const;

  /**
   * @return the cell with smallest id that can be stored in 'this'.
   */
  INLINE Kn_gid min() const;

  /**
   * @return the cell with biggest id that can be stored in 'this'.
   */
  INLINE Kn_gid max() const;


  // ------------------------- Protected standard services --------------------
public:

  /**
   * @return elem any element.
   * @return 'true' if [elem] is compatible with 'this' for set operations.
   */
  INLINE bool isCompatibleWith( Kn_gid elem ) const;
  
  // ------------------------- Set comparison services ------------------------
public:

  /**
   * @param other the object to compare with.
   * @return 'true' if the two sets contain the same elements.
   */
  INLINE bool operator==( const KnCharSet & other ) const;

  /**
   * @param other the object to compare with.
   * @return 'true' if the two sets are different.
   */
  INLINE bool operator!=( const KnCharSet & other ) const;
  
  
  // ------------------------- Set services -----------------------------------
public:

  /**
   * Non-const access to element, eg <code> b[ i ] = true </code>
   * @param elem an elem (between 'min()' and 'max()').
   * @return a reference on the element.
   * NB: if [elem] is outside the set, may cause trouble.
   */
  INLINE reference operator[]( Kn_gid elem );

  /**
   * Const access to element, eg <code> cout << b[ i ] </code>
   * @param elem an elem (between 'min()' and 'max()').
   * @return a boolean 'true' for 1, 'false' for 0.
   * NB: if [elem] is outside the set, may cause trouble.
   */
  INLINE bool operator[]( Kn_gid elem ) const;

  /**
   * Protected const access to element, eg <code> cout << b[ i ] </code>
   * @param elem an elem (between 'min()' and 'max()').
   * @return a boolean 'true' for 1, 'false' for 0.
   */
  INLINE bool at( Kn_gid elem ) const;


  // ------------------------- Set ops services -------------------------------
public:

  /**
   * Creates the set union of 'this' and [other].
   * @param other the object with which to merge.
   * @return the set union of 'this' and [other].
   */
  INLINE KnCharSet operator+( const KnCharSet & other ) const;

  /**
   * Union of the set [other] with 'this'.
   * @param other the object with which to merge.
   * @return a reference on 'this'.
   */
  INLINE KnCharSet& operator+=( const KnCharSet & other );

  /**
   * The element [elem] is added to 'this'.
   * @param elem the element to add to 'this'
   * @return a reference on 'this'.
   */
  INLINE KnCharSet& operator+=( Kn_gid elem );

  /**
   * Creates the set substraction of 'this' and [other].
   * @param other the object to substract.
   * @return the set substraction of 'this' and [other].
   */
  INLINE KnCharSet operator-( const KnCharSet & other ) const;

  /**
   * Substraction of [other] to 'this'.
   * @param other the object to substract.
   * @return a reference on 'this'.
   */
  INLINE KnCharSet& operator-=( const KnCharSet & other );

  /**
   * The element [elem] is removed from 'this'.
   * @param elem the element to remove from 'this'
   * @return a reference on 'this'.
   */
  INLINE KnCharSet& operator-=( Kn_gid elem );

  /**
   * Creates the set symmetric difference of 'this' and [other].
   * @param other the object to substract symmetrically.
   * @return the set symmetric difference of 'this' and [other].
   */
  INLINE KnCharSet operator^( const KnCharSet & other ) const;

  /**
   * Symmetric difference of [other] to 'this'.
   * @param other the object to substract symmetrically.
   * @return a reference on 'this'.
   */
  INLINE KnCharSet& operator^=( const KnCharSet & other );

  /**
   * The element [elem] is removed from 'this' if it was in 'this'. Otherwise
   * the element is added to 'this'. Ie symmetric difference.
   * @param elem the element to remove/add from 'this'
   * @return a reference on 'this'.
   */
  INLINE KnCharSet& operator^=( Kn_gid elem );

  /**
   * Creates the set intersection of 'this' and [other].
   * @param other the object with which to create an intersection.
   * @return the set union of 'this' and [other].
   */
  INLINE KnCharSet operator*( const KnCharSet & other ) const;

  /**
   * Intersection of the set [other] with 'this'.
   * @param other the object with which to intersect.
   * @return a reference on 'this'.
   */
  INLINE KnCharSet& operator*=( const KnCharSet & other );

  /**
   * Complement of the set.
   * @return the complement of 'this'.
   */
  INLINE KnCharSet operator~() const;


  // ------------------------- Scanning services ------------------------------
public:

  /**
   * To scan a set do:
   * <pre> 
   * KnCharSet s1 = ...
   * for ( KnCharSet::cell_iterator p = s1.begin(); p != s1.end(); ++p )
   *   cout << " " << *p;
   * cout << endl;
   * </pre>
   * @return the iterator pointing on the first cell belonging to 'this' or the iterator equal to 'end()' if the set is empty.
   * @see KnCharSet::cell_iterator,end
   * @TODO Create a 'const_cell_iterator'.
   */
  INLINE cell_iterator begin() const;

  /**
   * @return the iterator pointing on the element after the last cell of the set.
   * @see KnCharSet::cell_iterator,begin
   * @TODO Create a 'const_cell_iterator'.
   */
  INLINE cell_iterator end() const;

  /**
   * @return the first unsigned cell belonging to 'this'.
   */
  INLINE Kn_uid ubegin() const;

  /**
   * @param prev any cell that can belong to 'this'.
   * @return the unsigned cell after [prev] belonging to 'this'.
   */
  INLINE Kn_uid unext( Kn_uid prev ) const;

  /**
   * @return the unsigned cell that is returned when 'ubegin' or 'unext' have failed.
   * @see ubegin,unext
   */
  INLINE Kn_uid uend() const;

  /**
   * @return the first signed cell belonging to 'this'.
   */
  INLINE Kn_sid sbegin() const;

  /**
   * @param prev any cell that can belong to 'this'.
   * @return the signed cell after [prev] belonging to 'this'.
   */
  INLINE Kn_sid snext( Kn_sid prev ) const;

  /**
   * @return the signed cell that is returned when 'sbegin' or 'snext' have failed.
   * @see sbegin,snext
   */
  INLINE Kn_sid send() const;
  

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
   * Builds a set of Kn_uid or Kn_sid, given the element with smallest id [min]
   * and the element with biggest id [max]. The set is empty at initialization
   * if [val] is 0, otherwise the set contains all the elements from [min] to
   * [max]. A sanitizer clears up unused bits and words of the bitset, which 
   * are present when the sizes of the coordinates are not powers of 2.
   * @param sanitizer the sanitizer used to clear unused bits and words of the bitset
   * @param min the smallest possible id.
   * @param max the biggest possible id.
   * @param val 0=fills with zeros, otherwise fills with ones.
   * NB: protected. Internal use only. To create a 'KnCharSet' object, call
   * 'KnCharSet::create'
   * @see KnCharSet::create,KnCharSet::ucreate,KnCharSet::screate
   */
  KnCharSet( const Shared<Sanitizer> & sanitizer,
	       Kn_gid min, Kn_gid max, 
	       int val = 0 );

  /**
   * Clears up all unused bits and words of the bitset. Generally called after
   * a flipping operation on the whole set.
   */
  INLINE void doSanitize();

  
  // ------------------------- Datas ------------------------------------------
private:
  /**
   * It is used for clearing unused bits and words of the bitset (this is due
   * to the specific alignment of coordinates with powers of 2).
   */
  Shared<Sanitizer> m_sanitizer;
  /**
   * This is the smallest possible id in the set.
   */
  Kn_gid m_min;
  /**
   * This is one more than the biggest possible id in the set.
   */
  Kn_gid m_max1;

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE KnCharSet();
private:

  
  // ------------------------- Internals --------------------------------------
private:

  /**
   * Copy constructor from BitSet1.
   * @param bitset the bitset to copy
   * @param sanitizer the sanitizer used to clear unused bits and words of the bitset
   * @param min the smallest possible id.
   * @param max the biggest possible id.
   * @param to_sanitize when 'true', the set must be sanitized.
   */
  KnCharSet( const Bitset1 & bitset, 
	     const Shared<Sanitizer> & sanitizer,
	     Kn_gid min, Kn_gid max,
	     bool to_sanitize = true );
  
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'KnCharSet'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'KnCharSet' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const KnCharSet & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/KnCharSet.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnCharSet_h

#undef KnCharSet_RECURSES
#endif // else defined(KnCharSet_RECURSES)
