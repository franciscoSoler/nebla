/*
 *   Rutina remota (server). Se uso el esqueleto
 *   generado por rpcgen -a msg_iv.x  (version antigua)
 */

#include <stdio.h>
#include <rpc/rpc.h>	/* necesario siempre */ 
#include "msg_iv.h"	       /* generado por rpcgen */ 

retorno * printmessage_2_svc(char **p1, struct svc_req *rqstp)
{
	static retorno  resp1;  /* por el Segmentation Fault */
	FILE *f;
         
	/* mensaje de retorno exitoso, RPC solamente lo devuelve si resp1.cod_ret = 1 */
	
	
	strcpy (resp1.retorno_u.message,"Mensaje grabado:  "); /* asignacion de string segun c */
	strcat(resp1.retorno_u.message, *p1);
         resp1.cod_ret=1;     /* exito */

	f = fopen("log_iv.txt", "a+");  /* hace un log de los mensajes recibidos en log_iv.txt */
	if (f == NULL) {     /* no pudo */
	          resp1.cod_ret = 0;  /* fracaso */
                	return ( &resp1);   /* un entero (cod_ret) */
	}
	fprintf(f, "%s\n", *p1);
	fclose(f);     /*exito */
	printf("servidor: %s\n", resp1.retorno_u.message);
         return (&resp1);    /* el pointer al mensaje (message) */
}
