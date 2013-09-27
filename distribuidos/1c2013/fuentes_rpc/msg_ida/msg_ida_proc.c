/*
 * msg_ida_proc.c:  implementacion del procedure remoto
 *                       cada mensaje que recibe se graba en un log y 
 *                       devuelve 1: si la grabacion se hizo exitosamente
 *                                       0: en otro caso (error)
 */
#include <stdio.h>
#include <rpc/rpc.h>	/* necesario siempre */ 
#include "msg_ida.h"	         /* generado por rpcgen */ 

/*
 * Version remota de "printmessage" 
 */
int *printmessage_1_svc(char **msg, struct svc_req *svc_req)
         // char **msg;                        /* doble indireccion  obligada por rpcgen */	
	// struct svc_req *svc_req;    /* handle de la comunicacion */
{
	static int result; /*static obligatorio para los resultados */
	FILE *f;

	f = fopen("logida.txt", "a+");   /* hace un log en logida.txt de los mensajes recibidos */
	if (f == NULL) {       /* no pudo abrir el archivo */
		result = 0;
		return (&result);
	} 
	fprintf(f, "%s\n", *msg);  /* graba el mensaje en el archivo*/
	fclose(f);
	result = 1;     /* termina bien */
	printf("servidor: %s\n", *msg);
	return (&result);
}
