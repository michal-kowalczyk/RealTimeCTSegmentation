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
    void Reuse (unsigned char *image, Subimage &markedPixels, unsigned char outOfRangeSolutionNumber, unsigned int size);

    unsigned int GetSumOfMarkedPixels ();
    unsigned char GetAverageOfMarkedPixels ();
    unsigned int GetSumOfImagePixels ();
    unsigned char GetAverageOfImagePixels ();
    unsigned int GetSumOfNotMarkedPixels ();
    unsigned char GetAverageOfNotMarkedPixels ();
    unsigned char *GetMarkedPixelsValues ();
    unsigned char GetMaximalValueOfMarkedPixels ();
    unsigned char GetShiftMaximazingAverageValueOfMarkedPixels ();
    unsigned char RangeSafeValue (int value);

protected:
    unsigned char *image;

    Subimage &markedPixels;

    unsigned char outOfRangeSolutionNumber;

    unsigned int size;

    bool markedSumCalculated;
    unsigned int markedSum;

    bool markedAverageCalculated;
    unsigned char markedAverage;

    bool imageSumCalculated;
    unsigned int imageSum;

    bool imageAverageCalculated;
    unsigned char imageAverage;

    bool notMarkedSumCalculated;
    unsigned int notMarkedSum;

    bool notMarkedAverageCalculated;
    unsigned char notMarkedAverage;

    bool valuesCalculated;
    unsigned char *values;

    bool maximalValueCalculated;
    unsigned char maximalValue;

    bool shiftMaximazingAverageCalculated;
    unsigned char shiftMaximazingAverage;
};

#endif // MARKEDPIXELSINDICATORS_H
