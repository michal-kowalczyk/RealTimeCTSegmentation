#ifndef Types_h
#define Types_h

#include <cmath>

namespace LibTIM
{
//Machine dependant typedefs

typedef unsigned char U8;
typedef signed char S8;

typedef unsigned short U16;
typedef signed short S16;

typedef unsigned long  U32;
typedef signed long    S32;

//typedef float F32;
//typedef double F64;

//Table
template <class T, int N>
struct Table{
	Table(){};
	Table(const Table &v)
		{
		for(int i=0; i<N; i++) el[i]=v.el[i];
		}
	Table(int p) {for(int i=0; i<N; i++) el[i]=p;}
	Table(int *vect) {for(int i=0; i<N; i++) el[i]=vect[i];}

	T el[N];
    T &operator[](int i)
	{return el[i];}
    T operator[](int i) const {return el[i];}

	};

//Type of RGB point
typedef Table<U8,3> RGB;


template <class T, int N>
inline bool operator==(Table<T,N> &t1, Table<T,N> &t2)
{
    for(int i=0; i<N; i++)
        {
        if(t1[i]!=t2[i]) return false;
        }
    return true;
}


template <class T, int N>
inline bool operator!=(Table<T,N> &t1, Table<T,N> &t2)
{

    return !(t1==t2);
}

template <class T, int N>
inline bool operator<=(Table<T,N> &t1, Table<T,N> &t2)
{
    for(int i=0; i<N; i++)
        {
        if(t1[i]>t2[i]) return false;
        }
    return true;
}


template <class T, int N>
inline bool operator<(Table<T,N> &t1, Table<T,N> &t2)
{
    if(t1==t2) return false;
    for(int i=0; i<N; i++)
        {
        if(t1[i]>t2[i]) return false;
        }
    return true;
}


template <class T, int N>
inline bool operator>=(Table<T,N> &t1, Table<T,N> &t2)
{
    for(int i=0; i<N; i++)
        {
        if(t1[i]<t2[i]) return false;
        }
    return true;
}


template <class T, int N>
inline bool operator>(Table<T,N> &t1, Table<T,N> &t2)
{
    if(t1==t2) return false;
    for(int i=0; i<N; i++)
        {
        if(t1[i]<t2[i]) return false;
        }
    return true;
}


template <int N>
inline double operator-(Table<U8,N> &t1, Table<U8,N> &t2)
{
    double res=0.0;
    double sum=0.0;
    for(int i=0; i<N; i++)
        {
        sum+=(t1[i]-t2[i])*(t1[i]-t2[i]);
        }
    return sqrt(sum);
}


//Type of image size
typedef unsigned short TSize;

//Type of point spacing
typedef double TSpacing;

//Type of points coordinates
typedef int TCoord;

//Type of label
typedef unsigned long TLabel;

//Type of offset
typedef  long TOffset;

const float FLOAT_EPSILON=0.0000000001f;
}

#endif
