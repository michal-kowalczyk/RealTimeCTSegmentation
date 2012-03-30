///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : SimpleLinearRegression.cxx
//
// Creation : 2009/09/01
//
// Version : 2009/09/01
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
//	2009/09/01 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
//#include <boost/math/distributions/students_t.hpp>
#include "ImaGene/mathutils/SimpleLinearRegression.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/mathutils/SimpleLinearRegression.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const SimpleLinearRegression_RCS_ID = "@(#)class SimpleLinearRegression definition.";



///////////////////////////////////////////////////////////////////////////////
// class SimpleLinearRegression
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::SimpleLinearRegression::~SimpleLinearRegression()
{
}

/**
 * Constructor.
 * The object is invalid.
 *
 * @param eps_zero the value below which the absolute value of the
 * determinant is considered null.
 */
ImaGene::SimpleLinearRegression::SimpleLinearRegression( double eps_zero )
  : epsilon_zero( eps_zero ), m_n( 0 )
{
  clear();
}

/**
 * Clear all datas.
 */
void
ImaGene::SimpleLinearRegression::clear()
{
  m_n = 0;
  m_sum_x = 0.0;
  m_sum_x2 = 0.0;
  m_sum_y = 0.0;
  m_sum_xy = 0.0;
  m_Y.clear();
  m_X.clear();
  m_B[ 0 ] = 0.0;
  m_B[ 1 ] = 0.0;
  m_d = 0.0;
}

/**
 * Computes the regression of the current parameters.
 *
 * @return 'true' if the regression was valid (non null number of
 * samples, rank of X is 2), 'false' otherwise.
 */
bool
ImaGene::SimpleLinearRegression::computeRegression()
{
  if ( m_n <= 2 ) return false;
  m_d = m_n * m_sum_x2 - ( m_sum_x * m_sum_x );
  if ( ( m_d > -epsilon_zero ) && ( m_d < epsilon_zero ) )
    {
      m_d = 0.0;
      return false;
    }
  m_B[ 0 ] = ( m_sum_x2 * m_sum_y - m_sum_x * m_sum_xy ) / m_d;
  m_B[ 1 ] = ( -m_sum_x * m_sum_y + m_n * m_sum_xy ) / m_d;

  m_U.clear();
  m_norm_U2 = 0.0;
  for ( unsigned int i = 0; i < m_n; ++i )
    {
      double u = m_Y[ i ] - m_B[ 0 ] - m_B[ 1 ] * m_X[ i ];
      m_U.push_back( u );
      m_norm_U2 += u * u;
    }
  return true;
}




///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::SimpleLinearRegression::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[SimpleLinearRegression]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::SimpleLinearRegression::OK() const
{
  return true;
}




///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
