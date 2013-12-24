#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "SocketAcceptor.h"

SocketAcceptor::SocketAcceptor(int port, int listeners, const char* address)
    : m_lsd(0),
      m_port(port),
      m_amountListeners(listeners),
      m_address(address),
      m_listening(false) {}

SocketAcceptor::~SocketAcceptor() {
}

int SocketAcceptor::configureSocket() {
    if (m_listening == true) {
        return 0;
    }

    m_lsd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(m_port);

    if (m_address.size() > 0) {
        inet_pton(AF_INET, m_address.c_str(), &(address.sin_addr));
    }
    else {
        address.sin_addr.s_addr = INADDR_ANY;
    }
    
    int optval = 1;
    setsockopt(m_lsd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval); 
    
    int result = bind(m_lsd, (struct sockaddr*)&address, sizeof(address));
    if (result != 0) {
        Logger::logMessage(Logger::ERROR, "bind() failed");
        return result;
    }
    
    result = listen(m_lsd, 5);
    if (result != 0) {
        Logger::logMessage(Logger::ERROR, "listen() failed");
        return result;
    }
    m_listening = true;
    return result;
}

SocketStream* SocketAcceptor::accept() {
    if (m_listening == false) {
        return NULL;
    }

    struct sockaddr_in address;
    socklen_t len = sizeof(address);
    memset(&address, 0, sizeof(address));
    int sd = ::accept(m_lsd, (struct sockaddr*)&address, &len);
    if (sd < 0) {
        Logger::logMessage(Logger::ERROR, "accept() failed");
        return NULL;
    }
    return new SocketStream(sd, &address);
}

bool SocketAcceptor::destroy() {
    if (m_lsd > 0) {
        close(m_lsd);
        return true;
    }

    return false;
}
