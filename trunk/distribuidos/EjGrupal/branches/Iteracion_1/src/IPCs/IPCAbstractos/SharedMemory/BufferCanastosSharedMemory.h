#ifndef BUFFER_CANASTOS_SHARED_MEMORY
#define BUFFER_CANASTOS_SHARED_MEMORY

#include "../../../Common.h"
#include "../AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {

class BufferCanastosSharedMemory : public AbstractSharedMemory {

public:
	BufferCanastosSharedMemory(std::string IPCName = "") : AbstractSharedMemory (IPCName) {}
	virtual ~BufferCanastosSharedMemory() {}

	void readInfo (BufferCanastos* dato);
	void writeInfo ( BufferCanastos *dato );
	
protected:
	size_t getMemorySize();
};

}

#endif // BUFFER_CANASTOS_SHARED_MEMORY
