/** @file Vector.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Vector.h
//
// Creation : 2005/01/04
//
// Version : 2005/01/04
//
// Author : JOL
//
// Summary : Header file for files Vector.ih and Vector.cxx
//
// History :
//	2005/01/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Vector declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Vector_RECURSES)
#error Recursive header files inclusion detected in Vector.h
#else // defined(Vector_RECURSES)
#define Vector_RECURSES

#if !defined Vector_h
#define Vector_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
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
  // class Vector
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Vector' <p>
   * Aim: To represent an array of float of size fixed at instantiation.
   */
  class Vector
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~Vector();

    /**
     * Builds a new vector with the \a size size. If \a data is \c NULL
     * then the vector allows itself the required amount of
     * memory. 
     *
     * Else, the memory \a data points to is used to initialize and then
     * store the coefficients of the vector. In the former case, the
     * allocated memory gets freed by the destructor of the vector. In
     * the latter case the memory \a data points to is \em not freed
     * automatically.
     *
     * \param size determines the number of coefficients in the vector.
     *
     * \param data either is \c NULL or points to a valid memory area
     * that contains at least \a size float numbers. In the former case,
     * the caller remains the owner of the memory \a data points to.
     */
    Vector(uint size, float* data = NULL);
    
    
    /**
     * Copy constructor.
     *
     * \param v is the vector to be copied.
     */
    Vector(const Vector& v);
    
    
    /**
     * Builds a new vector with the \a coord coefficient
     * removed. Coefficients are reordered using the circular
     * permutation that brings the \a coords + \a 1-th coefficient to
     * the first index of the vector. This is to make ensure that the
     * basis is kept direct.
     *
     * \param v is the source vector.
     *
     * \param coord is the coordinate that get removed.
     */
    Vector(const Vector& v, uint coord);
    
    
    /**
     * Returns the size of the vector (i.e. the number of its
     * coefficients).
     */
    INLINE uint size(void) const;
    

    /**
     * Returns the  \a i-th coefficient of the vector.
     *
     * \pre The \a i index must lie between \a 0 and \a size() .
     *
     * \param i is the index of the retrieved coefficient.
     */
    INLINE const float& ro(uint i) const;
    
    
    /**
     * Returns a non-const reference to the \a i-th element of the
     * vector.
     *
     * \pre The \a i index must lie between \a 0 and \a size() .
     *
     * \param i is the index of the retrieved coefficient.
     */
    INLINE float& rw(uint i);
    
    
    /**
     * Assignement operator.
     *
     * \pre the size of the \a v vector and the size of the \a *this
     * vector must be identical
     */
    Vector& operator= (const Vector& v);
    
    
    /**
     * Multiplies \a *this by the \a coeff scalar number.
     *
     * \param coeff is the factor \a *this get multiplied by.
     */
    Vector& operator*= (float coeff);
    
    
    /**
     * Divides \a *this by the \a coeff scalar number.
     *
     * \pre The \a coeff number must be non null.
     *
     * \param coeff is the factor \a *this get divided by.
     */
    Vector& operator/= (float coeff);
    
    
    /**
     * Addition operator.
     *
     * \pre Vector \a v must have the same size as \a *this.
     *
     * \param v is the vector that gets added to \a *this.
     */
    Vector& operator+= (const Vector& v);
    
    
    /**
     * Substraction operator.
     *
     * \pre Vector \a v must have the same size as \a *this.
     *
     * \param v is the vector that gets substracted from \a *this.
     */
    Vector& operator-= (const Vector& v);
    
    /**
     * @return the address at which datas are stored. Be careful when
     * using this method.
     */
    INLINE float* data() const;
    
    /**
     * Resets all the values to zero.
     */
    INLINE void zero();
    
     /**
      * Assigns the values stored starting at address [data] to the vector.
      *
      * @pre data must be an array of size at least 'size()'.
      * @param data the address of the first value to assign.
     */
    INLINE void assign( float* data );

   
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
  protected:

    /**
     * Size of the array.
     */
    uint m_size;
    
    /**
     * Data.
     */
    float* m_data;
    
    /**
     * When 'true', the vector has instantiated the array 'data',
     * otherwise the data has been instantiated elsewhere.
     */
    bool m_owned;
    

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE Vector();

    // ------------------------- Internals ------------------------------------
  private:

    // ------------------------- Static array services -----------------------
  protected :
    /**
     * This map is used to store the association between matrix arrays
     * and float arrays. This is used to alloc memory only once for all
     * matrices when building a arrays of matrices.
     */
    static std::map<const void*, float*> sBuffers;
  
    
    /**
     * Opérateur new de placement. Utilisé pour permettre l'allocation
     * de tableau en invoquant un constructeur à paramètres.  
     */
    static void* operator new (size_t size, void* location);
    
    
    /**
     * Alloue de la mémoire pour stocker les données de matrixNb matrices
     * contenant chacune matrixDataSize coefficients, et destinées à être
     * placées dans le tableau de matrices pointé par key. L'association
     * entre le tableau de matrices et le tableau de données et conservée
     * dans sBuffers.
     */
    static float* newDataArray(const void* key, 
			       int elementNb, 
			       int elementSize);
    

    /**
     * Libère la zone mémoire associée au tableau de matrices key.
     *
     * Précondition (invérifiable, mais qui est vraie si tout est codée
     * correctement...) : le tableau passé en paramètre doit être un
     * tableau valide.
     */
    static void deleteDataArray(const void* key);

  public:
    /**
     * Usual new operator.
     *
     * \param size the requested amount of memory.
     */
    static void* operator new (size_t size);
    
    
    /**
     * Allocates an array of [nb] instances of Vector each of which
     * has a size [size]. It is the only way to allocate an array of
     * several Vectors.
     *
     * @param nb the number of Vector instances.
     * @param size the size of each Vector (ie dimension).
     * @return a pointer on an array of Vector.
     *
     * @see deleteArray
     */
    static Vector* newArray(uint nb, uint size );
    
    
    /**
     * Desallocates an array of Vectors instantiated by newArray..
     *
     * @param array the array of Vectors.
     * @param arraySize the number of allocated Vectors.
     *
     * @see newArray
     */
    static void deleteArray(Vector* array, uint arraySize);
    
    
  };
  

  /////////////////////////////////////////////////////////////////////////////
  // class Vector2D
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Vector2D' <p>
   * Aim: To represent an array of float of size 2.
   */
  class Vector2D : public Vector
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    INLINE ~Vector2D();

    /**
     * Constructor.
     */
    Vector2D();

    /**
     * Instantiate a vector with coordinates [x], [y]
     * @param x the first value
     * @param y the second value
     */
    Vector2D( float x, float y );

    
    /**
     * Copy constructor.
     *
     * @param v the vector to copy.
     */
    Vector2D( const Vector2D& v );

    /**
     * Copy constructor with conversion (no shared information).
     *
     * @pre v must be of size (at least) 2.
     * @param v the vector to copy.
     */
    Vector2D( const Vector& v );


    /**
     * Instantiates the vector [v]-[u].
     * @param u any vector of size (at least) 2.
     * @param v any vector of size (at least) 2.
     */
    Vector2D(const Vector& u, const Vector& v);

    /**
     * Assignment.
     * @param v any vector of size (at least) 2.
     * @return a reference on this.
     */
    Vector2D& operator= (const Vector& v);

    /**
     * Assignment.
     * @param v any vector2D.
     * @return a reference on this.
     */
    Vector2D& operator= (const Vector2D& v);
    
    /**
     * @return a const reference on the first value.
     */
    INLINE const float& x(void) const;
    
    
    /**
     * @return a const reference on the second value.
     */
    INLINE const float& y(void) const;


    /**
     * @return a reference on the first value.
     */
    INLINE float& x(void);
    

    /**
     * @return a reference on the second value.
     */
    INLINE float& y(void);


    // ----------------------- datas ------------------------------
  private:

    /**
     * Static array of 2 values.
     */
    float m_static_data[ 2 ];

  };
  
  


  /**
   * Overloads 'operator<<' for displaying objects of class 'Vector'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Vector' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const Vector & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/Vector.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Vector_h

#undef Vector_RECURSES
#endif // else defined(Vector_RECURSES)
