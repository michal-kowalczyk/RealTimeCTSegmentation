#ifndef CAMERASUPPORT_H
#define CAMERASUPPORT_H
#include <QThread>
#include <QMutex>


class CameraSupport{
public:
    CameraSupport (int width, int height);
    ~CameraSupport ();
    unsigned int GetWidth ();
    unsigned int GetHeight ();
    bool UpdateFrame ();
    const unsigned int *GetRGBA ();

protected:
    void convertYUVToRGBA ();

    class YUVToRGBAConverter{
    public:
        struct Coordinates{
            unsigned int start;
            unsigned int stop;
        };

        YUVToRGBAConverter(unsigned int *rgb, unsigned char *yuv, unsigned int width, unsigned int pointsNumber);
        void ConvertYUVToRGBA (Coordinates coordinates);
        void SetIterationsPerConversion (unsigned int iterationsPerConversion);
        Coordinates GetNextCoorinates ();
        void ResetCurrentIteration ();

    private:
        unsigned int clamp(int value);

        unsigned int *rgba;
        unsigned char *yuv;
        unsigned int width;
        unsigned int pointsNumber;
        unsigned int iterationsPerConversion;
        unsigned int currentIteration;
        unsigned int iterationsNumber;
    };

    YUVToRGBAConverter *converter;

    unsigned int width;
    unsigned int height;
    unsigned int pointsNumber;
    unsigned int yuvUVPoints;
    unsigned char *yuv;
    unsigned int *rgba;

    class ThreadsRGBCalculatuion : public QThread{
    public:
        ThreadsRGBCalculatuion(YUVToRGBAConverter* converter, QMutex *mutex);

    private:
        YUVToRGBAConverter* converter;
        QMutex *mutex;
        void run ();
    };

    typedef CameraSupport::ThreadsRGBCalculatuion THREAD;

    unsigned int threadsNumber;
    ThreadsRGBCalculatuion **threads;
    QMutex *mutex;
};

#endif // CAMERASUPPORT_H
