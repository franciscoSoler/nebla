#ifndef INFO_VUELOS_SHARED_MEMORY
#define INFO_VUELOS_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class InfoVuelosSharedMemory : public AbstractSharedMemory {

public:
	InfoVuelosSharedMemory() : AbstractSharedMemory () {}
	virtual ~InfoVuelosSharedMemory() {}

	Informacion* readInfo ();
	void writeInfo ( Informacion *dato );
	
protected:
	size_t getMemorySize();

};
#endif // INFO_VUELOS_SHARED_MEMORY
