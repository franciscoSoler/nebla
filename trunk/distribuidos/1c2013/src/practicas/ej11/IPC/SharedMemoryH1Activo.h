/* 
 * File:   SharedMemoryH1Activo.h
 * Author: distribuidos
 *
 * Created on April 12, 2013, 2:09 AM
 */

#ifndef SHAREDMEMORYH1ACTIVO_H
#define	SHAREDMEMORYH1ACTIVO_H
#include "AbstractSharedMemory.h"

class SharedMemoryH1Activo: public AbstractSharedMemory {
public:
    SharedMemoryH1Activo();
    void escribir(bool h1Activo);
    bool leer();
    ~SharedMemoryH1Activo();
private:
    size_t getMemorySize();

};

#endif	/* SHAREDMEMORYH1ACTIVO_H */

