#include "camerasupport.h"
#include <jni.h>
#include <QDebug>
#include <QThread>
#include <QMutex>


extern JavaVM *m_javaVM;
extern jobject cameraSupportClassPointer;


const short R[] = {-180,-179,-178,-177,-175,-174,-173,-172,-169,-168,-167,-166,-164,-163,-162,-161,-158,-157,-156,-155,-153,-152,-151,-150,-147,-146,-145,-144,-142,-141,-140,-139,-135,-134,-133,-132,-130,-129,-128,-127,-124,-123,-122,-121,-119,-118,-117,-116,-113,-112,-111,-110,-108,-107,-106,-105,-102,-101,-100,-99,-97,-96,-95,-94,-90,-89,-88,-87,-85,-84,-83,-82,-79,-78,-77,-76,-74,-73,-72,-71,-68,-67,-66,-65,-63,-62,-61,-60,-57,-56,-55,-54,-52,-51,-50,-49,-45,-44,-43,-42,-40,-39,-38,-37,-34,-33,-32,-31,-29,-28,-27,-26,-23,-22,-21,-20,-18,-17,-16,-15,-12,-11,-10,-9,-7,-6,-5,-4,0,1,2,3,5,6,7,8,11,12,13,14,16,17,18,19,22,23,24,25,27,28,29,30,33,34,35,36,38,39,40,41,45,46,47,48,50,51,52,53,56,57,58,59,61,62,63,64,67,68,69,70,72,73,74,75,78,79,80,81,83,84,85,86,90,91,92,93,95,96,97,98,101,102,103,104,106,107,108,109,112,113,114,115,117,118,119,120,123,124,125,126,128,129,130,131,135,136,137,138,140,141,142,143,146,147,148,149,151,152,153,154,157,158,159,160,162,163,164,165,168,169,170,171,173,174,175};
const short G1[] = {-44,-44,-44,-44,-43,-43,-43,-43,-42,-42,-42,-42,-41,-41,-41,-41,-39,-39,-39,-39,-38,-38,-38,-38,-37,-37,-37,-37,-36,-36,-36,-36,-33,-33,-33,-33,-32,-32,-32,-32,-31,-31,-31,-31,-30,-30,-30,-30,-28,-28,-28,-28,-27,-27,-27,-27,-26,-26,-26,-26,-25,-25,-25,-25,-22,-22,-22,-22,-21,-21,-21,-21,-20,-20,-20,-20,-19,-19,-19,-19,-17,-17,-17,-17,-16,-16,-16,-16,-15,-15,-15,-15,-14,-14,-14,-14,-11,-11,-11,-11,-10,-10,-10,-10,-9,-9,-9,-9,-8,-8,-8,-8,-6,-6,-6,-6,-5,-5,-5,-5,-4,-4,-4,-4,-3,-3,-3,-3,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,11,11,11,11,12,12,12,12,13,13,13,13,14,14,14,14,16,16,16,16,17,17,17,17,18,18,18,18,19,19,19,19,22,22,22,22,23,23,23,23,24,24,24,24,25,25,25,25,27,27,27,27,28,28,28,28,29,29,29,29,30,30,30,30,33,33,33,33,34,34,34,34,35,35,35,35,36,36,36,36,38,38,38,38,39,39,39,39,40,40,40,40,41,41,41};
const short G2[] = {-92,-92,-91,-91,-90,-90,-89,-89,-87,-87,-86,-86,-85,-85,-84,-84,-81,-81,-80,-80,-79,-79,-78,-78,-76,-76,-75,-75,-74,-74,-73,-73,-69,-69,-68,-68,-67,-67,-66,-66,-64,-64,-63,-63,-62,-62,-61,-61,-58,-58,-57,-57,-56,-56,-55,-55,-53,-53,-52,-52,-51,-51,-50,-50,-46,-46,-45,-45,-44,-44,-43,-43,-41,-41,-40,-40,-39,-39,-38,-38,-35,-35,-34,-34,-33,-33,-32,-32,-30,-30,-29,-29,-28,-28,-27,-27,-23,-23,-22,-22,-21,-21,-20,-20,-18,-18,-17,-17,-16,-16,-15,-15,-12,-12,-11,-11,-10,-10,-9,-9,-7,-7,-6,-6,-5,-5,-4,-4,0,0,1,1,2,2,3,3,5,5,6,6,7,7,8,8,11,11,12,12,13,13,14,14,16,16,17,17,18,18,19,19,23,23,24,24,25,25,26,26,28,28,29,29,30,30,31,31,34,34,35,35,36,36,37,37,39,39,40,40,41,41,42,42,46,46,47,47,48,48,49,49,51,51,52,52,53,53,54,54,57,57,58,58,59,59,60,60,62,62,63,63,64,64,65,65,69,69,70,70,71,71,72,72,74,74,75,75,76,76,77,77,80,80,81,81,82,82,83,83,85,85,86,86,87,87,88};
const short B[] = {-226,-225,-223,-222,-219,-218,-216,-215,-212,-211,-209,-208,-205,-204,-202,-201,-198,-197,-195,-194,-191,-190,-188,-187,-184,-183,-181,-180,-177,-176,-174,-173,-170,-169,-167,-166,-163,-162,-160,-159,-156,-155,-153,-152,-149,-148,-146,-145,-142,-141,-139,-138,-135,-134,-132,-131,-128,-127,-125,-124,-121,-120,-118,-117,-113,-112,-110,-109,-106,-105,-103,-102,-99,-98,-96,-95,-92,-91,-89,-88,-85,-84,-82,-81,-78,-77,-75,-74,-71,-70,-68,-67,-64,-63,-61,-60,-57,-56,-54,-53,-50,-49,-47,-46,-43,-42,-40,-39,-36,-35,-33,-32,-29,-28,-26,-25,-22,-21,-19,-18,-15,-14,-12,-11,-8,-7,-5,-4,0,1,3,4,7,8,10,11,14,15,17,18,21,22,24,25,28,29,31,32,35,36,38,39,42,43,45,46,49,50,52,53,56,57,59,60,63,64,66,67,70,71,73,74,77,78,80,81,84,85,87,88,91,92,94,95,98,99,101,102,105,106,108,109,113,114,116,117,120,121,123,124,127,128,130,131,134,135,137,138,141,142,144,145,148,149,151,152,155,156,158,159,162,163,165,166,169,170,172,173,176,177,179,180,183,184,186,187,190,191,193,194,197,198,200,201,204,205,207,208,211,212,214,215,218,219,221};
const unsigned char PIXELVALUE[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};


CameraSupport::CameraSupport(int width, int height){
    this -> width = width;
    this -> height = height;
    pointsNumber = width * height;
    yuvUVPoints = pointsNumber / 2;

    yuv = new unsigned char[3 * yuvUVPoints];
    rgba = new unsigned int[pointsNumber];

    threadsNumber = QThread::idealThreadCount ();
    if (threadsNumber < 2){
        threadsNumber = 1;
        converter = new YUVToRGBAConverter (rgba, yuv, width, pointsNumber, threadsNumber);
    } else{
        converter = new YUVToRGBAConverter (rgba, yuv, width, pointsNumber, threadsNumber);

        mutex = new QMutex ();

        threads = (AdditionalCalculator**)malloc (sizeof(AdditionalCalculator) * (threadsNumber - 1));
        for (unsigned int i = 0; i < threadsNumber - 1; i++)
            threads[i] = new AdditionalCalculator (converter, mutex);
    }

    started = false;

    Start ();
}


CameraSupport::~CameraSupport(){
    Stop ();

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


void CameraSupport::Start (){
    if (!started){
        started = true;

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
    } else
        qDebug () << "Camera already started!";
}


void CameraSupport::Stop (){
    if (started){
        started = false;

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
    } else
        qDebug () << "Camera wasn't started or already stopped!";
}


bool CameraSupport::IsRunning (){
    return started;
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
    converter -> InitializeConversion ();
    if (threadsNumber > 1){
        for (unsigned int i = 1; i < threadsNumber; i++)
            threads[i - 1] -> start ();

        while (true){
            mutex -> lock ();
            CameraSupport::YUVToRGBAConverter::Coordinates coordinates = converter -> GetNextCoorinates();
            mutex -> unlock ();
            if (coordinates.start >= coordinates.stop)
                break;
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


CameraSupport::AdditionalCalculator::AdditionalCalculator (CameraSupport::YUVToRGBAConverter *converter, QMutex *mutex){
    this -> converter = converter;
    this -> mutex = mutex;
}


void CameraSupport::AdditionalCalculator::run (){
    while (true){
        mutex -> lock ();
        CameraSupport::YUVToRGBAConverter::Coordinates coordinates = converter -> GetNextCoorinates();
        mutex -> unlock ();
        if (coordinates.start >= coordinates.stop)
            break;
        converter -> ConvertYUVToRGBA (coordinates);
    }
}


CameraSupport::YUVToRGBAConverter::YUVToRGBAConverter (unsigned int *rgba, unsigned char *yuv, unsigned int width, unsigned int pointsNumber, unsigned int threadsNumber){
    this -> rgba = rgba;
    this -> yuv = yuv;
    this -> width = width;
    this -> pointsNumber = pointsNumber;
    currentIteration = 0;
    iterationsNumber = pointsNumber / 2; // the same number as UV points!
    SetIterationsPerConversion (iterationsNumber / threadsNumber / 4);
    pixelsPosition = new unsigned int[iterationsNumber];
    for (unsigned int i = 0; i < iterationsNumber; i += 2)
        pixelsPosition[i] = (i / width) * width + i;
}


CameraSupport::YUVToRGBAConverter::~YUVToRGBAConverter (){
    delete []pixelsPosition;
}


void CameraSupport::YUVToRGBAConverter::InitializeConversion(){
    currentIteration = 0;
}


inline void CameraSupport::YUVToRGBAConverter::ConvertYUVToRGBA (Coordinates coordinates){
    unsigned int pixel, *rgbaPointer, *pixelPositionPointer = pixelsPosition + coordinates.start;
    unsigned char cR, cB, y, *yPointer, *cBcRPointer;
    short r, g, b;

    cBcRPointer = yuv + pointsNumber + coordinates.start;

    for (unsigned int i = coordinates.start; i < coordinates.stop; i += 2){
        cB = *cBcRPointer;
        cR = *(cBcRPointer + 1);
        cBcRPointer += 2;

        r = R[cR] + 226;
        g = G1[cB] + G2[cR] - 226;
        b = B[cB] + 226;

        pixel = *pixelPositionPointer;
        pixelPositionPointer += 2;

        rgbaPointer = rgba + pixel;
        yPointer = yuv + pixel;

        y = *yPointer;
        *rgbaPointer = PIXELVALUE[y + r] | ((PIXELVALUE[y - g] | ((PIXELVALUE[y + b] | 65280) << 8)) << 8);

        y = *(yPointer + 1);
        *(rgbaPointer + 1) = PIXELVALUE[y + r] | ((PIXELVALUE[y - g] | ((PIXELVALUE[y + b] | 65280) << 8)) << 8);

        rgbaPointer += width;
        yPointer += width;

        y = *yPointer;
        *rgbaPointer = PIXELVALUE[y + r] | ((PIXELVALUE[y - g] | ((PIXELVALUE[y + b] | 65280) << 8)) << 8);

        y = *(yPointer + 1);
        *(rgbaPointer + 1) = PIXELVALUE[y + r] | ((PIXELVALUE[y - g] | ((PIXELVALUE[y + b] | 65280) << 8)) << 8);

    }
}


void CameraSupport::YUVToRGBAConverter::SetIterationsPerConversion (unsigned int iterationsPerConversion){
    this -> iterationsPerConversion = iterationsPerConversion % 2 == 0 ? iterationsPerConversion : iterationsPerConversion + 1;
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
