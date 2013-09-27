#ifndef INFO_SALA_SHARED_MEMORY
#define INFO_SALA_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class InfoSalaSharedMemory : public AbstractSharedMemory {

public:
	InfoSalaSharedMemory() : AbstractSharedMemory () {}
	virtual ~InfoSalaSharedMemory() {}

	InfoSala* readInfo ();
	void writeInfo ( InfoSala *dato );
	
protected:
	size_t getMemorySize();

};
#endif // INFO_SALA_SHARED_MEMORY
