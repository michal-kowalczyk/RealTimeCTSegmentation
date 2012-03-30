///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Vector.cxx
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
#include "ImaGene/base/Vector.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/base/Vector.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const Vector_RCS_ID = "@(#)class Vector definition.";



///////////////////////////////////////////////////////////////////////////////
// class Vector
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::Vector::~Vector()
{
  if ( m_owned )
    {
      ASSERT_Vector( m_data != 0 );
      delete[] m_data;
    }
}

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
ImaGene::Vector::Vector( uint size, float* data )
  : m_size( size )
{
  if ( data == NULL )
    {
      m_owned = true;
      m_data = new float[ size ];
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
ImaGene::Vector::Vector(const Vector& v)
  : m_size( v.m_size )
{
  m_owned = true;
  m_data = new float[ m_size ];
  for ( uint i = 0; i < m_size; ++i )
    m_data[ i ] = v.m_data[ i ];
}

    
    
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
ImaGene::Vector::Vector(const Vector& v, uint coord)
  : m_size( v.m_size - 1 )
{
  m_owned = true;
  m_data = new float[ m_size ];
  for (uint i = 0 ; i < size(); ++i )
    m_data[ i ] = v.m_data[ (coord+1+i) % v.size() ];
}

/**
 * Assignement operator.
 *
 * \pre the size of the \a v vector and the size of the \a *this
 * vector must be identical
 */
ImaGene::Vector& 
ImaGene::Vector::operator= (const Vector& v)
{
  ASSERT_Vector( v.size() == size() );

  if ( this != &v )
    assign( v.m_data );
  return *this;
}



/**
 * Multiplies \a *this by the \a coeff scalar number.
 *
 * \param coeff is the factor \a *this get multiplied by.
 */
ImaGene::Vector&  
ImaGene::Vector::operator*= (float coeff)
{
  for ( uint i = 0; i < m_size; ++i )
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
ImaGene::Vector&  
ImaGene::Vector::operator/= (float coeff)
{
  for ( uint i = 0; i < m_size; ++i )
    m_data[ i ] /= coeff;
  return *this;
}



/**
 * Addition operator.
 *
 * \pre Vector \a v must have the same size as \a *this.
 *
 * \param v is the vector that gets added to \a *this.
 */
ImaGene::Vector&  
ImaGene::Vector::operator+= (const Vector& v)
{
  for ( uint i = 0; i < m_size; ++i )
    m_data[ i ] += v.m_data[ i ];
  return *this;
}



/**
 * Substraction operator.
 *
 * \pre Vector \a v must have the same size as \a *this.
 *
 * \param v is the vector that gets substracted from \a *this.
 */
ImaGene::Vector&  
ImaGene::Vector::operator-= (const Vector& v)
{
  for ( uint i = 0; i < m_size; ++i )
    m_data[ i ] -= v.m_data[ i ];
  return *this;
}





///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static array services -----------------------


/**
 * This map is used to store the association between matrix arrays
 * and float arrays. This is used to alloc memory only once for all
 * matrices when building a arrays of matrices.
 */
map<const void*, float*> ImaGene::Vector::sBuffers;




/**
 * Opérateur new de placement. Utilisé pour permettre l'allocation
 * de tableau en invoquant un constructeur à paramètres.  
 */
void* 
ImaGene::Vector::operator new (size_t size, void* location)
{
  return location;
}




/**
 * Usual new operator.
 *
 * @param size the requested amount of memory.
 */
void* 
ImaGene::Vector::operator new (size_t size)
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
ImaGene::Vector::newDataArray(const void* key, int elementNb, int elementSize)
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
ImaGene::Vector::deleteDataArray(const void* key)
{
  map<const void*, float*>::iterator it = sBuffers.find(key);
  ASSERT_Vector(it != sBuffers.end());

  delete[] it->second;
  sBuffers.erase(it);
}




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
ImaGene::Vector* 
ImaGene::Vector::newArray(uint nb, uint size )
{
  /* On alloue l'espace nécessaire sans invoquer le constructeur par
     défaut (puisque de toute manière il n'y en a pas). */
  Vector* result = (Vector*) (new uint8[nb * sizeof(Vector)]);

  /* On alloue le support des matrices et on l'enregistre (pour
     pouvoir retrouver le tableau à détruire lors de l'appel à delete) */
  float* data = newDataArray(result, nb, size );
  
  /* On invoque le constructeur approprié individuellement pour chaque
     élément du tableau en utilisant le new de placement. */
  for (uint i=0 ; i<nb ; ++i, data += size)
    new(result+i) Vector( size, data );

  return result;
}


 

/**
 * Détruit un tableau alloué avec newArray.
 */
void 
ImaGene::Vector::deleteArray(Vector* array, uint arraySize)
{
  /* On est obligé d'invoquer explicitement tous les destructeurs pour
     éviter que delete ne se trompe */
  for(uint i=0 ; i<arraySize ; ++i)
    array[i].~Vector(); 

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
ImaGene::Vector::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Vector(" << size(); // << " data=" << m_data;
  for ( uint i = 0; i < size(); ++ i)
    cout << " " << ro( i );
  cout << " ]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::Vector::OK() const
{
  return true;
}




///////////////////////////////////////////////////////////////////////////////
// Class Vector2D
///////////////////////////////////////////////////////////////////////////////


/**
 * Constructor.
 */
ImaGene::Vector2D::Vector2D()
  : Vector( 2, m_static_data )
{
}

/**
 * Copy constructor.
 *
 * @param v the vector to copy.
 */
ImaGene::Vector2D::Vector2D( const Vector2D& v )
  : Vector( 2, m_static_data )
{
  x() = v.x();
  y() = v.y();
}


/**
 * Instantiate a vector with coordinates [x], [y]
 * @param x the first value
 * @param y the second value
 */
ImaGene::Vector2D::Vector2D( float x, float y )
  : Vector( 2, m_static_data )
{
  this->x() = x;
  this->y() = y;
}



/**
 * Copy constructor with conversion (no shared information).
 *
 * @pre v must be of size (at least) 2.
 * @param v the vector to copy.
 */
ImaGene::Vector2D::Vector2D( const Vector& v )
  : Vector( 2, m_static_data )
{
  ASSERT_Vector( v.size() >= 2 );
  x() = v.ro( 0 );
  y() = v.ro( 1 );
}



/**
 * Instantiates the vector [v]-[u].
 * @param u any vector of size (at least) 2.
 * @param v any vector of size (at least) 2.
 */
ImaGene::Vector2D::Vector2D(const Vector& u, const Vector& v)
  : Vector( 2, m_static_data )
{
  ASSERT_Vector( ( u.size() >= 2 ) && ( v.size() >= 2 ) );
  x() = v.ro( 0 ) - u.ro( 0 );
  y() = v.ro( 1 ) - u.ro( 0 );
}


/**
 * Assignment.
 * @param v any vector of size (at least) 2.
 * @return a reference on this.
 */
ImaGene::Vector2D& 
ImaGene::Vector2D::operator= (const Vector& v)
{
  ASSERT_Vector( v.size() >= 2 );
  if ( this != &v )
    {
      x() = v.ro( 0 );
      y() = v.ro( 1 );
    }
  return *this;
}


/**
 * Assignment.
 * @param v any vector2D.
 * @return a reference on this.
 */
ImaGene::Vector2D& 
ImaGene::Vector2D::operator= (const Vector2D& v)
{
  if ( this != &v )
    {
      x() = v.x();
      y() = v.y();
    }
  return *this;
}





///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
