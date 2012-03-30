/** @file MultiscaleFreemanChain.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : MultiscaleFreemanChain.h
//
// Creation : 2009/07/09
//
// Version : 2009/07/09
//
// Author : JOL
//
// Summary : Header file for files MultiscaleFreemanChain.ih and MultiscaleFreemanChain.cxx
//
// History :
//	2009/07/09 : ?Name? : ?What?
//
// Rcs Id : "@(#)class MultiscaleFreemanChain declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(MultiscaleFreemanChain_RECURSES)
#error Recursive header files inclusion detected in MultiscaleFreemanChain.h
#else // defined(MultiscaleFreemanChain_RECURSES)
#define MultiscaleFreemanChain_RECURSES

#if !defined MultiscaleFreemanChain_h
#define MultiscaleFreemanChain_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/dgeometry2d/FreemanChainTransform.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class MultiscaleFreemanChain
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'MultiscaleFreemanChain' <p> Aim: Encodes a
   * freeman chain at different resolution and extracts its meaningful
   * scales.
   */
  class MultiscaleFreemanChain
  {
    // ----------------------- public types ------------------------------
  public:
    struct SubsampledChain;
    struct SubsampledChainKey;
    typedef std::map< SubsampledChainKey, SubsampledChain >::iterator iterator;
    typedef std::map< SubsampledChainKey, SubsampledChain >::const_iterator const_iterator;

    // ----------------------- inner classes ------------------------------
  public:

    /**
     * Represents the subsampled freeman chain at the given scales and
     * the mappings used for point correspondence.
     */
    struct SubsampledChain
    {
      FreemanChain subc;
      std::vector<uint> c2subc;
      std::vector<uint> subc2c;

      /**
       * @param idx index of a pointel (in the original chain)
       * @return the index of the corresponding pointel in [subc].
       */
      INLINE uint subpointel( uint idx ) const;

      /**
       * @param idx index of a surfel (in the original chain)
       *
       * @return the index of the corresponding surfel in [subc] and
       * the relative position within this surfel.
       */
      INLINE std::pair<uint,double> subsurfel( uint idx ) const;

      /**
       * @param idx index of a pointel (in the chain [subc])
       * @return the index of the corresponding pointel in the original chain.
       */
      INLINE uint backpointel( uint idx ) const;

      /**
       * @param idx index of a surfel (in the chain [subc])
       *
       * @return the indexes of the corresponding surfels in the
       * original chain.
       */
      INLINE std::pair<uint,uint> backsurfels( uint idx ) const;
    };

    /**
     * Represents the key for accessing a subsampled freeman chain at
     * the given scales and the mappings used for point
     * correspondence.
     */
    struct SubsampledChainKey
    {
      uint h;
      uint v;
      int x0;
      int y0;

      SubsampledChainKey( uint h1, uint v1, int xIni, int yIni )
	: h( h1 ), v( v1 ), x0( xIni ), y0( yIni )
      {
      }

      bool operator<( const SubsampledChainKey & other ) const 
      {
	return ( h < other.h ) 
	  || ( ( h == other.h ) 
	       && ( ( v < other.v ) 
		    || ( ( v == other.v ) 
			 && ( ( x0 < other.x0 ) 
			      || ( ( x0 == other.x0 ) 
				   && ( y0 < other.y0 ) )
			      )
			 )
		    )
	       );
      }

      bool operator==( const SubsampledChainKey & other ) const 
      {
	return ( h == other.h )
	  || ( v == other.v ) 
	  || ( x0 == other.x0 ) 
	  || ( y0 == other.y0 );
      }
    };


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~MultiscaleFreemanChain();

    /**
     * Constructor.
     */
    INLINE MultiscaleFreemanChain();

    /**
     * Defines the chain of 'FreemanChainTransform' used to define the
     * subsampling.
     *
     * @param transform the global subsampling transformation.
     *
     * @param subsample the subsampling transform that must be part
     * of the chain [transform].
     */
    void chooseSubsampler( const FreemanChainTransform & transform,
			   FreemanChainSubsample & subsample );
      
    /**
     * Computes the multiresolutions of the given Freeman chain [src]
     * from (1,1) up to (h,v)=(r,r) with shifts.
     *
     * @param src the source Freeman chain.
     * @param r the maximal resolution.
     */
    void init( const FreemanChain & src, uint r );


    /**
     * @return an iterator on the first element of the map [m_map].
     */
    INLINE iterator begin();

    /**
     * @return an iterator on the element after the last of the map [m_map].
     */
    INLINE iterator end();

    /**
     * @return a const iterator on the first element of the map [m_map].
     */
    INLINE const_iterator begin() const;

    /**
     * @return a const iterator on the element after the last of the
     * map [m_map].
     */
    INLINE const_iterator end() const;

    /**
     * @param key the key describing resolution and shifts.
     *
     * @return a pointer to the corresponding structure storing the
     * subsampled chain, or 0 if no one was found.
     */
    const SubsampledChain* get( const SubsampledChainKey & key ) const;

    /**
     * @param key the key describing resolution and shifts.
     *
     * @return a pointer to the corresponding structure storing the
     * subsampled chain, or 0 if no one was found.
     */
    SubsampledChain* get( const SubsampledChainKey & key );

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
  protected:

    /**
     * The global subsampling transform.
     */
    const FreemanChainTransform* m_transform;

    /**
     * The subsampling transform (with the multiresolution
     * parameters). Part of the chain of FreemanChainTransform
     * [m_transform].
     * @see m_transform
     */
    FreemanChainSubsample* m_subsample;

    /**
     * The source FreemanChain.
     */
    const FreemanChain* m_src;

    /**
     * Stores for a given (h,v,x0,y0) its subsampling.
     */
    std::map< SubsampledChainKey, SubsampledChain > m_map;

    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE MultiscaleFreemanChain( const MultiscaleFreemanChain & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE MultiscaleFreemanChain & operator=( const MultiscaleFreemanChain & other );
  
    // ------------------------- Internals ------------------------------------
  private:

    // ------------------------- static ------------------------------------
  public:

    /**
     * @param fc (maybe updated) the freeman chain code of the (closed)
     * contour. The chain code may be translated.
     * 
     * @return a dyn. alloc. statistics object storing the length of
     * maximal segments for each surfel. Contains as many statistics
     * variables as the number of surfels of [fc].
     */
    static Statistics* getStatsMaximalSegments( FreemanChain & fc );
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'MultiscaleFreemanChain'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'MultiscaleFreemanChain' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const MultiscaleFreemanChain & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/MultiscaleFreemanChain.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined MultiscaleFreemanChain_h

#undef MultiscaleFreemanChain_RECURSES
#endif // else defined(MultiscaleFreemanChain_RECURSES)
