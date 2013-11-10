#include <iostream>
#include <memory>
#include <API/Robot16_Despacho/ControllerR16_Despacho.h>
#include <API/Objects/Util.h>

int main(int argc, char* argv[]) {
    std::auto_ptr<IControllerR16_Despacho> controller( new ControllerR16_Despacho() );
    
    
    while (1) {
        // El pedido fue realizado por el Robot14
        PedidoDespacho pedido = controller->recibirPedido();
        
        // Llegó una caja para el cliente
        Caja unaCaja;
        controller->tomarCajaDeAPT(pedido, &unaCaja);
        controller->enviarCajaAlCliente(pedido.idCliente_, unaCaja);
        }
    
    return 0;
}

