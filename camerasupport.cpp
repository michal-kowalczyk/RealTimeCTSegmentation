#include "camerasupport.h"
#include <jni.h>
#include <QDebug>
#include <QThread>


extern JavaVM *m_javaVM;
extern jobject cameraSupportClassPointer;


CameraSupport::CameraSupport(int width, int height){
    this -> width = width;
    this -> height = height;
    pointsNumber = width * height;
    yuvUVPoints = pointsNumber / 2;

    yuv = new unsigned char[3 * yuvUVPoints];
    rgba = new unsigned int[pointsNumber];

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

    converter = new YUVToRGBAConverter(rgba, yuv, width, pointsNumber);

    threadsNumber = QThread::idealThreadCount ();
    if (threadsNumber < 2)
        threadsNumber = 1;
    else{
        converter -> SetIterationsPerConversion (yuvUVPoints / 2);

        mutex = new QMutex ();

        threads = (ThreadsRGBCalculatuion**)malloc (sizeof(ThreadsRGBCalculatuion) * (threadsNumber - 1));
        for (unsigned int i = 0; i < threadsNumber - 1; i++)
            threads[i] = new ThreadsRGBCalculatuion (converter, mutex);
    }
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

    if (threadsNumber > 1){
        for (unsigned int i = 0; i < threadsNumber - 1; i++)
            delete threads[i];
        free (threads);

        delete mutex;
    }

    delete converter;

    delete []rgba;
    delete []yuv;
}


unsigned int CameraSupport::GetWidth (){
    return width;
}


unsigned int CameraSupport::GetHeight (){
    return height;
}


const unsigned int *CameraSupport::GetRGBA (){
    return rgba;
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
        convertYUVToRGBA ();
    return frameReaded;
}


inline void CameraSupport::convertYUVToRGBA (){
    converter -> ResetCurrentIteration ();
    if (threadsNumber > 1){
        for (unsigned int i = 1; i < threadsNumber; i++)
            threads[i - 1] -> start ();

        int lastStart = -1;
        while (true){
            mutex -> lock ();
            CameraSupport::YUVToRGBAConverter::Coordinates coordinates = converter -> GetNextCoorinates();
            if (lastStart < (int)coordinates.start && coordinates.start < coordinates.stop)
                lastStart = coordinates.start;
            else{
                mutex -> unlock ();
                break;
            }
            mutex -> unlock ();
            converter -> ConvertYUVToRGBA (coordinates);
        }

        for (unsigned int i = 1; i < threadsNumber; i++)
            threads[i - 1] -> wait ();
    } else{
        CameraSupport::YUVToRGBAConverter::Coordinates coordinates;
        coordinates.start = 0;
        coordinates.stop = yuvUVPoints;
        converter -> ConvertYUVToRGBA (coordinates);
    }
}


CameraSupport::ThreadsRGBCalculatuion::ThreadsRGBCalculatuion (CameraSupport::YUVToRGBAConverter *converter, QMutex *mutex){
    this -> converter = converter;
    this -> mutex = mutex;
}


void CameraSupport::ThreadsRGBCalculatuion::run (){
    int lastStart = -1;
    while (true){
        mutex -> lock ();
        CameraSupport::YUVToRGBAConverter::Coordinates coordinates = converter -> GetNextCoorinates();
        if (lastStart < (int)coordinates.start && coordinates.start < coordinates.stop)
            lastStart = coordinates.start;
        else{
            mutex -> unlock ();
            break;
        }
        mutex -> unlock ();
        converter -> ConvertYUVToRGBA (coordinates);
    }
}


CameraSupport::YUVToRGBAConverter::YUVToRGBAConverter (unsigned int *rgba, unsigned char *yuv, unsigned int width, unsigned int pointsNumber){
    this -> rgba = rgba;
    this -> yuv = yuv;
    this -> width = width;
    this -> pointsNumber = pointsNumber;
    this -> currentIteration = 0;
    iterationsNumber = pointsNumber / 2; // the same number as UV points!
    SetIterationsPerConversion (iterationsNumber);
}


inline unsigned int CameraSupport::YUVToRGBAConverter::clamp (int value){
    if (value < 255){
        if (value > 0)
            return value;
        return 0;
    }
    return 255;
}


inline void CameraSupport::YUVToRGBAConverter::ConvertYUVToRGBA (Coordinates coordinates){
    unsigned int vIndex, uIndex, pixel;
    int b, c;

    /*int a, d, e, f;
    for (unsigned int i = coordinates.start; i < coordinates.stop; i += 2){
        vIndex = pointsNumber + i;
        uIndex = vIndex + 1;
        c = yuv[uIndex] - 128;
        b = yuv[vIndex] - 128;

        d = 409 * c + 128;
        e = 128 - 100 * b - 208 * c;
        f = 516 * b + 128;

        pixel = (i / width) * width + i;

        a = 298 * (yuv[pixel] - 16);

        rgba[pixel] = clamp((a + d) >> 8) + ((clamp ((a + e) >> 8) + ((clamp ((a + f) >> 8) + 65280) << 8)) << 8);

        a = 298 * (yuv[pixel + 1] - 16);
        rgba[pixel + 1] = clamp((a + d) >> 8) + ((clamp ((a + e) >> 8) + ((clamp ((a + f) >> 8) + 65280) << 8)) << 8);

        pixel += width;

        a = 298 * (yuv[pixel] - 16);
        rgba[pixel] = clamp((a + d) >> 8) + ((clamp ((a + e) >> 8) + ((clamp ((a + f) >> 8) + 65280) << 8)) << 8);

        a = 298 * (yuv[pixel + 1] - 16);
        rgba[pixel + 1] = clamp((a + d) >> 8) + ((clamp ((a + e) >> 8) + ((clamp ((a + f) >> 8) + 65280) << 8)) << 8);
    }*/

    int y;
    int R, G, B;
    for (unsigned int i = coordinates.start; i < coordinates.stop; i += 2){
        vIndex = pointsNumber + i;
        uIndex = vIndex + 1;
        b = (unsigned char)yuv[uIndex] - 128;
        c = (unsigned char)yuv[vIndex] - 128;

        pixel = (i / width) * width + i;

        y = yuv[pixel];
        R = (y + b + (b >> 2) + (b >> 3) + (b >> 5));
        G = (y - ((c >> 2) + (c >> 4) + (c >> 5)) - ((b >> 1) + (b >> 3) + (b >> 4) + (b >> 5)));
        B = (y + c + (c >> 1) + (c >> 2) + (c >> 6));
        rgba[pixel] =
                (R < 255 ? (R > 0 ? R : 0) : 255) +
                ((G < 255 ? (G > 0 ? G : 0) : 255) << 8) +
                ((B < 255 ? (B > 0 ? B : 0) : 255) << 16) +
                (255 << 24);
        y = yuv[pixel + 1];
        R = (y + b + (b >> 2) + (b >> 3) + (b >> 5));
        G = (y - ((c >> 2) + (c >> 4) + (c >> 5)) - ((b >> 1) + (b >> 3) + (b >> 4) + (b >> 5)));
        B = (y + c + (c >> 1) + (c >> 2) + (c >> 6));
        rgba[pixel + 1] =
                (R < 255 ? (R > 0 ? R : 0) : 255) +
                ((G < 255 ? (G > 0 ? G : 0) : 255) << 8) +
                ((B < 255 ? (B > 0 ? B : 0) : 255) << 16) +
                (255 << 24);

        pixel += width;

        y = yuv[pixel];
        R = (y + b + (b >> 2) + (b >> 3) + (b >> 5));
        G = (y - ((c >> 2) + (c >> 4) + (c >> 5)) - ((b >> 1) + (b >> 3) + (b >> 4) + (b >> 5)));
        B = (y + c + (c >> 1) + (c >> 2) + (c >> 6));
        rgba[pixel] =
                (R < 255 ? (R > 0 ? R : 0) : 255) +
                ((G < 255 ? (G > 0 ? G : 0) : 255) << 8) +
                ((B < 255 ? (B > 0 ? B : 0) : 255) << 16) +
                (255 << 24);

        y = yuv[pixel + 1];
        R = (y + b + (b >> 2) + (b >> 3) + (b >> 5));
        G = (y - ((c >> 2) + (c >> 4) + (c >> 5)) - ((b >> 1) + (b >> 3) + (b >> 4) + (b >> 5)));
        B = (y + c + (c >> 1) + (c >> 2) + (c >> 6));
        rgba[pixel + 1] =
                (R < 255 ? (R > 0 ? R : 0) : 255) +
                ((G < 255 ? (G > 0 ? G : 0) : 255) << 8) +
                ((B < 255 ? (B > 0 ? B : 0) : 255) << 16) +
                (255 << 24);
//        rgba[pixel + 1] =
//                (y + b + (b >> 2) + (b >> 3) + (b >> 5)) +
//                ((y - ((c >> 2) + (c >> 4) + (c >> 5)) - ((b >> 1) + (b >> 3) + (b >> 4) + (b >> 5))) << 8) +
//                ((y + c + (c >> 1) + (c >> 2) + (c >> 6)) << 16) +
//                (255 << 24);
    }
}


void CameraSupport::YUVToRGBAConverter::SetIterationsPerConversion (unsigned int iterationsPerConversion){
    this -> iterationsPerConversion = iterationsPerConversion;
}


CameraSupport::YUVToRGBAConverter::Coordinates CameraSupport::YUVToRGBAConverter::GetNextCoorinates (){
    Coordinates result;
    result.start = currentIteration;
    currentIteration += iterationsPerConversion;
    if (currentIteration > iterationsNumber){
        currentIteration = iterationsNumber;
        result.stop = iterationsNumber;
    } else
        result.stop = currentIteration;
    return result;
}

void CameraSupport::YUVToRGBAConverter::ResetCurrentIteration (){
    currentIteration = 0;
}
