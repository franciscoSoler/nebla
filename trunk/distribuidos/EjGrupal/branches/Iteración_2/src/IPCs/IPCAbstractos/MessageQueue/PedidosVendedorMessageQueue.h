/* 
 * File:   PedidosVendedorMessageQueue.h
 * Author: eze
 *
 * Created on November 4, 2013, 8:48 PM
 */

#ifndef PEDIDOSVENDEDORMESSAGEQUEUE_H
#define	PEDIDOSVENDEDORMESSAGEQUEUE_H



#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"

namespace IPC {
    
class PedidosVendedorMessageQueue : public AbstractMessageQueue 
{
public:

    PedidosVendedorMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idTipoReceptor = ID_TIPO_VACIO,
                TipoAgente idTipoAgente = ID_TIPO_VACIO) : AbstractMessageQueue
                (IPCName, idEmisor, idTipoReceptor, idTipoAgente) {}

    virtual ~PedidosVendedorMessageQueue() {}

    int enviarMensajePedido(long idReceptor, msg_pedido_t dato) {
        MsgAgenteReceptor msg;
        msg.mtype = MSG_MUX;
        msg.idTipoReceptor = this->idTipoReceptor;
        msg.idReceptor = idReceptor;
        msg.idEmisor = this->idEmisor;
        msg.idIPCReceptor = this->idIPC;
        strcpy(msg.dirIPCReceptor, this->dirIPC);


        if ( sizeof(msg_pedido_t) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer);
        }
        memcpy(msg.msg, &dato, sizeof(msg_pedido_t));
        
        MsgQueue msgQ("queueAMux", idEmisor, this->idTipoReceptor, this->idTipoEmisor);
        msgQ.getMsgQueue(DIRECTORY_MUX, this->idTipoEmisor);
        msgQ.send(msg);
        return 0;
        //return this->enviar((const void *)&dato,sizeof(MsgAgenteReceptor)-sizeof(long));
    }

    int recibirMensajePedido(int tipo, msg_pedido_t* buffer) {
        MsgAgenteReceptor msg;
        int resultado = recibir(tipo, (void *)buffer, sizeof (MsgAgenteReceptor) - sizeof (long));

        memcpy(buffer, msg.msg, sizeof(msg_pedido_t));
        return resultado;
    }

};

}


#endif	/* PEDIDOSVENDEDORMESSAGEQUEUE_H */

