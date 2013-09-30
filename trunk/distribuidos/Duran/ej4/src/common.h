#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

#define DIRECTORY "/home/Duran"

#define ID_COLA_RECIPIENTES	1

#define LLENADOR 	1
#define ETIQUETADOR 	2
#define IMPRESOR	3

typedef struct {
	int idProductor;
	int idRecipiente;
} Recipiente;

typedef struct {
	long mtype;
	Recipiente recipiente;
} RecipienteMessage;

#endif
