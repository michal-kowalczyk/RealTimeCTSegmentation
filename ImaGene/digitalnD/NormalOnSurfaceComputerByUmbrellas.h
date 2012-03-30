/** @file NormalOnSurfaceComputerByUmbrellas.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : NormalOnSurfaceComputerByUmbrellas.h
//
// Creation : 2009/05/18
//
// Version : 2009/05/18
//
// Author : Laurent Provot
//
// Summary : Header file for files NormalOnSurfaceComputerByUmbrellas.ih and NormalOnSurfaceComputerByUmbrellas.cxx
//
// History :
//	2009/05/18 : ?Name? : ?What?
//
// Rcs Id : "@(#)class NormalOnSurfaceComputerByUmbrellas declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(NormalOnSurfaceComputerByUmbrellas_RECURSES)
#error Recursive header files inclusion detected in NormalOnSurfaceComputerByUmbrellas.h
#else // defined(NormalOnSurfaceComputerByUmbrellas_RECURSES)
#define NormalOnSurfaceComputerByUmbrellas_RECURSES

#if !defined NormalOnSurfaceComputerByUmbrellas_h
#define NormalOnSurfaceComputerByUmbrellas_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/digitalnD/DigitalSurfaceGeometry.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class NormalOnSurfaceComputerByUmbrellas
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'NormalOnSurfaceComputerByUmbrellas' <p>
 * Aim: It represents a concrete strategy for the normal vector strategy.
 * The method is based on the (weighted) summation of elementary normal vector in a surfel neighborhood.
 * @see DigitalSurfaceGeometry,TangentOnSurfaceComputer,NormalOnSurfaceComputer,AreaOnSurfaceComputer,MeanCurvatureOnSurfaceComputer,LengthOnSurfaceComputer
 */
class NormalOnSurfaceComputerByUmbrellas : public NormalOnSurfaceComputer
{
  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  virtual ~NormalOnSurfaceComputerByUmbrellas();
  /**
   * Constructor.
   * @param order the number of "rings" of umbrellas taken into account to estimate the normal vector
   * @param weighted whether or not the estimation is done using a weighted summation.
   */
  NormalOnSurfaceComputerByUmbrellas( uint order = 1,
				      bool weighted = false );

  /**
   * Changes the order of the normal vector estimator
   * @param order the number of "rings" of umbrellas taken into account to estimate the normal vector.
   */
  INLINE void setOrder( uint order );

  /**
   * @return the order to consider for computation.
   */
  INLINE uint order() const;

  /**
   * Changes the order of the normal vector estimator
   * @param order the number of "rings" of umbrellas taken into account to estimate the normal vector.
   */
  INLINE void setWeighted( bool weighted );

  /**
   * @return whether or not the computation is a weighted summation or not.
   */
  INLINE bool isWeighted() const;

  // ------------------------- Implemented Geometry services ------------------
public:

  /**
   * Computes normals on [surf_geom_ctxt] with respect to the current parameters.
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
   * @see m_order, m_weighted
   */
  virtual void computeNormal( DigitalSurfaceGeometry & surf_geom_ctxt,
                              Kn_sid b,
                              Vector & vn);

  // --------------------- Normal vector services -----------------------
public:
  /**
   * Computes normals on [surf_geom_ctxt] using a bel neighborhood depending on the current order.
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
   * @see m_order
   */
  void computeSimpleNormal( DigitalSurfaceGeometry & surf_geom_ctxt,
                            Kn_sid b,
                            Vector & vn);

  /**
   * Computes normals on [surf_geom_ctxt] using a summation of simple normals in the order-zero umbrella of [b].
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param b a signed surfel.
   * @param vn (returns) the estimated normal vector at surfel [b], oriented toward the direct orientation (meaning "inside" the shape).
   * @see comupteSimpleNormal, m_order
   */
  void computeWeightedNormal( DigitalSurfaceGeometry & surf_geom_ctxt,
                              Kn_sid b,
                              Vector & vn);

 private:
  /**
   * Extends a set of surfels [surfels_set] by adding the surfels not in [surfels_set] that belong to umbrellas of pointels of [surfels_set].
   * @param surf_geom_ctxt the current context for computing some geometry measurements on the digital surface.
   * @param surfels_set (returns) the connected set of surfels to extend.
   */
  void extendNeighborhoodWithUmbrellas( DigitalSurfaceGeometry & surf_geom_ctxt,
                                        KnRCellSet & surfels_set );

  // ------------------------- Interface --------------------------------------
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
  
  // ------------------------- Datas ------------------------------------------
private:

  /**
   * The order (neighborhood taken into account for the estimation) of the normal vector estimator
   */
  uint m_order;
  
  /**
   * Whether or not the normal vector computation is done using weights
   */
  bool m_weighted;

  // ------------------------- Hidden services --------------------------------
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE NormalOnSurfaceComputerByUmbrellas( const NormalOnSurfaceComputerByUmbrellas & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE NormalOnSurfaceComputerByUmbrellas & operator=( const NormalOnSurfaceComputerByUmbrellas & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'NormalOnSurfaceComputerByUmbrellas'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'NormalOnSurfaceComputerByUmbrellas' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const NormalOnSurfaceComputerByUmbrellas & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/NormalOnSurfaceComputerByUmbrellas.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined NormalOnSurfaceComputerByUmbrellas_h

#undef NormalOnSurfaceComputerByUmbrellas_RECURSES
#endif // else defined(NormalOnSurfaceComputerByUmbrellas_RECURSES)
