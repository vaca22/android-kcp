#include <jni.h>
#include <string>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <android/log.h>
#include <unistd.h>
#include "ikcp.h"

#define TAG "gaga"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)


int udp_output(const char *buf, int len, ikcpcb *kcp, void *user) {
    union {
        int id;
        void *ptr;
    } parameter;
    parameter.ptr = user;

    LOGE("%d", len);

    return 0;
}




ikcpcb *kcp1;


extern "C"
JNIEXPORT jstring JNICALL
Java_com_vaca_myapplication_MainActivity_initKcp(JNIEnv *env, jobject thiz) {
    LOGE("fuck\n");
    kcp1 = ikcp_create(0x11223344, (void *) 0);
    kcp1->output = udp_output;
    ikcp_wndsize(kcp1, 128, 128);
    ikcp_nodelay(kcp1, 2, 10, 2, 1);
    kcp1->rx_minrto = 10;
    kcp1->fastresend = 1;

    char buffer[200];
    ikcp_send(kcp1, buffer, 200);
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}




extern "C"
JNIEXPORT void JNICALL
Java_com_vaca_myapplication_MainActivity_updateKcp(JNIEnv *env, jobject thiz, jlong t) {
    ikcp_update(kcp1, t);
}