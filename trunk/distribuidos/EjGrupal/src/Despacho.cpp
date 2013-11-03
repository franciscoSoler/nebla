#include <iostream>
#include <memory>
#include <sstream>
#include <API/Despacho/ControllerDespacho.h>
#include <API/Objects/Util.h>
#include <API/Objects/BufferODC.h>
#include <Common.h>

// OrdenDeCompra pedidoOrdenDeCompra(std::auto_ptr<IControllerDespacho> controller, PedidoDespacho pedido);

// Llego un cliente, busco su orden de compra y le doy los productos del tipo que
// se terminó
void pedidoCliente(std::auto_ptr<IControllerDespacho> controller, TipoProducto idProducto, OrdenDeCompra ordenDeCompra);
// Lote de algún producto completo
void pedidoRobot16(std::auto_ptr<IControllerDespacho> controller, PedidoDespacho pedido, OrdenDeCompra ordenDeCompra);

int main(int argc, char* argv[]) {
    std::auto_ptr<IControllerDespacho> controller ( new ControllerDespacho() );
    BufferODC bufferODC; 
    OrdenDeCompra odc;

    while (1) {
        PedidoDespacho pedido = controller->recibirPedido();
        
        // FIXME: Refactorizar por una Chain
        if (pedido.tipoPedido_ == PEDIDO_CLIENTE) {
            if ( bufferODC.getOrdenDeCompra(pedido.idOrdenDeCompra_, odc) ) {
                // El cliente envía en su orden su pedido el id de ordenDeCompra 
                // y el tipo de producto
                pedidoCliente(controller, pedido.idProducto_, odc);    
            }
            else {
                // ERROR: No debería entrar acá
            }
        }
        else if (pedido.tipoPedido_ == PEDIDO_ODC) {
            OrdenDeCompra odc = controller->obtenerOrdenDeCompra(pedido.idOrdenDeCompra_);
            bufferODC.agregarOrdenDeCompra(pedido.idOrdenDeCompra_, odc);            
        }
        else if (pedido.tipoPedido_ == PEDIDO_ROBOT16) {
            if ( bufferODC.getOrdenDeCompra(pedido.idOrdenDeCompra_, odc) ) {
                pedidoRobot16(controller, pedido, odc);    
            }
            else {
                // ERROR: No debería entrar acá
            }
        }
        else {
            // ERROR: No debería entrar acá
        }
    }
    return 0;
}

void pedidoCliente(std::auto_ptr<IControllerDespacho> controller, 
        TipoProducto idProducto, OrdenDeCompra ordenDeCompra) {
    
    // Llego el cliente, ordeno al Robot16 que vaya sacando las cajas.
    int cantidadDeProductos = ordenDeCompra.cantidadPorProducto_[idProducto-1];
    for (int i = 0; i < cantidadDeProductos; ++i) {
        controller->despacharProducto(idProducto, ordenDeCompra.idOrden_);
        sleep( Util::generateRandomNumber(1,5) );
    }


}

void pedidoRobot16(std::auto_ptr<IControllerDespacho> controller, PedidoDespacho pedido, 
                   OrdenDeCompra ordenDeCompra) {
    try {
        // Se envía al cliente el idProducto y el idNroCompra asociados al producto que 
        // se terminó de fabricar
        controller->notificarAClienteProductoTerminado(ordenDeCompra.idCliente_, 
                pedido.idOrdenDeCompra_, pedido.idProducto_);
    }
    catch (Exception & e) {
        std::cout << "Error de Aplicación: " << 
        e.get_error_description() << std::endl;
    }
}

