#ifndef PEDIDOS_AGV_MESSAGEQUEUE_H_
#define PEDIDOS_AGV_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"
#include <Logger/Logger.h>

namespace IPC {

class PedidosAgvMessageQueue : public AbstractMessageQueue
{

public:
	PedidosAgvMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idTipoAgente = ID_TIPO_VACIO) : AbstractMessageQueue(IPCName, 
                        idEmisor, idTipoAgente) {} 
	
	virtual ~PedidosAgvMessageQueue() {}

	int enviarPedidoAgv (long idReceptor, MensajePedidoAgv_5 dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idEmisor = this->idEmisor;
            msg.idReceptor = idReceptor;
            msg.idIPCReceptor = this->idIPC;
            strcpy(msg.dirIPCReceptor, this->dirIPC);

            if ( sizeof(MensajePedidoAgv_5) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }

            memcpy(msg.msg, &dato, sizeof(MensajePedidoAgv_5));
            this->colaMux->send(msg);
            return 0;
	}
	
	int recibirPedidoAgv ( int tipo, MensajePedidoAgv_5* buffer ) {
            MsgAgenteReceptor msg;
            this->colaMux->recv(tipo, msg);
            memcpy(buffer, msg.msg, sizeof(MensajePedidoAgv_5));
            return 0;
	}

};

}

#endif // PEDIDOS_AGV_MESSAGEQUEUE_H_

