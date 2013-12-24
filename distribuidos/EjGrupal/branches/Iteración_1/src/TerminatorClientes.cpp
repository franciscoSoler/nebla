#include <iostream>

#include "Exceptions/Exception.h"
#include "Logger/Logger.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/VendedorLibreMessageQueue.h"
#include "MsgQueue.h"

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setProcessInformation("Terminator Clientes:");

        IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
        inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID_C);
        inputQueueDespacho.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueDespacho creado");

        IPC::MsgQueue inputQueueCliente("inputQueueCliente");
        inputQueueCliente.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID_C);
        inputQueueCliente.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueCliente destruido");

        IPC::MsgQueue R16_Cliente_Queue("R16_Cliente_Queue");
        R16_Cliente_Queue.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID_C);
        R16_Cliente_Queue.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC R16_Cliente_Queue destruido");

        IPC::VendedorLibreMessageQueue vendedores("Vendedores Msg Queue");
        vendedores.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES_C);
        vendedores.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC VendedorLibreMessageQueue destruido");

        IPC::ClientesMessageQueue clientes("Clientes Msg Queue");
        clientes.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES_C);
        clientes.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC ClientesMessageQueue destruido");

        IPC::PedidosVendedorMessageQueue pedidos("Pedidos Msg Queue");
        pedidos.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS_C);
        pedidos.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC PedidosVendedorMessageQueue destruido");
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, e.get_error_description().c_str());
        abort();
    }
}

