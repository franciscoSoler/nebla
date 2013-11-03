/* 
 * File:   CreadorIPC.cpp
 * Author: fede
 *
 * Created on October 27, 2013, 5:06 PM
 */

#include <cstdlib>
#include "Cola.h"
#include "Semaforo.h"
#include "MemoriaCompartida.h"
#include "const.h"

using namespace std;

void crearColas()
{
    Cola<mensaje_inicial_t> vendedores(NOMBRE_ARCHIVO, LETRA_COLA_VENDEDORES);
    vendedores.crear();
    Cola<mensaje_inicial_t> clientes(NOMBRE_ARCHIVO, LETRA_COLA_CLIENTES);
    clientes.crear();
    for(int i = 0; i < CANT_VENDEDORES; i++)
    {
	Cola<pedido_t> pedidos(NOMBRE_ARCHIVO, LETRA_COLA_CLIENTES + (char) 1 + (char) i);
	pedidos.crear();
    }
    
    Cola<consulta_almacen_piezas_t> consultasAlmacen(NOMBRE_ARCHIVO, LETRA_COLA_CONSULTAS_ALMACEN_PIEZAS);
    consultasAlmacen.crear();
    Cola<respuesta_almacen_piezas_t> respuestasAlmacen(NOMBRE_ARCHIVO, LETRA_COLA_RESPUESTAS_ALMACEN_PIEZAS);
    respuestasAlmacen.crear();
}

void crearShmems()
{
    MemoriaCompartida shmemAlmacenTerminados(NOMBRE_ARCHIVO, LETRA_SHMEM_ALMACEN_TERMINADOS, TAM_ALMACEN * sizeof(EspacioAlmacenProductos));
    shmemAlmacenTerminados.crear();
    MemoriaCompartida shmemNumeroOrdenCompra(NOMBRE_ARCHIVO, LETRA_SHMEM_NRO_OC, sizeof(int));
    shmemNumeroOrdenCompra.crear();
    MemoriaCompartida shmemNumeroOrdenProduccion(NOMBRE_ARCHIVO, LETRA_SHMEM_NRO_OP, sizeof(int));
    shmemNumeroOrdenProduccion.crear();
}

void crearSemaforos()
{
    Semaforo mutexAlmacenTerminados(NOMBRE_ARCHIVO, LETRA_SEM_ALMACEN_TERMINADOS, 1);
    mutexAlmacenTerminados.crear();
}


int main(int argc, char** argv)
{
    crearColas();
    crearShmems();
    crearSemaforos();
    return 0;
}

