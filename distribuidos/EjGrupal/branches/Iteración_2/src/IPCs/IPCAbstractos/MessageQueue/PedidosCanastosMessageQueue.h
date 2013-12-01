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
#include <Logger/Logger.h>

namespace IPC {

class PedidosCanastosMessageQueue : public AbstractMessageQueue
{
public:
	PedidosCanastosMessageQueue(std::string IPCName = "", 
                long idEmisor = 0, TipoAgente idTipoAgente = ID_TIPO_VACIO) 
                     : AbstractMessageQueue(IPCName, idEmisor, idTipoAgente) {} 
	
	virtual ~PedidosCanastosMessageQueue() {}

	int enviarPedidoCanasto (long idReceptor, MensajePedidoRobotCinta_6 dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idEmisor = this->idEmisor;
            msg.idReceptor = idReceptor;
            msg.idIPCReceptor = this->idIPC;
            strcpy(msg.dirIPCReceptor, this->dirIPC);
            
            if ( sizeof(MensajePedidoRobotCinta_6) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }

            memcpy(msg.msg, &dato, sizeof(MensajePedidoRobotCinta_6));
            this->colaMux->send(msg);
            return 0;
	}
	
	int recibirPedidoCanasto ( int tipo, MensajePedidoRobotCinta_6* buffer ) {
            MsgAgenteReceptor msg;
            this->colaMux->recv(tipo, msg);
            memcpy(buffer, msg.msg, sizeof(MensajePedidoRobotCinta_6));
            return 0;
	}

};

}

#endif	/* PEDIDOSCANASTOSMESSAGEQUEUE_H */

