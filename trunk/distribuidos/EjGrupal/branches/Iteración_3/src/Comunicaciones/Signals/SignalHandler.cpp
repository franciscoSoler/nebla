#include "SignalHandler.h"
#include <string.h>

// Definition of static array of handlers
EventHandler* SignalHandler::signalHandlers_[] = {0};

SignalHandler & SignalHandler::getInstance() {
    /* Variable instantiated in the first use, destroyed 
     *  automatically in the end of the program
     */
    static SignalHandler m_Instance;

    for (int i = 0; i < NSIG; ++i) {
        signalHandlers_[i] = NULL;
    }

    return m_Instance;
}

EventHandler* SignalHandler::registerHandler(int signum, EventHandler *eh) {
    // Copy the <old_eh> from the <signum> slot in 
    // the <signal_handlers_> table.
    EventHandler *oldEh = SignalHandler::signalHandlers_[signum];

    // Store <eh> into the <signum> slot in the
    // <signal_handlers_> table.
    SignalHandler::signalHandlers_[signum] = eh;
 
    // Register the <dispatcher> to handle this
    // <signum>.
    struct sigaction sa;
    sa.sa_handler = SignalHandler::dispatcher;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signum, &sa, 0);

    return oldEh;
}

void SignalHandler::dispatcher (int signum) {
    // Perform a sanity check...
    if (SignalHandler::signalHandlers_[signum] != 0) {
        // Dispatch the handler's hook method.
        SignalHandler::signalHandlers_[signum]->handleSignal (signum);
    }
}

int SignalHandler::removeHandler(int signum) {
    // TODO: isn't important right now
    return 0;
}
