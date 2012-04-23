/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#include "grayscaleimagehistogram.h"


#ifdef USE_CONVENIENT_QT_INTERFACE
#include <QImage>
#endif


GrayScaleImageHistogram::GrayScaleImageHistogram () : Histogram (256){
}


GrayScaleImageHistogram::GrayScaleImageHistogram (unsigned char *pixelsArray, unsigned int pixelsNumber) : Histogram (256){
    Fill (pixelsArray, pixelsNumber);
}


GrayScaleImageHistogram::GrayScaleImageHistogram (unsigned char *pixelsArray, Subimage &subimage) : Histogram (256){
    Fill (pixelsArray, subimage);
}


inline void GrayScaleImageHistogram::Fill (unsigned char *pixelsArray, unsigned int pixelsNumber){
    unsigned char *pixelsArrayEndPointer = pixelsArray + pixelsNumber;
    while (pixelsArray < pixelsArrayEndPointer){
        data[*pixelsArray]++;
        pixelsArray++;
    }
}


inline void GrayScaleImageHistogram::Fill (unsigned char *pixelsArray, Subimage &subimage){
    for (Subimage::PixelsPositionsIterator i = subimage.begin (); i != subimage.end (); ++i)
        data[pixelsArray[*i]]++;
}


void GrayScaleImageHistogram::BubbleBrightening (unsigned char *grayScaleImagePointer, unsigned int imageSize){
    unsigned char colourReplacements[256], colourChangement[256];
    for (short currentColour = 0; currentColour < 256; currentColour++)
        colourChangement[currentColour] = currentColour;

    unsigned short theDarkestColour = 256;
    unsigned int *currentPointer = data + size - 1;

    for (short currentColour = 255; currentColour >= 0; currentColour--){
        if (*currentPointer){
            theDarkestColour--;
            colourChangement[currentColour] = theDarkestColour;
            colourChangement[theDarkestColour] = currentColour;
        }
        currentPointer--;
    }
    for (short currentColour = 0; currentColour < 256; currentColour++)
        colourReplacements[colourChangement[currentColour]] = currentColour;

    unsigned char *grayScaleImageEndPointer = grayScaleImagePointer + imageSize;
    while (grayScaleImagePointer < grayScaleImageEndPointer){
        *grayScaleImagePointer = colourReplacements[*grayScaleImagePointer];
        grayScaleImagePointer++;
    }
}


#ifdef USE_CONVENIENT_QT_INTERFACE

GrayScaleImageHistogram::GrayScaleImageHistogram (QImage &grayScaleImage) : Histogram (256){
    Fill (grayScaleImage);
}

GrayScaleImageHistogram::GrayScaleImageHistogram (QImage &grayScaleImage, Subimage &subimage) : Histogram (256){
    Fill (grayScaleImage, subimage);
}


inline void GrayScaleImageHistogram::Fill (QImage &grayScaleImage){
    unsigned int *pixelsArray = (unsigned int *)grayScaleImage.bits ();
    unsigned int *pixelsArrayEndPointer = pixelsArray + grayScaleImage.width () * grayScaleImage.height ();
    while (pixelsArray < pixelsArrayEndPointer){
        data[qRed (*pixelsArray)]++;
        pixelsArray++;
    }
}


inline void GrayScaleImageHistogram::Fill (QImage &grayScaleImage, Subimage &subimage){
    unsigned int *pixelsArray = (unsigned int *)grayScaleImage.bits ();
    for (Subimage::PixelsPositionsIterator i = subimage.begin (); i != subimage.end (); ++i)
        data[qRed (pixelsArray[*i])]++;
}

#endif
