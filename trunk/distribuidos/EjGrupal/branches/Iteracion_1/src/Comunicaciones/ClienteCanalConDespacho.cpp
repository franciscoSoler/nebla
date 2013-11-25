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
    Logger::setProcessInformation("ClienteCanalConDespacho:");
    char buffer[255];
    CommunicationsUtil util;
    long nroCliente;

    if ( util.parseArgs(argc, argv, nroCliente) == -1 ) {
        exit(-1);
    }

    ServersManager serversManager;
    SocketStream::SocketStreamPtr socketDespacho(
    serversManager.connectToServer("ServidorDespachoClientes") );
    assert( socketDespacho.get() );

    // Lo primero que hace el cliente es enviar su ID
    memcpy(buffer, &nroCliente, sizeof(long));
    if ( socketDespacho->send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al enviar mensaje a despacho");
        abort();
    }

    try {
        /* Cola por la cual el canal le envía mensajes al cliente */
        IPC::MsgQueue inputQueueCliente("inputQueueCliente");
        inputQueueCliente.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID_C);

        /* Cola por la cual el cliente le envía mensajes al Despacho por el canal */
        IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
        inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID_C);

        // Luego, espero recibir un mensaje del despacho y se lo envío al cliente
        bool ultimoPedido = false;
        while (not ultimoPedido) {
            if ( socketDespacho->receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensaje de despacho");
                abort();
            }

            Msg_RetiroProducto mensajeEntrada;
            memcpy(&mensajeEntrada, buffer, sizeof(Msg_RetiroProducto));
            ultimoPedido = mensajeEntrada.ultimoPedido_;
            inputQueueCliente.send(mensajeEntrada);

            // Espero el mensaje del cliente al Despacho, y dsp lo envío por el canal
            Msg_PedidoDespacho mensajeSalida;
            inputQueueDespacho.recv(MSG_PEDIDO_DESPACHO, mensajeSalida);

            memcpy(buffer, &mensajeSalida, sizeof(Msg_PedidoDespacho));
            if ( socketDespacho->send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al enviar mensaje a despacho");
                abort();
            }
        }

        Logger::logMessage(Logger::COMM, "Comunicación terminada. Cerrando canal");
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
    return 0;
}

