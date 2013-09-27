#ifndef SAMPLES_TABLE_SHARED_MEMORY
#define SAMPLES_TABLE_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class SamplesTableSharedMemory : public AbstractSharedMemory {

public:

	SamplesTableSharedMemory() : AbstractSharedMemory () {}

	virtual ~SamplesTableSharedMemory() {}

	SamplesTable* readInfo ();
	void writeInfo ( SamplesTable *dato );
	
protected:
	size_t getMemorySize();

};
#endif
