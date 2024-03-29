/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "../numeradorClientes.h"
#include "../commonNumerador.h"

int numerarCliente(char *host);
void devolverCliente(char *host, int numero);
void resetearClientes(char *host);
void obtenerListadoClientes(char *host, char *listado);

int
main (int argc, char *argv[])
{
	char *host;
	char deboSeguir;

	if (argc == 1) {
		host = "LOCALHOST";
	} else if (argc == 2) {
		host = argv[1];
	} else {
		printf ("Uso: %s server_host\n", argv[0]);
		exit (1);
	}

	printf("Ingrese una opcion:\n\tc - ObtenerCliente\n\td - DevolverCliente\n\tr - Resetear Clientes\n\tl - Listar Clientes\n\tf - Fin\n");

	deboSeguir = 0;
	while (deboSeguir == 0) {
                int cliente;
		char c;

		scanf("%c", &c);
		
		printf("Opcion elegida: %c\n", c);
		switch (c) {
			case 'c':
				cliente = numerarCliente(host);
				printf("Cliente obtenido: %d.\n", cliente);
				break;
			case 'd':
				printf("Ingrese el cliente a devolver.\n");
				scanf("%d", &cliente);
				devolverCliente(host, cliente);
				break;
			case 'r':
				printf("Reseteando clientes.\n");
				resetearClientes(host);
				break;
			case 'l': {
				int i;
				char clientes[MAX_CLIENTES];
				printf("Listando Clientes.\n");
				obtenerListadoClientes(host, clientes);
				for (i = 0; i < MAX_CLIENTES; ++i) printf ("Cliente %d: %d\n",i+1,clientes[i]);
				break;
			}
			case 'f':
				deboSeguir = 1;
				break;
			default:
				printf("Error, los valores posibles son:\n\tc - ObtenerCliente\n\td - DevolverCliente\n\tr - Resetear Clientes\n\tl - Listar Clientes\n\tf - Fin\n");
				break;	
		}
	}
	
	exit (0);
}

int numerarCliente(char *host)
{
	CLIENT *clnt;
	retorno  *result_1;
	char *obteneridcliente_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, NUMERADORCLIENTE, NUMERADORCLIENTE1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = obteneridcliente_1((void*)&obteneridcliente_1_arg, clnt);
	if (result_1 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */

	return (*result_1).retorno_u.numero;
}


void devolverCliente(char *host, int numero)
{
	CLIENT *clnt;

	void  *result_2;
	int  devolveridcliente_1_arg;

	devolveridcliente_1_arg = numero;
	
#ifndef	DEBUG
	clnt = clnt_create (host, NUMERADORCLIENTE, NUMERADORCLIENTE1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_2 = devolveridcliente_1(&devolveridcliente_1_arg, clnt);
	if (result_2 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void resetearClientes(char *host)
{
	CLIENT *clnt;
	
	void  *result_3;
	char *resetearclientes_1_arg;
	
#ifndef	DEBUG
	clnt = clnt_create (host, NUMERADORCLIENTE, NUMERADORCLIENTE1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_3 = resetearclientes_1((void*)&resetearclientes_1_arg, clnt);
	if (result_3 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


void obtenerListadoClientes(char *host, char *listado)
{
	CLIENT *clnt;

	retorno  *result_4;
	char *listadoclientes_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, NUMERADORCLIENTE, NUMERADORCLIENTE1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_4 = listadoclientes_1((void*)&listadoclientes_1_arg, clnt);
	if (result_4 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */

	memcpy(listado,(*result_4).retorno_u.informacion, MAX_CLIENTES);
	
}
