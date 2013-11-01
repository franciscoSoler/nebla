#ifndef ESTADO_ROBOT_11_SHARED_MEMORY
#define ESTADO_ROBOT_11_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class EstadoRobot11SharedMemory : public AbstractSharedMemory {

public:
	EstadoRobot11SharedMemory() : AbstractSharedMemory () {}
	virtual ~EstadoRobot11SharedMemory() {}

	bool *readInfo ();
	void writeInfo (bool *dato);
	
protected:
	size_t getMemorySize();

};
#endif // ESTADO_ROBOT_11_SHARED_MEMORY
