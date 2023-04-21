//
// Created by vaca on 4/21/23.
//

#ifndef TURN_ESP32_GET_NAME_H
#define TURN_ESP32_GET_NAME_H
#include <netdb.h>

int
getnameinfog(const struct sockaddr *sa, socklen_t salen,
            char *host, size_t hostlen,
            char *serv, size_t servlen, int flags);
#endif //TURN_ESP32_GET_NAME_H
