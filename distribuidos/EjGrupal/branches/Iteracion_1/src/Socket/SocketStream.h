#ifndef _SOCKET_STREAM_H_
#define _SOCKET_STREAM_H_


#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <memory>


class SocketStream
{
private:
    int          m_sd;
    std::string  m_peerIP;
    int          m_peerPort;

public: 
    SocketStream(int sd);
    SocketStream(int sd, struct sockaddr_in* address);
    ~SocketStream();

    ssize_t send(const char* buffer, size_t len);
    ssize_t receive(char* buffer, size_t len);
    bool destroy();

    // FIXME: See what happen when the first constructor is used
    std::string getPeerIP();
    int getPeerPort();
    int getSd();

private:
    SocketStream(const SocketStream& stream);

public:
    typedef std::auto_ptr<SocketStream> SocketStreamPtr;
};

#endif
