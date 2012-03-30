/** @file GridContour.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : GridContour.h
//
// Creation : 2011/02/28
//
// Version : 2011/02/28
//
// Author : JOL
//
// Summary : Header file for files GridContour.ih and GridContour.cxx
//
// History :
//	2011/02/28 : ?Name? : ?What?
//
// Rcs Id : "@(#)class GridContour declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(GridContour_RECURSES)
#error Recursive header files inclusion detected in GridContour.h
#else // defined(GridContour_RECURSES)
#define GridContour_RECURSES

#if !defined GridContour_h
#define GridContour_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/UndoableList.h"
#include "ImaGene/dgeometry2d/OctantChange.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/mathutils/CFraction.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class GridContour
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'GridContour' <p>
   * Aim: This class represents
   * a dynamic digital contour (i.e., modifiable with elementary
   * flips) which updates at each step its minimum length polygon (MLP
   * for short). It is particularly adapted to represent a deformable
   * contour model.
   */
  class GridContour
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~GridContour();

    /**
     * Constructor. Empty grid contour.
     */
    GridContour();


    /**
     * This method initializes consistently a dynamic contour from a
     * Freeman chain. The chain is supposed to model either the inner
     * 4-connected 8-border of an object ([inside] is 'true') or its
     * outer 4-connected 8-border ([inside] is 'false'). If you wish to
     * initialize the dynamic contour with a digital interpixel contour,
     * you should use the method FreemanChain::innerContour beforehands.
     *
     * The iterators should have a getCode() method returning between 0-3
     * according to the Freeman chain code and a nextInLoop() method to
     * visit the whole Freeman chain code.
     *
     * @param itb an iterator on the beginning of Freeman chain.
     * @param ite an iterator on the end of Freeman chain.
     * @param inside indicates if it is an inner or an outer border.
     */
    template <class FreemanChainIterator>
    void initFromFreemanChain( FreemanChainIterator itb, 
			       FreemanChainIterator ite, 
			       bool inside );


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
    INLINE GridContour( const GridContour & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE GridContour & operator=( const GridContour & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'GridContour'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'GridContour' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const GridContour & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/GridContour.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined GridContour_h

#undef GridContour_RECURSES
#endif // else defined(GridContour_RECURSES)
