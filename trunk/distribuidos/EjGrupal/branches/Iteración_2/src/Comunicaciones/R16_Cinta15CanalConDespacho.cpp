#include <iostream>
#include <string.h>
#include <assert.h>

#include <Logger/Logger.h>
#include <Common.h>

#include <IPCs/IPCTemplate/MsgQueue.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Socket/SocketStream.h>

int main(int argc, char* argv[]) {
    Logger::setProcessInformation("R16_Cinta15CanalConDespacho:");
    char buffer[255];

    ServersManager serversManager;
    SocketStream::SocketStreamPtr socketDespacho(
    serversManager.connectToServer("ServidorDespachoR16_Cinta15") );
    assert( socketDespacho.get() );
    Logger::logMessage(Logger::COMM, "Se conecta con Despacho");

    try {
        /* Cola por la cual envía mensajes al Robot16 */
        IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
        inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);

        // Espero mensajes del Robot16, y los envío por el canal
        while ( true ) {
            Msg_PedidoDespacho mensaje;
            inputQueueDespacho.recv(MSG_PEDIDO_DESPACHO, mensaje);
            Logger::logMessage(Logger::COMM,
            "Recibe mensaje, procede a enviarlo por el canal");

            memcpy(buffer, &mensaje, sizeof(Msg_PedidoDespacho));
            if ( socketDespacho->send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al enviar mensaje a Robot16");
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

