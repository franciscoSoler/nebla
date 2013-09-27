
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "../IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "../common.h"
#include "../Socket/inet.h"

extern int tcpoppas(int puerto);

int main(int argc, char **argv) {
	
	static char socketEntradaChar[15];         /* string que contiene el socket para el servidor de eco */
	
	char buffer[TAM_BUFFER];
	char server[TAM_BUFFER]; // Unused
	
	int puertoEntrada = 0;
	
	std::ifstream input ("Config", std::ifstream::in);
	if (! input) { 
		strcpy (buffer, "ServidorVendedorEntrada: Error al abrir el archivo\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	input >> server;
	input >> puertoEntrada;
	input.close();
	
	sprintf(buffer, "Servidor Vendedor Entrada: Levantando server: %s en puerto entrada %d\n", server,puertoEntrada);
	write (fileno(stdout),buffer, strlen(buffer));

	int 				socketEntradaPasivo;
	int					socketEntrada;	        /* socket conectado al cliente */
	unsigned			clilen;	        	/* longitud dir. cliente */
	struct sockaddr_in	cli_addr;
	
	if ( (socketEntradaPasivo = tcpoppas(puertoEntrada)) < 0) {
		perror("ServidorVendedorEntrada: no se puede abrir el stream socket");
		exit(1);
	}
	
	sprintf (buffer,"ServidorVendedorEntrada: se hizo el open pasivo, socket %d\n", socketEntradaPasivo);
	write(fileno(stdout), buffer, strlen(buffer));  

	while (true) {		
		clilen = sizeof(cli_addr);
		socketEntrada = accept(socketEntradaPasivo, (struct sockaddr *) &cli_addr, &clilen);
		if (socketEntrada < 0) {
       		perror ("Servidor Vendedor Entrada: error en el accept");
         	exit(1);
        }
        sprintf(socketEntradaChar, "%d\n", socketEntrada); /* Pasarle el socket al hijo que atiende */
        
		pid_t pid = fork();
		if (pid == 0) {
			close(socketEntradaPasivo);
			execlp("./canalEntradaVendedor", "canalEntradaVendedor", socketEntradaChar, (char *) 0);
			sprintf(buffer, "ServidorVendedorEntrada: Execlp ./canalEntradaVendedor error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
	}
	return 0;
} 
