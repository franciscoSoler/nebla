#ifndef PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_
#define PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"
#include <Logger/Logger.h>

#include <Comunicaciones/Objects/CommPacketWrapper.h>

namespace IPC {
    
class PedidosProduccionMessageQueue : public AbstractMessageQueue
{

public:
	PedidosProduccionMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idDuenioEstaCola = ID_TIPO_VACIO,
                TipoAgente idDuenioColaRemota = ID_TIPO_VACIO):AbstractMessageQueue
                (IPCName, idEmisor, idDuenioEstaCola, idDuenioColaRemota) {} 
	
	virtual ~PedidosProduccionMessageQueue() {}

	int enviarPedidoProduccion (long idReceptor, MensajePedidoProduccion dato) {
            CommPacketWrapper wrapper;
            wrapper.setReceiverType( AGENTE );
            
            wrapper.setDirIPC( dirIPC );
            wrapper.setIdDirIPC( idIPC );
            wrapper.setSenderId( idEmisor );
            wrapper.setReceiverAgentType( idDuenioColaRemota_ );
            wrapper.setReceiverId( idReceptor );
            MsgCanalSalidaAgente msg;
            wrapper.createPacketForQueues(msg, dato);


            if ( sizeof(MensajePedidoProduccion) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }
            
            MsgQueue msgQ("queue");
            msgQ.getMsgQueue(DIRECTORY_COMM, this->idDuenioEstaCola_);
            msgQ.send(msg);
            return 0;
	}
	
	int recibirPedidoProduccion ( int tipo, MensajePedidoProduccion* buffer ) {
            MsgAgenteReceptor msg;
            int resultado = recibir(tipo, (void *) & msg, sizeof (MsgAgenteReceptor) - sizeof (long));

            memcpy(buffer, msg.msg, sizeof(MensajePedidoProduccion));
            return resultado;
	}
        
    int enviarProximoPedidoProduccion (long idReceptor, MensajeProximoPedidoProduccion dato) {
        CommPacketWrapper wrapper;
        wrapper.setReceiverType( AGENTE );
        
        wrapper.setDirIPC( dirIPC );
        wrapper.setIdDirIPC( idIPC );
        wrapper.setSenderId( idEmisor );
        wrapper.setReceiverAgentType( idDuenioColaRemota_ );
        wrapper.setReceiverId( idReceptor );
        MsgCanalSalidaAgente msg;
        wrapper.createPacketForQueues(msg, dato);


        if ( sizeof(MensajeProximoPedidoProduccion) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }

        MsgQueue msgQ("queue");
        msgQ.getMsgQueue(DIRECTORY_COMM, this->idDuenioEstaCola_);
        msgQ.send(msg);
        return 0;
	}
	
	int recibirProximoPedidoProduccion ( int tipo, MensajeProximoPedidoProduccion* buffer ) {
            MsgAgenteReceptor msg;
            int resultado = recibir(tipo, (void *) & msg, sizeof (MsgAgenteReceptor) - sizeof (long));

            memcpy(buffer, msg.msg, sizeof(MensajeProximoPedidoProduccion));
            return resultado;
	}

};

}

#endif // PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_

