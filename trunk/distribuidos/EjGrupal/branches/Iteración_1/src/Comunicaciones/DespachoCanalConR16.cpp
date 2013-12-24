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
    Logger::setProcessInformation("DespachoCanalConR16:");
    char buffer[255];
    CommunicationsUtil util;
    int socketDescriptor = 0;

    if ( util.parseArgs(argc, argv, socketDescriptor) == -1 ) {
        exit(-1);
    }
    SocketStream socketR16(socketDescriptor);
    Logger::logMessage(Logger::COMM, "Se conecta con Robot16");

    try {
        /* Cola por la cual recibe mensajes del Desṕacho */
        IPC::MsgQueue inputQueueR16_Despacho("inputQueueR16_Despacho");
        inputQueueR16_Despacho.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_DESPACHO_INPUT_ID_D);

        // Espero mensajes del Despacho y luego los envío al Robot16 por el canal
        while ( true ) {
            Msg_FinProductoR16 mensaje;
            inputQueueR16_Despacho.recv(MSG_FIN_PRODUCTO_R16, mensaje);
            Logger::logMessage(Logger::COMM,
            "Recibe mensaje, procede a enviarlo por el canal");

            memcpy(buffer, &mensaje, sizeof(Msg_FinProductoR16));
            if ( socketR16.send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
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
