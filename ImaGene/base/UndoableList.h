/** @file UndoableList.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : UndoableList.h
//
// Creation : 2010/02/12
//
// Version : 2010/02/12
//
// Author : JOL
//
// Summary : Header file for files UndoableList.ih and UndoableList.cxx
//
// History :
//	2010/02/12 : ?Name? : ?What?
//
// Rcs Id : "@(#)class UndoableList declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(UndoableList_RECURSES)
#error Recursive header files inclusion detected in UndoableList.h
#else // defined(UndoableList_RECURSES)
#define UndoableList_RECURSES

#if !defined UndoableList_h
#define UndoableList_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <stack>
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
#ifdef DEBUG
  static int undoablelist_nb_cells = 0;
#endif

  /////////////////////////////////////////////////////////////////////////////
  // class UndoableList
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'UndoableList' <p> Aim: Represents a list
   * which can store creations and deletions, so that operations may
   * be undone. Otherwise, 
   */
  template <typename NodeType>
  class UndoableList
  {

    // ----------------------- Internal types ------------------------------
  public:

    /**
     * Encapsulates a node in a structure storing also pointers to the
     * previous and next node structure. It is used to implement a
     * double-linked chain list.
     */
    struct Cell {
      
      /**
       * It allows access to node, next and previous from the list.
       */
      friend class UndoableList<NodeType>;
      /**
       * It allows access to node, next and previous from the list.
       */
      friend class UndoableList<NodeType>::_quasi_iterator;
      /**
       * It allows access to node, next and previous from the list.
       */
      friend class UndoableList<NodeType>::_const_iterator;

    private:
      /**
       * The element in the list.
       */
      NodeType node;

      /**
       * A pointer to the previous cell of the list.
       */
      Cell* previous;

      /**
       * A pointer to the next cell of the list.
       */
      Cell* next;

    public:
      /**
       * Constructor of cell.
       *
       * @param n the value to store (which is copied into the cell).
       * @param pr a pointer to the previous cell (default is 0).
       * @param nx a pointer to the next cell (default is 0).
       */
      Cell( const NodeType & n,
	    Cell* pr = 0, Cell* nx = 0 )
	: node( n ), previous( pr ), next( nx )
      {
#ifdef DEBUG
	++undoablelist_nb_cells;
#endif
      }
      /**
       * Destructor of cell. If NodeType was a pointer, this
       * destructor does not aim to delete the pointed element.
       */
      ~Cell()
      {
#ifdef DEBUG
	--undoablelist_nb_cells;
#endif
      }
    private:
      /**
       * Default constructor is forbidden.
       */
      Cell();

      /**
       * Copy constructor is forbidden.
       */
      Cell( const Cell & other );

    };

    // ----------------------- Public enum types ------------------------------
  public:
    /**
     * Defines the different types of operations.
     */
    enum OperationType { Separator, Insertion, Suppression, Modification };

    /**
     * Stores the possible modifications and updates on the
     * list. Allows later undoing.
     */
    struct Modif {

    private:
      /**
       * Each update operation has a type.
       */
      OperationType m_type;

      /**
       * Each update operation may points to two cells. This is the first one.
       */
      Cell* m_it1;

      /**
       * Each update operation may points to two cells. This is the second one.
       */
      Cell* m_it2;

    private:

      /**
       * Constructor. Hidden so that 'Modif' object may only be
       * created through makeSeparator, makeSuppression,
       * makeInsertion, makeModification.
       *
       * @param t the update operation type.
       * @param it1 the first cell possibly pointed by this object.
       * @param it2 the second cell possibly pointed by this object.
       *
       * @see makeSeparator
       * @see makeInsertion
       * @see makeSuppression
       * @see makeModification
       */
      Modif( OperationType t, 
	     Cell* it1,
	     Cell* it2 )
	: m_type( t ), m_it1( it1 ), m_it2( it2 )
      {}

    public:

      /**
       * Default constructor. Does nothing. The object is not
       * valid. Defined so that such objects can be stored in a
       * collection.
       */
      Modif() {}

      /**
       * Copy constructor. Defined so that such objects can be stored in a
       * collection.
       *
       * @param other the object to clone.
       */
      Modif( const Modif & other ) 
	: m_type( other.m_type ), m_it1( other.m_it1 ), m_it2( other.m_it2 )
      {}

      /**
       * @return the operation type.
       */
      OperationType type() const
      {
	return m_type;
      }

      /**
       * @return the first cell pointed by this object.
       */
      Cell* cell1() const
      {
	return m_it1;
      }

      /**
       * @return the second cell pointed by this object.
       */
      Cell* cell2() const
      {
	return m_it2;
      }

      /**
       * @return 'true' if this object is a 'Separator'.
       */
      bool isSeparator() const
      { 
	return m_type == Separator;
      }

      /**
       * @return a 'Separator' object. Useful for undoing series of
       * operations in one call to 'undoGroup()'.
       *
       * @see undoGroup
       */
      static Modif makeSeparator()
      {
	return Modif( Separator, 0, 0 );
      }

      /**
       * @param cell the suppressed cell.
       * @param next_cell the cell following the suppressed cell.
       *
       * @return a 'Suppression' object. Used to memorize a
       * suppression operation in the list (like erase).
       */
      static Modif makeSuppression( Cell* cell, Cell* next_cell )
      {
	return Modif( Suppression, cell, next_cell );
      }

      /**
       * @param new_cell the inserted cell.
       *
       * @param pos the cell at which [new_cell] is inserted (it
       * becomes its next cell).
       *
       * @return an 'Insertion' object. Used to memorize an insertion
       * operation in the list (like insert).
       */
      static Modif makeInsertion( Cell* new_cell, Cell* pos )
      {
	return Modif( Insertion, new_cell, pos );
      }

      /**
       * @param replaced_cell the replaced cell.
       * @param new_cell the new cell which takes the place of [replaced_cell].
       *
       * @return a 'Modification' object. Used to memorize an assignment
       * operation in the list (like assign).
       */
      static Modif makeModification( Cell* replaced_cell, Cell* new_cell )
      {
	return Modif( Modification, replaced_cell, new_cell );
      }
    };

    /**
     * This class provides an iterator in the collection
     * 'UndoableList'. It is not a full iterator since non-const
     * dereferencing is forbidden. A user should use
     * 'UndoableList<NodeType>::assign' instead.
     */
    struct _quasi_iterator
    {
      //      typedef bidirectional_iterator_tag    iterator_category;
      typedef NodeType                      value_type;
      typedef const NodeType*               const_pointer;
      typedef const NodeType&               const_reference;

      /**
       * Stores the current cell pointed by the iterator.
       */
      Cell* m_current;

      /**
       * Destructor. Does nothing.
       */
      _quasi_iterator()
      {}

      /**
       * Constructor from position. Points on cell [pos].
       *
       * @param pos any cell (including 0, which is the end of the list).
       */
      _quasi_iterator( Cell* pos )
	: m_current( pos )
      {}

      /**
       * Copy constructor. Clones [other]
       *
       * @param other any iterator.
       */
      _quasi_iterator( const _quasi_iterator & other )
	: m_current( other.m_current )
      {}

      /**
       * Assignment. Clones [other]
       *
       * @param other any iterator.
       * @return a reference on 'this'.
       */
      _quasi_iterator& operator=( const _quasi_iterator & other )
      {
	if ( &other != this )
	  m_current = other.m_current;
	return *this;
      }

      /**
       * Conversion to const iterator.
       */
      operator typename ImaGene::UndoableList<NodeType>::_const_iterator() const
      {
	return _const_iterator( m_current );
      }

      /**
       * Read-only access.
       *
       * @return a reference on the value of the cell pointed by
       * 'this'.
       */
      const_reference
      operator*() const
      {
	return m_current->node;
      }

      /**
       * Read-only access.
       *
       * @return a pointer on the value of the cell pointed by
       * 'this'.
       */
      const_pointer
      operator->() const
      { 
	return &(m_current->node);
      }

      /**
       * Post-increment operation. The iterator is moved forward (next).
       *
       * @return a reference on 'this'.
       */
      _quasi_iterator&
      operator++()
      {
	m_current = m_current->next;
	return *this;
      }

      /**
       * Pre-increment operation. The iterator is moved forward (next).
       *
       * @return an iterator pointing on the previous position pointed
       * by 'this'.
       */
      _quasi_iterator
      operator++(int)
      {
	_quasi_iterator here( *this );
	m_current = m_current->next;
	return here;
      }

      /**
       * Post-decrement operation. The iterator is moved backward (previous).
       *
       * @return a reference on 'this'.
       */
      _quasi_iterator&
      operator--()
      {
	m_current = m_current->previous;
	return *this;
      }

      /**
       * Pre-decrement operation. The iterator is moved backward (previous).
       *
       * @return an iterator pointing on the previous position pointed
       * by 'this'.
       */
      _quasi_iterator
      operator--(int)
      {
	_quasi_iterator here( *this );
	m_current = m_current->previous;
	return here;
      }

      /**
       * Equality operator.
       *
       * @param other any other iterator.
       *
       * @return 'true' if 'this' and [other] points on the same cell,
       * 'false' otherwise.
       */
      bool
      operator==( const _quasi_iterator & other ) const
      { 
	return m_current == other.m_current;
      }

      /**
       * Inequality operator.
       *
       * @param other any other iterator.
       *
       * @return 'true' if 'this' and [other] points on different cells,
       * 'false' otherwise.
       */
      bool
      operator!=( const _quasi_iterator & other ) const
      { 
	return m_current != other.m_current;
      }

      /**
       * @return the cell currently pointed by 'this' iterator.
       */
      Cell* current() const
      {
	return m_current;
      }
    };

    /**
     * This class provides a co,st iterator in the collection
     * 'UndoableList'. 
     */
    struct _const_iterator
    {
      //      typedef bidirectional_iterator_tag    iterator_category;
      typedef NodeType                      value_type;
      typedef const NodeType*               const_pointer;
      typedef const NodeType&               const_reference;

      /**
       * Stores the current cell pointed by the iterator.
       */
      const Cell* m_current;

      /**
       * Destructor. Does nothing.
       */
      _const_iterator()
      {}

      /**
       * Constructor from position. Points on cell [pos].
       *
       * @param pos any cell (including 0, which is the end of the list).
       */
      _const_iterator( const Cell* pos )
	: m_current( pos )
      {}

      /**
       * Copy constructor. Clones [other]
       *
       * @param other any iterator.
       */
      _const_iterator( const _const_iterator & other )
	: m_current( other.m_current )
      {}

      /**
       * Assignment. Clones [other]
       *
       * @param other any iterator.
       * @return a reference on 'this'.
       */
      _const_iterator& operator=( const _const_iterator & other )
      {
	if ( &other != this )
	  m_current = other.m_current;
	return *this;
      }

      /**
       * Read-only access.
       *
       * @return a reference on the value of the cell pointed by
       * 'this'.
       */
      const_reference
      operator*() const
      {
	return m_current->node;
      }

      /**
       * Read-only access.
       *
       * @return a pointer on the value of the cell pointed by
       * 'this'.
       */
      const_pointer
      operator->() const
      { 
	return &(m_current->node);
      }

      /**
       * Post-increment operation. The iterator is moved forward (next).
       *
       * @return a reference on 'this'.
       */
      _const_iterator&
      operator++()
      {
	m_current = m_current->next;
	return *this;
      }

      /**
       * Pre-increment operation. The iterator is moved forward (next).
       *
       * @return an iterator pointing on the previous position pointed
       * by 'this'.
       */
      _const_iterator
      operator++(int)
      {
	_const_iterator here( *this );
	m_current = m_current->next;
	return here;
      }

      /**
       * Post-decrement operation. The iterator is moved backward (previous).
       *
       * @return a reference on 'this'.
       */
      _const_iterator&
      operator--()
      {
	m_current = m_current->previous;
	return *this;
      }

      /**
       * Pre-decrement operation. The iterator is moved backward (previous).
       *
       * @return an iterator pointing on the previous position pointed
       * by 'this'.
       */
      _const_iterator
      operator--(int)
      {
	_const_iterator here( *this );
	m_current = m_current->previous;
	return here;
      }

      /**
       * Equality operator.
       *
       * @param other any other iterator.
       *
       * @return 'true' if 'this' and [other] points on the same cell,
       * 'false' otherwise.
       */
      bool
      operator==( const _const_iterator & other ) const
      { 
	return m_current == other.m_current;
      }

      /**
       * Inequality operator.
       *
       * @param other any other iterator.
       *
       * @return 'true' if 'this' and [other] points on different cells,
       * 'false' otherwise.
       */
      bool
      operator!=( const _const_iterator & other ) const
      { 
	return m_current != other.m_current;
      }

      /**
       * @return the cell currently pointed by 'this' iterator.
       */
      const Cell* current() const
      {
	return m_current;
      }
    };


    // ----------------------- Standard services ------------------------------
  public:
    
    /**
     * Defines the public type 'quasi_iterator' from the internal type
     * '_quasi_iterator'.
     */
    typedef _quasi_iterator quasi_iterator;

    /**
     * Defines the public type 'const_iterator' from the internal type
     * '_const_iterator'.
     */
    typedef _const_iterator const_iterator;

    /**
     * Destructor. 
     */
    ~UndoableList();

    /**
     * Constructor.
     * @param store when 'true', stores modifications of the list.
     */
    UndoableList( bool store = true );

    /**
     * This operation takes O(n) operations, if n is the number of
     * elements in the list.
     *
     * @return the number of elements in the list.
     */
    uint size() const;

    // ----------------------- Undo services ----------------------------------
    /**
     * @return 'true' if the list is currently storing modifications.
     */
    bool isStoring() const;

    /**
     * @param store when 'true', stores modifications of the list.
     *
     * If store is 'false', then the stack of modifications is cleared.
     */
    void setStoringMode( bool store = true );

    /**
     * Begins a group of modifications.
     */
    void beginModifGroup();

    /**
     * Ends a group of modifications.
     */
    void endModifGroup();

    /**
     * Undo one operation if storing mode is on.
     * @return the type of undo operation done.
     */
    OperationType undoOne();

    /**
     * Undo one group of operations if storing mode is on.
     */
    void undoGroup();

    // ----------------------- Iterator services ------------------------------
    
    /**
     * @return an iterator on the first element of the list.
     */
    quasi_iterator begin();

    /**
     * @return an iterator on the last element of the list.
     */
    quasi_iterator end();

    /**
     * @return a const iterator on the first element of the list.
     */
    const_iterator begin() const;

    /**
     * @return a const iterator on the last element of the list.
     */
    const_iterator end() const;

    // ----------------------- List services ----------------------------------
  public:

    /**
     * Clears the list. Note: this modification is \b not stored,
     * whichever the storing mode of the list. It thus clears also the
     * stack of modifications. Storing mode is left unchanged.
     */
    void clear();

    /**
     *  @brief  Remove element at given position.
     *  @param  it Iterator pointing to element to be erased.
     *  @return  An iterator pointing to the next element (or end()).
     *
     *  This function will erase the element at the given position and
     *  thus shorten the list by one. Memorizes the transformation of
     *  the list if in storing mode.
     *
     *  Due to the nature of a list this operation can be done in
     *  constant time, and only invalidates iterators/references to
     *  the element being removed.  The user is also cautioned that
     *  this function only erases the element, and that if the element
     *  is itself a pointer, the pointed-to memory is not touched in
     *  any way.  Managing the pointer is the user's responsibilty.
     */
    quasi_iterator erase(quasi_iterator it);

    /**
     *  @brief  Remove a range of elements.
     *  @param  first  Iterator pointing to the first element to be erased.
     *  @param  last  Iterator pointing to one past the last element to be
     *                erased.
     *  @return  An iterator pointing to the element pointed to by @a last
     *           prior to erasing (or end()).
     *
     *  This function will erase the elements in the range @a
     *  [first,last) and shorten the list accordingly.  Memorizes the
     *  transformations of the list if in storing mode.
     *
     *  Due to the nature of a list this operation can be done in
     *  constant time, and only invalidates iterators/references to
     *  the element being removed.  The user is also cautioned that
     *  this function only erases the elements, and that if the
     *  elements themselves are pointers, the pointed-to memory is not
     *  touched in any way.  Managing the pointer is the user's
     *  responsibilty.
     */
    quasi_iterator erase(quasi_iterator first, quasi_iterator last);

    /**
     *  @brief  Inserts given value into %list before specified iterator.
     *  @param  position  An iterator into the %list.
     *  @param  x  Data to be inserted.
     *  @return  An iterator that points to the inserted data.
     *
     *  Memorizes the transformations of the list if in storing mode.
     *
     *  This function will insert a copy of the given value before
     *  the specified location.  Due to the nature of a %list this
     *  operation can be done in constant time, and does not
     *  invalidate iterators and references.
     */
    quasi_iterator
    insert( quasi_iterator position, const NodeType & x);

    /**
     *  @brief  Inserts a range into the %list.
     *  @tparam InputIterator any input iterator.
     *  @param  position  An iterator into the %list.
     *  @param  first  An input iterator.
     *  @param  last   An input iterator.
     *
     *  This function will insert copies of the data in the range [@a
     *  first,@a last) into the %list before the location specified by
     *  @a position.
     *
     *  This operation is linear in the number of elements inserted and
     *  does not invalidate iterators and references.
     */
    template <typename InputIterator>
    void
    insert( quasi_iterator position, 
	    InputIterator first, InputIterator last);

    /**
     *  @brief  Assigns given value into %list at specified iterator.
     *
     *  @param position An iterator into the %list, returns an
     *  iterator that points to the assigned data.
     *
     *  @param  x Data to be assigned.
     *
     *  Memorizes the transformations of the list if in storing mode.
     *
     *  This function will assign a copy of the given value at the
     *  specified location.  Due to the nature of a %list this
     *  operation can be done in constant time. The former iterator is
     *  invalidated if in storing mode, since in this case the
     *  assigned cell is in fact moved out of the list. The user
     *  should use the returned iterator to go on in the list.
     */
    void
    assign( quasi_iterator & position, const NodeType & x);


  protected:
    /**
     * Removes all stored modifs.
     */
    void clearModifs();

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param that_stream the output stream where the object is written.
     */
    void selfDisplay( std::ostream & that_stream ) const;

    /**
     * Writes/Displays the elements of the list on an output stream.
     * @param that_stream the output stream where the object is written.
     */
    void displayElements( std::ostream & that_stream ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool OK() const;

    /**
     * Performs several self tests on this class.
     *
     * @param out the output stream where some info is displayed.
     * @return 'true' if everything went well, 'false' otherwise.
     */
    static bool selfTest( std::ostream & out );

    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * The virtual node that is the element before the first and the
     * element after the last.
     */
    Cell m_vnode;

    /**
     * The starting point in the reversed list.
     */
    // Cell* m_rfirst;

    /**
     * The storing mode.
     */
    bool m_is_storing;

    /**
     * The stack for storing modifications.
     */
    std::stack< Modif > m_modifs;


    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE UndoableList( const UndoableList & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE UndoableList & operator=( const UndoableList & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'UndoableList'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'UndoableList' to write.
   * @return the output stream after the writing.
   */
  template <typename NodeType>
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const UndoableList<NodeType> & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/UndoableList.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined UndoableList_h

#undef UndoableList_RECURSES
#endif // else defined(UndoableList_RECURSES)
