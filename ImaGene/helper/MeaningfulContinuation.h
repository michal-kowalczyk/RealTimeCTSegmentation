/** @file MeaningfulContinuation.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : MeaningfulContinuation.h
//
// Creation : 2009/02/16
//
// Version : 2009/02/16
//
// Author : BK
//
// Summary : Header file for files MeaningfulContinuation.ih and MeaningfulContinuation.cxx
//
// History :
//	2009/02/16 : ?Name? : ?What?
//
// Rcs Id : "@(#)class MeaningfulContinuation declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(MeaningfulContinuation_RECURSES)
#error Recursive header files inclusion detected in MeaningfulContinuation.h
#else // defined(MeaningfulContinuation_RECURSES)
#define MeaningfulContinuation_RECURSES

#if !defined MeaningfulContinuation_h
#define MeaningfulContinuation_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <deque>

#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/base/Vector.h"


//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class MeaningfulContinuation
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'MeaningfulContinuation'
   *  <p> Aim: This class computes and stores all information
   *  related to Meaningful Continuation . See reference
   * [Cao2003].

   * [Cao2003] F. Cao,Good Continuations in digital image level lines,
   * IEEE International Conference on Computer Vision, 2003.
   */
  class MeaningfulContinuation
  {
    
  public:

    //////////////////////////////////////////////////
    ///// class EpsilonMeaningfulSegement
    //////////////////////////////////////////////////
    
    class EpsilonMeaningfulSegment{
      
    public:
      
      /**
       * Constructor. 
       *
       * @param epsilon: used to determine is the segment is a good continuation (i.e epsilon< NFA (Number of False Alarm))) 
       * @param Nc: parameter given in the NFA computation.
       * @param kappaMax: maximal angle to consider curvature sequence (max PI/2).
       *
       **/
      
      EpsilonMeaningfulSegment (double epsilon, uint Nc, double kappaMax);
      
      ~EpsilonMeaningfulSegment();
      
      /**
       * Check if the point can be added to the front of contour
       * according the new NFA and epsilon.
       *
       * @param pt: the new point
       * @param index: its associated index of the contour.
       * @return: true if point can be added to the segment with a new NFA < epsilon.
       *
       **/
      
      bool extendFront(const Vector2D &pt, uint index);
      

      /**
       * Check if the point can be added to the back of contour
       * according the new NFA and epsilon.
       *
       * @param pt: the new point
       * @param index: its associated index of the contour.
       * @return: true if point can be added to the segment with a new NFA < epsilon.
       *
       **/

      bool extendBack(const Vector2D &pt, uint index);      
      
      /**
       * Return the index of the last point added to the front. 
       **/
      uint front();

      /**
       * Return the index of the last point added to the back. 
       **/
      uint back();
      
      /**
       * Return the size = number of points added to the segment. 
       **/
      uint size();
      
      
    public:
      
      
    private:
      
      std::deque<Vector2D> m_FrontBackExtremPoints;
      
      uint indexBack;
      uint indexFront;
      
      uint m_size;
      double m_epsilon;
      uint m_Nc;
      double m_dThetaMax;
      double m_kappaMax;
      double NFA;
      
    };
    /// end class EpsilonMeaningfulSegment
    //////////////////////////////////////////////////





    

    // ----------------------- Standard services ------------------------------
  public:



    


    /**
     * Destructor. 
     */
    ~MeaningfulContinuation();

    /**
     * Constructor. The object is not valid yet.
     * @see init
     */
    MeaningfulContinuation();
    
    /** 
     * Initialization from polygon.
     *
     * @param polygon  the coordinates of polygon vertices.
     * @param epsilon: used to determine is the segment is a good continuation (i.e epsilon< NFA (Number of False Alarm))) 
     * @param Nc: parameter given in the NFA computation.
     * @param kappaMax: maximal angle to consider curvature sequence (max PI/2).
     *
     * @param closed when 'true' the polygon is closed, otherwise it
     * is an open polygonal line.
     */
    
    void init(const std::vector<ImaGene::Vector2D> & polygon, double epsilon,
	      double Nc, double kappaMax,  bool closed );
    


    // ----------------------- Simple services --------------------------------
  public:
    
    

    /**
     *  Compute and return the set of all meaningful max segments
     *  computed by maximal front extensions. When a segment can not
     *  be extended on the front, we simply start a new segment from
     *  the last tested point.
     *
     * @return the set of all consecutive max meaningfulsegments.
     */
    std::vector<EpsilonMeaningfulSegment> getMeaningfulMaxSegments() ;
    
    




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


    std::vector<Vector2D> m_polygon;
    double m_epsilon;
    double m_kappaMax;
    double m_Nc;
    bool m_closed;
    
    
    

    // ------------------------- Hidden services ------------------------------
  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE MeaningfulContinuation( const MeaningfulContinuation & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE MeaningfulContinuation & operator=( const MeaningfulContinuation & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
    
    
    
    
    
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'MeaningfulContinuation'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'MeaningfulContinuation' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const MeaningfulContinuation & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/MeaningfulContinuation.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined MeaningfulContinuation_h

#undef MeaningfulContinuation_RECURSES
#endif // else defined(MeaningfulContinuation_RECURSES)



