/*
 * remprint_iv.c:  version remota de "printmsg_iv.c" 
 * Envia un mensaje que se graba en un log y 
 *           recibe un mensaje si la operacion de log fue exitosa
 *           sino recibe un mensaje de error
 *           usa TCP como capa de transporte
 */
#include <stdio.h>
#include <rpc/rpc.h>		
#include "msg_iv.h"    /* generado por rpcgen */ 	

extern int cod_ret;
int main(int argc, char *argv[])
{
	CLIENT *cl;       /* estructura para el handle */
        char *server;   
        char *mes;
        char **pmes;
        static struct retorno *vuelta;
        server=argv[1];
	if (argc < 2) {
		printf("uso: %s host <message>\n", argv[0]);
		return(1);
	}
        mes=argv[2];
        pmes=&mes;
        cl=clnt_create(server,MESSAGEPROG,MESSAGEVERS,"tcp"); /* obtener el handle */
        if (cl == NULL ) {          /* falta el portmapper o el server no se registro */
                    clnt_pcreateerror(server);     
		  return (1);
		} 
        vuelta = printmessage_2(pmes,cl); 
	if (! vuelta->cod_ret ) {               /* cod_ret = 0 el log no se pudo hacer */
		printf("%s: No puedo grabar su Mensaje \n",
			argv[0]);
		return(1);
	} 
	printf("Mensaje entregado\n");   /* log remoto exitoso */
	printf("Respuesta: %s \n",vuelta->retorno_u.message);    /*mensaje recibido */
	return(0);
}
