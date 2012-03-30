#include <jni.h>
#include <android/log.h>

jobject cameraSupportClassPointer;

int JavaClassesLoader (JNIEnv* env){
    {
        const char* className = "pl/ekk/mkk/necessitas/CameraSupport";
        jclass clazz=env->FindClass(className);
        if (!clazz){
            __android_log_print(ANDROID_LOG_FATAL,"Qt", "Camera Support Native registration unable to find class '%s'", className);
            return JNI_FALSE;
        }
        jmethodID constr = env->GetMethodID(clazz, "<init>", "()V");
        if (!constr){
            __android_log_print(ANDROID_LOG_FATAL,"Qt", "Camera Support Native registration unable to find constructor for class '%s'", className);
            return JNI_FALSE;
        }
        jobject obj = env->NewObject(clazz, constr);
        cameraSupportClassPointer = env->NewGlobalRef(obj);
    }
    return JNI_TRUE;
}
