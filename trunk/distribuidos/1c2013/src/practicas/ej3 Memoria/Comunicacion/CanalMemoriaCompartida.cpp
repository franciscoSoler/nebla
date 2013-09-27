
#include "./../IPC/SharedMemory/TicketsInfoSharedMemory.h"
#include "./../IPC/Semaphore/Semaphore.h"

#include "../common.h"

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

int main(int argc, char **argv) {
	
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Canal Memoria Compartida: Error - Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int socketComunicacion = 0;
	sscanf(argv[1] , "%d", &socketComunicacion);
	
	sprintf (buffer,"Canal Memoria Compartida: Iniciando\n");
	write(fileno(stdout), buffer, strlen(buffer));
	
	// Obteniendo semaforo de control de acceso a la memoria compartida
	semControl = Semaphore();	
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);

	// Obteniendo memoria compartida
	shMem = TicketsInfoSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
	
	char buffer[TAM_BUFFER];
	PaqueteTicketsInfo paqueteIntercambio;
	bool deboSeguir;

	do {
		
		if (recibir(socketComunicacion, buffer, TAM_BUFFER) != TAM_BUFFER) {
			sprintf(buffer, "Canal Memoria Compartida - Error al recibir un pedido de memoria. ERROR: %d.\n", errno);
			write (fileno(stderr),buffer, strlen(buffer));
			close(socketComunicacion);
			exit(-1);
		}
		memcpy(&paqueteIntercambio, buffer, sizeof(PaqueteTicketsInfo));
		
		deboSeguir = paqueteIntercambio.deboSeguir;
		
		if (deboSeguir) {
			// Lllego un pedido de memoria compartida
			senControl.wait();
				TicketsInfo *info =  shMem.readInfo();
				/* Envio la memoria compartida */
				memcpy(&paqueteIntercambio.ticketsInfo, info, sizeof(TicketsInfo));
				memcpy(buffer, &paqueteIntercambio, sizeof(PaqueteTicketsInfo));
				if (enviar(socketComunicacion, buffer, TAM_BUFFER) != TAM_BUFFER) {
					sprintf(buffer, "Canal Memoria Compartida - Error al enviar la memoria\n");
					write (fileno(stderr),buffer, strlen(buffer));
					senControl.signal();
					close(socketComunicacion);
					exit(-1);
				}
			
				/* Recibo la memoria compartida actualizada */
				if (recibir(socketComunicacion, buffer, TAM_BUFFER) != TAM_BUFFER) {
					sprintf(buffer, "Canal Memoria Compartida - Error al recibir una actualización de memoria. ERROR: %d.\n", errno);
					write (fileno(stderr),buffer, strlen(buffer));
					senControl.signal();
					close(socketComunicacion);
					exit(-1);
				}
				memcpy(&paqueteIntercambio, buffer, sizeof(PaqueteTicketsInfo));
				shMem.writeInfo(paqueteIntercambio.ticketsInfo);
				deboSeguir = paqueteIntercambio.deboSeguir;
			
			senControl.signal();
		}
	} while (deboSeguir);
				
	close(socketEntrada);
	
}
