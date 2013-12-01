#include <iostream>

#include "Exceptions/Exception.h"
#include "Logger/Logger.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/VendedorLibreMessageQueue.h"
#include "MsgQueue.h"

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setProcessInformation("Terminator Despacho:");

        IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
        inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);
        inputQueueDespacho.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueDespacho creado");

        IPC::MsgQueue inputQueueCliente("inputQueueCliente");
        inputQueueCliente.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID);
        inputQueueCliente.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueCliente destruido");

        IPC::MsgQueue inputQueueR16_Despacho("inputQueueR16_Despacho");
        inputQueueR16_Despacho.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_DESPACHO_INPUT_ID);
        inputQueueR16_Despacho.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueR16_Despacho Destruido");


    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, e.get_error_description().c_str());
        abort();
    }
}
