#include "componenttreeinputgenerator.h"
#include <QImage>
#include <QDebug>
#include <QColor>

#include "grayscaleimagehistogram.h"
#include "markedpixelsindicators.h"


ComponentTreeInputGenerator::ComponentTreeInputGenerator (unsigned int size){
    this -> size = size;
    for (unsigned int i = 0; i < MAXIMAL_CANALS_NUMBER; i++)
        canalsData[i] = new unsigned char[size];
}


ComponentTreeInputGenerator::~ComponentTreeInputGenerator (){
    for (unsigned int i = 0; i < MAXIMAL_CANALS_NUMBER; i++)
        delete []canalsData[i];
}


void ComponentTreeInputGenerator::GenerateInput (const unsigned char *rgba, unsigned int *result, InputGeneratingParameters &parameters, Subimage &markedPixels){
    unsigned char canalsNumber = 0;
    if (parameters.redCanalIncluded)
        canalsNumber++;
    if (parameters.greenCanalIncluded)
        canalsNumber++;
    if (parameters.blueCanalIncluded)
        canalsNumber++;
    if (parameters.hueCanalIncluded)
        canalsNumber++;
    if (parameters.saturationCanalIncluded)
        canalsNumber++;
    if (parameters.valueCanalIncluded)
        canalsNumber++;
    if (parameters.lightnessCanalIncluded)
        canalsNumber++;
    if (parameters.cyanCanalIncluded)
        canalsNumber++;
    if (parameters.magentaCanalIncluded)
        canalsNumber++;
    if (parameters.yellowCanalIncluded)
        canalsNumber++;
    if (parameters.keyCanalIncluded)
        canalsNumber++;

    if (canalsNumber){
        unsigned char canal = 0;

        if (parameters.redCanalIncluded){
            for (unsigned int j = 0, i = 2; j < size; j++){
                canalsData[canal][j] = rgba[i];
                i += 4;
            }
            canal++;
        }

        if (parameters.greenCanalIncluded){
            for (unsigned int j = 0, i = 1; j < size; j++){
                canalsData[canal][j] = rgba[i];
                i += 4;
            }
            canal++;
        }

        if (parameters.blueCanalIncluded){
            for (unsigned int j = 0, i = 0; j < size; j++){
                canalsData[canal][j] = rgba[i];
                i += 4;
            }
            canal++;
        }

        if (parameters.hueCanalIncluded){
            for (unsigned int j = 0, i = 0; j < size; j++){
                canalsData[canal][j] = ((QColor (rgba[i + 2], rgba[i + 1], rgba[i]).hue () % 360) * 255) / 360;
                i += 4;
            }
            canal++;
        }

        if (parameters.saturationCanalIncluded){
            for (unsigned int j = 0, i = 0; j < size; j++){
                canalsData[canal][j] = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).saturation ();
                i += 4;
            }
            canal++;
        }

        if (parameters.valueCanalIncluded){
            for (unsigned int j = 0, i = 0; j < size; j++){
                canalsData[canal][j] = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).value ();
                i += 4;
            }
            canal++;
        }

        if (parameters.lightnessCanalIncluded){
            for (unsigned int j = 0, i = 0; j < size; j++){
                canalsData[canal][j] = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).lightness ();
                i += 4;
            }
            canal++;
        }

        if (parameters.cyanCanalIncluded){
            for (unsigned int j = 0, i = 0; j < size; j++){
                canalsData[canal][j] = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).cyan ();
                i += 4;
            }
            canal++;
        }

        if (parameters.magentaCanalIncluded){
            for (unsigned int j = 0, i = 0; j < size; j++){
                canalsData[canal][j] = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).magenta ();
                i += 4;
            }
            canal++;
        }

        if (parameters.yellowCanalIncluded){
            for (unsigned int j = 0, i = 0; j < size; j++){
                canalsData[canal][j] = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).yellow ();
                i += 4;
            }
            canal++;
        }

        if (parameters.keyCanalIncluded){
            for (unsigned int j = 0, i = 0; j < size; j++){
                canalsData[canal][j] = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).black ();
                i += 4;
            }
            canal++;
        }

        for (canal = 0; canal < canalsNumber; canal++){
            multipliers[canal] = 0;

            if (parameters.negationMode == NEGATION)
                for (unsigned int j = 0; j < size; j++)
                    canalsData[canal][j] = 255 - canalsData[canal][j];

            /*
              Transformations of image based on marker
              */
            if (markedPixels.Size ()){
                int theLightessValue = 255;//ui -> theLightessValueDial -> value();

                MarkedPixelsIndicators indicators (canalsData[canal], markedPixels, parameters.outOfRangeSolution, size);


                /**
                  * Negates image if average marked pixels value is lower then 128
                  * @advantage no problem with holes for marked area
                  * @disadvantage no improvement for marked area with average close to 128
                  */
                if (parameters.negationMode == AUTONEGATION){
                    MarkedPixelsIndicators inputIndicators (canalsData[canal], markedPixels, parameters.outOfRangeSolution, size);
                    if (inputIndicators.GetAverageOfMarkedPixels () < 128)
                        for (unsigned int j = 0; j < size; j++)
                            canalsData[canal][j] = 255 - canalsData[canal][j];
                }


                if (parameters.canalsMixingMode == WEIGHTED_MEAN_BY_COLOURS_DIFFERENCES){
                    multipliers[canal] = indicators.GetAverageOfMarkedPixels () - indicators.GetAverageOfImagePixels ();
                    multipliers[canal] = (multipliers[canal] * multipliers[canal] * multipliers[canal] * multipliers[canal]) / 65025;
                } else if (parameters.canalsMixingMode == WEIGHTED_MEAN_BY_BINARY_HISTOGRAM_COMPARISON){
                    GrayScaleImageHistogram inputHistogram (canalsData[canal], size);
                    GrayScaleImageHistogram markedPointsHistogram (canalsData[canal], markedPixels);
                    multipliers[canal] = 256 - GrayScaleImageHistogram::BinaryCompare (inputHistogram, markedPointsHistogram);
                    multipliers[canal] = (multipliers[canal] * multipliers[canal] * multipliers[canal] * multipliers[canal]) / 65025;
                } else if (parameters.canalsMixingMode == WEIGHTED_MEAN_BY_PROPORTIONAL_HISTOGRAM_COMPARISON){
                    GrayScaleImageHistogram inputHistogram (canalsData[canal], size);
                    GrayScaleImageHistogram markedPointsHistogram (canalsData[canal], markedPixels);
                    multipliers[canal] = GrayScaleImageHistogram::AverageDifferenceCompare (inputHistogram, markedPointsHistogram);
                }


                if (parameters.imageTransformationMode == MARKED_PIXELS_HISTOGRAM_BUBBLE_BRIGHTENING){
                    GrayScaleImageHistogram markedPointsHistogram (canalsData[canal], markedPixels);
                    markedPointsHistogram.BubbleBrightening (canalsData[canal], size);
                }


                /**
                  * Shifts all pixels to make the lightest marked point lightness value equal 255
                  */
                if (parameters.imageTransformationMode == MARKED_PIXELS_MAXIMAL_VALUE_BRIGHTENING){
                    unsigned char shift = theLightessValue - indicators.GetMaximalValueOfMarkedPixels ();
                    if (shift > 0)
                        for (unsigned int j = 0; j < size; j++)
                            canalsData[canal][j] = indicators.RangeSafeValue (canalsData[canal][j] + shift);
                }


                /**
                  * Shifts all pixels (by average of value of marked pixels) to have marked area lighter. If values of marked area are higher then 255, set it to 255.
                  * @advantage no holes in marked area
                  * @disadvantage bad behaviour when the lighest pixel is on a border of marker
                  */
                if (parameters.imageTransformationMode == MARKED_PIXELS_AVERAGE_PIXEL_VALUE_BRIGHTNENING){
                    unsigned char shift = theLightessValue - indicators.GetAverageOfMarkedPixels ();
                    if (shift > 0)
                        for (unsigned int j = 0; j < size; j++)
                            canalsData[canal][j] = indicators.RangeSafeValue (canalsData[canal][j] + shift);
                }


                /**
                  * Shifts all pixels values to make it marked object lighter (to make the lightest pixel value of the marker equal 255).
                  * @advantage makes dark areas lighter
                  * @disadvantage if there are reflexes on marked object, there will be no improvement
                  * @disadvantage if marker doesn't mark the lightest pixel of object, it will be changed to dark and probably wont be found.
                  * @proposedSolution set shifting that way to have maximal value less then 255 (to make some place for lighter pixels)
                  */
                if (parameters.imageTransformationMode == MARKED_PIXELS_MAXIMAL_AVERAGE_PIXEL_VALUE_BRIGHTENING){
                    unsigned char shift = theLightessValue - indicators.GetMaximalValueOfMarkedPixels ();
                    if (shift > 0)
                        for (unsigned int j = 0; j < size; j++)
                            canalsData[canal][j] = indicators.RangeSafeValue (canalsData[canal][j] + shift);
                }
            }
        }

        int multipliersSum = 0;
        for (unsigned j = 0; j < canalsNumber; j++){
            multipliersSum += multipliers[j];
            qDebug () << "Canal" << j + 1 << "multiplier:" << multipliers[j];
        }

        if (multipliersSum < 1){
            for (unsigned j = 0; j < canalsNumber; j++)
                multipliers[j] = 1;
            multipliersSum = canalsNumber;
        }

        int colour;
        for (unsigned int j = 0; j < size; j++){
            colour = 0;
            for (canal = 0; canal < canalsNumber; canal++)
                colour += multipliers[canal] * canalsData[canal][j];
            colour /= multipliersSum;
            colour = (unsigned char)colour;
            result[j] = colour | ((colour | ((colour | 65280) << 8)) << 8);
        }
    }
}


void ComponentTreeInputGenerator::GenerateInput (const unsigned char *rgba, QImage &result, InputGeneratingParameters &parameters, Subimage &markedPixels){
    GenerateInput (rgba, (unsigned int *)result.bits (), parameters, markedPixels);
}


void ComponentTreeInputGenerator::GenerateInput (QImage &image, QImage &result, InputGeneratingParameters &parameters, Subimage &markedPixels){
    GenerateInput ((const unsigned char *)image.constBits (), (unsigned int *)result.bits (), parameters, markedPixels);
}
