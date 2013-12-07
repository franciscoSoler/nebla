#ifndef PEDIDOS_AGV_MESSAGEQUEUE_H_
#define PEDIDOS_AGV_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"
#include <Common.h>
#include <Logger/Logger.h>

#include <Comunicaciones/Objects/CommPacketWrapper.h>

namespace IPC {

class PedidosAgvMessageQueue : public AbstractMessageQueue
{

public:
	PedidosAgvMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idDuenioEstaCola_ = ID_TIPO_VACIO,
                TipoAgente idDuenioColaRemota = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idDuenioEstaCola_, idDuenioColaRemota) {} 
	
	virtual ~PedidosAgvMessageQueue() {}

	int enviarPedidoAgv (long idReceptor, MensajePedidoAgv_5 dato) {
            CommPacketWrapper wrapper;
            wrapper.setDirIPC( dirIPC );
            wrapper.setIdDirIPC( idIPC );
            wrapper.setSenderId( idEmisor );
            wrapper.setReceiverType( idDuenioColaRemota_ );
            wrapper.setReceiverId( idReceptor );
            MsgCanalSalidaAgente msg;
            wrapper.createPacketForQueues(msg, dato);


            if ( sizeof(MensajePedidoAgv_5) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }
            
            MsgQueue msgQ("queue");
            msgQ.getMsgQueue(DIRECTORY_COMM, this->idDuenioEstaCola_);
            msgQ.send(msg);
            return 0;
	}
	
	int recibirPedidoAgv ( int tipo, MensajePedidoAgv_5* buffer ) {
            MsgAgenteReceptor msg;
            int resultado = recibir(tipo, (void *) & msg, sizeof (MsgAgenteReceptor) - sizeof (long));

            memcpy(buffer, msg.msg, sizeof(MensajePedidoAgv_5));
            return resultado;
	}

};

}

#endif // PEDIDOS_AGV_MESSAGEQUEUE_H_

