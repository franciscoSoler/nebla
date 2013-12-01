#include "MiddlewareAPI.h"
#include <Util.h>
#include <sstream>

void MiddlewareAPI::crearCanales(int idAgente, TipoAgente idTipoAgente) {
    Util::getInstance();

    std::stringstream argumentos;
    argumentos << idAgente << " " << idTipoAgente;

    Util::createProcess("CanalEntradaAgente", argumentos.str());
    Util::createProcess("CanalSalidaAgente", argumentos.str());
}

void MiddlewareAPI::crearHandlerDeColaSalida(const char* dirIPC, long idAgente,
                                             TipoAgente idTipoAgente) {

    Util::getInstance();

    std::stringstream argumentos;
    argumentos << dirIPC << " " << idAgente << " " << idTipoAgente;

    Util::createProcess("MuxTipoAgente", argumentos.str());
}

