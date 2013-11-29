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
    Logger::setProcessInformation("DespachoCanalConR16_Cinta15:");
    char buffer[255];
    CommunicationsUtil util;
    int socketDescriptor = 0;

    if ( util.parseArgs(argc, argv, socketDescriptor) == -1 ) {
        exit(-1);
    }
    SocketStream socketR16(socketDescriptor);
    Logger::logMessage(Logger::COMM, "Se conecta con Robot16");

    try {
        /* Cola por la cual envía mensajes al Robot16 */
        IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
        inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID_D);

        // Espero mensajes del canal, y luego los envío al Robot16
        while ( true ) {
            if ( socketR16.receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensaje de Despacho");
                abort();
            }
            Logger::logMessage(Logger::COMM,
            "Recibe mensaje del canal, Procede a enviarlo al Robot16");

            Msg_PedidoDespacho mensaje;
            memcpy(&mensaje, buffer, sizeof(Msg_PedidoDespacho));
            inputQueueDespacho.send(mensaje);
        }

        Logger::logMessage(Logger::COMM, "Comunicación terminada. Cerrando canal");
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }

    return 0;
}
