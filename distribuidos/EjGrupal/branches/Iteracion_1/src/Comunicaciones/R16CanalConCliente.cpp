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
    Logger::setProcessInformation("R16CanalConCliente:");
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
        IPC::MsgQueue R16_Cliente_Queue_("R16_Cliente_Queue");
        R16_Cliente_Queue_.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);


        while (true) {
            // Espero mensajes del despacho hacia el cliente
            Msg_EnvioCajaCliente mensajeSalida;
            R16_Cliente_Queue_.recv(idCliente, mensajeSalida);
            Logger::logMessage(Logger::COMM, "Recibe mensaje, procede a enviarlo por el canal");

            memcpy(buffer, &mensajeSalida, sizeof(mensajeSalida));
            if ( socketCliente.send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al enviar mensaje al Cliente");
                abort();
            }
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }

    return 0;
}
