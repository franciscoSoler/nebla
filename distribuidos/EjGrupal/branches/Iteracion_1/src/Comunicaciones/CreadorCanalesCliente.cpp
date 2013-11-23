
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "../IPCs/IPCAbstractos/MessageQueue/VendedorLibreMessageQueue.h"
#include "../Logger/Logger.h"
#include "../Common.h"

int main(int argc, char **argv) {
    static char numClienteString[10];
    Logger::setProcessInformation("Creador Canales Cliente");        
    char buffer[TAM_BUFFER];

    IPC::VendedorLibreMessageQueue getVendedorQueue;
    try {
        getVendedorQueue = IPC::VendedorLibreMessageQueue("VendedoresMessageQueue");
        getVendedorQueue.getMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES_C);
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
        Logger::logMessage(Logger::COMM, buffer);

        sprintf(numClienteString, "%ld", handShake.emisor);

        pid_t pid = fork();
        if (pid == 0) {
            execlp("./CanalSalidaCliente", "CanalSalidaCliente", numClienteString, (char *) 0);
            sprintf(buffer, "CreadorCanalesCliente: Execlp ./CanalSalidaCliente error: %s", strerror(errno));
            write(fileno(stdout), buffer, strlen(buffer));
            exit(-1);
        }

        pid_t pid2 = fork();
        if (pid2 == 0) {
            execlp("./CanalEntradaCliente", "CanalEntradaCliente", numClienteString, (char *) 0);
            sprintf(buffer, "Creador Canales Cliente: Execlp ./CanalEntradaCliente error: %s", strerror(errno));
            write(fileno(stdout), buffer, strlen(buffer));
            exit(-1);
        }
        }
    return 0;
} 
