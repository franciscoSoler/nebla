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

    void setIdDirIPC(int idDirIPC) {
        idDirIPC_ = idDirIPC;
    }

    void setDirIPC(const char* dirIPC) {
        dirIPC_ = dirIPC;
    }

    template <class T>
    void createPacket(MsgCanalSalidaAgente & msg, const T & data) {

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

private:
    long receiverId_;
    long senderId_;
    TipoAgente receiverType_;
    int idDirIPC_;
    const char* dirIPC_;
};

#endif // COMMPACKETWRAPPER_H
