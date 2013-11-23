#ifndef SIGNAL_HANDLER_H_
#define SIGNAL_HANDLER_H_

#include "EventHandler.h"
#define NSIG    64

class SignalHandler {
public:
    // Entry point.
    static SignalHandler & getInstance();

    // Register an event handler <eh> for <signum>
    // and return a pointer to any existing <Event_Handler>
    // that was previously registered to handle <signum>.
    static EventHandler* registerHandler(int signum, EventHandler *eh);

    // Remove the <Event_Handler> for <signum>
    // by setting the slot in the <signal_handlers_>
    // table to NULL. Return -1 if the handler cannot be removed 
    // (it doesn't exists)
    static int removeHandler(int signum);

private:
    // Ensure we're a Singleton.
    SignalHandler() {}
    SignalHandler(SignalHandler const &);
    void operator = (SignalHandler const &);

    // Entry point adapter installed into <sigaction> 
    // (must be a static method or a stand-alone 
    // extern "C" function).
    static void dispatcher(int signum);

    // Table of pointers to concrete <Event_Handler>s
    // registered by applications.  NSIG is the number of 
    // signals defined in </usr/include/sys/signal.h>.
    static EventHandler* signalHandlers_[NSIG];
};

#endif /* SIGNAL_HANDLER_H_ */
