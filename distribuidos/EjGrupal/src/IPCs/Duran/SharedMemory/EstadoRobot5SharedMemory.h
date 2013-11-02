#ifndef ESTADO_ROBOT_5_SHARED_MEMORY
#define ESTADO_ROBOT_5_SHARED_MEMORY

#include "../../../Common.h"
#include "../../AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {

class EstadoRobot5SharedMemory : public AbstractSharedMemory {

public:
	EstadoRobot5SharedMemory(std::string IPCName = "") : AbstractSharedMemory (IPCName) {}
	virtual ~EstadoRobot5SharedMemory() {}

	bool *readInfo ();
	void writeInfo (bool *dato);
	
protected:
	size_t getMemorySize();

};
}
#endif // ESTADO_ROBOT_5_SHARED_MEMORY
