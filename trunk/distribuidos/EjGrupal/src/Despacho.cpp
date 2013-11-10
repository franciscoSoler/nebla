#include <iostream>
#include <memory>
#include <sstream>
#include <API/Despacho/ControllerDespacho.h>
#include <API/Objects/Util.h>
#include <API/Objects/Dictionary.h>
#include <Common.h>
#include <Logger/Logger.h>


typedef std::auto_ptr<IControllerDespacho> autoPtrControllerDespacho;

// Llego un cliente, busco su orden de compra y le doy los productos del tipo que
// se termino
void pedidoCliente(autoPtrControllerDespacho & controller,
        PedidoDespacho pedido, Dictionary<OrdenDeCompra> & bufferODC);

void pedidoOrdenDeCompra(autoPtrControllerDespacho & controller,
                         PedidoDespacho pedido, Dictionary<OrdenDeCompra> & buffer);

// Lote de algún producto completo
void pedidoRobot16(autoPtrControllerDespacho & controller, PedidoDespacho pedido,
                   OrdenDeCompra* ordenDeCompra);



int main(int argc, char* argv[]) {
    autoPtrControllerDespacho controller ( new ControllerDespacho() );
    Dictionary<OrdenDeCompra> bufferODC;
    OrdenDeCompra* odc;

    while (1) {
        PedidoDespacho pedido = controller->recibirPedido();
        
        if (pedido.tipoPedido_ == PEDIDO_CLIENTE) {
            pedidoCliente(controller, pedido, bufferODC);

        }
        else if (pedido.tipoPedido_ == PEDIDO_ODC) {
            pedidoOrdenDeCompra(controller, pedido, bufferODC);
        }
        else if (pedido.tipoPedido_ == PEDIDO_ROBOT16) {
            odc = bufferODC.get(pedido.idOrdenDeCompra_);
            if ( odc != NULL ) {
                pedidoRobot16(controller, pedido, odc);    
            }
            else {
                // ERROR: No debería entrar acá
                Logger::logMessage(Logger::ERROR, "Flujo inválido - PEDIDO_ROBOT16");
                abort();
            }
        }
        else {
            // ERROR: No debería entrar acá
            Logger::logMessage(Logger::ERROR, "Flujo inválido - PEDIDO_INVALIDO");
            abort();
        }
    }
    return 0;
}

void pedidoCliente(autoPtrControllerDespacho & controller,
        PedidoDespacho pedido, Dictionary<OrdenDeCompra> & bufferODC) {

    OrdenDeCompra* odc = bufferODC.get(pedido.idOrdenDeCompra_);
    TipoProducto idProducto = pedido.idProducto_;

    if ( odc != NULL ) {
        // Llego el cliente, ordeno al Robot16 que vaya sacando las cajas.
        int cantidadDeProductos = odc->cantidadPorProducto_[idProducto-1];

        // Actualizo la orden de compra. Quito de la misma, el producto
        // a despachar
        odc->cantidadPorProducto_[idProducto-1] = 0;
        odc->productoTerminado_[idProducto-1] = true;

        for (int i = 0; i < cantidadDeProductos; ++i) {
            controller->despacharProducto(idProducto, odc->idOrden_);
            sleep( Util::generateRandomNumber(1,5) );
        }

        // Si la orden de compra está satisfecha, se debe eliminar
        bool ordenTerminada = false;
        for (int i = 0; i < CANTIDAD_PRODUCTOS; ++i) {
            if (odc->productoTerminado_[i] ) {
                ordenTerminada = true;
                break;
            }
        }

        if ( ordenTerminada ) {
            // Se terminó la orden, la elimino del diccionario
            bufferODC.erase(pedido.idOrdenDeCompra_);
        }
    }
    else {
        // ERROR: No debería entrar acá
        Logger::logMessage(Logger::ERROR, "Flujo inválido - PEDIDO_CLIENTE");
        abort();
    }
}

void pedidoOrdenDeCompra(autoPtrControllerDespacho & controller,
                         PedidoDespacho pedido, Dictionary<OrdenDeCompra> & bufferODC) {

    OrdenDeCompra odc = controller->obtenerOrdenDeCompra(pedido.idOrdenDeCompra_);
    bufferODC.insert(pedido.idOrdenDeCompra_, new OrdenDeCompra(odc) );
}

void pedidoRobot16(autoPtrControllerDespacho & controller, PedidoDespacho pedido,
                   OrdenDeCompra* ordenDeCompra) {
    try {
        // Se envía al cliente el idProducto y el idNroCompra asociados al producto que 
        // se terminó de fabricar
        controller->notificarAClienteProductoTerminado(ordenDeCompra->idCliente_,
                pedido.idOrdenDeCompra_, pedido.idProducto_);
    }
    catch (Exception & e) {
        std::cout << "Error de Aplicación: " << 
        e.get_error_description() << std::endl;
    }
}

