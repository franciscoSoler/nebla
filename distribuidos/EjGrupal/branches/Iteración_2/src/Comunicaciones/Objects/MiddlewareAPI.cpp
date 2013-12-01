#include "MiddlewareAPI.h"
#include "Logger.h"
#include <Util.h>
#include <sstream>
#include <list>

void MiddlewareAPI::crearCanales(int idAgente, TipoAgente idTipoAgente) {
    Util::getInstance();

    std::list<std::string> args;

    std::stringstream argumentos;
    argumentos << idAgente;
    args.push_back(argumentos.str());
    argumentos.str("");

    argumentos << idTipoAgente;
    args.push_back(argumentos.str());
    
    Util::createProcess("CanalEntradaAgente", args);
    Util::createProcess("CanalSalidaAgente", args);
}

