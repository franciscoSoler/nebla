/* 
 * File:   SharedMemoryCantidadOxigenos.h
 * Author: distribuidos
 *
 * Created on April 12, 2013, 9:49 PM
 */

#ifndef SHAREDMEMORYCANTIDADOXIGENOS_H
#define	SHAREDMEMORYCANTIDADOXIGENOS_H
#include "AbstractSharedMemory.h"

class SharedMemoryCantidadOxigenos: public AbstractSharedMemory {
public:
    SharedMemoryCantidadOxigenos();
    void escribir(int cantOxigenos);
    int leer();
    virtual ~SharedMemoryCantidadOxigenos();
private:
    size_t getMemorySize();
};

#endif	/* SHAREDMEMORYCANTIDADOXIGENOS_H */

