/* 
 * File:   EstadoRobot5SharedMemory.cpp
 * Author: francisco
 * 
 * Created on 26 de octubre de 2013, 22:30
 */

#include "EstadoRobot5SharedMemory.h"

namespace IPC {

//-----------------------------------------------------------------------------
size_t EstadoRobot5SharedMemory::getMemorySize() {
	return sizeof(EstadoRobot5);
}

//-----------------------------------------------------------------------------
void EstadoRobot5SharedMemory::readInfo (EstadoRobot5* dato) {
    memcpy(dato, this->data, getMemorySize());
}

//-----------------------------------------------------------------------------
void EstadoRobot5SharedMemory::writeInfo ( EstadoRobot5 *dato ) {
    memcpy(this->data, dato, getMemorySize());
}

}