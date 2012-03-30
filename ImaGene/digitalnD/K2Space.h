/** @file K2Space.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : K2Space.h
//
// Creation : 2008/12/15
//
// Version : 2008/12/15
//
// Author : JOL
//
// Summary : Header file for files K2Space.ih and K2Space.cxx
//
// History :
//	2008/12/15 : ?Name? : ?What?
//
// Rcs Id : "@(#)class K2Space declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(K2Space_RECURSES)
#error Recursive header files inclusion detected in K2Space.h
#else // defined(K2Space_RECURSES)
#define K2Space_RECURSES

#if !defined K2Space_h
#define K2Space_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class K2Space
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'K2Space' <p> Aim: This class specializes a
 * 'KnSpace' to represent a 2D digital space. In 2D, a lot of
 * functions is simplified. Surfaces are just 1D contours. A surfel is
 * called a linel and has just a tangent direction and an orthogonal
 * direction.
 */
class K2Space : public KnSpace
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~K2Space();

  /**
   * Constructor.
   * @param width the size along the x-axis (or 0-axis)
   * @param height the size along the y-axis (or 1-axis)
   */
  K2Space( uint width, uint height );
  
  // ------------------------- Basic services ------------------------------
public:

  /**
   * @return the width of the space (number of pixels along x-axis).
   */
  INLINE uint width() const;
  /**
   * @return the height of the space (number of pixels along x-axis).
   */
  INLINE uint height() const;

  /**
   * Creates the pointel at coordinate (x,y) (which has Khalimsky
   * coordinates (2x,2y).
   *
   * @param x its x coordinate
   * @param y its y coordinate
   * @return the unsigned code of this pointel.
   */
  INLINE Kn_uid upointel( uint x, uint y ) const;

  /**
   * Creates the pointel at coordinate (x,y) (which has Khalimsky
   * coordinates (2x,2y), and assign to it an orientation.
   *
   * @param x its x coordinate
   * @param y its y coordinate
   * @param s the sign is Kn_Types::POS or Kn_Types::NEG.
   * @return the signed code of this pointel.
   */
  INLINE Kn_sid spointel( uint x, uint y, Kn_sign s ) const;

  /**
   * Creates the pixel at coordinate (x,y) (which has Khalimsky
   * coordinates (2x+1,2y+1).
   *
   * @param x its x coordinate
   * @param y its y coordinate
   * @return the unsigned code of this pixel.
   */
  INLINE Kn_uid upixel( uint x, uint y ) const;

  /**
   * Creates the pixel at coordinate (x,y) (which has Khalimsky
   * coordinates (2x+1,2y+1), and assign to it an orientation.
   *
   * @param x its x coordinate
   * @param y its y coordinate
   * @param s the sign is Kn_Types::POS or Kn_Types::NEG.
   * @return the signed code of this pixel.
   */
  INLINE Kn_sid spixel( uint x, uint y, Kn_sign s ) const;

  /**
   * Creates a linel whose base is (x,y) and directed according to the
   * Freeman code fc.
   * - 0: linel from (x,y) to (x+1,y)
   * - 1: linel from (x,y) to (x,y+1)
   * - 2: linel from (x,y) to (x-1,y)
   * - 3: linel from (x,y) to (x,y-1)
   *
   * @param x its x coordinate
   * @param y its y coordinate
   * @return the unsigned code of this linel.
   */
  INLINE Kn_uid ulinel( uint x, uint y, uint fc ) const;

  /**
   * Creates a linel whose base is (x,y) and directed according to the
   * Freeman code fc.
   * - 0: linel from (x,y) to (x+1,y)
   * - 1: linel from (x,y) to (x,y+1)
   * - 2: linel from (x,y) to (x-1,y)
   * - 3: linel from (x,y) to (x,y-1)
   * 
   * The linel is oriented positively in the direction given by the
   * Freeman code, meaning that the pointed pointel has positive
   * orientation in the lower boundary of this linel. For instance, l
   * = slinel(3,2,0) is the linel of Khalimsky coordinate (7,4), going
   * from pointels (3,2) to (4,2), and its lower boundary is the
   * positive pointel (4,2) and the negative pointel (3,2).
   *
   * @param x its x coordinate
   * @param y its y coordinate
   * @return the signed code of this linel.
   */
  INLINE Kn_sid slinel( uint x, uint y, uint fc ) const;

  /**
   * From the Khalimsky coordinates of a point in Kn, builds its unsigned code.
   * @param x its x coordinate
   * @param y its y coordinate
   * @return the unsigned code of this point.
   *
   * Example: ukcode( 4, 3 ) codes the vertical linel
   of coordinate (2,1).
   */
  INLINE Kn_uid ukcode( uint x, uint y ) const;

  /**
   * From the Khalimsky coordinates of a point in Kn, builds its signed code.
   * @param x its x coordinate
   * @param y its y coordinate
   * @param s the sign is Kn_Types::POS or Kn_Types::NEG.
   * @return the signed code of this point.
   *
   * Example: skcode( 4, 3, Kn_Types::POS ) codes the vertical linel
   of coordinate (2,1), with some orientation.
   */
  INLINE Kn_sid skcode( uint x, uint y, Kn_sign s ) const;

  // ------------------------- Cell decoding services -------------------------
public:
  /**
   * @param p any unsigned cell.
   * @return the x (or 0-th) (digital) coordinate of p
   */
  INLINE uint ux( Kn_uid p ) const;
  /**
   * @param p any unsigned cell.
   * @return the y (or 1-th) (digital) coordinate of p
   */
  INLINE uint uy( Kn_uid p ) const;
  /**
   * @param p any signed cell.
   * @return the x (or 0-th) (digital) coordinate of p
   */
  INLINE uint sx( Kn_sid p ) const;
  /**
   * @param p any signed cell.
   * @return the y (or 1-th) (digital) coordinate of p
   */
  INLINE uint sy( Kn_sid p ) const;

  // ------------------------- Cell topology services -------------------------
public:
  /**
   * @param p any unsigned cell.
   * @return the dimension of the cell [p].
   * NB: optimized in dimension 2.
   */
  INLINE uint udim( Kn_uid p ) const;

  /**
   * @param p any signed cell.
   * @return the dimension of the cell [p].
   * NB: optimized in dimension 2.
   */
  INLINE uint sdim( Kn_sid p ) const;

  /**
   * @param b any unsigned cell.
   * @return 'true' if [b] is a linel (spans one coordinate).
   */
  INLINE bool uisLinel( Kn_uid b ) const;

  /**
   * @param b any signed cell.
   * @return 'true' if [b] is a linel (spans one coordinate).
   */
  INLINE bool sisLinel( Kn_sid b ) const;

  /**
   * Given an unsigned linel [s], returns its orthogonal direction (ie,
   * the coordinate where the linel is closed).
   *
   * @param s an unsigned linel
   * @return the orthogonal direction of [s]
   */
  INLINE uint uorthDir( Kn_uid s ) const;

  /**
   * Given a signed linel [s], returns its orthogonal direction (ie,
   * the coordinate where the linel is closed).
   *
   * @param s a signed linel
   * @return the orthogonal direction of [s]
   */
  INLINE uint sorthDir( Kn_sid s ) const;

  /**
   * Given an unsigned linel [s], returns its tangent direction (ie,
   * the coordinate where the linel is open).
   *
   * @param s an unsigned linel
   * @return the tangent direction of [s]
   */
  INLINE uint utanDir( Kn_uid s ) const;

  /**
   * Given a signed linel [s], returns its tangent direction (ie,
   * the coordinate where the linel is open).
   *
   * @param s a signed linel
   * @return the tangent direction of [s]
   */
  INLINE uint stanDir( Kn_sid s ) const;


  // ------------------- Implementation of abstract services ------------------
public:

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
  
  // ------------------------- Datas ------------------------------------------
protected:
  /**
   * Stores some information for a given Freeman code.
   */
  struct FreemanCodeLUT {
    /**
     * The first unsigned linel in the digital space with this code.
     */
    Kn_uid ulinel;
    /**
     * The first signed linel in the digital space with this code.
     */
    Kn_sid slinel;
  };

  /**
   * LUT for speeding up Freeman code to linel conversion services.
   */
  FreemanCodeLUT m_fc_lut[ 4 ];

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   */
  INLINE K2Space();

  /**
   * Initializes some tables specific to KnSpace in 2D.
   * @param width the size along the x-axis (or 0-axis)
   * @param height the size along the y-axis (or 1-axis)
   * @see m_fc_lut
   */
  void init( uint width, uint height );

private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE K2Space( const K2Space & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE K2Space & operator=( const K2Space & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'K2Space'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'K2Space' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const K2Space & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/K2Space.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined K2Space_h

#undef K2Space_RECURSES
#endif // else defined(K2Space_RECURSES)
