
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "../IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "../common.h"

extern int tcpopact(char *, int);

int main(int argc, char **argv) {

	static char numClienteString[10];
		
	char buffer[TAM_BUFFER];

	GetVendedorMessageQueue getVendedorQueue = GetVendedorMessageQueue();
	getVendedorQueue.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR_2);
	
	while (true) {
		HandShakeMessage handShake;
		if (getVendedorQueue.recibirPedido(1,&handShake) < 0) return -1;
		
		sprintf(buffer, "Creador Canales Clientes: Llego el cliente: %d\n", handShake.numero);
		write(fileno(stdout), buffer, strlen(buffer));
		
		sprintf(numClienteString,"%d",handShake.numero);
			
		pid_t pid = fork();
		if (pid == 0) {
			execlp("./canalSalidaCliente", "canalSalidaCliente", numClienteString , (char *) 0);
			sprintf(buffer, "CreadorCanalesCliente: Execlp ./canalSalidaCliente error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
			exit(-1);
		}
		
		pid_t pid2 = fork();
		if (pid2 == 0) {
			execlp("./canalEntradaCliente", "canalEntradaCliente", numClienteString , (char *) 0);
			sprintf(buffer, "Creador Canales Cliente: Execlp ./canalEntradaCliente error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	return 0;
} 
