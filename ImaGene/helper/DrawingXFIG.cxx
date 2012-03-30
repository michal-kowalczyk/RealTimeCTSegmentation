#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "ImaGene/helper/DrawingXFIG.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"



using namespace std;
using namespace ImaGene;
 

static const int RESOLUTION = 3200;
static const int SCALE = 50;
static int fillIntensity = 20;




/**
 *   Include header 
 */  

void 
DrawingXFIG::includeXFIGHeader(ostream &os, int resolution, int scaleEPS){

  os << "#FIG 3.2 \nLandscape \nCenter \nInches \nLetter  \n" <<scaleEPS << "\nSingle \n1" 
     << " \n"<< resolution <<" 1" << endl;

}

void 
DrawingXFIG::setFillIntensity(int val){
  fillIntensity=val;
}

void 
DrawingXFIG::drawContour(ostream &os, const vector<Vector2D> & vectPoints, int color, int fillColor, int epaisseurTrait, 
				bool closeCnt,bool filled, int depth, double zoom, double x0, double y0){
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);

  // param:  2 (polyline) 1(polyline 3polygone) linestyle thickness pen_color fill_color depth	 
  os <<setiosflags(ios_base::fixed);
  os<< setprecision(0);

  int tailleVectPoint = vectPoints.size();
  os << "\n 2 " << ((closeCnt)? 3 : 1 )<< " 0 "  << epaisseurTrait << " " << color  << 
    " " << fillColor<<  " " << depth << " -1 "<<((filled)?20:-1)  <<" 0.000 0 0 -1 0 0 "
     << tailleVectPoint + ((closeCnt)?1:0)   << " "  <<endl;  
  for (int i=0; i<tailleVectPoint; i++ ){  
    Vector2D pt= vectPoints.at(i);
    os << pt.x()*SCALE*zoom+x0*SCALE << " " << pt.y()*SCALE*zoom+y0*SCALE << " " ;        
  }
  // Selon la doc il faut que le point N = point 0
  if(closeCnt){
    Vector2D pt= vectPoints.at(0);
    os << pt.x()*SCALE*zoom+x0*SCALE << " " << pt.y()*SCALE*zoom+y0*SCALE << " " ;        
  }

  os << endl;
}






void 
DrawingXFIG::drawContour(ostream &os, const vector<Vector2i> & vectPoints, int color, int fillColor, int epaisseurTrait, 
				bool closeCnt,bool filled, int depth, double zoom, double x0, double y0){
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);

  // param:  2 (polyline) 1(polyline 3polygone) linestyle thickness pen_color fill_color depth	 
  os <<setiosflags(ios_base::fixed);
  os<< setprecision(0);

  int tailleVectPoint = vectPoints.size();
  os << "\n 2 " << ((closeCnt)? 3 : 1 )<< " 0 "  << epaisseurTrait << " " << color  << 
    " " << fillColor<<  " " << depth << " -1 "<<((filled)?20:-1)  <<" 0.000 0 0 -1 0 0 "
     << tailleVectPoint + ((closeCnt)?1:0)   << " "  <<endl;  
  for (int i=0; i<tailleVectPoint; i++ ){  
    Vector2i pt= vectPoints.at(i);
    os << pt.x()*SCALE*zoom+x0*SCALE << " " << pt.y()*SCALE*zoom+y0*SCALE << " " ;        
  }
  // Selon la doc il faut que le point N = point 0
  if(closeCnt){
    Vector2i pt= vectPoints.at(0);
    os << pt.x()*SCALE*zoom+x0*SCALE << " " << pt.y()*SCALE*zoom+y0*SCALE << " " ;        
  }

  os << endl;
}





void 
DrawingXFIG::drawContourPixels(ostream &os, const FreemanChain & fc, int color, double zoom, int x0, int y0, int depth){
  for (FreemanChain::const_iterator it = fc.begin();it != fc.end(); ++it ){  
    Vector2i pt (((*it).x())*zoom+x0, ((*it).y())*zoom+y0);
    drawPixel(os, pt, 0,color, zoom, depth);
  }
}


void 
DrawingXFIG::drawContour(ostream &os, const FreemanChain & fc, int color, uint linewidth, int x0, int y0, int depth){
  vector<Vector2i> vectToaff;
  for (FreemanChain::const_iterator it = fc.begin();it != fc.end(); ++it ){  
    Vector2i pt (((*it).x()), ((*it).y()));
    vectToaff.push_back(pt);    
  }
  drawContour(os, vectToaff, color, color, linewidth, fc.isClosed(), false, depth, 1.0, x0, y0);
}



void 
DrawingXFIG::drawContourPixelsCorner(ostream &os, const FreemanChain & fc, int color, double zoom, int x0, int y0, int depth){

  for (FreemanChain::const_iterator it = fc.begin();it != fc.end(); ++it ){  
    Vector2i pt (((*it).x())*zoom+x0, ((*it).y())*zoom+y0);
    drawPixelCorner(os, pt, 0,color, zoom, depth);
  }
}







void 
DrawingXFIG::drawContourPixels(ostream & os, const vector<Vector2i> &vectContour, int color,
				      double zoom, int x0, int y0, int depth){
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);

  for (int i=0; i<vectContour.size(); i++ ){  
    drawPixel(os, vectContour.at(i), 0,color, zoom, depth);
  }
}












void 
DrawingXFIG::drawPixel(ostream &os, const Vector2i &pixel, int color, int colorFill, double taille,  int depth){
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);
  
  Vector2D p0 (pixel.x()+taille/2.0, pixel.y()+taille/2.0);
  Vector2D p1 (pixel.x()+taille/2.0, pixel.y()-taille/2.0);
  Vector2D p2 (pixel.x()-taille/2.0, pixel.y()-taille/2.0);
  Vector2D p3 (pixel.x()-taille/2.0, pixel.y()+taille/2.0);
  
  os << "\n 2 3 0 1 " << color  << " " << colorFill << 
    " " << depth<<    " -1 "<< fillIntensity << " 0.000 0 0 -1 0 0 5 " <<endl ;  
  
  os << p0.x()*SCALE << " " << p0.y()*SCALE << " " << p1.x()*SCALE << " " << p1.y()*SCALE << " " << p2.x()*SCALE << " " << p2.y()*SCALE << " "<< 
    p3.x()*SCALE << " " << p3.y()*SCALE << " "  << p0.x()*SCALE << " " << p0.y()*SCALE << endl;
   
  os << endl;
}

void 
DrawingXFIG::drawPixel(ostream &os, const Vector2D &pixel, int color, int colorFill, double taille,  int depth){
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);
  
  Vector2D p0 (pixel.x()+taille/2.0, pixel.y()+taille/2.0);
  Vector2D p1 (pixel.x()+taille/2.0, pixel.y()-taille/2.0);
  Vector2D p2 (pixel.x()-taille/2.0, pixel.y()-taille/2.0);
  Vector2D p3 (pixel.x()-taille/2.0, pixel.y()+taille/2.0);
  
  os << "\n 2 3 0 1 " << color  << " " << colorFill << 
    " " << depth<<    " -1 "<< fillIntensity << " 0.000 0 0 -1 0 0 5 " <<endl ;  
  
  os << p0.x()*SCALE << " " << p0.y()*SCALE << " " << p1.x()*SCALE << " " << p1.y()*SCALE << " " << p2.x()*SCALE << " " << p2.y()*SCALE << " "<< 
    p3.x()*SCALE << " " << p3.y()*SCALE << " "  << p0.x()*SCALE << " " << p0.y()*SCALE << endl;
   
  os << endl;
}












void  
DrawingXFIG::drawImage(ostream & os, string nomImage, int width, int height, int depth){
os <<setiosflags(ios_base::fixed);
  os << setprecision(0);

  os << "2 5 0 1 0 -1 " << depth << " -1 -1 0.000 0 0 -1 0 0 5"<< endl<< " 0 " <<nomImage << endl << " 0 0 " << width*SCALE  << " 0 " << width*SCALE << " " << height*SCALE << " 0 " << height*SCALE << " 0 0 " << endl ; 
  os << endl;
}


void  
DrawingXFIG::drawImage(ostream & os, string nomImage, int x0, int y0, int width, int height, int depth){
os <<setiosflags(ios_base::fixed);
  os << setprecision(0);

  os << "2 5 0 1 0 -1 " << depth << " -1 -1 0.000 0 0 -1 0 0 5"<< endl<< " 0 " <<nomImage << endl << x0*SCALE << " "<< y0*SCALE << " " << x0*SCALE + width*SCALE  << " " << y0*SCALE <<" " << x0*SCALE+width*SCALE << " " << y0*SCALE+height*SCALE << " " << x0*SCALE << " "  << y0*SCALE+height*SCALE << " " << x0*SCALE << " "<< y0*SCALE << " "  << endl ; 
  os << endl;
}



/**
 * 
 **/


void
DrawingXFIG::drawCross(ostream &os, const Vector2i &point, int color, int largeur, int linewidth, int depth){ 
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);
  os << "\n 2 1 0 " << linewidth << " "  <<color << " 7 " << depth <<" -1 -1 0.000 0 0 -1 0 0 2"<<endl;
  os << (int)((point.x()-largeur/2.0)*SCALE) << " " << (int)((point.y()-largeur/2.0)*SCALE) << " " 
     << (int)((point.x()+largeur/2.0)*SCALE) << " " << (int)((point.y()+largeur/2.0)*SCALE) << endl; 
  os << "\n 2 1 0 "<< linewidth <<  " "  <<color << " 7 " << depth << " -1 -1 0.000 0 0 -1 0 0 2"<<endl;
  os << (int)((point.x()+largeur/2.0)*SCALE) << " " << (int)((point.y()-largeur/2.0)*SCALE) << " " 
     << (int)((point.x()-largeur/2.0)*SCALE) << " " << (int)((point.y()+largeur/2.0)*SCALE) << endl;   
}

/**
 * 
 **/


void
DrawingXFIG::drawCross(ostream &os, const Vector2D &point, int color, int largeur, int linewidth, int depth){ 
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);
  os << "\n 2 1 0 " << linewidth << " "  <<color << " 7 " << depth <<" -1 -1 0.000 0 0 -1 0 0 2"<<endl;
  os << (int)((point.x()-largeur/2.0)*SCALE) << " " << (int)((point.y()-largeur/2.0)*SCALE) << " " 
     << (int)((point.x()+largeur/2.0)*SCALE) << " " << (int)((point.y()+largeur/2.0)*SCALE) << endl; 
  os << "\n 2 1 0 "<< linewidth <<  " "  <<color << " 7 " << depth << " -1 -1 0.000 0 0 -1 0 0 2"<<endl;
  os << (int)((point.x()+largeur/2.0)*SCALE) << " " << (int)((point.y()-largeur/2.0)*SCALE) << " " 
     << (int)((point.x()-largeur/2.0)*SCALE) << " " << (int)((point.y()+largeur/2.0)*SCALE) << endl;   
}





void 
DrawingXFIG::drawLine(ostream &os, const Vector2i &point1, const Vector2i &point2,int color, int linewidth, int depth){ 
   os <<setiosflags(ios_base::fixed);
   os << setprecision(0);
   os << "\n 2 1 0 " << linewidth << " " <<color << " 7 " << depth <<" -1 -1 0.000 0 0 -1 0 0 2"<<endl;
   os << (point1.x())*SCALE << " " << (point1.y())*SCALE << " " 
	<< (point2.x())*SCALE << " " << (point2.y())*SCALE << endl; 
}


void 
DrawingXFIG::drawLine(ostream &os, const Vector2D &point1, const Vector2D &point2,int color, int linewidth, int depth){ 
   os <<setiosflags(ios_base::fixed);
   os << setprecision(0);
   os << "\n 2 1 0 " << linewidth << " " <<color << " 7 " << depth <<" -1 -1 0.000 0 0 -1 0 0 2"<<endl;
   os << (point1.x())*SCALE << " " << (point1.y())*SCALE << " " 
	<< (point2.x())*SCALE << " " << (point2.y())*SCALE << endl; 
}



void 
DrawingXFIG::drawLineVector(ostream &os, const Vector2D &point1, const Vector2D &point2,int color, int linewidth, int depth){ 
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);                                                                 //     arrow type=0, arrow style=1, arrowThickness=2*linewidth
  os << "\n 2 1 0 " << linewidth << " " <<color << " 7 " << depth <<" -1 -1 0.000 0 0 -1 1 0 2 "<< endl;
  os << "  1 0 1.00 20.00 20.00 " <<endl;
   os << (point1.x())*SCALE << " " << (point1.y())*SCALE << " " 
	<< (point2.x())*SCALE << " " << (point2.y())*SCALE << endl; 
}





void
DrawingXFIG::drawCircle(ostream &os, const Vector2i &point, int color, double radius, int depth, bool filled, int linewidth){ 
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);
  int taille = (int)(SCALE*radius);
  os << "1 3 0 "  << linewidth << " " <<color << " " << color <<" " << depth<< " -1 "<< ((filled)? "20" : "-1")  << " 0.000 1 0.0000 "<< point.x()*SCALE << " "<< point.y()*SCALE 
     << " " << taille << " " << taille << " " << point.x()*SCALE << " " << point.y()*SCALE << " " << 
    point.x()*SCALE << " "  <<point.y()*SCALE << endl;
}


void
DrawingXFIG::drawCircle(ostream &os, const Vector2D &point, int color, double radius, int depth, bool filled, int linewidth){ 
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);
  int taille = (int)(SCALE*radius);
  os << "1 3 0 "  << linewidth << " " <<color << " " << color <<" " << depth<< " -1 "<< ((filled)? "20" : "-1") << " 0.000 1 0.0000 "<< point.x()*SCALE << " "<< point.y()*SCALE 
     << " " << taille << " " << taille << " " << (int) (point.x()*SCALE) << " " << (int) (point.y()*SCALE) << " " << 
    point.x()*SCALE << " "  <<point.y()*SCALE << endl;
}




void
DrawingXFIG::drawCircles(ostream &os, const vector<Vector2i> &vectPoint, int color, double radius, int depth, bool filled, int linewidth ){ 
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);
  uint tailleVect = vectPoint.size();
  int taille = (int)(SCALE*radius);
  for(int i=0; i < tailleVect; i++){
    Vector2i p = vectPoint.at(i);
    drawCircle(os, p, color, radius, depth, filled ,linewidth);
  }
}








void 
DrawingXFIG::drawContourFromPointsSet(ostream &os, const vector<Vector2i> &vectPoints, const vector<Vector2i> &vectContour,
				      int color, int fillColor, int epaisseurTrait, bool closeCnt,bool filled, int depth, 
				      int zoom, int x0, int y0){
  // Creation des index des points 
  vector<PointAndIndex> vectPAI;
  for(uint i=0; i<vectPoints.size(); i++){
    Vector2i pt  = vectPoints.at(i); 
    int index = getIndexOfPoint(vectContour, pt );
    PointAndIndex pti;
    pti.index= index;
    pti.point = pt;
    vectPAI.push_back(pti);
  }

  vector<Vector2i> vectToAff;
  sort(vectPAI.begin(), vectPAI.end(), compPtAndIndex);
  vector<PointAndIndex>::iterator iter=vectPAI.begin();
  while(iter!= vectPAI.end()){
    vectToAff.push_back((*iter).point);
    //    drawCircle(cout, (*iter).point, 4, 1, -1);
    iter++;
  }
  drawContour(os, vectToAff, color, fillColor, epaisseurTrait, closeCnt, filled, depth, zoom, x0, y0);
  
}








int 
DrawingXFIG::getIndexOfPoint(const vector<Vector2i> &contour, const Vector2i & point ){
  int position =0;
  int tailleContour = contour.size();
  while (position < tailleContour){
    Vector2i pt = contour.at(position);
    if(pt==point){
      return position;      
    }    
    position++;
  }
  return -1;    
}











bool 
DrawingXFIG::compPtAndIndex(const PointAndIndex  &pt1 , const PointAndIndex  &pt2 ){
  return pt1.index< pt2.index;
}











vector<Vector2i> 
DrawingXFIG::getSnowFlakeContour(const vector<Vector2D> &contour, int level){

  vector<Vector2D> vResult =contour;
  
  for (int l =0; l<level; l++){
    vector<Vector2D> contourI = vResult; 
    int taille = contourI.size();
    vector<Vector2D> v;
    vResult=v;
      for(int i=0; i<taille; i++){
      Vector2D ptCourant = contourI.at(i);
      Vector2D ptSuivant = contourI.at((i+1)%taille);
      vResult.push_back(ptCourant);
      Vector2D ptCentre = Vector2D((ptCourant.x()+ptSuivant.x())/2.0,(ptCourant.y()+ptSuivant.y())/2.0);
      Vector2D pt1 = Vector2D((2.0*ptCourant.x()+1.0*ptSuivant.x())/3.0,(2.0*ptCourant.y()+1.0*ptSuivant.y())/3.0);
      Vector2D pt3 = Vector2D((1.0*ptCourant.x()+2.0*ptSuivant.x())/3.0,(1.0*ptCourant.y()+2.0*ptSuivant.y())/3.0);
      double d = sqrt((pt3.x()-pt1.x())*(pt3.x()-pt1.x())+(pt3.y()-pt1.y())*(pt3.y()-pt1.y())); 
      double d2 = (pt3.x()-pt1.x())*(pt3.x()-pt1.x())+(pt3.y()-pt1.y())*(pt3.y()-pt1.y()); 
      vResult.push_back(pt1);
    
      if(ptCourant.y()== ptSuivant.y()){
	double x =ptCentre.x();
	double y =sqrt(d2-((d/2.0)*(d/2.0)));
	if(ptCourant.x()<ptSuivant.x())
	  vResult.push_back(Vector2D(x,ptCourant.y()-y));
	else
	  vResult.push_back(Vector2D(x,ptCourant.y()+y));
      
      }else if(ptCourant.x()== ptSuivant.x()){
	double y =(pt1.y()+pt3.y())/2.0;
	double x =sqrt(d*d-((d/2.0)*(d/2.0)));
	if(ptCourant.y()<ptSuivant.y())
	  vResult.push_back(Vector2D(ptCourant.x()-x,y));
	else
	  vResult.push_back(Vector2D(ptCourant.x()+x,y));
      
      
      }else{
 
	double a = (ptCourant.y() - ptSuivant.y())/(ptCourant.x() - ptSuivant.x());
	//double b = ptCourant.y()-a*ptCourant.x();
	double ap = -1.0/a;
	double bp = ptCentre.y()-ap*ptCentre.x();
      
      
	double x1 = (pt1.x() + ap*(pt1.y() - bp) - 
		     sqrt(-pow(pt1.x()*ap - pt1.y() + bp,2) + (1 + pow(ap,2))*pow(d,2)))/
	  (1.0 + pow(ap,2));
      
	double y1 =(bp + ap*(pt1.x() + ap*pt1.y() - 
			     sqrt(-pow(pt1.x()*ap - pt1.y() + bp,2) + 
				  (1.0 + pow(ap,2))*pow(d,2))))/(1.0 + pow(ap,2));
      
      

	double x2= (pt1.x() + ap*(pt1.y() - bp) + 
		    sqrt(-pow(pt1.x()*ap - pt1.y() + bp,2) + (1 + pow(ap,2))*pow(d,2)))/
	  (1 + pow(ap,2));

	double y2 =(bp + ap*(pt1.x() + ap*pt1.y() + sqrt(-pow(pt1.x()*ap - pt1.y() + bp,2) + 
							 (1 + pow(ap,2))*pow(d,2))))/(1 + pow(ap,2));

	Vector2D vdep = Vector2D(pt3.x()-pt1.x(),pt3.y()-pt1.y());
	Vector2D vpoint = Vector2D(x1-pt1.x(),y1-pt1.y());
      
	if(vdep.x()*vpoint.y()-vdep.y()*vpoint.x()<0){
	  vResult.push_back(Vector2D(x1,y1));
	}else{
	  vResult.push_back(Vector2D(x2,y2));
	}

      }
    
    
      vResult.push_back(pt3);
      
      
    }
  }

  vector<Vector2i> vResultInt;
  for (int i=0; i< vResult.size(); i++){
    Vector2D pi = vResult.at(i);
    vResultInt.push_back(Vector2i((int)pi.x(), (int)pi.y()));    
  }
  

  return vResultInt;
}













void 
DrawingXFIG::drawPixelCorner(ostream &os, const Vector2i &pixel, int color, int colorFill, double taille, int depth, bool filled){
  os <<setiosflags(ios_base::fixed);
  os << setprecision(0);
  
  Vector2D p0 (pixel.x(), pixel.y());
  Vector2D p1 (pixel.x()+taille, pixel.y());
  Vector2D p2 (pixel.x()+taille, pixel.y()+taille);
  Vector2D p3 (pixel.x(), pixel.y()+taille);
  
  
  os << "\n 2 3 0 1 " << color  << " " << colorFill << 
    " " << depth<<    " -1 " << (filled? fillIntensity:-1) << " 0.000 0 0 -1 0 0 5 " <<endl ;  
  
  
  os << p0.x()*SCALE << " " << p0.y()*SCALE << " " << p1.x()*SCALE << " " << p1.y()*SCALE << " " << p2.x()*SCALE << " " << p2.y()*SCALE << " "<< 
    p3.x()*SCALE << " " << p3.y()*SCALE << " "  << p0.x()*SCALE << " " << p0.y()*SCALE << endl;
   
  os << endl;
}









void 
DrawingXFIG::drawDSS(ostream &os , 
		     const C4CSegment & dss, 
		     const Frame2D & frame,
		     int color,
		     double agrandissement,
		     double decalX,
		     double decalY,
		     int linewidth,
		     uint ms_mode = 0,
		     double ms_coef_mode = 0.0 ){
  
  Vector2i u( frame.transformPoint( dss.u() ) );
  Vector2i up( frame.transformPoint( dss.up() ) );
  Vector2i l( frame.transformPoint( dss.l() ) );
  Vector2i lp( frame.transformPoint( dss.lp() ) );
  


  if ( ms_mode & 32 )
    {
      DrawingXFIG::drawCircle (os, u,  color, 1, 30);
      DrawingXFIG::drawCircle (os, up,  color, 1, 30);
    }
  if ( ms_mode & 16 )
    {
      DrawingXFIG::drawCircle (os, l,  color, 1, 30);
      DrawingXFIG::drawCircle (os, lp,  color, 1, 30);
    }
 
  Vector2i c( frame.transformPoint( dss.c_n() ) );
  Vector2i cp( frame.transformPoint( dss.cp_n() ) );
  if ( ms_mode & 64 )
    {
      DrawingXFIG::drawCircle (os, c,  color, 1, 30);
    }
  Vector2D c1 = frame.transformPoint( dss.project( dss.c_n(), dss.u() ) );
  Vector2D c2 = frame.transformPoint( dss.project( dss.c_n(), dss.l() ) );
  Vector2D cp1 = frame.transformPoint( dss.project( dss.cp_n(), dss.u() ) );
  Vector2D cp2 = frame.transformPoint( dss.project( dss.cp_n(), dss.l() ) );
  vector<Vector2D> xy( 5 );

  
  xy[ 0 ] = xy[ 4 ] = Vector2D(c1.x(),c1.y());
  xy[1] = Vector2D(c2.x(), c2.y());
  xy[2] = Vector2D(cp2.x(), cp2.y());
  xy[3] = Vector2D(cp1.x(), cp1.y());
  

  vector<uint> z;
  Mathutils::cfrac( z, dss.a() < 0 ? -dss.a() : dss.a(), dss.b() );
  
  int line_style = 0; // solid
  if ( ms_mode & 4 )
    line_style = ( z.size() % 2 ) == 0 
      ? 0 // even
      : 1; // odd

  int thickness = ( ms_mode % 8 ) != 0 ? 2 : 1;
  //filter.outputPolylineInView( x, y, color, thickness, line_style, 45 );
  DrawingXFIG::drawContour(cout, xy, color, color, linewidth, true, false, 30, agrandissement, decalX, decalY);
  

  
//   ostringstream out_str;
//   if ( ms_mode & 1 )
//     out_str << "(" << dss.a() << "," << dss.b() << ";" << dss.mu() << ")"; 
//   if ( ms_mode & 2 )
//     {
//       out_str << "[" << z[ 0 ]; 
//       for ( uint i = 1; i < z.size(); ++i )
// 	out_str << "," << z[ i ];
//       out_str << "]";
//     }
//   // cerr << out_str.str() << endl;
//   if ( out_str.str() != "" )
//     {
//       float angle = atan2( (double) dss.a(), (double) dss.b() );
//       double xbase = ( c1.x() + cp1.x() ) / 2.0;
//       double ybase = ( c1.y() + cp1.y() ) / 2.0;
//       xbase += ms_coef_mode * ( c2.x() - c1.x() );
//       ybase += ms_coef_mode * ( c2.y() - c1.y() );

//       filter.outputText( xbase, ybase,
// 			 out_str.str(),
// 			 -frame.angleToX( angle ),
// 			 1, 0, 12, color, 20 );
//     }

}





void 
ImaGene::DrawingXFIG::drawBlurredSegment(ostream &os, const ImaGene::BlurredSegmentTgtCover::Segment segment,
					 uint color, uint linewidth, uint depth){

  vector<Vector2D> xy;
  xy.push_back(segment.ptALongestSegment1);
  xy.push_back(segment.ptBLongestSegment1);
  xy.push_back(segment.ptCLongestSegment2);
  xy.push_back(segment.ptDLongestSegment2);
  
DrawingXFIG::drawContour(os, xy, color, color, linewidth, true, false, depth, 1.0, 0.0, 0.0);
  
  
}

  



