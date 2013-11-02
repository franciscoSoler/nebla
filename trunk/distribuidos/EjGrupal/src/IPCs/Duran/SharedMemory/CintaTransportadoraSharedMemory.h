#ifndef CINTA_TRANSPORTADORA_SHARED_MEMORY
#define CINTA_TRANSPORTADORA_SHARED_MEMORY

#include "../../../Common.h"
#include "../../AbstractSharedMemory/AbstractSharedMemory.h"

namespace IPC {
    
class CintaTransportadoraSharedMemory : public AbstractSharedMemory {

public:
	CintaTransportadoraSharedMemory(std::string IPCName = "") : AbstractSharedMemory (IPCName) {}
	virtual ~CintaTransportadoraSharedMemory() {}

	CintaTransportadora_6* readInfo ();
	void writeInfo (CintaTransportadora_6 *dato);
	
protected:
	size_t getMemorySize();

};
}
#endif // CINTA_TRANSPORTADORA_SHARED_MEMORY
