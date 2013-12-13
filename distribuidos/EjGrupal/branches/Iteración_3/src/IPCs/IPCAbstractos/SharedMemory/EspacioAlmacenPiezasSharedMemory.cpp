/* 
 * File:   EspacioAlmacenPiezasSharedMemory.cpp
 * Author: francisco
 * 
 * Created on 25 de octubre de 2013, 9:27
 */

#include "EspacioAlmacenPiezasSharedMemory.h"

namespace IPC {

//-----------------------------------------------------------------------------
size_t EspacioAlmacenPiezasSharedMemory::getMemorySize() {
	return sizeof(EstructuraAlmacenPiezas);
}

//-----------------------------------------------------------------------------
void EspacioAlmacenPiezasSharedMemory::readInfo (EstructuraAlmacenPiezas* dato) {
    memcpy(dato, this->data, getMemorySize());
}

//-----------------------------------------------------------------------------
void EspacioAlmacenPiezasSharedMemory::writeInfo (EstructuraAlmacenPiezas *dato ) {
    memcpy(this->data, dato, getMemorySize());
}

}
