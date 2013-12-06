/* 
 * File:   PedidosVendedorMessageQueue.h
 * Author: eze
 *
 * Created on November 4, 2013, 8:48 PM
 */

#ifndef PEDIDOSVENDEDORMESSAGEQUEUE_H
#define	PEDIDOSVENDEDORMESSAGEQUEUE_H



#include "../AbstractMessageQueue/AbstractMessageQueue.h"
#include "../../../Common.h"

#include <Comunicaciones/Objects/CommPacketWrapper.h>

namespace IPC {
    
class PedidosVendedorMessageQueue : public AbstractMessageQueue 
{
public:

    PedidosVendedorMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idDuenioEstaCola = ID_TIPO_VACIO,
                TipoAgente idDuenioColaRemota = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idDuenioEstaCola, idDuenioColaRemota) {}

    virtual ~PedidosVendedorMessageQueue() {}

    int enviarMensajePedido(long idReceptor, msg_pedido_t dato) {
        CommPacketWrapper wrapper;
        wrapper.setDirIPC( dirIPC );
        wrapper.setIdDirIPC( idIPC );
        wrapper.setSenderId( idEmisor );
        wrapper.setReceiverType( idDuenioColaRemota_ );
        wrapper.setReceiverId( idReceptor );
        MsgCanalSalidaAgente msg;
        wrapper.createPacket(msg, dato);

        if ( sizeof(msg_pedido_t) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }
        
        MsgQueue msgQ("queueAColaSalida");
        msgQ.getMsgQueue(DIRECTORY_COMM, this->idDuenioEstaCola_);
        msgQ.send(msg);
        return 0;
    }

    int recibirMensajePedido(int tipo, msg_pedido_t* buffer) {
        MsgAgenteReceptor msg;
        int resultado = recibir(tipo, (void *) & msg, sizeof (MsgAgenteReceptor) - sizeof (long));

        memcpy(buffer, msg.msg, sizeof(msg_pedido_t));
        return resultado;
    }

};

}


#endif	/* PEDIDOSVENDEDORMESSAGEQUEUE_H */

