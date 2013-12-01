/* 
 * File:   ClientesMessageQueue.h
 * Author: eze
 *
 * Created on November 4, 2013, 8:44 PM
 */

#ifndef CLIENTESMESSAGEQUEUE_H
#define	CLIENTESMESSAGEQUEUE_H

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"
#include <middlewareCommon.h>
#include <Logger/Logger.h>

namespace IPC {

class ClientesMessageQueue : public AbstractMessageQueue 
{
public:

    ClientesMessageQueue(std::string IPCName = "", long idEmisor = 0) : AbstractMessageQueue(IPCName, idEmisor) {}

    virtual ~ClientesMessageQueue() {}

    int enviarMensajeRespuesta(long idReceptor, msg_respuesta_pedido_t dato) {
        MsgAgenteReceptor msg;
        msg.mtype = MSG_MUX;
        msg.idEmisor = this->idEmisor;
        msg.idReceptor = idReceptor;

        if ( sizeof(msg_respuesta_pedido_t) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }

        memcpy(msg.msg, &dato, sizeof(MensajeBarrera));
        return this->enviar ((const void *) &msg, sizeof (MsgAgenteReceptor) - sizeof (long)); 
    }

    int recibirMensajeRespuesta(int tipo, msg_respuesta_pedido_t* buffer) {
        MsgAgenteReceptor msg;
        int resultado = this->recibir(tipo, (void *)buffer, sizeof (MsgAgenteReceptor) - sizeof (long));
        memcpy(buffer, msg.msg, sizeof(msg_respuesta_pedido_t));
        return resultado;
    }


};

}

#endif	/* CLIENTESMESSAGEQUEUE_H */

