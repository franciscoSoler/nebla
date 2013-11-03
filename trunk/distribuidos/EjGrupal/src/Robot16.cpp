#include <iostream>
#include <memory>
#include <API/Robot16/ControllerRobot16.h>
#include <API/Objects/Util.h>

int main(int argc, char* argv[]) {
    std::auto_ptr<IControllerRobot16> controller( new ControllerRobot16() );
    
    
    while (1) {
        // El pedido fue realizado por el Robot14
        PedidoDespacho pedido = controller->recibirPedido();
        
        if (pedido.idProducto_ == NULL_PRODUCT) {
            // Mensaje del Robot14
            while (controller->moverCinta()) {
                sleep ( Util::getInstance().generateRandomNumber(1,2) );
            }

            Caja unaCaja;
            controller->tomarCajaCinta15(unaCaja);
            sleep( Util::getInstance().generateRandomNumber(1,2) );

            if ( controller->depositarCajaEnAPT(unaCaja, unaCaja.ordenDeCompra_) ) {
                // Se debe notificar al Despacho de que se termino el lote
                controller->informarAlDespachoProductoTerminado(unaCaja.ordenDeCompra_, 
                        unaCaja.idProducto_);
            }
            sleep( Util::getInstance().generateRandomNumber(1,2) );
        }
        else {    
            // Llegó una caja para el cliente
            Caja unaCaja;
            controller->tomarCajaDeAPT(pedido, &unaCaja);
            controller->enviarCajaAlCliente(pedido.idCliente_, unaCaja);
        }
    }
    
    return 0;
}

