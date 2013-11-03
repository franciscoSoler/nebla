/* 
 * File:   AlmacenProductosTerminados.cpp
 * Author: fede
 *
 * Created on October 27, 2013, 2:04 PM
 */

#include <cstdlib>
#include <unistd.h>
#include "const.h"
#include "Semaforo.h"
#include "MemoriaCompartida.h"

using namespace std;

int main(int argc, char** argv)
{
    /* Nada de comportamiento! Sólo lo uso para agregar aleatóriamente productos 
     * y reservas simulando que ya hubo comportamiento. */
    Semaforo mutexAlmacenTerminados(NOMBRE_ARCHIVO, LETRA_SEM_ALMACEN_TERMINADOS);
    mutexAlmacenTerminados.obtener();
    
    MemoriaCompartida shmemAlmacenTerminados(NOMBRE_ARCHIVO, LETRA_SHMEM_ALMACEN_TERMINADOS, TAM_ALMACEN * sizeof(EspacioAlmacenProductos));
    EspacioAlmacenProductos* almacenTerminados = (EspacioAlmacenProductos*) shmemAlmacenTerminados.obtener();
    
    MemoriaCompartida shmemNumeroOrdenProduccion(NOMBRE_ARCHIVO, LETRA_SHMEM_NRO_OP, sizeof(int));
    int* numeroOP = (int*) shmemNumeroOrdenProduccion.obtener();

    MemoriaCompartida shmemNumeroOrdenCompra(NOMBRE_ARCHIVO, LETRA_SHMEM_NRO_OC, sizeof(int));
    int* numeroOC = (int*) shmemNumeroOrdenCompra.obtener();
        
    /* Lleno arbitrariamente 1/3 del almacén. */
    srand(time(NULL) + getpid());
    mutexAlmacenTerminados.p();
    *numeroOC = 0;
    *numeroOP = 0;
    for(int i = 0; i < TAM_ALMACEN / 3; i++)
    {
	int numeroEspacio = rand() % TAM_ALMACEN;
	OrdenDeCompra oc;
	OrdenDeProduccion op;
	oc.vendedor = -1;
	oc.cliente = -1;
	oc.numero = rand();
	op.vendedor = -1;
	op.numero = rand();
		
	switch(rand() % 5)
	{
	    case 0:
		almacenTerminados[numeroEspacio].estado = PRODUCTO_PARA_ENTREGAR;
		break;
	    case 1:
		almacenTerminados[numeroEspacio].estado = RESERVADO_DISPONIBLE;
		break;
	    case 2:
		almacenTerminados[numeroEspacio].estado = PRODUCTO_DISPONIBLE;
		break;
	    case 3:
		almacenTerminados[numeroEspacio].estado = RESERVADO_VENDIDO;
		break;	
	}
	
	almacenTerminados[numeroEspacio].ordenCompra = oc;
	almacenTerminados[numeroEspacio].ordenProduccion = op;
	almacenTerminados[numeroEspacio].ordenProduccion.tipoProducto = rand() % 3;
    }
    mutexAlmacenTerminados.v();
    
    while(true)
    {
	mutexAlmacenTerminados.p();
	for(int i = 0; i < TAM_ALMACEN; i++)
	{
	    EspacioAlmacenProductos espacio = almacenTerminados[i];

	    cout << "Espacio número " << i << " se encuentra ";
	    switch(espacio.estado)
	    {
		case VACIO:
		    cout << "vacío." << endl;
		    break;
		case PRODUCTO_PARA_ENTREGAR:
		    cout << "con un producto de tipo " << espacio.ordenProduccion.tipoProducto << " para entregar." << endl;
		    break;
		case PRODUCTO_DISPONIBLE:
		    cout << "con un producto de tipo " << espacio.ordenProduccion.tipoProducto << " sin dueño." << endl;
		    break;
		case RESERVADO_DISPONIBLE:
		    cout << "reservado para un producto de tipo " << espacio.ordenProduccion.tipoProducto << " sin dueño." << endl;
		    break;
		case RESERVADO_VENDIDO:
		    cout << "reservado para un producto de tipo " << espacio.ordenProduccion.tipoProducto << " con dueño." << endl;
	    }

	}
	mutexAlmacenTerminados.v();
	char a = getchar();
    }

    return 0;
}