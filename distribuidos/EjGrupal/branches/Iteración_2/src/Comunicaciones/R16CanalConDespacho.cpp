#include <iostream>
#include <string.h>
#include <assert.h>

#include <Logger/Logger.h>
#include <Common.h>

#include <IPCs/IPCTemplate/MsgQueue.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Socket/SocketStream.h>

int main(int argc, char* argv[]) {
    Logger::setProcessInformation("R16CanalConDespacho:");
    char buffer[255];

    ServersManager serversManager;
    SocketStream::SocketStreamPtr socketDespacho(
    serversManager.connectToServer("ServidorDespachoR16") );
    assert( socketDespacho.get() );
    Logger::logMessage(Logger::COMM, "Se conecta con Despacho");

    try {
        /* Cola por la cual envía mensajes al Robot16 */
        IPC::MsgQueue inputQueueR16_Despacho("inputQueueR16_Despacho");
        inputQueueR16_Despacho.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_DESPACHO_INPUT_ID);

        // Espero mensajes del canal, y luego los envío al Robot16
        while ( true ) {
            if ( socketDespacho->receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensaje de Despacho");
                abort();
            }
            Logger::logMessage(Logger::COMM,
            "Recibe mensaje del canal, Procede a enviarlo al Robot16");

            Msg_FinProductoR16 mensaje;
            memcpy(&mensaje, buffer, sizeof(Msg_FinProductoR16));
            inputQueueR16_Despacho.send(mensaje);
        }

        Logger::logMessage(Logger::COMM, "Comunicación terminada. Cerrando canal");
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }

    return 0;
}

