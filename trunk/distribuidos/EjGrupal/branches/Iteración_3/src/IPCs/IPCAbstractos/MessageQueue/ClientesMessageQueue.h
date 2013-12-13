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

#include <Comunicaciones/Objects/CommPacketWrapper.h>

namespace IPC {

class ClientesMessageQueue : public AbstractMessageQueue 
{
public:

    ClientesMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idDuenioEstaCola = ID_TIPO_VACIO,
                TipoAgente idDuenioColaRemota_ = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idDuenioEstaCola, idDuenioColaRemota_) {}

    virtual ~ClientesMessageQueue() {}

    int enviarMensajeRespuesta(long idReceptor, msg_respuesta_pedido_t dato) {
        CommPacketWrapper wrapper;
        wrapper.setDirIPC( dirIPC );
        wrapper.setIdDirIPC( idIPC );
        wrapper.setSenderId( idEmisor );
        wrapper.setReceiverType( idDuenioColaRemota_ );
        wrapper.setReceiverId( idReceptor );
        MsgCanalSalidaAgente msg;
        wrapper.createPacketForQueues(msg, dato);


        if ( sizeof(msg_respuesta_pedido_t) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }

        MsgQueue msgQ("queueAColaSalida");
        msgQ.getMsgQueue(DIRECTORY_COMM, this->idDuenioEstaCola_);
        msgQ.send(msg);
        return 0;
    }

    int recibirMensajeRespuesta(int tipo, msg_respuesta_pedido_t* buffer) {
        MsgAgenteReceptor msg;
        int resultado = recibir(tipo, (void *) & msg, sizeof (MsgAgenteReceptor) - sizeof (long));

        memcpy(buffer, msg.msg, sizeof(msg_respuesta_pedido_t));
        return resultado;
    }
};

}

#endif	/* CLIENTESMESSAGEQUEUE_H */

