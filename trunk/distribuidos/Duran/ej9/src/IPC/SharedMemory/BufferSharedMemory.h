#ifndef BUFFER_SHARED_MEMORY
#define BUFFER_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class BufferSharedMemory : public AbstractSharedMemory {

public:
	BufferSharedMemory() : AbstractSharedMemory () {}
	virtual ~BufferSharedMemory() {}

	Buffer* readInfo ();
	void writeInfo ( Buffer *dato );
	
protected:
	size_t getMemorySize();

};
#endif // BUFFER_SHARED_MEMORY
