#ifndef _COMMON_NUMERADOR_H_
#define _COMMON_NUMERADOR_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAX_CLIENTES 50
#define MAX_VENDEDORES 10

#define DIRECTORY_NUMERADOR_CLIENTES 	"./DNumClientes"

#define ID_NUMERADOR_CLIENTES 	 1

typedef struct {
	char idClientes[MAX_CLIENTES];
} ClientesInfo;

#endif
