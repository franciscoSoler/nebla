#ifndef TICKETS_INFO_SHARED_MEMORY
#define TICKETS_INFO_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class CountingSharedMemory : public AbstractSharedMemory {

public:

	CountingSharedMemory() : AbstractSharedMemory () {}

	virtual ~CountingSharedMemory() {}

	int* readInfo ();
	void writeInfo ( int *dato );
	
protected:
	size_t getMemorySize();

};
#endif //TICKETS_INFO_SHARED_MEMORY
