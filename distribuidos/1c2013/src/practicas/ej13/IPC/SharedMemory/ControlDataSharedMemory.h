#include "../../common.h"
#include "AbstractSharedMemory.h"

class ControlDataSharedMemory : public AbstractSharedMemory
{

protected:
	size_t getMemorySize();
	
public:
	ControlDataSharedMemory() : AbstractSharedMemory() {};
	virtual ~ControlDataSharedMemory() {};
	void read(ControlData* aControlData) const;
	void write(const ControlData* aControlData); 		
};

