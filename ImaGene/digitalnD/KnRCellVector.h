/** @file KnRCellVector.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnRCellVector.h
//
// Creation : 2003/02/06
//
// Version : 2003/02/06
//
// Author : JOL
//
// Summary : Header file for files KnRCellVector.ih and KnRCellVector.cxx
//
// History :
//	2003/02/06 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnRCellVector declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnRCellVector_RECURSES)
#error Recursive header files inclusion detected in KnRCellVector.h
#else // defined(KnRCellVector_RECURSES)
#define KnRCellVector_RECURSES

#if !defined KnRCellVector_h
#define KnRCellVector_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/KnSpace.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class KnRCellVector
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnRCellVector' <p>
 * Aim: Base class to represent a vector indexed by restricted cell codes.
 */
template <class T>
class KnRCellVector : private std::vector<T>
{
  // ------------------------- public types ------------------------------
public:
  /**
   * read-write random access iterator on vector.
   */
  typedef typename std::vector<T>::iterator iterator;
  /**
   * read-only random access iterator on vector.
   */
  typedef typename std::vector<T>::const_iterator const_iterator;

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  INLINE ~KnRCellVector();

public:
  /**
   * Constructor from space and number of cells.
   * @param ks a reference to a digital space.
   * @param s the size of the vector.
   */
  INLINE KnRCellVector( const KnSpace & ks,
		     uint s );

  // ------------------------- vector services --------------------------------
public:

  /**
   * @return the number of elements stored in the vector.
   */
  INLINE uint size() const;
  
  /**
   * Read/write accessor.
   * [rc_offset] must be a valid index.
   * @param rc_offset any restricted code.
   * @return the value in the vector at index [rc_offset].
   */
  INLINE T& rAt( Kn_grid rc_offset );
  /**
   * Read-only accessor.
   * [rc_offset] must be a valid index.
   * @param rc_offset any restricted code.
   * @return the value in the vector at index [rc_offset].
   */
  INLINE const T& rAt( Kn_grid rc_offset ) const;
  /**
   * Iterator.
   * @return an iterator pointing on the first element of the vector.
   */
  INLINE iterator begin();
  /**
   * Iterator.
   * @return an iterator pointing after the last element of the vector.
   */
  INLINE iterator end();
  /**
   * Const iterator.
   * @return an iterator pointing on the first element of the vector.
   */
  INLINE const_iterator begin() const;
  /**
   * Const iterator.
   * @return an iterator pointing after the last element of the vector.
   */
  INLINE const_iterator end() const;
  

  // ------------------------- Interface --------------------------------------
public:
  /**
   * Writes/Displays the object on an output stream.
   * @param that_stream the output stream where the object is written.
   */
  INLINE void selfDisplay( std::ostream & that_stream ) const;
  /**
   * Checks the validity/consistency of the object.
   * @return 'true' if the object is valid, 'false' otherwise.
   */
  INLINE bool OK() const;
  
  // ------------------------- Datas ------------------------------------------
protected:

  /**
   * Digital space where cells are defined.
   */
  const KnSpace & m_ks;


  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE KnRCellVector();

private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE KnRCellVector( const KnRCellVector & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE KnRCellVector & operator=( const KnRCellVector & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};


// inline functions and external operators                 //

/**
 * Overloads 'operator<<' for displaying objects of class 'KnRCellVector'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'KnRCellVector' to write.
 * @return the output stream after the writing.
 */
template <class T>
std::ostream&
operator<<( std::ostream & that_stream,
	    const KnRCellVector<T> & that_object_to_display );


///////////////////////////////////////////////////////////////////////////////
// class KnRUCellVector
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnRUCellVector' <p>
 * Aim: To represent a vector indexed by restricted unsigned cell codes.
 * Specialization of KnRCellVector.
 */
template <class T>
class KnRUCellVector : public KnRCellVector<T>
{
  // ------------------------- Standard services ------------------------------
public:

  /**
   * Constructor from digital space [ks] and dimension of cells [k].
   * @param ks the digital space containing the cells.
   * @param k the dimension of the cells that define the domain of the vector.
   */
  INLINE KnRUCellVector( const KnSpace & ks, uint k );

  /**
   * @return the dimension of the unsigned cells that define the domain of 
   * the vector.
   */
  INLINE uint cellDim() const;
  

  // ------------------------- vector services --------------------------------
public:

  /**
   * Read/write accessor.
   * @param c any valid cell (ie. a 'cellDim()' cell).
   * @return the value in the vector corresponding to [c].
   */
  INLINE T& operator[]( Kn_uid c );

  /**
   * Read-only accessor.
   * @param c any valid cell (ie. a 'cellDim()' cell).
   * @return the value in the vector corresponding to [c].
   */
  INLINE const T& operator[]( Kn_uid c ) const;

  // ------------------------- index services --------------------------------
public:

  /**
   * @param c any valid cell (ie. a 'cellDim()' cell).
   * @return the corresponding index in the vector.
   */
  INLINE Kn_urid index( Kn_uid c ) const;
  /**
   * @param rc any unsigned restricted cell code (ie index in the vector).
   * @return the corresponding cell in the space.
   */
  INLINE Kn_uid cell( Kn_urid rc ) const;
  

  // ------------------------- Datas ------------------------------------------
private:

  /**
   * Dimension of the cells that are used as indices of the vector.
   */
  uint m_k;


  // ------------------------- Internals --------------------------------------
private:
  
};


///////////////////////////////////////////////////////////////////////////////
// class KnRSCellVector
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnRSCellVector' <p>
 * Aim: To represent a vector indexed by restricted signed cell codes.
 * Specialization of KnRCellVector.
 */
template <class T>
class KnRSCellVector : public KnRCellVector<T>
{
  // ------------------------- Standard services ------------------------------
public:

  /**
   * Constructor from digital space [ks] and dimension of cells [k].
   * @param ks the digital space containing the cells.
   * @param k the dimension of the cells that define the domain of the vector.
   */
  INLINE KnRSCellVector( const KnSpace & ks, uint k );

  /**
   * @return the dimension of the signed cells that define the domain of 
   * the vector.
   */
  INLINE uint cellDim() const;
  

  // ------------------------- vector services --------------------------------
public:

  /**
   * Read/write accessor.
   * @param c any valid cell (ie. a 'cellDim()' cell).
   * @return the value in the vector corresponding to [c].
   */
  INLINE T& operator[]( Kn_sid c );

  /**
   * Read-only accessor.
   * @param c any valid cell (ie. a 'cellDim()' cell).
   * @return the value in the vector corresponding to [c].
   */
  INLINE const T& operator[]( Kn_sid c ) const;

  // ------------------------- index services --------------------------------
public:

  /**
   * @param c any valid cell (ie. a 'cellDim()' cell).
   * @return the corresponding index in the vector.
   */
  INLINE Kn_srid index( Kn_sid c ) const;
  /**
   * @param rc any unsigned restricted cell code (ie index in the vector).
   * @return the corresponding cell in the space.
   */
  INLINE Kn_sid cell( Kn_srid rc ) const;
  

  // ------------------------- Datas ------------------------------------------
private:

  /**
   * Dimension of the cells that are used as indices of the vector.
   */
  uint m_k;


  // ------------------------- Internals --------------------------------------
private:
  
};


} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/KnRCellVector.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnRCellVector_h

#undef KnRCellVector_RECURSES
#endif // else defined(KnRCellVector_RECURSES)
