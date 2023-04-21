//
// Created by vaca on 4/21/23.
//

#include <arpa/inet.h>
#include <stdio.h>
#include "get_name.h"
#include "fake-rfc2553.h"

int
getnameinfog(const struct sockaddr *sa, socklen_t salen,
            char *host, size_t hostlen,
            char *serv, size_t servlen, int flags)
{


    const struct sockaddr_in *sinp = (const struct sockaddr_in *) sa;

    switch (sa->sa_family) {
        case AF_INET:
            if (flags & NI_NUMERICHOST) {
                if (inet_ntop (AF_INET, &sinp->sin_addr, host, hostlen) == NULL) {
                    return 0;
                }
            }

            if (flags & NI_NUMERICSERV) {
                if (snprintf(serv, servlen, "%d", ntohs (sinp->sin_port)) < 0) {
                    return 0;
                }
            }

            break;
    }


    return 0;
}