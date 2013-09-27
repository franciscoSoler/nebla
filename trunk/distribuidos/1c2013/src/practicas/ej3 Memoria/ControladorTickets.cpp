
#include "ControladorTickets.h"
#include "common.h"

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

ControladorTickets::ControladorTickets () {
	/*// Creando semaforo de control de acceso a la memoria compartida
	semControl = Semaphore();	
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);

	// Obteniendo memoria compartida
	shMem = TicketsInfoSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);*/
}

ControladorTickets::~ControladorTickets () {

}

void ControladorTickets::conectarMemoria () {
	sprintf(buffer, "Controlador Tickets: Conectando canal de memoria\n");
	write(fileno(stdout), buffer, strlen(buffer));
	
	char server[TAM_BUFFER];
	int puertoEntrada = 0;
	int puertoSalida = 0;
	std::ifstream input ("Config", std::ifstream::in);
	if (! input) { 
		std::cerr << "Canal Entrada Cliente: Error al abrir el archivo\n"; 
		return -1; 
	}
	input >> server; // Unused
	input >> puertoEntrada; // Unused
	input >> puertoSalida; // Unused
	input >> server; // Unused
	input >> server;
	input >> puertoEntrada;
	input.close();
	
	socketComunicacion = tcpopact(server, puertoEntrada);

	sprintf(buffer, "Controlador Tickets - Conectado al socket: %d\n", socketComunicacion);
	write (fileno(stdout),buffer, strlen(buffer));
}

void ControladorTickets::desconectarMemoria () {
	char buffer [255];
	PaqueteTicketsInfo paqueteIntercambio;
		
	// Envio el pedido por la memoria compartida
	paqueteIntercambio.deboSeguir = false;
	
	memcpy(buffer, &paqueteIntercambio, sizeof(PaqueteTicketsInfo));
	if (enviar(socketComunicacion, buffer, TAM_BUFFER) != TAM_BUFFER) {
		sprintf(buffer, "Canal Memoria Compartida - Error al enviar la memoria\n");
		write (fileno(stderr),buffer, strlen(buffer));
		senControl.signal();
		close(socketComunicacion);
		exit(-1);
	}
}


void ControladorTickets::obtenerTickets(int cantidad, int dinero, ResultadoCompra &resultadoOperacion) {
	char buffer [255];
	/*semControl.wait(0);
	{*/

		//TicketsInfo *info =  shMem.readInfo();

		PaqueteTicketsInfo paqueteIntercambio;
		
		// Envio el pedido por la memoria compartida
		paqueteIntercambio.deboSeguir = true;
		memcpy(buffer, &paqueteIntercambio, sizeof(PaqueteTicketsInfo));
		if (enviar(socketComunicacion, buffer, TAM_BUFFER) != TAM_BUFFER) {
			sprintf(buffer, "Canal Memoria Compartida - Error al enviar la memoria\n");
			write (fileno(stderr),buffer, strlen(buffer));
			senControl.signal();
			close(socketComunicacion);
			exit(-1);
		}
		
		// Me quedo esperando la respuesta
		if (recibir(socketComunicacion, buffer, TAM_BUFFER) != TAM_BUFFER) {
			sprintf(buffer, "Canal Memoria Compartida - Error al recibir un pedido de memoria. ERROR: %d.\n", errno);
			write (fileno(stderr),buffer, strlen(buffer));
			close(socketComunicacion);
			exit(-1);
		}
		memcpy(&paqueteIntercambio, buffer, sizeof(PaqueteTicketsInfo));
		
		TicketsInfo info = paqueteIntercambio.ticketsInfo;
		
		if (info.cantidad >= cantidad) {
			// La cantidad de tickets disponibles es suficiente
			if (info.dinero <= dinero) {
				// El dinero brindado es suficiente
				resultadoOperacion.resultado = true;
				resultadoOperacion.vuelto = dinero - info.dinero;
				sprintf(buffer, "%d\n", infocantidad);
				strcpy (resultadoOperacion.mensaje, buffer);
				info.cantidad -- ;
				//shMem.writeInfo(info);
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
		
		
		// Retorno el valor de la memoria compartida
		paqueteIntercambio.deboSeguir = true;
		paqueteIntercambio.ticketsInfo = info;
		
		memcpy(buffer, &paqueteIntercambio, sizeof(PaqueteTicketsInfo));
		if (enviar(socketComunicacion, buffer, TAM_BUFFER) != TAM_BUFFER) {
			sprintf(buffer, "Canal Memoria Compartida - Error al enviar la memoria\n");
			write (fileno(stderr),buffer, strlen(buffer));
			senControl.signal();
			close(socketComunicacion);
			exit(-1);
		}
	/*}
	semControl.signal(0);*/
}

