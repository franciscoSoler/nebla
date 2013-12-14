/* 
 * File:   VendedoresMessageQueue.h
 * Author: eze
 *
 * Created on November 4, 2013, 8:46 PM
 */

#ifndef VENDEDORESMESSAGEQUEUE_H
#define	VENDEDORESMESSAGEQUEUE_H

#include "../AbstractMessageQueue/AbstractMessageQueue.h"
#include <Common.h>
#include <Logger/Logger.h>

#include <Comunicaciones/Objects/CommPacketWrapper.h>

namespace IPC {
    
class VendedorLibreMessageQueue : public AbstractMessageQueue
{
public:

    VendedorLibreMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idDuenioEstaCola = ID_TIPO_VACIO,
                TipoAgente idDuenioColaRemota = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idDuenioEstaCola, idDuenioColaRemota) {}

    virtual ~VendedorLibreMessageQueue() {}

    int enviarMensajeInicial(long idReceptor, mensaje_inicial_t dato) {
        CommPacketWrapper wrapper;
        wrapper.setReceiverType( AGENTE );
        
        wrapper.setDirIPC( dirIPC );
        wrapper.setIdDirIPC( idIPC );
        wrapper.setSenderId( idEmisor );
        wrapper.setReceiverAgentType( idDuenioColaRemota_ );
        wrapper.setReceiverId( idReceptor );
        MsgCanalSalidaAgente msg;
        wrapper.createPacketForQueues(msg, dato);


        if ( sizeof(mensaje_inicial_t) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }

        
        sprintf(this->buffer, "idEmisor: %ld, idTipoReceptor: %d, idTipoAgente: %d",
                    idEmisor, this->idDuenioEstaCola_, this->idDuenioColaRemota_);
        Logger::logMessage(Logger::TRACE, this->buffer);
        
        MsgQueue msgQ("queueAColaSalida");
        msgQ.getMsgQueue(DIRECTORY_COMM, this->idDuenioEstaCola_);
        msgQ.send(msg);
        return 0;
    }

    int recibirMensajeInicial(int tipo, mensaje_inicial_t* buffer) {
        MsgAgenteReceptor msg;
        int resultado = recibir(tipo, (void *) & msg, sizeof (MsgAgenteReceptor) - sizeof (long));

        memcpy(buffer, msg.msg, sizeof(mensaje_inicial_t));
        return resultado;
    }


};

}

#endif	/* VENDEDORESMESSAGEQUEUE_H */

