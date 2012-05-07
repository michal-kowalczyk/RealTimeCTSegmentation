/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#include "histogram.h"

#include "timeseries/src/tsutil.h"

Histogram::Histogram (unsigned int size) :
    size (size),
    data ((unsigned int *)calloc (size, sizeof (unsigned int))),//new unsigned int[size]),
    beginIterator (data),
    endIterator (data + size)
{
/*    unsigned int *dataPointer = data, *dataEndPointer = data + size;
    while (dataPointer < dataEndPointer){
        *dataPointer = 0;
        dataPointer++;
    }*/
}


Histogram::~Histogram (){
    delete []data;
}


Histogram::Histogram (const Histogram& histogram) :
    size (histogram.size),
    data (new unsigned int[size]),
    beginIterator (data),
    endIterator (data + size)
{
    memcpy (data, histogram.data, size * sizeof (unsigned int));
}


unsigned short Histogram::BinaryCompare (Histogram &histogram1, Histogram &histogram2){
    unsigned short result = 0;

    if (histogram1.Size () != histogram2.Size ())
        throw "Histograms have different sizes!";

    for (DataIterator i1 = histogram1.begin(), i2 = histogram2.begin(); i1 != histogram1.end(); ++i1, ++i2)
        if (*i1 && *i2)
            result++;

    return result;
}


/**
  @todo iterators for histograms
  @todo change dtw implementation to use arrays, not vectors
  */
double Histogram::DynamicTimeWarping (Histogram &histogram1, Histogram &histogram2){
/*    std::vector<double> vector1, vector2;

    for (unsigned int i = 0; i < histogram1.Size (); i++)
        vector1.push_back (histogram1[i]);
    for (unsigned int i = 0; i < histogram2.Size (); i++)
        vector2.push_back (histogram2[i]);

    return TimeSeries::dtw (vector1, vector2);*/
}



/**
  The range of result is 0 - 10000
  */
unsigned short Histogram::AverageDifferenceCompare (Histogram &histogram1, Histogram &histogram2){
    unsigned short result = 0;

    if (histogram1.Size () != histogram2.Size ())
        throw "Histograms have different sizes!";

    unsigned int value1, value2;

    for (DataIterator i1 = histogram1.begin(), i2 = histogram2.begin(); i1 != histogram1.end(); ++i1, ++i2){
        value1 = *i1;
        value2 = *i2;
        if (value1 && value2){
            if (value1 > value2)
                result += (value2 * 10000) / value1;
            else
                result += (value1 * 10000) / value2;
        }
    }

    return result / 255;

//    while (histogram1Pointer < histogram1EndPointer){
//        value1 = *histogram1Pointer;
//        value2 = *histogram2Pointer;
//        if (value1 && value2){
//            if (value1 > value2)
//                result += ((value2 * 10000) / (value1 - value2));
//            else
//                result += ((value1 * 10000) / (value2 - value1));
//        }

//        histogram1Pointer++;
//        histogram2Pointer++;
//    }

//    return result / 256;
}
