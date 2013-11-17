/* 
 * File:   EstadoRobot5SharedMemory.h
 * Author: francisco
 *
 * Created on 26 de octubre de 2013, 22:30
 */

#ifndef ESTADOROBOT5SHAREDMEMORY_H
#define	ESTADOROBOT5SHAREDMEMORY_H

#include "../../../Common.h"
#include "../IPCs/IPCAbstractos/AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {

class EstadoRobot5SharedMemory : public AbstractSharedMemory {
protected:
    size_t getMemorySize();
public:
    EstadoRobot5SharedMemory(std::string IPCName = "") : AbstractSharedMemory(IPCName) {};
    virtual ~EstadoRobot5SharedMemory() {};
    
    void readInfo (EstadoRobot5 *dato);
    void writeInfo (EstadoRobot5 *dato );
private:

};

}

#endif	/* ESTADOROBOT5SHAREDMEMORY_H */

