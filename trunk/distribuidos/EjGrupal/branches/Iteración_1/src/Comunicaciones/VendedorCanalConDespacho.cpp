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
    Logger::setProcessInformation("VendedorCanalConDespacho:");
    char buffer[255];
    CommunicationsUtil util;
    long idVendedor = 0;

    if ( util.parseArgs(argc, argv, idVendedor) == -1 ) {
        exit(-1);
    }

    ServersManager serversManager;
    SocketStream::SocketStreamPtr socketDespacho(
    serversManager.connectToServer("ServidorDespachoVendedores") );
    assert( socketDespacho.get() );


    // Envío mi ID al Despacho (solo para debug por el momento)
    memcpy(buffer, &idVendedor, sizeof(long));
    if ( socketDespacho->send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al enviar mensaje a Despacho");
        abort();
    }


    try {
        /* Cola por la cual el canal le envía recibe mensajes del vendedor */
        IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
        inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);


        while ( true ) {
            // Primero espero un mensaje de envio de ODC
            Msg_PedidoDespacho primerMensajeSalida;
            inputQueueDespacho.recv(idVendedor, primerMensajeSalida);
            Logger::logMessage(Logger::COMM,
            "Recibe primer mensaje, procede a enviarlo por el canal");

            /* FIXME: Cambio forzado respecto de la versión concurrente
             * para que todo funcione
             */
            primerMensajeSalida.mtype = MSG_PEDIDO_DESPACHO;

            memcpy(buffer, &primerMensajeSalida, sizeof(Msg_PedidoDespacho));
            if ( socketDespacho->send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al enviar mensaje a Despacho");
                abort();
            }

            // Luego recibe la ODC y la envía por el canal
            Msg_EnvioODCDespacho segundoMensajeSalida;
            inputQueueDespacho.recv(idVendedor, segundoMensajeSalida);
            Logger::logMessage(Logger::COMM,
            "Recibe segundo mensaje, procede a enviarlo por el canal");

            /* FIXME: Cambio forzado respecto de la versión concurrente
             * para que todo funcione
             */
            segundoMensajeSalida.mtype = MSG_ENVIO_ODC_DESPACHO;

            memcpy(buffer, &segundoMensajeSalida, sizeof(Msg_EnvioODCDespacho));
            if ( socketDespacho->send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al enviar mensaje a Despacho");
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
