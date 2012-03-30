/** @file Matrix.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Matrix.h
//
// Creation : 2005/01/04
//
// Version : 2005/01/04
//
// Author : JOL
//
// Summary : Header file for files Matrix.ih and Matrix.cxx
//
// History :
//	2005/01/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Matrix declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Matrix_RECURSES)
#error Recursive header files inclusion detected in Matrix.h
#else // defined(Matrix_RECURSES)
#define Matrix_RECURSES

#if !defined Matrix_h
#define Matrix_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
//////////////////////////////////////////////////////////////////////////////
namespace ImaGene {
  class Vector;
}

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class Matrix
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Matrix' <p>
   * Aim: To represent a matrix of float of size fixed at instantiation.
   */
  class Matrix
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~Matrix();

    /**
     * Builds a new matrix with \a rowNb rows and \a columNb
     * columns. If \a data is \c NULL then the vector allows itself
     * the required amount of memory.
     *
     * Else, the memory \a data points to is used to initialize and then
     * store the coefficients of the vector. In the former case, the
     * allocated memory gets freed by the destructor of the vector. In
     * the latter case the memory \a data points to is \em not freed
     * automatically.
     *
     * \param rowNb the number of rows
     * \param columnNb the number of columns.
     *
     * \param data either is \c NULL or points to a valid memory area
     * that contains at least \a size float numbers. In the former case,
     * the caller remains the owner of the memory \a data points to.
     */
    Matrix(uint rowNb, uint columnNb, float* data = NULL);
    
    
    /**
     * Copy constructor.
     *
     * \param v is the matrix to be copied.
     */
    Matrix(const Matrix& v);
    
    
    /**
     * @return the number of rows in the matrix.
     */
    INLINE uint rows(void) const;

    /**
     * @deprecated
     * @return the number of rows in the matrix.
     * @see rows
     */
    INLINE uint lines(void) const;


    /**
     * @return  the number of columns in the matrix.
     */
    INLINE uint columns(void) const;


    /**
     * Returns a constant reference to the coefficient at position (\a
     * i, \a j) in the matrix.
     *
     * \pre \a i must lay between \a 0 and \c this->lines() and \a j
     * must lay between \a 0 and \c this->columns() .
     *
     * \param i the line of the coefficient to be read.
     *
     * \param j the column of the coefficient to be read.
     */
    INLINE const float& ro(uint i, uint j) const;
    
    
    /**
     * Returns a modifiable reference to the coefficient at position (\a
     * i, \a j) in the matrix.
     *
     * \pre \a i must lay between \a 0 and \c this->lines() and \a j
     * must lay between \a 0 and \c this->columns() .
     *
     * \param i the line of the coefficient to be read.
     *
     * \param j the column of the coefficient to be read.
     */
    INLINE float& rw(uint i, uint j);
    
    
    /**
     * Assignement operator.
     *
     * \pre the size of the \a v matrix and the size of the \a *this
     * matrix must be identical
     */
    Matrix& operator= (const Matrix& v);
    
    
    /**
     * Multiplies \a *this by the \a coeff scalar number.
     *
     * \param coeff is the factor \a *this get multiplied by.
     */
    Matrix& operator*= (float coeff);
    
    
    /**
     * Divides \a *this by the \a coeff scalar number.
     *
     * \pre The \a coeff number must be non null.
     *
     * \param coeff is the factor \a *this get divided by.
     */
    Matrix& operator/= (float coeff);
        
 
    /**
     * Addition operator.
     *
     * \pre Matrix \a v must have the same size as \a *this.
     *
     * \param v is the vector that gets added to \a *this.
     */
    Matrix& operator+= (const Matrix& v);
    
    
    /**
     * Substraction operator.
     *
     * \pre Matrix \a v must have the same size as \a *this.
     *
     * \param v is the vector that gets substracted from \a *this.
     */
    Matrix& operator-= (const Matrix& v);
   
    /**
     * Computes the product \a prod = \a *this \f$ \times \f$ \a m.
     *
     * \pre the size of the \a prod matrix must match that of the
     * product of \a *this by \a m.
     *
     * \param m the matrix \a *this get multiplied by.
     * 
     * \param prod the matrix the result is written to.
     */
    void rMult(const Matrix& m, Matrix& prod) const;
    
    /**
     * Computes the product \a prod = \a *this \f$ \times \f$ \a m.
     *
     * \pre the size of the \a prod vector must match that of the
     * product of \a *this by \a m.
     *
     * \param m the vector \a *this get multiplied by.
     * 
     * \param prod the vector the result is written to.
     */
    void rMult(const Vector& m, Vector& prod) const;
    
    
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
     * Nb of rows of the matrix.
     */
    uint m_nb_rows;

    /**
     * Nb of columns of the matrix.
     */
    uint m_nb_columns;
    
    /**
     * Data.
     */
    float* m_data;
    
    /**
     * When 'true', the matrix has instantiated the array 'data',
     * otherwise the data has been instantiated elsewhere.
     */
    bool m_owned;
    

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE Matrix();

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
     * Allocates an array of [nb] instances of Matrix each of which
     * has a size [size]. It is the only way to allocate an array of
     * several Matrixs.
     *
     * @param nb the number of Matrix instances.
     * @param rowNb the number of rows of each Matrix.
     * @param columnNb the number of rows of each Matrix.
     * @return a pointer on an array of Matrix.
     *
     * @see deleteArray
     */
    static Matrix* newArray(uint nb, uint rowNb, uint columnNb );
    
    
    /**
     * Desallocates an array of Matrixs instantiated by newArray..
     *
     * @param array the array of Matrixs.
     * @param arraySize the number of allocated Matrixs.
     *
     * @see newArray
     */
    static void deleteArray(Matrix* array, uint arraySize);
    
    
  };
  


  /**
   * Overloads 'operator<<' for displaying objects of class 'Matrix'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'Matrix' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const Matrix & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/Matrix.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Matrix_h

#undef Matrix_RECURSES
#endif // else defined(Matrix_RECURSES)
