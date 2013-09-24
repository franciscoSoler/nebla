#ifndef CONTROLADOR_ADMINISTRADOR_H_
#define CONTROLADOR_ADMINISTRADOR_H_

#include "./IPC/SharedMemory/InfoMuseoSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"

class ControladorAdministrador
{
	
private:
	Semaphore semControl;
	InfoMuseoSharedMemory shMem;
	
public:
	ControladorAdministrador();
	virtual ~ControladorAdministrador();

	bool abrirMuseo();
	bool cerrarMuseo();
};

#endif // CONTROLADOR_ADMINISTRADOR_H_

