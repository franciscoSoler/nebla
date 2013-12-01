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

namespace IPC {
    
class VendedorLibreMessageQueue : public AbstractMessageQueue
{
public:

    VendedorLibreMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idTipoReceptor = ID_TIPO_VACIO,
                TipoAgente idTipoAgente = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idTipoReceptor, idTipoAgente) {}

    virtual ~VendedorLibreMessageQueue() {}

    int enviarMensajeInicial(long idReceptor, mensaje_inicial_t dato) {
        MsgAgenteReceptor msg;
        msg.mtype = MSG_MUX;
        msg.idTipoReceptor = this->idTipoReceptor;
        msg.idReceptor = idReceptor;
        msg.idEmisor = this->idEmisor;
        msg.idIPCReceptor = this->idIPC;
        strcpy(msg.dirIPCReceptor, this->dirIPC);


        if ( sizeof(mensaje_inicial_t) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }
        memcpy(msg.msg, &dato, sizeof(mensaje_inicial_t));

        
        MsgQueue msgQ("queueAMux", idEmisor, this->idTipoAgente);
        msgQ.getMsgQueue(DIRECTORY_MUX, this->idTipoAgente);
        msgQ.send(msg);
        return 0;
        //return this->enviar((const void *)&dato,sizeof(MsgAgenteReceptor)-sizeof(long));
    }

    int recibirMensajeInicial(int tipo, mensaje_inicial_t* buffer) {
        MsgAgenteReceptor msg;
        int resultado = recibir(tipo, (void *)buffer, sizeof (MsgAgenteReceptor) - sizeof (long));

        memcpy(buffer, msg.msg, sizeof(mensaje_inicial_t));
        return resultado;
    }


};

}

#endif	/* VENDEDORESMESSAGEQUEUE_H */

