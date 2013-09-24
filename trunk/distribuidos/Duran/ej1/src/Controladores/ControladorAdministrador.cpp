
#include "ControladorAdministrador.h"
#include "common.h"

ControladorAdministrador::ControladorAdministrador () {
	// Creando semaforo de control de acceso a la memoria compartida
	semControl = Semaphore();
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);

	// Obteniendo memoria compartida
	shMem = InfoMuseoSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
}

ControladorAdministrador::~ControladorAdministrador () {
}

bool ControladorAdministrador::abrirMuseo() {
	char buffer[TAM_BUFFER];
	bool resultado;
	semControl.wait(0);
	{
		InfoMuseo *info =  shMem.readInfo();
		if ((*info).abierto) {
			resultado = false;
		}
		else {
			resultado = true;
			(*info).abierto = true;
			(*info).total = 0;
			sprintf (buffer, "Controlador administrador: Abriendo museo\n");
			write (fileno(stdout),buffer, strlen(buffer));
			shMem.writeInfo(info);
		}
	}
	semControl.signal(0);
	
	return resultado;
}

bool ControladorAdministrador::cerrarMuseo() {
	char buffer[TAM_BUFFER];
	bool resultado;
	int total;
	semControl.wait(0);
	{
		InfoMuseo *info =  shMem.readInfo();
		if (!(*info).abierto) {
			resultado = false;
		}
		else {
			resultado = true;
			(*info).abierto = false;
			total = (*info).total;
			sprintf (buffer, "Controlador administrador: Cerrando museo, ingresaron %d personas\n", total);
			write (fileno(stdout),buffer, strlen(buffer));
			shMem.writeInfo(info);
		}		
		shMem.writeInfo(info);
	}
	semControl.signal(0);
		
	return resultado;
}
