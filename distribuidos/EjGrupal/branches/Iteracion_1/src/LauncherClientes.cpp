#include <iostream>

#include <sys/stat.h>
#include <sys/types.h>

#include "Common.h"
#include "Exceptions/Exception.h"
#include "Logger/Logger.h"
#include "IPCs/IPCTemplate/MsgQueue.h"
#include "VendedorLibreMessageQueue.h"
#include "ClientesMessageQueue.h"
#include "PedidosVendedorMessageQueue.h"


static char buffer[255];
static char param1[20];
// static char param2[20];

void createIPCs();
void createDirectory(std::string directoryName);
void createProcess(std::string processName, int amountOfProcesses = 1, int parameterOffset = 0);

int main(int argc, char* argv[]) {
    try {
        createDirectory(DIRECTORY_ROBOT_16);
        createDirectory(DIRECTORY_VENDEDOR);
        createDirectory(DIRECTORY_CLIENTE);

        createIPCs();

        createProcess("Cliente", 1, 1);

        // Procesos correspondientes al Middleware
        createProcess("CreadorCanalesCliente");
        /* A este Proceso, se le debería enviar el mismo ID que tiene
         * el cliente. Como es un autonumérico, por ahora esto funciona
         */
        createProcess("ClienteCanalConDespacho", 1, 1);
        createProcess("ClienteCanalConR16", 1, 1);
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR,
        e.get_error_description().c_str());
        return -1;
    }

    return 0;
}

void createDirectory(std::string directoryName) {
    mkdir(directoryName.c_str(), 0777);
}

void createIPCs() {
    // Logger::getInstance().createLog();
    Logger::getInstance().setProcessInformation("LauncherCliente:");

    /* NOTA: La notación para colas es que el input o output es respecto
     * del nombre del proceso asociado a la cola. inputQueueClient por
     * ejemplo indica que es la cola donde el cliente recibe mensajes
     */
    IPC::MsgQueue inputQueueCliente("inputQueueCliente");
    inputQueueCliente.create(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID_C);
    Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueCliente creado");

    IPC::MsgQueue R16_Cliente_Queue("R16_Cliente_Queue");
    R16_Cliente_Queue.create(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID_C);
    Logger::logMessage(Logger::IMPORTANT, "IPC R16_Cliente_Queue creado");

    IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
    inputQueueDespacho.create(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID_C);
    Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueDespacho creado");

    IPC::VendedorLibreMessageQueue vendedores ("Vendedores Msg Queue");
    vendedores.createMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES_C);
    Logger::logMessage(Logger::IMPORTANT, "IPC VendedorLibreMessageQueue creado");

    IPC::ClientesMessageQueue clientes ("Clientes Msg Queue");
    clientes.createMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES_C);
    Logger::logMessage(Logger::IMPORTANT, "IPC ClientesMessageQueue creado");

    IPC::PedidosVendedorMessageQueue pedidos ("Pedidos Msg Queue");
    pedidos.createMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS_C);
    Logger::logMessage(Logger::IMPORTANT, "IPC PedidosVendedorMessageQueue creado");
}

void createProcess(std::string processName, int amountOfProcesses, int parameterOffset) {
    pid_t pid;

    for (int i = parameterOffset; i < amountOfProcesses + parameterOffset; i++) {
        if ((pid = fork()) < 0) {
            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);
        }
        else if (pid == 0) {
            // Child process. Pass the arguments to the process and call exec
            sprintf(param1, "%d", i);
            sprintf(buffer, "./%s", processName.c_str());
            execlp(buffer, processName.c_str(), param1, (char *) 0);

            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);

            return;
        }
    }
}
