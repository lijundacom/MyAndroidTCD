//
// Created by ljd-pc on 2016/5/3.
//

// ------------------------------------------------------------
// Copyright (c) 2011, 深圳开立科技有限公司。All rights reserved.
// Name        	: DebugPrint.h
// Description 	: 定义数据、图像处理时所用到的数据信息
// History	 	: lvmj, 2013-07-23
// ------------------------------------------------------------
#ifndef DEBUG_PRINT_H_
#define DEBUG_PRINT_H_

#include <android/log.h>

//--------打印函数-----------------//

#define LOG_TAG    "pj1210-test"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)


#define LOG(tag,...)  __android_log_print(ANDROID_LOG_INFO, tag, __VA_ARGS__)



#endif				//DEBUG_PRINT_H_

