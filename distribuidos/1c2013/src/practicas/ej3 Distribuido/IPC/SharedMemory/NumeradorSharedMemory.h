#ifndef NUMERADOR_SHARED_MEMORY
#define NUMERADOR_SHARED_MEMORY

#include "AbstractSharedMemory.h"
#include "../../Numerador/commonNumerador.h"

class NumeradorSharedMemory : public AbstractSharedMemory {

public:

	NumeradorSharedMemory() : AbstractSharedMemory () {}

	virtual ~NumeradorSharedMemory() {}

	NumeradorInfo* readInfo ();
	void writeInfo ( NumeradorInfo *dato );
	
protected:
	size_t getMemorySize();

};
#endif //NUMERADOR_CLIENTES_SHARED_MEMORY
