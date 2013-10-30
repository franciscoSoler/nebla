#ifndef BUFFER_CANASTO_SHARED_MEMORY
#define BUFFER_CANASTO_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class BufferCanastoSharedMemory : public AbstractSharedMemory {

public:
	BufferCanastoSharedMemory() : AbstractSharedMemory () {}
	virtual ~BufferCanastoSharedMemory() {}

	Canasto* readInfo ();
	void writeInfo ( Canasto *dato );
	
protected:
	size_t getMemorySize();

};
#endif // BUFFER_CANASTO_SHARED_MEMORY
