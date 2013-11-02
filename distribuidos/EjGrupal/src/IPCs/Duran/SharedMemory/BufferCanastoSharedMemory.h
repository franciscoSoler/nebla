#ifndef BUFFER_CANASTO_SHARED_MEMORY
#define BUFFER_CANASTO_SHARED_MEMORY

#include "../../../Common.h"
#include "../../AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {

class BufferCanastoSharedMemory : public AbstractSharedMemory {

public:
	BufferCanastoSharedMemory(std::string IPCName = "") : AbstractSharedMemory (IPCName) {}
	virtual ~BufferCanastoSharedMemory() {}

	Canasto* readInfo ();
	void writeInfo ( Canasto *dato );
	
protected:
	size_t getMemorySize();

};
}
#endif // BUFFER_CANASTO_SHARED_MEMORY
