/** @file OctantChange.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : OctantChange.h
//
// Creation : 2010/02/11
//
// Version : 2010/02/11
//
// Author : JOL
//
// Summary : Header file for files OctantChange.ih and OctantChange.cxx
//
// History :
//	2010/02/11 : ?Name? : ?What?
//
// Rcs Id : "@(#)class OctantChange declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(OctantChange_RECURSES)
#error Recursive header files inclusion detected in OctantChange.h
#else // defined(OctantChange_RECURSES)
#define OctantChange_RECURSES

#if !defined OctantChange_h
#define OctantChange_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/base/Vector2i.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class OctantChange
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'OctantChange' <p> Aim: This class is used
   * to model an octant change. Generally it brings geometric object
   * in the first octant and vice-versa.
   */
  class OctantChange
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Constructor.
     * The user may indicate which symmetries the class should perform.
     * Default to nothing to do.
     *
     * @param sx0 when 'true', symmetry around x = 0.
     * @param sy0 when 'true', symmetry around y = 0.
     * @param sxy when 'true', symmetry around x = y.
     */
    OctantChange( bool sx0 = false, bool sy0 = false, bool sxy = false );

    /**
     * Destructor. 
     */
    ~OctantChange();

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    OctantChange( const OctantChange & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    OctantChange & operator=( const OctantChange & other );


    /**
     * Reinitializes the object to model an octant change from the
     * octant specified by vector [v] toward the first octant. A
     * vector (x,y) in the first octant then satisfies 0 <= y <= x.
     *
     * @param v any 2D vector.
     */
    void initByCastIntoFirstOctant( Vector2i v );

    /**
     * Transforms the vector [u] according to the current
     * transformation. If the object was initialised with
     * initByCastIntoFirstOctant( v ), then if [u] is in the same
     * octant as [v], then the transformed vector is in the first
     * octant.
     *
     * @param u any vector.
     * @return the transformation of [u].
     */
    Vector2i cast( Vector2i u ) const;

    /**
     * Transforms the vector [u] according to the current
     * transformation. If the object was initialised with
     * initByCastIntoFirstOctant( v ), then if [u] is in the first
     * octant, then the transformed vector is in the same octant as
     * [v].
     *
     * @param u any vector.
     * @return the reverse transformation of [u].
     */
    Vector2i castBack( Vector2i u ) const;

    /**
     * @return 'true' if this object makes a symmetry around x = y.
     */
    bool isSymXeqY() const;

    /**
     * @return 'true' if this object makes a symmetry around x = 0.
     */
    bool isSymXeq0() const;

    /**
     * @return 'true' if this object makes a symmetry around y = 0.
     */
    bool isSymYeq0() const;

    /**
     * @return 'true' if this object makes a direct transformation
     * (determinant is 1), 'false' otherwise (determinant is -1).
     */
    bool isDirect() const;

    /**
     * @return 'true' if this object makes an indirect transformation
     * (determinant is -1), 'false' otherwise (determinant is 1).
     */
    bool isIndirect() const;


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
     * Stores if one should make a symmetry around x = 0.
     */
    bool m_sym_x_eq_0;

    /**
     * Stores if one should make a symmetry around y = 0.
     */
    bool m_sym_y_eq_0;

    /**
     * Stores if one should make a symmetry around x = y.
     */
    bool m_sym_x_eq_y;

    // ------------------------- Hidden services ------------------------------
  protected:

  private:

  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'OctantChange'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'OctantChange' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const OctantChange & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/OctantChange.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined OctantChange_h

#undef OctantChange_RECURSES
#endif // else defined(OctantChange_RECURSES)
