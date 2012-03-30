/** @file HashTable.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : HashTable.h
//
// Creation : 2004/12/21
//
// Version : 2004/12/21
//
// Author : JOL
//
// Summary : Header file for files HashTable.ih and HashTable.cxx
//
// History :
//	2004/12/21 : ?Name? : ?What?
//
// Rcs Id : "@(#)class HashTable declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(HashTable_RECURSES)
#error Recursive header files inclusion detected in HashTable.h
#else // defined(HashTable_RECURSES)
#define HashTable_RECURSES

#if !defined HashTable_h
#define HashTable_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class HashTable
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'HashTable' <p>
   * Aim: To represent a hash table as a template.
   * The templatized object must have a copy constructor and an assignment
   * service. It must also have an empty constructor which represent the
   * error.
   */
  template <class Value>
  class HashTable
  {
    /**
     * For now, Key is an unsigned int of 32 bits.
     */
    typedef uint Key;
    
    /**
     * represents an entry in the hash table.
     */
    struct HashTableEntry
    {
      Key key;
      Value value;
      HashTableEntry* next;
      
//       INLINE ~HashTableEntry();
      
//       INLINE HashTableEntry();
//       INLINE HashTableEntry( const Key & other_key,
// 			     const Value & other_value,
// 			     HashTableEntry* next_entry );
      
    };
 


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    INLINE ~HashTable();

    /**
     * Constructor.
     * The object is empty.
     */
    INLINE HashTable();
    
    /**
     * Initializes the hash table for a given set of keys [m] and for
     * an approximate number of values [n].
     *
     * @param m no key can be bigger than this number.
     * @param n the number of lists in the hashtable.
     */
    INLINE void init( uint m, uint n );

    /**
     * Clears the data structure.
     */
    INLINE void clear();

    /**
     * @return the number of entries in the hashtable.
     */
    INLINE uint size() const;

    /**
     * Hashes the given key.
     *
     * @param key any key.
     * @return an integer between 0 and m_n - 1
     */
    INLINE uint hash( Key key ) const;

    /**
     * Gives the number of lists which have 0, 1, ..., >= n-1 entries.
     * 
     * @param n maximum number of entries per list.
     * @param tbl an array of size [n], (returns).
     */
    INLINE void distribution( uint n, uint* tbl ) const;
    
    // ----------------------- Access services ------------------------------
  public:

    /**
     * Maps the specified key to the specified value in the HashTable.
     *
     * @param key a key.
     * @param object a pointer to an object.
     */
    INLINE void put( Key key, const Value & value );
    
    /**
     * Accessor. Tells if a key is present in the hashtable.
     *
     * @param key a key.
     * @return 'true' if the key was present, 'false' otherwise.
     */
    INLINE bool contains( Key key ) const;
   
    /**
     * Accessor. Returns the value associated with a given key. If the
     * key is not present, returns the default value.
     *
     * @param key a key.
     * @return the value to which the specified key is mapped in.
     */
    INLINE const Value & get( Key key ) const;

    /**
     * Accessor. Returns the value associated with a given key. If the
     * key is not present, returns 'false', otherwise returns 'true'.
     *
     * @param key a key.
     * @param value (returns) the value associated with the key.
     * @return 'true' if the key was present, 'false' otherwise.
     */
    INLINE bool get( Key key, Value & value ) const;
    
    /**
     * Deletes the given key [key] from the HashTable.
     *
     * @param key a key.
     */
    INLINE void erase( Key key );
    
    /**
     * Removes the key and its corresponding value from the HashTable.
     *
     * @param key a key.
     * @return the corresponding object.
     */
    INLINE Value remove( Key key );
     
    /**
     * Read-only accessor. Returns the value associated with a given
     * key. If the key is not present, returns the default value.
     *
     * @param key a key.
     * @return the value to which the specified key is mapped in.
     */
    INLINE const Value & operator[]( Key key ) const;

    /**
     * Read-write accessor. Returns a reference to the value
     * associated with a given key. If the key is not present, returns
     * the default value.
     *
     * @param key a key.
     * @return the value to which the specified key is mapped in.
     */
    INLINE Value & operator[]( Key key );

    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * Array pointing to each list of entries.
     */
    HashTableEntry** m_entries;
    
    /**
     * Size of array [m_entries].
     * @see m_entries.
     */
    uint m_n;

    /**
     * Number of entries in the hashtable.
     */
    uint m_size;

    /**
     * First prime number bigger than the number of keys.
     */
    unsigned long long m_p;
    
    /**
     * A random number between 1 and m_p - 1.
     */
    unsigned long long m_a;

    /**
     * A random number between 0 and m_p - 1.
     */
    unsigned long long m_b;

    /**
     * Default value.
     */
    Value m_default;


    // ------------------------- Hidden services ------------------------------
  protected:

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE HashTable( const HashTable & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE HashTable & operator=( const HashTable & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  

    

  };


  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/HashTable.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined HashTable_h

#undef HashTable_RECURSES
#endif // else defined(HashTable_RECURSES)
