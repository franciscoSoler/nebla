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
    
    void setReceiverType(TipoReceptor receiverType) {
        receiverType_ = receiverType;
    }

    void setReceiverAgentType(TipoAgente receiverAgentType) {
        receiverAgentType_ = receiverAgentType;
    }
    
    void setReceiverAdministratorType(CommTipoMensajeAgentes receiverAdministratorType) {
        receiverAdministratorType_ = receiverAdministratorType;
    }
    
    void setReceiverBrokerType(CommTipoMensajeBrokers receiverBrokerType) {
        receiverBrokerType_ = receiverBrokerType;
    }
    
    void setSenderAgentType(TipoAgente senderType) {
        senderAgentType_ = senderType;
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

        DireccionamientoMsgAgente dirAgente;
        dirAgente.idReceiverAgentType = receiverAgentType_;
        dirAgente.idReceptor = receiverId_;
        
        MsgCanalEntradaBroker msgCanalEntradaBroker;
        msgCanalEntradaBroker.receiverType = receiverType_;
        //msgCanalEntradaBroker.idReceptor = receiverId_;
        //msgCanalEntradaBroker.idTipoAgente = receiverAgentType_;
        memcpy(&msgCanalEntradaBroker.direccionamiento, &dirAgente, sizeof(DireccionamientoMsgAgente));
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

        DireccionamientoMsgAgente dirAgente;
        dirAgente.idReceiverAgentType = receiverAgentType_;
        dirAgente.idReceptor = receiverId_;
        
        MsgCanalEntradaBroker msgCanalEntradaBroker;
        msgCanalEntradaBroker.receiverType = receiverType_;
        //msgCanalEntradaBroker.idReceptor = receiverId_;
        //msgCanalEntradaBroker.idTipoAgente = receiverAgentType_;
        memcpy(&msgCanalEntradaBroker.direccionamiento, &dirAgente, sizeof(DireccionamientoMsgAgente));
        memcpy(&msgCanalEntradaBroker.msg, &msgCanalSalidaBroker, sizeof(MsgCanalSalidaBroker));

        msg.mtype = senderId_;
        memcpy(&msg.msg, &msgCanalEntradaBroker, sizeof(MsgCanalEntradaBroker));
    }
    
    void createPacketRequestShMem(MsgCanalSalidaAgente & msg) {

        MsgPedidoMemoriaAdministrador msgPedidoMemoriaAdministrador;
        msgPedidoMemoriaAdministrador.mtype = idShMem_;
        //msgPedidoMemoriaAdministrador.idReceptor = receiverId_;
        msgPedidoMemoriaAdministrador.idTipoEmisor = senderAgentType_;
        msgPedidoMemoriaAdministrador.idEmisor = senderId_;

        DireccionamientoMsgAdministrador dirAdm;
        dirAdm.idMemory = idShMem_;
        dirAdm.idMsgAdmType = receiverAdministratorType_;
        
        MsgCanalEntradaBroker msgCanalEntradaBroker;
        msgCanalEntradaBroker.receiverType = receiverType_;
        //msgCanalEntradaBroker.idReceptor = receiverId_;
        //msgCanalEntradaBroker.idTipoAgente = receiverAgentType_;
        memcpy(&msgCanalEntradaBroker.direccionamiento, &dirAdm, sizeof(DireccionamientoMsgAdministrador));
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
        
        msg.mtype = receiverId_;
        msg.msg = msgCanalEntradaAgente;
        memcpy(& msg.msg, &msgCanalEntradaAgente, sizeof(MsgCanalEntradaAgente));
    }
    
    template <class T>
    void createPacketReturnShMem(MsgCanalSalidaAgente & msg, const T & data) {

        MsgEntregaMemoriaAdministrador msgEntregaMemoriaAdministrador;
        msgEntregaMemoriaAdministrador.mtype = idShMem_;
        memcpy(& msgEntregaMemoriaAdministrador.memoria, &data, sizeof(T));
        
        DireccionamientoMsgAdministrador dirAdm;
        dirAdm.idMemory = idShMem_;
        dirAdm.idMsgAdmType = receiverAdministratorType_;
        
        MsgCanalEntradaBroker msgCanalEntradaBroker;
        msgCanalEntradaBroker.receiverType = receiverType_;
        //msgCanalEntradaBroker.idReceptor = receiverId_;
        //msgCanalEntradaBroker.idTipoAgente = receiverAgentType_;
        memcpy(&msgCanalEntradaBroker.direccionamiento, &dirAdm, sizeof(DireccionamientoMsgAdministrador));
        memcpy(&msgCanalEntradaBroker.msg, &msgEntregaMemoriaAdministrador, sizeof(MsgEntregaMemoriaAdministrador));

        msg.mtype = senderId_;
        memcpy(&msg.msg, &msgCanalEntradaBroker, sizeof(MsgCanalEntradaBroker));
    }

private:
    long receiverId_;
    long senderId_;
    TipoReceptor receiverType_;
    TipoAgente receiverAgentType_;
    CommTipoMensajeAgentes receiverAdministratorType_;
    CommTipoMensajeBrokers receiverBrokerType_;
    TipoAgente senderAgentType_;
    int idDirIPC_;
    int idShMem_;
    const char* dirIPC_;
};

#endif // COMMPACKETWRAPPER_H
