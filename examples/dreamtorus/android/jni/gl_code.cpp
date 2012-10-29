/*

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

*/

#include <jni.h>
#include <android/log.h>

#include <GL/Regal.h>

#include "render.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

#define  LOG_TAG    "dreamtorus"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C"
{
  JNIEXPORT void JNICALL Java_com_regal_dreamtorus_DreamtorusLib_init(JNIEnv *env, jobject obj,  jint width, jint height);
  JNIEXPORT void JNICALL Java_com_regal_dreamtorus_DreamtorusLib_step(JNIEnv *env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_regal_dreamtorus_DreamtorusLib_init(JNIEnv *env, jobject obj,  jint width, jint height)
{
  // On Android there is a new context each time the application
  // is activated - we ought to clean up the Regal resources for
  // the old context, but there is no API for that yet.

  RegalMakeCurrent(eglGetCurrentContext());
  dreamTorusInit();
  dreamTorusReshape(width, height);
}

JNIEXPORT void JNICALL Java_com_regal_dreamtorus_DreamtorusLib_step(JNIEnv *env, jobject obj)
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  dreamTorusDisplay( false );
}
