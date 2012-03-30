/** @file Lattice.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Lattice.h
//
// Creation : 2008/02/15
//
// Version : 2008/02/15
//
// Author : JOL
//
// Summary : Header file for files Lattice.ih and Lattice.cxx
//
// History :
//	2008/02/15 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Lattice declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Lattice_RECURSES)
#error Recursive header files inclusion detected in Lattice.h
#else // defined(Lattice_RECURSES)
#define Lattice_RECURSES

#if !defined Lattice_h
#define Lattice_h

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
  // class Lattice
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Lattice' <p>
   * Aim: Represents an n-dimensional lattice in an m-dimensional space.
   */
  class Lattice
  {

  public:
    /**
     * Integer coding for the lattice.
     */
    typedef int32 lattice_int;

    // ----------------------- Static services ------------------------------
  public:

    /**
     * Constructs a default 2D lattice centered at [x0], with vectors
     * (dh,0,0) and (0,dh,0).
     *
     * @param l (returns) the lattice
     * @param x0 the origin of the lattice embedding (a 3D point)
     * @param dh the grid step or length of each vector.
     */
    static void defaultZ2toZ3( Lattice & l, const double* x0, double dh = 1.0 ); 

    /**
     * Constructs a default 3D lattice centered at [x0], with vectors
     * (dh,0,0) and (0,dh,0) and (0,0,dh).
     *
     * @param l (returns) the lattice
     * @param x0 the origin of the lattice embedding (a 3D point)
     * @param dh the grid step or length of each vector.
     */
    static void defaultZ3toZ3( Lattice & l, const double* x0, double dh = 1.0 );

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~Lattice();

    /**
     * Constructor.
     */
    Lattice();

    /**
     * @return the dimension of the lattice space.
     */
    INLINE uint n() const;

    /**
     * @return the dimension of the target space.
     */
    INLINE uint m() const;

    /**
     * Defines the standard lattice when n=m, the standard injection
     * when n < m, and when m > n, vectors are overlapping.
     *
     * @param n the dimension of the lattice space
     * @param m the dimension of the target space where the lattice is represented.
     * @param dh the length of the elementary displacement.
     */
    void init( uint n, uint m, double dh = 1.0 );

    /**
     * Sets the origin of the space.
     * @param x is a vector of size m.
     */
    void setOrigin( const double* x );  

    /**
     * Sets the elementary vector along direction i
     * @param i is a coordinate between 0 and n-1.
     * @param v is a vector of size m.
     */
    void setVector( uint i, const double* v );  

    /**
     * Immerse a point of the lattice into the target space of dimension m.
     *
     * @param p the n coordinates of a point in the lattice.
     * @param x (returns) its m coordinates in the target space of dimension m.
     */
    void immerse( const lattice_int* p, double* x ) const;

    /**
     * Immerse a point of the lattice into the target space of dimension m.
     *
     * @param p the n coordinates of a point in the lattice.
     * @param x (returns) its m coordinates in the target space of dimension m.
     */
    void immerse( const lattice_int* p, float* x ) const;


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
     * Dimension of lattice space;
     */
    uint m_n;

    /**
     * Dimension of target space;
     */
    uint m_m;

    /**
     * Origin. Array of size m_m;
     */
    double* m_x0;

    /**
     * Vectors for each dimension. Array of size m_n times m_m;
     */
    double* m_v;

    // ------------------------- Hidden services ------------------------------

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE Lattice( const Lattice & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE Lattice & operator=( const Lattice & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'Lattice'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Lattice' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const Lattice & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/Lattice.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Lattice_h

#undef Lattice_RECURSES
#endif // else defined(Lattice_RECURSES)
