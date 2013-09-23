#ifndef MUESTRAS_SHARED_MEMORY
#define MUESTRAS_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class MuestrasSharedMemory : public AbstractSharedMemory {

public:
	MuestrasSharedMemory() : AbstractSharedMemory () {}
	virtual ~MuestrasSharedMemory() {}

	Muestras* readInfo ();
	void writeInfo ( Muestras *dato );
	
protected:
	size_t getMemorySize();

};
#endif // MUESTRAS_SHARED_MEMORY
