#ifndef MEANINGFULSCALESALPHAADJUST_H
#define MEANINGFULSCALESALPHAADJUST_H

#include <QImage>
#include "segmentationbasedoncomponenttree.h"


class MeaningfulScalesAlphaAdjust{
public:
    MeaningfulScalesAlphaAdjust (SegmentationBasedOnComponentTree &algorithm, QImage &result, unsigned int indicator);
    double FindAlpha (double alphaStep, int samplingSizeMax);

protected:
    SegmentationBasedOnComponentTree &segmentationAlgorithm;
    QImage &segmentationResult;
    unsigned int indicatorId;

    double indicator (unsigned int &freemanChainSize, unsigned int &totalNoise, unsigned int &resultArea);
};

#endif // MEANINGFULSCALESALPHAADJUST_H
