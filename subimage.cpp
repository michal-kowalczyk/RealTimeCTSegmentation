/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#include "subimage.h"

#ifdef USE_CONVENIENT_QT_INTERFACE
#include <QImage>
#endif


Subimage::Subimage (unsigned int size) :
    inputSize (size),
    pixelsCoordinates (new unsigned int[inputSize]),
    pixelNumber (0),
    beginIterator (pixelsCoordinates),
    endIterator (pixelsCoordinates){}


Subimage::Subimage (const Subimage& subimage) :
    inputSize (subimage.inputSize),
    pixelsCoordinates (new unsigned int[inputSize]),
    pixelNumber (subimage.pixelNumber),
    beginIterator (pixelsCoordinates),
    endIterator (pixelsCoordinates + pixelNumber)
{
    memcpy (pixelsCoordinates, subimage.pixelsCoordinates, pixelNumber * sizeof (unsigned int));
}


Subimage::~Subimage (){
    delete []pixelsCoordinates;
}


#ifdef USE_CONVENIENT_QT_INTERFACE

void Subimage::Update (QImage& subimage){
    const unsigned int* subimageStartPointer = (const unsigned int*)subimage.constBits ();
    pixelNumber = 0;
    for (unsigned int j = 0; j < inputSize; j++){
        if (*subimageStartPointer > 0){
            pixelsCoordinates[pixelNumber] = j;
            pixelNumber++;
        }
        subimageStartPointer++;
    }
    endIterator = pixelsCoordinates + pixelNumber;
}

#endif
