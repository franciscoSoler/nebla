#include <iostream>
#include <memory>
#include <API/Robot16_Cinta15/ControllerR16_Cinta15.h>
#include <API/Objects/Util.h>

int main(int argc, char* argv[]) {
    std::auto_ptr<IControllerR16_Cinta15> controller( new ControllerR16_Cinta15() );
    
    
    while (1) {
        // El pedido fue realizado por el Robot14
        controller->recibirOrdenParaMoverCaja();

        while (controller->moverCinta()) {
            sleep ( Util::getInstance().generateRandomNumber(1,2) );
        }

        Caja unaCaja;
        controller->tomarCajaCinta15(unaCaja);
        sleep( Util::getInstance().generateRandomNumber(1,2) );

        long idOrdenDeCompra = 0;
        if ( controller->depositarCajaEnAPT(unaCaja, idOrdenDeCompra) ) {
            // Se debe notificar al Despacho de que se termino el lote
            controller->informarAlDespachoProductoTerminado(idOrdenDeCompra,
                    unaCaja.idProducto_);
        }
        sleep( Util::getInstance().generateRandomNumber(1,2) );
    }

    return 0;
}

