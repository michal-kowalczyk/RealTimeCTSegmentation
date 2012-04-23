#ifndef COMPONENTTREEINPUTGENERATOR_H
#define COMPONENTTREEINPUTGENERATOR_H

#define MAXIMAL_CANALS_NUMBER 11

#include "subimage.h"


class QImage;


enum negationModeValues{
    NO_NEGATION,
    NEGATION,
    AUTONEGATION
};


enum canalsMixingModeValues{
    AVERAGE_MEAN,
    WEIGHTED_MEAN_BY_COLOURS_DIFFERENCES,
    WEIGHTED_MEAN_BY_BINARY_HISTOGRAM_COMPARISON,
    WEIGHTED_MEAN_BY_PROPORTIONAL_HISTOGRAM_COMPARISON
};


enum imageTransformationModeValues{
    NO_TRANSFORMATION,
    MARKED_PIXELS_HISTOGRAM_BUBBLE_BRIGHTENING,
    MARKED_PIXELS_MAXIMAL_VALUE_BRIGHTENING,
    MARKED_PIXELS_AVERAGE_PIXEL_VALUE_BRIGHTNENING,
    MARKED_PIXELS_MAXIMAL_AVERAGE_PIXEL_VALUE_BRIGHTENING
};


struct InputGeneratingParameters{
    bool redCanalIncluded;
    bool greenCanalIncluded;
    bool blueCanalIncluded;
    bool hueCanalIncluded;
    bool saturationCanalIncluded;
    bool valueCanalIncluded;
    bool lightnessCanalIncluded;
    bool cyanCanalIncluded;
    bool magentaCanalIncluded;
    bool yellowCanalIncluded;
    bool keyCanalIncluded;

    negationModeValues negationMode;
    canalsMixingModeValues canalsMixingMode;
    imageTransformationModeValues imageTransformationMode;
    unsigned char outOfRangeSolution;
};


class ComponentTreeInputGenerator{
public:
    ComponentTreeInputGenerator (unsigned int size);
    ~ComponentTreeInputGenerator ();

    void GenerateInput (const unsigned char *rgba, unsigned int *result, InputGeneratingParameters &parameters, Subimage &markedPixels);
    void GenerateInput (const unsigned char *rgba, QImage &result, InputGeneratingParameters &parameters, Subimage &markedPixels);
    void GenerateInput (QImage &image, QImage &result, InputGeneratingParameters &parameters, Subimage &markedPixels);

private:
    unsigned int size;

    unsigned char* canalsData[MAXIMAL_CANALS_NUMBER];
    unsigned int multipliers[MAXIMAL_CANALS_NUMBER];
};

#endif // COMPONENTTREEINPUTGENERATOR_H
