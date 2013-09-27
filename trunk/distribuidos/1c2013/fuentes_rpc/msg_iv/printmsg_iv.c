/*
 * printmsg.c:  invoca una subrutina que hace un log de un mensaje 
 *                      y devuelve un mensaje 
 */
#include <stdio.h>
#include <string.h>
typedef struct       /* estructura con variantes */ 
               {int cod_ret;  /* si hay error */
               union {
                   char message[800];} retorno_u ;   /* no hay error */
                } retorno;

retorno *printmessage(char *p1);

int main(int argc, char *argv[])
{
	char *mes;
        retorno *vuelta;
	if (argc < 2) {
		printf( "uso: %s <message>\n", argv[0]);
		return(1);
	}
        mes=argv[1];
        vuelta = printmessage(mes); 
	if (! vuelta->cod_ret ) {
		printf("%s: No puedo grabar su Mensaje \n",
			argv[0]);
		return(1);
	} 
	printf("Mensaje entregado\n");
	printf("Respuesta: %s \n",vuelta->retorno_u.message);
	return(0);
}

/*
 * Graba el mensaje en un archivo log (log_sinRPC_iv-txt) 
 * Devuelve un mensaje indicando si se pudo o no grabar 
 */
retorno *printmessage(char *p1)
	
{
        static retorno resp;   /* static para evitar el Segmentation Fault */
	FILE *f;
 
        strcpy (resp.retorno_u.message,"Todo bien ");
                              /* asignacion de string segun c */ 
        resp.cod_ret=1;     /* exito */

	f = fopen("log_sinRPC_iv.txt", "a+");
	if (f == NULL) {     /* no pudo */
	          resp.cod_ret = 0;  /* fracaso */
                	return ((retorno *) &resp);   /* un entero (cod_ret) */
	}
	fprintf(f, "%s\n", p1);
	fclose(f);     /*exito */
        return ((retorno *) &resp);    /* el pointer al mensaje (message) */
}
