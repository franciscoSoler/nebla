
#include "ControladorTickets.h"
#include "common.h"

ControladorTickets::ControladorTickets () {
	// Creando semaforo de control de acceso a la memoria compartida
	semControl = Semaphore();	
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);

	// Obteniendo memoria compartida
	shMem = TicketsInfoSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
}

ControladorTickets::~ControladorTickets () {

}

void ControladorTickets::obtenerTickets(int cantidad, int dinero, ResultadoCompra &resultadoOperacion) {
	char buffer [255];
	semControl.wait(0);
	{
		TicketsInfo *info =  shMem.readInfo();
		if ((*info).cantidad >= cantidad) {
			// La cantidad de tickets disponibles es suficiente
			if ((*info).dinero <= dinero) {
				// El dinero brindado es suficiente
				resultadoOperacion.resultado = true;
				resultadoOperacion.vuelto = dinero - (*info).dinero;
				sprintf(buffer, "%d\n", (*info).cantidad);
				strcpy (resultadoOperacion.mensaje, buffer);
				(*info).cantidad -- ;
				shMem.writeInfo(info);
			}
			else {
				// El dinero es insuficiente
				resultadoOperacion.resultado = false;
				strcpy (resultadoOperacion.mensaje, "Monto insuficinete\n");
			}
		}
		else {
			// No hay tickets suficientes
			resultadoOperacion.resultado = false;
			strcpy (resultadoOperacion.mensaje, "No hay tickets suficientes\n");
		}
	}
	semControl.signal(0);
}
