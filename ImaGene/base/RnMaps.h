/** @file RnMaps.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : RnMaps.h
//
// Creation : 2003/07/24
//
// Version : 2003/07/24
//
// Author : JOL
//
// Summary : Header file for files RnMaps.ih and RnMaps.cxx
//
// History :
//	2003/07/24 : ?Name? : ?What?
//
// Rcs Id : "@(#)class RnMaps declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(RnMaps_RECURSES)
#error Recursive header files inclusion detected in RnMaps.h
#else // defined(RnMaps_RECURSES)
#define RnMaps_RECURSES

#if !defined RnMaps_h
#define RnMaps_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
// #include "LinAlg/LinAlg/Vector.hpp"
// #include "LinAlg/LinAlg/Matrix.hpp"
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Matrix.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class RnMapImp
///////////////////////////////////////////////////////////////////////////////


/** 
 * Description of class 'RnMapImp' <p>
 * Aim: Implementation abstract class for 'RnMap'.
 * Represents a mapping from a space Rn to a space Rm.
 * @see RnMap
 */
class RnMapImp
{
  // ------------------------- Standard services ------------------------------
public:
//   /**
//    * Destructor. 
//    */
//   virtual ~RnMapImp();
  
  /**
   * @return the dimension of mapping domain (0 is any).
   */
  virtual uint dimIn() const = 0;
  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  virtual uint dimOut() const = 0;
  
  /**
   * Abstract service. Clone service.
   * @return a dyn. alloc. clone of 'this'.
   */
  virtual RnMapImp* clone() const = 0;

  /**
   * Abstract service. Mapping operator.
   * @param x the in-vector
   * @return the out vector.
   */
  virtual Vector eval( const Vector & x ) const = 0;
  
};




///////////////////////////////////////////////////////////////////////////////
// class RnMap
///////////////////////////////////////////////////////////////////////////////

/** 
 * Description of class 'RnMap' <p>
 * Aim: Bridge class to represent any mapping from Rn to Rm. Uses 'RnMapImp'
 * hierarchy to build mappings.
 * @see RnMapImp
 */
class RnMap
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~RnMap();

  /**
   * Constructor. Creates the identity mapping.
   */
  RnMap();

  /**
   * Creates the mapping [f].
   * @param f the mapping.
   */
  RnMap( const RnMapImp & f );

  /**
   * Copy constructor.
   * @param other the object to copy.
   */
  RnMap( const RnMap & other );

  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference to 'this'.
   */
  RnMap& operator=( const RnMap & other );


  /**
   * @return a reference to the implementation of the map.
   */
  INLINE const RnMapImp& imp() const;
  
  
  // ------------------------- mapping services ------------------------------
public:

  /**
   * @return the dimension of mapping domain (0 is any).
   */
  INLINE uint dimIn() const;

  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  INLINE uint dimOut() const;
  
  /**
   * Mapping operator.
   * @param x the in-vector
   * @return the out vector.
   */
  INLINE Vector eval( const Vector & x ) const;


  // ------------------------- operator services ------------------------------
public:

  /**
   * Composition operator.
   * @param g any mapping.
   * @return the mapping 'this(g(x))'.
   */
  RnMap operator()( const RnMap & g ) const;

  /**
   * Translation operator.
   * @param g any mapping.
   * @return the mapping 'this(x) + v'.
   */
  RnMap operator+( const Vector & v ) const;

  
  // ------------------------- static services ------------------------------
public:

  /**
   * Defines the identity mapping (of any dimension).
   * @return the corresponding mapping.
   */
  static RnMap makeIdentity();

  /**
   * Defines a linear mapping through its matrix.
   * @param m the matrix defining the mapping.
   * @return the corresponding mapping.
   */
  static RnMap makeLinearMap( const Matrix & m );

  /**
   * Defines a scaling linear mapping through a diagonal matrix.
   * @param v the vector defining the mapping.
   * @return the corresponding mapping.
   */
  static RnMap makeScalingMap( const Vector & v );

  /**
   * Defines a translation mapping.
   * @param v the vector defining the mapping.
   * @return the corresponding mapping.
   */
  static RnMap makeTranslationMap( const Vector & v );
  
  /**
   * Defines the implicit mapping from R^center.size() to R defining
   * a centered sphere (for 0, inside is negative, outside positive).
   * @param r the radius of the sphere.
   * @return the corresponding mapping.
   */
  static RnMap makeImplicitSphere( float r );
  
  /**
   * Defines the implicit mapping from R^center.size() to R defining
   * a sphere (for 0, inside is negative, outside positive).
   * @param r the radius of the sphere.
   * @param center the center of the sphere.
   * @return the corresponding mapping.
   */
  static RnMap makeImplicitSphere( float r, const Vector & center );

  /**
   * Defines the implicit mapping from R^n.size() to R defining
   * a half-space (for 0, inside is negative, outside positive).
   * @param n the normal to the plane pointing outward.
   * @return the corresponding mapping.
   */
  static RnMap makeImplicitHalfSpace( const Vector & n );

  /**
   * Defines a 3D rotation mapping along the x-axis.
   * @param theta the angle of the rotation (in radian).
   * @return the corresponding mapping.
   */
  static RnMap make3DRotationX( float theta );

  /**
   * Defines a 3D rotation mapping along the y-axis.
   * @param theta the angle of the rotation (in radian).
   * @return the corresponding mapping.
   */
  static RnMap make3DRotationY( float theta );

  /**
   * Defines a 3D rotation mapping along the z-axis.
   * @param theta the angle of the rotation (in radian).
   * @return the corresponding mapping.
   */
  static RnMap make3DRotationZ( float theta );

  /**
   * Minimum operator.
   * @param f any mapping.
   * @param g any mapping.
   * @return the mapping 'min(f(x),g(x))'
   */
  static RnMap min( const RnMap & f, const RnMap & g );
  
  /**
   * Maximum operator.
   * @param f any mapping.
   * @param g any mapping.
   * @return the mapping 'min(f(x),g(x))'
   */
  static RnMap max( const RnMap & f, const RnMap & g );


  /**
   * Negation operator.
   * @param f any mapping.
   * @return the mapping '-f(x)'
   */
  static RnMap neg( const RnMap & f );

  /**
   * Defines a 3D translation mapping.
   * @param x0 the x-displacement defining the mapping.
   * @param y0 the x-displacement defining the mapping.
   * @param z0 the x-displacement defining the mapping.
   * @return the corresponding mapping.
   */
  INLINE
  static RnMap makeTranslation3D( float x0, float y0, float z0 );
  
  /**
   * Defines a 3D ellipsoid aligned with axes.
   * @param a the half-radius along the x-coordinate.
   * @param b the half-radius along the y-coordinate.
   * @param c the half-radius along the z-coordinate.
   * @return the implicit mapping of the ellipsoid.
   */
  INLINE
  static RnMap makeEllipsoid3D( float a, float b, float c );

  /**
   * Defines a half-space going through <x0,y0,z0> with normal <a,b,c>
   *
   * @param x0 the x-position of some point of the half-space.
   * @param y0 the y-position of some point of the half-space.
   * @param z0 the z-position of some point of the half-space.
   * @param a the x-coordinate of the normal vector.
   * @param b the y-coordinate of the normal vector.
   * @param c the z-coordinate of the normal vector.
   * @return the implicit mapping of the ellipsoid.
   */
  INLINE
  static RnMap makeHalfSpace3D( float x0, float y0, float z0,
				float a, float b, float c );

  
  // ------------------------- data -----------------------------
private:

  /**
   * The associated mapping.
   */
  RnMapImp* m_ptr_f;

  // ------------------------- Forbidden services -----------------------------
private:
  
};







///////////////////////////////////////////////////////////////////////////////
// class RnMapIdentity
///////////////////////////////////////////////////////////////////////////////

/** 
 * Description of class 'RnMapIdentity' <p>
 * Aim: class to represent the identity mapping.
 */
class RnMapIdentity : public RnMapImp
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~RnMapIdentity();

  /**
   * Constructor.
   * Creates the identity mapping.
   */
  RnMapIdentity();
  
  /**
   * @return the dimension of mapping domain (0 is any).
   */
  virtual uint dimIn() const;
  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  virtual uint dimOut() const;
  
  /**
   * Clone service.
   * @return a dyn. alloc. clone of 'this'.
   */
  virtual RnMapImp* clone() const;

  /**
   * Mapping operator.
   * @param x the in-vector
   * @return the out vector (ie [x]).
   */
  virtual Vector eval( const Vector & x ) const;
  

  // ------------------------- Forbidden services -----------------------------
private:

  /**
   * Copy constructor. (forbidden)
   * @param other the object to copy.
   */
  INLINE RnMapIdentity( const RnMapIdentity & other );
  /**
   * Assignment. (forbidden)
   * @param other the object to copy.
   * @return a reference to 'this'.
   */
  INLINE RnMapIdentity& operator=( const RnMapIdentity & other );
  
};



///////////////////////////////////////////////////////////////////////////////
// class RnMapComposer
///////////////////////////////////////////////////////////////////////////////

/** 
 * Description of class 'RnMapComposer' <p>
 * Aim: class to represent the composition of two mappings 'RnMap'.
 */
class RnMapComposer : public RnMapImp
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~RnMapComposer();

  /**
   * Constructor from two mappings.
   * Creates the mapping 'f o g'. We must have 'g.dimOut() == f.dimIn()'.
   * @param f the first mapping
   * @param g the second mapping.
   */
  RnMapComposer( const RnMapImp & f, const RnMapImp & g );
  
  /**
   * @return the dimension of mapping domain (0 is any).
   */
  virtual uint dimIn() const;
  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  virtual uint dimOut() const;
  
  /**
   * Clone service.
   * @return a dyn. alloc. clone of 'this'.
   */
  virtual RnMapImp* clone() const;

  /**
   * Mapping operator.
   * @param x the in-vector
   * @return the out vector.
   */
  virtual Vector eval( const Vector & x ) const;
  
  // ------------------------- data -----------------------------
private:

  /**
   * The first mapping in 'f o g'.
   */
  RnMapImp* m_f;

  /**
   * The second mapping in 'f o g'.
   */
  RnMapImp* m_g;
  

  // ------------------------- Forbidden services -----------------------------
private:
  /**
   * Default constructor. (forbidden)
   */
  INLINE RnMapComposer();
  /**
   * Copy constructor. (forbidden)
   * @param other the object to copy.
   */
  INLINE RnMapComposer( const RnMapComposer & other );
  /**
   * Assignment. (forbidden)
   * @param other the object to copy.
   * @return a reference to 'this'.
   */
  INLINE RnMapComposer& operator=( const RnMapComposer & other );
  
};




///////////////////////////////////////////////////////////////////////////////
// class RnMapBinaryOperator
///////////////////////////////////////////////////////////////////////////////

/** 
 * Description of class 'RnMapBinaryOperator' <p>
 * Aim: class to represent a binary operation between two mappings 'RnMap'.
 */
class RnMapBinaryOperator : public RnMapImp
{
  // ------------------------- Standard services ------------------------------
public:

  /**
   * Lists the different binary operations that can be applied on maps.
   */
  enum oper2 { ADD, SUB, MUL, DIV, MAX, MIN };
  
  /**
   * Destructor. 
   */
  virtual ~RnMapBinaryOperator();

  /**
   * Constructor from two mappings.
   * Creates the mapping 'op(f(x),g(x))'. 
   * One must have 'f.dimOut() == g.dimOut()'.
   * @param op the binary operator.
   * @param f the first mapping
   * @param g the second mapping.
   */
  RnMapBinaryOperator( oper2 op, const RnMapImp & f, const RnMapImp & g );
  
  /**
   * @return the dimension of mapping domain (0 is any).
   */
  virtual uint dimIn() const;
  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  virtual uint dimOut() const;
  
  /**
   * Clone service.
   * @return a dyn. alloc. clone of 'this'.
   */
  virtual RnMapImp* clone() const;

  /**
   * Mapping operator.
   * @param x the in-vector
   * @return the out vector.
   */
  virtual Vector eval( const Vector & x ) const;
  
  // ------------------------- data -----------------------------
private:

  /**
   * The binary operator.
   */
  oper2 m_op;
  
  /**
   * The first mapping in 'f o g'.
   */
  RnMapImp* m_f;

  /**
   * The second mapping in 'f o g'.
   */
  RnMapImp* m_g;
  

  // ------------------------- Forbidden services -----------------------------
private:
  /**
   * Default constructor. (forbidden)
   */
  INLINE RnMapBinaryOperator();
  /**
   * Copy constructor. (forbidden)
   * @param other the object to copy.
   */
  INLINE RnMapBinaryOperator( const RnMapBinaryOperator & other );
  /**
   * Assignment. (forbidden)
   * @param other the object to copy.
   * @return a reference to 'this'.
   */
  INLINE RnMapBinaryOperator& operator=( const RnMapBinaryOperator & other );
  
};


///////////////////////////////////////////////////////////////////////////////
// class RnMapUnaryOperator
///////////////////////////////////////////////////////////////////////////////

/** 
 * Description of class 'RnMapUnaryOperator' <p>
 * Aim: class to represent a binary operation between two mappings 'RnMap'.
 */
class RnMapUnaryOperator : public RnMapImp
{
  // ------------------------- Standard services ------------------------------
public:

  /**
   * Lists the different binary operations that can be applied on maps.
   */
  enum oper1 { NEG };
  
  /**
   * Destructor. 
   */
  virtual ~RnMapUnaryOperator();

  /**
   * Constructor from two mappings.
   * Creates the mapping 'op(f(x))'. 
   * @param op the unary operator.
   * @param f the mapping
   * @param g the second mapping.
   */
  RnMapUnaryOperator( oper1 op, const RnMapImp & f );
  
  /**
   * @return the dimension of mapping domain (0 is any).
   */
  virtual uint dimIn() const;
  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  virtual uint dimOut() const;
  
  /**
   * Clone service.
   * @return a dyn. alloc. clone of 'this'.
   */
  virtual RnMapImp* clone() const;

  /**
   * Mapping operator.
   * @param x the in-vector
   * @return the out vector.
   */
  virtual Vector eval( const Vector & x ) const;
  
  // ------------------------- data -----------------------------
private:

  /**
   * The unary operator.
   */
  oper1 m_op;
  
  /**
   * The mapping f in 'op(f)'.
   */
  RnMapImp* m_f;


  // ------------------------- Forbidden services -----------------------------
private:
  /**
   * Default constructor. (forbidden)
   */
  INLINE RnMapUnaryOperator();
  /**
   * Copy constructor. (forbidden)
   * @param other the object to copy.
   */
  INLINE RnMapUnaryOperator( const RnMapUnaryOperator & other );
  /**
   * Assignment. (forbidden)
   * @param other the object to copy.
   * @return a reference to 'this'.
   */
  INLINE RnMapUnaryOperator& operator=( const RnMapUnaryOperator & other );
  
};





///////////////////////////////////////////////////////////////////////////////
// class RnMapTranslater
///////////////////////////////////////////////////////////////////////////////

/** 
 * Description of class 'RnMapTranslater' <p>
 * Aim: class to represent the translation mapping.
 */
class RnMapTranslater : public RnMapImp
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~RnMapTranslater();

  /**
   * Constructor.
   * Creates the translater mapping of vector [v]. Add [v] to any vector [x]
   * given in parameter in 'eval'.
   * @param v the translation vector.
   */
  RnMapTranslater( const Vector & v );
  
  /**
   * @return the dimension of mapping domain (0 is any).
   */
  virtual uint dimIn() const;
  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  virtual uint dimOut() const;
  
  /**
   * Clone service.
   * @return a dyn. alloc. clone of 'this'.
   */
  virtual RnMapImp* clone() const;

  /**
   * Mapping operator.
   * [x] must have the same dimension as [m_v].
   * @param x the in-vector
   * @return the out vector.
   */
  virtual Vector eval( const Vector & x ) const;
  
  // ------------------------- data -----------------------------
private:

  /**
   * The translation vector.
   */
  Vector m_v;

  // ------------------------- Forbidden services -----------------------------
private:
  /**
   * Default constructor. (forbidden)
   */
  INLINE RnMapTranslater();
  /**
   * Copy constructor. (forbidden)
   * @param other the object to copy.
   */
  INLINE RnMapTranslater( const RnMapTranslater & other );
  /**
   * Assignment. (forbidden)
   * @param other the object to copy.
   * @return a reference to 'this'.
   */
  INLINE RnMapTranslater& operator=( const RnMapTranslater & other );
  
};



///////////////////////////////////////////////////////////////////////////////
// class RnMapConstantDotProduct
///////////////////////////////////////////////////////////////////////////////

/** 
 * Description of class 'RnMapConstantDotProduct' <p>
 * Aim: class to represent a dot product mapping with a constant vector.
 */
class RnMapConstantDotProduct : public RnMapImp
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~RnMapConstantDotProduct();

  /**
   * Constructor.
   * Creates the dot product mapping of vector [v]. Performs the scalar
   * product of [v] with any vector [x] given in parameter in 'eval'.
   * @param v the vector.
   */
  RnMapConstantDotProduct( const Vector & v );
  
  /**
   * @return the dimension of mapping domain (0 is any).
   */
  virtual uint dimIn() const;
  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  virtual uint dimOut() const;
  
  /**
   * Clone service.
   * @return a dyn. alloc. clone of 'this'.
   */
  virtual RnMapImp* clone() const;

  /**
   * Mapping operator.
   * [x] must have the same dimension as [m_v].
   * @param x the in-vector
   * @return the out vector.
   */
  virtual Vector eval( const Vector & x ) const;
  
  // ------------------------- data -----------------------------
private:

  /**
   * The constant vector for the dot product.
   */
  Vector m_v;

  // ------------------------- Forbidden services -----------------------------
private:
  /**
   * Default constructor. (forbidden)
   */
  INLINE RnMapConstantDotProduct();
  /**
   * Copy constructor. (forbidden)
   * @param other the object to copy.
   */
  INLINE RnMapConstantDotProduct( const RnMapConstantDotProduct & other );
  /**
   * Assignment. (forbidden)
   * @param other the object to copy.
   * @return a reference to 'this'.
   */
  INLINE RnMapConstantDotProduct& operator=( const RnMapConstantDotProduct & other );
  
};


///////////////////////////////////////////////////////////////////////////////
// class RnMapMatrixMultiplier
///////////////////////////////////////////////////////////////////////////////

/** 
 * Description of class 'RnMapMatrixMultiplier' <p>
 * Aim: class to represent the matrix multiplier mapping.
 */
class RnMapMatrixMultiplier : public RnMapImp
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~RnMapMatrixMultiplier();

  /**
   * Constructor from matrix.
   * Creates the matrix multiplier mapping of matrix [m]. Multiplies [m] to any
   * vector [x] given in parameter in 'eval'.
   * @param m the matrix.
   */
  RnMapMatrixMultiplier( const Matrix & m );
  
  /**
   * @return the dimension of mapping domain (0 is any).
   */
  virtual uint dimIn() const;
  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  virtual uint dimOut() const;
  
  /**
   * Clone service.
   * @return a dyn. alloc. clone of 'this'.
   */
  virtual RnMapImp* clone() const;

  /**
   * Mapping operator.
   * [x] must have the same dimension as 'm_m.columns()'.
   * @param x the in-vector
   * @return the out vector.
   */
  virtual Vector eval( const Vector & x ) const;
  
  // ------------------------- data -----------------------------
private:

  /**
   * The matrix.
   */
  Matrix m_m;

  // ------------------------- Forbidden services -----------------------------
private:
  /**
   * Default constructor. (forbidden)
   */
  INLINE RnMapMatrixMultiplier();
  /**
   * Copy constructor. (forbidden)
   * @param other the object to copy.
   */
  INLINE RnMapMatrixMultiplier( const RnMapMatrixMultiplier & other );
  /**
   * Assignment. (forbidden)
   * @param other the object to copy.
   * @return a reference to 'this'.
   */
  INLINE RnMapMatrixMultiplier& operator=( const RnMapMatrixMultiplier & other );
  
};


///////////////////////////////////////////////////////////////////////////////
// class RnMapImplicitSphere
///////////////////////////////////////////////////////////////////////////////

/** 
 * Description of class 'RnMapImplicitSphere' <p>
 * Aim: class to represent the implicit definition of a sphere (ball) centered
 * on (0,..,0).
 */
class RnMapImplicitSphere : public RnMapImp
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~RnMapImplicitSphere();

  /**
   * Constructor.
   * Creates the implicit sphere mapping of radius [r]. 
   * @param r the radius of the sphere.
   */
  RnMapImplicitSphere( float r );
  
  /**
   * @return the dimension of mapping domain (0 is any).
   */
  virtual uint dimIn() const;
  /**
   * @return the dimension of mapping range (0 is same as in).
   */
  virtual uint dimOut() const;
  
  /**
   * Clone service.
   * @return a dyn. alloc. clone of 'this'.
   */
  virtual RnMapImp* clone() const;

  /**
   * Mapping operator.
   * @param x the in-vector
   * @return the out vector.
   */
  virtual Vector eval( const Vector & x ) const;
  
  // ------------------------- data -----------------------------
private:

  /**
   * The radius of the sphere.
   */
  float m_r;

  // ------------------------- Forbidden services -----------------------------
private:
  /**
   * Default constructor. (forbidden)
   */
  INLINE RnMapImplicitSphere();
  /**
   * Copy constructor. (forbidden)
   * @param other the object to copy.
   */
  INLINE RnMapImplicitSphere( const RnMapImplicitSphere & other );
  /**
   * Assignment. (forbidden)
   * @param other the object to copy.
   * @return a reference to 'this'.
   */
  INLINE RnMapImplicitSphere& operator=( const RnMapImplicitSphere & other );
  
};

// ///////////////////////////////////////////////////////////////////////////////
// // class RnImplicitMap
// ///////////////////////////////////////////////////////////////////////////////

// /** 
//  * Description of class 'RnImplicitMap' <p>
//  * Aim: Bridge class to represent any mapping from Rn to R. Uses 'RnMapImp'
//  * hierarchy to build mappings.
//  * An implicit map is a couple of 'RnMap's (f,g) such that f: Rn->R and 
//  * g: Rm->Rn (generally m=n). To evaluate the implicit map at x, it computes
//  * the value f(g(x)).
//  * @see RnMapImp
//  */
// class RnImplicitMap
// {
//   // ------------------------- Standard services ------------------------------
// public:
//   /**
//    * Destructor. 
//    */
//   ~RnImplicitMap();

//   /**
//    * Creates the mapping (imp,Id).
//    * @param imp the implicit mapping.
//    */
//   RnImplicitMap( const RnMap & imp );

//   /**
//    * Creates the mapping (imp,g).
//    * @param imp the implicit mapping.
//    * @param g a mapping.
//    */
//   RnImplicitMap( const RnMap & imp, const RnMap & g );

//   /**
//    * Copy constructor.
//    * @param other the object to copy.
//    */
//   RnImplicitMap( const RnImplicitMap & other );

//   /**
//    * Assignment.
//    * @param other the object to copy.
//    * @return a reference to 'this'.
//    */
//   RnImplicitMap& operator=( const RnImplicitMap & other );
  
  
//   // ------------------------- mapping services ------------------------------
// public:

//   /**
//    * @return the dimension of mapping domain (0 is any).
//    */
//   INLINE uint dimIn() const;

//   /**
//    * @return the dimension of mapping range.
//    */
//   INLINE uint dimOut() const;
  
//   /**
//    * Mapping operator.
//    * @param x the in-vector
//    * @return the scalar result.
//    */
//   INLINE float eval( const Vector & x ) const;


//   // ------------------------- operator services ------------------------------
// public:

//   /**
//    * Post-composition operator.
//    * @param g any mapping.
//    * @return the mapping (imp,f(g(x)) from (imp,f).
//    */
//   RnImplicitMap operator()( const RnMap & g ) const;

//   /**
//    * Intersection service.
//    * @param m any implicit mapping (imp2,g)
//    * @return the mapping 'max(imp(f(x)),imp2(g(x)))' from (imp,f)
//    */
//   RnImplicitMap max( const RnImplicitMap & m ) const;

//   /**
//    * Union service.
//    * @param m any implicit mapping (imp2,g)
//    * @return the mapping 'max(imp(f(x)),imp2(g(x)))' from (imp,f)
//    */
//   RnImplicitMap min( const RnImplicitMap & m ) const;


//   // ------------------------- data ------------------------------
// private:

//   /**
//    * Stores the mapping from Rn to R.
//    */
//   RnMapImp* m_implicit_map;

//   /**
//    * Stores a transformation from Rm to Rn to perform before evaluating
//    * 'm_implicit_map'.
//    * @see m_implicit_map
//    */
//   RnMapImp* m_transformation;


//   // ------------------------- forbidden services -----------------------------
// private:

//   /**
//    * Default Constructor. forbidden
//    */
//   INLINE RnImplicitMap();

// };


// /**
//  * Pre-composition operator.
//  * @param g any mapping.
//  * @param m any implicit mapping (imp,f).
//  * @return the mapping (imp,g(f(x))) from (imp,f).
//  */
// RnImplicitMap operator()( const RnMap & g, const RnImplicitMap & m );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "RnMaps.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined RnMaps_h

#undef RnMaps_RECURSES
#endif // else defined(RnMaps_RECURSES)
