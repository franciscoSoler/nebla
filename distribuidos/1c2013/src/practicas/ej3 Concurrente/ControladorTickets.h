#ifndef CONTROLADOR_TICKETS_H_
#define CONTROLADOR_TICKETS_H_

#include "./IPC/SharedMemory/TicketsInfoSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"

class ControladorTickets
{
private:
	Semaphore semControl;
	TicketsInfoSharedMemory shMem;
		
public:

	ControladorTickets(); 

	virtual ~ControladorTickets();

	void obtenerTickets(int cantidad, int dinero, ResultadoCompra &resultadoOperacion);

};

#endif // CONTROLADOR_TICKETS_H_
