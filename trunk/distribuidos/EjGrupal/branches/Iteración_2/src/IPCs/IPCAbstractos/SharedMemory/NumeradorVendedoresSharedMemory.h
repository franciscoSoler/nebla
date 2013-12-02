#ifndef NUMERADOR_VENDEDORES_SHARED_MEMORY
#define NUMERADOR_VENDEDORES_SHARED_MEMORY

#include "../AbstractSharedMemory/AbstractSharedMemory.h"
#include "../../../Numerador/commonNumerador.h"

namespace IPC {
    
class NumeradorVendedoresSharedMemory : public AbstractSharedMemory {

public:
	NumeradorVendedoresSharedMemory(std::string IPCName = "") : AbstractSharedMemory () {}

	virtual ~NumeradorVendedoresSharedMemory() {}

	VendedoresInfo* readInfo ();
	void writeInfo ( VendedoresInfo *dato );
	
protected:
	size_t getMemorySize();

};

}

#endif //NUMERADOR_VENDEDORES_SHARED_MEMORY
