/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#ifndef SUBIMAGE_H
#define SUBIMAGE_H


/**
  Use this to add to class Qt convenient methods. If you aren't using Qt, you can delete this line
  */
#define USE_CONVENIENT_QT_INTERFACE


#ifdef USE_CONVENIENT_QT_INTERFACE
class QImage;
#endif

#include <iterator>

class Subimage{
public:
    Subimage (unsigned int size);
    Subimage (const Subimage& subimage);
    ~Subimage ();
    inline unsigned int& operator[] (unsigned int i){
        return pixelsCoordinates[i];
    }
    inline unsigned int Size (){
        return pixelNumber;
    }

    class PixelsPositionsIterator : public std::iterator<std::input_iterator_tag, unsigned int>{
        unsigned int* pointer;
    public:
        PixelsPositionsIterator (unsigned int* pointer){
            this -> pointer = pointer;
        }
        inline PixelsPositionsIterator& operator++ (){
            ++pointer;
            return *this;
        }
        inline bool operator== (const PixelsPositionsIterator& ppi){
            return pointer == ppi.pointer;
        }
        inline bool operator!= (const PixelsPositionsIterator& ppi){
            return pointer != ppi.pointer;
        }
        inline unsigned int& operator* (){
            return *pointer;
        }
    };

    inline PixelsPositionsIterator begin (){
        return beginIterator;
    }

    inline PixelsPositionsIterator end (){
        return endIterator;
    }

#ifdef USE_CONVENIENT_QT_INTERFACE
    void Update (QImage& subimage);
#endif

protected:
    unsigned int inputSize;

    unsigned int* pixelsCoordinates;
    unsigned int pixelNumber;

    PixelsPositionsIterator beginIterator;
    PixelsPositionsIterator endIterator;
};

#endif // SUBIMAGE_H
