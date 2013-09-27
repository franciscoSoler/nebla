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

class AbstractSharedMemory
{
private:
	int id;
	
	
protected:
	void* data;

public:
	/* Pre:
 	 * Post:
 	 */ 	
	AbstractSharedMemory();

	/* Pre:
 	 * Post:
 	 */
	~AbstractSharedMemory();

	/* 
	 * Metodo utilizado para obtener una Memoria Compartida, si no existe
	 * una cola de mensajes con la misma clave el metodo falla.
	 */
	int getSharedMemory(char *fileName, int id);
	
	/* 
	 * Metodo utilizado para crear la Memoria Compartida, si ya 
	 * existe una memoria compartida con la misma clave el metodo falla.
	 * El launcher es el encargado de crear los ipcs, y por lo tanto de 
	 * invocar este metodo.
	 */
	int createSharedMemory(char *fileName, int id);
	
	int destroy(void);

private:
	int getCantAttachProcesses(void) const;
	
	int attachMemory();

	int getId(char *fileName, int id, int flags);

protected:
	virtual size_t getMemorySize() = 0;
};
#endif
