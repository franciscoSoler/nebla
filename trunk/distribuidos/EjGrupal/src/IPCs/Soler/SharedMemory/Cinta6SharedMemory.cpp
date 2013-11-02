/* 
 * File:   Cinta6SharedMemory.cpp
 * Author: francisco
 * 
 * Created on 25 de octubre de 2013, 9:27
 */

#include "Cinta6SharedMemory.h"

namespace IPC {

//-----------------------------------------------------------------------------
size_t Cinta6SharedMemory::getMemorySize() {
	return sizeof(CintaTransportadora_6);
}

//-----------------------------------------------------------------------------
void Cinta6SharedMemory::readInfo (CintaTransportadora_6* dato) {
    memcpy(dato, this->data, getMemorySize());
}

//-----------------------------------------------------------------------------
void Cinta6SharedMemory::writeInfo ( CintaTransportadora_6 *dato ) {
    memcpy(this->data, dato, getMemorySize());
}

}

