/*
 * rprintmsg.c: version remota de "printmsg.c"
 */
#include <stdio.h>
#include <rpc/rpc.h>		
#include "msg_ida.h"    /* generado por rpcgen */ 	

int main(argc, argv)
	int argc;
	char *argv[];
{
	CLIENT *cl;
	int *result;
	char *server;
	char *message;

	if (argc < 3) {
		printf("uso: %s host mensaje\n", argv[0]);
		return(1);
	}

	/*
	 *   Recuperar los parametros de la linea de comando
	 */
	server = argv[1];
	message = argv[2];

	/*
	 * Crear el "handle" del cliente para llamar MESSAGEPROG en el 
	 * servidor. 
	 * Usamos  "udp" para la  comunicacion.
	 */
	cl = clnt_create(server, MESSAGEPROG, MESSAGEVERS, "udp");
	if (cl == NULL) {
		/*
		 * No se pudo hacer contacto
		 * error y adios.
		 */
		clnt_pcreateerror(server);
		return(1);
	}
	
	/*
	 * llamar al procedure "printmessage" en el servidor
	 */
	result = printmessage_1(&message, cl);
	if (result == NULL) {
		/*
		 * error durante la comunicacion. 
	 	 * 
		 */
		clnt_perror(cl, server);
		return(1);
	}

	/*
	 * Contacto con el remote procedure.
	 */
	if (*result == 0) {
		/*
		 * el servidor no pudo imprimir el mensaje  
		 */
		printf("%s: %s no pudo imprimir el mensaje\n", 
			argv[0], server);	
		return(1);
	} 

	/*
	 * exito 
	 */
	printf("Mensaje entregado a %s!\n", server);
         return(0);
}
