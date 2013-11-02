#include "EstadoRobot11SharedMemory.h"

//-----------------------------------------------------------------------------
size_t EstadoRobot11SharedMemory::getMemorySize() {
	return sizeof(bool);
}

//-----------------------------------------------------------------------------
bool *EstadoRobot11SharedMemory::readInfo () {
    return (bool*)(this->data);
}

//-----------------------------------------------------------------------------
void EstadoRobot11SharedMemory::writeInfo ( bool *dato ) {
    memcpy(this->data, dato, sizeof(bool));
}