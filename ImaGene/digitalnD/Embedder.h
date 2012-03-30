/** @file Embedder.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Embedder.h
//
// Creation : 2004/02/04
//
// Version : 2004/02/04
//
// Author : JOL
//
// Summary : Header file for files Embedder.ih and Embedder.cxx
//
// History :
//	2004/02/04 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Embedder declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Embedder_RECURSES)
#error Recursive header files inclusion detected in Embedder.h
#else // defined(Embedder_RECURSES)
#define Embedder_RECURSES

#if !defined Embedder_h
#define Embedder_h

//////////////////////////////////////////////////////////////////////////////
//#include "LinAlg/LinAlg/Vector.hpp"
#include "ImaGene/base/Vector.h"
#include "ImaGene/digitalnD/KnTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

  class KnSpace;
  
  /////////////////////////////////////////////////////////////////////////////
  // class Embedder
  /////////////////////////////////////////////////////////////////////////////

  /** 
   * Description of class 'Embedder' <p>
   * Aim: An interface to embed digital cells in the Euclidean space.
   */
  class Embedder
  {
    // ------------------------- Standard services ----------------------------
  public:

    /**
     * Destructor. Does nothing
     */
    INLINE virtual ~Embedder();
    
    /**
     * Constructor.
     * The embedder is not valid yet.
     * @see init
     */
    INLINE Embedder();
    
    /**
     * Constructor.
     * @param space the digital space where the cells are defined.
     */
    INLINE Embedder( const KnSpace* space );
    
    /**
     * Sets the digital space.
     * @param space the digital space where the cells are defined.
     */
    INLINE void init( const KnSpace* space );
    
    /**
     * @return the digital space associated to the embedder.
     */
    INLINE const KnSpace* space() const;


    /**
     * @return a clone of 'this'.
     */
    virtual Embedder* clone() const = 0;
    
    
    // ----------------------- Embedding services -----------------------------
  public:
    
    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     * @param p any unsigned cell.
     * @param coords (returns) the coordinates of [p] when embedded in the space.
     */
    virtual void uembed( Kn_uid p, float* coords ) const = 0;
    
    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     * @param p any signed cell.
     * @param coords (returns) the coordinates of [p] when embedded in the space.
     */
    virtual void sembed( Kn_sid p, float* coords ) const = 0;
    
    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     * @param p any unsigned cell.
     * @param coords (returns) the coordinates of [p] when embedded in the space.
     * @see uembed( Kn_uid p, float* coords )
     * NB: calls 'uembed', could be optimized.
     */
    INLINE virtual void uembed( Kn_uid p, Vector & coords ) const;

    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     * @param p any signed cell.
     * @param coords (returns) the coordinates of [p] when embedded in the space.
     * @see sembed( Kn_uid p, float* coords )
     * NB: calls 'sembed', could be optimized.
     */
    INLINE virtual void sembed( Kn_sid p, Vector & coords ) const;


    // ----------------------- Other embedding services -----------------------
  public:
    
    /**
     * Embeds the vector [p] in the Euclidean space as a point.  By
     * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
     * ), coords ).
     *

     * @param p any vector expressed in the discrete space.  
     * @param coords (returns) the coordinates of [p] when embedded in the space.
     */
    virtual void embedVector( const float* p, float* coords ) const = 0;

    /**
     * Embeds the vector [p] in the Euclidean space as a point.  By
     * definition, uembed( p, coords ) <=> embedVector( ucentroid( p
     * ), coords ).
     *

     * @param p any vector expressed in the discrete space.  
     * @param coords (returns) the coordinates of [p] when embedded in the space.
     */
    INLINE virtual void embedVector( const Vector & p, Vector & coords ) const;
    

    // ----------------------- inverse embedding services ---------------------
  public:

    /**
     * @param coords any coordinates in the space.
     * @return the unsigned spel whose embedding is the closest to [coords].
     */
    virtual Kn_uid uinverseSpel( const float* coords ) const = 0;

    /**
     * @param coords any coordinates in the space.
     * @return the unsigned spel whose embedding is the closest to [coords].
     * @see uinverseSpel( const float* )
     * NB: call uinverseSpel( const float* ) after having dynamically allocated a float array.
     */
    INLINE virtual Kn_uid uinverseSpel( const Vector & coords ) const;

    // ------------------------- datas --------------------------------------
  private:
    /**
     * Space in which cells are embedded.
     */
    const KnSpace* m_space;
    
  };

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "Embedder.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Embedder_h

#undef Embedder_RECURSES
#endif // else defined(Embedder_RECURSES)
