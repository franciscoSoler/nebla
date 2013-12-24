#ifndef I_CONTROLLER_DESPACHO_
#define I_CONTROLLER_DESPACHO_

#include <Common.h>

class IControllerDespacho {
    
public:
    virtual PedidoDespacho recibirPedido() = 0;
    
    /* Indicarle al Robot16 que tiene que sacar un producto del AGV
     * y colocarlo en el Buffer del Despacho
     */
    virtual void despacharProducto(PedidoDespacho pedido, bool ultimoProductoDeODC) = 0;
    
    /* Se llama al cliente para que este venga a buscar su Pedido
     */
    virtual void notificarAClienteProductoTerminado(PedidoDespacho pedido, bool ultimoPedido) = 0;
    
    virtual ~IControllerDespacho() {};
};

#endif /* I_CONTROLLER_DESPACHO_ */

