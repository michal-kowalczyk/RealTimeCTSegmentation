/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <iterator>

class Histogram{
public:
    Histogram (unsigned int size);
    ~Histogram ();
    Histogram (const Histogram& histogram);

    inline unsigned int& operator[] (unsigned int i){
        return data[i];
    }
    inline unsigned int Size (){
        return size;
    }

    class DataIterator : public std::iterator<std::input_iterator_tag, unsigned int>{
        unsigned int* pointer;
    public:
        DataIterator (unsigned int* pointer){
            this -> pointer = pointer;
        }
        inline DataIterator& operator++ (){
            ++pointer;
            return *this;
        }
        inline bool operator== (const DataIterator& di){
            return pointer == di.pointer;
        }
        inline bool operator!= (const DataIterator& di){
            return pointer != di.pointer;
        }
        inline unsigned int& operator* (){
            return *pointer;
        }
    };

    inline DataIterator begin (){
        return beginIterator;
    }

    inline DataIterator end (){
        return endIterator;
    }

    static unsigned short BinaryCompare (Histogram &histogram1, Histogram &histogram2);
    static double DynamicTimeWarping (Histogram &histogram1, Histogram &histogram2);
    static unsigned short AverageDifferenceCompare (Histogram &histogram1, Histogram &histogram2);

protected:
    unsigned int size;
    unsigned int *data;

    DataIterator beginIterator;
    DataIterator endIterator;
};

#endif // HISTOGRAM_H
