#ifndef SEMAPHOREMUTEX_H
#define	SEMAPHOREMUTEX_H

#include "CommSemaphore.h"

namespace IPC {

class CommSemaphoreMutex : public CommSemaphore {
public:
    CommSemaphoreMutex(std::string IPCName = "");
    
    virtual ~CommSemaphoreMutex();
    
    void wait(int numSem = 0);
        
private:

};
}

#endif	/* SEMAPHOREMUTEX_H */

