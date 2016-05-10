//
// Created by ljd-pc on 2016/5/3.
//

#include <jni.h>
#include "include/image/utility/ThreadManage.h"
#include "include/image/mode/MultiDeepModeTCDThread.h"
#include "include/image/utility/ImageDef.h"
#include "include/image/utility/DebugPrint.h"

extern "C"{
JNIEXPORT jstring JNICALL
        Java_com_example_ljd_myandroidtcd_USImageJNI_InitALLThreads(JNIEnv *env, jclass type);
JNIEXPORT jintArray JNICALL
        Java_com_example_ljd_myandroidtcd_USImageJNI_GetMultiDeepModeTCDARGBArray(JNIEnv *env,
                                                                                  jclass type);
JNIEXPORT jint JNICALL
        Java_com_example_ljd_myandroidtcd_USImageJNI_GetDeepPoint(JNIEnv *env, jclass type);
}
JNIEXPORT jstring JNICALL
Java_com_example_ljd_myandroidtcd_USImageJNI_InitALLThreads(JNIEnv *env, jclass type) {

    // TODO

    ThreadSpace::InitALLThreads();
    return env->NewStringUTF( "returnValue");
}

JNIEXPORT jintArray JNICALL
Java_com_example_ljd_myandroidtcd_USImageJNI_GetMultiDeepModeTCDARGBArray(JNIEnv *env,
                                                                          jclass type) {
    int* C_MultiDeepModeTCDARGBArray = NULL;
    //int buff[DEEP_POINTS];
    C_MultiDeepModeTCDARGBArray = GetMultiDeepModeTCDFrameBuffer(C_MultiDeepModeTCDARGBArray);

    //LOGI("C_MultiDeepModeTCDARGBArray[5] = %d",C_MultiDeepModeTCDARGBArray[5]);
    jintArray JAVA_MultiDeepModeTCDARGBArray = env->NewIntArray(DEEP_POINTS);
    //LOGI("Java_com_example_ljd_myandroidtcd_USImageJNI_GetMultiDeepModeTCDARGBArray: GetMultiDeepModeTCDFrameBuffer done");
    env->SetIntArrayRegion(JAVA_MultiDeepModeTCDARGBArray,0,DEEP_POINTS,C_MultiDeepModeTCDARGBArray);
    return JAVA_MultiDeepModeTCDARGBArray;

}

JNIEXPORT jint JNICALL
Java_com_example_ljd_myandroidtcd_USImageJNI_GetDeepPoint(JNIEnv *env, jclass type) {

    return NativeGetDeepPoint();

}