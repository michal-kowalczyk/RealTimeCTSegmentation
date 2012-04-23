///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : LinearMinimizer.cxx
//
// Creation : 2004/11/24
//
// Version : 2004/11/24
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
//	2004/11/24 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/helper/LinearMinimizer.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/LinearMinimizer.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const LinearMinimizer_RCS_ID = "@(#)class LinearMinimizer definition.";

///////////////////////////////////////////////////////////////////////////////
// class LinearMinimizer
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::LinearMinimizer::~LinearMinimizer()
{
  reset();
}


/**
 * Constructor. The object is not valid.
 */
ImaGene::LinearMinimizer::LinearMinimizer()
  : m_values( 0 )
{
}


/**
 * Reset all. The object is as if it has just been created.
 */
void 
ImaGene::LinearMinimizer::reset()
{
  if ( m_values != 0 )
    {
      delete[] m_values;
      m_values = 0;
    }
  m_sum = 0.0;
  m_max = 0.0;
}


/**
 * (Re-)initializes the linear minimizer.
 *
 * @param nb an upper bound for the number of values to optimize.
 */
void 
ImaGene::LinearMinimizer::init( uint nbmax )
{
  reset();
  m_values = new ValueInfo[ nbmax ];
  m_max_size = nbmax;
  m_size = nbmax;
  m_is_curve_open = false;
}




///////////////////////////////////////////////////////////////////////////////
// ------------------------- Optimization services --------------------------

/**
 * @param i1 the first value to be optimized (between 0 and 'size()-1').
 * @param i2 the value after the last to be optimized (between 0 and 'size()-1') * @return the current energy of the system.
 */
double
ImaGene::LinearMinimizer::getEnergy( uint i1, uint i2 ) const
{
  Mathutils::AngleComputer ac;
  Mathutils::ModuloComputer mc( size() );
  double E = 0.0;
  for ( uint i = mc.next( i1 ); i != i2; ) 
    {
      uint inext = mc.next( i );
      const ValueInfo & vi = this->ro( i );
      const ValueInfo & viprev = this->ro( mc.previous( i ) );
      E += Mathutils::sqr( ac.deviation( vi.value, viprev.value ) )
	/ viprev.dist_to_next;
      i = inext;
    }
  return E;
}

/**
 * @param i1 the first value to be optimized (between 0 and 'size()-1').
 * @param i2 the value after the last to be optimized (between 0 and 'size()-1') * @return the former energy of the system.
 */
double
ImaGene::LinearMinimizer::getFormerEnergy( uint i1, uint i2 ) const
{
  Mathutils::AngleComputer ac;
  Mathutils::ModuloComputer mc( size() );
  double E = 0.0;
  for ( uint i = mc.next( i1 ); i != i2; ) 
    {
      uint inext = mc.next( i );
      const ValueInfo & vi = this->ro( i );
      const ValueInfo & viprev = this->ro( mc.previous( i ) );
      E += Mathutils::sqr( ac.deviation( vi.old_value, viprev.old_value ) )
	/ viprev.dist_to_next;
      i = inext;
    }
  return E;
}

/**
 * @return the energy gradient computed at the current position.
 */
std::vector<double>
ImaGene::LinearMinimizer::getGradient() const
{
  Mathutils::AngleComputer ac;
  Mathutils::ModuloComputer mc( size() );

  vector<double> grad( size() );
  for ( uint i = 0; i < size(); i++ ) 
    {
      uint iprev = mc.previous( i );
      uint inext = mc.next( i );
      const ValueInfo & vi = this->ro( i );
      double val = vi.value;
      if ( m_is_curve_open && ( i == ( size() - 1 ) ) )
	{ // free extremity to the front/right.
	  const ValueInfo & viprev = this->ro( iprev );
	  double valp = viprev.value; 
	  grad[ i ] = 2.0 * ac.deviation( val, valp ) / viprev.dist_to_next;
	}
      else if ( m_is_curve_open && ( i ==  0 ) )
	{ // free extremity to the back/left.
	  const ValueInfo & vinext = this->ro( inext );
	  double valn = vinext.value;
	  grad[ i ] = -2.0 * ac.deviation( valn, val ) / vi.dist_to_next;
	}
      else
	{ // standard case.
	  const ValueInfo & viprev = this->ro( iprev );
	  const ValueInfo & vinext = this->ro( inext );
	  double valp = viprev.value; 
	  double valn = vinext.value;
	  grad[ i ] = 2.0*( ac.deviation( val, valp ) / viprev.dist_to_next 
			    - ac.deviation( valn, val ) / vi.dist_to_next 
			    ) ;
	}
    }
  return grad;
}

/**
 * @return the energy gradient computed at the former position.
 */
std::vector<double>
ImaGene::LinearMinimizer::getFormerGradient() const
{
  Mathutils::AngleComputer ac;
  Mathutils::ModuloComputer mc( size() );

  vector<double> grad( size() );
  for ( uint i = 0; i < size(); i++ ) 
    {
      uint iprev = mc.previous( i );
      uint inext = mc.next( i );
      const ValueInfo & vi = this->ro( i );
      double val = vi.old_value;
      if ( m_is_curve_open && ( i == ( size() - 1 ) ) )
	{ // free extremity to the front/right.
	  const ValueInfo & viprev = this->ro( iprev );
	  double valp = viprev.old_value; 
	  grad[ i ] = 2.0 * ac.deviation( val, valp ) / viprev.dist_to_next;
	}
      else if ( m_is_curve_open && ( i ==  0 ) )
	{ // free extremity to the back/left.
	  const ValueInfo & vinext = this->ro( inext );
	  double valn = vinext.old_value;
	  grad[ i ] = -2.0 * ac.deviation( valn, val ) / vi.dist_to_next;
	}
      else
	{ // standard case.
	  const ValueInfo & viprev = this->ro( iprev );
	  const ValueInfo & vinext = this->ro( inext );
	  double valp = viprev.old_value; 
	  double valn = vinext.old_value;
	  grad[ i ] = 2.0*( ac.deviation( val, valp ) / viprev.dist_to_next 
			    - ac.deviation( valn, val ) / vi.dist_to_next 
			    ) ;
	}
    }
  return grad;
}

  
/**
 * Move each value on the straight line joining its neighboring
 * values while maintaining the value between its bounds.
 * The optimization is performed on all values.
 *
 * @return the sum of the displacements.
 */
double 
ImaGene::LinearMinimizer::optimize()
{
  return optimize( 0, 0 );
}


/**
 * Move each value on the straight line joining its neighboring
 * values while maintaining the value between its bounds.
 * The optimization is performed on values [i1] included to [i2] excluded.
 *
 * @param i1 the first value to be optimized (between 0 and 'size()-1').
 * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
 * @return the sum of the displacements.
 */
double 
ImaGene::LinearMinimizer::optimize( uint i1, uint i2 )
{
//   static uint istep = 0;
//   istep = istep == 0 ? 1 : 0;

  ASSERT_LinearMinimizer( size() > 2 );

  Mathutils::AngleComputer ac;
  Mathutils::ModuloComputer mc( size() );

  uint i = i1;
  // (I) first pass to work on old values.
  do 
    {
      ValueInfo & vi = this->rw( i );
      vi.old_value = vi.value;
      // go to next.
      i = mc.next( i );
    }
  while ( i != i2 );

//   // (DEBUG) : solution for disk.
//   int nb = 0; // JOL
//   double D = 0.0; // JOL
//   do 
//     {
//       ValueInfo & vi = this->rw( i );
//       vi.old_value = vi.value;
//       // go to next.
//       i = mc.next( i );


//       D += vi.dist_to_next; // JOL
//       ++nb; // JOL

//     }
//   while ( i != i2 );

//   // JOL calcul solution => ok
//   double A = M_PI / 2.0; // JOL
//   double B = 2.0 * M_PI / D;
//   i = i1;
//   do 
//     {
//       ValueInfo & vi = this->rw( i );
//       cout << "Solution " << i << " " << ac.cast( A ) << " " << (vi.dist_to_next/D) << endl;
//       vi.old_value = vi.value = ac.cast( A );
//       A += B * vi.dist_to_next;
//       i = mc.next( i );
//     } // JOL
//   while ( i != i2 ); 
//   // (DEBUG) : FIN JOL

  this->oneStep( i1, i2 );
  
  m_sum = 0.0;
  m_max = 0.0;
  i = i1;
  do 
    {
      const ValueInfo & vi = this->ro( i );
      double diff = fabs( ac.deviation( vi.value, vi.old_value ) );
      if ( diff > m_max )
	m_max = diff;
      m_sum += diff;
      i = mc.next( i );
    }
  while ( i != i2 );

//   double E1 = 0.0;
//   double E2 = 0.0;
//   double E3 = 0.0;
//   for ( i = mc.next( i1 ); i != i2; ) 
//     {
//       uint inext = mc.next( i );
//       const ValueInfo & vi = this->ro( i );
//       const ValueInfo & viprev = this->ro( mc.previous( i ) );
//       E1 += Mathutils::sqr( ac.deviation( vi.old_value, viprev.old_value ) )
// 	/ viprev.dist_to_next;
//       E2 += Mathutils::sqr( ac.deviation( vi.value, viprev.value ) )
// 	/ viprev.dist_to_next;
//       E3 += viprev.dist_to_next;
//       i = inext;
//     }

  
//    cerr << "Energy (" << E3 << ") from " << E1 << " -> " << E2 
//         << " D=" << (E2 - E1) << endl;
  return m_sum;
}


/**
 * Should be used to stop the minimization process. The smaller is
 * this value, the more the optimization is at an end. May have
 * several meanings, like the infinite norm of the last displacement
 * or the infinite norm of the projected gradient.
 *
 * @return an upper bound on the norm of the last displacement. 
 */
double
ImaGene::LinearMinimizer::lastDelta() const
{
  return max();
}


/**
 * The method which performs the optimization effectively. The user
 * may override it. The optimization is performed on values [i1]
 * included to [i2] excluded. Afterwards, the field ValueInfo::value
 * should contain the new value.
 *
 * @param i1 the first value to be optimized (between 0 and 'size()-1').
 * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
 */
void
ImaGene::LinearMinimizer::oneStep( uint i1, uint i2 )
{
  Mathutils::AngleComputer ac;
  Mathutils::ModuloComputer mc( size() );

  double mid;
  uint i = i1; 
  uint iprev = mc.previous( i );
  do 
    {
      uint inext = mc.next( i );
      ValueInfo & vi = this->rw( i );
      //      vi.old_value = vi.value;
      if ( m_is_curve_open && ( i == ( size() - 1 ) ) )
	{ // free extremity to the front/right.
	  const ValueInfo & viprev = this->ro( iprev );
	  mid = viprev.old_value; // - viprev.delta;
	}
      else if ( m_is_curve_open && ( i ==  0 ) )
	{ // free extremity to the back/left.
	  const ValueInfo & vinext = this->ro( inext );
	  mid = vinext.old_value; // + vi.delta;
	}
      else
	{ // standard case.
	  const ValueInfo & viprev = this->ro( iprev );
	  const ValueInfo & vinext = this->ro( inext );
	  double valp = viprev.old_value; // - viprev.delta;
	  double valn = vinext.old_value; // + vi.delta;

	  // old
	  double y = ac.deviation( valn, valp );
	  mid = ( viprev.dist_to_next * y )
	    / ( vi.dist_to_next + viprev.dist_to_next );
	  mid = ac.cast( mid + valp );

	}
      if ( ac.less( mid, vi.min ) ) mid = vi.min;
      if ( ac.less( vi.max, mid ) ) mid = vi.max;
      double mvt = ac.deviation( mid, vi.old_value );
      vi.value = ac.cast( vi.old_value + 0.5 * mvt );
      
      // vi.value = ac.cast( vi.old_value + 0.5 * mvt );
      // vi.value = mid;

//       cout << "[optimize] i=" << i 
// 	   << " v=[" << vi.min << "," << vi.value << "," << vi.max << "]"
// 	   << endl;
      
      // go to next.
      iprev = i;
      i = inext;
//       if ( i == i2 ) break;
//       inext = mc.next( i );
//       iprev = i;
//       i = inext;      
    }
  while ( i != i2 );
  

}




/**
 * The method which performs the optimization effectively. The user
 * may override it. The optimization is performed on values [i1]
 * included to [i2] excluded. Afterwards, the field ValueInfo::value
 * should contain the new value.
 *
 * @param i1 the first value to be optimized (between 0 and 'size()-1').
 * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
 */
void
ImaGene::LinearMinimizerByRelaxation::oneStep( uint i1, uint i2 )
{
  Mathutils::AngleComputer ac;
  Mathutils::ModuloComputer mc( size() );

  double mid;
  uint i = i1; 
  uint iprev = mc.previous( i );
  do 
    {
      uint inext = mc.next( i );
      ValueInfo & vi = this->rw( i );
      //      vi.old_value = vi.value;
      if ( m_is_curve_open && ( i == ( size() - 1 ) ) )
	{ // free extremity to the front/right.
	  const ValueInfo & viprev = this->ro( iprev );
	  mid = viprev.value; // - viprev.delta;
	}
      else if ( m_is_curve_open && ( i ==  0 ) )
	{ // free extremity to the back/left.
	  const ValueInfo & vinext = this->ro( inext );
	  mid = vinext.old_value; // + vi.delta;
	}
      else
	{ // standard case.
	  const ValueInfo & viprev = this->ro( iprev );
	  const ValueInfo & vinext = this->ro( inext );
	  double valp = viprev.value; // - viprev.delta;
	  double valn = vinext.value;
	    // : vinext.old_value; 

	  // old
	  double y = ac.deviation( valn, valp );
	  mid = ( viprev.dist_to_next * y )
	    / ( vi.dist_to_next + viprev.dist_to_next );
	  mid = ac.cast( mid + valp );
// 	  cerr << i << " y=" << y << " valp=" << valp
// 	       << " valn=" << valn 
// 	       << " mid1=" << mid
// 	       << " dp=" << viprev.dist_to_next
// 	       << " dn=" << vi.dist_to_next << endl;
	}
      if ( ac.less( mid, vi.min ) ) mid = vi.min;
      if ( ac.less( vi.max, mid ) ) mid = vi.max;
      vi.value = mid;
      //cerr << " mid2=" <<vi.min << "<" <<  mid << "<" << vi.max << endl;

//       cout << "[optimize] i=" << i 
// 	   << " v=[" << vi.min << "," << vi.value << "," << vi.max << "]"
// 	   << endl;
      
      // go to next.
      iprev = i;
      i = inext;
//       if ( i == i2 ) break;
//       inext = mc.next( i );
//       iprev = i;
//       i = inext;      
    }
  while ( i != i2 );
  
//   double E1 = getFormerEnergy( i1, i2 );
//   double E2 = getEnergy( i1, i2 );
//   cerr << "E1=" << E1 << " E2=" << E2 << " s=RLX" << endl;

}

/**
 * Should be used to stop the minimization process. The smaller is
 * this value, the more the optimization is at an end. May have
 * several meanings, like the infinite norm of the last displacement
 * or the infinite norm of the projected gradient.
 *
 * @return an upper bound on the norm of the last displacement. 
 */
double
ImaGene::LinearMinimizerByRelaxation::lastDelta() const
{
  return max();
}



/**
 * The method which performs the optimization effectively. The user
 * may override it. The optimization is performed on values [i1]
 * included to [i2] excluded. Afterwards, the field ValueInfo::value
 * should contain the new value.
 *
 * @param i1 the first value to be optimized (between 0 and 'size()-1').
 * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
 */
void
ImaGene::LinearMinimizerByGradientDescent::oneStep( uint i1, uint i2 )
{
  Mathutils::AngleComputer ac;
  Mathutils::ModuloComputer mc( size() );

  vector<double> grad ( getFormerGradient() );
  double mid;
  uint i = i1; 
  do 
    {
      uint inext = mc.next( i );
      ValueInfo & vi = this->rw( i );
      double val = vi.old_value;
      mid = ac.cast( val - m_step*grad[ i ] );
      if ( ac.less( mid, vi.min ) ) mid = vi.min;
      if ( ac.less( vi.max, mid ) ) mid = vi.max;
      vi.value = mid;
      // go to next.
      i = inext;
    }
  while ( i != i2 );
  double E1 = getFormerEnergy( i1, i2 );
  double E2 = getEnergy( i1, i2 );
  cerr << "E1=" << E1 << " E2=" << E2 << " s=" << m_step << endl;
  

}

/**
 * Should be used to stop the minimization process. The smaller is
 * this value, the more the optimization is at an end. May have
 * several meanings, like the infinite norm of the last displacement
 * or the infinite norm of the projected gradient.
 *
 * @return an upper bound on the norm of the last displacement. 
 */
double
ImaGene::LinearMinimizerByGradientDescent::lastDelta() const
{
  vector<double> grad ( getFormerGradient() );
  double ninf = 0.0;
  for ( uint i = 0; i < size(); i++ ) 
    {
      const ValueInfo & vi = this->ro( i );
      if ( vi.value != vi.old_value )
	{
	  double n = fabs( grad[ i ] );
	  if ( n > ninf ) ninf = n;
	}
    }
  return ninf;
}



/**
 * The method which performs the optimization effectively. The user
 * may override it. The optimization is performed on values [i1]
 * included to [i2] excluded. Afterwards, the field ValueInfo::value
 * should contain the new value.
 *
 * @param i1 the first value to be optimized (between 0 and 'size()-1').
 * @param i2 the value after the last to be optimized (between 0 and 'size()-1').
 */
void
ImaGene::LinearMinimizerByAdaptiveStepGradientDescent::oneStep( uint i1, uint i2 )
{
  Mathutils::AngleComputer ac;
  Mathutils::ModuloComputer mc( size() );
  vector<double> grad ( getFormerGradient() );

  double mid;
  uint i = i1; 
  //  uint iprev = mc.previous( i );
  do 
    {
      uint inext = mc.next( i );
      ValueInfo & vi = this->rw( i );
      double val = vi.old_value;
      mid = ac.cast( val - m_step*grad[ i ] );
      if ( ac.less( mid, vi.min ) ) mid = vi.min;
      if ( ac.less( vi.max, mid ) ) mid = vi.max;
      vi.value = mid;
      // go to next.
      i = inext;

//       //      vi.old_value = vi.value;
//       if ( m_is_curve_open && ( i == ( size() - 1 ) ) )
// 	{ // free extremity to the front/right.
// 	  const ValueInfo & viprev = this->ro( iprev );
// 	  mid = viprev.value; // - viprev.delta;
// 	  cerr << "[LinearMinimizerByAdaptiveStepGradientDescent::oneStep] NOT IMPLEMENTED"
// 	       << endl;
// 	}
//       else if ( m_is_curve_open && ( i ==  0 ) )
// 	{ // free extremity to the back/left.
// 	  const ValueInfo & vinext = this->ro( inext );
// 	  mid = vinext.old_value; // + vi.delta;
// 	  cerr << "[LinearMinimizerByAdaptiveStepGradientDescent::oneStep] NOT IMPLEMENTED"
// 	       << endl;
// 	}
//       else
// 	{ // standard case.
// 	  const ValueInfo & viprev = this->ro( iprev );
// 	  const ValueInfo & vinext = this->ro( inext );
// 	  double valp = viprev.old_value; // - viprev.delta;
// 	  double valn = vinext.old_value;
// 	  double val = vi.old_value;
// 	    // : vinext.old_value; 

// 	  // old
// 	  double grad = 2.0*( ac.deviation( val, valp ) / viprev.dist_to_next 
// 			      - ac.deviation( valn, val ) / vi.dist_to_next 
// 			      ) ;
// 	  mid = ac.cast( val - m_step*grad );
// // 	  cerr << i << " y=" << y << " valp=" << valp
// // 	       << " valn=" << valn 
// // 	       << " mid1=" << mid
// // 	       << " dp=" << viprev.dist_to_next
// // 	       << " dn=" << vi.dist_to_next << endl;
// 	}
//       if ( ac.less( mid, vi.min ) ) mid = vi.min;
//       if ( ac.less( vi.max, mid ) ) mid = vi.max;
//       vi.value = mid;
//       //cerr << " mid2=" <<vi.min << "<" <<  mid << "<" << vi.max << endl;

// //       cout << "[optimize] i=" << i 
// // 	   << " v=[" << vi.min << "," << vi.value << "," << vi.max << "]"
// // 	   << endl;
      
//       // go to next.
//       iprev = i;
//       i = inext;
// //       if ( i == i2 ) break;
// //       inext = mc.next( i );
// //       iprev = i;
// //       i = inext;      
    }
  while ( i != i2 );

  double E1 = getFormerEnergy( i1, i2 );
  double E2 = getEnergy( i1, i2 );
  cerr << "E1=" << E1 << " E2=" << E2 << " s=" << m_step << endl;
  if ( E1 <= E2 )
    {
      /* Going back to previous position. */
//       i = i1; 
//       do 
// 	{
// 	  uint inext = mc.next( i );
// 	  ValueInfo & vi = this->rw( i );
// 	  double tmp = vi.value;
// 	  vi.value = vi.old_value;
// 	  vi.old_value = tmp;
// 	  i = inext;
// 	}
//       while ( i != i2 );
      m_step /= 4.0;
    }
  else
    {
      /* doubling step. */
      m_step *= 2.0;
    }
}

/**
 * Should be used to stop the minimization process. The smaller is
 * this value, the more the optimization is at an end. May have
 * several meanings, like the infinite norm of the last displacement
 * or the infinite norm of the projected gradient.
 *
 * @return an upper bound on the norm of the last displacement. 
 */
double
ImaGene::LinearMinimizerByAdaptiveStepGradientDescent::lastDelta() const
{
  vector<double> grad ( getFormerGradient() );
  double ninf = 0.0;
  for ( uint i = 0; i < size(); i++ ) 
    {
      const ValueInfo & vi = this->ro( i );
      if ( vi.value != vi.old_value )
	{
	  double n = fabs( grad[ i ] );
	  if ( n > ninf ) ninf = n;
	}
    }
  return ninf;
}


// ///////////////////////////////////////////////////////////////////////////////
// // ------------------------- Other services --------------------------

// /**
//  * The index is the greatest one lesser or equal to [cabs].
//  * @param cabs a valid curvilinear abscissa on the contour.
//  * @return the index of the value corresponding to [cabs].
//  */
// uint 
// ImaGene::LinearMinimizer::indexFromCurvilinearAbscissa( double cabs )
// {
//   uint i1 = 0;
//   uint i2 = size();
//   ValueInfo vi;
//   vi.curv_abscissa = cabs;
//   return upper_bound( m_values + i1, m_values + i2, vi, 
// 		      ValueInfoCurvAbsCmp() )
//     - m_values;
// }


// /**
//  * The index is the greatest one lesser or equal to [dabs].
//  * @param dabs a valid discrete abscissa on the contour.
//  * @return the index of the value corresponding to [dabs].
//  */
// uint 
// ImaGene::LinearMinimizer::indexFromDiscreteAbscissa( double dabs )
// {
//   uint i2 = size();
//   ValueInfo vi;
//   vi.disc_abscissa = dabs;
//   if ( m_values[ 1 ].disc_abscissa > dabs ) return 0;
//   uint idx = upper_bound( m_values + 1, m_values + i2, vi, 
// 			  ValueInfoDiscAbsCmp() )
//     - m_values;
//   if ( idx == i2 ) return 0;
//   else return idx;
// }


// /**
//  * @param cabs a valid curvilinear abscissa on the contour.
//  * @return the associated value (which is a linear interpolation).
//  */
// double 
// ImaGene::LinearMinimizer::valueFromCurvilinearAbscissa( double cabs )
// {
//   Mathutils::AngleComputer ac;
//   uint i = indexFromCurvilinearAbscissa( cabs );
//   uint in = ( i + 1 ) % size();
//   const ValueInfo & vi = ro( i );
//   const ValueInfo & vin = ro( in );
//   return ac.cast( ac.deviation( vin.value, vi.value ) 
// 		  * ( cabs - vi.curv_abscissa ) 
// 		  / vi.dist_to_next + vi.value );
// }


// /**
//  * @param cabs a valid curvilinear abscissa on the contour.
//  * @return the slope at this position (it is not a continuous function).
//  */
// double 
// ImaGene::LinearMinimizer::slopeFromCurvilinearAbscissa( double cabs )
// {
//   Mathutils::AngleComputer ac;
//   uint i = indexFromCurvilinearAbscissa( cabs );
//   uint in = ( i + 1 ) % size();
//   const ValueInfo & vi = ro( i );
//   const ValueInfo & vin = ro( in );
//   return ac.deviation( vin.value, vi.value )
//     / vi.dist_to_next;
// }


// /**
//  * @param dabs a valid discrete abscissa on the contour.
//  * @return the associated value (which is a linear interpolation).
//  */
// double 
// ImaGene::LinearMinimizer::valueFromDiscreteAbscissa( double dabs )
// {
//   Mathutils::AngleComputer ac;

//   double max_dabs = ro( size() - 1 ).disc_abscissa + 1.0f;
//   uint i = indexFromDiscreteAbscissa( dabs );
//   uint in = ( i + 1 ) % size();
//   const ValueInfo & vi = ro( i );
//   const ValueInfo & vin = ro( in );
//   cout << "dabs=" << dabs << " vi.dabs=" << vi.disc_abscissa << endl;
//   double delta = dabs - vi.disc_abscissa;
//   if ( delta < 0.0f ) delta += max_dabs;
//   //ASSERT_LinearMinimizer( delta < 1.0f );
//   return ac.cast( ac.deviation( vin.value, vi.value ) * delta
// 		  + vi.value );
// }


// /**
//  * @param dabs a valid discrete abscissa on the contour.
//  * @return the slope at this position (it is not a continuous function).
//  */
// double 
// ImaGene::LinearMinimizer::slopeFromDiscreteAbscissa( double dabs )
// {
//   Mathutils::AngleComputer ac;

//   uint i = indexFromDiscreteAbscissa( dabs );
//   uint in = ( i + 1 ) % size();
//   const ValueInfo & vi = ro( i );
//   const ValueInfo & vin = ro( in );

//   return ac.deviation( vin.value, vi.value )
//     / vi.dist_to_next;
// }

  
  
 



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::LinearMinimizer::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[LinearMinimizer::standard 0.5]";
}

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::LinearMinimizerByRelaxation::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[LinearMinimizer::relaxation]";
}

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::LinearMinimizerByGradientDescent::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[LinearMinimizer::gradient descent " << m_step << "]";
}

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::LinearMinimizerByAdaptiveStepGradientDescent::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[LinearMinimizer::gradient descent with adaptive step " << m_step << "]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::LinearMinimizer::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
