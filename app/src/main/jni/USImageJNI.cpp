//
// Created by ljd-pc on 2016/5/3.
//

#include <jni.h>
#include "include/image/utility/ThreadManage.h"
extern "C"{
JNIEXPORT jstring JNICALL
        Java_com_example_ljd_myandroidtcd_USImageJNI_InitALLThreads(JNIEnv *env, jclass type);
}
JNIEXPORT jstring JNICALL
Java_com_example_ljd_myandroidtcd_USImageJNI_InitALLThreads(JNIEnv *env, jclass type) {

    // TODO

    ThreadSpace::InitALLThreads();
    return env->NewStringUTF( "returnValue");
}