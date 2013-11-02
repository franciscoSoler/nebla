#ifndef BUFFER_CANASTO_SHARED_MEMORY
#define BUFFER_CANASTO_SHARED_MEMORY

#include "../../Common.h"
#include "AbstractSharedMemory.h"

class BufferCanastoSharedMemory : public AbstractSharedMemory {

public:
	BufferCanastoSharedMemory() : AbstractSharedMemory () {}
	virtual ~BufferCanastoSharedMemory() {}

	void readInfo (BufferCanastos* dato);
	void writeInfo ( BufferCanastos *dato );
	
protected:
	size_t getMemorySize();
};
#endif // BUFFER_CANASTO_SHARED_MEMORY
