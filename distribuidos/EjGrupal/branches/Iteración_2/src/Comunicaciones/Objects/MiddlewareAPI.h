#ifndef MIDDLEWARE_API
#define MIDDLEWARE_API

#include <middlewareCommon.h>

class MiddlewareAPI {
public:
    void crearCanales(int idAgente, TipoAgente idTipoAgente);
    void crearHandlerDeColaSalida(const char* dirIPC, long idAgente,
                                  TipoAgente idTipoAgente);
};

#endif  /* MIDDLEWARE_API */
