/* 
 * File:   EspacioAlmacenPiezasSharedMemory.h
 * Author: francisco
 *
 * Created on 25 de octubre de 2013, 9:27
 */

#ifndef ESPACIOALMACENPIEZASSHAREDMEMORY_H
#define	ESPACIOALMACENPIEZASSHAREDMEMORY_H

#include "../../../Common.h"
#include "../AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {

class EspacioAlmacenPiezasSharedMemory  : public AbstractSharedMemory {
protected:
    size_t getMemorySize();
    
public:
    EspacioAlmacenPiezasSharedMemory(std::string IPCName = "") : AbstractSharedMemory (IPCName) {};
    virtual ~EspacioAlmacenPiezasSharedMemory() {};
    
    void readInfo (EstructuraAlmacenPiezas *dato);
    void writeInfo (EstructuraAlmacenPiezas *dato);
};

}

#endif	/* ESPACIOALMACENPIEZASSHAREDMEMORY_H */

