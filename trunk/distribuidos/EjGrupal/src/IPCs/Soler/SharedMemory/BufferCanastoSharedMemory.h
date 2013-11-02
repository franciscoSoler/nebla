#ifndef BUFFER_CANASTO_SHARED_MEMORY
#define BUFFER_CANASTO_SHARED_MEMORY

#include "../../Common.h"
#include "../../AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {

BufferCanastoSharedMemory::BufferCanastoSharedMemory(std::string IPCName) : IPCObject(IPCName)
{    
}
    
class BufferCanastoSharedMemory : public AbstractSharedMemory {

public:
	BufferCanastoSharedMemory(std::string IPCName = "") : AbstractSharedMemory(IPCName) {};
        virtual ~BufferCanastoSharedMemory() {}

	void readInfo (BufferCanastos* dato);
	void writeInfo ( BufferCanastos *dato );
	
protected:
	size_t getMemorySize();
};

}

#endif // BUFFER_CANASTO_SHARED_MEMORY
