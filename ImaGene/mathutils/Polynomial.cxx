///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Polynomial.cxx
//
// Creation : 2004/08/10
//
// Version : 2004/08/10
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
//	2004/08/10 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/Polynomial.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/Polynomial.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const Polynomial_RCS_ID = "@(#)class Polynomial definition.";

///////////////////////////////////////////////////////////////////////////////
// class Polynomial
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static services --------------------------------


/**
 * A bell shape between 0 and 1 with border conditions.
 * Defined as: p(0)=0, p(0.5)=a, p(1)=0.
 * p'(0)=0, p'(1)=0, p"(0)=0, p"(1)=0.
 * @return the corresponding polynomial.
 */
ImaGene::Polynomial
ImaGene::Polynomial::bellShape2( float a )
{
  Polynomial p( 6 );
  p.setCoef( 0, 0.0f );
  p.setCoef( 1, 0.0f );
  p.setCoef( 2, 0.0f );
  p.setCoef( 3, 64*a );
  p.setCoef( 4, -192*a );
  p.setCoef( 5, 192*a );
  p.setCoef( 6, -64*a );
  return p;
}

 

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::Polynomial::~Polynomial()
{
}


/**
 * Evaluates the polynomial at [x].
 * @param x any value.
 */
float
ImaGene::Polynomial::value( float x ) const
{
  const PolynomialData & p = m_data.ro();
  int k = p.size - 1;
  if ( k >= 0 )
    {
      float y = p.coefs[ k ];
      while ( --k >= 0 )
	y = y * x + p.coefs[ k ];
      return y;
    }
  return 0.0f;
}


/**
 * @return the derivative polynomial.
 */
ImaGene::Polynomial 
ImaGene::Polynomial::derivative() const
{
  if ( order() == 0 ) return Polynomial();
  Polynomial d( order() - 1 );
  for ( int i = 0; i < order(); ++i )
    d.setCoef( i, ( i + 1 ) * getCoef( i + 1 ) );
  return d;
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::Polynomial::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Polynomial]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::Polynomial::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
