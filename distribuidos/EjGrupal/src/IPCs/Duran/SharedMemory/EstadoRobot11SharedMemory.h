#ifndef ESTADO_ROBOT_11_SHARED_MEMORY
#define ESTADO_ROBOT_11_SHARED_MEMORY

#include "../../../Common.h"
#include "../../AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {

class EstadoRobot11SharedMemory : public AbstractSharedMemory {

public:
	EstadoRobot11SharedMemory(std::string IPCName = "") : AbstractSharedMemory (IPCName) {}
	virtual ~EstadoRobot11SharedMemory() {}

	bool *readInfo ();
	void writeInfo (bool *dato);
	
protected:
	size_t getMemorySize();

};
}
#endif // ESTADO_ROBOT_11_SHARED_MEMORY
