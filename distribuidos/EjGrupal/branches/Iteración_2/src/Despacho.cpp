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
    char buffer[255]; 

    while (1) {
        PedidoDespacho pedido = controller->recibirPedido();
        if (pedido.tipoPedido_ == PEDIDO_CLIENTE) {
            pedidoCliente(controller, pedido, bufferODC);

        }
        else if (pedido.tipoPedido_ == PEDIDO_ODC) {
            pedidoOrdenDeCompra(controller, pedido, bufferODC);
        }
        else if (pedido.tipoPedido_ == PEDIDO_ROBOT16) {
            sprintf(buffer, "Despacho recibe mensaje de Robot16_Cinta15 con ODC N°%ld",
                    pedido.idOrdenDeCompra_);
            Logger::logMessage(Logger::IMPORTANT, buffer);

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

        // Actualizo la orden de compra. Quito de la misma el producto
        // a despachar
        odc->cantidadPorProducto_[idProducto-1] = 0;

        // Si la orden de compra está satisfecha, se debe eliminar
        bool ordenTerminada = true;
        for (int i = 0; i < CANTIDAD_PRODUCTOS; ++i) {
            if ( odc->cantidadPorProducto_[i] ) {
                ordenTerminada = false;
                break;
            }
        }

        PedidoDespacho pedido;
        pedido.idProducto_ = idProducto;
        pedido.idOrdenDeCompra_ = odc->idOrden_;
        pedido.idCliente_ = odc->idCliente_;

        for (int i = 0; i < cantidadDeProductos; ++i) {
            bool ultimoProductoDeODC =
            ordenTerminada == true && (i+1) == cantidadDeProductos;

            controller->despacharProducto(pedido, ultimoProductoDeODC);
            sleep( Util::generateRandomNumber(1,5) );
        }

        if ( ordenTerminada ) {
            // Se terminó la orden, la elimino del diccionario
            char buffer[255];
            sprintf(buffer, "ODC N°%ld terminada, la misma es eliminada",
                    pedido.idOrdenDeCompra_);
            Logger::logMessage(Logger::DEBUG, buffer);
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

    OrdenDeCompra odc = pedido.odc_;

    // Al recibir la ODC, se verifica si algun pedido se encuentra en stock, y se
    // envia mensaje al cliente para que venga a retirar el mismo
    for (int i = 0; i < CANTIDAD_PRODUCTOS; ++i) {
        if ( odc.productoTerminado_[i] == true ) {
            odc.productoTerminado_[i] = false;
            odc.faltantesAEntregar_--;

            PedidoDespacho pedido;
            pedido.idCliente_ = odc.idCliente_;
            pedido.idOrdenDeCompra_ = odc.idOrden_;
            pedido.idProducto_ = static_cast<TipoProducto> (i+1);

            char buffer_[255];
            sprintf(buffer_, "Stock - Notifica a Cliente N°%lu que puede retirar su pedido: "
            "idProducto: %d - idOrdenCompra: %lu.", pedido.idCliente_,
            pedido.idProducto_, pedido.idOrdenDeCompra_);
            Logger::logMessage(Logger::IMPORTANT, buffer_);

            controller->notificarAClienteProductoTerminado(pedido, odc.faltantesAEntregar_ == 0);
        }
    }

    if (! bufferODC.insert(pedido.idOrdenDeCompra_, new OrdenDeCompra(odc) ) ) {
            Logger::logMessage(Logger::ERROR, "Existe más de una ODC con el mismo identificador");
    }
}

void pedidoRobot16(autoPtrControllerDespacho & controller, PedidoDespacho pedido,
                   OrdenDeCompra* ordenDeCompra) {
    try {
        PedidoDespacho pedidoAEnviar;
        pedidoAEnviar.idCliente_ = ordenDeCompra->idCliente_;
        pedidoAEnviar.idOrdenDeCompra_ = ordenDeCompra->idOrden_;
        pedidoAEnviar.idProducto_ = pedido.idProducto_;

        // Se envía al cliente el idProducto y el idNroCompra asociados al producto que 
        // se terminó de fabricar
        ordenDeCompra->faltantesAEntregar_--;

        char buffer_[255];
        sprintf(buffer_, "Notifica a Cliente N°%lu que puede retirar su pedido: "
        "idProducto: %d - idOrdenCompra: %lu.", pedidoAEnviar.idCliente_,
        pedidoAEnviar.idProducto_, pedidoAEnviar.idOrdenDeCompra_);
        Logger::logMessage(Logger::IMPORTANT, buffer_);


        controller->notificarAClienteProductoTerminado(
                pedidoAEnviar, ordenDeCompra->faltantesAEntregar_ == 0);
    }
    catch (Exception & e) {
        std::cout << "Error de Aplicación: " << 
        e.get_error_description() << std::endl;
    }
}
