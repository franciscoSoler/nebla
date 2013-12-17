#include "MiddlewareAPI.h"
#include <sstream>
#include <string>
#include <memory>

#include <ConfigFileParser/ConfigFileParser.h>
#include <Common.h>
#include <Logger/Logger.h>
#include <API/Objects/Util.h>


int MiddlewareAPI::crearCanales(const char* paramAgenteEnConfigFile,
                  int idAgente, TipoAgente idTipoAgente) {

    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(SERVERS_CONFIG_FILE) );
    cfg->parse();

    int cantidadBrokers = cfg->getConfigFileParam("CantidadBrokers", -1);
    int idBroker = cfg->getConfigFileParam(paramAgenteEnConfigFile, -1);

    if ( cantidadBrokers == -1 || idBroker == -1 ) {
        Logger::logMessage(Logger::ERROR, "crearCanales(): "
        "Error al parsear parametros del archivo de configuracion");
        return -1;

    }

    if ( idBroker == 0 ) {
        // Elijo el id al cual se va a conectar el usuario de forma aleatoria.
        idBroker = Util::generateRandomNumber(1, cantidadBrokers);
    }

    char buffer[255];
    sprintf(buffer, "Agente procede a conectarse al Broker Nº%d", idBroker);
    Logger::logMessage(Logger::IMPORTANT, buffer);

    Util::getInstance();
    std::stringstream arg1;
    std::stringstream arg2;
    std::stringstream arg3;
    arg1 << idAgente;
    arg2 << idTipoAgente;
    arg3 << idBroker;

    Util::createProcess("CanalEntradaAgente", arg1.str(), arg2.str(), arg3.str());
    Util::createProcess("CanalSalidaAgente", arg1.str(), arg2.str(), arg3.str());

    return 0;
}

