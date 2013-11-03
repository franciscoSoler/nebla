/* 
 * File:   DestructorIPC.cpp
 * Author: fede
 *
 * Created on October 27, 2013, 5:04 PM
 */

#include <cstdlib>
#include "Cola.h"
#include "Semaforo.h"
#include "MemoriaCompartida.h"
#include "const.h"

using namespace std;

void destruirColas()
{
    Cola<mensaje_inicial_t> vendedores(NOMBRE_ARCHIVO, LETRA_COLA_VENDEDORES);
    vendedores.destruir();
    Cola<mensaje_inicial_t> clientes(NOMBRE_ARCHIVO, LETRA_COLA_CLIENTES);
    clientes.destruir();
    for(int i = 0; i < CANT_VENDEDORES; i++)
    {
	Cola<pedido_t> pedidos(NOMBRE_ARCHIVO, LETRA_COLA_CLIENTES + (char) 1 + (char) i);
	pedidos.destruir();
    }
    Cola<consulta_almacen_piezas_t> consultasAlmacen(NOMBRE_ARCHIVO, LETRA_COLA_CONSULTAS_ALMACEN_PIEZAS);
    consultasAlmacen.destruir();
    Cola<respuesta_almacen_piezas_t> respuestasAlmacen(NOMBRE_ARCHIVO, LETRA_COLA_RESPUESTAS_ALMACEN_PIEZAS);
    respuestasAlmacen.destruir();
    Cola<MensajePedidoProduccion> mensajesRobot5(NOMBRE_ARCHIVO_ROBOT5, LETRA_ROBOT5);
    mensajesRobot5.destruir();
}

void destruirShmems()
{
    MemoriaCompartida shmemAlmacenTerminados(NOMBRE_ARCHIVO, LETRA_SHMEM_ALMACEN_TERMINADOS, TAM_ALMACEN * sizeof(EspacioAlmacenProductos));
    shmemAlmacenTerminados.liberar();
    MemoriaCompartida shmemNumeroOrdenCompra(NOMBRE_ARCHIVO, LETRA_SHMEM_NRO_OC, sizeof(int));
    shmemNumeroOrdenCompra.liberar();
    MemoriaCompartida shmemNumeroOrdenProduccion(NOMBRE_ARCHIVO, LETRA_SHMEM_NRO_OP, sizeof(int));
    shmemNumeroOrdenProduccion.liberar();
}

void destruirSemaforos()
{
    Semaforo mutexAlmacenTerminados(NOMBRE_ARCHIVO, LETRA_SEM_ALMACEN_TERMINADOS, 1);
    mutexAlmacenTerminados.destruir();
}

int main(int argc, char** argv)
{
    destruirSemaforos();
    destruirColas();
    destruirShmems();
 
    return 0;
}
