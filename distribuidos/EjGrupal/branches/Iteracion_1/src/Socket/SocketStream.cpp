#include <arpa/inet.h>
#include <sys/socket.h>
#include <sstream>
#include "SocketStream.h"
#include <iostream>

SocketStream::SocketStream(int sd)
    : m_sd(sd),
      m_peerIP(""),
      m_peerPort(-1) {

}

SocketStream::SocketStream(int sd, struct sockaddr_in* address) : m_sd(sd) {
    char ip[50];
    inet_ntop(AF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);
    m_peerIP = ip;
    m_peerPort = ntohs(address->sin_port);
}

SocketStream::~SocketStream() {
}

bool SocketStream::destroy() {
    return ( (close(m_sd) == -1) ? false : true );
}

ssize_t SocketStream::send(const char* buffer, size_t len) {
    size_t bytesSend = 0;
    size_t lastBytesSend = 0;

    while (len) {
        lastBytesSend = write(m_sd, ((unsigned char *) buffer) + bytesSend, len);

        if (lastBytesSend <= 0) {
            break;
        }

        bytesSend += lastBytesSend;
        len -= lastBytesSend;
    }

    return bytesSend;
}

ssize_t SocketStream::receive(char* buffer, size_t len) {
    size_t bytesReceived = 0;
    size_t lastBytesReceived = 0;

    while (len) {
        lastBytesReceived = read(m_sd, ((unsigned char *) buffer) + bytesReceived, len);

        if (lastBytesReceived <= 0) {
            // TODO: This is an error. See how to handle that.
            break;
        }

        bytesReceived += lastBytesReceived;
        len -= lastBytesReceived;
    }

    return bytesReceived;
}

std::string SocketStream::getPeerIP() {
    // TODO: getsockname
    return m_peerIP;
}

int SocketStream::getPeerPort() {
    // TODO: getsockname
    return m_peerPort;
}

int SocketStream::getSd() {
    return m_sd;
}
