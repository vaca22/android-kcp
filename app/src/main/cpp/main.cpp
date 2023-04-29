#include <jni.h>
#include <string>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <android/log.h>
#include <unistd.h>
#include <pthread.h>

#define TAG "tcomm"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define printf(...) __android_log_print(ANDROID_LOG_ERROR, "vaca", __VA_ARGS__);
static int sockfd;

JavaVM *g_VM;
jobject g_obj;


void createSocket() {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int set = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &set, sizeof(int));
    //设定指定监听的端口
    struct sockaddr_in recvAddr;
    memset(&recvAddr, 0, sizeof(struct sockaddr_in));
    recvAddr.sin_family = AF_INET;//IPV4
    recvAddr.sin_port = htons(14972);//监听的本地端口
    recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);//本地ip
    // 必须绑定，否则无法监听
    if(bind(sockfd, (struct sockaddr *)&recvAddr, sizeof(struct sockaddr)) == -1){
        printf("bind fail\n");
    }else{
        printf("bind success\n");
    }
}


[[noreturn]] void beat_heart() {
    JNIEnv *env;
    bool mNeedDetach;
    int getEnvStat = (*g_VM).GetEnv( (void **) &env,JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        if ((*g_VM).AttachCurrentThread( &env, NULL) != 0) {
            return ;
        }
        mNeedDetach = JNI_TRUE;
    }

    jclass javaClass = (*env).GetObjectClass( g_obj);
    if (javaClass == nullptr) {
        (*g_VM).DetachCurrentThread();
        return ;
    }
    jmethodID javaCallbackId = (*env).GetMethodID( javaClass,"recvData", "(Ljava/lang/String;)V");
    if (javaCallbackId == nullptr) {
        return ;
    }

    while (true) {
        struct sockaddr_in server_sin;
        socklen_t server_sz = sizeof(server_sin);
        char buf[40];
        int len=recvfrom(sockfd, &buf, sizeof(buf) - 1, 0, (struct sockaddr *) &server_sin, &server_sz);
        printf("%s", buf);
        buf[len]=0;
        env->CallVoidMethod(g_obj, javaCallbackId, env->NewStringUTF(buf));
    }
}

void send_ping() {
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr("1.14.135.210");
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(14971);
    char buf2[] = "wsmslgh";
    sendto(sockfd, buf2, strlen(buf2), 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
}

extern "C" JNIEXPORT void JNICALL
Java_com_vaca_myapplication_MainActivity_initUdp(
        JNIEnv *env,
        jobject obj) {

    (*env).GetJavaVM( &g_VM);
    g_obj = (*env).NewGlobalRef( obj);
    createSocket();
    pthread_t thread1;
    pthread_t thread2;
    char *message1 = "Thread 1";
    pthread_create(&thread1, nullptr, reinterpret_cast<void *(*)(void *)>(beat_heart),
                   (void *) message1);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_vaca_myapplication_MainActivity_sendUdp(JNIEnv *env, jobject thiz) {
    send_ping();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_vaca_myapplication_MainActivity_beginStun(JNIEnv *env, jobject thiz, jstring ip,
                                                   jint port) {
    char *ip2 = (char *) (*env).GetStringUTFChars( ip, nullptr);
    int port2 = port;
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(ip2);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port2);
    char buf2[] = "wsmslgh";
    sendto(sockfd, buf2, strlen(buf2), 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
    (*env).ReleaseStringUTFChars( ip, ip2);
}