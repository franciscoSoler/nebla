/* 
 * File:   VendedoresMessageQueue.h
 * Author: eze
 *
 * Created on November 4, 2013, 8:46 PM
 */

#ifndef VENDEDORESMESSAGEQUEUE_H
#define	VENDEDORESMESSAGEQUEUE_H

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"
#include <middlewareCommon.h>
#include <Logger/Logger.h>

namespace IPC {
    
class VendedorLibreMessageQueue : public AbstractMessageQueue
{
public:

    VendedorLibreMessageQueue(std::string IPCName = "", long idEmisor = 0) : AbstractMessageQueue(IPCName, idEmisor) {}

    virtual ~VendedorLibreMessageQueue() {}

    int enviarMensajeInicial(long idReceptor, mensaje_inicial_t dato) {
        MsgAgenteReceptor msg;
        msg.mtype = MSG_MUX;
        msg.idEmisor = this->idEmisor;
        msg.idReceptor = idReceptor;

        if ( sizeof(mensaje_inicial_t) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }

        memcpy(msg.msg, &dato, sizeof(mensaje_inicial_t));
        return this->enviar((const void *)&dato, sizeof(MsgAgenteReceptor)-sizeof(long));
    }

    int recibirMensajeInicial(int tipo, mensaje_inicial_t* buffer) {
        MsgAgenteReceptor msg;
        int resultado = this->recibir(tipo, (void *)buffer, sizeof (MsgAgenteReceptor) - sizeof (long));
        memcpy(buffer, msg.msg, sizeof(mensaje_inicial_t));
        return resultado;
    }


};

}

#endif	/* VENDEDORESMESSAGEQUEUE_H */

