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
    Logger::setProcessInformation("DespachoCanalConCliente:");
    char buffer[255];
    CommunicationsUtil util;
    int socketDescriptor = 0;
    long idCliente = 0;

    if ( util.parseArgs(argc, argv, socketDescriptor) == -1 ) {
        exit(-1);
    }
    SocketStream socketCliente(socketDescriptor);

    // Recibo el ID del cliente
    if ( socketCliente.receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al enviar mensaje a despacho");
        abort();
    }
    memcpy(&idCliente, buffer, sizeof(long));
    sprintf(buffer, "Se conecta con Cliente - ID: %ld", idCliente);
    Logger::logMessage(Logger::COMM, buffer);

    try {
        /* Cola por la cual el canal le envía mensajes al cliente */
        IPC::MsgQueue inputQueueCliente("inputQueueCliente");
        inputQueueCliente.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID_D);

        /* Cola por la cual el cliente le envía mensajes al Despacho por el canal */
        IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
        inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID_D);

        bool ultimoPedido = false;
        while (not ultimoPedido) {
            // Espero mensajes del despacho hacia el cliente
            Msg_RetiroProducto mensajeSalida;
            inputQueueCliente.recv(idCliente, mensajeSalida);
            Logger::logMessage(Logger::COMM, "Recibe mensaje, procede a enviarlo por el canal");

            ultimoPedido = mensajeSalida.ultimoPedido_;

            memcpy(buffer, &mensajeSalida, sizeof(mensajeSalida));
            if ( socketCliente.send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al enviar mensaje al Cliente");
                abort();
            }

            // Espero respuesta del cliente
            if ( socketCliente.receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensaje de Cliente");
                abort();
            }

            Msg_PedidoDespacho mensajeEntrada;
            memcpy(&mensajeEntrada, buffer, sizeof(Msg_PedidoDespacho));
            inputQueueDespacho.send(mensajeEntrada);
        }

        sprintf(buffer, "Comunicación terminada con Cliente N°%ld. Cerrando canal", idCliente);
        Logger::logMessage(Logger::COMM, buffer);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
    return 0;
}
