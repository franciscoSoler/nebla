/* 
 * File:   Barrera1112MessageQueue.h
 * Author: francisco
 *
 * Created on 25 de octubre de 2013, 0:50
 */

#ifndef BARRERA1112MESSAGEQUEUE_H
#define	BARRERA1112MESSAGEQUEUE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <Logger/Logger.h>
#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include <Comunicaciones/Objects/CommPacketWrapper.h>

namespace IPC {

class Barrera1112MessageQueue : public AbstractMessageQueue
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
	Barrera1112MessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idDuenioEstaCola = ID_TIPO_VACIO, 
                TipoAgente idDuenioColaRemota = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idDuenioEstaCola, idDuenioColaRemota) {} 
	
	virtual ~Barrera1112MessageQueue() {}

	int send (long idReceptor, MensajeBarrera dato) {
            CommPacketWrapper wrapper;
            wrapper.setDirIPC( dirIPC );
            wrapper.setIdDirIPC( idIPC );
            wrapper.setSenderId( idEmisor );
            wrapper.setReceiverType( idDuenioColaRemota_ );
            wrapper.setReceiverId( idReceptor );
            MsgCanalSalidaAgente msg;
            wrapper.createPacketForQueues(msg, dato);


            if ( sizeof(MensajeBarrera) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }
            
            MsgQueue queue("queueAMux");
            queue.getMsgQueue(DIRECTORY_COMM, this->idDuenioEstaCola_);
            queue.send(msg);
            return 0;
	}
	
	int receive ( int tipo, MensajeBarrera* buffer ) {
            MsgAgenteReceptor msg;
            int resultado = this->receive(tipo, &msg);

            memcpy(buffer, msg.msg, sizeof(MensajeBarrera));
            return resultado;
	}

};

}

#endif	/* BARRERA1112MESSAGEQUEUE_H */

