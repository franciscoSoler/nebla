#ifndef I_CONTROLLER_DESPACHO_
#define I_CONTROLLER_DESPACHO_

#include <Common.h>

class IControllerDespacho {
    
public:
    virtual PedidoDespacho recibirPedido() = 0;
    
    /* Recibe orden de compra enviada por el vendedor. Debe recibir un 
     * pedido que corrobore que ya hay una ODC antes de llamar a este método
     */
    virtual OrdenDeCompra obtenerOrdenDeCompra(long idOrden) = 0;
    
    /* Indicarle al Robot16 que tiene que sacar un producto del AGV
     * y colocarlo en el Buffer del Despacho
     */
    virtual void despacharProducto(TipoProducto producto, uint idOrdenCompra, long idCliente) = 0;
    
    /* Se llama al cliente para que este venga a buscar su Pedido
     */
    virtual void notificarAClienteProductoTerminado(long idCliente, 
            long idOrdenCompra, TipoProducto idProducto) = 0;
    
    virtual ~IControllerDespacho() {};
};

#endif /* I_CONTROLLER_DESPACHO_ */

