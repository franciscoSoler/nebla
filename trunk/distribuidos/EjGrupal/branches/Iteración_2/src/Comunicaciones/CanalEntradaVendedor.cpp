#include "../IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "../IPCs/IPCAbstractos/MessageQueue/VendedorLibreMessageQueue.h"

#include "../Common.h"
#include "Logger.h"
#include "LockFile.h"
#include "PedidosVendedorMessageQueue.h"

#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Socket/SocketStream.h>


int main(int argc, char **argv) {
    char buffer[TAM_BUFFER];    
    Logger::getInstance().setProcessInformation("Canal Entrada Vendedor:");
    int sd;
    CommunicationsUtil util;

    if ( util.parseArgs(argc, argv, sd) == -1) {
        exit(-1);
    }

    Logger::logMessage(Logger::COMM, "Conectando canal de entrada");
    SocketStream socketEntrada(sd);

    try {
        IPC::VendedorLibreMessageQueue getVendedorQueue = IPC::VendedorLibreMessageQueue("VendedoresMessageQueue");
        getVendedorQueue.getMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);
	
        /* Obtengo un vendedor libre */
        mensaje_inicial_t handShake; 
        getVendedorQueue.recibirMensajeInicial(TIPO_VENDEDOR_LIBRE, &handShake);
		
        int nroVendedor = handShake.emisor;
    
        sprintf(buffer, "Canal Entrada Vendedor %d",nroVendedor);
        Logger::getInstance().setProcessInformation(buffer);

        Logger::logMessage(Logger::COMM,"Recibe un vendedor libre");
        
        IPC::PedidosVendedorMessageQueue pedidos = IPC::PedidosVendedorMessageQueue("PedidosMsgQueue");
        pedidos.getMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);
	
        /* Envio el numero de vendedor correspondiente */
        char nroVendedorChar[TAM_BUFFER];
        StartComunicationMessage startMsg;
        startMsg.numero = nroVendedor;
        memcpy(nroVendedorChar, &startMsg, sizeof(StartComunicationMessage));

        sprintf(buffer, "Enviando vendedor %d por el socket %d", nroVendedor, socketEntrada.getSd());
        Logger::logMessage(Logger::COMM, buffer);

        if ( socketEntrada.send(nroVendedorChar, TAM_BUFFER) != TAM_BUFFER ) {
            Logger::logMessage(Logger::ERROR,"Error al enviar el numero de vendedor.");
        }

        bool deboSeguir = true;
        while (deboSeguir) {

            net_msg_pedido_t netMsg;
            msg_pedido_t pedido;
		
            if ( socketEntrada.receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                sprintf(buffer, "Error al recibir un pedido de ticket. ERROR: %d.", errno);
                Logger::logMessage(Logger::ERROR, buffer);
                socketEntrada.destroy();
                exit(-1);
            }

            memcpy(&netMsg, buffer, sizeof(net_msg_pedido_t));
            sprintf(buffer, "Recibió un pedido. Destino: %d Origen: %d Tipo: %d.", netMsg.destino, netMsg.origen, netMsg.tipo);
            Logger::logMessage(Logger::COMM, buffer);
		
            pedido = netMsg.mensaje;
            pedidos.enviarMensajePedido(pedido);
		
            if (netMsg.tipo != 0) {
                deboSeguir = false;
                Logger::logMessage(Logger::COMM,"El cliente finalizó la compra.");
            }
        }

        socketEntrada.destroy();
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}
