///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ContourHelper.cxx
//
// Creation : 2008/05/19
//
// Version : 2008/05/19
//
// Author : Jacques-Olivier Lachaud
//
// email : lachaud@labri.fr
//
// Purpose : ??
//
// Distribution :
//
// Use :
//	??
//
// Todo :
//	O ??
//
// History :
//	2008/05/19 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <algorithm>

#include "ImaGene/helper/ContourHelper.h"
#include "ImaGene/helper/ShapeHelper.h"

#include "ImaGene/base/Proxy.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/Shapes.h"

#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/digitalnD/Frame2D.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnShapes.h"
#include "ImaGene/digitalnD/C4CIteratorOnBdry.h"

// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/ContourHelper.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace ImaGene;

const char* const ContourHelper_RCS_ID = "@(#)class ContourHelper definition.";



///////////////////////////////////////////////////////////////////////////////
// class ContourHelper
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ContourHelper::~ContourHelper()
{
}

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
void
ImaGene::ContourHelper::displayContour( std::ostream & out, 
					KnSpace* ks, 
					C4CIteratorOnSurface* cp, 
					bool loop,
					uint i,
					uint j )
{
  //   cout << "#######################################################" << endl;
  //   cout << "# displayBoundary" << endl
  //        << "# displays the points on the boundary as couples." << endl
  //        << "# x y" << endl;
  Proxy<C4CIteratorOnSurface> it( (C4CIteratorOnSurface*) cp->clone() );
  Proxy<C4CIteratorOnSurface> stop( (C4CIteratorOnSurface*) cp->clone() );
  Frame2D frame;
  frame.init( ks, i, j );
  do
    {
      Kn_sid sbel = it->current();
      frame.setSurfelFrame( sbel, it->trackDir() );
      Vector2i p( frame.transformPoint( Vector2i( 0, 0 ) ) );
      out << p.x() << " " << p.y() << endl;
      if ( it->next() == 0 ) break;
    }
  while ( ! it->equals( *stop ) );
  if ( loop )
    {
      Kn_sid sbel = it->current();
      frame.setSurfelFrame( sbel, it->trackDir() );
      Vector2i p( frame.transformPoint( Vector2i( 0, 0 ) ) );
      out << p.x() << " " << p.y() << endl;
    }

}





/**
 * Displays in [out] the contour as a freeman chaincode
 * (e.g. "000100101120..."). The 4-connected contour is defined by the
 * iterator [cp]. If the dimension of [ks] is greater than 2, then the
 * contour is generally drawn on a surface, and the plane of the
 * contour is specified with [i] and [j].
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
 
void
ImaGene::ContourHelper::displayContourWord
( ostream & out, 
  KnSpace* ks, C4CIteratorOnSurface* cp,
  uint i, uint j )
{
  Proxy<C4CIteratorOnSurface> it( (C4CIteratorOnSurface*) cp->clone() );
  Proxy<C4CIteratorOnSurface> stop( (C4CIteratorOnSurface*) cp->clone() );
  Frame2D frame;
  frame.init( ks, 0, 1 );
  do
    {
      Kn_sid sbel = it->current();
      frame.setSurfelFrame( sbel, it->trackDir() );
      Vector2i p1( frame.transformPoint( Vector2i( 0, 0 ) ) );
      Vector2i p2( frame.transformPoint( Vector2i( 1, 0 ) ) );
      int dx = p2.x() - p1.x();
      int dy = p2.y() - p1.y();
      char code = '0' + (char) ( dx != 0 ? (1 - dx) : (2 - dy) );
      out << code; //<< "(" << dx << "," << dy << ")";
      if ( it->next() == 0 ) break;
    }
  while ( ! it->equals( *stop ) );
  out << endl; // << "# nb=" << nb << endl;
}

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
 */

void
ImaGene::ContourHelper::displayFreemanChain
( std::ostream & out, 
  KnSpace* ks, C4CIteratorOnSurface* cp,
  uint i, uint j, bool inverseYaxis )
{
  ostringstream oss;
  FreemanChain fc;
  Proxy<C4CIteratorOnSurface> it( (C4CIteratorOnSurface*) cp->clone() );
  Proxy<C4CIteratorOnSurface> stop( (C4CIteratorOnSurface*) cp->clone() );
  Frame2D frame;
  frame.init( ks, 0, 1 );
  bool first = true;
  do
    {
      Kn_sid sbel = it->current();
      frame.setSurfelFrame( sbel, it->trackDir() );
      Vector2i p1( frame.transformPoint( Vector2i( 0, 0 ) ) );
      Vector2i p2( frame.transformPoint( Vector2i( 1, 0 ) ) );
      if ( first )
	{
	  
	  fc.x0 = p1.x();
	  fc.y0 = p1.y();
	  if(inverseYaxis){
	    uint height= ks->size(1);
	    fc.y0 = height-p2.y()-1;
	  }
	  first = false;
	}
      int dx = p2.x() - p1.x();
      int dy = p2.y() - p1.y();
      if(inverseYaxis){
	dy=-dy;
      }
      char code = '0' + (char) ( dx != 0 ? (1 - dx) : (2 - dy) );
      oss << code; 
      if ( it->next() == 0 ) break;
    }
  while ( ! it->equals( *stop ) );
  fc.chain = oss.str();
  FreemanChain::write( out, fc );
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

vector <ImaGene::Vector2D> 
ImaGene::ContourHelper::getPolygonFromStream(std::istream &in, uint indiceX, uint indiceY){
  vector<Vector2D> vectResult;
  string str;
  getline(in, str );
  while ( in.good() ){
    if ( ( str != "" ) && ( str[ 0 ] != '#' ) ){
      istringstream in_str( str );
      int idx = 0;
      double val=0.0;
      bool foundX=false;
      bool foundY=false;
      double x=0.0, y=0.0;
      while ( in_str.good()&& (!foundX || !foundY)){
	bool isOK = (in_str >> val);
	if (isOK && (idx == indiceX) ){
	  foundX=true;
	  x=val;
	}
	if (isOK &&  (idx == indiceY) ){
	  foundY=true;
	  y=val;
	}
      	++idx;
      }
      
      if(foundX && foundY){
	vectResult.push_back(Vector2D(x,y));
      }
    }
    getline(in, str );
  }
  return vectResult;
}







/**
 * Extract all the polygons from an input stream. Each line of the input
 * stream should the set of all vertex of a polygon.  
 *
 * @param in the input stream,
 *
 * @return the vector containing all the polygon.
 */

vector <vector <ImaGene::Vector2D> >
ImaGene::ContourHelper::getPolygonsFromStream(std::istream &in){
  vector< vector<Vector2D> > vectResult;
  string str;
  getline(in, str );
  while ( in.good() ){
    if ( ( str != "" ) && ( str[ 0 ] != '#' ) ){
      vector <ImaGene::Vector2D> aContour;
      istringstream in_str( str );
      double x=0.0, y=0.0;
      while ( in_str.good()){
	bool isOK = (in_str >> x);
	isOK = isOK &&  (in_str >>y);
	if(isOK){
	  aContour.push_back(Vector2D(x,y));
	}
      }
      vectResult.push_back(aContour);
    }
    getline(in, str );
  }
  return vectResult;
}



  

    
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
    
    
std::vector<std::vector<ImaGene::Vector2D> > 
ImaGene::ContourHelper::getImageIsoContours(std::istream &in, uint threshold, uint minSize, bool inverseYaxis){
  KnSpace* ks;
  KnCharSet* voxset;
  vector<std::vector<ImaGene::Vector2D> > vectResult;
  ShapeHelper::importFromPGM( in, ks, voxset, threshold, 1, true );
  KnRCellSet bdry = KnShapes::smakeBoundary( *ks, *voxset );
  KnRCellSet not_visited( bdry );
  BelAdjacency badj( *ks, true );
  for ( KnRCellSet::cell_iterator cell_it = bdry.begin();
	cell_it != bdry.end();
	++cell_it )
    {
      Kn_sid bel = *cell_it;
      uint k = *( ks->sbegin_dirs( bel ) );
      C4CIteratorOnBdry c4c_it( badj, bel, k, *voxset );
      bool is_open;
      uint nb_surfels = C4CIterator::size( c4c_it, is_open );
      if ( nb_surfels >= minSize )
	{
	  Proxy<C4CIteratorOnSurface> cp
	    ( (C4CIteratorOnSurface*) c4c_it.clone() );
	  vector<Vector2D> v;
	  ImaGene::ContourHelper::compContourVertices(v,  ks, cp, !is_open,  0, 1 , inverseYaxis);
	  vectResult.push_back(v);
	  
	}
      // Clear contour from set of bels.
      bel = c4c_it.current();
      Kn_sid sbel = bel;
      do
	{
	  bdry[ bel ] = false;
	  if ( c4c_it.next() == 0 ) break;
	  bel = c4c_it.current();
	}
      while ( bel != sbel );
    }
  delete ks;
  delete voxset;
  
  return vectResult;
}

  



    


bool
ImaGene::ContourHelper::containsLoop(const std::vector<ImaGene::Vector2D> &vectContour){
  for(int i=3; i< vectContour.size(); i++){
    
    Vector2D ptA = vectContour.at(i);
    Vector2D ptB = vectContour.at(i-1);
    
    for(int k=i-2; k-1>=0; k--){
      Vector2D ptC = vectContour.at(k);
      Vector2D ptD = vectContour.at(k-1);
      Vector2D intersection;
      bool inter= computeSegmentIntersection(ptA, ptB, ptC, ptD, intersection);
      if(inter){
	cerr << "Intersection at :" << intersection.x() << " " << intersection.y() << endl;
	return true;
      }
      
    }
    
  }
  return false;
}
   





    
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
    


vector<Vector2D> 
ImaGene::ContourHelper::transformLoop(const std::vector<ImaGene::Vector2D> &vectContour, bool removeLoop, double epsilon, double aPrecision){
  vector<Vector2D> contourTransformed =vectContour;
  bool transf=true;
  uint nbLoopRemoved= 0;
  uint posStart=2;
  cerr<< "Checking for loop : [" ;
  while(transf){
    cerr << ".";
    contourTransformed = transfromALoopFromIndex(posStart, transf, contourTransformed, removeLoop, epsilon, aPrecision);
    nbLoopRemoved++;
  }
  cerr << "]"<< endl;;
  return contourTransformed;
}









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

vector<Vector2D> 
ImaGene::ContourHelper::transfromALoopFromIndex(uint &index, bool &loopFound, const std::vector<ImaGene::Vector2D> &vectContour, 
						bool removeLoop, double epsilon, double aPrecision){
  vector<Vector2D> contourTransformed;
  // use to improve detection of alignement.
  

  for(int i=0;i<=index;i++){
    contourTransformed.push_back(vectContour.at(i));
  }
  
  for( int i=index+1; i< vectContour.size(); i++){
    Vector2D ptA = vectContour.at(i);
    Vector2D ptB = vectContour.at(i-1);
        
    int posASaved= i;
    int posDSaved;
    int j=i-2;
    while(j>0){
      Vector2D ptC = contourTransformed.at(j);
      Vector2D ptD = contourTransformed.at(j-1);
      posDSaved=j-1;
      Vector2D intersection;
      Vector2D vDC (ptC.x()-ptD.x(), ptC.y()-ptD.y()); 
      Vector2D vAB (ptB.x()-ptA.x(), ptB.y()-ptA.y()); 
      Vector2D vDCn =vDC; vDCn /= VectorUtils::norm(vDCn);
      Vector2D vABn =vAB; vABn  /= VectorUtils::norm(vABn);
      Vector2D vDA (ptA.x()-ptD.x(), ptA.y()-ptD.y()); 
      Vector2D vDB (ptB.x()-ptD.x(), ptB.y()-ptD.y()); 
      Vector2D vCA (ptA.x()-ptC.x(), ptA.y()-ptC.y()); 
       
       
      //CAS 1: intersection de A dans DC
      if(!removeLoop && (abs(VectorUtils::det(vDC,vCA))<=aPrecision) && (VectorUtils::norm(vCA)<=VectorUtils::norm(vDC)) &&
	 (VectorUtils::norm(vDA)<=VectorUtils::norm(vDC))){
	//cerr << "cas 1 : A in [CD] "<<ptA.x()<< " " << ptA.y() << endl;
	// Reconstitution of the contour
	vector<Vector2D> tmp;
	for(int m=0; m< vectContour.size(); m++){
	  if(m==posASaved){
	    tmp.push_back( Vector2D ((vectContour.at(posASaved)).x()+ epsilon*vABn.x(),
				     (vectContour.at(posASaved)).y()+ epsilon*vABn.y()));
	  }else{
	    tmp.push_back(vectContour.at(m));
	  }
	}
	loopFound=true;
	index= posASaved-1;
	return tmp;
      }
       
      //CAS 2: intersection de AB avec un des sommets (dans l'ordre =D)
      bool cas2=false;
      if(!removeLoop && abs(VectorUtils::det(vAB,vDA))<=aPrecision && VectorUtils::norm(vDA)<=VectorUtils::norm(vAB) &&
	 VectorUtils::norm(vDB)<=VectorUtils::norm(vAB)){
	//cerr << "cas 2 : D in [AB] "<<ptD.x()<< " " << ptD.y() << endl;
	cas2=true;
      }
       
      // CAS "NORMAL"
      if(computeSegmentIntersection(ptA, ptB, ptC, ptD, intersection) || cas2){
	 	 
	Vector2D sommetDansBoucle = contourTransformed.back();
	contourTransformed.pop_back();
	vector<Vector2D> listeDansBoucle; 
	 
	while (!((sommetDansBoucle.x()== ptD.x()) &&
		 (sommetDansBoucle.y()== ptD.y()))){
	  listeDansBoucle.push_back(sommetDansBoucle);
	  sommetDansBoucle = contourTransformed.back(); 
	  contourTransformed.pop_back();
	}
	contourTransformed.push_back(sommetDansBoucle);
	if (cas2){
	  intersection=ptD;
	}
	// transformation du point intersection en 4 points distincts suivant les directions.
	Vector2D ptDdec ( intersection.x()-vDCn.x()*epsilon, intersection.y()-vDCn.y()*epsilon);  
	Vector2D ptCdec ( intersection.x()+vDCn.x()*epsilon, intersection.y()+vDCn.y()*epsilon);  
	Vector2D ptAdec ( intersection.x()-vABn.x()*epsilon, intersection.y()-vABn.y()*epsilon);  
	Vector2D ptBdec ( intersection.x()+vABn.x()*epsilon, intersection.y()+vABn.y()*epsilon);  
	//---
	
	if(cas2){
	  contourTransformed.push_back(ptD);
	}else{	
	  contourTransformed.push_back(ptDdec);
	  contourTransformed.push_back(ptBdec);
	}
	
	for(int l=0; l< listeDansBoucle.size();l++){
	  contourTransformed.push_back(listeDansBoucle.at(l));
	}
	contourTransformed.push_back(ptCdec);
	contourTransformed.push_back(ptAdec);
	
	// Merge contourTransformed reste contour initial
	vector<Vector2D> tmp = contourTransformed;
	if(!removeLoop){
	  for(int m=posASaved; m< vectContour.size(); m++){
	    tmp.push_back(vectContour.at(m));
	  }
	}
	loopFound=true;
	index= posDSaved+1;
	return tmp;
      }
      j--;
    }
    
    contourTransformed.push_back(vectContour.at(i));        
  }  
  loopFound=false;
  return contourTransformed;
  
}






/**
 * Compute the bondind box of the contour.
 *
 *  @param contour 
 *  @param (return) ptMinXY the lower left point of the bounding box.
 *  @param (return) ptMaxXY the upper right point of the bounding box.
 **/


void
ImaGene::ContourHelper::computeContourBoundingBox(const vector<Vector2D> & contour, Vector2D &ptMinXY, Vector2D &ptMaxXY ){
  ptMaxXY.x()= contour.at(0).x();
  ptMaxXY.y()= contour.at(0).y();
  ptMinXY.x()= contour.at(0).x();
  ptMinXY.y()= contour.at(0).y();
  for(int i=1; i<contour.size(); i++){
    Vector2D p= contour.at(i);
    if(p.x() < ptMinXY.x())
      ptMinXY.x()=p.x();
    if(p.y() < ptMinXY.y())
      ptMinXY.y()=p.y();
    if(p.x() > ptMaxXY.x())
      ptMaxXY.x()=p.x();
    if(p.y() > ptMaxXY.y())
      ptMaxXY.y()=p.y();	
  }
}






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



vector<Vector2D>
ImaGene::ContourHelper::smoothContourFromNoise(const vector<Vector2D> & contour, const vector<DiskConstraint> &constraint,
					       const vector<uint> & constraintIndex, double epsilon, double coeffPtExt, 
					       double coeffPt, bool isOpen){
  vector <Vector2D> resu =contour; 
  bool hasChanged=true;
  uint nb=0;
  if(isOpen){
    cerr << "is open..." << isOpen<< endl;
  }
  cerr << "Iterating contour transform [ " ;
  while(hasChanged){
    cerr << ".";
    nb++;
    hasChanged=false;
    for(int i=0; i<resu.size(); i++){
      Vector2D p= resu.at(i);
      Vector2D ptMid;
      
      ptMid.x() = coeffPtExt*(resu.at((i-1+resu.size())%resu.size()).x());
      ptMid.y() = coeffPtExt*(resu.at((i-1+resu.size())%resu.size()).y());
      
      if(isOpen && (i+1==resu.size()))
	break;
      ptMid.x()+= coeffPtExt*(resu.at((i+1)%resu.size()).x());
      ptMid.y()+= coeffPtExt*(resu.at((i+1)%resu.size()).y());
      
      ptMid.x()+= p.x()*coeffPt;
      ptMid.y()+= p.y()*coeffPt;
      
      ptMid.x()/=(coeffPt+coeffPtExt*2.0);
      ptMid.y()/=(coeffPt+coeffPtExt*2.0);
      
      if(checkDiskConstaints(ptMid, i, constraint, constraintIndex)){
	if(abs(resu.at(i).x()-ptMid.x())>= epsilon || abs(resu.at(i).y()-ptMid.y())>=epsilon ){
	  hasChanged=true;
	  resu.at(i).x()=ptMid.x();
	  resu.at(i).y()=ptMid.y();
	}
      }
    }
  }
  cerr << " ]" << endl;
  return resu;
}





    
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


bool
ImaGene::ContourHelper::checkDiskConstaints(Vector2D pt, uint index, const vector<DiskConstraint> &constraint, 
					    const vector<uint> &constraintIndex){
  int posConstraint = constraintIndex.at(index);
  int posConstraintMin = constraintIndex.at((index+constraintIndex.size()-1)%constraintIndex.size());
  int posConstraintMax = constraintIndex.at((index+1)%constraintIndex.size())%constraintIndex.size();
  for(int k=posConstraintMin; ((k-1+constraint.size())%constraint.size())!=posConstraintMax; k++){
  
  DiskConstraint cons= constraint.at(k%constraint.size());
  Vector2D vPtC(cons.center.x()-pt.x(), cons.center.y()-pt.y());
  double norm = VectorUtils::norm(vPtC);
  if(norm<=cons.radius)
    return true;
  }  
  
    return false;
}






vector<vector<Vector2D> >
ImaGene::ContourHelper::filterMeaningfulParts(vector<Vector2D> contour, const vector<DiskConstraint> &constraint,
					      const vector<uint> &constraintIndex, double threshold, 
					      const vector<double> &slopeConstraints, double slopeThresholdMin, 
					      double slopeThresholdMax){
  
  vector<bool> vectValid;
  for(uint i=0; i< contour.size(); i++){
    vectValid.push_back(true);
  }  
  for(uint i=0; i<contour.size(); i++){
    DiskConstraint iCons = constraint.at(constraintIndex.at(i)); 
    double constraintRadius = iCons.radius;
    if(constraintRadius>threshold){
      vectValid.at(i)=false;
      //scan backward to remove all consecutive points included in the disk
      bool scan=true;
      uint k=1;
      while(scan && k<contour.size() ){
	Vector2D ptBack = contour.at((i-k+contour.size())%contour.size());
	Vector2D ptBackPt(iCons.center.x()-ptBack.x(), iCons.center.y()-ptBack.y());
	double dist = VectorUtils::norm(ptBackPt);
	if(dist<iCons.radius){
	  vectValid.at((i-k+contour.size())%contour.size())=false;
       	}else{
	  scan=false;
	}
	k++;
      }
      //scan forward to remove all consecutive points included in the disk
      k=1;
      scan=true;
      while(scan && k<contour.size() ){
	Vector2D ptFront = contour.at((i+k)%contour.size());
	Vector2D ptFrontPt(iCons.center.x()-ptFront.x(), iCons.center.y()-ptFront.y());
	double dist = VectorUtils::norm(ptFrontPt);
	if(dist<iCons.radius){
	  vectValid.at((i+k)%contour.size())=false;
       	}else{
	  scan=false;
	}
      k++;
      }
      
    }
  }
  
  bool slopeConstrained = slopeConstraints.size()!=0;
  //Construct the resulting set of contours
  vector<vector<Vector2D> > vectContours;
  vector<Vector2D> currentContour;
  for(int i=0;i<contour.size();i++){
    if(vectValid.at(i)&& (!slopeConstrained ||((slopeConstraints.at(i)<= slopeThresholdMax) &&
					       (slopeConstraints.at(i)>= slopeThresholdMin))) ){
      currentContour.push_back(contour.at(i));
    }else{
      if(currentContour.size()!=0){
	vectContours.push_back(currentContour);
	currentContour.clear();
      }
    }
  }
  if(currentContour.size()!=0){
    vectContours.push_back(currentContour);
  }

  
  return vectContours;
  
}






/**
 *  Compute the Minimum Euclidean Spanning Tree from a set of
 *  unordered point by using the Kruskal algorithm
 * 
 *  @param pointSet: the source of unordered point set.
 *  @return the Minimum Euclidean Spanning Tree
 *
 **/


ImaGene::ContourHelper::ValuedEdgeSet 
ImaGene::ContourHelper::getMinEuclideanSpanningTree(const std::vector<ImaGene::Vector2D> &pointSet){
  vector<uint> vectParent;
  ImaGene::ContourHelper::ValuedEdgeSet graphSRC, graphResult;
  // creating all the edges of the initial graph
  for(int i=0; i<pointSet.size(); i++){
    Vector2D ptA = pointSet.at(i);     
    for(int j=i+1; j<pointSet.size();j++){
      Vector2D ptB = pointSet.at(j);
      double weightAB = VectorUtils::norm(Vector2D(ptB.x()-ptA.x(), ptB.y()-ptA.y()));
      Edge e (i,j);
      pair<double, Edge> weightedPair(weightAB, e);
      graphSRC.push_back(weightedPair);
    }
  }
  //Initialisation of the parent for each vertex
  for(int i=0; i<pointSet.size();i++){
    vectParent.push_back(i);
  }
  
  std::sort(graphSRC.begin(), graphSRC.end());
  for(int i=0; i<graphSRC.size(); i++){
    uint rootA = getSetParent(vectParent, graphSRC.at(i).second.first);
    uint rootB = getSetParent(vectParent, graphSRC.at(i).second.second);
    if(rootA!=rootB){
      graphResult.push_back(graphSRC.at(i));
      vectParent.at(rootA)=vectParent.at(rootB);
    }
  }
  return graphResult;
}










/**
 * Return the vector containing the vertex index of the
 * longestPath contained in the tree passing through the root.
 *  
 * @param graph: the initial tree represented by its set of vertex
 * @param nbVertex: the number of the vertex set of the tree.
 * @return the index associated to the point longest path passing through the root. 
 **/

vector<uint>
ImaGene::ContourHelper::getLongestPathFromTree(const ValuedEdgeSet &graph, uint nbVertex ){  
  vector<uint> vertexSet = getLeaf(graph, nbVertex);
  vector<double> weightToParent;
  vector<uint> parentSet = getParent(graph, nbVertex, weightToParent);
  vector<uint> degree = getVertexDegree(graph,  nbVertex ); 
  vector<uint> nbMarked;
  vector<vector<uint> > vectLongestPathRoot;
  // give for each vertex the current weight of the actual maximal path
  vector<double> vertexPathWeight;
  
  // used to store for each vertex their longest path.
  vector<vector<uint> > vectLongestPath;
  // initialisation by single vertex for the initial leaf.
  for(int i=0; i<nbVertex; i++){
    vector<uint> v;
    vectLongestPath.push_back(v);
    vertexPathWeight.push_back(0.0);
    nbMarked.push_back(0);
  }
 
  for(int i=0; i<vertexSet.size(); i++){
    vectLongestPath.at(vertexSet.at(i)).push_back(vertexSet.at(i));
  }
  
  
  vector<uint> newVertexSet;
  uint lastVertex=0;
  uint prevLastVertex=0;
  uint root=0;
  do{
    newVertexSet.clear();
    for(int i=0; i< vertexSet.size(); i++){
      uint parent = parentSet.at(vertexSet.at(i));
      if(vectLongestPath.at(parent).size()==0 || (vertexPathWeight[vertexSet[i]]+weightToParent[vertexSet[i]]>vertexPathWeight[parent])){
	vertexPathWeight[parent]=vertexPathWeight[vertexSet[i]]+weightToParent[vertexSet[i]];
	vector<uint> newMaxPath= vectLongestPath.at(vertexSet[i]);      
	if(parentSet[parent]!=parent){
	  newMaxPath.push_back(parent);
	  vectLongestPath.at(parent)=newMaxPath;
	}else{
       	  vectLongestPathRoot.push_back(newMaxPath);
	  root=parent;
	}
      }
      nbMarked[parent]++;
      if(nbMarked[parent]+1==degree[parent] && parentSet[parent]!=parent){
	newVertexSet.push_back(parent);
      }
      prevLastVertex = lastVertex;
      lastVertex=vertexSet[i];      
    }
    vertexSet=newVertexSet;
    
  }while(vertexSet.size()>0);
  
  
  vector<uint> part1;
  vector<uint> part2;
  
  //cas peu probable 
  if(vectLongestPathRoot.size()==1){
    return vectLongestPathRoot.at(0); 
  }else{
    part1 = vectLongestPathRoot.at(vectLongestPathRoot.size()-1); 
    part2 = vectLongestPathRoot.at(vectLongestPathRoot.size()-2); 
    // Récupère le second plus long chemin.
    if(vectLongestPathRoot.size()>2){
      for(int i=2; i<vectLongestPathRoot.size(); i++){
	vector<uint> part = vectLongestPathRoot.at(vectLongestPathRoot.size()-1-i); 
	if(part.size()!=0 && part2.size()!=0){
	  if(vertexPathWeight[part.at(part.size()-1)] > vertexPathWeight[part2.at(part2.size()-1)]){
	    part2=part;
	  }
	}
      }
    }
  }
  
  vector<uint> mergedParts;

  for(int i=0; i<part1.size(); i++){
    mergedParts.push_back(part1.at(i));
  }
  mergedParts.push_back(root);
  for(int i=0; i<part2.size(); i++){
    mergedParts.push_back(part2.at(part2.size()-i-1));
  }
  return mergedParts ;
}










uint 
ImaGene::ContourHelper::getSetParent(const std::vector<uint> &vectParent, uint vertexId){
  while(vectParent[vertexId]!=vertexId){
    vertexId = vectParent[vertexId];
  }
  return vertexId;
}






vector<uint> 
ImaGene::ContourHelper::getParent(const ValuedEdgeSet &graph, uint nbVertex, vector<double> &weightToParent){
  vector<uint> vectParent;
  vector<bool> markedEdge;
  weightToParent.clear();
  for(uint i=0; i<graph.size(); i++){
    markedEdge.push_back(false);
  }
  vector<uint> vertexDegree = getVertexDegree(graph, nbVertex);
  for(uint i=0; i<nbVertex; i++){
    vectParent.push_back(i);
    weightToParent.push_back(0.0);
  }
  
  bool hasChanged=true;
  while(hasChanged){
    hasChanged=false;
    for(uint i=0; i< graph.size();i++){
      if(!markedEdge[i] && vertexDegree[graph.at(i).second.first]==1 ){
	markedEdge[i]=true;
	hasChanged=true;
       	vertexDegree[graph.at(i).second.first]--;
	vertexDegree[graph.at(i).second.second]--;
	vectParent[graph.at(i).second.first]=graph.at(i).second.second;
	weightToParent[graph.at(i).second.first]=graph.at(i).first;
      }else if(!markedEdge[i] && vertexDegree[graph.at(i).second.second]==1 ){
	hasChanged=true;
	markedEdge[i]=true;
	vertexDegree[graph.at(i).second.first]--;
	vertexDegree[graph.at(i).second.second]--;
	vectParent[graph.at(i).second.second]=graph.at(i).second.first;
	weightToParent[graph.at(i).second.second]=graph.at(i).first;
      }
    }
  }
  return vectParent;
}






vector<uint> 
ImaGene::ContourHelper::getVertexDegree(const ValuedEdgeSet &graph, uint nbVertex ){
  vector<uint> vertexDegree;
  for(uint i=0; i<nbVertex; i++){
    vertexDegree.push_back(0);
  }
  for(uint i=0; i< graph.size();i++){
    vertexDegree[graph[i].second.first]++;  
    vertexDegree[graph[i].second.second]++;  
  }
  
  return vertexDegree;
}



vector<uint> 
ImaGene::ContourHelper::getLeaf(const ValuedEdgeSet &graph, uint nbVertex ){
  vector<uint> leafSet;
  vector<uint> vertexDegree = getVertexDegree(graph, nbVertex); 
  for(int i=0; i< nbVertex; i++){
    if(vertexDegree[i]==1){
      leafSet.push_back(i);
    }
  }
  return leafSet;
}


  
  













///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::ContourHelper::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[ContourHelper]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::ContourHelper::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////


