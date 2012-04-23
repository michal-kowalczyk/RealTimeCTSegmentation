/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

class Histogram{
public:
    Histogram (unsigned int size);
    ~Histogram ();

    inline unsigned int& operator[] (unsigned int i){
        return data[i];
    }
    inline unsigned int Size (){
        return size;
    }

    static unsigned short BinaryCompare (Histogram &histogram1, Histogram &histogram2);
    static double DynamicTimeWarping (Histogram &histogram1, Histogram &histogram2);
    static unsigned short AverageDifferenceCompare (Histogram &histogram1, Histogram &histogram2);

protected:
    unsigned int *data;
    unsigned int size;
};

#endif // HISTOGRAM_H
