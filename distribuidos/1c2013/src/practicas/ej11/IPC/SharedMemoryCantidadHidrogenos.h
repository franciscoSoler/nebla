/* 
 * File:   SharedMemoryCantidadHidrogenos.h
 * Author: distribuidos
 *
 * Created on April 12, 2013, 2:25 AM
 */

#ifndef SHAREDMEMORYCANTIDADHIDROGENOS_H
#define	SHAREDMEMORYCANTIDADHIDROGENOS_H
#include "AbstractSharedMemory.h"
class SharedMemoryCantidadHidrogenos: public AbstractSharedMemory {
public:
    SharedMemoryCantidadHidrogenos();
    virtual ~SharedMemoryCantidadHidrogenos();
    void escribir(int cantHidrogenos);
    int leer();
    
private:
    size_t getMemorySize();

};

#endif	/* SHAREDMEMORYCANTIDADHIDROGENOS_H */

