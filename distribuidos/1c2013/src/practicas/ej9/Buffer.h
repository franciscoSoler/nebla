
#ifndef _BUFFER_
#define _BUFFER_

#include "./IPC/SharedMemory/CountingSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"

class Buffer
{
private:
	Semaphore semControl;
	Semaphore semProductores;
	CountingSharedMemory shMem;
	
	int cantidadProductores;
		
public:
	/* Pre:
 	 * Post:
 	 */ 	
	Buffer(int cantidadProductores);

	/* Pre:
 	 * Post:
 	 */
	~Buffer();

	/* 
	 */
	bool producir(int nroProductor);
		
};
#endif //_BUFFER_
