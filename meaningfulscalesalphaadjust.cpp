#include "meaningfulscalesalphaadjust.h"
#include "meaningfulscales.h"
#include "float.h"
#include "timer.h"

#include <QDebug>

#include "vector"

Timer timers[10];


MeaningfulScalesAlphaAdjust::MeaningfulScalesAlphaAdjust (SegmentationBasedOnComponentTree &algorithm, QImage &result, unsigned int indicator) :
    segmentationAlgorithm (algorithm),
    segmentationResult (result)
{
    this -> indicatorId = indicator;
}


double MeaningfulScalesAlphaAdjust::FindAlpha (double alphaStep, int samplingSizeMax = 7){
    cerr.rdbuf(0);

    qDebug () << "alpha size noise theBestSegmentationIndicator";
    double alpha = 0.0;
    segmentationAlgorithm.DoSegmentationWithPreviousComponentTree (alpha, segmentationResult, timers);
    unsigned int previousIterationPixelsFoundedNumber;
    unsigned int pixelsFoundedNumber = segmentationAlgorithm.FoundedPixelNumber ();

    MeaningfulScales ms (segmentationResult);
    ms.samplingSizeMax = samplingSizeMax;
    unsigned int freemanChainSize = ms.GetFreemanChainSize ();
    unsigned int sumOfNoise;
    double theBestIndicatorValue;
    double theBestAlpha = alpha;
    if (freemanChainSize > MeaningfulScales::MinimalFreemanChainSize){
        sumOfNoise = ms.GetSumOfNoise ();
        theBestIndicatorValue = indicator (freemanChainSize, sumOfNoise, pixelsFoundedNumber);
        //cout << alpha << "\t" << freemanChainSize << "\t" << sumOfNoise << "\t" << pixelsFoundedNumber << "\t" << theBestIndicatorValue << endl;
        qDebug () << alpha << freemanChainSize << sumOfNoise << pixelsFoundedNumber << theBestIndicatorValue;
    } else{
        sumOfNoise = ms.GetSumOfNoise ();
        theBestIndicatorValue = DBL_MAX;
        //qDebug () << alpha << "Too short Freeman chain." << freemanChainSize << "<" << MeaningfulScales::MinimalFreemanChainSize;
    }

    alpha += alphaStep;
    while (alpha <= 1.0){
        previousIterationPixelsFoundedNumber = pixelsFoundedNumber;
        segmentationAlgorithm.DoSegmentationWithPreviousComponentTree (alpha, segmentationResult, timers);
        pixelsFoundedNumber = segmentationAlgorithm.FoundedPixelNumber ();
        if (previousIterationPixelsFoundedNumber != pixelsFoundedNumber){
            MeaningfulScales ms2 (segmentationResult);
            ms2.samplingSizeMax = samplingSizeMax;
            freemanChainSize = ms2.GetFreemanChainSize ();
            sumOfNoise = ms2.GetSumOfNoise ();

            if (freemanChainSize > MeaningfulScales::MinimalFreemanChainSize){
                double indicatorValue = indicator (freemanChainSize, sumOfNoise, pixelsFoundedNumber);
                //cout << alpha << "\t" << freemanChainSize << "\t" << sumOfNoise << "\t" << pixelsFoundedNumber << "\t" << indicatorValue << endl;
                qDebug () << alpha << freemanChainSize << sumOfNoise << pixelsFoundedNumber << indicatorValue;
                if (indicatorValue < theBestIndicatorValue){
                    theBestIndicatorValue = indicatorValue;
                    theBestAlpha = alpha;
                }
            } else{
            //    qDebug () << alpha << "Too short Freeman chain." << freemanChainSize << "<" << MeaningfulScales::MinimalFreemanChainSize;
            }
        }

        alpha += alphaStep;
    }

    segmentationAlgorithm.DoSegmentationWithPreviousComponentTree (theBestAlpha, segmentationResult, timers);

    return theBestAlpha;
}


double MeaningfulScalesAlphaAdjust::indicator (unsigned int &freemanChainSize, unsigned int &totalNoise, unsigned int &resultArea){
    switch (indicatorId){
        case 1:
            return (double)totalNoise / (double)freemanChainSize;
        case 2://apple//BlueBottle//CLetterBetterMarker
            return (double)totalNoise / (double)resultArea;
        case 3://ContourS
            return (double)totalNoise / (double)(freemanChainSize * freemanChainSize);
        case 4://BlueBottle//ContourS//DigitalGeometryBookBetterMarker
            return (double)totalNoise / (double)(freemanChainSize * resultArea);
        case 5://CLetterBetterMarker//ContourS//GreenBottle
            return (double)totalNoise / (double)(freemanChainSize * freemanChainSize * resultArea);
        case 6://apple//BlueBottle//DigitalGeometryBookBetterMarker
            return (double)totalNoise / (double)(resultArea * resultArea);
        case 7:
            return (double)totalNoise / (double)(freemanChainSize * resultArea * resultArea);
        case 8:
            return (double)(totalNoise - freemanChainSize) / (double)(freemanChainSize);
        case 9://apple//BlueBottle//CLetterBetterMarker
            return (double)(totalNoise - freemanChainSize) / (double)(resultArea);
        case 10://ContourS
            return (double)(totalNoise - freemanChainSize) / (double)(freemanChainSize * freemanChainSize);
        case 11://BlueBottle//ContourS
            return (double)(totalNoise - freemanChainSize) / (double)(freemanChainSize * resultArea);
        case 12://CLetterBetterMarker//ContourS//GreenBottle
            return (double)(totalNoise - freemanChainSize) / (double)(freemanChainSize * freemanChainSize * resultArea);
        case 13://apple//BlueBottle
            return (double)(totalNoise - freemanChainSize) / (double)(resultArea * resultArea);
        case 14:
            return (double)(totalNoise - freemanChainSize) / (double)(freemanChainSize * resultArea * resultArea);
    }
    return (double)totalNoise/(double)freemanChainSize;
}
