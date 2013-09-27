#ifndef _COMMON_NUMERADOR_H_
#define _COMMON_NUMERADOR_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAX_CLIENTES 10
#define MAX_VENDEDORES 10

#define DIRECTORY_NUM "/home/distribuidos"

#define ID_SEMCONTROL_NUMERADOR	9
#define ID_SHMEM_NUMERADOR	10

typedef struct {
	char numerosVendedores[MAX_VENDEDORES]; 
	char numerosClientes[MAX_CLIENTES];
} NumeradorInfo;

#endif
