#ifndef TICKETS_INFO_SHARED_MEMORY
#define TICKETS_INFO_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class TicketsInfoSharedMemory : public AbstractSharedMemory {

public:

	TicketsInfoSharedMemory() : AbstractSharedMemory () {}

	virtual ~TicketsInfoSharedMemory() {}

	TicketsInfo* readInfo ();
	void writeInfo ( TicketsInfo *dato );
	
protected:
	size_t getMemorySize();

};
#endif //TICKETS_INFO_SHARED_MEMORY
