#ifndef CINTA_TRANSPORTADORA_SHARED_MEMORY
#define CINTA_TRANSPORTADORA_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class CintaTransportadoraSharedMemory : public AbstractSharedMemory {

public:
	CintaTransportadoraSharedMemory() : AbstractSharedMemory () {}
	virtual ~CintaTransportadoraSharedMemory() {}

	CintaTransportadora_6* readInfo ();
	void writeInfo (CintaTransportadora_6 *dato);
	
protected:
	size_t getMemorySize();

};
#endif // CINTA_TRANSPORTADORA_SHARED_MEMORY
