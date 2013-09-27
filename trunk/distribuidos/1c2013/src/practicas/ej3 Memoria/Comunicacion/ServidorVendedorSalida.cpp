
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "../IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "../common.h"
#include "../Socket/inet.h"


extern int tcpoppas(int puerto);

int main(int argc, char **argv) {
	
	static char socketSalidaChar[15];

	char buffer[TAM_BUFFER];

	char server[TAM_BUFFER]; // Unused
	int puertoEntrada = 0; // Unused
	int puertoSalida = 0;
	
	std::ifstream input ("Config", std::ifstream::in);
	if (! input) { 
		std::cout << "Error al abrir el archivo\n"; 
		exit(1); 
	}
	input >> server;
	input >> puertoEntrada;
	input >> puertoSalida;
	input.close();

	sprintf(buffer, "Servidor Vendedor Salida: Levantando server: %s en puerto salida %d\n", server,puertoSalida);
	write (fileno(stdout),buffer, strlen(buffer));

	int 				socketSalidaPasivo;
	int					socketSalida;	    /* socket conectado al cliente */
	unsigned			clilen;	        	/* longitud dir. cliente */
	struct sockaddr_in	cli_addr;

	if ( (socketSalidaPasivo = tcpoppas(puertoSalida)) < 0) {
		perror("Servidor Vendedor Salida: No se puede abrir el stream socket");
		exit(1);
	}
	
	sprintf (buffer,"Servidor Vendedor Salida: se hizo el open pasivo, socket %d\n", socketSalidaPasivo);
	write(fileno(stdout), buffer, strlen(buffer));  

	while (true) {
		clilen = sizeof(cli_addr);

		socketSalida = accept(socketSalidaPasivo, (struct sockaddr *) &cli_addr, &clilen);
		
		sprintf (buffer,"Servidor Vendedor Salida: se conecto un cliente al socket %d\n", socketSalida);
		write(fileno(stdout), buffer, strlen(buffer));  		
		
		sprintf(socketSalidaChar, "%d\n", socketSalida); /* Pasarle el socket al hijo que atiende */

		pid_t pid = fork();
		if (pid == 0) {	
			close(socketSalidaPasivo);
			execlp("./canalSalidaVendedor", "canalSalidaVendedor", socketSalidaChar, (char *) 0);
			sprintf(buffer, "Servidor Vendedor Salida: Execlp ./canalSalidaVendedor error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
	}
	
	return 0;
}
