#ifndef CAMERASUPPORT_H
#define CAMERASUPPORT_H


#include <QThread>
class QMutex;


class CameraSupport{
public:
    CameraSupport (int width, int height);
    ~CameraSupport ();
    void Start ();
    void Stop ();
    bool IsRunning ();
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

        YUVToRGBAConverter (unsigned int *rgba, unsigned char *yuv, unsigned int width, unsigned int pointsNumber, unsigned int threadsNumber);
        ~YUVToRGBAConverter ();
        void InitializeConversion ();
        void ConvertYUVToRGBA (Coordinates coordinates);
        void SetIterationsPerConversion (unsigned int iterationsPerConversion);
        Coordinates GetNextCoorinates ();

    private:
        unsigned int *rgba;
        unsigned char *yuv;
        unsigned int **rgbPixelsPosition;
        unsigned int *pixelsPosition;
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

    class AdditionalCalculator : public QThread{
    public:
        AdditionalCalculator(YUVToRGBAConverter* converter, QMutex *mutex);

    private:
        YUVToRGBAConverter* converter;
        QMutex *mutex;
        void run ();
    };

    unsigned int threadsNumber;
    AdditionalCalculator **threads;
    QMutex *mutex;

private:
    bool started;
};

#endif // CAMERASUPPORT_H
