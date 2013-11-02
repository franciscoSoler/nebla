#include "EstadoRobot5SharedMemory.h"

//-----------------------------------------------------------------------------
size_t EstadoRobot5SharedMemory::getMemorySize() {
    return sizeof(bool);
}

//-----------------------------------------------------------------------------
bool* EstadoRobot5SharedMemory::readInfo () {
    return (bool *)(this->data);
}

//-----------------------------------------------------------------------------
void EstadoRobot5SharedMemory::writeInfo ( bool *dato ) {
    memcpy(this->data, dato, sizeof(bool));
}