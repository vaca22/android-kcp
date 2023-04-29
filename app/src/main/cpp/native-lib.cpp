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


void createSocket() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOGE("create socket error\n");
    } else {
        printf("fuck2\n");
    }
//    struct sockaddr_in sin;
//    memset(&sin, 0, sizeof(sin));
//    sin.sin_family = AF_INET;
//    sin.sin_addr.s_addr = INADDR_ANY;
//    sin.sin_port = htons(14977);
//    bind(sockfd, (struct sockaddr *) &sin, sizeof(sin));
}


[[noreturn]] void beat_heart() {
    while (true) {
        struct sockaddr_in server_sin;
        socklen_t server_sz = sizeof(server_sin);
        char buf[4] = {'a', 'b', 'c', 'd'};
        char buf2[4] = {'a', 'b', 'c', 'd'};


        recvfrom(sockfd, &buf, sizeof(buf) - 1, 0, (struct sockaddr *) &server_sin, &server_sz);
        printf("%s", buf);
    }
}

void beat_heart2() {
    usleep(2000);

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr("192.168.6.14");
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(14971);
    char buf2[]="fuckyou";

    sendto(sockfd, buf2, strlen(buf2), 0,(struct sockaddr *)&dest_addr, sizeof(dest_addr));

}

extern "C" JNIEXPORT jstring JNICALL
Java_com_vaca_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {


    createSocket();
    pthread_t thread1;
    pthread_t thread2;
    char *message1 = "Thread 1";
    pthread_create(&thread1, nullptr, reinterpret_cast<void *(*)(void *)>(beat_heart),
                   (void *) message1);

    char *message2 = "Thread 2";
    pthread_create(&thread2, nullptr, reinterpret_cast<void *(*)(void *)>(beat_heart2),
                   (void *) message2);


    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());


}
