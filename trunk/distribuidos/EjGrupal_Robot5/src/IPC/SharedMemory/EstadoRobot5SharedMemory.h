#ifndef ESTADO_ROBOT_5_SHARED_MEMORY
#define ESTADO_ROBOT_5_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class EstadoRobot5SharedMemory : public AbstractSharedMemory {

public:
	EstadoRobot5SharedMemory() : AbstractSharedMemory () {}
	virtual ~EstadoRobot5SharedMemory() {}

	bool *readInfo ();
	void writeInfo (bool *dato);
	
protected:
	size_t getMemorySize();

};
#endif // ESTADO_ROBOT_5_SHARED_MEMORY
