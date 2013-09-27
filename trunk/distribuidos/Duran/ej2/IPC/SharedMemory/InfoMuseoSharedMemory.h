#ifndef INFO_MUSEO_SHARED_MEMORY
#define INFO_MUSEO_SHARED_MEMORY

#include "../../common.h"
#include "AbstractSharedMemory.h"

class InfoMuseoSharedMemory : public AbstractSharedMemory {

public:
	InfoMuseoSharedMemory() : AbstractSharedMemory () {}
	virtual ~InfoMuseoSharedMemory() {}

	InfoMuseo* readInfo ();
	void writeInfo ( InfoMuseo *dato );
	
protected:
	size_t getMemorySize();

};
#endif // INFO_MUSEO_SHARED_MEMORY
