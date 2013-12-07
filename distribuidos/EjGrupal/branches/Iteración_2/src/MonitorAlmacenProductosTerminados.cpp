/* 
 * File:   AlmacenProductosTerminados.cpp
 * Author: fede
 *
 * Created on October 27, 2013, 2:04 PM
 */

#include <cstdlib>
#include <unistd.h>
#include "Common.h"
#include "IPCs/Semaphore/Semaphore.h"
#include <IPCs/IPCTemplate/SharedMemory.h>
//#include "Objects/SmMemAlmacenProductosTerminados.h"

using namespace std;

int main(int argc, char** argv)
{
    /* Nada de comportamiento! Sólo lo uso para agregar aleatóriamente productos 
     * y reservas simulando que ya hubo comportamiento. */
    IPC::Semaphore mutex = IPC::Semaphore("Acceso Almacen Terminados");
    mutex.getSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
    
    IPC::SharedMemory<AlmacenProductosTerminados> shmemAlmacenTerminados("shMemAlmacenTerminados");
    shmemAlmacenTerminados.getSharedMemory(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS);
    
    AlmacenProductosTerminados almacenPT;
    
    while(true)
    {
	mutex.wait();
	
        shmemAlmacenTerminados.read(&almacenPT);
        EspacioAlmacenProductos matrizAPT[TAM_ALMACEN] = almacenPT.almacen;
        
	for(int i = 0; i < TAM_ALMACEN; i++)
	{
	    EspacioAlmacenProductos espacio = matrizAPT[i];
	    cout << "Espacio numero " << i << " se encuentra ";
	    switch(espacio.estado)
	    {
		case VACIO:
		    cout << "vacio." << endl;
		    break;
		case PRODUCTO_PARA_ENTREGAR:
		    cout << "con un producto de tipo " << espacio.tipoProducto << " para entregar." << endl;
		    break;
		case PRODUCTO_DISPONIBLE:
		    cout << "con un producto de tipo " << espacio.tipoProducto << " sin dueño." << endl;
		    break;
		case RESERVADO_DISPONIBLE:
		    cout << "reservado para un producto de tipo " << espacio.tipoProducto << " sin dueño." << endl;
		    break;
		case RESERVADO_VENDIDO:
		    cout << "reservado para un producto de tipo " << espacio.tipoProducto << " con dueño." << endl;
	    }

	}
	mutex.signal();
	
	//cout << endl;
	char a = getchar(); a = a;
    }

    return 0;
}