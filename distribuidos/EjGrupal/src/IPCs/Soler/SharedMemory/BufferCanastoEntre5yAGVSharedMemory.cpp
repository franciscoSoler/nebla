/* 
 * File:   BufferCanastoEntre5yAGVSharedMemory.cpp
 * Author: francisco
 * 
 * Created on 28 de octubre de 2013, 13:58
 */

#include "BufferCanastoEntre5yAGVSharedMemory.h"

namespace IPC {

//-----------------------------------------------------------------------------
size_t BufferCanastoEntre5yAGVSharedMemory::getMemorySize() {
	return sizeof(Canasto);
}

//-----------------------------------------------------------------------------
void BufferCanastoEntre5yAGVSharedMemory::readInfo (Canasto* dato) {
    memcpy(dato, this->data, getMemorySize());
}

//-----------------------------------------------------------------------------
void BufferCanastoEntre5yAGVSharedMemory::writeInfo ( Canasto *dato ) {
    memcpy(this->data, dato, getMemorySize());
}

}