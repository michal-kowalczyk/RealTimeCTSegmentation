///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : GlobalC4CGeometry.cxx
//
// Creation : 2004/11/25
//
// Version : 2004/11/25
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
//	2004/11/25 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <algorithm>
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/mathutils/Statistics.h"
#include "ImaGene/helper/GlobalC4CGeometry.h"
#include "ImaGene/digitalnD/Frame2D.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "GlobalC4CGeometry.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const GlobalC4CGeometry_RCS_ID = "@(#)class GlobalC4CGeometry definition.";

///////////////////////////////////////////////////////////////////////////////
// class GlobalC4CGeometry
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ------------------------- Static services ------------------------------

/**
 * Displays the geometry of [global_geom] on the output stream [out].
 * @param out any output stream.
 * @param global_geom any (initialized and computed) global geometry object.
 */
void
ImaGene::GlobalC4CGeometry::displayGeometry
( ostream & out,
  const GlobalC4CGeometry & global_geom )
{
  out << "# Geometry of common parts." << endl
      << "# cp back afront bms ems a1 a2 min max l cabs dl dabs est_c" << endl
      << "# Nb common parts is " << global_geom.nbCommonParts() << endl
      << "# Modulo surfel is " << global_geom.m_nb_surfels << endl
      << "# Modulo maximal segment is " << global_geom.m_nb_ms << endl;
  
  Mathutils::AngleComputer ac;
  Statistics scurv( 1 );

  for ( uint i = 0; i < global_geom.nbCommonParts(); ++i )
    {
      const C4CTangentialCover::CommonPart & cp 
	= global_geom.getCommonPart( i );
      const GlobalC4CGeometry::CommonPartGeometry & cpgeom 
	= global_geom.getCommonPartGeometry( i );
      double c = ac.deviation( cpgeom.angle2, cpgeom.angle1 ) 
	/ cpgeom.length;
      out << i 
	  << " " << cp.back_surfel_idx 
	  << " " << cp.after_front_surfel_idx
	  << " " << cp.begin_ms
	  << " " << cp.end_ms
	  << " " << cpgeom.angle1
	  << " " << cpgeom.angle2
	  << " " << cpgeom.min
	  << " " << cpgeom.max
	  << " " << cpgeom.length
	  << " " << cpgeom.curv_abscissa
	  << " " << cpgeom.disc_length
	  << " " << cpgeom.disc_abscissa
	  << " " << c
	  << endl;
      scurv.addValue( 0, c );
    }
  scurv.terminate();
  out << "# Stats curvature estimation :" << endl
      << "# nbs mean min max var uvar sqrt(var) sqrt(uvar) rel_min% rel_max% " << endl
      << "# " << scurv.samples( 0 )
      << " " << scurv.mean( 0 )
      << " " << scurv.min( 0 )
      << " " << scurv.max( 0 )
      << " " << scurv.variance( 0 )
      << " " << scurv.unbiasedVariance( 0 )
      << " " << sqrt( scurv.variance( 0 ) )
      << " " << sqrt( scurv.unbiasedVariance( 0 ) )
      << " " << 100.0 * ( scurv.mean( 0 ) - scurv.min( 0 ) ) / scurv.mean( 0 )
      << " " << 100.0 * ( scurv.max( 0 ) - scurv.mean( 0 ) ) / scurv.mean( 0 )
      << endl;
}



///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::GlobalC4CGeometry::~GlobalC4CGeometry()
{
  reset();
}



/**
 * Constructor. Does nothing. The object is not valid.
 */
ImaGene::GlobalC4CGeometry::GlobalC4CGeometry()
  : m_minimizer( 0 ), m_cps( 0 ), m_cp_geoms( 0 )
{
  m_minimizer_owner = false;
}


/**
 * Resets the geometry. The object is in the same state as if it was
 * just created.
 */
void
ImaGene::GlobalC4CGeometry::reset()
{
  if ( m_minimizer_owner && ( m_minimizer != 0 ) ) delete m_minimizer;
  if ( m_cps != 0 ) delete[] m_cps;
  if ( m_cp_geoms != 0 ) delete[] m_cp_geoms;
  m_minimizer = 0;
  m_minimizer_owner = false;
  m_cps = 0;
  m_cp_geoms = 0;
}


/**
 * Initializes the object so that it can compute the geometry of the
 * contour given in parameter. The contour is represented by its
 * tangential cover [tcover] and needs also an iterator [it] to make
 * the correct change of frames.
 *
 * @param tcover the tangential cover of a digital curve.
 * @param tcover_geometry the geometry of the tangential cover [tcover].
 */
void
ImaGene::GlobalC4CGeometry::init
( const C4CTangentialCover & tcover,
  const C4CTangentialCoverGeometry & tcover_geometry )
{
  reset();

  Mathutils::ModuloComputer mcsu( tcover.nbSurfels() );
  Mathutils::ModuloComputer mcms( tcover.nbMaximalSegments() );
  Mathutils::AngleComputer ac;

  // m_size = tcover.nbSurfels();
  // m_surfels = new Kn_sid[ m_size ];
  m_max_nb_cp = tcover.nbMaximalSegments() * 2;
  m_nb_surfels = tcover.nbSurfels();
  m_nb_ms = tcover.nbMaximalSegments();
    
  m_minimizer_owner = true;
  m_minimizer = new LinearMinimizer;
  R2RFunction* lambda = new TriangleFunction;
  m_cps = new C4CTangentialCover::CommonPart[ m_max_nb_cp ];
  m_cp_geoms = new CommonPartGeometry[ m_max_nb_cp ];
  
  // Initialization.
  C4CTangentialCover::CommonPart cp = tcover.beginCP();
  uint s1 = cp.back_surfel_idx;
  C4CTangentialCover::SurfelMaximalSegments sms;
  uint b = 0;
  // double length_first_cp = 1.0;
  do
    {
//       cerr << "[init] b=" << b 
// 	   << " CP=(" << cp.begin_ms << "," << cp.end_ms << ")" 
// 	   << "-(" << cp.back_surfel_idx << "," 
// 	   << cp.after_front_surfel_idx << ")" 
// 	   << endl;
      // Updates the array of common parts.
      m_cps[ b ] = cp;
      
      // Computes the geometric quantities for the value [b].
      // LinearMinimizer::ValueInfo & vb = m_minimizer->rw( b );

      const C4CTangentialCoverGeometry::MaximalSegmentGeometry & msgeo 
	= tcover_geometry.geometry( cp.begin_ms );
      double ref_value = msgeo.angle_to_x;
      double min = msgeo.min_angle;
      double max = msgeo.max_angle;
      double angle = 0.0f;
      uint nb = 1;
      for ( uint i = mcms.next( cp.begin_ms );
	    i != cp.end_ms; 
	    mcms.increment( i ) )
	{
	  const C4CTangentialCoverGeometry::MaximalSegmentGeometry & msgeo2
	    = tcover_geometry.geometry( i );
	  min = ac.min( msgeo2.min_angle, min );
	  max = ac.max( max, msgeo2.max_angle );
	  angle += ac.deviation( msgeo2.angle_to_x, ref_value );
	  ++nb;
	}

      // Fills the CommonPart geometry.
      CommonPartGeometry & cpgeom = m_cp_geoms[ b ];
      cpgeom.angle1 = ac.cast( ( angle / (double) nb ) + ref_value );
      cpgeom.min = min;
      cpgeom.max = max;
      cpgeom.disc_abscissa = cp.back_surfel_idx;
      cpgeom.disc_length = mcsu.posDiff( cp.after_front_surfel_idx,
					 cp.back_surfel_idx );
      
      sms.begin_ms = cp.begin_ms;
      sms.end_ms = cp.end_ms;
//       cpgeom.length = 
// 	C4CTangentialCoverGeometry
// 	::lengthByAverage( tcover,
// 			   tcover_geometry,
// 			   sms,
// 			   cp.back_surfel_idx,
// 			   cp.after_front_surfel_idx );
      cpgeom.length = 
	C4CTangentialCoverGeometry
	::lengthByLambdaMS( tcover,
			    tcover_geometry,
			    *lambda,
			    sms,
			    cp.back_surfel_idx,
			    cp.after_front_surfel_idx );
      
      // Go to next
      tcover.nextCP( cp );
      ++b;
    } 
  while ( cp.back_surfel_idx != s1 );

  // Second loop to compute absolute information.
  double cabs = 0.0f;
  m_nb_cp = b;
  for ( uint i = 0; i < b; ++i )
    {
      m_cp_geoms[ i ].curv_abscissa = cabs;
      cabs += m_cp_geoms[ i ].length;
    }
  delete lambda;
}


/**
 * Initializes the object so that it can compute the geometry of the
 * contour given in parameter. The contour is represented by its
 * tangential cover [tcover] and its cover geometry
 * [tcover_geometry].
 *
 * @param tcover the tangential cover of a digital curve.
 * @param tcover_geometry the geometry of the tangential cover [tcover].
 * @param elength the elementary lengthes of the surfels.
 * @param lm a specialized linear minimizer.
 */
void
ImaGene::GlobalC4CGeometry::init
( const C4CTangentialCover & tcover,
  const C4CTangentialCoverGeometry & tcover_geometry,
  const C4CELength & elength,
  LinearMinimizer* lm )
{
  reset();

  Mathutils::ModuloComputer mcsu( tcover.nbSurfels() );
  Mathutils::ModuloComputer mcms( tcover.nbMaximalSegments() );
  Mathutils::AngleComputer ac;

  m_max_nb_cp = tcover.nbMaximalSegments() * 2;
  m_nb_surfels = tcover.nbSurfels();
  m_nb_ms = tcover.nbMaximalSegments();

  if ( lm != 0 )
    {
      m_minimizer = lm;
      m_minimizer_owner = false;
    }
  else
    {
      m_minimizer = new LinearMinimizer;
      m_minimizer_owner = true;
    }
      
  m_cps = new C4CTangentialCover::CommonPart[ m_max_nb_cp ];
  m_cp_geoms = new CommonPartGeometry[ m_max_nb_cp ];
  
  // Initialization.
  C4CTangentialCover::CommonPart cp = tcover.beginCP();
  uint s1 = cp.back_surfel_idx;
  uint b = 0;
  do
    {
      // Updates the array of common parts.
      m_cps[ b ] = cp;
      
      // Computes the geometric quantities for the value [b].
      const C4CTangentialCoverGeometry::MaximalSegmentGeometry & msgeo 
	= tcover_geometry.geometry( cp.begin_ms );
      double ref_value = msgeo.angle_to_x;
      double min = msgeo.min_angle;
      double max = msgeo.max_angle;
      double angle = 0.0f;
      uint nb = 1;
      for ( uint i = mcms.next( cp.begin_ms );
	    i != cp.end_ms; 
	    mcms.increment( i ) )
	{
	  const C4CTangentialCoverGeometry::MaximalSegmentGeometry & msgeo2
	    = tcover_geometry.geometry( i );
	  min = ac.min( msgeo2.min_angle, min );
	  max = ac.max( max, msgeo2.max_angle );
	  angle += ac.deviation( msgeo2.angle_to_x, ref_value );
	  ++nb;
	}

      // Fills the CommonPart geometry.
      CommonPartGeometry & cpgeom = m_cp_geoms[ b ];
      cpgeom.angle1 = ac.cast( ( angle / (double) nb ) + ref_value );
      cpgeom.min = min;
      cpgeom.max = max;
      cpgeom.disc_abscissa = cp.back_surfel_idx;
      cpgeom.disc_length = mcsu.posDiff( cp.after_front_surfel_idx,
					 cp.back_surfel_idx );
      
      cpgeom.length = elength.length( cp.back_surfel_idx,
				      cp.after_front_surfel_idx );
      
//       cerr << "[init] b=" << b 
// 	   << " CP=(" << cp.begin_ms << "," << cp.end_ms << ")" 
// 	   << "-(" << cp.back_surfel_idx << "," 
// 	   << cp.after_front_surfel_idx << ")" 
// 	   << " LEN=" << cpgeom.length
// 	   << endl;
      // Go to next
      tcover.nextCP( cp );
      ++b;
    } 
  while ( cp.back_surfel_idx != s1 );

  // Second loop to compute absolute information.
  double cabs = 0.0f;
  m_nb_cp = b;
  for ( uint i = 0; i < b; ++i )
    {
      m_cp_geoms[ i ].curv_abscissa = cabs;
      cabs += m_cp_geoms[ i ].length;
    }
}

  


/**
 * Called before any optimization is performed.
 */
void
ImaGene::GlobalC4CGeometry::prepareLinearMinimizer() 
{
  Mathutils::ModuloComputer mccp( nbCommonParts() );
  Mathutils::AngleComputer ac;
  
  m_minimizer->init( m_nb_cp );
  m_minimizer->setSize( m_nb_cp );
  for ( uint i = 0; i < m_nb_cp; ++i )
    {
      // LinearMinimizer::ValueInfo & vprev = m_minimizer->rw( i - 1 );
//       LinearMinimizer::ValueInfo & vcur = m_minimizer->rw( i );
//       const CommonPartGeometry & cpgeom = m_cp_geoms[ i ];
//       const CommonPartGeometry & cpgeom_n = m_cp_geoms[ mccp.next( i ) ];
//       vcur.value = cpgeom.angle1;
//       vcur.min = cpgeom.min;
//       vcur.max = cpgeom.max;
//       vcur.dist_to_next = (cpgeom.length + cpgeom_n.length) / 2.0f;
//       cerr << "Value[" << i << "]"
// 	   << " value=" << vcur.value
// 	   << " min=" << vcur.min
// 	   << " max=" << vcur.max
// 	   << " dist_to_next=" << vcur.dist_to_next
// 	   << endl;
       // LinearMinimizer::ValueInfo & vprev = m_minimizer->rw( i - 1 );
       LinearMinimizer::ValueInfo & vcur = m_minimizer->rw( i );
       const CommonPartGeometry & cpgeom = m_cp_geoms[ i ];
       const CommonPartGeometry & cpgeom_p = m_cp_geoms[ mccp.previous( i ) ];
       vcur.value = cpgeom.angle1;
       vcur.min = ac.max( cpgeom_p.min, cpgeom.min );
       vcur.max = ac.min( cpgeom.max, cpgeom_p.max );
//        vcur.min = ac.less( cpgeom_p.min, cpgeom.min ) 
//  	? cpgeom.min : cpgeom_p.min;
//        vcur.max = ac.less( cpgeom.max, cpgeom_p.max )
//  	? cpgeom.max : cpgeom_p.max;

       // Discrete length.
       vcur.dist_to_next = cpgeom.length;
       // Euclidean length estimation.
       // vcur.dist_to_next = cpgeom.length;
//        cerr << "Value[" << i << "]"
//  	   << " value=" << vcur.value
//  	   << " min=" << vcur.min
//  	   << " max=" << vcur.max
//  	   << " dist_to_next=" << vcur.dist_to_next
//  	   << endl;
    }
}



/**
 * Called after an optimization to retrieve the result.
 */
void
ImaGene::GlobalC4CGeometry::afterLinearMinimizer()
{
  Mathutils::ModuloComputer mccp( m_nb_cp );

  for ( uint i = 0; i < nbCommonParts(); ++i )
    {
      // LinearMinimizer::ValueInfo & vprev = m_minimizer->rw( i - 1 );
      const LinearMinimizer::ValueInfo & vcur = m_minimizer->ro( i );
      m_cp_geoms[ i ].angle1 = vcur.value;
      m_cp_geoms[ mccp.previous( i ) ].angle2 = vcur.value;
    }
}

//   // Second loop to compute absolute information.
//   LinearMinimizer::ValueInfo & vb = m_minimizer->rw( 0 );
//   // LinearMinimizer::ValueInfo & vbprev = m_minimizer->rw( b - 1 );
//   // vbprev.delta = Mathutils::angleAroundZero( vb.ref_value - vbprev.ref_value );
//   vb.curv_abscissa = 0.0f;
//   double last_curv_abscissa = 0.0f;
//   double last_dist_to_next = vb.dist_to_next;
//   for ( uint i = 1; i < b; ++i )
//     {
//       // LinearMinimizer::ValueInfo & vprev = m_minimizer->rw( i - 1 );
//       LinearMinimizer::ValueInfo & vcur = m_minimizer->rw( i );
//       // vprev.delta = 
//       // Mathutils::angleAroundZero( vcur.ref_value - vprev.ref_value );
//       vcur.curv_abscissa = last_curv_abscissa + last_dist_to_next;
//       last_curv_abscissa = vcur.curv_abscissa;
//       last_dist_to_next = vcur.dist_to_next;
//     }

//   // Third loop to display info.
//   for ( uint i = 0; i < b; ++i )
//     {
//       LinearMinimizer::ValueInfo & vb = m_minimizer->rw( i );
//       cout << "Value[" << i << "]"
// 	   << " value=" << vb.value
// 	   << " ref_value=" << vb.ref_value
// 	   << " min=" << vb.min
// 	   << " max=" << vb.max
// 	   << " delta=" << vb.delta
// 	   << " dist_to_next=" << vb.dist_to_next
// 	   << " curv_abscissa=" << vb.curv_abscissa
// 	   << " disc_abscissa=" << vb.disc_abscissa << endl;
//     }

//   // The minimizer has [b] values.
//   m_minimizer->setSize( b );
  
//   delete lambda;
  
// }




///////////////////////////////////////////////////////////////////////////////
// ------------------------- Geometry services ------------------------------

  
/**
 * Computes the geometry of the whole contour. Stops when both
 * conditions are true (if they are valid).
 *
 * @param max_eps defines when to finish the optimization process
 * (ignored if negative).
 *
 * @param sum_eps defines when to finish the optimization process
 * (ignored if negative).
 */
void
ImaGene::GlobalC4CGeometry::computeContourGeometry
( double max_eps, double sum_eps )
{
  double var;

  prepareLinearMinimizer();
  
  uint n = 0;
  do
    {
      var = m_minimizer->optimize();
//       cerr << "[computeContourGeometry] Optimizing " 
// 	   << "S=" << m_minimizer->sum()
// 	   << " M=" << m_minimizer->max() << endl;
      ++n;
    }
  while ( ( ( max_eps > 0.0 ) && (  m_minimizer->lastDelta() > max_eps ) )
	  || ( ( sum_eps > 0.0 ) && (  m_minimizer->sum() > sum_eps ) ) );
  
  cout << "# Optimization statistics : " << *m_minimizer << endl
       << "# nb max sum" << endl
       << "# OptStats=" << n << " " << m_minimizer->max() 
       << " " << m_minimizer->sum() << endl;
  afterLinearMinimizer();
}




/**
 * @param cabs a valid curvilinear abscissa on the contour. 
 * @return the index of the common part containing the point of curvilinear abscissa [cabs].
   */
uint
ImaGene::GlobalC4CGeometry::indexCPFromCurvilinearAbscissa( double cabs ) const
{
  uint i1 = 0;
  uint i2 = nbCommonParts();
  CommonPartGeometry vi;
  vi.curv_abscissa = cabs;
  return upper_bound( m_cp_geoms + i1, m_cp_geoms + i2, vi, 
		      CPGCurvAbsCmp() )
    - m_cp_geoms;
}


/**
 * @param dabs a valid discrete abscissa on the contour. 
 * @return the index of the common part containing the point of discrete abscissa [dabs].
 */
uint
ImaGene::GlobalC4CGeometry::indexCPFromDiscreteAbscissa( double dabs ) const
{
  uint i1 = 0;
  uint i2 = nbCommonParts();
  uint mid1 = i2 / 3;
  uint mid2 = ( 2 * i2 ) / 3;
  
  CommonPartGeometry vi;
  vi.disc_abscissa = (uint) floor( dabs );

  uint cp;
  CPGDiscAbsCmp cmp( m_nb_surfels );
  if ( ( ! cmp( vi, getCommonPartGeometry( 0 ) ) )
       && cmp( vi, getCommonPartGeometry( mid1 ) ) )
    cp = upper_bound( m_cp_geoms + i1, m_cp_geoms + mid1, vi, cmp )
      - m_cp_geoms;
  else if ( ( ! cmp( vi, getCommonPartGeometry( mid1 ) ) )
       && cmp( vi, getCommonPartGeometry( mid2 ) ) )
    cp = upper_bound( m_cp_geoms + mid1, m_cp_geoms + mid2, vi, cmp )
      - m_cp_geoms;
  // JOL 2008/07/04: problem with trichotomy. A good trichotomy of
  // the common part is not always a good trichotomy of the
  // surfels.
  else
    {
      i1 = mid2; 
      while ( ( i1 != i2 ) 
	      && getCommonPartGeometry( i1 ).disc_abscissa <= vi.disc_abscissa )
	++i1;
      cp = i1; // upper bound
    }
  //     {
  //       ASSERT_GlobalC4CGeometry( ( ! cmp( vi, getCommonPartGeometry( mid2 ) ) )
  //  				&& cmp( vi, getCommonPartGeometry( 0 ) )
  //  				);
  //       cp = upper_bound( m_cp_geoms + mid2, m_cp_geoms + i2, vi, cmp )
  // 	- m_cp_geoms;
  //     }
  return ( cp == 0 ) ? nbCommonParts() - 1 : cp - 1;

}




  
/**
 * Returns the contour geometry at a given point specified by its
 * curvilinear abscissa.
 *
 * @param cabs a valid curvilinear abscissa on the contour. 
 * @param lgeo (returns) the local geometry at this point.
 */
void
ImaGene::GlobalC4CGeometry::geometryFromCurvilinearAbscissa
( double cabs, 
  LocalGeometry & lgeo ) const
{
  uint cp = indexCPFromCurvilinearAbscissa( cabs );
  lgeo.angle_to_x 
    = getCommonPartGeometry( cp ).angleFromCurvilinearAbscissa( cabs );
  lgeo.curvature 
    = getCommonPartGeometry( cp ).curvatureFromCurvilinearAbscissa( cabs );
  lgeo.geo_distance = cabs;
}


/**
 * Returns the contour geometry at a given point specified by its
 * discrete abscissa (pointel have integer abscissa while surfel
 * have half-integer abscissa).
 *
 * @param dabs a valid discrete abscissa on the contour. 
 * @param lgeo (returns) the local geometry at this point.
 */
void
ImaGene::GlobalC4CGeometry::geometryFromDiscreteAbscissa
( double dabs, 
  LocalGeometry & lgeo ) const
{
  uint cp = indexCPFromDiscreteAbscissa( dabs );
  if ( dabs < getCommonPartGeometry( cp ).disc_abscissa )
    dabs += (double) m_nb_surfels;

//   cerr << "dabs=" << dabs << " idx=" << cp << endl;
//   if ( dabs < getCommonPartGeometry( cp ).disc_abscissa )
//     {
//       dabs += (double) m_nb_surfels;
//       cerr << "   new dabs=" << dabs << " disc_abscissa=" << getCommonPartGeometry( cp ).disc_abscissa << endl;
//     }    
  lgeo.angle_to_x 
    = getCommonPartGeometry( cp ).angleFromDiscreteAbscissa( dabs );
  lgeo.curvature 
    = getCommonPartGeometry( cp ).curvatureFromDiscreteAbscissa( dabs );
  lgeo.geo_distance
    = getCommonPartGeometry( cp ).curvilinearFromDiscreteAbscissa( dabs );
}


/**
 * Returns the contour geometry at a given point specified by its
 * discrete abscissa (pointel have integer abscissa while surfel
 * have half-integer abscissa).
 *
 * @param i1 index of the first surfel where the local geometry is computed.
 * @param i2 index of the first after the last surfel where the local geometry is computed.
 * @param lgeo_tbl (returns) an array of size at least i2 - i1 where the local geometry of the specified surfels are stored.
 *
 * @todo not done
 */
void
ImaGene::GlobalC4CGeometry::contourGeometry
( uint i1, uint i2,
  LocalGeometry* lgeo_tbl ) const
{
  ASSERT_GlobalC4CGeometry( false && "NOT IMPLEMENTED." );
}

 
///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::GlobalC4CGeometry::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[GlobalC4CGeometry]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::GlobalC4CGeometry::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
