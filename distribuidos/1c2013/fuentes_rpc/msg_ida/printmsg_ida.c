/*
 * printmsg.c:  imprime un mensaje en la consola 
 */
#include <stdio.h>

int printmessage(char *msg);

int main(int argc, char *argv[])
{
	char *message;

	if (argc < 2) {
		printf ("usage: %s <message>\n", argv[0]);
		return(1);
	}
	message = argv[1];

	if (!printmessage(message)) {
		printf("%s: No puedo imprimir su Mensaje \n",
			argv[0]);
		return(1);
	} 
	printf("Mensaje entregado\n");
	return(0);
}

/*
 * Graba un mensaje en un archivo de log y lo muestra
 * Devuelve un valor indicando si se pudo o no grabar en el log 
 */
int printmessage(char *msg)
{
	FILE *f;

	f = fopen("log_ida_sinRPC.txt", "a+");
	if (f == NULL) {     /* no pudo */
		return (0);   /* devuelve error */
	}
	fprintf(f, "%s\n", msg);
	fclose(f);     /*exito */
	printf("%s\n", msg);
	return(1);
}
