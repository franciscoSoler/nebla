#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include <Common.h>
#include <middlewareCommon.h>
#include <Exceptions/Exception.h>
#include "Logger/Logger.h"

#include <API/Objects/Util.h>
#include "IPCs/IPCTemplate/MsgQueue.h"
#include <Comunicaciones/Objects/ServersManager.h>

void getMsgQueueIPCs();
void getMsgQueueDirectory(std::string directoryName);

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setProcessInformation("TerminatorBrokers:");

        // Se crea una cola por cada Agente
        IPC::MsgQueue colaAgente;
        
        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_CLIENTE);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola cliente destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_VENDEDOR);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola vendedor destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_AP);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola AlmacenDePiezas destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_AGV);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola AGV destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_ROBOT5_AGV);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot5AGV destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_ROBOT5_CINTA);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot5Cinta destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_ROBOT11);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot11 destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_ROBOT12);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot12 destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_ROBOT14);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot14 destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_ROBOT16_CINTA);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot16Cinta destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_ROBOT16_DESPACHO);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot16Despacho destruida");

        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_DESPACHO);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Despacho destruida"); 
        
        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_MEMORIA);
        colaAgente.destroy();  
        Logger::logMessage(Logger::COMM, "Cola Memorias destruida");
   
        colaAgente.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_PEDIDO_MEMORIA);
        colaAgente.destroy();     
        Logger::logMessage(Logger::COMM, "Cola Pedidos Memorias destruida"); 
    
        // TODO: Destruir todas las memorias compartidas de contadoras

    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }
    
    ServersManager serversManager;
    serversManager.killServers();
    
    return 0;
}
    
