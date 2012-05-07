/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#include "markedpixelsindicators.h"


/**
  remember copy initialization functions to Reuse function
  */
MarkedPixelsIndicators::MarkedPixelsIndicators (unsigned char *image, Subimage &markedPixels, unsigned char outOfRangeSolutionNumber, unsigned int size) :
    image (image),
    markedPixels (markedPixels),
    outOfRangeSolutionNumber (outOfRangeSolutionNumber),
    size (size),
    markedSumCalculated (false),
    markedAverageCalculated (false),
    imageSumCalculated (false),
    imageAverageCalculated (false),
    notMarkedSumCalculated (false),
    notMarkedAverageCalculated (false),
    valuesCalculated (false),
    values (0),
    maximalValueCalculated (false),
    shiftMaximazingAverageCalculated (false)
{
}


MarkedPixelsIndicators::~MarkedPixelsIndicators (){
    if (valuesCalculated)
        delete []values;
}


void MarkedPixelsIndicators::Reuse (unsigned char *image, Subimage &markedPixels, unsigned char outOfRangeSolutionNumber, unsigned int size){
    this -> image = image;
    if (valuesCalculated){
        if (this -> markedPixels.Size () != markedPixels.Size())
            delete []values;
        valuesCalculated = false;
    }
    this -> markedPixels = markedPixels;
    this -> outOfRangeSolutionNumber = outOfRangeSolutionNumber;
    this -> size = size;
    markedSumCalculated = false;
    markedAverageCalculated = false;
    imageSumCalculated = false;
    imageAverageCalculated = false;
    notMarkedSumCalculated = false;
    notMarkedAverageCalculated = false;
    maximalValueCalculated = false;
    shiftMaximazingAverageCalculated = false;
}


unsigned int MarkedPixelsIndicators::GetSumOfMarkedPixels (){
    if (markedSumCalculated)
        return markedSum;

    markedSum = 0;
    for (Subimage::PixelsPositionsIterator i = markedPixels.begin (); i != markedPixels.end (); ++i)
        markedSum += image[*i];

    markedSumCalculated = true;

    return markedSum;
}


/**
  Average value of marked pixels
  */
unsigned char MarkedPixelsIndicators::GetAverageOfMarkedPixels (){
    if (markedAverageCalculated)
        return markedAverage;

    markedAverage = GetSumOfMarkedPixels () / markedPixels.Size ();

    markedAverageCalculated = true;

    return markedAverage;
}


/**
  Calculating sum of all pixels
  */
unsigned int MarkedPixelsIndicators::GetSumOfImagePixels (){
    if (imageSumCalculated)
        return imageSum;

    imageSum = 0;
    unsigned char *theLastPixelPointer = image + size - 1;
    while (theLastPixelPointer >= image){
        imageSum += *theLastPixelPointer;
        theLastPixelPointer--;
    }

    imageSumCalculated = true;

    return imageSum;
}


/**
  Average value of all pixels
  */
unsigned char MarkedPixelsIndicators::GetAverageOfImagePixels (){
    if (imageAverageCalculated)
        return imageAverage;

    imageAverage = GetSumOfImagePixels () / size;

    imageAverageCalculated = true;

    return imageAverage;
}


/**
  Calculating sum of not marked pixels
  */
unsigned int MarkedPixelsIndicators::GetSumOfNotMarkedPixels (){
    if (notMarkedSumCalculated)
        return notMarkedSum;

    notMarkedSum = GetSumOfImagePixels () - GetSumOfMarkedPixels ();

    notMarkedSumCalculated = true;

    return notMarkedSum;
}


/**
  Average value of not marked pixels
  */
unsigned char MarkedPixelsIndicators::GetAverageOfNotMarkedPixels (){
    if (notMarkedAverageCalculated)
        return notMarkedAverage;

    if (size - markedPixels.Size () > 0)
        notMarkedAverage = GetSumOfNotMarkedPixels () / (size - markedPixels.Size ());
    else
        notMarkedAverage = 0;

    imageAverageCalculated = true;

    return notMarkedAverage;
}


/**
  Create array with marked pixel values
  */
unsigned char* MarkedPixelsIndicators::GetMarkedPixelsValues (){
    if (valuesCalculated)
        return values;

    if (values == 0)
        values = new unsigned char[markedPixels.Size ()];
    unsigned char* valuesPointer = values;
    for (Subimage::PixelsPositionsIterator i = markedPixels.begin (); i != markedPixels.end (); ++i){
        *valuesPointer = image[*i];
        valuesPointer++;
    }

    valuesCalculated = true;

    return values;
}


/**
  Maximal value of marked pixels.
  */
unsigned char MarkedPixelsIndicators::GetMaximalValueOfMarkedPixels (){
    if (maximalValueCalculated)
        return maximalValue;

    maximalValue = 0;
    unsigned char colour;
    for (Subimage::PixelsPositionsIterator i = markedPixels.begin (); i != markedPixels.end (); ++i){
        colour = image[*i];
        if (maximalValue < colour)
            maximalValue = colour;
    }

    maximalValueCalculated = true;

    return maximalValue;
}


/**
  Shift that makes maximum of function of average values of marked pixels
  */
unsigned char MarkedPixelsIndicators::GetShiftMaximazingAverageValueOfMarkedPixels (){
    if (shiftMaximazingAverageCalculated)
        return shiftMaximazingAverage;

    unsigned int colourSums[255];
    colourSums[0] = GetAverageOfMarkedPixels ();
    shiftMaximazingAverage = 0;
    unsigned char colour;
    for (unsigned int c = 1; c < 255; c++){
        colourSums[c] = 0;
        for (Subimage::PixelsPositionsIterator i = markedPixels.begin (); i != markedPixels.end (); ++i){
            colour = image[*i] + c;
            colourSums[c] += colour;
        }
        if (colourSums[shiftMaximazingAverage] < colourSums[c])
            shiftMaximazingAverage = c;
    }

    shiftMaximazingAverageCalculated = true;

    return shiftMaximazingAverage;
}


/**
  @todo use dynamic linking function instead of switch construction
  @todo inline?
  */
unsigned char MarkedPixelsIndicators::RangeSafeValue (int value){
    if (value < 256)
        return value;

    switch (outOfRangeSolutionNumber){
        case 1:
            return value - 255;
        case 2:
            return 255;
        case 6:
            return 510 - value;
        default:
            return value;
    }
}
