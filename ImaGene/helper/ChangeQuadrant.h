/** @file ChangeQuadrant.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ChangeQuadrant.h
//
// Creation : 2009/06/30
//
// Version : 2009/06/30
//
// Author : JOL
//
// Summary : Header file for files ChangeQuadrant.ih and ChangeQuadrant.cxx
//
// History :
//	2009/06/30 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ChangeQuadrant declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ChangeQuadrant_RECURSES)
#error Recursive header files inclusion detected in ChangeQuadrant.h
#else // defined(ChangeQuadrant_RECURSES)
#define ChangeQuadrant_RECURSES

#if !defined ChangeQuadrant_h
#define ChangeQuadrant_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/Vector.h"
#include "ImaGene/mathutils/Line2D.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/digitalnD/Frame2D.h"
#include "ImaGene/helper/C4CTangentialCoverGeometry.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class SegmentMLP
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ChangeQuadrant' <p>
   * Aim: Encodes the ChangeQuadrant of a digital contour.
   */
  class ChangeQuadrant
  {
  public:


    /** 
     * Description of struct 'ChangeQuadrant::segmentMLP' <p>
     * Aim: Encodes each edge of the segmentMLP.
     */
    struct segmentMLP
    {
      public:
        FreemanChain fc;
        Vector2i first_point; // The first point of DSS
        Vector2i last_point;  // The last point of DSS 
        int a; 
        int b;
        int mu; // The lower bound of DSS
        int mu_sup; // The upper bound of DSS
        int first_code; // The first code of the FreemanChain of DSS
        int Quadrant; // Quadrant for each DSS of the Freeman chain code.
        bool convex_back; 
        bool convex_front;
        int nbpointtested; // How is the number of tested points for known the characteristics (a,b,mu) of DSS
    };


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~ChangeQuadrant();
     /**
     * @return the Segments MLP of the polygon.
     */ 
    INLINE const std::vector<segmentMLP> & getSegmentMLP() const;
    
         

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param that_stream the output stream where the object is written.
     */
    void selfDisplay( std::ostream & that_stream ) const;

    /**
     * Transform all the characteristics of DSS from the Global Quadrant
     * to Original Quadrant.
     */   
     void TransformGlobalQuad2OriginalQuad
     ( ImaGene::ChangeQuadrant::segmentMLP & GlobalSegmentMLP, 
       ImaGene::ChangeQuadrant::segmentMLP & OriginalSegmentMLP, 
       int h, int v );
  
    /**
     * Transform all the characteristics of DSS from the Original Quadrant
     * to First Quadrant.
     */
     void TransformOriginalQuad2FirstQuad
     ( ImaGene::ChangeQuadrant::segmentMLP & FirstSegmentMLPhv, 
       ImaGene::ChangeQuadrant::segmentMLP & OriginalSegmentMLPhv );

     /**
      * Transform the first point, last point and the FreemanChain of DSS 
      * from the Original Quadrant to Original Quadrant (h,v).
      */
     void TransformFCOriginalQuad2OriginalQuadhv
     ( ImaGene::ChangeQuadrant::segmentMLP & OriginalSegmentMLP, 
       ImaGene::ChangeQuadrant::segmentMLP & OriginalSegmentMLPhv, 
       int h, int v);

     /**
      * Transform all the characteristics of DSS from the First Quadrant
      * to Original Quadrant.
      */
     void TransformFirstQuad2OriginalQuad
     ( ImaGene::ChangeQuadrant::segmentMLP & FirstDSSMLPhv, 
       ImaGene::ChangeQuadrant::segmentMLP & OriginalDSSMLPhv, 
       ImaGene::ChangeQuadrant::segmentMLP & SecondSegmentMLPhv, 
       ImaGene::ChangeQuadrant::segmentMLP & GlobalSegmentMLP );   
   
  // ------------------------- Datas ----------------------------------------
  private:

    /**
     * stores the set of segmentMLP of the characteristic polygon.
     */
    std::vector<segmentMLP>  m_segmentMLP;    
    
    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    //INLINE ChangeQuadrant( const ChangeQuadrant & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    //INLINE ChangeQuadrant & operator=( const ChangeQuadrant & other );
  
    // ------------------------- Internals ------------------------------------
  private:
   
};

  /**
   * Overloads 'operator<<' for displaying objects of class 'ChangeQuadrant'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ChangeQuadrant' to write.
   * @return the output stream after the writing.
   */
 // INLINE std::ostream&
 // operator<<( std::ostream & that_stream, 
 //	        const ChangeQuadrant & that_object_to_display );


  /**
   * Overloads 'operator<<' for displaying objects of class
   * 'ChangeQuadrant::segmentMLP'.
   *
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ChangeQuadrant' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
 	        const ChangeQuadrant::segmentMLP & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/ChangeQuadrant.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ChangeQuadrant_h

#undef ChangeQuadrant_RECURSES
#endif // else defined(ChangeQuadrant_RECURSES)
