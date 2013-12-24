#ifndef MIDDLEWARE_API
#define MIDDLEWARE_API

/* Candidato para moverse a ServersManager
 */

#include <middlewareCommon.h>

class MiddlewareAPI {
public:
    int crearCanales(const char* paramAgenteEnConfigFile,
                     int idAgente, TipoAgente idTipoAgente);
};

#endif  /* MIDDLEWARE_API */
