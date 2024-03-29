#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "SocketConnector.h"
#include <Logger/Logger.h>

SocketStream* SocketConnector::connect(int port, const char* server) {
    struct sockaddr_in address;

    memset (&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (resolveHostName(server, &(address.sin_addr)) != 0 ) {
        inet_pton(AF_INET, server, &(address.sin_addr));
    } 

    int sd = socket(AF_INET, SOCK_STREAM, 0);

    if ( ::connect(sd, (struct sockaddr*)&address, sizeof(address)) != 0 ) {
        Logger::logMessage(Logger::ERROR, "connect() failed");
        return NULL;
    }
    return new SocketStream(sd, &address);
}

int SocketConnector::resolveHostName(const char* hostname, struct in_addr* addr) {
    struct addrinfo *res;
  
    int result = getaddrinfo (hostname, NULL, NULL, &res);
    if (result == 0) {
        memcpy(addr, &((struct sockaddr_in *) res->ai_addr)->sin_addr, sizeof(struct in_addr));
        freeaddrinfo(res);
    }
    return result;
}
