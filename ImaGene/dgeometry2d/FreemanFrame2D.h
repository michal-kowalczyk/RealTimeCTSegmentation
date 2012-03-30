/** @file FreemanFrame2D.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : FreemanFrame2D.h
//
// Creation : 2005/04/20
//
// Version : 2005/04/20
//
// Author : JOL
//
// Summary : Header file for files FreemanFrame2D.ih and FreemanFrame2D.cxx
//
// History :
//	2005/04/20 : ?Name? : ?What?
//
// Rcs Id : "@(#)class FreemanFrame2D declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(FreemanFrame2D_RECURSES)
#error Recursive header files inclusion detected in FreemanFrame2D.h
#else // defined(FreemanFrame2D_RECURSES)
#define FreemanFrame2D_RECURSES

#if !defined FreemanFrame2D_h
#define FreemanFrame2D_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/Vector.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class FreemanFrame2D
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'FreemanFrame2D' <p> Aim: Gives methods to
   * transform geometric or vector quantities defined in a local frame
   * given by a Freeman move into the classical euclidean frame.
   */
  class FreemanFrame2D
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~FreemanFrame2D();

    /**
     * Constructor. The object is not valid.
     */
    INLINE FreemanFrame2D();
    
    /**
     * Initializes the frame with a Freeman move [fcode], 0 east, 1
     * north, 2 west, 3 south.
     * The Freeman move/code represents the local frame. The inside
     * lies to the left (ie. ccw) of the Freeman move.
     *
     * @param fc a 4-connected Freeman move/code.
     */
    INLINE void init( uint fc );


    // ------------------------- Geometric services ---------------------------
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

    /**
     * The Freeman move/code representing the local frame. The inside
     * lies to the left (ie. ccw) of the Freeman move.
     */
    uint m_fc;
    

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE FreemanFrame2D( const FreemanFrame2D & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE FreemanFrame2D & operator=( const FreemanFrame2D & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'FreemanFrame2D'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'FreemanFrame2D' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const FreemanFrame2D & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/FreemanFrame2D.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined FreemanFrame2D_h

#undef FreemanFrame2D_RECURSES
#endif // else defined(FreemanFrame2D_RECURSES)
