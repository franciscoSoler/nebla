#ifndef PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_
#define PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"
#include <Logger/Logger.h>

namespace IPC {
    
class PedidosProduccionMessageQueue : public AbstractMessageQueue
{

public:
	PedidosProduccionMessageQueue(std::string IPCName = "", long 
                idEmisor = 0, TipoAgente idTipoAgente = ID_TIPO_VACIO) 
                : AbstractMessageQueue(IPCName, idEmisor, idTipoAgente) {} 
	
	virtual ~PedidosProduccionMessageQueue() {}

	int enviarPedidoProduccion (long idReceptor, MensajePedidoProduccion dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idEmisor = this->idEmisor;
            msg.idReceptor = idReceptor;
            msg.idIPCReceptor = this->idIPC;
            strcpy(msg.dirIPCReceptor, this->dirIPC);

            if ( sizeof(MensajePedidoProduccion) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }

            memcpy(msg.msg, &dato, sizeof(MensajePedidoProduccion));
            this->colaMux->send(msg);
            return 0;
	}
	
	int recibirPedidoProduccion ( int tipo, MensajePedidoProduccion* buffer ) {
            MsgAgenteReceptor msg;
            this->colaMux->recv(tipo, msg);
            memcpy(buffer, msg.msg, sizeof(MensajePedidoProduccion));
            return 0;
	}
        
        int enviarProximoPedidoProduccion (long idReceptor, MensajeProximoPedidoProduccion dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idEmisor = this->idEmisor;
            msg.idReceptor = idReceptor;
            msg.idIPCReceptor = this->idIPC;
            strcpy(msg.dirIPCReceptor, this->dirIPC);

            if ( sizeof(MensajeProximoPedidoProduccion) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }

            memcpy(msg.msg, &dato, sizeof(MensajeProximoPedidoProduccion));
            this->colaMux->send(msg);
            return 0;
	}
	
	int recibirProximoPedidoProduccion ( int tipo, MensajeProximoPedidoProduccion* buffer ) {
            MsgAgenteReceptor msg;
            this->colaMux->recv(tipo, msg);
            memcpy(buffer, msg.msg, sizeof(MensajeProximoPedidoProduccion));
            return 0;
	}

};

}

#endif // PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_

