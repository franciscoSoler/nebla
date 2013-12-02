#ifndef _COMMON_NUMERADOR_H_
#define _COMMON_NUMERADOR_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define HOST "LOCALHOST"

#define MAX_CLIENTES 50
#define MAX_VENDEDORES 10

#define DIRECTORY_NUMERADOR 	"./DNumerador"

#define ID_NUMERADOR_CLIENTES 	 1
#define ID_NUMERADOR_VENDEDORES  2

typedef enum {
	DESASIGNADO,
	ASIGNADO
} EstadoClientes;

typedef struct {
	EstadoClientes idClientes[MAX_CLIENTES];
} ClientesInfo;

typedef enum {
	NOASIGNADO,
	LIBRE,
	OCUPADO
} EstadoVendedores;

typedef struct {
	EstadoVendedores idVendedores[MAX_VENDEDORES];
} VendedoresInfo;

#endif
