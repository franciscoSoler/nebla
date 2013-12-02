#include "MiddlewareAPI.h"
#include "Logger.h"
#include <Util.h>
#include <sstream>
#include <string>

void MiddlewareAPI::crearCanales(int idAgente, TipoAgente idTipoAgente) {
    Util::getInstance();

    std::list<std::string> args;

    std::stringstream arg1;
    std::stringstream arg2;
    arg1 << idAgente;
    arg2 << idTipoAgente;

    Util::createProcess("CanalEntradaAgente", arg1.str(), arg2.str());
    Util::createProcess("CanalSalidaAgente", arg1.str(), arg2.str());
}

