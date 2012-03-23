#include "camerasupport.h"
#include <jni.h>
#include <QDebug>
//THREADING BEGIN
#include <QThread>
//THREADING END


extern JavaVM *m_javaVM;
extern jobject cameraSupportClassPointer;


CameraSupport::CameraSupport(int width, int height){
    this -> width = width;
    this -> height = height;
    pointsNumber = width * height;
    yuvUVPoints = pointsNumber / 2;

    yuv = new unsigned char[3 * pointsNumber / 2];
    rgb = new unsigned char[4 * pointsNumber];

    //THREADING BEGIN
    rgbCalculation = new ThreadsRGBCalculatuion(this, 0, yuvUVPoints / 2);
    //THREADING END

    JNIEnv* env;
    if (m_javaVM -> AttachCurrentThread (&env, NULL) < 0){
        qCritical () << "AttachCurrentThread failed";
        return;
    }

    jclass applicationClass = env -> GetObjectClass (cameraSupportClassPointer);
    if (applicationClass){
        jmethodID starPreviewMethodId = env->GetMethodID (applicationClass, "startPreview", "(II)V");
        env -> CallVoidMethod (cameraSupportClassPointer, starPreviewMethodId, (jint)width, (jint)height);
    }

    m_javaVM -> DetachCurrentThread ();
}


CameraSupport::~CameraSupport(){
    JNIEnv* env;
    if (m_javaVM -> AttachCurrentThread (&env, NULL) < 0){
        qCritical() << "AttachCurrentThread failed";
        return;
    }

    jclass applicationClass = env -> GetObjectClass (cameraSupportClassPointer);

    if (applicationClass){
        jmethodID stopPreviewMethodId = env -> GetMethodID (applicationClass, "stopPreview", "()V");
        env -> CallVoidMethod (cameraSupportClassPointer, stopPreviewMethodId);

    }

    m_javaVM -> DetachCurrentThread ();

    delete []rgb;
    delete []yuv;

    //THREADING BEGIN
    delete rgbCalculation;
    //THREADING END
}


unsigned int CameraSupport::GetWidth (){
    return width;
}


unsigned int CameraSupport::GetHeight (){
    return height;
}


//THREADING BEGIN
void CameraSupport::ThreadsRGBCalculatuion::run (){
    cameraSupport -> convertYUVToRGB (startValue, stopValue);
}


CameraSupport::ThreadsRGBCalculatuion::ThreadsRGBCalculatuion (CameraSupport* cameraSupport, unsigned int startValue, unsigned int stopValue){
    this -> cameraSupport = cameraSupport;
    this -> startValue = startValue;
    this -> stopValue = stopValue;
}


inline void CameraSupport::convertYUVToRGB (unsigned int start, unsigned int stop){
    unsigned int vIndex, uIndex, pixelStart, pixel, leftTop;
    int a, b, c, d, e, f;

    for (unsigned int i = start; i < stop; i += 2){
        vIndex = pointsNumber + i;
        uIndex = vIndex + 1;
        b = yuv[uIndex] - 128;
        c = yuv[vIndex] - 128;

        d = 409 * c + 128;
        e = 128 - 100 * b - 208 * c;
        f = 516 * b + 128;

        leftTop = (i / width) * width + i;

        pixel = leftTop;
        pixelStart = pixel * 3;

        a = 298 * (yuv[pixel] - 16);

        rgb[pixelStart] = clamp ((a + d) >> 8);
        rgb[pixelStart + 1] = clamp ((a + e) >> 8);
        rgb[pixelStart + 2] = clamp ((a + f) >> 8);

        a = 298 * (yuv[pixel + 1] - 16);
        rgb[pixelStart + 3] = clamp ((a + d) >> 8);
        rgb[pixelStart + 4] = clamp ((a + e) >> 8);
        rgb[pixelStart + 5] = clamp ((a + f) >> 8);

        pixel = leftTop + width;
        pixelStart = pixel * 3;

        a = 298 * (yuv[pixel] - 16);
        rgb[pixelStart] = clamp ((a + d) >> 8);
        rgb[pixelStart + 1] = clamp ((a + e) >> 8);
        rgb[pixelStart + 2] = clamp ((a + f) >> 8);

        a = 298 * (yuv[pixel + 1] - 16);
        rgb[pixelStart + 3] = clamp ((a + d) >> 8);
        rgb[pixelStart + 4] = clamp ((a + e) >> 8);
        rgb[pixelStart + 5] = clamp ((a + f) >> 8);
    }
}
//THREADING END


inline void CameraSupport::convertYUVToRGB (){
    //THREADING BEGIN
    unsigned int start = yuvUVPoints / 2;
    rgbCalculation -> start ();
    convertYUVToRGB (start, yuvUVPoints);
    rgbCalculation -> wait ();
    //THREADING END

    /*unsigned int vIndex, uIndex, pixelStart, pixel, leftTop;
    int a, b, c, d, e, f;

    for (unsigned int i = 0; i < yuvUVPoints; i += 2){
        vIndex = pointsNumber + i;
        uIndex = vIndex + 1;
        b = yuv[uIndex] - 128;
        c = yuv[vIndex] - 128;

        d = 409 * c + 128;
        e = 128 - 100 * b - 208 * c;
        f = 516 * b + 128;

        leftTop = (i / width) * width + i;

        pixel = leftTop;
        pixelStart = pixel * 3;

        a = 298 * (yuv[pixel] - 16);

        rgb[pixelStart] = clamp ((a + d) >> 8);
        rgb[pixelStart + 1] = clamp ((a + e) >> 8);
        rgb[pixelStart + 2] = clamp ((a + f) >> 8);

        a = 298 * (yuv[pixel + 1] - 16);
        rgb[pixelStart + 3] = clamp ((a + d) >> 8);
        rgb[pixelStart + 4] = clamp ((a + e) >> 8);
        rgb[pixelStart + 5] = clamp ((a + f) >> 8);

        pixel = leftTop + width;
        pixelStart = pixel * 3;

        a = 298 * (yuv[pixel] - 16);
        rgb[pixelStart] = clamp ((a + d) >> 8);
        rgb[pixelStart + 1] = clamp ((a + e) >> 8);
        rgb[pixelStart + 2] = clamp ((a + f) >> 8);

        a = 298 * (yuv[pixel + 1] - 16);
        rgb[pixelStart + 3] = clamp ((a + d) >> 8);
        rgb[pixelStart + 4] = clamp ((a + e) >> 8);
        rgb[pixelStart + 5] = clamp ((a + f) >> 8);
    }*/
}


inline unsigned char CameraSupport::clamp(int value){
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}


bool CameraSupport::UpdateFrame(){
    bool frameReaded = false;

    JNIEnv* env;
    if (m_javaVM -> AttachCurrentThread (&env, NULL) < 0){
        qCritical() << "AttachCurrentThread failed";
        return false;
    }

    jclass applicationClass = env -> GetObjectClass (cameraSupportClassPointer);

    if (applicationClass){
        jmethodID getFrameID = env -> GetMethodID (applicationClass, "getFrame", "()[B");
        jobject frameObject = env -> CallObjectMethod (cameraSupportClassPointer, getFrameID);

        if (frameObject){
            jbyteArray *frameByteArray = reinterpret_cast<jbyteArray*>(&frameObject);
            jsize frameByteArrayLength = env -> GetArrayLength (*frameByteArray);
            env -> GetByteArrayRegion (*frameByteArray, (jsize)0, frameByteArrayLength, (jbyte*)yuv);
            jbyte *frameBytePointer = env->GetByteArrayElements (*frameByteArray, NULL);
            env -> ReleaseByteArrayElements (*frameByteArray, frameBytePointer, 0);

            frameReaded = true;
        }
    }

    m_javaVM -> DetachCurrentThread ();

    if (frameReaded)
        convertYUVToRGB ();
    return frameReaded;
}


const unsigned char *CameraSupport::GetRGB (){
    return rgb;
}
