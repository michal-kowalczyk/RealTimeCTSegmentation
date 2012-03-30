/** @file Shared.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Shared.h
//
// Creation : 2000/12/01
//
// Version : 2002/08/02
//
// Author : JOL
//
// Summary :
//
// History :
//	2000/12/01 : JOL : tested in tdispace-2/tmain.cxx|checkShared()
//                         - creation/duplication/modification.
//	2002/08/02 : JOL : slightly modified (methods ro() and rw())
//                         to follow coding conventions.
//
// Rcs Id : "@(#)class Shared declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Shared_RECURSES)
#error Recursive header files inclusion detected in Shared.h
#else // defined(Shared_RECURSES)
#define Shared_RECURSES

#if !defined Shared_h
#define Shared_h

//////////////////////////////////////////////////////////////////////////////
//                                                                          //

#include <iostream>


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// Template class Shared
///////////////////////////////////////////////////////////////////////////////
/**
 * Description of class 'Shared'
 * Aim: a template class to manipulate objects. It avoids unnecessary cloning
 * of objects.
 * Exemple: <pre>
 * Bitset1 bset( 1000 );
 * Shared<Bitset1> sset1( bset );  // copy bset
 * Shared<Bitset1> sset2( sset1 ); // references sset1.
 */
template <class T>
class Shared
{
  struct Cell
  {
  public:
    inline Cell( const T & elem )
      : m_nb_ref( 1 ), 
	m_element( elem ) // Calls T copy constructor with arg elem.
      {}
    int m_nb_ref;
    T m_element;
  private:
    inline Cell();
    inline Cell( const Cell & other );
    inline Cell & operator=( const Cell & other );
  };
  
public :
  
  // ------------------------- Standard services ------------------------------
  /**
   * Destructor
   */
  inline ~Shared()
    {
      // One less reference to shared object. Indivisible action.
      m_shared->m_nb_ref--;
      if ( m_shared->m_nb_ref == 0 ) delete m_shared;
    }
  
  /**
   * Constructor from 'T'.
   */
  inline Shared( const T & element )
    {
      m_shared = new Cell( element );
    }
  /**
   * Constructor from 'Shared' object.
   */
  inline Shared( const Shared<T> & other )
    {
      // One more reference to shared object. Indivisible action.
      other.m_shared->m_nb_ref++;
      m_shared = other.m_shared;
    }
  /**
   * Assignment from 'Shared' object.
   */
  inline Shared<T>& operator=( const Shared<T> & other )
    {
      if ( this != &other )
	{
	  // One less reference to shared object. Indivisible action.
	  m_shared->m_nb_ref--;
	  if ( m_shared->m_nb_ref == 0 ) delete m_shared;
	  // One more reference to shared object. Indivisible action.
	  other.m_shared->m_nb_ref++;
	  m_shared = other.m_shared;
	}
      return *this;
    }
  /**
   * Read-only access to shared object.
   */
  inline const T & getR() const
    {
      return m_shared->m_element;
    }

  /**
   * Read-write access to shared object. Note that the shared object
   * is cloned if it is has more than one reference on it.
   */
  inline T & getRW()
    {
      if ( m_shared->m_nb_ref > 1 )
	{
	  // One less reference to shared object.
	  m_shared->m_nb_ref--;
	  m_shared = new Cell( m_shared->m_element );
	}
      return m_shared->m_element;
    }  

  /**
   * Read-only access to shared object.
   */
  inline const T & ro() const
    {
      return getR();
    }

  /**
   * Read-write access to shared object. Note that the shared object
   * is cloned if it is has more than one reference on it.
   */
  inline T & rw()
    {
      return getRW();
    }
  
  // ------------------------- Interface --------------------------------------
  /**
   * Writes/Displays the object on an output stream.
   * @param thatStream the output stream where the object is written.
   */
  inline void selfDisplay( std::ostream& that_stream ) const
    {
      that_stream << "[Shared<size=" << sizeof( T )
		  << ">, ref=" << m_shared->m_nb_ref << "]";
    }
  
  /**
   * Checks the validity/consistency of the object.
   * @return 'true' if the object is valid, 'false' otherwise.
   */
  inline bool OK(void) const
    {
      return ( m_shared != 0 ) && ( m_shared->m_nb_ref > 0 );
    }
  
  // ------------------------- Datas ------------------------------------------
private :
  Cell* m_shared;

  // ------------------------- Hidden services --------------------------------
  // ------------------------- Internals --------------------------------------
};

/**
 * Overloads 'operator <<' for displaying objects of class 'Shared'.
 * @param thatStream the output stream where the object is written.
 * @param thatObjectToDisplay the object of class 'Shared' to write.
 * @return the output stream after the writing.
 */
template <class T>
inline std::ostream&
operator <<( std::ostream& that_stream, 
	     const Shared<T> & that_object_to_display )
{
  that_object_to_display.selfDisplay( that_stream );
  return that_stream;
}

} // namespace ImaGene

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Shared_h

#undef Shared_RECURSES
#endif // else defined(Shared_RECURSES)
