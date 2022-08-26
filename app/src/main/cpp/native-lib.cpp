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





void sendData(char *data,int len){
    if (!sockfd) {
        LOGE("socket is failed! \n");
        return;
    }
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(6000);
    if (inet_pton(AF_INET, dest_ip, &client_addr.sin_addr) <= 0) {
        LOGE("inet_pton error for\n");
        return;
    }
    sendto(sockfd,data, len, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
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
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOGE("create socket error\n");
    }else{
        LOGE("fuck2\n");
    }
    env->ReleaseByteArrayElements(ip, array1, 0);
}






extern "C"
JNIEXPORT void JNICALL
Java_com_vaca_myapplication_MainActivity_sendUdp(JNIEnv *env, jobject thiz, jbyteArray data,
                                                 jint len) {

    signed char *array1 = env->GetByteArrayElements(data, NULL);
    sendData(reinterpret_cast<char *>(array1), len);
    env->ReleaseByteArrayElements(data, array1, 0);

}


const char *NATIVE_GREET_CLASS_NAME = "com/vaca/myapplication/MainActivity";



static void say_hello_jni(JNIEnv *env, jobject object, jobject greet_listener);
static JNINativeMethod gMethods_Greet[] = {
        {"native_say_hello", "(Lcom/vaca/myapplication/MainActivity$GreetListener;)V", (void *)say_hello_jni}
};

static int registerNativeMethods(JNIEnv *env)
{
    jclass clazz = env->FindClass(NATIVE_GREET_CLASS_NAME);
    if (clazz == NULL)
        return JNI_FALSE;
    if (env->RegisterNatives(clazz, gMethods_Greet, sizeof(gMethods_Greet) / sizeof(gMethods_Greet[0])) < 0)
        return JNI_FALSE;
    env->DeleteLocalRef(clazz);
    clazz = NULL;
    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK)
        return -1;
    assert(env != NULL);
    if (!registerNativeMethods(env))
        return -1;

//    hover_jvm = vm;
    return JNI_VERSION_1_6;
}



typedef struct
{
    jobject o_greet_listener;
    jmethodID m_greet_receive;

    jclass z_greet_callback;
    jmethodID m_greet_callback_init;
} jni_callback_t;

static jni_callback_t *jni_callback;






void *async_thread_func(void *args)
{
    // JNI_OnLoad 时保存 jvm，这里使用 jvm->AttachCurrentThread 获取 JNIEnv，暂不做详细介绍.
    JNIEnv *env = NULL;
//    hover_attach_jni_env(&env);

    // 使用 java 构造函数生成 GreetCallback 的实例
    int greetID = 1;
    jstring greetMsg = env->NewStringUTF("say hi to java.");
    jobject greet_callback = env->NewObject(jni_callback->z_greet_callback, jni_callback->m_greet_callback_init, greetID, greetMsg);

    // 调用 GreetListener 的 onGreetReceive 函数，完成调用流程.
    env->CallVoidMethod(jni_callback->o_greet_listener, jni_callback->m_greet_receive, greet_callback);

    // 销毁局部引用
    env->DeleteLocalRef(greetMsg);
    env->DeleteLocalRef(greet_callback);
    greetMsg = NULL;
    greet_callback = NULL;

    // 销毁全局引用 --- 如果有多处或多次回调，自行判断销毁时机.
    env->DeleteGlobalRef(jni_callback->o_greet_listener);
    env->DeleteGlobalRef(jni_callback->z_greet_callback);
    jni_callback->o_greet_listener = NULL;
    jni_callback->z_greet_callback = NULL;
    free(jni_callback);
    jni_callback = NULL;

    return 0;
}






extern "C"
JNIEXPORT void JNICALL
Java_com_vaca_myapplication_MainActivity_native_1say_1hello(JNIEnv *env, jobject object,
                                                            jobject greet_listener) {
    if(!greet_listener)
        return;

    // 获得回调接口 函数onGreetReceive 的 methodID.
    jclass clazz_greet_listener = env->GetObjectClass(greet_listener);
    jmethodID method_greet_receive = env->GetMethodID(clazz_greet_listener, "onGreetReceive", "(Lcom/jnidemo/GreetCallback;)V");

    // 获得自定义 Callback类 GreetCallback 构造函数的 methodID.
    jclass clazz_greet_callback = env->FindClass("com/vaca/myapplication/GreetCallback");
    jmethodID method_greet_callback_init = env->GetMethodID(clazz_greet_callback, "<init>", "(ILjava/lang/String;)V");

    // 这里创建 jni_callback_t 的实例，创建 Listener 和 Callback 的全局引用并赋值.
    jni_callback = (jni_callback_t *)malloc(sizeof(jni_callback_t));
    memset(jni_callback, 0, sizeof(jni_callback_t));
    jni_callback->o_greet_listener = env->NewGlobalRef(greet_listener);
    jni_callback->m_greet_receive = method_greet_receive;
    jni_callback->z_greet_callback = (jclass)env->NewGlobalRef(clazz_greet_callback);
    jni_callback->m_greet_callback_init = method_greet_callback_init;

    // 销毁局部引用
    env->DeleteLocalRef(clazz_greet_listener);
    env->DeleteLocalRef(clazz_greet_callback);
    clazz_greet_listener = NULL;
    clazz_greet_callback = NULL;

    // 这里创建子线程模拟异步回调
//    pthread_t ntid;
//    pthread_create(&ntid, NULL, async_thread_func, NULL);

    jstring greetMsg = env->NewStringUTF("say hi to java.");
    jobject greet_callback = env->NewObject(jni_callback->z_greet_callback, jni_callback->m_greet_callback_init, 56, greetMsg);

    // 调用 GreetListener 的 onGreetReceive 函数，完成调用流程.
    env->CallVoidMethod(jni_callback->o_greet_listener, jni_callback->m_greet_receive, greet_callback);

}


static void say_hello_jni(JNIEnv *env, jobject object, jobject greet_listener)
{
    if(!greet_listener)
        return;

    // 获得回调接口 函数onGreetReceive 的 methodID.
    jclass clazz_greet_listener = env->GetObjectClass(greet_listener);
    jmethodID method_greet_receive = env->GetMethodID(clazz_greet_listener, "onGreetReceive", "(Lcom/jnidemo/GreetCallback;)V");

    // 获得自定义 Callback类 GreetCallback 构造函数的 methodID.
    jclass clazz_greet_callback = env->FindClass("com/vaca/myapplication/GreetCallback");
    jmethodID method_greet_callback_init = env->GetMethodID(clazz_greet_callback, "<init>", "(ILjava/lang/String;)V");

    // 这里创建 jni_callback_t 的实例，创建 Listener 和 Callback 的全局引用并赋值.
    jni_callback = (jni_callback_t *)malloc(sizeof(jni_callback_t));
    memset(jni_callback, 0, sizeof(jni_callback_t));
    jni_callback->o_greet_listener = env->NewGlobalRef(greet_listener);
    jni_callback->m_greet_receive = method_greet_receive;
    jni_callback->z_greet_callback = (jclass)env->NewGlobalRef(clazz_greet_callback);
    jni_callback->m_greet_callback_init = method_greet_callback_init;

    // 销毁局部引用
    env->DeleteLocalRef(clazz_greet_listener);
    env->DeleteLocalRef(clazz_greet_callback);
    clazz_greet_listener = NULL;
    clazz_greet_callback = NULL;

    // 这里创建子线程模拟异步回调
//    pthread_t ntid;
//    pthread_create(&ntid, NULL, async_thread_func, NULL);

    jstring greetMsg = env->NewStringUTF("say hi to java.");
    jobject greet_callback = env->NewObject(jni_callback->z_greet_callback, jni_callback->m_greet_callback_init, 56, greetMsg);

    // 调用 GreetListener 的 onGreetReceive 函数，完成调用流程.
    env->CallVoidMethod(jni_callback->o_greet_listener, jni_callback->m_greet_receive, greet_callback);

}