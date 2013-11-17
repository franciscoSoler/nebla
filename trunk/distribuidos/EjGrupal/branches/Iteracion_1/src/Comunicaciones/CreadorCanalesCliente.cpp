
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "../IPCs/IPCAbstractos/MessageQueue/VendedoresMessageQueue.h"
#include "../Logger/Logger.h"
#include "../Common.h"

extern int tcpopact(char *, int);

int main(int argc, char **argv) {
	static char numClienteString[10];

        Logger::setProcessInformation("Creador Canales Cliente");
        
	char buffer[TAM_BUFFER];

        IPC::VendedoresMessageQueue getVendedorQueue;
        try {
            getVendedorQueue = IPC::VendedoresMessageQueue("VendedoresMessageQueue");
            getVendedorQueue.createMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES_C);
        }
        catch (Exception & e) {
            Logger::logMessage(Logger::ERROR, e.get_error_description());
            exit(-1);
        }
	
	while (true) {
        mensaje_inicial_t handShake;
        try {
            getVendedorQueue.recibirMensajeInicial(TIPO_BUSCANDO_VENDEDOR, &handShake);
        } catch (Exception & e) {
            Logger::logMessage(Logger::ERROR, e.get_error_description());
            exit(-1);
        }

        sprintf(buffer, "Llego el cliente: %ld.", handShake.emisor);
        Logger::logMessage(Logger::TRACE, buffer);

        sprintf(numClienteString, "%ld", handShake.emisor);

        pid_t pid = fork();
        if (pid == 0) {
            execlp("./canalSalidaCliente", "canalSalidaCliente", numClienteString, (char *) 0);
            sprintf(buffer, "CreadorCanalesCliente: Execlp ./canalSalidaCliente error: %s\n", strerror(errno));
            write(fileno(stdout), buffer, strlen(buffer));
            exit(-1);
        }

        pid_t pid2 = fork();
        if (pid2 == 0) {
            execlp("./canalEntradaCliente", "canalEntradaCliente", numClienteString, (char *) 0);
            sprintf(buffer, "Creador Canales Cliente: Execlp ./canalEntradaCliente error: %s\n", strerror(errno));
            write(fileno(stdout), buffer, strlen(buffer));
            exit(-1);
        }
    }
    return 0;
} 
