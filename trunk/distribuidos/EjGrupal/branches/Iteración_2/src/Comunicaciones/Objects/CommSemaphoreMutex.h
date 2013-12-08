#ifndef SEMAPHOREMUTEX_H
#define	SEMAPHOREMUTEX_H

#include "CommSemaphore.h"

namespace COMM {

class CommSemaphoreMutex : public CommSemaphore {
public:
    CommSemaphoreMutex(std::string CommName = "", int idShMem = 0);
    
    virtual ~CommSemaphoreMutex();
    
    void wait(int numSem = 0);
        
private:
    int idShMem_;
};
}

#endif	/* SEMAPHOREMUTEX_H */

