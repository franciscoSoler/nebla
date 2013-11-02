/* 
 * File:   BufferCanastoEntre5yAGVSharedMemory.h
 * Author: francisco
 *
 * Created on 28 de octubre de 2013, 13:58
 */

#ifndef BUFFERCANASTOENTRE5YAGVSHAREDMEMORY_H
#define	BUFFERCANASTOENTRE5YAGVSHAREDMEMORY_H

#include "../../../Common.h"
#include "../AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {

class BufferCanastoEntre5yAGVSharedMemory : public AbstractSharedMemory {
public:
    BufferCanastoEntre5yAGVSharedMemory(std::string IPCName = "") : AbstractSharedMemory(IPCName) {}
    virtual ~BufferCanastoEntre5yAGVSharedMemory() {}
    
    void readInfo (Canasto* dato);
    void writeInfo (Canasto *dato);
private:
    size_t getMemorySize();
};

}

#endif	/* BUFFERCANASTOENTRE5YAGVSHAREDMEMORY_H */

