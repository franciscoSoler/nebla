#ifndef NUMERADOR_CLIENTES_SHARED_MEMORY
#define NUMERADOR_CLIENTES_SHARED_MEMORY

#include "../AbstractSharedMemory/AbstractSharedMemory.h"
#include "../../../Numerador/commonNumerador.h"

namespace IPC {
    
class NumeradorClientesSharedMemory : public AbstractSharedMemory {

public:
	NumeradorClientesSharedMemory(std::string IPCName = "") : AbstractSharedMemory () {}

	virtual ~NumeradorClientesSharedMemory() {}

	ClientesInfo* readInfo ();
	void writeInfo ( ClientesInfo *dato );
	
protected:
	size_t getMemorySize();

};

}

#endif //NUMERADOR_CLIENTES_SHARED_MEMORY
