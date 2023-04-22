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
#include "cJSON/cJSON.h"
#include "turn.h"

#define TAG "tcomm"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

static int sockfd;


void createSocket() {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOGE("create socket error\n");
    } else {
        LOGE("fuck2\n");
    }
}


void connectSocket(char *ipaddr, int port) {
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipaddr, &servaddr.sin_addr) <= 0) {
        LOGE("inet_pton error for\n");
        return;
    }
    LOGV("connect...");
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        LOGE("connect error\n");
        return;
    }
    LOGV("connect succeed!!!");
}


int receiveData(int n) {
    if (!sockfd) {
        LOGE("socket is failed!");
        return 0;
    }
    jbyte buf[n];
    int rec_len;
    if ((rec_len = (int) recv(sockfd, buf, sizeof(buf), 0)) == -1) {
        LOGE("recv error");
        return 0;
    } else {
        LOGE("fuck  %s", buf);
    }
    return rec_len;
}

void sendData(char *data) {
    if (!sockfd) {
        printf("socket is failed! \n");
        return;
    }

    if (send(sockfd, data, strlen(data), 0) < 0) {

    }
}

void closeSocket() {
    if (!sockfd)
        return;
//    shutdown(sockfd,0);
    close(sockfd);
}


JavaVM *g_VM;
jobject g_obj;

void sendDataCallback(Juice_thing *juiceThing) {
    JNIEnv *env;

    bool mNeedDetach;
    //获取当前native线程是否有没有被附加到jvm环境中
    int getEnvStat = (*g_VM).GetEnv( (void **) &env,JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        //如果没有， 主动附加到jvm环境中，获取到env
        if ((*g_VM).AttachCurrentThread( &env, NULL) != 0) {
            return;
        }
        mNeedDetach = JNI_TRUE;
    }
    cJSON *monitor = cJSON_CreateObject();
    cJSON *id = cJSON_CreateString("15769415445");
    cJSON_AddItemToObject(monitor, "id", id);
    cJSON *toid = cJSON_CreateString("13207759669");
    cJSON_AddItemToObject(monitor, "toid", toid);
    cJSON *action = cJSON_CreateString(juiceThing->type);
    cJSON_AddItemToObject(monitor, "action", action);
    cJSON *content = cJSON_CreateString(juiceThing->content);
    cJSON_AddItemToObject(monitor, "content", content);
    char *string = cJSON_Print(monitor);
    jclass javaClass = (*env).GetObjectClass( g_obj);
    if (javaClass == nullptr) {
        (*g_VM).DetachCurrentThread();
        return;
    }
    jmethodID javaCallbackId = (*env).GetMethodID( javaClass,
                                                   "fuck", "(Ljava/lang/String;)V");
    if (javaCallbackId == nullptr) {
        return;
    }

    env->CallVoidMethod(g_obj, javaCallbackId, env->NewStringUTF(string));
    cJSON_Delete(monitor);
    if(mNeedDetach) {
        (*g_VM).DetachCurrentThread();
    }
    env = nullptr;
}

void fuck2() {
    test_turn();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_vaca_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject obj) {
    test_turn();
    std::string hello = "Hello frffom C++";
    return env->NewStringUTF(hello.c_str());
}

void process_message(char *message) {
//    cJSON *root = cJSON_Parse(message);
//    if (root != nullptr) {
//        char *n1 = cJSON_GetStringValue(cJSON_GetObjectItem(root, "action"));
//        char *n2 = cJSON_GetStringValue(cJSON_GetObjectItem(root, "content"));
//        LOGE("action:%s\n", n1);
//        LOGE("content:%s\n", n2);
//        if (strcmp(n1, "candidate") == 0) {
//            add_remote_candidate(n2);
//        } else if (strcmp(n1, "ice") == 0) {
//            set_remote_description(n2);
//        } else if (strcmp(n1, "done") == 0) {
//            done_gather();
//        }
//
//        cJSON_Delete(root);
//    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_vaca_myapplication_MainActivity_runAction(JNIEnv *env, jobject thiz, jbyteArray action) {
    signed char *arrayChem = env->GetByteArrayElements(action, NULL);
    process_message(reinterpret_cast<char *>(arrayChem));


    env->ReleaseByteArrayElements(action, arrayChem, 0);
}