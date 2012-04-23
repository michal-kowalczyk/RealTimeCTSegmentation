/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#ifndef MARKEDPIXELSINDICATORS_H
#define MARKEDPIXELSINDICATORS_H

#include "subimage.h"

class MarkedPixelsIndicators{
public:
    MarkedPixelsIndicators (unsigned char *image, Subimage &markedPixels, unsigned char outOfRangeSolutionNumber, unsigned int size);
    ~MarkedPixelsIndicators ();

    unsigned int GetSumOfMarkedPixels ();
    unsigned char GetAverageOfMarkedPixels ();
    unsigned int GetSumOfImagePixels ();
    unsigned char GetAverageOfImagePixels ();
    unsigned char *GetMarkedPixelsValues ();
    unsigned char GetMaximalValueOfMarkedPixels ();
    unsigned char GetShiftMaximazingAverageValueOfMarkedPixels ();
    unsigned char RangeSafeValue (int value);

protected:
    unsigned char *image;

    Subimage &markedPixels;

    unsigned char outOfRangeSolutionNumber;

    unsigned int size;

    bool sumCalculated;
    unsigned int sum;

    bool averageCalculated;
    unsigned char average;

    bool imageSumCalculated;
    unsigned int imageSum;

    bool imageAverageCalculated;
    unsigned char imageAverage;

    bool valuesCalculated;
    unsigned char *values;

    bool maximalValueCalculated;
    unsigned char maximalValue;

    bool shiftMaximazingAverageCalculated;
    unsigned char shiftMaximazingAverage;
};

#endif // MARKEDPIXELSINDICATORS_H
