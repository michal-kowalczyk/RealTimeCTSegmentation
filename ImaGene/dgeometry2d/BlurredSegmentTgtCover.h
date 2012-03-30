/** @file BlurredSegmentTgtCover.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : BlurredSegmentTgtCover.h
//
// Creation : 
//
// Version : 
//
// Author : MS
//
// Summary : Header file for files BlurredSegmentTgtCover.ih and BlurredSegmentTgtCover.cxx
//
// History :
//	2010/8/10 : ?Name? : ?What?
//
// Rcs Id : "@(#)class BlurredSegmentTgtCover declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(BlurredSegmentTgtCover_RECURSES)
#error Recursive header files inclusion detected in BlurredSegmentTgtCover.h
#else // defined(BlurredSegmentTgtCover_RECURSES)
#define BlurredSegmentTgtCover_RECURSES

#if !defined BlurredSegmentTgtCover_h
#define BlurredSegmentTgtCover_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <deque>
#include <vector>
#include "ImaGene/dgeometry2d/C4CSegment.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/Vector.h"
#include "ImaGene/dgeometry2d/C4CIterator.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/mathutils/Statistics.h"
#include "ImaGene/helper/ScaleProfile.h"

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  ///////////////////////////////////////////////////////////////////////////////
  // class BlurredSegmentTgtCover
  ///////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'BlurredSegmentTgtCover' <p> Aim: A tangential
   * cover is the set of maximal segments associated with a digital
   * curve.
   */
  class BlurredSegmentTgtCover
  {
  
  public: 

    struct Segment{
      /**
       * debut : Indices of the points in the curve structure.
       */
      int m_first;
      /**
       * fin : Indices of points in the curve structure.
       */
      int m_last;
      /**
       * Coefficient a in (a,b,mu,omega) characterizing the line segment.
       */
      double m_a;
      /**
       * Coefficient b in (a,b,mu,omega) characterizing the line segment.
       */
      double m_b;
      /**
       * Coefficient mu in (a,b,mu,omega) characterizing the line segment.
       */
      double m_mu;
      /**
       * Coefficient omega in (a,b,mu,omega) characterizing the line segment.
       */
      double m_omega;



      // Ajout BK 

      /** 
       * Points used to display Blurred Segment and to compute segment length. 
       * Length is given by ||ptALongestSegment1, ptBLongestSegment1|| or  by
       * ||ptCLongestSegment1, ptDLongestSegment1||
       * 
       * 
       *  The boundingBox of a blurred segment can then be displayed for instance by:
       * ptALongestSegment1, ptBLongestSegment1, ptCLongestSegment2, ptDLongestSegment2 .
       *
       **/

     
      Vector2D ptALongestSegment1, ptBLongestSegment1, ptCLongestSegment2, ptDLongestSegment2;
      
      Vector2D ptP, ptQ, ptS;
      
      int nbElementsAfterInitialisation;
      uint nbElements;
      
      double boxLength;
      
      // Fin Ajout BK
      
     
      // ------------------------- Accessors --------------------------------------

    public:

      /**
       * @return coefficient a in (a,b,mu,omega) characterizing the line segment.
       */
      INLINE double a() const;
      /**
       * @return coefficient b in (a,b,mu,omega) characterizing the line segment.
       */
      INLINE double b() const;
      /**
       * @return coefficient mu in (a,b,mu,omega) characterizing the line segment.
       */
      INLINE double mu() const;
      /**
       * @return coefficient omega in (a,b,mu,omega) characterizing the line segment.
       */
      INLINE double omega() const;
      /**
       * @return the indice of the first point in the line segment.
       */
      INLINE int first() const;
      /**
       * @return the indice of the last point in the line segment.
       */
      INLINE int last() const;
 
      
      /**
       * The double liste queue for Melkman algorithm
       */
      
      //  20/02/2011  Adaptation/Fusion code de Mouhammad par rapport à l'amélioration
      //  de l'estimation de la boite englobante du segment flou.
      std::deque<int> D;
      
      

    };

  public:

    struct Inclusion{
      /**
       * Moyenne of the lenght of Maximal Segment.
       */
      double m_moyenne;

      /**
       * ecart_type of the lenght of Maximal Segment.
       */
      double m_ecart_type;

      /**
       * Number of inclusion of Maximal Segment.
       */
      std::vector<int> number_inclusion;  

    public:
 
      /**
       * @return Moyenne of the lenght of Maximal Segment.
       */
      INLINE double moyenne() const;

      /**
       * @return ecart_type of the lenght of Maximal Segment.
       */
      INLINE double ecart_type() const;

  
    };

    // ------------------------- Standard Services ------------------------------------------
  public:
 
    /**
     * Destructor.
     */
    ~BlurredSegmentTgtCover();

    /**
     * Initialize the Tangential cover.
     */
    void init();



    /**
     * Initialization by using the FreemanChain c to define the contour.
     *
     *  @param c FreemanChain 
     *  @result initialisation of the contour from the FreemanChain
     *
     */
  
    void  init( const ImaGene::FreemanChain & c );



  
    /**
     * Initialization by using the FreemanChain c to define the contour.
     *
     *  @param contourVect the recontour represented by Vector2D 
     *  @result initialisation of the contour from the vector of  Vector2D points
     *
     */

    void  init( const std::vector<ImaGene::Vector2D> & contourVect, bool isClosed=true );




  



    // ------------------------- Datas ------------------------------------------
  public: 

    /**
     * Stores the set of Segments of the characteristic shape.
     */
    std::vector<Segment> m_Segments;

    /**
     * Stores the set ( moyenne and ecart_type ) of Inclusion.
     */
    Inclusion m_inclusion;

    /**
     * The double liste queue for Melkman algorithm
     */
    std::deque<int> D;

    /**
     * The set of the double liste queue for Melkman algorithm
     */
    std::vector< std::deque<int> > VectDeque;    
 
    /*
     * The points (x,y) of the curve.
     */
    std::vector<Vector2D> PointsCurve;
 
    /**
     * the height of the convex hull
     */
    double height;

    /**
     * the width of the convex hull
     */
    double width;



    bool myIsClosed;
    
    Vector2D edgeP, edgeQ, vertexS;
    Vector2D edgePh, edgeQh, vertexSh;
    Vector2D edgePw, edgeQw, vertexSw;
    Vector2D sav_P, sav_Q, sav_S;
    /**
     * The number of points (x,y) of the curve.
     */
    int nbpointsCurve;

    /**
     * test if a point is Left|On|Right of an infinite line.
     * Input: three pointd P0, P1 and P2
     * Return: >0 for P2 left of the line through P0 and P1
     *         =0 for P2 on the linegetPointsContour();
     *         <0 for P2 right of the line.   i
     */
    INLINE double IsLeft( int P0 , int P1 , int P2  ) const;

    /**
     * @return the total points of a contour.
     */
    INLINE std::vector<Vector2D> getPointsContour() const;

    /**
     * @return the m_segments(first_point,last_point,a,b,mu,omega) of the 
     * tangential cover of the contour.
     */
    INLINE std::vector<Segment> getSegmentContour() const;

    /**
     * @return the m_inclusion(moyenne and ecart_type) of lenght of the
     * tangential cover of the contour.
     */
    INLINE Inclusion getInclusion() const;

    /**
     * @return the points bound (P,Q,S) of the 
     * tangential cover of the contour.
     */
    INLINE std::vector<Vector2D> getPointsBoundTC() const;

  





    // ------------------------- Internals ------------------------------------------
  public:
 
 
    /**
     * InitMelkman
     * Initialize Melkman algorithm.
     */
    void InitMelkman( int P0 , int P1 , int P2  );
  
    /**
     * Melkman
     * Add a point in a convex using one step of Melkman algorithm.
     */
    void Melkman( int P );

    /**
     * IsConvexValid
     * Depending on connexity, return if a convex is valid.
     */
    bool IsConvexValid( std::deque<int> & D , float thick , int connexity , int blurred );

    /**
     * MainDiagonal
     * Return the main diagonal width of a convex set
     */
    void MainDiagonal( std::deque<int> & D );

    /**
     * ExtractSegment
     * For all points of the curve, compute the longest DSS starting at the point.
     * Note: write the result to stdout in formi
     * Point (x,y) P (x,y) Q (x,y) S (x,y)
     */
    void ExtractSegment( float thick , int connexity , int blurred );
 
    /**
     * Print TgCover
     * Print out the computed Tangential Cover.
     */
    void PrintTgCover();
 
 





  

    /**
     * Get statistics for each point of the contour
     * 
     * @return a dyn. alloc. statistics object storing the length of maximal segments for each contour point. 
     * Contains as many statistics variables as the number of surfels of [fc]. 
     * 
     **/
    // Ajout BK 18/02/2011
  
    Statistics* getStatsMaximalSegments(double scaleWidth);
  

    
    
    void  getScaleProfile(std::vector<double>  vectScalesWidth, uint index, ScaleProfile &sp);
    
    
    
    
    void computeNoiseAndSlope(std::vector<double> &vectNoise, 
			       std::vector<double> &vectSlope, 
			       std::vector<double> vectScalesWidth, uint minSize, double maxSlope);
    
    
    
    std::vector<double>  getNoiseLevels(std::vector<double>  vectScaleWidth, uint minSize, double maxSlope);
    
    

    /**
     *  Compute and update the Bounds of the BlurredSeg according different parameters.
     * 
     * @param segment (return)
     *
     **/

    //  20/02/2011  Adaptation/Fusion code de Mouhammad par rapport à l'amélioration
    //  de l'estimation de la boite englobante du segment flou.


    void 
    compSegmentBoundsFromExtremPt( const ImaGene::Vector2D & fp, const ImaGene::Vector2D & lp,
				   ImaGene::BlurredSegmentTgtCover::Segment & aSegment, double minVisibleWidthBounds =0.2);
    
    


    /**
     * Compute the basic Bounds of Blurred Segmente by updating the values of segment bounding points 
     *  ptALongestSegment1, ptBLongestSegment ptCLongestSegment2 and ptDLongestSegment2
     * 
     * @param aSegment (return).
     */
    //  20/02/2011  Adaptation/Fusion code de Mouhammad par rapport à l'amélioration
    //  de l'estimation de la boite englobante du segment flou.

    
    void computeBasicBounds( ImaGene::BlurredSegmentTgtCover::Segment & aSegment, double minVisibleWidthBounds=0.2);
    
  
    

    /**
     * Compute the Bounds of Blurred Segmente by taking into account
     *  correctly the extremity even by change of direction for the end
     *  points.  The values of segment bounding points are updated (
     *  ptALongestSegment1, ptBLongestSegment ptCLongestSegment2 and
     *  ptDLongestSegment2)
     * 
     * @param aSegment (return).
     */
    
    //  20/02/2011  Adaptation/Fusion code de Mouhammad par rapport à l'amélioration
    //  de l'estimation de la boite englobante du segment flou.
    
    

    void computeRealBounds( ImaGene::BlurredSegmentTgtCover::Segment &aSegment,  double minVisibleWidthBounds=0.2);
      
      
  };
} // namespace ImaGene 


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/BlurredSegmentTgtCover.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined BlurredSegmentTgtCover_h

#undef BlurredSegmentTgtCover_RECURSES
#endif // else defined(BlurredSegmentTgtCover_RECURSES)
