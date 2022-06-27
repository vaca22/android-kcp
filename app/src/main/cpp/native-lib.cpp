#include <jni.h>
#include <string>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <android/log.h>
#include <unistd.h>

#define TAG "tcomm"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

static int sockfd;




void createSocket(){
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOGE("create socket error\n");
    }else{
        LOGE("fuck2\n");
    }
}



void connectSocket( char *ipaddr,int port){
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipaddr, &servaddr.sin_addr) <= 0) {
        LOGE("inet_pton error for\n");
        return;
    }
    LOGV("connect...");
//    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
//        LOGE("connect error\n");
//        return;
//    }
    int ret=bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(ret<0){
        LOGV("connect fail!!!");
    }else{
        LOGV("connect succeed!!!");
    }

}


int  receiveData(int n){
    if (!sockfd) {
        LOGE("socket is failed!");
        return 0;
    }
    jbyte buf[n];
    int rec_len;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    if ((rec_len = (int) recvfrom(sockfd, buf, sizeof(buf), 0,(struct sockaddr*)&client_addr, &len)) == -1) {
        LOGE("recv error");
        return 0;
    }else{
        LOGE("fuck  %s",buf);
    }
    return rec_len;
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

void closeSocket(){
    if (!sockfd)
        return;
//    shutdown(sockfd,0);
    close(sockfd);
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_vaca_myapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {



    LOGE("fuck\n");
    createSocket();
    //这个是自己的ip
    connectSocket("192.168.5.100",12345);
    for(int k=0;k<1000;k++){
        sendData("fuck");
    }

 //   receiveData(24);





    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());


}
