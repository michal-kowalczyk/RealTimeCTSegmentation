/** @file FreemanChainTransform.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : FreemanChainTransform.h
//
// Creation : 2009/07/08
//
// Version : 2009/07/08
//
// Author : JOL
//
// Summary : Header file for files FreemanChainTransform.ih and FreemanChainTransform.cxx
//
// History :
//	2009/07/08 : ?Name? : ?What?
//
// Rcs Id : "@(#)class FreemanChainTransform declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(FreemanChainTransform_RECURSES)
#error Recursive header files inclusion detected in FreemanChainTransform.h
#else // defined(FreemanChainTransform_RECURSES)
#define FreemanChainTransform_RECURSES

#if !defined FreemanChainTransform_h
#define FreemanChainTransform_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class FreemanChainTransform
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'FreemanChainTransform' <p> Aim: An abstract
   * class to module a transformation between two freeman chains which
   * keeps the correspondence between elements of the two chains.
   */
  class FreemanChainTransform
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE FreemanChainTransform();

    /**
     * Destructor. 
     */
    virtual ~FreemanChainTransform();

    /**
     * Applies the transformation to chain [src], constructing chain
     * [dst]. It is worthy to note that the index transformation
     * relates \b pointels of [src] with \b pointels of [dst] and the
     * reverse, pointels which are at the \b beginning of the vector
     * that is the Freeman code at same index (ie, beginning of the
     * linel).
     *
     * @param dst (returns) the output Freeman chain.
     *
     * @param src2dst (returns) an array of size 'src.chain.size()'
     * giving for each index in [src] its corresponding index in
     * [dst].
     *
     * @param dst2src (returns) an array of size 'dst.chain.size()'
     * giving for each index in [dst] its corresponding index in
     * [src].
     *
     * @param src the input Freeman chain.
     *
     * @return 'true' if the transformation worked, 'false' if there
     * was a problem. Outputs may not be valid.
     */
    virtual bool apply( FreemanChain & dst,
			std::vector<uint> & src2dst,
			std::vector<uint> & dst2src,
			const FreemanChain & src ) const = 0;

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


    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE FreemanChainTransform( const FreemanChainTransform & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE FreemanChainTransform & operator=( const FreemanChainTransform & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'FreemanChainTransform'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'FreemanChainTransform' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const FreemanChainTransform & that_object_to_display );

  

  struct FreemanChainSubsample : public FreemanChainTransform
  {
  public:
    int m_h;
    int m_v;
    int m_x0;
    int m_y0;
  public:
    virtual ~FreemanChainSubsample() {}
    FreemanChainSubsample( int h, int v, int x0, int y0 )
      : m_h( h ), m_v( v ), m_x0( x0 ), m_y0( y0 )
    {}
    virtual bool apply( FreemanChain & dst,
			std::vector<uint> & src2dst,
			std::vector<uint> & dst2src,
			const FreemanChain & src ) const
    {
      //    std::cerr << "[FreemanChainSubsample::apply]" << std::endl;
      bool ok = FreemanChain::subsample( dst, src2dst, dst2src, src,
					 m_h, m_v, m_x0, m_y0 ); 
//       std::cerr << "[FreemanChainSubsample::apply] ok=" << ok 
// 		<< " dst.chain.size()=" << dst.chain.size() << std::endl;
      return ok && ( dst.chain.size() != 0 );
    }
  };

  struct FreemanChainInnerCCW : public FreemanChainTransform
  {
  private:
    bool m_ccw;
  public:
    virtual ~FreemanChainInnerCCW() {}
    FreemanChainInnerCCW( bool ccw = true )
      : m_ccw( ccw )
    {}
    virtual bool apply( FreemanChain & dst,
			std::vector<uint> & src2dst,
			std::vector<uint> & dst2src,
			const FreemanChain & src ) const
    {
      // std::cerr << "[FreemanChainInnerCCW::apply] " << src.chain << std::endl;
      FreemanChain::innerContour( dst, src2dst, dst2src, src, m_ccw );
      return dst.chain.size() != 0;
    }
  };

  struct FreemanChainOuterCCW : public FreemanChainInnerCCW
  {
  private:
  public:
    virtual ~FreemanChainOuterCCW() {}
    FreemanChainOuterCCW( bool ccw = true )
      : FreemanChainInnerCCW( ! ccw )
    {}

  };

  struct FreemanChainCompose : public FreemanChainTransform
  {
  private:
    const FreemanChainTransform* m_f;
    const FreemanChainTransform* m_g;
  public:
    virtual ~FreemanChainCompose() {}
    FreemanChainCompose( const FreemanChainTransform & f, 
			 const FreemanChainTransform & g )
      : m_f( &f ), m_g( &g )
    {}
    virtual bool apply( FreemanChain & dst,
			std::vector<uint> & src2dst,
			std::vector<uint> & dst2src,
			const FreemanChain & src ) const
    {
      // std::cerr << "[FreemanChainCompose::apply] g " << std::endl;
      std::vector<uint> g_src2inter;
      std::vector<uint> g_inter2src;
      FreemanChain inter;
      std::vector<uint> f_inter2dst;
      std::vector<uint> f_dst2inter;
      bool success_f = false;
      bool success_g = m_g->apply( inter, g_src2inter, g_inter2src, src );
      if ( success_g )
	{
	  // std::cerr << "[FreemanChainCompose::apply] f(g) " << std::endl;
	  success_f = m_f->apply( dst, f_inter2dst, f_dst2inter, inter );
	  if ( success_f )
	    {
	      // std::cerr << "[FreemanChainCompose::apply] linking f o g " << std::endl;
	      src2dst.resize( src.chain.size() );
	      dst2src.resize( dst.chain.size() );
	      ASSERT_FreemanChain
		( ( src.chain.size() == g_src2inter.size() )
		  && ( dst.chain.size() == f_dst2inter.size() )
		  && ( inter.chain.size() == g_inter2src.size() )
		  && ( inter.chain.size() == f_inter2dst.size() ) );
	      for ( uint i = 0; i < src.chain.size(); i++ ) 
		{
		  if ( g_src2inter[ i ] >= f_inter2dst.size() )
		    std::cerr << "[FreemanChainCompose::apply] "
			      << "g_src2inter[ i ] >= f_inter2dst.size()"
			      << " i=" << i
			      << " " << g_src2inter[ i ] << ">="
			      << f_inter2dst.size() << std::endl
			      << "src=" << src.chain 
			      << " " << src.chain.size() << std::endl
			      << "int=" << inter.chain 
			      << " " << inter.chain.size() << std::endl
			      << "dst=" << dst.chain
			      << " " << dst.chain.size() << std::endl;
		  src2dst[ i ] = f_inter2dst.at( g_src2inter[ i ] );
		}
	      for ( uint j = 0; j < dst.chain.size(); j++ ) 
		{
		  if ( f_dst2inter[ j ] >= g_inter2src.size() )
		    std::cerr << "[FreemanChainCompose::apply] "
			      << "f_dst2inter[ j ] >= g_inter2src.size()"
			      << " j=" << j
			      << " " << f_dst2inter[ j ] << ">="
			      << g_inter2src.size() << std::endl
			      << "src=" << src.chain 
			      << " " << src.chain.size() << std::endl
			      << "int=" << inter.chain 
			      << " " << inter.chain.size() << std::endl
			      << "dst=" << dst.chain
			      << " " << dst.chain.size() << std::endl;
		  dst2src[ j ] = g_inter2src.at( f_dst2inter[ j ] ); 
		}
	    }
	}
//       std::cerr << "[FreemanChainCompose::apply] "
// 		<< " success_f=" << success_f
// 		<< " success_g=" << success_g
// 		<< std::endl;
      return success_f && success_g;
    }

  };

  struct FreemanChainCleanOuterSpikesCCW : public FreemanChainTransform
  {
  private:
    bool m_ccw;
  public:
    virtual ~FreemanChainCleanOuterSpikesCCW() {}
    FreemanChainCleanOuterSpikesCCW( bool ccw = true )
      : m_ccw( ccw )
    {}
    virtual bool apply( FreemanChain & dst,
			std::vector<uint> & src2dst,
			std::vector<uint> & dst2src,
			const FreemanChain & src ) const
    {
      //      std::cerr << "[FreemanChainCleanOuterSpikesCCW::apply]" << std::endl;
      bool ok = FreemanChain::cleanOuterSpikes( dst, src2dst, dst2src, 
						src, m_ccw  );
//       std::cerr << "[FreemanChainCleanOuterSpikesCCW::apply] ok=" << ok 
// 		<< " dst.chain.size()=" << dst.chain.size() << std::endl;
      return ok && ( dst.chain.size() != 0 );
    }
  };

  struct FreemanChainCleanInnerSpikesCCW : public FreemanChainCleanOuterSpikesCCW
  {
  private:
    bool m_ccw;
  public:
    virtual ~FreemanChainCleanInnerSpikesCCW() {}
    FreemanChainCleanInnerSpikesCCW()
      : FreemanChainCleanOuterSpikesCCW( false )
    {}
  };

  struct FreemanChainCleanSpikesCCW : public FreemanChainTransform
  {
  private:
    bool m_ccw;
    uint m_nb;
    FreemanChainCleanInnerSpikesCCW m_clean_in;
    FreemanChainCleanOuterSpikesCCW m_clean_out;
    FreemanChainTransform** m_delegate;
  public:
    virtual ~FreemanChainCleanSpikesCCW() 
    {
      for ( uint i = 1; i < m_nb; ++i )
	delete m_delegate[ i ];
      delete[ ] m_delegate;
    }
    FreemanChainCleanSpikesCCW( uint nb )
      : m_nb( nb )
    {
      m_delegate = new FreemanChainTransform*[ nb ];
      m_delegate[ 0 ] = new FreemanChainCompose( m_clean_in, m_clean_out );
      for ( uint i = 1; i < nb; ++i )
	{
	  m_delegate[ i ] = new FreemanChainCompose( *m_delegate[ 0 ], *m_delegate[ i - 1 ] ); 
	}
    }
    virtual bool apply( FreemanChain & dst,
			std::vector<uint> & src2dst,
			std::vector<uint> & dst2src,
			const FreemanChain & src ) const
    {
      // std::cerr << "[FreemanChainCleanSpikesCCW::apply]" << std::endl;
      return m_delegate[ m_nb - 1 ]->apply( dst, src2dst, dst2src, src );
    }
  };



} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/FreemanChainTransform.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined FreemanChainTransform_h

#undef FreemanChainTransform_RECURSES
#endif // else defined(FreemanChainTransform_RECURSES)
