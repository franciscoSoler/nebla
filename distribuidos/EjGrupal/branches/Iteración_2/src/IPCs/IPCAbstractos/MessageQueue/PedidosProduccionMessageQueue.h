#ifndef PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_
#define PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

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
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idTipoReceptor = this->idDuenioColaRemota_;
            msg.idReceptor = idReceptor;
            msg.idEmisor = this->idEmisor;
            msg.idIPCReceptor = this->idIPC;
            strcpy(msg.dirIPCReceptor, this->dirIPC);


            if ( sizeof(MensajePedidoProduccion) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }
            memcpy(msg.msg, &dato, sizeof(MensajePedidoProduccion));
            
            MsgQueue msgQ("queueAMux", idEmisor, this->idDuenioEstaCola_, this->idDuenioColaRemota_);
            msgQ.getMsgQueue(DIRECTORY_MUX, this->idDuenioEstaCola_);
            msgQ.send(msg);
            return 0;
            //return this->enviar((const void *)&msg,sizeof(MsgAgenteReceptor)-sizeof(long));
	}
	
	int recibirPedidoProduccion ( int tipo, MensajePedidoProduccion* buffer ) {
            MsgAgenteReceptor msg;
            int resultado = recibir(tipo, (void *) & msg, sizeof (MsgAgenteReceptor) - sizeof (long));

            memcpy(buffer, msg.msg, sizeof(MensajePedidoProduccion));
            return resultado;
	}
        
        int enviarProximoPedidoProduccion (long idReceptor, MensajeProximoPedidoProduccion dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idTipoReceptor = this->idDuenioColaRemota_;
            msg.idReceptor = idReceptor;
            msg.idEmisor = this->idEmisor;
            msg.idIPCReceptor = this->idIPC;
            strcpy(msg.dirIPCReceptor, this->dirIPC);


            if ( sizeof(MensajeProximoPedidoProduccion) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }
            memcpy(msg.msg, &dato, sizeof(MensajeProximoPedidoProduccion));
            
            MsgQueue msgQ("queueAMux", idEmisor, this->idDuenioEstaCola_, this->idDuenioColaRemota_);
            msgQ.getMsgQueue(DIRECTORY_MUX, this->idDuenioEstaCola_);
            msgQ.send(msg);
            return 0;
            //return this->enviar((const void *)&dato,sizeof(MsgAgenteReceptor)-sizeof(long));
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

