#ifndef SH_MEMORY_
#define SH_MEMORY_

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "../../Exceptions/Exception.h"
#include "../../IPCObject/IPCObject.h"

namespace IPC {

class AbstractSharedMemory : public IPCObject
{
private:
	int id;
	char buffer[255];
	
protected:
	void* data;

public:
	/* Pre:
 	 * Post:
 	 */ 	
	AbstractSharedMemory(std::string IPCName = "");

	/* Pre:
 	 * Post:
 	 */
	virtual ~AbstractSharedMemory();

	/* 
	 * Metodo utilizado para obtener una Memoria Compartida, si no existe
	 * una cola de mensajes con la misma clave el metodo falla.
	 */
	int getSharedMemory(const char *fileName, int id);
	
	/* 
	 * Metodo utilizado para crear la Memoria Compartida, si ya 
	 * existe una memoria compartida con la misma clave el metodo falla.
	 * El launcher es el encargado de crear los ipcs, y por lo tanto de 
	 * invocar este metodo.
	 */
	int createSharedMemory(const char *fileName, int id);
	
	int destroy(void);

private:
	int getCantAttachProcesses(void) const;
	
	int attachMemory();

	int getId(const char *fileName, int id, int flags);

protected:
	virtual size_t getMemorySize() = 0;
};

}

#endif
