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


/**
  @todo create and destroy indicators/histograms only in constructor and destructor of this class; don't do it everytime that method runs
  */
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
        unsigned int rgbaSize = size * 4;

        if (parameters.redCanalIncluded){
            unsigned char *data = *(canalsData + canal);
            for (unsigned int i = 2; i < rgbaSize; i += 4){
                *data = rgba[i];
                data++;
            }
            canal++;
        }

        if (parameters.greenCanalIncluded){
            unsigned char *data = *(canalsData + canal);
            for (unsigned int i = 1; i < rgbaSize; i += 4){
                *data = rgba[i];
                data++;
            }
            canal++;
        }

        if (parameters.blueCanalIncluded){
            unsigned char *data = *(canalsData + canal);
            for (unsigned int i = 0; i < rgbaSize; i += 4){
                *data = rgba[i];
                data++;
            }
            canal++;
        }

        // --- BEGIN --- HSV/HSL all combination
        if (parameters.hueCanalIncluded && parameters.saturationCanalIncluded && parameters.valueCanalIncluded){
            unsigned char *hueData = *(canalsData + canal);
            unsigned char *saturationData = *(canalsData + canal + 1);
            unsigned char *valueData = *(canalsData + canal + 2);
            int hue;
            unsigned int i = 0;
            while (i < rgbaSize){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getHsv(&hue, (int *)saturationData, (int *)valueData);
                *hueData = (hue * 255) / 360;
                hueData++;
                saturationData++;
                valueData++;
                i += 4;
            }
            canal += 3;

            if (parameters.lightnessCanalIncluded){
                unsigned char *data = *(canalsData + canal);
                i = 0;
                while (i < rgbaSize){
                    *data = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).lightness ();
                    data++;
                    i += 4;
                }
                canal++;
            }
        } else if (parameters.hueCanalIncluded && parameters.saturationCanalIncluded && parameters.lightnessCanalIncluded){
            unsigned char *hueData = *(canalsData + canal);
            unsigned char *saturationData = *(canalsData + canal + 1);
            unsigned char *lightnessData = *(canalsData + canal + 2);
            int hue;
            unsigned int i = 0;
            while (i < rgbaSize){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getHsl(&hue, (int *)saturationData, (int *)lightnessData);
                *hueData = (hue * 255) / 360;
                hueData++;
                saturationData++;
                lightnessData++;
                i += 4;
            }
            canal += 3;
        } else if (parameters.hueCanalIncluded && parameters.saturationCanalIncluded){
            unsigned char *hueData = *(canalsData + canal);
            unsigned char *saturationData = *(canalsData + canal + 1);
            int hue, temp;
            unsigned int i = 0;
            while (i < rgbaSize){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getHsv(&hue, (int *)saturationData, &temp);
                *hueData = (hue * 255) / 360;
                hueData++;
                saturationData++;
                i += 4;
            }
            canal += 2;
        } else if (parameters.hueCanalIncluded && parameters.valueCanalIncluded){
            unsigned char *hueData = *(canalsData + canal);
            unsigned char *valueData = *(canalsData + canal + 1);
            int hue, temp;
            unsigned int i = 0;
            while (i < rgbaSize){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getHsv(&hue, &temp, (int *)valueData);
                *hueData = (hue * 255) / 360;
                hueData++;
                valueData++;
                i += 4;
            }
            canal += 2;
        } else if (parameters.hueCanalIncluded && parameters.lightnessCanalIncluded){
            unsigned char *hueData = *(canalsData + canal);
            unsigned char *lightnessData = *(canalsData + canal + 1);
            int hue, temp;
            unsigned int i = 0;
            while (i < rgbaSize){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getHsl(&hue, &temp, (int *)lightnessData);
                *hueData = (hue * 255) / 360;
                hueData++;
                lightnessData++;
                i += 4;
            }
            canal += 2;
        } else if (parameters.saturationCanalIncluded && parameters.valueCanalIncluded){
            unsigned char *saturationData = *(canalsData + canal);
            unsigned char *valueData = *(canalsData + canal + 1);
            int temp;
            unsigned int i = 0;
            while (i < rgbaSize){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getHsv(&temp, (int *)saturationData, (int *)valueData);
                saturationData++;
                valueData++;
                i += 4;
            }
            canal += 2;
        } else if (parameters.saturationCanalIncluded && parameters.lightnessCanalIncluded){
            unsigned char *saturationData = *(canalsData + canal);
            unsigned char *lightnessData = *(canalsData + canal + 1);
            int temp;
            unsigned int i = 0;
            while (i < rgbaSize){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getHsl(&temp, (int *)saturationData, (int *)lightnessData);
                saturationData++;
                lightnessData++;
                i += 4;
            }
            canal += 2;
        } else if (parameters.hueCanalIncluded){
            unsigned char *data = *(canalsData + canal);
            unsigned int i = 0;
            while  (i < rgbaSize){
                *data = (QColor (rgba[i + 2], rgba[i + 1], rgba[i]).hue () * 255) / 360;
                data++;
                i += 4;
            }
            canal++;
        } else if (parameters.saturationCanalIncluded){
            unsigned char *data = *(canalsData + canal);
            unsigned int i = 0;
            while  (i < rgbaSize){
                *data = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).saturation ();
                data++;
                i += 4;
            }
            canal++;
        } else{
            if (parameters.valueCanalIncluded){
                unsigned char *data = *(canalsData + canal);
                unsigned int i = 0;
                while  (i < rgbaSize){
                    *data = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).value ();
                    data++;
                    i += 4;
                }
                canal++;
            }

            if (parameters.lightnessCanalIncluded){
                unsigned char *data = *(canalsData + canal);
                unsigned int i = 0;
                while  (i < rgbaSize){
                    *data = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).lightness ();
                    data++;
                    i += 4;
                }
                canal++;
            }
        }
        // --- END --- HSV/HSL all combination

        // --- BEGIN --- CMYK all combination
        if (parameters.cyanCanalIncluded && parameters.magentaCanalIncluded && parameters.yellowCanalIncluded && parameters.keyCanalIncluded){
            unsigned char *cyanData = *(canalsData + canal);
            unsigned char *magentaData = *(canalsData + canal + 1);
            unsigned char *yellowData = *(canalsData + canal + 2);
            unsigned char *keyData = *(canalsData + canal + 3);

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk((int *)cyanData, (int *)magentaData, (int *)yellowData, (int *)keyData);
                cyanData++;
                magentaData++;
                yellowData++;
                keyData++;
            }

            canal += 4;
        } else if (parameters.cyanCanalIncluded && parameters.magentaCanalIncluded && parameters.yellowCanalIncluded){
            unsigned char *cyanData = *(canalsData + canal);
            unsigned char *magentaData = *(canalsData + canal + 1);
            unsigned char *yellowData = *(canalsData + canal + 2);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk((int *)cyanData, (int *)magentaData, (int *)yellowData, &temp);
                cyanData++;
                magentaData++;
                yellowData++;
            }

            canal += 3;
        } else if (parameters.cyanCanalIncluded && parameters.magentaCanalIncluded && parameters.keyCanalIncluded){
            unsigned char *cyanData = *(canalsData + canal);
            unsigned char *magentaData = *(canalsData + canal + 1);
            unsigned char *keyData = *(canalsData + canal + 2);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk((int *)cyanData, (int *)magentaData, &temp, (int *)keyData);
                cyanData++;
                magentaData++;
                keyData++;
            }

            canal += 3;
        } else if (parameters.cyanCanalIncluded && parameters.yellowCanalIncluded && parameters.keyCanalIncluded){
            unsigned char *cyanData = *(canalsData + canal);
            unsigned char *yellowData = *(canalsData + canal + 1);
            unsigned char *keyData = *(canalsData + canal + 2);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk((int *)cyanData, &temp, (int *)yellowData, (int *)keyData);
                cyanData++;
                yellowData++;
                keyData++;
            }

            canal += 3;
        } else if (parameters.magentaCanalIncluded && parameters.yellowCanalIncluded && parameters.keyCanalIncluded){
            unsigned char *magentaData = *(canalsData + canal);
            unsigned char *yellowData = *(canalsData + canal + 1);
            unsigned char *keyData = *(canalsData + canal + 2);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk(&temp, (int *)magentaData, (int *)yellowData, (int *)keyData);
                magentaData++;
                yellowData++;
                keyData++;
            }

            canal += 3;
        } else if (parameters.cyanCanalIncluded && parameters.magentaCanalIncluded){
            unsigned char *cyanData = *(canalsData + canal);
            unsigned char *magentaData = *(canalsData + canal + 1);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk((int *)cyanData, (int *)magentaData, &temp, &temp);
                cyanData++;
                magentaData++;
            }

            canal += 2;
        } else if (parameters.cyanCanalIncluded && parameters.yellowCanalIncluded){
            unsigned char *cyanData = *(canalsData + canal);
            unsigned char *yellowData = *(canalsData + canal + 1);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk((int *)cyanData, &temp, (int *)yellowData, &temp);
                cyanData++;
                yellowData++;
            }

            canal += 2;
        } else if (parameters.cyanCanalIncluded && parameters.keyCanalIncluded){
            unsigned char *cyanData = *(canalsData + canal);
            unsigned char *keyData = *(canalsData + canal + 1);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk((int *)cyanData, &temp, &temp, (int *)keyData);
                cyanData++;
                keyData++;
            }

            canal += 2;
        } else if (parameters.magentaCanalIncluded && parameters.yellowCanalIncluded){
            unsigned char *magentaData = *(canalsData + canal);
            unsigned char *yellowData = *(canalsData + canal + 1);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk(&temp, (int *)magentaData, (int *)yellowData, &temp);
                magentaData++;
                yellowData++;
            }

            canal += 2;
        } else if (parameters.magentaCanalIncluded && parameters.keyCanalIncluded){
            unsigned char *magentaData = *(canalsData + canal);
            unsigned char *keyData = *(canalsData + canal + 1);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk(&temp, (int *)magentaData, &temp, (int *)keyData);
                magentaData++;
                keyData++;
            }

            canal += 2;
        } else if (parameters.yellowCanalIncluded && parameters.keyCanalIncluded){
            unsigned char *yellowData = *(canalsData + canal);
            unsigned char *keyData = *(canalsData + canal + 1);

            int temp;

            for (unsigned int i = 0; i < rgbaSize; i += 4){
                QColor (rgba[i + 2], rgba[i + 1], rgba[i]).getCmyk(&temp, &temp, (int *)yellowData, (int *)keyData);
                yellowData++;
                keyData++;
            }

            canal += 2;
        } else if (parameters.cyanCanalIncluded){
            unsigned char *data = *(canalsData + canal);
            for (unsigned int i = 0; i < rgbaSize; i += 4){
                *data = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).cyan ();
                data++;
            }
            canal++;
        } else if (parameters.magentaCanalIncluded){
            unsigned char *data = *(canalsData + canal);
            for (unsigned int i = 0; i < rgbaSize; i += 4){
                *data = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).magenta ();
                data++;
            }
            canal++;
        } else if (parameters.yellowCanalIncluded){
            unsigned char *data = *(canalsData + canal);
            for (unsigned int i = 0; i < rgbaSize; i += 4){
                *data = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).yellow ();
                data++;
            }
            canal++;
        } else if (parameters.keyCanalIncluded){
            unsigned char *data = *(canalsData + canal);
            for (unsigned int i = 0; i < rgbaSize; i += 4){
                *data = QColor (rgba[i + 2], rgba[i + 1], rgba[i]).black ();
                data++;
            }
            canal++;
        }
        // --- END --- CMYK all combination

        unsigned char *currentCanalData;
        for (canal = 0; canal < canalsNumber; canal++){
            multipliers[canal] = 0;
            currentCanalData = *(canalsData + canal);

            if (parameters.negationMode == NEGATION)
                for (unsigned int j = 0; j < size; j++)
                    currentCanalData[j] = 255 - currentCanalData[j];

            /*
              Transformations of image based on marker
              */
            if (markedPixels.Size ()){
                MarkedPixelsIndicators indicators (currentCanalData, markedPixels, parameters.outOfRangeSolution, size);


                /**
                  * Negates image if average marked pixels value is lower then 128
                  * @advantage no problem with holes for marked area
                  * @disadvantage no improvement for marked area with average close to 128
                  */
                if (parameters.negationMode == AUTONEGATION){
                    MarkedPixelsIndicators inputIndicators (currentCanalData, markedPixels, parameters.outOfRangeSolution, size);
                    //if (inputIndicators.GetAverageOfMarkedPixels () < 128)
                    if (inputIndicators.GetAverageOfMarkedPixels () < inputIndicators.GetAverageOfNotMarkedPixels ())
                        for (unsigned int j = 0; j < size; j++)
                            currentCanalData[j] = 255 - currentCanalData[j];
                }


                if (parameters.canalsMixingMode == WEIGHTED_MEAN_BY_COLOURS_DIFFERENCES){
                    //multipliers[canal] = indicators.GetAverageOfMarkedPixels () - indicators.GetAverageOfImagePixels ();
                    multipliers[canal] = indicators.GetAverageOfMarkedPixels () - indicators.GetAverageOfNotMarkedPixels ();
                    multipliers[canal] = (multipliers[canal] * multipliers[canal] * multipliers[canal] * multipliers[canal]) / 65025;
                } else if (parameters.canalsMixingMode == WEIGHTED_MEAN_BY_BINARY_HISTOGRAM_COMPARISON){
                    GrayScaleImageHistogram inputHistogram (currentCanalData, size);
                    GrayScaleImageHistogram markedPointsHistogram (currentCanalData, markedPixels);
                    multipliers[canal] = 256 - GrayScaleImageHistogram::BinaryCompare (inputHistogram, markedPointsHistogram);
                    multipliers[canal] = (multipliers[canal] * multipliers[canal] * multipliers[canal] * multipliers[canal]) / 65025;
                } else if (parameters.canalsMixingMode == WEIGHTED_MEAN_BY_PROPORTIONAL_HISTOGRAM_COMPARISON){
                    GrayScaleImageHistogram inputHistogram (currentCanalData, size);
                    GrayScaleImageHistogram markedPointsHistogram (currentCanalData, markedPixels);
                    multipliers[canal] = GrayScaleImageHistogram::AverageDifferenceCompare (inputHistogram, markedPointsHistogram);
                }


                if (parameters.imageTransformationMode == MARKED_PIXELS_HISTOGRAM_BUBBLE_BRIGHTENING){
                    GrayScaleImageHistogram markedPointsHistogram (currentCanalData, markedPixels);
                    markedPointsHistogram.BubbleBrightening (currentCanalData, size);
                } else if (parameters.imageTransformationMode == MARKED_PIXELS_MAXIMAL_VALUE_BRIGHTENING){
                    /**
                      * Shifts all pixels to make the lightest marked point lightness value equal 255
                      */
                    unsigned char shift = 255 - indicators.GetMaximalValueOfMarkedPixels ();
                    if (shift > 0)
                        for (unsigned int j = 0; j < size; j++)
                            currentCanalData[j] = indicators.RangeSafeValue (currentCanalData[j] + shift);
                } else if (parameters.imageTransformationMode == MARKED_PIXELS_AVERAGE_PIXEL_VALUE_BRIGHTNENING){
                    /**
                      * Shifts all pixels (by average of value of marked pixels) to have marked area lighter. If values of marked area are higher then 255, set it to 255.
                      * @advantage no holes in marked area
                      * @disadvantage bad behaviour when the lighest pixel is on a border of marker
                      */
                    unsigned char shift = 255 - indicators.GetAverageOfMarkedPixels ();
                    if (shift > 0)
                        for (unsigned int j = 0; j < size; j++)
                            currentCanalData[j] = indicators.RangeSafeValue (currentCanalData[j] + shift);
                } else if (parameters.imageTransformationMode == MARKED_PIXELS_MAXIMAL_AVERAGE_PIXEL_VALUE_BRIGHTENING){
                    /**
                      * Shifts all pixels values to make it marked object lighter (to make the lightest pixel value of the marker equal 255).
                      * @advantage makes dark areas lighter
                      * @disadvantage if there are reflexes on marked object, there will be no improvement
                      * @disadvantage if marker doesn't mark the lightest pixel of object, it will be changed to dark and probably wont be found.
                      * @proposedSolution set shifting that way to have maximal value less then 255 (to make some place for lighter pixels)
                      */
                    unsigned char shift = 255 - indicators.GetMaximalValueOfMarkedPixels ();
                    if (shift > 0)
                        for (unsigned int j = 0; j < size; j++)
                            currentCanalData[j] = indicators.RangeSafeValue (currentCanalData[j] + shift);
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
