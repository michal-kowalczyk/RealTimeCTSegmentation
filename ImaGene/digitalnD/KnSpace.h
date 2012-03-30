/** @file KnSpace.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnSpace.h
//
// Creation : 2002/07/20
//
// Version : 2002/07/20
//
// Author : JOL
//
// Summary : To represent a digital n-dimensional space.
//
// History :
//	2002/07/20 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnSpace declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnSpace_RECURSES)
#error Recursive header files inclusion detected in KnSpace.h
#else // defined(KnSpace_RECURSES)
/**
 * Useful to avoid recursive inclusion.
 */
#define KnSpace_RECURSES

#if !defined KnSpace_h
#define KnSpace_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <deque>
//#include "LinAlg/LinAlg/Vector.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Shared.h"
#include "ImaGene/base/TypedObject.h"
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/Sanitizer.h"
//////////////////////////////////////////////////////////////////////////////

// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class KnSpace
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnSpace' <p>
 * Aim: To represent and code a Khalimsky's space Kn and its elements. 
 * This coding allows a nice coding of digital spaces embedded in Kn.
 */
  class KnSpace : public TypedObject
{
 public:

  /**
   * represents a stack or a queue of unsigned cells.
   */
  typedef std::deque<Kn_uid> deque_ucells;

  /**
   * represents a stack or a queue of signed cells.
   */
  typedef std::deque<Kn_sid> deque_scells;


  // ------------------------- TypedObject services ---------------------------
public:

  /**
   * @return the type id of the instantiated subclass.
   * NB: not used at the moment and unstable.
   */
  // JOL changed since ImageLib-new
  INLINE virtual uint getTypeId() const;

  // ------------------------- Helper classes ------------------------------
public:
  /**
   * This class is useful for looping on all "interesting" coordinates of a
   * cell. For instance, surfels in Z3 have two interesting coordinates (the
   * ones spanned by the surfel).
   */
  class dir_iterator 
  {
    /**
     * the current direction.
     */
    uint m_k;
    /**
     * the directions to iterate.
     */
    uint m_dirs;

  public:
    /**
     * Constructor from directions of cell.
     * @param dirs the directions of a cell.
     */
    dir_iterator( uint dirs ) 
      : m_k( 0 ), m_dirs( dirs )
    {
      if ( m_dirs ) find();
    }

    /**
     * @return the current direction.
     */
    uint operator*() const
    {
      return m_k;
    }

    /**
     * @return the current coded directions.
     */
    uint codedDirs() const
    {
      return m_dirs;
    }

    /**
     * Pre-increment. Go to next direction.
     */
    dir_iterator & operator++()
    {
      m_dirs >>= 1;
      ++m_k;
      if ( m_dirs ) find();
      return *this;
    }
    
    /** 
     * Fast comparison with unsigned integer. Comparison == with 0 is 'true' at
     * the end of the iteration.
     * @param coded_dirs any coded directions.
     * @return 'true' if the iterator dirs are different from the coded dirs.
     */
    bool operator!=( uint coded_dirs ) const
    {
      return m_dirs != coded_dirs;
    }

    /** 
     * @return 'true' if the iteration is ended.
     */
    bool end() const
    {
      return m_dirs == 0;
    }

    /** 
     * Slow comparison with other iterator. Useful to check for end of loop.
     * @param other any direction iterator.
     */
    bool operator!=( const dir_iterator & other ) const
    {
      return ( m_dirs != other.m_dirs )
	|| ( m_k != other.m_k );
    }

    /** 
     * Slow comparison with other iterator.
     * @param other any direction iterator.
     */
    bool operator==( const dir_iterator & other ) const
    {
      return ( m_dirs == other.m_dirs )
	&& ( m_k == other.m_k );
    }
    
  private:
    /**
     * Look for valid coordinate (m_dirs must be != 0).
     */
    void find()
    {
      while ( ( m_dirs & 1 ) == 0 )
	{
	  m_dirs >>= 1;
	  ++m_k;
	}
    }
  };


  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor
   */
  ~KnSpace();

  /**
   * Constructor.
   * This constructor prepares many codes to offer an efficient implementation
   * of Kn. To scan a subspace of Kn, use a 'KnSpaceScanner'.
   * NB: you should call 'OK()' after an instanciation to check if your space
   * can be coded with 'Kn_id's.
   * TODO: provide "big" Kn_id's for big Kn spaces.
   * @param n is the dimension of the space.
   * @param sizes is an array containing the sizes along each dimension (sizes of the digital space).
   * @see KnSpaceScanner
   */
  KnSpace( uint n, const uint64* sizes );

  /**
   * Constructor.
   * This constructor prepares many codes to offer an efficient implementation
   * of Kn. To scan a subspace of Kn, use a 'KnSpaceScanner'.
   * NB: you should call 'OK()' after an instanciation to check if your space
   * can be coded with 'Kn_id's.
   * TODO: provide "big" Kn_id's for big Kn spaces.
   * @param n is the dimension of the space.
   * @param sizes is an array containing the sizes along each dimension (sizes of the digital space).
   * @see KnSpaceScanner
   */
  KnSpace( uint n, const uint32* sizes );


  // ------------------------- Protected standard services --------------------
protected:
  /**
   * Default constructor.
   * the space is empty (dim = 0).
   * @see init
   */
  KnSpace();
  /**
   * This init service is called by the constructor or derived class at
   * instanciation.
   * @param n is the dimension of the space.
   * @param sizes is an array containing the sizes along each dimension (sizes of the digital space).
   */
  void init( uint n, const Kn_size* sizes );
  
  // ------------------------- Basic services ------------------------------
public:
  /**
   * @return the dimension of the space.
   */
  INLINE uint dim() const;
  /**
   * @param k a coordinate (from 0 to 'dim()-1').
   * @return the width of the space in the [k]-dimension.
   */
  INLINE Kn_size size( uint k ) const;
  /**
   * @return the size of the whole space.
   */
  INLINE Kn_size size() const;
  /**
   * @param k a dimension (from 0 to 'dim()').
   * @return the number of [k]-cells in this space.
   */
  INLINE Kn_size nbCells( uint k ) const;
  /**
   * @param k a coordinate (from 0 to 'dim()-1').
   * @return the width of the space in the [k]-dimension (rounded to the closest power of 2).
   */
  INLINE Kn_size trueSize( uint k ) const;
  /**
   * @return the true size of the whole space (meaning sizes have been rounded to the closest power of 2).
   */
  INLINE Kn_size trueSize() const;
  /**
   * @param k a dimension (from 0 to 'dim()').
   * @return the true number of [k]-cells in this space.
   */
  INLINE Kn_size trueNbCells( uint k ) const;
  /**
   * @param k a coordinate (from 0 to 'dim()-1').
   * @return the log_2 of the width of the space in the [k]-dimension (log_2(trueSize))
   */
  INLINE uint logSize( uint k ) const;


  // ------------------------- Cell coding services ---------------------------
public:
  /**
   * From the coordinates of a point in Kn, builds its unsigned code.
   * @param x an array of size 'dimension()'.
   * @return the unsigned code of this point.
   */
  INLINE Kn_uid ukcode( const Kn_size* x ) const;
  /**
   * From the coordinates of a point in Kn and a sign, builds its signed code.
   * @param x an array of size 'dimension()'.
   * @param s the sign is Kn_Types::POS or Kn_Types::NEG.
   * @return the signed code of this point.
   */
  INLINE Kn_sid skcode( const Kn_size* x, Kn_sign s ) const;
  /**
   * From the coordinates of a point in Kn, builds its positive code.
   * @param x an array of size 'dimension()'.
   * @return the positive signed code of this point.
   */
  INLINE Kn_sid poskcode( const Kn_size* x ) const;
  /**
   * From the coordinates of a point in Kn, builds its negative code.
   * @param x an array of size 'dimension()'.
   * @return the negative signed code of this point.
   */
  INLINE Kn_sid negkcode( const Kn_size* x ) const;

  /**
   * From the digital coordinates of a point in Zn and a cell type, builds its
   * unsigned code.
   * @param x an array of size 'dimension()'.
   * @param c a (unsigned) celltype.
   * @return the unsigned code of this point.
   */
  INLINE Kn_uid ucode( const Kn_size* x, Kn_uid c ) const;
  /**
   * From the digital coordinates of a point in Zn and a signed cell type,
   * builds its unsigned code.
   * @param x an array of size 'dimension()'.
   * @param c a (signed) celltype.
   * @return the signed code of this point.
   */
  INLINE Kn_sid scode( const Kn_size* x, Kn_sid c ) const;

  // ------------------------- Cell decoding services -------------------------
public:
  /**
   * @param p any unsigned cell.
   * @param k a valid dimension.
   * @return the k-th (digital) coordinate of p
   */
  INLINE Kn_size udecodeCoord( Kn_uid p, uint k ) const;
  /**
   * @param p any signed cell.
   * @param k a valid dimension.
   * @return the k-th (digital) coordinate of p
   */
  INLINE Kn_size sdecodeCoord( Kn_sid p, uint k ) const;
  /**
   * Fills the array [x] with the digital coordinates of [p].
   * @param p any unsigned cell.
   * @param x an array of at least 'dim()' size.
   */
  INLINE void udecodeCoords( Kn_uid p, Kn_size* x ) const;
  /**
   * Fills the array [x] with the digital coordinates of [p].
   * @param p any signed cell.
   * @param x an array of at least 'dim()' size.
   */
  INLINE void sdecodeCoords( Kn_sid p, Kn_size* x ) const;
  /**
   * Fills the array [x] with the digital coordinates of [p].
   * @param p any unsigned cell.
   * @param x an array of at least 'dim()' size.
   */
  INLINE void udecodeCoords( Kn_uid p, int* x ) const;
  /**
   * Fills the array [x] with the digital coordinates of [p].
   * @param p any signed cell.
   * @param x an array of at least 'dim()' size.
   */
  INLINE void sdecodeCoords( Kn_sid p, int* x ) const;
  /**
   * @param p any unsigned cell.
   * @param k a valid dimension.
   * @return the k-th (Khalimsky) coordinate of p
   */
  INLINE Kn_size ukdecodeCoord( Kn_uid p, uint k ) const;
  /**
   * @param p any signed cell.
   * @param k a valid dimension.
   * @return the k-th (Khalimsky) coordinate of p
   */
  INLINE Kn_size skdecodeCoord( Kn_sid p, uint k ) const;
  /**
   * Fills the array [x] with the Khalimsky coordinates of [p].
   * @param p any unsigned cell.
   * @param x an array of at least 'dim()' size.
   */
  INLINE void ukdecodeCoords( Kn_uid p, Kn_size* x ) const;
  /**
   * Fills the array [x] with the Khalimsky coordinates of [p].
   * @param p any signed cell.
   * @param x an array of at least 'dim()' size.
   */
  INLINE void skdecodeCoords( Kn_sid p, Kn_size* x ) const;
  /**
   * Fills the array [x] with the Khalimsky coordinates of [p].
   * @param p any unsigned cell.
   * @param x an array of at least 'dim()' size.
   */
  INLINE void ukdecodeCoords( Kn_uid p, int* x ) const;
  /**
   * Fills the array [x] with the Khalimsky coordinates of [p].
   * @param p any signed cell.
   * @param x an array of at least 'dim()' size.
   */
  INLINE void skdecodeCoords( Kn_sid p, int* x ) const;
  /**
   * @return the sign of the signed cell p.
   */
  INLINE Kn_sign decodeSign( Kn_sid p ) const;


  // -------------------- Conversion signed/unsigned --------------------------
public:
  /**
   * Creates a signed cell from an unsigned one and a given sign.
   * @param p any unsigned cell.
   * @param s a sign.
   * @return the signed version of the cell [p] with sign [s].
   */
  INLINE Kn_sid signs( Kn_uid p, Kn_sign s ) const;
  /**
   * Creates a positively signed cell from an unsigned one.
   * @param p any unsigned cell.
   * @return the signed version of the cell [p] with positive sign.
   */
  INLINE Kn_sid signsPos( Kn_uid p ) const;
  /**
   * Creates a negatively signed cell from an unsigned one.
   * @param p any unsigned cell.
   * @return the signed version of the cell [p] with negative sign.
   */
  INLINE Kn_sid signsNeg( Kn_uid p ) const;
  /**
   * Creates an unsigned cell from a signed one.
   * @param p any signed cell.
   * @return the unsigned version of the cell [p].
   */
  INLINE Kn_uid unsigns( Kn_sid p ) const;

  /**
   * Creates the signed cell with the inverse sign of [p].
   * @param p any signed cell.
   * @return the cell [p] with opposite sign.
   */
  INLINE Kn_sid sopp( Kn_sid p ) const;

  /**
   * Creates a positively signed cell from an signed one.
   * @param p any signed cell.
   * @return the cell [p] with positive sign.
   */
  INLINE Kn_sid toPos( Kn_sid p ) const;

  /**
   * Creates a negatively signed cell from an signed one.
   * @param p any signed cell.
   * @return the cell [p] with negative sign.
   */
  INLINE Kn_sid toNeg( Kn_sid p ) const;


  // -------------------- Conversion between cell-types -----------------------
public:

  /**
   * Creates a spel-like cell with the same coordinates as the given cell.
   * @param p any unsigned cell.
   * @return an unsigned spel-like cell with the same coordinates as [p].
   */
  INLINE Kn_uid uspel( Kn_uid p ) const;

  /**
   * Creates a spel-like cell with the same coordinates as the given cell.
   * @param p any signed cell.
   * @return a signed spel-like cell with the same coordinates as [p].
   */
  INLINE Kn_sid sspel( Kn_sid p ) const;

  /**
   * Creates a pointel-like cell with the same coordinates as the given cell.
   * @param p any unsigned cell.
   * @return an unsigned pointel-like cell with the same coordinates as [p].
   */
  INLINE Kn_uid upointel( Kn_uid p ) const;

  /**
   * Creates a pointel-like cell with the same coordinates as the given cell.
   * @param p any signed cell.
   * @return a signed pointel-like cell with the same coordinates as [p].
   */
  INLINE Kn_sid spointel( Kn_sid p ) const;


  // -------------------- Coordinates services -----------------------
public:

  /**
   * @param p any unsigned cell.
   * @return a word containing only the coordinates of [p].
   * NB: equivalent to returning the pointel with same coordinates.
   */
  INLINE Kn_coords ucoords( Kn_uid p ) const;

  /**
   * @param p any signed cell.
   * @return a word containing only the coordinates of [p].
   * NB: equivalent to returning the unsigned pointel with same coordinates.
   */
  INLINE Kn_coords scoords( Kn_sid p ) const;
  
  /**
   * Removes the [i]-coordinate from [p].
   * @param p a coordinate word with n coordinates.
   * @param i any coordinate.
   * @return a coordinate word with n-1-coordinates.
   */
  INLINE Kn_coords project( Kn_coords p, uint i ) const;

  /**
   * Removes the [i]-coordinate and the [j]-coordinate from [p].
   * @param p a coordinate word with n coordinates.
   * @param i any coordinate.
   * @param j any coordinate different from [i].
   * @return a coordinate word with n-2-coordinates.
   */
  INLINE Kn_coords project( Kn_coords p, uint i, uint j ) const;



  // ------------------------- Cell topology services -------------------------
public:
  /**
   * @param p any unsigned cell.
   * @return the topology word of [p].
   */
  INLINE uint utopology( Kn_uid p ) const;

  /**
   * @param p any signed cell.
   * @return the topology word of [p].
   */
  INLINE uint stopology( Kn_sid p ) const;

  /**
   * @param p any unsigned cell.
   * @return the dimension of the cell [p].
   */
  INLINE uint udim( Kn_uid p ) const;

  /**
   * @param p any signed cell.
   * @return the dimension of the cell [p].
   */
  INLINE uint sdim( Kn_sid p ) const;

  /**
   * @param b any unsigned cell.
   * @return 'true' if [b] is a surfel (spans all but one coordinate).
   */
  INLINE bool uisSurfel( Kn_uid b ) const;

  /**
   * @param b any signed cell.
   * @return 'true' if [b] is a surfel (spans all but one coordinate).
   */
  INLINE bool sisSurfel( Kn_sid b ) const;
  

  // -------------------- Restricted cell codes services ----------------------
public:

  /**
   * Useful for mappings from set of unsigned k-cells.
   * @param c any unsigned cell.
   * @return the restricted cell code of [c] (every k-cell has a unique restricted code).
   */
  INLINE Kn_urid urcode( Kn_uid c ) const;

  /**
   * Useful for mappings from set of signed k-cells.
   * @param c any signed cell.
   * @return the restricted cell code of [c] (every signed k-cell has a unique restricted code).
   */
  INLINE Kn_srid srcode( Kn_sid c ) const;

  /**
   * Useful for mappings 'f' from set of signed k-cells but with f(c)=f(-c).
   * @param c any signed cell.
   * @return the unsigned restricted cell code of [c] (every signed k-cell has a unique restricted code).
   */
  INLINE Kn_urid srucode( Kn_sid c ) const;
  
  /**
   * @param c any unsigned restricted cell code.
   * @param k the dimension of [c].
   * @return the cell code of [c].
   */
  INLINE Kn_uid urdecode( Kn_urid c, uint k ) const;

  /**
   * @param c any signed restricted cell code.
   * @param k the dimension of [c].
   * @return the cell code of [c].
   */
  INLINE Kn_sid srdecode( Kn_srid c, uint k ) const;


  // -------------------- Iterator services for cells ------------------------
public:

  /**
   * Given an unsigned cell [p], returns an iterator to iterate over each 
   * coordinate the cell spans. (A spel spans all coordinates; a surfel all but
   * one, etc). Example: <pre>
   * Kn_uid p;
   * KnSpace::dir_iterator q;
   * // Variant 1
   * for ( q = ks.ubegin_dirs( p ); q != 0; ++q ) ...
   * // Variant 2
   * for ( q = ks.ubegin_dirs( p ); ! q.end(); ++q ) ...
   * </pre>
   * @param p any unsigned cell.
   * @return an iterator that points on the first coordinate spanned by the cell.
   */
  INLINE dir_iterator ubegin_dirs( Kn_uid p ) const;

  /**
   * Given a signed cell [p], returns an iterator to iterate over each 
   * coordinate the cell spans. (A spel spans all coordinates; a surfel all but
   * one, etc). Example: <pre>
   * Kn_sid p;
   * KnSpace::dir_iterator q;
   * // Variant 1
   * for ( q = ks.sbegin_dirs( p ); q != 0; ++q ) ...
   * // Variant 2
   * for ( q = ks.sbegin_dirs( p ); ! q.end(); ++q ) ...
   * </pre>
   * @param p any signed cell.
   * @return an iterator that points on the first coordinate spanned by the cell.
   */
  INLINE dir_iterator sbegin_dirs( Kn_sid p ) const;

  /**
   * Given an unsigned cell [p], returns an iterator to iterate over each 
   * coordinate the cell does not span. (A spel spans all coordinates; 
   * a surfel all but one, etc). Example: <pre>
   * Kn_uid p;
   * KnSpace::dir_iterator q;
   * // Variant 1
   * for ( q = ks.ubegin_inv_dirs( p ); q != 0; ++q ) ...
   * // Variant 2
   * for ( q = ks.ubegin_inv_dirs( p ); ! q.end(); ++q ) ...
   * </pre>
   * @param p any unsigned cell.
   * @return an iterator that points on the first coordinate not spanned by the cell.
   */
  INLINE dir_iterator ubegin_inv_dirs( Kn_uid p ) const;

  /**
   * Given a signed cell [p], returns an iterator to iterate over each 
   * coordinate the cell does not span. (A spel spans all coordinates; 
   * a surfel all but one, etc). Example: <pre>
   * Kn_sid p;
   * KnSpace::dir_iterator q;
   * // Variant 1
   * for ( q = ks.sbegin_inv_dirs( p ); q != 0; ++q ) ...
   * // Variant 2
   * for ( q = ks.sbegin_inv_dirs( p ); ! q.end(); ++q ) ...
   * </pre>
   * @param p any signed cell.
   * @return an iterator that points on the first coordinate spanned by the cell.
   */
  INLINE dir_iterator sbegin_inv_dirs( Kn_sid p ) const;

  /**
   * Given an unsigned surfel [s], returns its orthogonal direction (ie,
   * the coordinate where the surfel is closed).
   *
   * @param s an unsigned surfel
   * @return the orthogonal direction of [s]
   */
  INLINE uint uorthDir( Kn_uid s ) const;

  /**
   * Given a signed surfel [s], returns its orthogonal direction (ie,
   * the coordinate where the surfel is closed).
   *
   * @param s a signed surfel
   * @return the orthogonal direction of [s]
   */
  INLINE uint sorthDir( Kn_sid s ) const;


  // -------------------- Unsigned Coded geometry services --------------------
 public:

  /**
   * Useful for scanning the space.
   * NB: you can also code it with coordinates in {0,1}.
   * @return the first cell of the space with the same type as [p].
   */
  INLINE Kn_uid ufirst( Kn_uid p ) const;

  /**
   * @return the unsigned cell of the space with the lowest id.
   */
  INLINE Kn_uid ufirst() const;

  /**
   * @return the surfel-like unsigned cell of the space with the lowest id.
   */
  INLINE Kn_uid ufirstSurfel() const;

  /**
   * @param r the dimension of the cell.
   * @return the unsigned [r]-cell of the space with the lowest id.
   */
  INLINE Kn_uid ufirstCell( uint r ) const;

  /**
   * @return the last cell of the space with the same type as [p].
   */
  INLINE Kn_uid ulast( Kn_uid p ) const;

  /**
   * @return the unsigned cell of the space with the highest id.
   */
  INLINE Kn_uid ulast() const;

  /**
   * @return the surfel-like unsigned cell of the space with the highest id.
   */
  INLINE Kn_uid ulastSurfel() const;

  /**
   * @param r the dimension of the cell.
   * @return the unsigned [r]-cell of the space with the highest id.
   */
  INLINE Kn_uid ulastCell( uint r ) const;

  /**
   * NB: you can go out of the space.
   * @param p any cell.
   * @param k the coordinate that is changed.
   * @return the same element as [p] except for an incremented coordinate [k].
   */
  INLINE Kn_uid ugetIncr( Kn_uid p, uint k ) const;
  /**
   * Increment [p] in the [0]-direction (x).
   * NB: you can go out of the space.
   * NB: Use instead p++ for speed-up.
   * @param p (modified) any cell.
   */
  static INLINE void uincrX( Kn_uid & p );
  
  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the tested coordinate.
   * @return true if [p] cannot have its [k]-coordinate augmented without leaving the space.
   */
  INLINE bool uisMax( Kn_uid p, uint k ) const;
  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the concerned coordinate.
   * @return the cell similar to [p] but with the maximum allowed [k]-coordinate.
   */
  INLINE Kn_uid ugetMax( Kn_uid p, uint k ) const;
  /**
   * NB: you can go out of the space.
   * @param p any cell.
   * @param k the coordinate that is changed.
   * @return the same element as [p] except for an decremented coordinate [k].
   */
  INLINE Kn_uid ugetDecr( Kn_uid p, uint k ) const;
  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the tested coordinate.
   * @return true if [p] cannot have its [k]-coordinate decreased without leaving the space.
   */
  INLINE bool uisMin( Kn_uid p, uint k ) const;
  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the concerned coordinate.
   * @return the cell similar to [p] but with the minimum allowed [k]-coordinate.
   */
  INLINE Kn_uid ugetMin( Kn_uid p, uint k ) const;
  /**
   * NB: you can go out of the space.
   * @param p any cell.
   * @param k the coordinate that is changed.
   * @param x the increment.
   * @return the same element as [p] except for a coordinate [k] incremented with x.
   */
  INLINE Kn_uid ugetAdd( Kn_uid p, uint k, Kn_size x ) const;
  /**
   * NB: you can go out of the space.
   * @param p any cell.
   * @param k the coordinate that is changed.
   * @param x the decrement.
   * @return the same element as [p] except for a coordinate [k] decremented with x.
   */
  INLINE Kn_uid ugetSub( Kn_uid p, uint k, Kn_size x ) const;
  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the coordinate that is tested.
   * @return the number of increment to do to reach the maximum value.
   */
  INLINE Kn_size udistanceMax( Kn_uid p, uint k ) const;
  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the coordinate that is tested.
   * @return the number of decrement to do to reach the minimum value.
   */
  INLINE Kn_size udistanceMin( Kn_uid p, uint k ) const;
  /**
   * Code coordinates in a digital space. It is in fact a pointel of the space.
   * @param x an array of size 'dimension()'.
   * @return the unsigned code of the corresponding pointel.
   */
  INLINE Kn_uid ucodeCoords( const Kn_size* x ) const;
  /**
   * Add the coded coordinates [coord] (ie a pointel) to [p]. 
   * NB: you can go out of the space.
   * @param p any cell.
   * @param coord any pointel.
   * @return the unsigned code of the cell [p] translated by [coord].
   */
  INLINE Kn_uid utranslatePos( Kn_uid p, Kn_uid coord ) const;
  /**
   * Get the coded bound of [p] in the [k]th direction constrained by
   * [bound].
   * @param p any cell.
   * @param bound the element acting as bound.
   * @param k the concerned coordinate.
   * @return the unsigned code of the bound of [p].
   */
  INLINE Kn_uid ugetBound( Kn_uid p, Kn_uid bound, uint k ) const;
  /**
   * Get the coded bound of [p] in the 0th direction (x) constrained by
   * [bound]. (For speed-up)
   * @param p any cell.
   * @param bound the element acting as bound.
   * @return the unsigned code of the bound of [p].
   */
  INLINE Kn_uid ugetBoundX( Kn_uid p, Kn_uid bound ) const;

  /**
   * Increment the cell [p] to its next position (as classically done in
   * a scanning).
   * NB: also it is very generic, it is better to use a 'KnSpaceScanner' for
   * an efficient scanning (when you know the dimension of the space at compile
   * time.
   * Exemple: <pre> 
   * KnSpace kn( ... );
   * Kn_uid first, last; // lower and upper bounds 
   * Kn_uid p = first;
   * do 
   *   { // ... whatever [p] is the current cell
   *   }
   * while ( kn.uincrInBounds( p, first, last ) ); 
   * </pre>
   * @param p any cell.
   * @param lower the lower bound.
   * @param upper the upper bound.
   * @return true if p is still within the bounds, false if the scanning is finished.
   */
  INLINE bool uincrInBounds( Kn_uid & p, Kn_uid lower, Kn_uid upper ) const;
  

  // -------------------- Signed Coded geometry services --------------------
public:

  /**
   * Useful for scanning the space.
   * NB: you can also code it with coordinates in {0,1}.
   * @return the first cell of the space with the same type as [p].
   */
  INLINE Kn_sid sfirst( Kn_sid p ) const;

  /**
   * @return the signed cell of the space with the lowest id.
   */
  INLINE Kn_sid sfirst() const;

  /**
   * @return the surfel-like signed cell of the space with the lowest id.
   */
  INLINE Kn_sid sfirstSurfel() const;

  /**
   * @param r the dimension of the cell.
   * @return the signed [r]-cell of the space with the lowest id.
   */
  INLINE Kn_sid sfirstCell( uint r ) const;

  /**
   * @return the last cell of the space with the same type as [p].
   */
  INLINE Kn_sid slast( Kn_sid p ) const;

  /**
   * @return the signed cell of the space with the highest id.
   */
  INLINE Kn_sid slast() const;

  /**
   * @return the surfel-like signed cell of the space with the highest id.
   */
  INLINE Kn_sid slastSurfel() const;

  /**
   * @param r the dimension of the cell.
   * @return the signed [r]-cell of the space with the highest id.
   */
  INLINE Kn_sid slastCell( uint r ) const;


  /**
   * NB: you can go out of the space.
   * @param p any cell.
   * @param k the coordinate that is changed.
   * @return the same element as [p] except for an incremented coordinate [k].
   */
  INLINE Kn_sid sgetIncr( Kn_sid p, uint k ) const;

  /**
   * Increment [p] in the [0]-direction (x).
   * NB: you can go out of the space.
   * @param p (modified) any cell.
   */
  static INLINE void sincrX( Kn_sid & p );

  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the tested coordinate.
   * @return true if [p] cannot have its [k]-coordinate augmented without leaving the space.
   */
  INLINE bool sisMax( Kn_sid p, uint k ) const;

  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the concerned coordinate.
   *
   * @return the cell similar to [p] but with the maximum allowed
   * [k]-coordinate.
   */
  INLINE Kn_sid sgetMax( Kn_sid p, uint k ) const;

  /**
   * NB: you can go out of the space.
   * @param p any cell.
   * @param k the coordinate that is changed.
   * @return the same element as [p] except for an decremented coordinate [k].
   */
  INLINE Kn_sid sgetDecr( Kn_sid p, uint k ) const;

  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the tested coordinate.
   * @return true if [p] cannot have its [k]-coordinate decreased without leaving the space.
   */
  INLINE bool sisMin( Kn_sid p, uint k ) const;

  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the concerned coordinate.
   * @return the cell similar to [p] but with the minimum allowed [k]-coordinate.
   */
  INLINE Kn_sid sgetMin( Kn_sid p, uint k ) const;

  /**
   * NB: you can go out of the space.
   * @param p any cell.
   * @param k the coordinate that is changed.
   * @param x the increment.
   * @return the same element as [p] except for a coordinate [k] incremented with x.
   */
  INLINE Kn_sid sgetAdd( Kn_sid p, uint k, Kn_size x ) const;

  /**
   * NB: you can go out of the space.
   * @param p any cell.
   * @param k the coordinate that is changed.
   * @param x the decrement.
   * @return the same element as [p] except for a coordinate [k] decremented with x.
   */
  INLINE Kn_sid sgetSub( Kn_sid p, uint k, Kn_size x ) const;

  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the coordinate that is tested.
   * @return the number of increment to do to reach the maximum value.
   */
  INLINE Kn_size sdistanceMax( Kn_sid p, uint k ) const;

  /**
   * Useful to check if you are going out of the space.
   * @param p any cell.
   * @param k the coordinate that is tested.
   * @return the number of decrement to do to reach the minimum value.
   */
  INLINE Kn_size sdistanceMin( Kn_sid p, uint k ) const;

  /**
   * Code coordinates in a digital space. It is in fact a pointel of the space.
   * @param x an array of size 'dimension()'.
   * @return the signed code of the corresponding pointel.
   */
  INLINE Kn_sid scodeCoords( const Kn_size* x ) const;

  /**
   * Add the coded positive coordinates [coord] (ie a pointel) to [p]. 
   * NB: you can go out of the space.
   * @param p any cell.
   * @param coord any pointel.
   * @return the unsigned code of the cell [p] translated by [coord].
   */
  INLINE Kn_sid stranslatePos( Kn_sid p, Kn_sid coord ) const;

  /**
   * Get the coded bound of [p] in the [k]th direction constrained by
   * [bound].
   * @param p any cell.
   * @param bound the element acting as bound.
   * @param k the concerned coordinate.
   * @return the unsigned code of the bound of [p].
   */
  INLINE Kn_sid sgetBound( Kn_sid p, Kn_sid bound, uint k ) const;

  /**
   * Get the coded bound of [p] in the 0th direction (x) constrained by
   * [bound]. (For speed-up)
   * @param p any cell.
   * @param bound the element acting as bound.
   * @return the unsigned code of the bound of [p].
   */
  INLINE Kn_sid sgetBoundX( Kn_sid p, Kn_sid bound ) const;

  /**
   * Increment the cell [p] to its next position (as classically done in
   * a scanning).
   * NB: also it is very generic, it is better to use a 'KnSpaceScanner' for
   * an efficient scanning (when you know the dimension of the space at compile
   * time.
   * Exemple: <pre> 
   * KnSpace kn( ... );
   * Kn_sid first, last; // lower and upper bounds 
   * Kn_sid p = first;
   * do 
   *   { // ... whatever [p] is the current cell
   *   }
   * while ( kn.sincrInBounds( p, first, last ) ); 
   * </pre>
   * @param p any cell.
   * @param lower the lower bound.
   * @param upper the upper bound.
   *
   * @return true if p is still within the bounds, false if the
   * scanning is finished.
   */
  INLINE bool sincrInBounds( Kn_sid & p, Kn_sid lower, Kn_sid upper ) const;



  // -------------------- Neighborhood services -------------------------------
public:
  
  /**
   * Computes the 1-neighborhood of the cell [cell] and add the cells
   * to the front or the back of [l].
   *
   * @param cell the unsigned cell of interest.
   * @param l (modified) the queue where neighboring cells are added.
   * @param front when 'true', add cells to the front, otherwise to the back.
   */
  void uneighborhood( Kn_uid cell, deque_ucells & l, bool front ) const;

  /**
   * Computes the 1-neighborhood of the cell [cell] and add the cells
   * to the front or the back of [l].
   *
   * @param cell the signed cell of interest.
   * @param l (modified) the queue where neighboring cells are added.
   * @param front when 'true', add cells to the front, otherwise to the back.
   */
  void sneighborhood( Kn_sid cell, deque_scells & l, bool front ) const;


  // -------------------- Geometry services ----------------------------------
public:
  /**
   * @param p any unsigned cell.
   * @return the coordinates of the centroid of the cell [p] in Rn.
   */
  INLINE Vector ucentroid( Kn_uid p ) const;
  /**
   * @param p any unsigned cell.
   * @param k any coordinate axis.
   * @return the k-coordinate of the centroid of the cell [p] in Rn.
   */
  INLINE float ucentroid( Kn_uid p, uint k ) const;
  /**
   * Returns in [v] the coordinates of the centroid of the cell [p] in Rn.
   * @param p any unsigned cell.
   * @param v any vector of size 'dim()'.
   */
  INLINE void ucentroid( Kn_uid p, Vector & v ) const;
  /**
   * @param p any signed cell.
   * @return the coordinates of the centroid of the cell [p] in Rn.
   */
  INLINE Vector scentroid( Kn_sid p ) const;
  /**
   * @param p any signed cell.
   * @param k any coordinate axis.
   * @return the k-coordinate of the centroid of the cell [p] in Rn.
   */
  INLINE float scentroid( Kn_sid p, uint k ) const;
  /**
   * Returns in [v] the coordinates of the centroid of the cell [p] in Rn.
   * @param p any signed cell.
   * @param v any vector of size 'dim()'.
   */
  INLINE void scentroid( Kn_sid p, Vector & v ) const;
  /**
   * Returns the basis of vectors that forms the geometry of the cell [p].
   * To get all base vectors, just keep one coordinate different from 0, and
   * sets the other to zero.
   * NB: if [p] is a 1-cell, then the base is constituted of only one vector,
   * which is the returned one.
   * @param p any unsigned cell.
   * @return a vector coding the vector basis.
   */
  INLINE const Vector & uvectorBasis( Kn_uid p ) const;
  /**
   * Returns the basis of vectors orthogonal to the one that forms the geometry
   * of the cell [p].
   * To get all orthogonal base vectors, just keep one coordinate different
   * from 0, and sets the other to zero.
   * NB: if [p] is a n-1-cell, then the base is constituted of only one vector,
   * which is the returned one. (ususally called the "normal" of the cell.
   * @param p any unsigned cell.
   * @return a vector coding the vector basis.
   */
  INLINE const Vector & uorthVectorBasis( Kn_uid p ) const;
  /**
   * Returns the basis of vectors that forms the geometry of the cell [p].
   * To get all base vectors, just keep one coordinate different from 0, and
   * sets the other to zero.
   * NB: if [p] is a 1-cell, then the base is constituted of only one vector,
   * which is the returned one.
   * @param p any signed cell.
   * @return a vector coding the vector basis.
   */
  INLINE const Vector & svectorBasis( Kn_sid p ) const;
  /**
   * Returns the basis of vectors orthogonal to the one that forms the geometry
   * of the cell [p].
   * To get all orthogonal base vectors, just keep one coordinate different
   * from 0, and sets the other to zero.
   * NB: if [p] is a n-1-cell, then the base is constituted of only one vector,
   * which is the returned one. (ususally called the "normal" of the cell.
   * @param p any signed cell.
   * @return a vector coding the vector basis.
   */
  INLINE const Vector & sorthVectorBasis( Kn_sid p ) const;
  /**
   * Returns the normal vector of the surfel [b] pointing in the direct 
   * orientation (usually inside).
   * @param b any signed surfel.
   * @return a vector coding the vector basis.
   */
  INLINE Vector snormalInside( Kn_sid b ) const;
  /**
   * Returns the normal vector of the surfel [b] pointing in the direct 
   * orientation (usually inside).
   * @param p any signed surfel.
   * @param n (returns) a vector coding the vector basis.
   */
  INLINE void snormalInside( Kn_sid b, Vector & n ) const;

  // -------------------- Adjacency services ----------------------------------
public:
  /**
   * Two k-cells [p1] and [p2] are low-adjacent if they have in their 
   * intersection a cell of dimension l such that 0 <= k-l <= lvl.
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any unsigned cell.
   * @param p2 any unsigned cell of same type as [p1].
   * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
   * @return 'true' if the two cells are [lvl]-low adjacent.
   */
  bool uareLowAdjacent( Kn_uid p1, Kn_uid p2, uint & lvl ) const;
  /**
   * Two k-cells [p1] and [p2] are up-adjacent if they have in their 
   * intersection a cell of dimension l such that 0 <= l-k <= lvl.
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any unsigned cell.
   * @param p2 any unsigned cell of same type as [p1].
   * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
   * @return 'true' if the two cells are [lvl]-up adjacent.
   */
  bool uareUpAdjacent( Kn_uid p1, Kn_uid p2, uint & lvl ) const;
  /**
   * Two k-cells [p1] and [p2] with same type are adjacent if they have in
   * their intersection a cell of dimension l such that -lvl <= l-k <= lvl.
   * NB: if (l-k) is positive, it is an up-adjacency, otherwise low-adjacency.
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any unsigned cell.
   * @param p2 any unsigned cell of same type as [p1].
   * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
   * @return 'true' if the two cells are [lvl]-up adjacent.
   */
  bool uareAdjacent( Kn_uid p1, Kn_uid p2, uint & lvl ) const;

  /**
   * Two k-cells [p1] and [p2] are low-adjacent if they have in their 
   * intersection a cell of dimension l such that 0 <= k-l <= lvl.
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any signed cell.
   * @param p2 any signed cell of same type and sign as [p1].
   * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
   * @return 'true' if the two cells are [lvl]-low adjacent.
   */
  bool sareLowAdjacent( Kn_sid p1, Kn_sid p2, uint & lvl ) const;
  /**
   * Two k-cells [p1] and [p2] are up-adjacent if they have in their 
   * intersection a cell of dimension l such that 0 <= l-k <= lvl.
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any signed cell.
   * @param p2 any signed cell of same type and sign as [p1].
   * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
   * @return 'true' if the two cells are [lvl]-up adjacent.
   */
  bool sareUpAdjacent( Kn_sid p1, Kn_sid p2, uint & lvl ) const;
  /**
   * Two k-cells [p1] and [p2] with same type are adjacent if they have in
   * their intersection a cell of dimension l such that -lvl <= l-k <= lvl.
   * NB: if (l-k) is positive, it is an up-adjacency, otherwise low-adjacency.
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any signed cell.
   * @param p2 any signed cell of same type and sign as [p1].
   * @param lvl (returns) the level of the adjacency ( 1 = direct adjacency ).
   * @return 'true' if the two cells are [lvl]-up adjacent.
   */
  bool sareAdjacent( Kn_sid p1, Kn_sid p2, uint & lvl ) const;
 
  /**
   * Given a cell [p], a coordinate [k], and a positive or negative
   * displacement [pos], return the adjacent (up or low) cell of same type
   * as [p] in this direction. <p>
   * NB: You may go out of the space.
   * @param p any unsigned cell.
   * @param k any coordinate.
   * @param pos 'true' is positive along coordinate, 'false' negative.
   * @return the cell adjacent to [p] along [pos][k].
   */
  INLINE Kn_uid uadjacent( Kn_uid p, uint k, bool pos ) const;

  /**
   * Given a cell [p], a coordinate [k], and a positive or negative
   * displacement [pos], return the adjacent (up or low) cell of same type
   * as [p] in this direction. <p>
   * NB: You may go out of the space.
   * @param p any signed cell.
   * @param k any coordinate.
   * @param pos 'true' is positive along coordinate, 'false' negative.
   * @return the cell adjacent to [p] along [pos][k].
   */
  INLINE Kn_sid sadjacent( Kn_sid p, uint k, bool pos ) const;


  // -------------------- Incidence services ----------------------------------
public:

  /**
   * A k-cell [p1] and a l-cell [p2] are incident if one is a face of the
   * other.
   * NB: if (k-l) is positive, [p1] has [p2] in its lower-incidence
   * and [p2] has [p1] in its upper-incidence.
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any unsigned cell.
   * @param p2 any unsigned cell.
   * @param lvl (returns) the level of the incidency ( dim(k)-dim(l) ).
   * @return 'true' if the two cells are [lvl]-incident.
   */
  bool uareIncidentComputation( Kn_uid p1, Kn_uid p2, int & lvl ) const;
  
  /**
   * A l-cell [p2] is in the low-incidence of a k-cell [p1] if [p2] is a face
   * of [p1].
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any unsigned cell.
   * @param p2 any unsigned cell.
   * @param lvl (returns) the level of the incidency ( dim(k)-dim(l) ).
   * @return 'true' if [p2] is a face of [p1] ([p2] [lvl]-low-incident to [p1])
   */
  bool uisInLowIncidenceComputation( Kn_uid p1, Kn_uid p2, int & lvl ) const;

  /**
   * A l-cell [p2] is in the up-incidence of a k-cell [p1] if [p1] is a face
   * of [p2].
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any unsigned cell.
   * @param p2 any unsigned cell.
   * @param lvl (returns) the level of the incidency ( dim(l)-dim(k) ).
   * @return 'true' if [p1] is a face of [p2] ([p2] [lvl]-up-incident to [p1])
   */
  bool uisInUpIncidenceComputation( Kn_uid p1, Kn_uid p2, int & lvl ) const;

  /**
   * A k-cell [p1] and a l-cell [p2] are incident if one is a face of the
   * other.
   * NB: if (k-l) is positive, [p1] has [p2] in its lower-incidence
   * and [p2] has [p1] in its upper-incidence.
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any unsigned cell.
   * @param p2 any unsigned cell.
   * @param lvl (returns) the level of the incidency ( dim(k)-dim(l) ).
   * @return 'true' if the two cells are [lvl]-incident.
   */
  INLINE bool uareIncident( Kn_uid p1, Kn_uid p2, int & lvl ) const;
  
  /**
   * A l-cell [p2] is in the low-incidence of a k-cell [p1] if [p2] is a face
   * of [p1].
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * @param p1 any unsigned cell.
   * @param p2 any unsigned cell.
   * @param lvl (returns) the level of the incidency ( dim(k)-dim(l) ).
   * @return 'true' if [p2] is a face of [p1] ([p2] [lvl]-low-incident to [p1])
   */
  INLINE bool uisInLowIncidence( Kn_uid p1, Kn_uid p2, int & lvl ) const;

  /**
   * A l-cell [p2] is in the up-incidence of a k-cell [p1] if [p1] is a face
   * of [p2].
   * NB: '[lvl] == 0' implies that p1 and p2 are equal.
   * TODO: we could use a log2(nb_incident) algorithm to check if [p1] and [p2]
   * are incident. nb_incident is bounded by 2^dim().
   * @param p1 any unsigned cell.
   * @param p2 any unsigned cell.
   * @param lvl (returns) the level of the up-incidency ( dim(l)-dim(k) ).
   * @return 'true' if [p1] is a face of [p2] ([p2] [lvl]-up-incident to [p1])
   */
  INLINE bool uisInUpIncidence( Kn_uid p1, Kn_uid p2, int & lvl ) const;
  
  /**
   * Given a cell [p], a coordinate [k], and a positive or negative
   * displacement [pos], return the 1-low-incident cell to [p].
   * The positive incident has a [k]-coordinate with +1 compared to [p],
   * the negative has the same coordinates as [p]. 
   * <p>
   * NB: You may go out of the space.
   * @param p any unsigned cell.
   * @param k any coordinate.
   * @param pos 'true' is positive along coordinate, 'false' negative.
   * @return the cell 1-low-incident to [p] along [pos][k].
   */
  INLINE Kn_uid ulowIncident( Kn_uid p, uint k, bool pos ) const;

  /**
   * Given a cell [p], a coordinate [k], and a positive or negative
   * displacement [pos], return the 1-up-incident cell to [p].
   * The negative incident has a [k]-coordinate with -1 compared to [p],
   * the positive has the same coordinates as [p]. 
   * <p>
   * NB: You may go out of the space.
   * @param p any unsigned cell.
   * @param k any coordinate.
   * @param pos 'true' is positive along coordinate, 'false' negative.
   * @return the cell 1-up-incident to [p] along [pos][k].
   */
  INLINE Kn_uid uupIncident( Kn_uid p, uint k, bool pos ) const;

  /**
   * Given a cell [p], a coordinate [k], and a positive or negative
   * displacement [pos], return the 1-incident cell to [p]
   * in this direction. The negative incident has a [k]-coordinate 
   * with -1 compared to [p], the positive has the same coordinates as [p]. 
   * <p>
   * NB: You may go out of the space.
   * @param p any unsigned cell.
   * @param k any coordinate.
   * @param pos 'true' is positive along coordinate, 'false' negative.
   * @return the cell 1-incident to [p] along [pos][k].
   */
  INLINE Kn_uid uincident( Kn_uid p, uint k, bool pos ) const;

  /**
   * Given a signed cell [p], a coordinate [k], and a positive or negative
   * displacement [pos], return the 1-low-incident cell to [p].
   * The positive incident has a [k]-coordinate with +1 compared to [p],
   * the negative has the same coordinates as [p]. 
   * <p>
   * NB: You may go out of the space.
   * @param p any signed cell.
   * @param k any coordinate.
   * @param pos 'true' is positive along coordinate, 'false' negative.
   * @return the cell 1-low-incident to [p] along [pos][k].
   */
  INLINE Kn_sid slowIncident( Kn_sid p, uint k, bool pos ) const;
  /**
   * Given a signed cell [p], a coordinate [k], and a positive or negative
   * displacement [pos], return the 1-up-incident cell to [p].
   * The negative incident has a [k]-coordinate with -1 compared to [p],
   * the positive has the same coordinates as [p]. 
   * <p>
   * NB: You may go out of the space.
   * @param p any signed cell.
   * @param k any coordinate.
   * @param pos 'true' is positive along coordinate, 'false' negative.
   * @return the cell 1-up-incident to [p] along [pos][k].
   */
  INLINE Kn_sid supIncident( Kn_sid p, uint k, bool pos ) const;
  /**
   * Given a signed cell [p], a coordinate [k], and a positive or negative
   * displacement [pos], return the signed 1-incident cell to [p] (up or low).
   * The positive incident has a [k]-coordinate +1(low)+0(up) compared to [p],
   * The negative incident has a [k]-coordinate +0(low)-1(up) compared to [p],
   * <p>
   * NB: You may go out of the space.
   * @param p any signed cell.
   * @param k any coordinate.
   * @param pos 'true' is positive along coordinate, 'false' negative.
   * @return the cell 1-incident to [p] along [pos][k].
   */
  INLINE Kn_sid sincident( Kn_sid p, uint k, bool pos ) const;
  
  /**
   * Return 'true' if the direct orientation of [p] along [k] is in the
   * positive coordinate direction. The direct orientation in a direction
   * allows to go from positive incident cells to positive incident cells.
   * This means that <code>
   * ks.decodeSign( ks.sincident( p, k, ks.sdirect( p, k ) ) )
   * == KnTypes::POS </code> 
   * is always true.
   * @param p any signed cell.
   * @param k any coordinate.
   * @return the direct orientation of [p] along [k].
   */
  INLINE bool sdirect( Kn_sid p, uint k ) const;
  


  // ------------------------- Interface --------------------------------------
public:
  /**
   * Writes/Displays an element on an output stream.
   * @param p the (unsigned) element to display.
   * @param that_stream the output stream where the object is written.
   */
  void displayKn_uid( Kn_uid p,
		      std::ostream & that_stream = std::cout ) const;
  /**
   * Writes/Displays an element on an output stream.
   * @param p the (signed) element to display.
   * @param that_stream the output stream where the object is written.
   */
  void displayKn_sid( Kn_sid p,
		      std::ostream & that_stream = std::cout ) const;
  /**
   * Writes/Displays the fields of a Kn_uid.
   * @param that_stream the output stream where the object is written.
   */
  void displayKn_uid_fields( std::ostream & that_stream = std::cout ) const;
  /**
   * Writes/Displays the fields of a Kn_sid.
   * @param that_stream the output stream where the object is written.
   */
  void displayKn_sid_fields( std::ostream & that_stream = std::cout ) const;
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


  
  // ------------------------- Datas ------------------------------------------
protected:
  /**
   * Dimension of the space. If equal to 0, space is not initialized.
   */
  uint m_dim;
  /**
   * Size of this dimension. It is also one more than the maximum coordinate
   * allowed.
   */
  Kn_size* m_size;
  /**
   * Size of the space, ie its number of spels.
   */
  Kn_size m_nb_spels;
  /**
   * Number of bits used to represent an element of Kn in this space.
   */
  uint m_nb_bits;
  /**
   * Number of types of cells.
   */
  uint m_nb_celltypes;  
  /**
   * It is the coded value of the maxima in all dimensions as a Kn_uid.
   */
  Kn_uid m_uid_coded_maxs;
  /**
   * For each coordinate, it is the coded value of the maximum.
   * Ex: if the y-coordinate is shift by 8 and has maximum 10, then
   * m_uid_coded_max[ 1 ] = &0x00000a00
   * NB: the coded value of the minimum is always 0x00000000.
   */
  Kn_uid* m_uid_coded_max;
  /**
   * For each coordinate, it is the coded value of the positive increment.
   * Ex: if the y-coordinate is shift by 8, then
   * m_uid_coded_incr[ 1 ] = &0x00000100
   */
  Kn_uid* m_uid_coded_incr;
  /**
   * For each possible cell type, it is the coded value of a positive 
   * increment in each coordinate set to 1 of the cell type. 
   * Ex: if the y-coordinate is shift by 8, then
   * m_uid_coded_celltype_incr[ 3 ] = &0x00000101
   */
  Kn_uid* m_uid_coded_celltype_incr;
  /**
   * This array stores the different coded vertices of any cell-type. 
   * For instance, for the code 5 (101), then there are four possible
   * vertices: 0x00000000, 0x00000001, 0x00010000, 0x00010001
   * if x-coordinate and y-coordinate take each 8 bits.
   * Note that you should shift the code by '<< dim()' to access this array.
   * To get the number of vertices, just use 'KnUtils::countSetBits'.
   * @see KnUtils::countSetBits
   */
  Kn_uid* m_uid_coded_coords_vtx;
  /**
   * It is the coded value of the maxima in all dimensions as a Kn_sid.
   */
  Kn_sid m_sid_coded_maxs;
  /**
   * For each coordinate, it is the coded value of the maximum.
   * Ex: if the y-coordinate is shift by 8 and has maximum 10, then
   * m_sid_coded_max[ 1 ] = &0x00000a00
   * NB: the coded value of the minimum is always 0x00000000.
   */
  Kn_sid* m_sid_coded_max;
  /**
   * For each coordinate, it is the coded value of the positive increment.
   * Ex: if the y-coordinate is shift by 8, then
   * m_sid_coded_incr[ 1 ] = &0x00000100
   */
  Kn_sid* m_sid_coded_incr;
  /**
   * For each possible cell type, it is the coded value of a positive 
   * increment in each coordinate set to 1 of the cell type. 
   * Ex: if the y-coordinate is shift by 8, then
   * m_sid_coded_celltype_incr[ 3 ] = &0x00000101
   */
  Kn_sid* m_sid_coded_celltype_incr;
  /**
   * This array stores the different coded vertices of any cell-type. 
   * For instance, for the code 5 (101), then there are four possible
   * vertices: 0x00000000, 0x00000001, 0x00010000, 0x00010001
   * if x-coordinate and y-coordinate take each 8 bits.
   * Note that you should shift the code by '<< dim()' to access this array.
   * To get the number of vertices, just use 'KnUtils::countSetBits'.
   * @see KnUtils::countSetBits
   */
  Kn_sid* m_sid_coded_coords_vtx;

  /**
   * Field for selecting the coordinates field of a Kn_uid.
   * looks like: 0x00008fff for a 256x128 image
   */
  Kn_uid_Field m_uid_coords;
  /**
   * Field for selecting the coordinates field of a Kn_sid.
   * looks like: 0x00008fff for a 256x128 image
   */
  Kn_sid_Field m_sid_coords;
  /**
   * Field for selecting the coordinates field of a Kn_sid and its sign.
   * looks like: 0x0000ffff for a 256x128 image
   */
  Kn_sid_Field m_sid_sign_coords;

  /**
   * Field array to reach the k-th coordinate within a Kn_uid.
   */
  Kn_uid_Field* m_uid_coord;
  /**
   * Field array to reach the k-th coordinate within a Kn_sid.
   */
  Kn_sid_Field* m_sid_coord;

  /**
   * Field to reach the sign in a Kn_sid.
   */
  Kn_sid_Field m_sid_sign;
  /**
   * Field to reach all directions and the sign in a Kn_sid.
   */
  Kn_sid_Field m_sid_dirs_and_sign;

  /**
   * Field to select all directions in a Kn_uid.
   */
  Kn_uid_Field m_uid_dirs;
  /**
   * Field to select all directions in a Kn_sid.
   */
  Kn_sid_Field m_sid_dirs;

  /**
   * Field array to select one direction in a Kn_uid.
   */
  Kn_uid_Field* m_uid_dir;
  /**
   * Field array to select one direction in a Kn_uid.
   */
  Kn_sid_Field* m_sid_dir;

  // ------------------------- Vector attributes ------------------------------
  /**
   * An array of vector which represents the different vector basis 
   * for each type of cell. Any vector formed with linear combinations of
   * vector of the basis has a null scalar product with any vector of the
   * orthogonal basis.
   */
  Vector* m_basis;
  /**
   * An array of vector which represents the different orthogonal vector basis 
   * for each type of cell.  Any vector formed with linear combinations of
   * vector of the orthogonal basis has a null scalar product with any vector
   * of the basis.
   */
  Vector* m_orth_basis;

  // ------------------------- Incidence attributes ---------------------------
private:
  /**
   * LUT to store if two cells are low-incident or not (size (1<<2*dim())).
   * Given two cells [p1] and [p2] of cell type [t1] and [t2], then
   * m_low_incident[ t1 << (1<<dim()) + t2 ] stores a boolean.
   * If 'false' then they cannot be low incident.
   * Otherwise, you should check the table 'm_uid_coded_coords_vtx' to get
   * the different possible coordinate increments where the bit of the code
   * is 1. 
   */
  bool* m_low_incident;
  /**
   * LUT to store if two cells are up-incident or not (size (1<<2*dim())).
   * Given two cells [p1] and [p2] of cell type [t1] and [t2], then
   * m_up_incident[ t1 << (1<<dim()) + t2 ] stores a boolean.
   * If 'false' then they cannot be up incident.
   * Otherwise, you should check the table 'm_uid_coded_coords_vtx' to get
   * the different possible coordinate increments where the bit of the code
   * is 1. 
   */
  bool* m_up_incident;
  /**
   * Array of size (1<<dim())*dim(). For a given unsigned cell-type [c] and a
   * given direction [k], stores the sign of the permutation of coordinates to 
   * get back [c]. 'true' is +1, 'false' is -1. 
   * Useful to compute sign of incidence.
   * Exemples: <pre>
   * if [c]=zyx and [k]=y then [c]-[k]=zx and sign(zyx,y.zx) = -1 (1 perm)
   * if [c]=zyx and [k]=x then [c]-[k]=zy and sign(zyx,x.zy) = +1 (2 perms)
   * if [c]=zyx and [k]=x then [c]-[k]=zy and sign(zyx,x.zy) = +1 (0 perms)
   * if [c]=tzyx and [k]=y then [c]-[k]=tzx and sign(tzyx,y.tzx) = -1 (2 perms)
   * To get the index in the array: m_permutation[ ( k << n ) + c ]
   * </pre>
   */
  bool* m_permutation;
  /**
   * Array of size (1<<(dim()+1)*dim(). For a given signed cell-type [c] and
   * a given direction [k], stores the signed cell-type of the first incident.
   * It is a LUT for computing 1-incident cells.
   * To get the index in the array: m_permutation[ ( k << (n+1) ) + c ]
   * @see m_sid_incident2
   */
  Kn_sid* m_sid_incident1;
  /**
   * Array of size (1<<(dim()+1)*dim(). For a given signed cell-type [c] and
   * a given direction [k], stores the signed cell-type of the second incident.
   * It is a LUT for computing 1-incident cells.
   * To get the index in the array: m_permutation[ ( k << (n+1) ) + c ]
   * @see m_sid_incident1
   */
  Kn_sid* m_sid_incident2;
  /**
   * Array of size (1<<(dim()+1)^2. For two given signed cell-type [c1] and
   * [c2], stores the low incidence value (c1:c2). Note that this array is a
   * very sparse matrix, since this value may be different from 0 only if 
   * c2 is one-dimension less than c1.
   * It is a LUT for computing incidence matrices.
   * To get the index in the array: m_permutation[ ( c1 << (n+1) ) + c2 ]
   */
  int8* m_sid_low_incidence_matrix;
  /**
   * Array of size (1<<(dim()+1)*dim(). For a given sign cell-type [c] and a
   * given direction [k], stores 'true' if the direct orientation means a
   * positive coordinate displacement, 'false' otherwise. The direct 
   * orientation is the one where the 1-incident cell is positive.
   */
  bool* m_sid_direct_orientation;
  
  // ------------------------- Sanitizer accessor ---------------------------
public:
  /**
   * Returns a reference to the proper bitset sanitizer depending on whether
   * or not the set is composed of oriented cells or not and on whether or not
   * the set is composed of cells with different topology.
   * @param oriented if 'true' the bitset will be associated to a set of oriented cells, otherwise it will be associated to a set of unoriented cells.
   * @param mixed if 'true' the bitset will be associated to a set of cells with different topologies, otherwise the bitset will be associated to a set of cells with the same topology.
   * @return the reference to the proper sanitizer.
   * NB: Internal use of 'KnRCellSet' and 'KnCharSet'.
   * @see Bitset1,KnRCellSet,KnCharSet
   */
  const Shared<Sanitizer> & getSanitizer( bool oriented,
					  bool mixed ) const;
  
  // ------------------------- Sanitizer attributes ---------------------------
private:
  /**
   * Sanitizer for all sets composed of a single type of unsigned cell.
   */
  Shared<Sanitizer>* m_sanitizer_set_unsigned_single_cell;
  /**
   * Sanitizer for all sets composed of all types of unsigned cells.
   */
  Shared<Sanitizer>* m_sanitizer_set_unsigned_mixed_cells;
  /**
   * Sanitizer for all sets composed of a single type of signed cell.
   */
  Shared<Sanitizer>* m_sanitizer_set_signed_single_cell;
  /**
   * Sanitizer for all sets composed of all types of signed cells.
   */
  Shared<Sanitizer>* m_sanitizer_set_signed_mixed_cells;
  

  // --------- Combinatorial and restricted cell codes attributes -------------
private:
  
  /**
   * Array of binomials (n r), with n=dim() and r=0..n
   */
  uint* m_binomials;

  /**
   * LUT to transform unsigned cells into restricted cells (topology is
   * minimized so that mappings from r-cells are less memory costly).
   * m_urcode[ k.utopology( c ) ] returns the transformed topology of c.
   */
  Kn_urid* m_urcode;

  /**
   * LUT to transform signed cells into restricted cells (topology is
   * minimized so that mappings from r-cells are less memory costly).
   * m_srcode[ k.stopology( c ) ] returns the transformed topology of c.
   */
  Kn_srid* m_srcode;

  /**
   * LUT to transform unsigned restricted cells into cells (topology is
   * expanded back. It is a 2d array (needs cell dimension).
   * m_urdecode[r][ k.utopology( rc ) ] returns the code of rc 
   * if rc is a r-cell.
   */
  Kn_uid** m_urdecode;

  /**
   * LUT to transform signed restricted cells into cells (topology is
   * expanded back. It is a 2d array (needs cell dimension).
   * m_srdecode[r][ k.stopology( rc ) ] returns the code of rc 
   * if rc is a r-cell.
   */
  Kn_sid** m_srdecode;
  

  // ------------------------- Hidden services --------------------------------
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  inline KnSpace( const KnSpace & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  inline KnSpace & operator=( const KnSpace & other );
  
  // ------------------------- Internals --------------------------------------
private:
  /**
   * Internal method to compute masks from dimension and given sizes.
   * @param n is the dimension of the space.
   * @param sizes is an array containing the sizes along each dimension (sizes of the digital space).
   */
  bool computeSizeAndFields( uint n, const Kn_size* sizes );

  /**
   * Internal methods to reset all mask attributes.
   * Should be called at construction or after freeing masks.
   */
  void resetSizeAndFields();

  /**
   * Internal method to alloc memory for masks.
   * @param n is the dimension of the space.
   */
  void allocSizeAndFields( uint n );

  /**
   * Internal methods to dealloc memory for masks.
   */
  void freeSizeAndFields();

  /**
   * Internal methods to reset all vector attributes.
   * Should be called at construction or after freeing vectors.
   */
  void resetVectors();

  /**
   * Internal methods to alloc memory for vectors.
   * @param n is the dimension of the space.
   */
  void allocVectors( uint n );

  /**
   * Internal methods to dealloc memory for masks.
   */
  void freeVectors();
  
  /**
   * Internal methods to compute vectors from the space dimension.
   * @param n is the dimension of the space.
   * @return 'true'
   */
  bool computeVectors( uint n );

  /**
   * Internal methods to compute incidence LUT from dimension and given sizes.
   * NB: is called by 'computeSizeAndFields'.
   * @param n is the dimension of the space.
   */
  bool computeIncidenceLUT( uint n );
  
  /**
   * Internal methods to compute LUT for sign of permutations from dimension.
   * Useful to compute the signed incidence and the boundary operators.
   * NB: is called by 'computeSizeAndFields'.
   * @param n is the dimension of the space.
   */
  bool computePermutationLUT( uint n );

  /**
   * Internal methods to reset all sanitizers attributes.
   * Should be called at construction or after freeing sanitizers.
   */
  void resetSanitizers();

  /**
   * Internal methods to dealloc memory for sanitizers.
   */
  void freeSanitizers();

  /**
   * Internal methods to compute sanitizers for all kind of sets.
   * @param n is the dimension of the space.
   * @return 'true'
   */
  bool computeAndAllocSanitizers( uint n );

  /**
   * Internal methods to compute direct orientations for each cell-type.
   * Useful to digital surface tracking.
   * NB: is called by 'computeSizeAndFields'.
   * @param n is the dimension of the space.
   */
  bool computeDirectOrientationLUT( uint n );

  /**
   * Internal method to compute restricted cell codes LUT.
   * Useful to define mappings from r-cells (minimizes memory space).
   * @param n is the dimension of the space.
   */
  bool computeAndAllocRestrictedCellCodeLUT( uint n );

  /**
   * Internal method to reset restricted cell codes LUT.
   * Should be called at construction or after freeing sanitizers.
   */
  void resetRestrictedCellCodeLUT();

  /**
   * Internal method to dealloc memory for restricted cell codes LUT.
   */
  void freeRestrictedCellCodeLUT();

};

/**
 * Overloads 'operator<<' for displaying objects of class 'KnSpace'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'KnSpace' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream,
	    const KnSpace & that_object_to_display );

/**
 * Increment [CELL] in the [0]-direction (x).
 * NB: you can go out of the space.
 * @param CELL (modified) any cell.
 */
#define KnSpace_uincrX(CELL) ++CELL


} // namespace ImaGene
 
///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/KnSpace.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnSpace_h

#undef KnSpace_RECURSES
#endif // else defined(KnSpace_RECURSES)
