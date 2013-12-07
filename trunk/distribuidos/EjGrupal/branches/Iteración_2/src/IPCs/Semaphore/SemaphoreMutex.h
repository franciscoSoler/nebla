#ifndef SEMAPHOREMUTEX_H
#define	SEMAPHOREMUTEX_H

#include "Semaphore.h"

namespace IPC {

class SemaphoreMutex : public Semaphore {
public:
    SemaphoreMutex(std::string IPCName = "");
    
    virtual ~SemaphoreMutex();
    
    void wait(int numSem = 0);
        
private:

};
}

#endif	/* SEMAPHOREMUTEX_H */

