#ifndef SIGINT_HANDLER_H_
#define SIGINT_HANDLER_H_

#include <signal.h>
#include "EventHandler.h" 
#include <assert.h>
#include <Logger/Logger.h>
#include <Exceptions/Exception.h>

class SIGINT_Handler : public EventHandler {
public:
    SIGINT_Handler (void) : gracefulQuit_ (0) {}

    // Hook method.
    int handleSignal(int signum) {
        assert (signum == SIGINT);
        Logger::logMessage(Logger::DEBUG, "SIGINT - Servidor ha sido eliminado");
        this->gracefulQuit_ = 1;
        return 0;
    }

    // Accessor.
    sig_atomic_t gracefulQuit(void) {
        return this->gracefulQuit_;
    }

private:
  sig_atomic_t gracefulQuit_;
};

#endif /* SIGINT_HANDLER_H_ */ 
