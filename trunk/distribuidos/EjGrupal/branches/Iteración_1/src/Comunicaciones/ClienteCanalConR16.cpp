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
    Logger::setProcessInformation("ClienteCanalConR16:");
    char buffer[255];
    CommunicationsUtil util;
    long nroCliente;

    if ( util.parseArgs(argc, argv, nroCliente) == -1 ) {
        exit(-1);
    }

    ServersManager serversManager;
    SocketStream::SocketStreamPtr socketR16(
    serversManager.connectToServer("ServidorR16Clientes") );
    assert( socketR16.get() );

    // Lo primero que hace el cliente es enviar su ID
    memcpy(buffer, &nroCliente, sizeof(long));
    if ( socketR16->send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al enviar mensaje a Robot16");
        abort();
    }

    try {
        /* Cola por la cual el canal le envía mensajes al cliente */
        IPC::MsgQueue R16_Cliente_Queue_("R16_Cliente_Queue");
        R16_Cliente_Queue_.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID_C);

        bool ultimoProductoDeODC = false;
        while ( not ultimoProductoDeODC ) {
            if ( socketR16->receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensaje del R16");
                abort();
            }

            Msg_EnvioCajaCliente mensaje;
            memcpy(&mensaje, buffer, sizeof(Msg_EnvioCajaCliente));
            ultimoProductoDeODC = mensaje.ultimoProductoDeODC_;
            R16_Cliente_Queue_.send(mensaje);
        }

        Logger::logMessage(Logger::COMM, "Comunicación terminada. Cerrando canal");
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }

    return 0;
}
