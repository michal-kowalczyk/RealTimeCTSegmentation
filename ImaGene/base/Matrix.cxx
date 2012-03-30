///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Matrix.cxx
//
// Creation : 2005/01/04
//
// Version : 2005/01/04
//
// Author : Jacques-Olivier Lachaud
//
// email : lachaud@labri.fr
//
// Purpose : ??
//
// Distribution :
//
// Use :
//	??
//
// Todo :
//	O ??
//
// History :
//	2005/01/04 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/base/Matrix.h"
#include "ImaGene/base/Vector.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/base/Matrix.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const Matrix_RCS_ID = "@(#)class Matrix definition.";



///////////////////////////////////////////////////////////////////////////////
// class Matrix
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::Matrix::~Matrix()
{
  if ( m_owned )
    {
      ASSERT_Matrix( m_data != 0 );
      delete[] m_data;
    }
}


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
ImaGene::Matrix::Matrix( uint rowNb, uint columnNb, float* data )
  : m_nb_rows( rowNb ), m_nb_columns( columnNb )
{
  if ( data == NULL )
    {
      m_owned = true;
      m_data = new float[ rowNb * columnNb ];
    }
  else
    {
      m_owned = false;
      m_data = data;
    }
}


    
/**
 * Copy constructor.
 *
 * \param v is the vector to be copied.
 */
ImaGene::Matrix::Matrix(const Matrix& v)
  : m_nb_rows( v.m_nb_rows), m_nb_columns( v.m_nb_columns )
{
  m_owned = true;
  uint size = m_nb_columns * m_nb_rows;
  m_data = new float[ size ];
  for ( uint i = 0; i < size; ++i )
    m_data[ i ] = v.m_data[ i ];
}

    
/**
 * Assignement operator.
 *
 * \pre the size of the \a v vector and the size of the \a *this
 * vector must be identical
 */
ImaGene::Matrix& 
ImaGene::Matrix::operator= (const Matrix& v)
{
  ASSERT_Matrix( v.lines() == lines() );
  ASSERT_Matrix( v.columns() == columns() );

  if ( this != &v )
    {
      uint size = m_nb_columns * m_nb_rows;
      for ( uint i = 0; i < size; ++i )
	m_data[ i ] = v.m_data[ i ];
    }

  return *this;
}



/**
 * Multiplies \a *this by the \a coeff scalar number.
 *
 * \param coeff is the factor \a *this get multiplied by.
 */
ImaGene::Matrix&  
ImaGene::Matrix::operator*= (float coeff)
{
  uint size = m_nb_columns * m_nb_rows;
  for ( uint i = 0; i < size; ++i )
    m_data[ i ] *= coeff;
  return *this;
}



/**
 * Divides \a *this by the \a coeff scalar number.
 *
 * \pre The \a coeff number must be non null.
 *
 * \param coeff is the factor \a *this get divided by.
 */
ImaGene::Matrix&  
ImaGene::Matrix::operator/= (float coeff)
{
  uint size = m_nb_columns * m_nb_rows;
  for ( uint i = 0; i < size; ++i )
    m_data[ i ] /= coeff;
  return *this;
}

 

/**
 * Addition operator.
 *
 * \pre Matrix \a v must have the same size as \a *this.
 *
 * \param v is the vector that gets added to \a *this.
 */
ImaGene::Matrix&  
ImaGene::Matrix::operator+= (const Matrix& v)
{
  ASSERT_Matrix( v.lines() == lines() );
  ASSERT_Matrix( v.columns() == columns() );

  uint size = m_nb_columns * m_nb_rows;
  for ( uint i = 0; i < size; ++i )
    m_data[ i ] += v.m_data[ i ];
  return *this;
}



/**
 * Substraction operator.
 *
 * \pre Matrix \a v must have the same size as \a *this.
 *
 * \param v is the vector that gets substracted from \a *this.
 */
ImaGene::Matrix&  
ImaGene::Matrix::operator-= (const Matrix& v)
{
  ASSERT_Matrix( v.lines() == lines() );
  ASSERT_Matrix( v.columns() == columns() );

  uint size = m_nb_columns * m_nb_rows;
  for ( uint i = 0; i < size; ++i )
    m_data[ i ] -= v.m_data[ i ];
  return *this;
}


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
void
ImaGene::Matrix::rMult(const Matrix& m, Matrix& prod) const
{
  ASSERT_Matrix( prod.lines() == lines() );
  ASSERT_Matrix( prod.columns() == m.columns() );

  for(uint i=0 ; i<prod.m_nb_rows ; ++i)
    for(uint j=0 ; j<prod.m_nb_columns ; ++j)
      {
	/* Calcule le nouveau coefficient en (i, j) */
	float & v = prod.rw(i,j);
	v = ro(i,0) * m.ro(0,j);
	for ( uint k = 1; k < m_nb_columns; ++k )
	  v += ro(i,k) * m.ro(k,j);
      }

}

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
void
ImaGene::Matrix::rMult(const Vector& m, Vector& prod) const
{
  ASSERT_Matrix( prod.size() == lines() );
  ASSERT_Matrix( columns() == m.size() );

  for(uint i=0 ; i < prod.size() ; ++i)
    {
      /* Calcule le nouveau coefficient en i */
      float & v = prod.rw( i );
      v = ro( i, 0 ) * m.ro( 0 );
      for ( uint k = 1; k < m_nb_columns; ++k )
	v += ro( i, k ) * m.ro( k );
      }
}

    




///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static array services -----------------------


/**
 * This map is used to store the association between matrix arrays
 * and float arrays. This is used to alloc memory only once for all
 * matrices when building a arrays of matrices.
 */
map<const void*, float*> ImaGene::Matrix::sBuffers;




/**
 * Opérateur new de placement. Utilisé pour permettre l'allocation
 * de tableau en invoquant un constructeur à paramètres.  
 */
void* 
ImaGene::Matrix::operator new (size_t size, void* location)
{
  return location;
}




/**
 * Usual new operator.
 *
 * @param size the requested amount of memory.
 */
void* 
ImaGene::Matrix::operator new (size_t size)
{
  return ::operator new (size);
}




/**
 * Alloue de la mémoire pour stocker les données de matrixNb matrices
 * contenant chacune matrixDataSize coefficients, et destinées à être
 * placées dans le tableau de matrices pointé par key. L'association
 * entre le tableau de matrices et le tableau de données et conservée
 * dans sBuffers.
 */
float*
ImaGene::Matrix::newDataArray(const void* key, int elementNb, int elementSize)
{
  float* result = new float[elementNb * elementSize];
  sBuffers.insert(make_pair(key, result));

  return result;
}




/**
 * Libère la zone mémoire associée au tableau de matrices key.
 *
 * @pre (invérifiable, mais qui est vraie si tout est codée
 * correctement...) : le tableau passé en paramètre doit être un
 * tableau valide.
 */
void
ImaGene::Matrix::deleteDataArray(const void* key)
{
  map<const void*, float*>::iterator it = sBuffers.find(key);
  ASSERT_Matrix(it != sBuffers.end());

  delete[] it->second;
  sBuffers.erase(it);
}




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
ImaGene::Matrix* 
ImaGene::Matrix::newArray(uint nb, uint rowNb, uint columnNb )
{
  /* On alloue l'espace nécessaire sans invoquer le constructeur par
     défaut (puisque de toute manière il n'y en a pas). */
  Matrix* result = (Matrix*) (new uint8[nb * sizeof(Matrix)]);

  /* On alloue le support des matrices et on l'enregistre (pour
     pouvoir retrouver le tableau à détruire lors de l'appel à delete) */
  uint size = rowNb * columnNb;
  float* data = newDataArray(result, nb, size );
  
  /* On invoque le constructeur approprié individuellement pour chaque
     élément du tableau en utilisant le new de placement. */
  for (uint i=0 ; i<nb ; ++i, data += size)
    new(result+i) Matrix( rowNb, columnNb, data );

  return result;
}


 

/**
 * Détruit un tableau alloué avec newArray.
 */
void 
ImaGene::Matrix::deleteArray(Matrix* array, uint arraySize)
{
  /* On est obligé d'invoquer explicitement tous les destructeurs pour
     éviter que delete ne se trompe */
  for(uint i=0 ; i<arraySize ; ++i)
    array[i].~Matrix(); 

  /* On détruit alors les supports des matrices */
  deleteDataArray(array);

  /* On peut alors détruire array de la même manière qu'il a été
     alloué. */
  delete[] (uint8*)array;
}





///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::Matrix::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Matrix]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::Matrix::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
