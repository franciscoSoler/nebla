#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

// Path utilizado para inicializar los ipc a través de la función ftok()
#define DIRECTORY "../../../src"

// Ids de los IPCs (Creo que esto está mal)
#define ID_SHMEM			1
#define ID_SEMCONTROL		2
#define ID_SEMPRODUCTORES	3
#define ID_SEMANALIZADORES	4

#define CANT_ANALIZADORES 6
#define CANT_PRODUCTORES 5

typedef struct {
	int valor;
	bool estado;
} Sample;

typedef struct {
	
	Sample muestra [CANT_PRODUCTORES];
	
	/* false -> productor no esta bloqueado
	 * true -> el productor esta bloqueado y debe ser liberado
	 */
	//bool toma [CANT_PRODUCTORES];
	
	 /* Pos muestra -> Indica si los respectivos analizadores ya analizaron la muestra.
	 */
	bool sampleReady[CANT_PRODUCTORES][CANT_ANALIZADORES];
	
	/* false -> analizador no esta bloqueado
	 * true -> el analizador esta bloqueado y debe ser liberado
	 */
	bool analiza [CANT_ANALIZADORES];
} SamplesTable;

#endif
