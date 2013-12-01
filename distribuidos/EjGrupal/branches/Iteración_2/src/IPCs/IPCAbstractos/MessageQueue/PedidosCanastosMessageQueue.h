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

namespace IPC {

class PedidosCanastosMessageQueue : public AbstractMessageQueue
{
private:
        int send (MsgAgenteReceptor dato) {
            int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MsgAgenteReceptor)-sizeof(long),0 );
            if (resultado == -1) {
                    sprintf(this->buffer, "Barrera1112MessageQueue - Fallo la operacion send: %s", strerror(errno));
                    throw Exception(std::string(this->buffer));
            }
            return resultado;
	}
	
	int receive ( int tipo, MsgAgenteReceptor* buffer ) {
            int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MsgAgenteReceptor)-sizeof(long),tipo,0 );
            if (resultado == -1) {
                    sprintf(this->buffer, "Barrera1112MessageQueue - Fallo la operacion recv: %s", strerror(errno));
                    throw Exception(std::string(this->buffer));
            }
            return resultado;
	}
    
public:
	PedidosCanastosMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idTipoReceptor = ID_TIPO_VACIO,
                TipoAgente idTipoAgente = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idTipoReceptor, idTipoAgente) {} 
	
	virtual ~PedidosCanastosMessageQueue() {}

	int enviarPedidoCanasto (long idReceptor, MensajePedidoRobotCinta_6 dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idTipoReceptor = this->idTipoReceptor;
            msg.idReceptor = idReceptor;
            msg.idEmisor = this->idEmisor;
            msg.idIPCReceptor = this->idIPC;
            strcpy(msg.dirIPCReceptor, this->dirIPC);


            if ( sizeof(MensajePedidoRobotCinta_6) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }
            memcpy(msg.msg, &dato, sizeof(MensajePedidoRobotCinta_6));
            
            MsgQueue msgQ("queueAMux", idEmisor, this->idTipoReceptor, this->idTipoAgente);
            msgQ.getMsgQueue(DIRECTORY_MUX, this->idTipoAgente);
            msgQ.send(msg);
            return 0;
            //return this->send(msg);
	}
	
	int recibirPedidoCanasto ( int tipo, MensajePedidoRobotCinta_6* buffer ) {
            MsgAgenteReceptor msg;
            int resultado = this->receive(tipo, &msg);

            memcpy(buffer, msg.msg, sizeof(MensajePedidoRobotCinta_6));
            return resultado;
	}

};

}

#endif	/* PEDIDOSCANASTOSMESSAGEQUEUE_H */

