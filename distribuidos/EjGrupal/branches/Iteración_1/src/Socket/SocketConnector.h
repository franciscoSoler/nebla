#ifndef _SOCKET_CONNECTOR_H_
#define _SOCKET_CONNECTOR_H_

#include <netinet/in.h>
#include "SocketStream.h"
#include <memory>

class SocketConnector
{
 public:
    SocketStream* connect(int port, const char* server);
    
 private:
    int resolveHostName(const char* host, struct in_addr* addr);

 public:
    typedef std::auto_ptr<SocketConnector> SocketConnectorPtr;

};

#endif
