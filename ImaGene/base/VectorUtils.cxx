///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : VectorUtils.cxx
//
// Creation : 2004/11/22
//
// Version : 2004/11/22
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
//	2004/11/22 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include "ImaGene/base/VectorUtils.h"
//#include "ImaGene/planeRecognition/utils.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/base/VectorUtils.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const VectorUtils_RCS_ID = "@(#)class VectorUtils definition.";

///////////////////////////////////////////////////////////////////////////////
// class VectorUtils
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Static services - public :

/**
 * @param v any vector.
 * @return its euclidean norm.
 */
float 
ImaGene::VectorUtils::norm( const Vector2D & v )
{
  return sqrt( v.ro( 0 ) * v.ro( 0 ) + v.ro( 1 ) * v.ro( 1 ) );
}

/**
 * @param v (returns) normalize the vector so that its euclidean norm is 1.
 */
void
ImaGene::VectorUtils::normalize( Vector2D & v )
{
  float n = norm( v );
  v /= n;
}

/**
 * @param v1 any vector.
 * @param v2 any vector.
 * @return the norm of the vector [v1]-[v2], ie the distance between [v1] and [v2] when seen as points.
 */
float 
ImaGene::VectorUtils::distance( const Vector2D & v1, const Vector2D & v2 )
{ 
  Vector2D v3( v1 );
  v3 -= v2;
  return norm( v3 );
}

/**
 * @param v1 any vector.
 * @param v2 any vector.
 * @return the dot product of [v1] and [v2].
 */
float
ImaGene::VectorUtils::dotProduct( const Vector2D & v1, const Vector2D & v2 )
{ 
  return v1.ro( 0 ) * v2.ro( 0 ) + v1.ro( 1 ) * v2.ro( 1 );
}

/**
 * @param v any vector.
 * @param s any scalar.
 * @return v*s.
 */
ImaGene::Vector2D
ImaGene::VectorUtils::product( const Vector2D & v, const float & s )
{ 
  Vector2D result = Vector2D(v.x()*s,v.y()*s);
  return result;
}

// /**
//  * @param u any vector.
//  * @param sign signe for the bezout (1 or -1).
//  * @return the Bezout vector of u.
//  */
// ImaGene::Vector2D
// ImaGene::VectorUtils::bezout( const Vector2D & u, const int sign )
// { 
//   P bez = ExtendedEuclid(-u.y(), u.x(), sign);
//   int x = get_si( bez.x );
//   int y = get_si( bez.y );
//   Vector2D result = Vector2D(x,y);
//   return result;
// }

// /**
//  * @param u any vector.
//  * @param sign signe for the bezout (1 or -1).
//  * @return the Bezout vector of u.
//  */
// ImaGene::Vector2D
// ImaGene::VectorUtils::closestBezout( const Vector2D & u, const int sign )
// { 
//   P bez = ExtendedEuclid(-u.y(), u.x(), sign);
//   int x = get_si( bez.x );
//   int y = get_si( bez.y );
//   Vector2D result = Vector2D(x,y);
//   if(u.x()>=0)
//   {
//     while(result.x()<=u.x())
//       result += u;
//     result -= u;
//   }
//   else
//   {
//     while(result.x()>=u.x())
//       result += u;
//     result -= u;
//   }
//   return result;
// }

// /**
//  * @param u any vector.
//  * @param sign sign for the bezout (1 or -1).
//  * @return the Bezout vector of u.
//  */
// ImaGene::Vector2D
// ImaGene::VectorUtils::bezoutWithConstraints( const Vector2D & u, const int sign,  Vector2i A, int & borneSup )
// {   
//   Vector2D bez = bezout( u, sign );
//   Vector2i bezi = Vector2i(bez.x(),bez.y());

//   Vector2i p = A;
//   p += bezi;  //p=A+bez
//   Vector2i N = Vector2i(1,0);
//   int dp1 = p.x()*N.x() + p.y()*N.y();
//   int dp2 = u.x()*N.x() + u.y()*N.y();
//   int fl = get_si( floor(borneSup-dp1,dp2) ); //.get_si();
//   int ce = get_si( ceil(borneSup-dp1,dp2) ); //.get_si();
  
//   if( p.x()<=borneSup )
//   {
//     //utiliser fl
//     bez +=  product(u,fl);
//   }
//   else
//   {
//     //utiliser ce
//     bez +=  product(u,ce);
//   }

//   return bez;
// }

float
ImaGene::VectorUtils::det( const Vector2D & v1, const Vector2D & v2 )
{ 
  return v1.ro( 0 ) * v2.ro( 1 ) - v1.ro( 1 ) * v2.ro( 0 );
}

/**
 * @param v any vector.
 * @return the angle of [v] with the x-axis.
 */
float
ImaGene::VectorUtils::getAngle( const Vector2D & v )
{ 
  return atan2f( v.ro( 1 ), v.ro( 0 ) );
}



///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::VectorUtils::~VectorUtils()
{
}

///////////////////////////////////////////////////////////////////////////////
// Interface - public :


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
