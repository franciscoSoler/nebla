/* 
 * File:   Cinta6SharedMemory.h
 * Author: francisco
 *
 * Created on 25 de octubre de 2013, 9:27
 */

#ifndef CINTA6SHAREDMEMORY_H
#define	CINTA6SHAREDMEMORY_H

#include "../../Common.h"
#include "../../AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {

class Cinta6SharedMemory  : public AbstractSharedMemory {
protected:
    size_t getMemorySize();
    
public:
    Cinta6SharedMemory(std::string IPCName = "") : AbstractSharedMemory (IPCName){}
    virtual ~Cinta6SharedMemory() {};
    
    void readInfo (CintaTransportadora_6* dato);
    void writeInfo ( CintaTransportadora_6 *dato );
};

}

#endif	/* CINTA6SHAREDMEMORY_H */

