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


static int sockfd;
ikcpcb *kcp1;
char dest_ip[20];

int udp_output(const char *buf, int len, ikcpcb *kcp, void *user) {
    union {
        int id;
        void *ptr;
    } parameter;
    parameter.ptr = user;

    LOGE("%d", len);

    return 0;
}







extern "C"
JNIEXPORT void JNICALL
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

}




extern "C"
JNIEXPORT void JNICALL
Java_com_vaca_myapplication_MainActivity_updateKcp(JNIEnv *env, jobject thiz, jlong t) {
    ikcp_update(kcp1, t);
}





void sendData(char *data){
    if (!sockfd) {
        printf("socket is failed! \n");
        return;
    }
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(6000);
    if (inet_pton(AF_INET, "192.168.5.101", &client_addr.sin_addr) <= 0) {
        LOGE("inet_pton error for\n");
        return;
    }
    sendto(sockfd,data, strlen(data), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_vaca_myapplication_MainActivity_initUdp(JNIEnv *env, jobject thiz, jbyteArray ip) {
    signed char *array1 = env->GetByteArrayElements(ip, NULL);

    unsigned char x1=array1[0];
    unsigned char x2=array1[1];
    unsigned char x3=array1[2];
    unsigned char x4=array1[3];

    sprintf(dest_ip,"%d.%d.%d.%d",x1,x2,x3,x4);

    LOGE("%s",dest_ip);

    env->ReleaseByteArrayElements(ip, array1, 0);
/*    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOGE("create socket error\n");
    }else{
        LOGE("fuck2\n");
    }*/
}