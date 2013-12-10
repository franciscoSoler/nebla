#ifndef COMMPACKETWRAPPER_H
#define COMMPACKETWRAPPER_H

#include <middlewareCommon.h>
#include <Logger/Logger.h>

class CommPacketWrapper {
public:
    void setReceiverId(long receiverId) {
        receiverId_ = receiverId;
    }

    void setSenderId(long senderId) {
        senderId_ = senderId;
    }

    void setReceiverType(TipoAgente receiverType) {
        receiverType_ = receiverType;
    }
    
    void setSenderType(TipoAgente senderType) {
        senderType_ = senderType;
    }

    void setIdDirIPC(int idDirIPC) {
        idDirIPC_ = idDirIPC;
    }

    void setDirIPC(const char* dirIPC) {
        dirIPC_ = dirIPC;
    }

    void setIdShMem(int idShMem) {
        idShMem_ = idShMem;
    }
    
    template <class T>
    void createPacketForQueues(MsgCanalSalidaAgente & msg, const T & data) {

        MsgAgenteReceptor msgAgenteReceptor;
        msgAgenteReceptor.mtype = receiverId_;
        memcpy(&msgAgenteReceptor.msg, &data, sizeof(T));

        MsgCanalEntradaAgente msgCanalEntradaAgente;
        strcpy(msgCanalEntradaAgente.directorioIPC, dirIPC_);
        msgCanalEntradaAgente.idIPC = idDirIPC_;
        memcpy(& msgCanalEntradaAgente.msg, &msgAgenteReceptor, sizeof(MsgAgenteReceptor));

        MsgCanalSalidaBroker msgCanalSalidaBroker;
        msgCanalSalidaBroker.mtype = receiverId_;
        msgCanalSalidaBroker.msg = msgCanalEntradaAgente;
        memcpy(& msgCanalSalidaBroker.msg, &msgCanalEntradaAgente, sizeof(MsgCanalEntradaAgente));

        MsgCanalEntradaBroker msgCanalEntradaBroker;
        msgCanalEntradaBroker.idReceptor = receiverId_;
        msgCanalEntradaBroker.idTipoReceptor = receiverType_;
        memcpy(&msgCanalEntradaBroker.msg, &msgCanalSalidaBroker, sizeof(MsgCanalSalidaBroker));

        msg.mtype = senderId_;
        memcpy(&msg.msg, &msgCanalEntradaBroker, sizeof(MsgCanalEntradaBroker));
    }
    
    void createPacketForSemaphores(MsgCanalSalidaAgente & msg) {

        MsgAgenteReceptor msgAgenteReceptor;
        msgAgenteReceptor.mtype = receiverId_;

        MsgCanalEntradaAgente msgCanalEntradaAgente;
        strcpy(msgCanalEntradaAgente.directorioIPC, dirIPC_);
        msgCanalEntradaAgente.idIPC = idDirIPC_;
        memcpy(& msgCanalEntradaAgente.msg, &msgAgenteReceptor, sizeof(MsgAgenteReceptor));

        MsgCanalSalidaBroker msgCanalSalidaBroker;
        msgCanalSalidaBroker.mtype = receiverId_;
        msgCanalSalidaBroker.msg = msgCanalEntradaAgente;
        memcpy(& msgCanalSalidaBroker.msg, &msgCanalEntradaAgente, sizeof(MsgCanalEntradaAgente));

        MsgCanalEntradaBroker msgCanalEntradaBroker;
        msgCanalEntradaBroker.idReceptor = receiverId_;
        msgCanalEntradaBroker.idTipoReceptor = receiverType_;
        memcpy(&msgCanalEntradaBroker.msg, &msgCanalSalidaBroker, sizeof(MsgCanalSalidaBroker));

        msg.mtype = senderId_;
        memcpy(&msg.msg, &msgCanalEntradaBroker, sizeof(MsgCanalEntradaBroker));
    }
    
    void createPacketRequestShMem(MsgCanalSalidaAgente & msg) {

        MsgPedidoMemoriaAdministrador msgPedidoMemoriaAdministrador;
        msgPedidoMemoriaAdministrador.mtype = idShMem_;
        msgPedidoMemoriaAdministrador.idReceptor = receiverId_;
        msgPedidoMemoriaAdministrador.idTipoEmisor = senderType_;
        msgPedidoMemoriaAdministrador.idEmisor = senderId_;

        MsgCanalEntradaBroker msgCanalEntradaBroker;
        msgCanalEntradaBroker.idReceptor = receiverId_;
        msgCanalEntradaBroker.idTipoReceptor = receiverType_;
        memcpy(&msgCanalEntradaBroker.msg, &msgPedidoMemoriaAdministrador, sizeof(MsgPedidoMemoriaAdministrador));

        msg.mtype = senderId_;
        memcpy(&msg.msg, &msgCanalEntradaBroker, sizeof(MsgCanalEntradaBroker));
    }
    
    template <class T>
    void createPacketReplyShMem(MsgCanalSalidaBroker & msg, const T & data) {

        MsgAgenteReceptor msgAgenteReceptor;
        msgAgenteReceptor.mtype = receiverId_;
        memcpy(&msgAgenteReceptor.msg, &data, sizeof(T));

        MsgCanalEntradaAgente msgCanalEntradaAgente;
        strcpy(msgCanalEntradaAgente.directorioIPC, dirIPC_);
        msgCanalEntradaAgente.idIPC = idDirIPC_;
        memcpy(& msgCanalEntradaAgente.msg, &msgAgenteReceptor, sizeof(MsgAgenteReceptor));
        
        msg.mtype = senderId_;
        msg.msg = msgCanalEntradaAgente;
        memcpy(& msg.msg, &msgCanalEntradaAgente, sizeof(MsgCanalEntradaAgente));
    }
    
    template <class T>
    void createPacketReturnShMem(MsgCanalSalidaAgente & msg, const T & data) {

        MsgEntregaMemoriaAdministrador msgEntregaMemoriaAdministrador;
        msgEntregaMemoriaAdministrador.mtype = idShMem_;
        memcpy(& msgEntregaMemoriaAdministrador.memoria, &data, sizeof(T));

        MsgCanalEntradaBroker msgCanalEntradaBroker;
        msgCanalEntradaBroker.idReceptor = receiverId_;
        msgCanalEntradaBroker.idTipoReceptor = receiverType_;
        memcpy(&msgCanalEntradaBroker.msg, &msgEntregaMemoriaAdministrador, sizeof(MsgEntregaMemoriaAdministrador));

        msg.mtype = senderId_;
        memcpy(&msg.msg, &msgCanalEntradaBroker, sizeof(MsgCanalEntradaBroker));
    }

private:
    long receiverId_;
    long senderId_;
    TipoAgente receiverType_;
    TipoAgente senderType_;
    int idDirIPC_;
    int idShMem_;
    const char* dirIPC_;
};

#endif // COMMPACKETWRAPPER_H
