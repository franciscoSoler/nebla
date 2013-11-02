/* 
 * File:   EstadoRobot5SharedMemory.h
 * Author: francisco
 *
 * Created on 26 de octubre de 2013, 22:30
 */

#ifndef ESTADOROBOT5SHAREDMEMORY_H
#define	ESTADOROBOT5SHAREDMEMORY_H

#include "../../Common.h"
#include "AbstractSharedMemory.h"

class EstadoRobot5SharedMemory : public AbstractSharedMemory {
protected:
    size_t getMemorySize();
public:
    EstadoRobot5SharedMemory() {};
    virtual ~EstadoRobot5SharedMemory() {};
    
    void readInfo (EstadoRobot5* dato);
    void writeInfo ( EstadoRobot5 *dato );
private:

};

#endif	/* ESTADOROBOT5SHAREDMEMORY_H */

