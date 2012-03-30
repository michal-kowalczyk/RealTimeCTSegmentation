/** @file SmartSet.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : SmartSet.h
//
// Creation : 2011/06/23
//
// Version : 2011/06/23
//
// Author : JOL
//
// Summary : Header file for files SmartSet.ih and SmartSet.cxx
//
// History :
//	2011/06/23 : ?Name? : ?What?
//
// Rcs Id : "@(#)class SmartSet declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(SmartSet_RECURSES)
#error Recursive header files inclusion detected in SmartSet.h
#else // defined(SmartSet_RECURSES)
#define SmartSet_RECURSES

#if !defined SmartSet_h
#define SmartSet_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <set>
#include "ImaGene/base/CowPtr.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class SmartSet
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'SmartSet' <p> Aim: The type for an
   * arbitrary large set of vertices which can be copied without cost
   * if not modified. Based on std::set.
   */
  template <typename T>
  class SmartSet
  {
    // ----------------------- types ------------------------------
  public:
    typedef std::set<T> _Set;
    typedef typename _Set::iterator iterator;
    typedef typename _Set::const_iterator const_iterator;

    // ----------------------- Standard services ------------------------------
  public:
    /**
     * Constructor. Empty set.
     */
    inline SmartSet() 
      : mySet( new _Set )
    {}

    /**
     * Destructor. 
     */
    inline ~SmartSet() 
    {
      // automatic: delete mySet;
    }

    /**
       Copy constructor.
       @param other the object to clone.
    */
    inline SmartSet( const SmartSet & other ) 
      : mySet( other.mySet )
    {}

    /**
       Assignment.
       @param other the object to clone.
    */
    inline SmartSet & operator=( const SmartSet & other ) 
    {
      if ( this != &other )
	mySet = other.mySet;
      return *this;
    }

    inline void clear()
    {
      mySet->clear();
    }

    inline unsigned int size() const
    {
      return mySet->size();
    }

    inline iterator begin()
    {
      return mySet->begin();
    }

    inline iterator end()
    {
      return mySet->end();
    }

    inline const_iterator begin() const
    {
      return mySet->begin();
    }

    inline const_iterator end() const
    {
      return mySet->end();
    }

    inline bool operator[]( const T & vtx ) const
    {
      return mySet->find( vtx ) != mySet->end();
    }

    inline SmartSet& operator+=( const T & vtx )
    {
      mySet->insert( vtx );
      return *this;
    }

    inline SmartSet& operator-=( const T & vtx )
    {
      mySet->erase( vtx );
      return *this;
    }

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
    CowPtr< _Set > mySet;

  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'SmartSet'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'SmartSet' to write.
   * @return the output stream after the writing.
   */
  template <typename T>
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const SmartSet<T> & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/SmartSet.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined SmartSet_h

#undef SmartSet_RECURSES
#endif // else defined(SmartSet_RECURSES)
