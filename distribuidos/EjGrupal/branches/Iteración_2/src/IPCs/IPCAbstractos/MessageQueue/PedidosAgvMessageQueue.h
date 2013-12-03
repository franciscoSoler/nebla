#ifndef PEDIDOS_AGV_MESSAGEQUEUE_H_
#define PEDIDOS_AGV_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"

namespace IPC {

class PedidosAgvMessageQueue : public AbstractMessageQueue
{

public:
	PedidosAgvMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idTipoReceptor = ID_TIPO_VACIO,
                TipoAgente idTipoAgente = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idTipoReceptor, idTipoAgente) {} 
	
	virtual ~PedidosAgvMessageQueue() {}

	int enviarPedidoAgv (long idReceptor, MensajePedidoAgv_5 dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idTipoReceptor = this->idTipoReceptor;
            msg.idReceptor = idReceptor;
            msg.idEmisor = this->idEmisor;
            msg.idIPCReceptor = this->idIPC;
            strcpy(msg.dirIPCReceptor, this->dirIPC);


            if ( sizeof(MensajePedidoAgv_5) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }
            memcpy(msg.msg, &dato, sizeof(MensajePedidoAgv_5));
            
            MsgQueue msgQ("queueAMux", idEmisor, this->idTipoReceptor, this->idTipoEmisor);
            msgQ.getMsgQueue(DIRECTORY_MUX, this->idTipoEmisor);
            msgQ.send(msg);
            return 0;
            //return this->enviar ((const void *) &dato, sizeof (MsgAgenteReceptor) - sizeof (long)); 
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

