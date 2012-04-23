/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#ifndef GRAYSCALEIMAGEHISTOGRAM_H
#define GRAYSCALEIMAGEHISTOGRAM_H


#include "histogram.h"
#include "subimage.h"


/**
  Use this to add to class Qt convenient methods. If you aren't using Qt, you can delete this line
  */
#define USE_CONVENIENT_QT_INTERFACE


#ifdef USE_CONVENIENT_QT_INTERFACE
class QImage;
#endif


class GrayScaleImageHistogram : public Histogram{
public:
    GrayScaleImageHistogram ();
    GrayScaleImageHistogram (unsigned char *pixelsArray, unsigned int pixelsNumber);
    GrayScaleImageHistogram (unsigned char *pixelsArray, Subimage &subimage);
    void Fill (unsigned char *pixelsArray, unsigned int pixelsNumber);
    void Fill (unsigned char *pixelsArray, Subimage &subimage);
    void BubbleBrightening (unsigned char *grayScaleImagePointer, unsigned int imageSize);

#ifdef USE_CONVENIENT_QT_INTERFACE
    GrayScaleImageHistogram (QImage &grayScaleImage);
    GrayScaleImageHistogram (QImage &grayScaleImage, Subimage &subimage);
    void Fill (QImage &grayScaleImage);
    void Fill (QImage &grayScaleImage, Subimage &subimage);
#endif
};

#endif // GRAYSCALEIMAGEHISTOGRAM_H
