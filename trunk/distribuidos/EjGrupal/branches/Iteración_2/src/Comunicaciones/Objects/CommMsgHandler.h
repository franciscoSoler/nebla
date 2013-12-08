#ifndef COMM_MSG_HANDLER_H_
#define COMM_MSG_HANDLER_H_

#include <IPCs/IPCTemplate/MsgQueue.h>
#include <string>
#include <Logger/Logger.h>
#include <Common.h>
#include <iostream>

#include <middlewareCommon.h>
#include <Comunicaciones/Objects/CommPacketWrapper.h>

namespace COMM {

class CommMsgHandler {
public:
    CommMsgHandler(long senderId = 0,
                   TipoAgente senderTypeId = ID_TIPO_VACIO,
                   TipoAgente receiverTypeId = ID_TIPO_VACIO)
        : senderId_(senderId),
          senderTypeId_(senderTypeId),
          receiverTypeId_(receiverTypeId) {

        try {
            if ( senderTypeId_ != ID_TIPO_VACIO ) {
                senderMsgQueue_.getMsgQueue(DIRECTORY_COMM, senderTypeId_);
            }
        }
        catch(Exception & e) {
            Logger::logMessage(Logger::ERROR, e.get_error_description());
            abort();
        }
    }

    void setReceptorInfo(std::string name,
                         const char* dirIPC, int idIPC,
                         TipoAgente receiverTypeId = ID_TIPO_VACIO) {

        if ( receiverTypeId != ID_TIPO_VACIO ) {
            receiverTypeId_ = receiverTypeId;
            /* sprintf(buffer_, "MsgQueue %s: Bad info", name.c_str());
            Logger::logMessage(Logger::ERROR, buffer_); */
        }

        receiverDirIPC_ = dirIPC;
        receiverIdIPC_ = idIPC;

        try {
            receiverMsgQueue_ = IPC::MsgQueue(name);
            receiverMsgQueue_.getMsgQueue(dirIPC, idIPC);
        }
        catch(Exception & e) {
            Logger::logMessage(Logger::ERROR, e.get_error_description());
            abort();
        }
    }

    template<class T>
    void send( long receiverId, T& data ) {
        CommPacketWrapper wrapper;
        wrapper.setDirIPC( receiverDirIPC_ );
        wrapper.setIdDirIPC( receiverIdIPC_ );
        wrapper.setSenderId( senderId_ );
        wrapper.setReceiverType( receiverTypeId_ );
        wrapper.setReceiverId( receiverId );
        MsgCanalSalidaAgente msg;
        wrapper.createPacketForQueues(msg, data);

        try {
            senderMsgQueue_.send(msg);
        }
        catch(Exception & e) {
            Logger::logMessage(Logger::ERROR, e.get_error_description());
            abort();
        }
    }

    template<class T>
    void recv( long type, T& data ) {
        MsgAgenteReceptor msg;

        try {
            receiverMsgQueue_.recv(type, msg);
        }
        catch(Exception & e) {
            Logger::logMessage(Logger::ERROR, e.get_error_description());
            abort();
        }
        memcpy(&data, msg.msg, sizeof(T));
    }

private:
    IPC::MsgQueue senderMsgQueue_;
    IPC::MsgQueue receiverMsgQueue_;
    const char* receiverDirIPC_;
    int receiverIdIPC_;
    char buffer_[255];
    std::string name_;
    long senderId_;
    TipoAgente senderTypeId_;
    TipoAgente receiverTypeId_;

};

}

#endif /* COMM_MSG_HANDLER */

