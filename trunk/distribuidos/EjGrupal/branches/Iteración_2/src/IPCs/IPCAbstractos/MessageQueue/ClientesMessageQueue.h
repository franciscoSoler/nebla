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

namespace IPC {

class ClientesMessageQueue : public AbstractMessageQueue 
{
public:

    ClientesMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idTipoAgente = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idTipoAgente) {}

    virtual ~ClientesMessageQueue() {}

    int enviarMensajeRespuesta(long idReceptor, msg_respuesta_pedido_t dato) {
        
        
        
        MsgAgenteReceptor msg;
        msg.mtype = MSG_MUX;
        msg.idEmisor = this->idEmisor;
        msg.idReceptor = idReceptor;
        msg.idIPCReceptor = this->idIPC;
        strcpy(msg.dirIPCReceptor, this->dirIPC);


        if ( sizeof(msg_respuesta_pedido_t) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }
        memcpy(msg.msg, &dato, sizeof(msg_respuesta_pedido_t));

        MsgQueue msgQ("queueAMux", idEmisor, this->idTipoAgente);
        msgQ.getMsgQueue(DIRECTORY_MUX, this->idTipoAgente);
        msgQ.send(msg);
        return 0;
        return this->enviar ((const void *) &dato, sizeof (MsgAgenteReceptor) - sizeof (long)); 
    }

    int recibirMensajeRespuesta(int tipo, msg_respuesta_pedido_t* buffer) {
        MsgAgenteReceptor msg;
        int resultado = recibir(tipo, (void *)buffer, sizeof (MsgAgenteReceptor) - sizeof (long));

        memcpy(buffer, msg.msg, sizeof(msg_respuesta_pedido_t));
        return resultado;
    }
 
    int getMessageQueue(const char *fileName, int id) {
        this->idIPC = id;
        // Caso base de la recursividad: Código REEE entendible
        return AbstractMessageQueue::getMessageQueue(DIRECTORY_MUX, ID_TIPO_CLIENTE);
    }


};

}

#endif	/* CLIENTESMESSAGEQUEUE_H */

