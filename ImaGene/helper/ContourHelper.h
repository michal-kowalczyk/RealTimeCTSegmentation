/** @file ContourHelper.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : ContourHelper.h
//
// Creation : 2008/05/19
//
// Version : 2008/05/19
//
// Author : JOL
//
// Summary : Header file for files ContourHelper.ih and ContourHelper.cxx
//
// History :
//	2008/05/19 : ?Name? : ?What?
//
// Rcs Id : "@(#)class ContourHelper declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(ContourHelper_RECURSES)
#error Recursive header files inclusion detected in ContourHelper.h
#else // defined(ContourHelper_RECURSES)
#define ContourHelper_RECURSES

#if !defined ContourHelper_h
#define ContourHelper_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <vector>
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Proxy.h"
#include "ImaGene/digitalnD/Frame2D.h"
#include "ImaGene/base/VectorUtils.h"




//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class ContourHelper
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'ContourHelper' <p> Aim: Helper for classes
   * for processing 2D contours or contours on surfaces, as defined by
   * a C4CIterator. Not instantiable.
   */
  class ContourHelper
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~ContourHelper();

    // ----------------------- Static services ------------------------------
  public:

    
    
    typedef struct{
      ImaGene::Vector2D center;
      double radius;      
    }  DiskConstraint;

    typedef std::pair<int, int> Edge;
    typedef std::vector< std::pair< double, Edge > > ValuedEdgeSet;
 
    


    
    /**
     * Displays in [out] the coordinates of the vertices of the
     * 4-connected contour defined by the iterator [cp]. If the
     * dimension of [ks] is greater than 2, then the contour is
     * generally drawn on a surface, and the plane of the contour is
     * specified with [i] and [j].
     * 
     * @param out (returns) the output stream,
     *
     * @param ks the digital space (of dimension 2 or more),
     *
     * @param cp the iterator on the digital contour,
     *
     * @param loop when 'true', the first vertex is displayed again at
     * the end to close the contour,
     * 
     * @param i the first coordinate axis (when ks.dim()>2), default 0,
     *
     * @param j the second coordinate axis (when ks.dim()>2), default 1.
     */
    static void displayContour( std::ostream & out, 
				KnSpace* ks, 
				C4CIteratorOnSurface* cp, 
				bool loop,
				uint i = 0,
				uint j = 1 );





    /**
     * Extract the set of vertices of the 4-connected contour defined by
     * the iterator [cp]. If the dimension of [ks] is greater than 2,
     * then the contour is generally drawn on a surface, and the plane
     * of the contour is specified with [i] and [j].
     * 
     * @param vectResult (returns) the set of vertices,
     *
     * @param ks the digital space (of dimension 2 or more),
     *
     * @param cp the iterator on the digital contour,
     *
     * @param loop when 'true', the first vertex is displayed again at
     * the end to close the contour,
     * 
     * @param i the first coordinate axis (when ks.dim()>2), default 0,
     *
     * @param j the second coordinate axis (when ks.dim()>2), default 1.
     */
    template<class T> 
    static void  compContourVertices( std::vector<T> &vectResult, 
				     KnSpace* ks, 
				     C4CIteratorOnSurface* cp, 
				      bool loop,
				      uint i = 0,
				      uint j = 1, bool inverseYaxis=false){
      
      Proxy<C4CIteratorOnSurface> it( (C4CIteratorOnSurface*) cp->clone() );
      Proxy<C4CIteratorOnSurface> stop( (C4CIteratorOnSurface*) cp->clone() );
      Frame2D frame;
      vectResult.clear();
      
      
      frame.init( ks, i, j );
      do
	{
	  Kn_sid sbel = it->current();
	  frame.setSurfelFrame( sbel, it->trackDir() );
	  Vector2i v= frame.transformPoint( Vector2i( 0, 0 ) ); 
	  if(inverseYaxis){
	    int height= ks->size(1);
	    v.y()= height-v.y()-1;
	  }
	  
	  T p(v.x(), v.y());
	  vectResult.push_back(p);
	  if ( it->next() == 0 ) break;
	}
      while ( ! it->equals( *stop ) );
      if ( loop )
	{
	  Kn_sid sbel = it->current();
	  frame.setSurfelFrame( sbel, it->trackDir() );
	  Vector2i v= frame.transformPoint( Vector2i( 0, 0 ) ); 
	   if(inverseYaxis){
	    int height= ks->size(1);
	    v.y()= height-v.y()-1;
	   }
	  T p( v.x(), v.y());
	  vectResult.push_back(p);
	}
    
      
    }

      
    
    
    


    /**
     * Displays in [out] the contour as a freeman chaincode
     * (e.g. "000100101120..."). The 4-connected contour is defined by
     * the iterator [cp]. If the dimension of [ks] is greater than 2,
     * then the contour is generally drawn on a surface, and the plane
     * of the contour is specified with [i] and [j].
     * 
     * @param out (returns) the output stream,
     *
     * @param ks the digital space (of dimension 2 or more),
     *
     * @param cp the iterator on the digital contour,
     *
     * @param i the first coordinate axis (when ks.dim()>2), default 0,
     *
     * @param j the second coordinate axis (when ks.dim()>2), default 1.
     */
 
    static void displayContourWord( std::ostream & out, 
				    KnSpace* ks, C4CIteratorOnSurface* cp,
				    uint i = 0, uint j = 0 );

    /**
     * Displays in [out] the contour as a freeman chaincode
     * (e.g. "x y 000100101120..."). The 4-connected contour is defined by
     * the iterator [cp]. If the dimension of [ks] is greater than 2,
     * then the contour is generally drawn on a surface, and the plane
     * of the contour is specified with [i] and [j].
     * 
     * @param out (returns) the output stream,
     *
     * @param ks the digital space (of dimension 2 or more),
     *
     * @param cp the iterator on the digital contour,
     *
     * @param i the first coordinate axis (when ks.dim()>2), default 0,
     *
     * @param j the second coordinate axis (when ks.dim()>2), default 1.
     * 
     * @param inverseYaxis used to switch the vertical representation (used for DGtal LibBoard), default false.   
     * 
     */
 
    static void displayFreemanChain( std::ostream & out, 
				     KnSpace* ks, C4CIteratorOnSurface* cp,
				     uint i = 0, uint j = 0, bool inverseYaxis=false );
    

    
    /**
     * Extract a polygon from an input stream. Each line of the input
     * stream should represent a vertex of the polygon.  The two
     * coordinate positions in the line can be specified by indiceX,
     * indiceY (default 0 and 1). If one value x ou y is not found, the point
     * is not added to the vector result.
     *
     * @param in the input stream,
     * @param indiceX indiceY, the position of the vertex coordinates in each line. 
     *
     * @return the vector containing the vertex polygon.
     */
    
    static std::vector <Vector2D> getPolygonFromStream(std::istream &in, 
						       uint indiceX=0, uint indiceY=1);



    /**
     * Extract all the polygons from an input stream. Each line of the input
     * stream should the set of all vertex of a polygon.  
     *
     * @param in the input stream,
     *
     * @return the vector containing all the polygon.
     */
    
    static std::vector <std::vector <ImaGene::Vector2D> >
    getPolygonsFromStream(std::istream &in);
   
 
    
    /**
     * Return all the iso contours of level [threshold] of an
     * pgm image. 
     *
     *
     * @param in: input stream associated to the pgm image file.  
     * @param threshold: used to select the set of iso contours.
     * @param minSize: select only contour of min size (default 0). 
     * @return the vector containing all the contour.
     *
     **/
    
    static std::vector<std::vector<ImaGene::Vector2D> > 
    getImageIsoContours(std::istream &in, uint threshold, uint minSize=0, bool inverseYaxis=false);

    


    


    
    /**
     * Determine if a contour is counter clockwise or not.
     
     * @param contour the vector of vertex polygon,
     * @return true if the polygon is counter clockwise oriented.
     */ 

    template<class T> 
    static bool isCCW(const std::vector<T> &contour){
      int size = contour.size();
      double somme=0.0;
      T  p0 = contour.at(0);
      for(int i=1; i<size; i++){
	T p = contour.at(i);
	T pSuiv = contour.at((i+1)%size);
	double v1x = p.x()- p0.x();
	double v1y = p.y()- p0.y();
	double v2x = pSuiv.x()- p.x();
	double v2y = pSuiv.y()- p.y();
	somme+=(v1x*v2y)-(v2x*v1y);
      }
      return (somme>0.0);     
    }
    
    

    /***
     * Verify if a contour is Counter Clockwise, if not the contour is reversed.
     *
     * @param (return) contour
     * @return true if the contour was reversed.
     */
    
    
    template<class T> 
    static bool transformCCW(std::vector<T> &contour){
      if(!isCCW(contour)){
	for(int i=0;i<(contour.size()/2); i++){
	  T tmp = contour.at(i);
	  contour.at(i)= contour.at(contour.size()-i-1);
	  contour.at(contour.size()-i-1)=tmp;      
	}    
	return true;
      }else
	return false;
    }
    

    /***
     * Verify if a contour is Counter Clockwise, if not the contour is reversed.
     *
     * @param (return) contour
     * @return true if the contour was reversed.
     */
    
    
    template<class T> 
    static bool transformCW(std::vector<T> &contour){
      if(isCCW(contour)){
	for(int i=0;i<(contour.size()/2); i++){
	  T tmp = contour.at(i);
	  contour.at(i)= contour.at(contour.size()-i-1);
	  contour.at(contour.size()-i-1)=tmp;      
	}    
	return true;
      }else
	return false;
    }
    

/**
 * Extract a polygon from an input stream. Each line of the input
 * stream should represent a vertex of the polygon.  The two
 * coordinate positions in the line can be specified by indiceX,
 * indiceY (default 0 and 1). If one value x ou y is not found, the point
 * is not added to the vector result. 
 *
 * @param in the input stream,
 * @param indiceX indiceY, the position of the vertex coordinates in each line. 
 *
 * @return the vector containing the vertex polygon.
 */

    template <class T> 
    static std::vector<T>
    extractField(std::istream &in, uint indice){
      std::vector<T> vectResult;
      std::string str;
      getline(in, str );
      while ( in.good() ){
	if ( ( str != "" ) && ( str[ 0 ] != '#' ) ){
	  std::istringstream in_str( str );
	  uint idx = 0;
	  T val;
	  bool found=false;
	  while ( in_str.good()&& !found){
	    bool isOK = (in_str >> val);
	    if (isOK && (idx == indice) ){
	      found=true;
	    }
	    idx++;
	  }
	  
	  if(found){
	    vectResult.push_back(val);
	  }
	}
	getline(in, str );
      }
      return vectResult;
    }
  





    /**
     * Estimate if an polygonal curve can be considered as an open or
     * close curve. The algorithm first computes the mean distance
     * between the curve vertices. If the distance between extremety
     * points are greater than [factor] times [mean distance] it
     * returns true;
     *
     * @param polygon : the open or close polygon 
     
     * @param factor : the factor for considering that extremety
     * points are closed enought to be considered as
     * closed. (default=1)     
     *
     **/

    template <class T> 
    static bool
    isOpenPolygon(const std::vector<T>  &polygon, double factor=2.0){
      double meanDistance = getMeanVertexDistances(polygon, false);
      T ptA = polygon.at(0);
      T ptB = polygon.at(polygon.size()-1);
      double extrDistance = VectorUtils::norm(Vector2D(ptA.x() -ptB.x(), ptA.y() -ptB.y()));
      return extrDistance > meanDistance*factor;
      
    }


    template <class T>
    static double 
    getMeanVertexDistances(const std::vector<T> &polygon, bool isClosed=false){
      double mean=0.0;
      for(int i=1; i< polygon.size(); i++){
	T ptA = polygon.at(i);
	T ptB = polygon.at(i-1);
	double dist  =VectorUtils::norm(T(ptA.x() -ptB.x(), ptA.y() -ptB.y()));
	mean+=dist;
      }
      if(isClosed){
	double dist = VectorUtils::norm(T(polygon.at(0).x()-polygon.at(polygon.size()-1).x(),
						 polygon.at(0).y()-polygon.at(polygon.size()-1).y()));
	mean/=polygon.size();
      }else{
	mean/=(polygon.size()-1);
      }
      return mean;
    }



/**
 * Extract the polygon part located between the two point ptA and ptB (included).
 *
 *
 * @param polygon 
 * @param ptA, ptB 
 *
 * @return the vector containing the vertex polygon located betwee ptA and ptB.
 *
 */
    
    template <class T> 
    static std::vector<T>
    selectPolygonPart(const std::vector<T>  &polygon, T ptA, T ptB){
      std::vector<T> vectResult;
      int indicePtA=-1;
      int indicePtB=-1;
      // Need first to find ptA to avoid loop configuration... 
      for(int i=0; i<polygon.size(); i++){
	if((indicePtA==-1) && (polygon.at(i).x()==ptA.x()) &&( polygon.at(i).y()==ptA.y())){
	  indicePtA = i;
	}else if((indicePtA!=-1) && (indicePtB==-1) && (polygon.at(i).x()==ptB.x()) && (polygon.at(i).y()==ptB.y())){
	  indicePtB = i;
	  break;
	}
      }
      
      // Not found, perhaps ptB was before ptA
      if( indicePtA !=-1 && indicePtB==-1){
	for(int i=0; i<polygon.size(); i++){
	  T pt = polygon.at((i+indicePtA)%polygon.size());
	  if(pt.x()==ptB.x() && pt.y()==ptB.y()){
	    indicePtB=(i+indicePtA)%polygon.size();
	    break;
	  } 
	}	
      }
      
      if(indicePtA==-1 || indicePtB==-1){
	std::cerr << "Warning empty vector (PtA or PtB not found...)" << std::endl;
	return vectResult;
      }
      
      if(indicePtA==indicePtB){
	std::cerr << "Warning selection contains only one point" << std::endl;
	vectResult.push_back(polygon.at(indicePtA));
	vectResult.push_back(polygon.at(indicePtB));
	return vectResult;
      }
      for(int i=indicePtA; (i%polygon.size())!=indicePtB; i++){
	vectResult.push_back(polygon.at(i%polygon.size()));
      }
      vectResult.push_back(polygon.at(indicePtB));
      return vectResult;
    }
    
    



    
    /**
     * Compute the projection of a Point ptC on the real line defined by the two points (ptA,ptB), and
     * return true if the projected point is inside the segment closed interval [A,B].
     * 
     * @param ptA, ptB : the two points defining the straight line.
     * @param ptC: the point to be projected.
     * @param ptProjected (return) the projected point.
     * @return: true if ptProjected is inside the segment [A,B].
     **/
    
    INLINE static bool 
    projetOnStraightLine(const ImaGene::Vector2D & ptA, const ImaGene::Vector2D & ptB,
			 const ImaGene::Vector2D & ptC,
			 ImaGene::Vector2D &ptProjected);
    



    /**
     * Compute if exists the intersection between two segments [ptA, ptB] and [ptC, ptD].
     *
     * @param ptA, ptB, ptC, ptD : the two segments [ptA, ptB] and [ptC, ptD].
     * @param (return) ptProjected  
     *
     **/    
    
    
    INLINE static bool
    computeSegmentIntersection(const ImaGene::Vector2D & ptA, const ImaGene::Vector2D & ptB, 
			       const ImaGene::Vector2D & ptC, const ImaGene::Vector2D & ptD,
			       ImaGene::Vector2D & ptIntersection);    

    
    
    
    
    
    /**
     * Transform by changing if necessary the point order and adding
     * two distincts intersecton points or simply remove a loop (with
     * parameter removeLoop) .
     *  
     * @param vectContour : the contour.
     *
     * @param removeLoop: used to remove loop when detected else the loop is transformed.
     * @param epsilon: the shift used to duplicate intersection point
     * detected in the transformation.
     * @param precision: used to detect special case (intersection alignement with contour point).  
     * 
     **/
    


    static std::vector<ImaGene::Vector2D> 
    transformLoop(const std::vector<ImaGene::Vector2D> &vectContour, bool removeLoop, 
		  double epsilon=0.2, double aPrecision=0.0001);
    


    
    static bool containsLoop(const std::vector<ImaGene::Vector2D> &vectContour);
    

    
    /**
     * Used to transform a polygon from index @param index, by
     * changing if necessary the point order and adding two distincts
     * intersecton points or simply remove a loop (with parameter
     * removeLoop) .
     *  
     * @param index (return): the index starting the detection. The
     * value is updated after the detection to the index of the first
     * loop detected.
     * 
     * @param loopFound (return): give the status detection.
     * @param vectContour : the  contour.
     *
     * @param removeLoop: used to remove loop when detected else the loop is transformed.
     * @param epsilon: the shift used to duplicate intersection point
     * detected in the transformation.
     * @param precision: used to detect special case (intersection alignement with contour point).  
     * 
     * @return the new contour.
     **/
    

    static std::vector<ImaGene::Vector2D> 
    transfromALoopFromIndex(uint &index, bool &loopFound, const std::vector<ImaGene::Vector2D> &vectContour, 
			    bool removeLoop, double epsilon=0.2, double aPrecision=0.0001);
    
    

    

    /**
     * Compute the bondind box of the contour.
     *
     *  @param contour 
     *  @param (return) ptMinXY the lower left point of the bounding box.
     *  @param (return) ptMaxXY the upper right point of the bounding box.
     **/
    
    static void computeContourBoundingBox(const std::vector<ImaGene::Vector2D> & contour, ImaGene::Vector2D &ptMinXY, 
					  ImaGene::Vector2D &ptMaxXY );

    
   

 
    /**
     * Compute a new contour by a iterative process which defines a
     * new vertex by the mean value of its two neigborhood points. At
     * each step the transformation is applied only if the resulting
     * point belong the disk constraints.
     * 
     *  @param contour : the initial source contour.  
     *  @param :constraint: the disk constraint.
     * 
     *  @param constraintIndex: give for each point the position of its
     *  associated constraint (needed since others constraints are
     *  added by interpolation between two consecutive vertex).
     *     
     *  @param epsilon: precision used to stop the iterative process.
     *  @param coeffPtExt: the weight of the two neighboor vertex used to compute the mean point in the iterative process. 
     *  @param coeffPt: the weight of the considered vertex used to compute the mean point in the iterative process. 
     *  epsilon @return the new contour.
     **/   

    static std::vector<ImaGene::Vector2D>
    smoothContourFromNoise(const std::vector<ImaGene::Vector2D> & contour, const std::vector<DiskConstraint> &constraint,
			   const std::vector<uint> &constraintIndex, double epsilon=0.0001, double coeffPtExt=0.5,
			   double coeffPt=1.0, bool isOpen=true );
    


    
    /**
     * Check if a point verify the disk constraint.
     *
     * @param pt: the point to checked.
     * @param index: the index of considered point.
     * @param the vector containing all the constraint.
     * @param constraintIndex: give for each point the position of its
     *  associated constraint (needed since others constraints are
     *  added by interpolation between two consecutive vertex).
     *
     * @return true if almost one constraint is checked.
     *
     **/
    
    static bool
    checkDiskConstaints(ImaGene::Vector2D pt, uint index, const std::vector<DiskConstraint> &constraint, 
			const std::vector<uint> &constraintIndex);




    /**
     *  Compute the Minimum Euclidean Spanning Tree from a set of
     *  unordered point by using the Kruskal algorithm
     * 
     *  @param pointSet: the source of unordered point set.
     *  @return the Minimum Euclidean Spanning Tree
     *
     **/
    
    static ValuedEdgeSet
    getMinEuclideanSpanningTree(const std::vector<ImaGene::Vector2D> &pointSet);
    
    

    
    
   
    
    
    /**
     * Return the vector containing the vertex index of the
     * longestPath contained in the tree passing through the root.
     *  
     * @param graph: the initial tree represented by its set of vertex
     * @param nbVertex: the number of the vertex set of the tree.
     * @return the index associated to the point longest path passing through the root. 
     **/
    
    static std::vector<uint> getLongestPathFromTree(const ValuedEdgeSet & tree, uint nbVertex );
    
    

    static std::vector<uint> getLeaf(const ValuedEdgeSet &graph, uint nbVertex);  
    static std::vector<uint> getParent(const ValuedEdgeSet &graph, uint nbVertex, std::vector<double> &weightToParent);
    
   
    
  
    
   
    
    static std::vector<std::vector<ImaGene::Vector2D> >
    filterMeaningfulParts(std::vector<Vector2D> contour, const std::vector<DiskConstraint> &constraint,
			  const std::vector<uint> &constraintIndex, double threshold, 
			  const std::vector<double> & slopeConstraints, double slopeThresholdMin, double slopeThresholdMax);
    
    




  private:
    static uint  getSetParent(const std::vector<uint> &vectParent, uint vertexId);
    static std::vector<uint> getVertexDegree(const ValuedEdgeSet &graph, uint nbVertex);
    

    
    

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

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE ContourHelper();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE ContourHelper( const ContourHelper & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE ContourHelper & operator=( const ContourHelper & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'ContourHelper'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'ContourHelper' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const ContourHelper & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/ContourHelper.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ContourHelper_h

#undef ContourHelper_RECURSES
#endif // else defined(ContourHelper_RECURSES)
