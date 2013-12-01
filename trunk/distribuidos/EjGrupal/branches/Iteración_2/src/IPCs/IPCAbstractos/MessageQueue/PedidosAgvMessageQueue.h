#ifndef PEDIDOS_AGV_MESSAGEQUEUE_H_
#define PEDIDOS_AGV_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"
#include <middlewareCommon.h>
#include <Logger/Logger.h>

namespace IPC {

class PedidosAgvMessageQueue : public AbstractMessageQueue
{

public:
	PedidosAgvMessageQueue(std::string IPCName = "", long idEmisor = 0) : AbstractMessageQueue(IPCName, idEmisor) {} 
	
	virtual ~PedidosAgvMessageQueue() {}

	int enviarPedidoAgv (long idReceptor, MensajePedidoAgv_5 dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idEmisor = this->idEmisor;
            msg.idReceptor = idReceptor;

            if ( sizeof(MensajePedidoAgv_5) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }

            memcpy(msg.msg, &dato, sizeof(MensajePedidoAgv_5));
            return this->enviar((const void *)&dato, sizeof(MsgAgenteReceptor)-sizeof(long));
	}
	
	int recibirPedidoAgv ( int tipo, MensajePedidoAgv_5* buffer ) {
            MsgAgenteReceptor msg;
            int resultado = this->recibir(tipo, (void *)buffer, sizeof (MsgAgenteReceptor) - sizeof (long));
            memcpy(buffer, msg.msg, sizeof(MensajePedidoAgv_5));
            return resultado;
	}

};

}

#endif // PEDIDOS_AGV_MESSAGEQUEUE_H_

