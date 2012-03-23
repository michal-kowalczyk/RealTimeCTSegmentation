#ifndef CAMERASUPPORT_H
#define CAMERASUPPORT_H
//THREADING BEGIN
#include <QThread>
//THREADING END

class CameraSupport{
public:
    CameraSupport (int width, int height);
    ~CameraSupport ();
    unsigned int GetWidth ();
    unsigned int GetHeight ();
    bool UpdateFrame ();
    const unsigned char *GetRGB ();
    //THREADING BEGIN
    void convertYUVToRGB (unsigned int start, unsigned int stop);
    //THREADING END

protected:
    void convertYUVToRGB ();
    unsigned char clamp(int value);

    //THREADING BEGIN
    class ThreadsRGBCalculatuion : public QThread{
    public:
        ThreadsRGBCalculatuion(CameraSupport* cameraSupport, unsigned int startValue, unsigned int stopValue);
    private:
        CameraSupport* cameraSupport;
        unsigned int startValue;
        unsigned int stopValue;
        void run ();
    };

    ThreadsRGBCalculatuion *rgbCalculation;
    //THREADING END
    unsigned int width;
    unsigned int height;
    unsigned int pointsNumber;
    unsigned int yuvUVPoints;
    unsigned char *yuv;
    unsigned char *rgb;
};

#endif // CAMERASUPPORT_H
