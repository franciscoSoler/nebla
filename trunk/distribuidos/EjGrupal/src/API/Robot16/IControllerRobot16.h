#ifndef I_CONTROLLER_ROBOT_16_
#define I_CONTROLLER_ROBOT_16_

#include <common.h>

class IControllerRobot16 {
    
public:
    // FIXME; Cambiar nombre, poco representativo
    virtual PedidoDespacho recibirPedido() = 0;
    virtual bool moverCinta() = 0;
    virtual bool tomarCajaCinta15(Caja & unaCaja) = 0;
    /* Retorna true si terminó de procesarse el producto asociado a la orden
     * de compra del producto. 
     */
    virtual bool depositarCajaEnAPT(Caja unaCaja, long & idNroOrdenAPT) = 0;
    virtual void informarAlDespachoProductoTerminado(long idNroOrden, 
                                                     TipoProducto tipo) = 0;
    virtual void tomarCajaDeAPT(PedidoDespacho pedido, Caja* unaCaja) = 0;
    virtual void enviarCajaAlCliente(long idCliente, Caja unaCaja) = 0;
    virtual ~IControllerRobot16() {};
};

#endif /* I_CONTROLLER_ROBOT_16_ */

