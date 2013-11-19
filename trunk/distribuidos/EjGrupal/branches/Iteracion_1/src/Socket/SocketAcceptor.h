#ifndef _SOCKET_ACCEPTOR_H_
#define _SOCKET_ACCEPTOR_H_

#include <string>
#include <memory>
#include <netinet/in.h>
#include "SocketStream.h"

#define DEFAULT_AMOUNT_LISTENERS    5

class SocketAcceptor
{

private:
    int m_lsd;
    int m_port;
    int m_amountListeners;
    std::string m_address;
    bool m_listening;

public:    
    SocketAcceptor(int port, int listeners = DEFAULT_AMOUNT_LISTENERS,
                   const char* address="");
    ~SocketAcceptor();

    /* Here the socket do the bind and listen */
    int start();

    SocketStream* accept();

private:
    SocketAcceptor() {}

public:
    typedef std::auto_ptr<SocketAcceptor> SocketAcceptorPtr;
};

#endif
