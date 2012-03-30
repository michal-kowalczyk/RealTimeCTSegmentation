#ifndef MEANINGFULSCALES_H
#define MEANINGFULSCALES_H

#include <QImage>
#include <sstream>
#include "ImaGene/dgeometry2d/FreemanChain.h"


class MeaningfulScales
{
public:
    MeaningfulScales(QImage image);
    static void GetPGMStream (std::ostream &stream, QImage image, int channel);
    static void GetFreemanChain (std::ostream &result, QImage image);
    //static uint estimMaxSamplingSize (FreemanChain fc);
    QImage GetResult();

    static const int RED = 1;
    static const int GREEN = 2;
    static const int BLUE = 3;

protected:
    QImage boxes;
};

#endif // MEANINGFULSCALES_H
