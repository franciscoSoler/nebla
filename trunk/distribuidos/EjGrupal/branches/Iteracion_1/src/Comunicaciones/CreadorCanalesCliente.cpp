
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "../IPCs/IPCAbstractos/MessageQueue/VendedorLibreMessageQueue.h"
#include "../Logger/Logger.h"
#include "../Common.h"
#include <API/Objects/Util.h>

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

        Util::getInstance();
        Util::createProcess("CanalSalidaCliente", 1, handShake.emisor);
        Util::createProcess("CanalEntradaCliente", 1, handShake.emisor);
        Util::createProcess("ClienteCanalConDespacho", 1, handShake.emisor);
        Util::createProcess("ClienteCanalConR16", 1, handShake.emisor);
    }
    return 0;
} 
