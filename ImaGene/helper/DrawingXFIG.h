#ifndef DRAWINGXFIG_H
#define DRAWINGXFIG_H



#include <vector>
#include <stdlib.h>
#include <iostream>
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.h"
#include "ImaGene/dgeometry2d/BlurredSegmentTgtCover.h"

#include "ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnShapes.h"
#include "ImaGene/digitalnD/KnSpaceScanner.h"
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
#include "ImaGene/digitalnD/ObjectBoundary.h"
#include "ImaGene/digitalnD/Frame2D.h"


#include "ImaGene/helper/C4CTangentialCoverGeometry.h"
#include "ImaGene/helper/GlobalC4CGeometry.h"
#include "ImaGene/helper/ContourHelper.h"
#include "ImaGene/helper/ShapeHelper.h"





namespace ImaGene {


class DrawingXFIG{


  
  
public:


  /**
   *   Include header 
   */  
 
  
  static void includeXFIGHeader(ostream &os, int resolution, int scaleEPS);  
  static void drawContour(ostream &os, const std::vector<ImaGene::Vector2i> &vectPoints, int color, int fillColor,
			  int epaisseurTrait, bool closeCnt,bool filled, int depth, double zoom, double x0, double y0);
  static void drawContour(ostream &os, const std::vector<ImaGene::Vector2D> &vectPoints, int color, int fillColor,
			  int epaisseurTrait, bool closeCnt,bool filled, int depth, double zoom, double x0, double y0);
  static void drawContourPixels(ostream &os, const FreemanChain & fc, int color, double zoom, int x0, int y0, int depth);
  static void drawContour(ostream &os, const FreemanChain & fc, int color, uint linewidth, int x0, int y0, int depth);
  
  static void drawContourPixelsCorner(ostream &os, const FreemanChain & fc, int color, double zoom, int x0, int y0, int depth);
  static void drawContourPixels(ostream & os, const std::vector<ImaGene::Vector2i> & vectContour, int color,
				double zoom, int x0, int y0, int depth);
  static void drawContourFromPointsSet(ostream &os, const std::vector<ImaGene::Vector2i> &vectPoints, 
				       const std::vector<ImaGene::Vector2i> &vectContour,
				       int color, int fillColor, int epaisseurTrait, bool closeCnt,bool filled, int depth, 
				       int zoom, int x0, int y0);

  static void drawPixel(ostream &os, const ImaGene::Vector2i &pixel, int color, int colorFill, double taille, int depth);
  static void drawPixel(ostream &os, const ImaGene::Vector2D &pixel, int color, int colorFill, double taille, int depth);
  static void drawPixelCorner(ostream &os, const ImaGene::Vector2i &pixel, int color, int colorFill, double taille, int depth, bool filled=true);

  static void drawImage(ostream & os, string nomImage, int width, int height, int depth);
  static void drawImage(ostream & os, string nomImage, int x0, int y0, int width, int height, int depth);
  static void drawCross(ostream &os, const ImaGene::Vector2i &point, int color, int largeur,int linewidth, int depth);
  static void drawCross(ostream &os, const ImaGene::Vector2D &point, int color, int largeur,int linewidth, int depth);
  static void drawLine(ostream &os, const ImaGene::Vector2i &point1, const ImaGene::Vector2i & point2,int color, int linewidth, int depth);
  static void drawLine(ostream &os, const ImaGene::Vector2D &point1, const ImaGene::Vector2D & point2,int color, int linewidth, int depth);
  static void drawLineVector(ostream &os, const ImaGene::Vector2D &point1, const ImaGene::Vector2D & point2,int color, int linewidth, int depth);
  static void drawCircle(ostream &os, const ImaGene::Vector2i &point, int color, double radius, int depth, bool filled=true, int linewidth= 1);
  static void drawCircle(ostream &os, const ImaGene::Vector2D &point, int color,
			 double radius, int depth, bool filled=true, int linewidth = 1);
  static void drawCircles(ostream &os, const std::vector<ImaGene::Vector2i> & vectPoint,
			  int color, double radius, int depth, bool filled =true, int linewidth=1);
  
  
  static vector<ImaGene::Vector2i> getSnowFlakeContour(const vector<ImaGene::Vector2D> &contour, int level);  
  
  static void drawDSS(ostream &os, const ImaGene::C4CSegment & dss, const Frame2D & frame, int color, 
		      double agrandissement,double decalX, double decalY, int linewidth, uint ms_mode,
		      double ms_coef_mode );
  
  
  static void drawBlurredSegment(ostream &os, const ImaGene::BlurredSegmentTgtCover::Segment segment,
				 uint color, uint linewidth, uint depth);

  
  
  
  static void setFillIntensity(int val);


  


private:

  
  struct PointAndIndex{
    Vector2i point;
    uint index;
  };

  

  static bool compPtAndIndex(const PointAndIndex  &pt1 , const PointAndIndex  &pt2 );
  
  
  static int getIndexOfPoint(const vector<ImaGene::Vector2i> &contour, const ImaGene::Vector2i &point );









};
}
#endif









