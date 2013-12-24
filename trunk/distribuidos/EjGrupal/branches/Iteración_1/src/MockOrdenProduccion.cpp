/* 
 * File:   MockOrdenProduccion.cpp
 * Author: knoppix
 *
 * Created on November 3, 2013, 2:10 AM
 */

#include <cstdlib>

#include "IPCs/IPCAbstractos/MessageQueue/PedidosProduccionMessageQueue.h"
#include "Common.h"

int generateRandomNumber(int minValue, int maxValue) {
    srand( time(NULL) + getpid() );
    return minValue + rand() % ( (abs(maxValue - minValue)) );
}

int main(int argc, char** argv) {
    char buffer[TAM_BUFFER];
    int nroOrdenDeCompra = generateRandomNumber(1,150);
    IPC::PedidosProduccionMessageQueue colaPedidosProduccion = IPC::PedidosProduccionMessageQueue("Mock PedidosProduccionMessageQueue");
    colaPedidosProduccion.getMessageQueue(DIRECTORY_ROBOT_5,ID_COLA_PEDIDOS_PRODUCCION);

    while (true) {

        
        PedidoProduccion pedidoProduccion;
        pedidoProduccion.cantidad = generateRandomNumber(15,101);
        pedidoProduccion.diferenciaMinima = generateRandomNumber(0, pedidoProduccion.cantidad);
        pedidoProduccion.nroOrdenCompra = nroOrdenDeCompra++;
        pedidoProduccion.tipo = (TipoProducto)1;

        MensajePedidoProduccion mensajePedidoProduccion;
        mensajePedidoProduccion.mtype = TIPO_PEDIDO_PRODUCCION;
        mensajePedidoProduccion.pedidoProduccion = pedidoProduccion;
        
        sprintf(buffer, "Enviando pedido: Orden de compra: %d, Cantidad: %d, Tipo: %d, Diferencia: %d\n",
                pedidoProduccion.nroOrdenCompra,
                pedidoProduccion.cantidad,
                pedidoProduccion.tipo,
                pedidoProduccion.diferenciaMinima);
        
        write(fileno(stdout),buffer,strlen(buffer));
        
        colaPedidosProduccion.enviarPedidoProduccion(mensajePedidoProduccion);
        getchar();
    }
    return 0;
}

