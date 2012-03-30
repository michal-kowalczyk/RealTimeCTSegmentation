/** @file Frame2D.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Frame2D.h
//
// Creation : 2004/11/24
//
// Version : 2004/11/24
//
// Author : JOL
//
// Summary : Header file for files Frame2D.ih and Frame2D.cxx
//
// History :
//	2004/11/24 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Frame2D declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Frame2D_RECURSES)
#error Recursive header files inclusion detected in Frame2D.h
#else // defined(Frame2D_RECURSES)
#define Frame2D_RECURSES

#if !defined Frame2D_h
#define Frame2D_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
//#include "LinAlg/LinAlg2D/Vector2D.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/digitalnD/KnSpace.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene {
  
///////////////////////////////////////////////////////////////////////////////
// class Frame2D
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'Frame2D' <p> Aim: To represent a 2D plane
 * spanned by two axes in the nD space. Simplifies some geometric
 * computations linked to a surfel frame. A surfel frame is centered
 * on a surfel with first axis in the direction of the direct track
 * direction and second axis in the direction of the indirect orthogonal
 * direction (ie the exterior).
 */
class Frame2D
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~Frame2D();

  /**
   * Constructor. The object is not valid.
   */
  INLINE Frame2D();

  /**
   * Copy constructor.
   * @param other the object to clone.
   */
  INLINE Frame2D( const Frame2D & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   */
  INLINE Frame2D & operator=( const Frame2D & other );

  /**
   * @return the associated digital space.
   */
  KnSpace* space() const;

  /**
   * Initializes the frame with a digital space [ks], the first axis
   * [i] and the second axis [j].
   *
   * @param ks a pointer on a digital space.
   * @param i the first axis (ie "x").
   * @param j the second axis (ie "y").
   */
  void init( KnSpace* ks, uint i, uint j );

  /**
   * Most geometric computations of 'Frame2D' transforms
   * vectors/angles expressed in a surfel frame to the common
   * frame. This method sets the surfel frame.
   *
   * <pre> both the orthogonal direction of [s] and [track_dir] are
   * 'x()' or 'y()' </pre>
   *
   * @param s any surfel with 'orthDir( s )' equal to 'x()' or 'y()'.
   * @param track_dir a direction different from 'orthDir( s )' and
   * equal to 'x()' or 'y()'.
   */
  void setSurfelFrame( Kn_sid s, uint track_dir );

  //void setFrame2D( int x0, int y0, uint x_axis_code, uint y_axis_code );  

  /**
   * @return the first axis.
   */
  INLINE uint x() const;

  /**
   * @return the second axis.
   */
  INLINE uint y() const;
  

  // ------------------------- Geometric services -----------------------------
public:
  

  /**
   * Given an angle expressed in the current surfel frame, returns the
   * angle wrt axis 'x()'.
   *
   * @param angle any angle in radian.
   * @return the angle wrt axis 'x()'.
   */
  float angleToX( float angle ) const;

  /**
   * Given an angle expressed in the current surfel frame, returns a
   * unit vector: the direction expressed in the frame.
   *
   * @param angle any angle in radian.
   * @return the direction as a vector in the frame.
   */
  Vector2D direction( float angle ) const;
  
  /**
   * Given a vector expressed in the current surfel frame, returns the
   * corresponding vector expressed in the 2D frame.
   *
   * @param v any vector of the surfel frame.
   * @return the vector in the 2D frame.
   */
  Vector2D transform( const Vector2D & v ) const;

  /**
   * Given a point expressed in the current surfel frame, returns the
   * corresponding point expressed in the 2D frame.
   *
   * @param p any point of the surfel frame.
   * @return the point in the 2D frame.
   */
  Vector2D transformPoint( const Vector2D & p ) const;
  
  /**
   * Given a vector expressed in the current surfel frame, returns the
   * corresponding vector expressed in the 2D frame.
   *
   * @param v any vector of the surfel frame.
   * @return the vector in the 2D frame.
   */
  Vector2i transform( const Vector2i & v ) const;

  /**
   * Given a point expressed in the current surfel frame, returns the
   * corresponding point expressed in the 2D frame.
   *
   * @param p any point of the surfel frame.
   * @return the point in the 2D frame.
   */
  Vector2i transformPoint( const Vector2i & p ) const;
  

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
private:

  /**
   * The digital space in which the frame is embedded.
   */
  KnSpace* m_ks;
  
  /**
   * The first axis.
   */
  uint m_x;

  /**
   * The second axis.
   */
  uint m_y;
  
  /**
   * The surfel defining the base of the surfel frame.
   */
  Kn_sid m_surfel;

  /**
   * The direction defining the first axis of the surfel frame.
   */
  uint m_tdir;

  /**
   * The orthogonal direction to [m_surfel].
   */
  uint m_odir;

  /**
   * The positive direction along the chosen x-axis. (By convention
   * track direct orientation).
   */
  bool m_tpos;

  /**
   * The positive direction along the chosen y-axis. (By convention
   * orthogonal indirect orientation, ie toward exterior of object boundary.).
   */
  bool m_opos;

  /**
   * Tells if the direct orientation of the track direction is
   * positive or negative.
   */
  float m_tsign;

  /**
   * Tells if the direct orientation of the orthogonal direction is
   * positive or negative.
   */
  float m_osign;

  /**
   * The x()-coordinate of the origin of the frame.
   * Affects only point operations.
   */
  int m_x0;

  /**
   * The y()-coordinate of the origin of the frame.
   * Affects only point operations.
   */
  int m_y0;


  // ------------------------- Hidden services --------------------------------
protected:
private:
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'Frame2D'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'Frame2D' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const Frame2D & that_object_to_display );


} // namespace ImaGene



///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/Frame2D.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Frame2D_h

#undef Frame2D_RECURSES
#endif // else defined(Frame2D_RECURSES)
