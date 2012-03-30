/** @file ImplicitSurfaceEmbedder.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ImplicitSurfaceEmbedder.h
//
// Creation : 2005/01/18
//
// Version : 2005/01/18
//
// Author : JOL
//
// Summary : Header file for files ImplicitSurfaceEmbedder.ih and ImplicitSurfaceEmbedder.cxx
//
// History :
//	2005/01/18 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ImplicitSurfaceEmbedder declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ImplicitSurfaceEmbedder_RECURSES)
#error Recursive header files inclusion detected in ImplicitSurfaceEmbedder.h
#else // defined(ImplicitSurfaceEmbedder_RECURSES)
#define ImplicitSurfaceEmbedder_RECURSES

#if !defined ImplicitSurfaceEmbedder_h
#define ImplicitSurfaceEmbedder_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/Embedder.h"
#include "ImaGene/3d/ImplicitSurface.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class ImplicitSurfaceEmbedder
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ImplicitSurfaceEmbedder' <p> Aim:
   * Implements an 'Embedder' tailored to the geometry of an implicit
   * surface. More precisely, surfels have an embedding that attempt
   * to lie on the isovalue 0 of the implicit surface.
   */
  class ImplicitSurfaceEmbedder : public Embedder
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~ImplicitSurfaceEmbedder();


    /**
     * Constructor. The embedder [spel_embedder] is used to embed
     * spels. It is referenced in the object. The object is not valid
     * yet and need an implicit surface.
     * @see init
     *
     * @param spel_embedder any embedder.
     * @param epsilon this positive value specifies the min distance between an embedding of a spel and the embedding of a surfel as a multiple of the distance between two adjacent voxels.
     */
    ImplicitSurfaceEmbedder( const Embedder & spel_embedder,
			     float epsilon = 0.001 );

    /**
     * Initializes the embedder for the implicit surface [is].
     *
     * @param is any implicit surface (with same dimension as the
     * embedder). It is referenced and not cloned.
     */
    void init( const ImplicitSurface* is );


    // ----------------------- Implemented abstract services ------------------
  public:
    
    /**
     * @return a clone of 'this'.
     */
    virtual Embedder* clone() const;
    


    // ----------------------- Embedding services -----------------------------
  public:

    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     * @param p any unsigned cell.
     * @param coords (returns) the coordinates of [p] when embedded in the space.
     */
    virtual void uembed( Kn_uid p, float* coords ) const;
    
    /**
     * Embeds the cell [p] in the Euclidean space as a point.
     * @param p any signed cell.
     * @param coords (returns) the coordinates of [p] when embedded in the space.
     */
    virtual void sembed( Kn_sid p, float* coords ) const;


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
    virtual void embedVector( const float* p, float* coords ) const;


    // ----------------------- inverse embedding services ---------------------
  public:

    /**
     * @param coords any coordinates in the space.
     * @return the unsigned spel whose embedding is the closest to [coords].
     */
    virtual Kn_uid uinverseSpel( const float* coords ) const;



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
  private:

    /**
     * Spel embedder.
     */
    const Embedder & m_spel_embedder;
    
    /**
     * A pointer on the digital surface (not owned).
     */
    const ImplicitSurface* m_is;
    
    /**
     * This value is used to prevent surfels embedding to lie exactly
     * on a spel embedding.
     */
    float m_epsilon;


    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE ImplicitSurfaceEmbedder();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ImplicitSurfaceEmbedder( const ImplicitSurfaceEmbedder & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ImplicitSurfaceEmbedder & operator=( const ImplicitSurfaceEmbedder & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'ImplicitSurfaceEmbedder'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ImplicitSurfaceEmbedder' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const ImplicitSurfaceEmbedder & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/3d/ImplicitSurfaceEmbedder.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ImplicitSurfaceEmbedder_h

#undef ImplicitSurfaceEmbedder_RECURSES
#endif // else defined(ImplicitSurfaceEmbedder_RECURSES)
