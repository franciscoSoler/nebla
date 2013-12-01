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
#include <middlewareCommon.h>
#include <Logger/Logger.h>

namespace IPC {
    
class PedidosVendedorMessageQueue : public AbstractMessageQueue 
{
public:

    PedidosVendedorMessageQueue(std::string IPCName = "", long idEmisor = 0) : AbstractMessageQueue(IPCName, idEmisor) {}

    virtual ~PedidosVendedorMessageQueue() {}

    int enviarMensajePedido(long idReceptor, msg_pedido_t dato) {
        MsgAgenteReceptor msg;
        msg.mtype = MSG_MUX;
        msg.idEmisor = this->idEmisor;
        msg.idReceptor = idReceptor;

        if ( sizeof(msg_pedido_t) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }

        memcpy(msg.msg, &dato, sizeof(msg_pedido_t));
        return this->enviar((const void *)&dato, sizeof(MsgAgenteReceptor)-sizeof(long));
    }

    int recibirMensajePedido(int tipo, msg_pedido_t* buffer) {
        MsgAgenteReceptor msg;
        int resultado = this->recibir(tipo, (void *)buffer, sizeof (MsgAgenteReceptor) - sizeof (long));
        memcpy(buffer, msg.msg, sizeof(msg_pedido_t));
        return resultado;
    }

};

}


#endif	/* PEDIDOSVENDEDORMESSAGEQUEUE_H */

