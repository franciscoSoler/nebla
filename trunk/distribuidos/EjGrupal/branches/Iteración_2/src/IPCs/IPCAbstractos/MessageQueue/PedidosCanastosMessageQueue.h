/* 
 * File:   PedidosCanastosMessageQueue.h
 * Author: francisco
 *
 * Created on 25 de octubre de 2013, 0:13
 */

#ifndef PEDIDOSCANASTOSMESSAGEQUEUE_H
#define	PEDIDOSCANASTOSMESSAGEQUEUE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"
#include <middlewareCommon.h>
#include <Logger/Logger.h>

namespace IPC {

class PedidosCanastosMessageQueue : public AbstractMessageQueue
{
private:
        int enviarPedidoCanasto (MsgAgenteReceptor dato) {
            int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MsgAgenteReceptor)-sizeof(long),0 );
            if (resultado == -1) {
                sprintf(this->buffer, "PedidosCanastosMessageQueue - Fallo la operacion send: %s", strerror(errno));
                throw Exception(std::string(this->buffer));
            }
            return resultado;
	}
	
	int recibirPedidoCanasto ( int tipo, MsgAgenteReceptor* buffer ) {
            int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MsgAgenteReceptor)-sizeof(long),tipo,0 );
            if (resultado == -1) {
                sprintf(this->buffer, "PedidosCanastosMessageQueue - Fallo la operacion recv: %s", strerror(errno));
                throw Exception(std::string(this->buffer));
            }
            return resultado;
	}

public:
	PedidosCanastosMessageQueue(std::string IPCName = "", long idEmisor = 0) : AbstractMessageQueue(IPCName, idEmisor) {} 
	
	virtual ~PedidosCanastosMessageQueue() {}

	int enviarPedidoCanasto (long idReceptor, MensajePedidoRobotCinta_6 dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idEmisor = this->idEmisor;
            msg.idReceptor = idReceptor;
            
            if ( sizeof(MensajePedidoRobotCinta_6) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }

            memcpy(msg.msg, &dato, sizeof(MensajePedidoRobotCinta_6));
            return this->enviarPedidoCanasto(msg);
	}
	
	int recibirPedidoCanasto ( int tipo, MensajePedidoRobotCinta_6* buffer ) {
            MsgAgenteReceptor msg;
            int resultado = this->recibirPedidoCanasto(tipo, &msg);
            memcpy(buffer, msg.msg, sizeof(MensajePedidoRobotCinta_6));
            return resultado;
	}

};

}

#endif	/* PEDIDOSCANASTOSMESSAGEQUEUE_H */

