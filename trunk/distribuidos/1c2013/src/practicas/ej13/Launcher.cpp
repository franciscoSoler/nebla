
#include "./IPC/SharedMemory/ControlDataSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"


static char buffer[TAM_BUFFER];

int main(int argc, char **argv)
{

	if (argc != 3) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT LECTORES] [CANT ESCRITORES]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	
	int cantLectores = 0;
	sscanf(argv[1] , "%d", &cantLectores);
	
	int cantEscritores = 0;
	sscanf(argv[2] , "%d", &cantEscritores);
	
	sprintf(buffer, "Launcher: Lectores: %d Escritores: %d\n", cantLectores, cantEscritores);
	write(fileno(stdout), buffer, strlen(buffer));
		
	// Creando semaforo de control de acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	semControl.createSemaphore((char *) DIRECTORY, ID_SEMCONTROL, 1);
	semControl.initializeSemaphore(0, 1);

	// Creando el semáforo del escritor y el del lector
	Semaphore semLec = Semaphore();
	semLec.createSemaphore((char*) DIRECTORY, ID_SEMLEC, 1);
	semLec.initializeSemaphore(0, 0);
	Semaphore semEsc = Semaphore();
	semEsc.createSemaphore((char *) DIRECTORY, ID_SEMESC, 1);
	semEsc.initializeSemaphore(0,0);
	
	// Creando memoria compartida
	ControlDataSharedMemory shMem = ControlDataSharedMemory();
	shMem.createSharedMemory((char *)DIRECTORY, ID_SHMEM);
	
	ControlData initialData;
	initialData.estado = LIBRE;
	initialData.cLectoresLeyendo = 0;
	initialData.cEscritoresEscribiendo = 0;
	initialData.cLectoresEsperando = 0;
	initialData.cEscritoresEsperando = 0;
	initialData.cAccesos = 0;
		
	semControl.wait();
	{
		shMem.write(&initialData);
	}		
	semControl.signal();
	
	// Creando Lectores
	for (int i = 1; i <= cantLectores; i++) 
	{
		pid_t pid = fork();
		if (pid == 0) 
		{
			// Paso como parámetro el Id del Lector
			sprintf(buffer, "%d", i);
			execlp("./lector", "lector", buffer, (char *) 0);
			sprintf(buffer, "Launcher: Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
	}
	
	// Creando Escritores
	for (int i = 1; i <= cantEscritores; i++) 
	{
		pid_t pid = fork();
		if (pid == 0) 
		{
			sprintf(buffer, "%d", i);
			execlp("./escritor", "escritor", buffer, (char *) 0);
			sprintf(buffer, "Launcher: Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
	}
	
	sprintf(buffer, "Launcher: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));
	return 0;
}
