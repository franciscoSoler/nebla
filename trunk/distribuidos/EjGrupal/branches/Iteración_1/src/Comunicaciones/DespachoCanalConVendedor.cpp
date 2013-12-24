#include <iostream>
#include <string.h>
#include <assert.h>

#include <Logger/Logger.h>
#include <Common.h>

#include <IPCs/IPCTemplate/MsgQueue.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Socket/SocketStream.h>

int main(int argc, char* argv[]) {
    Logger::setProcessInformation("DespachoCanalConVendedor:");
    char buffer[255];
    CommunicationsUtil util;
    int socketDescriptor = 0;
    long idVendedor = 0;

    if ( util.parseArgs(argc, argv, socketDescriptor) == -1 ) {
        exit(-1);
    }
    SocketStream socketVendedor(socketDescriptor);

    // Recibo el ID del vendedor
    if ( socketVendedor.receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al recibir mensaje de Vendedor");
        abort();
    }

    memcpy(&idVendedor, buffer, sizeof(long));
    sprintf(buffer, "Se conecta con Vendedor - ID: %ld", idVendedor);
    Logger::logMessage(Logger::COMM, buffer);

    try {
        /* Cola por la cual el canal envía mensajes al despacho */
        IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
        inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID_D);

        // Espero mensajes del Vendedor hacia el Despacho
        while ( true ) {
            // Recibe primer mensaje
            if ( socketVendedor.receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensaje de Vendedor");
                abort();
            }
            Logger::logMessage(Logger::COMM,
            "Recibe primer mensaje del canal, Procede a enviarlo a Despacho");

            Msg_PedidoDespacho mensajeEntrada;
            memcpy(&mensajeEntrada, buffer, sizeof(Msg_PedidoDespacho));
            inputQueueDespacho.send(mensajeEntrada);

            // Recibe segundo mensaje
            if ( socketVendedor.receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensaje de Vendedor");
                abort();
            }
            Logger::logMessage(Logger::COMM,
            "Recibe segundo mensaje del canal, Procede a enviarlo a Despacho");

            Msg_EnvioODCDespacho segundoMensajeEntrada;
            memcpy(&segundoMensajeEntrada, buffer, sizeof(Msg_EnvioODCDespacho));
            inputQueueDespacho.send(segundoMensajeEntrada);
        }

        Logger::logMessage(Logger::COMM, "Comunicación terminada. Cerrando canal");
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }

    return 0;
}

