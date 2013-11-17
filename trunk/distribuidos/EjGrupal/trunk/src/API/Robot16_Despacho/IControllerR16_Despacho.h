#ifndef I_CONTROLLER_R16_DESPACHO_H_
#define I_CONTROLLER_R16_DESPACHO_H_

#include <Common.h>

class IControllerR16_Despacho {
    
public:
    // FIXME: Cambiar nombre, poco representativo
    virtual PedidoDespacho recibirPedido() = 0;
    virtual void tomarCajaDeAPT(PedidoDespacho pedido, Caja* unaCaja) = 0;
    virtual void enviarCajaAlCliente(long idCliente, Caja unaCaja) = 0;
    virtual ~IControllerR16_Despacho() {};
};

#endif /* I_CONTROLLER_R16_DESPACHO_H_ */

