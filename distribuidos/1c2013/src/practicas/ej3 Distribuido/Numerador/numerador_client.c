/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "numerador.h"

#include "commonNumerador.h"

extern int numerarVendedor(char *host);
extern int numerarCliente(char *host);
extern void devolverVendedor(char *host, int numero);
extern void devolverCliente(char *host, int numero);

extern void resetearClientes(char *host);
extern void resetearVendedores (char *host);
extern void obtenerListadoClientes(char *host, char *listado);
extern void obtenerListadoVendedores(char *host, char *listado);

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
		printf ("Usage: %s server_host\n", argv[0]);
		exit (1);
	}

	printf("Ingrese una opcion:\n\tc - ObtenerCliente\n\tv - ObtenerVendedor\n\ta - DevolverCliente\n\tb - DevolverVendedor\n\tr - Resetear Clientes\n\tt - Resetar Vendedores\n\tl - Listar Clientes\n\tk - ListarVendedores\n\tf - Fin\n");

	deboSeguir = 0;
	while (deboSeguir == 0) {
		int cliente;
		int vendedor;
		char c;

		scanf("%c", &c);
		
		printf("Opcion elegida: %c\n", c);
		switch (c) {
			case 'c':
				cliente = numerarCliente(host);
				printf("Cliente obtenido: %d.\n", cliente);
				break;
			case 'v': 
				vendedor = numerarVendedor(host);
				printf("Vendedor obtenido: %d.\n", vendedor);
				break;
			case 'a':
				printf("Ingrese el cliente a devolver.\n");
				scanf("%d", &cliente);
				devolverCliente(host, cliente);
				break;
			case 'b':
				printf("Ingrese el vendedor a devolver.\n");
				scanf("%d", &vendedor);
				devolverVendedor(host, vendedor);
				break;
			case 'r':
				printf("Reseteando clientes.\n");
				resetearClientes(host);
				break;
			case 't':
				printf("Reseteando vendedores.\n");
				resetearVendedores(host);
				break;
			case 'l': {
				int i;
				char clientes[MAX_CLIENTES];
				printf("Listando Clientes.\n");
				obtenerListadoClientes(host, clientes);
				for (i = 0; i < MAX_CLIENTES; ++i) printf ("Cliente %d: %d\n",i+1,clientes[i]);
				break;
			}
			case 'k': {
				int j;
				char vendedores[MAX_VENDEDORES];
				printf("Listando Vendedores.\n");
				obtenerListadoVendedores(host, vendedores);
				for (j = 0; j < MAX_VENDEDORES; ++j) printf ("Vendedor %d: %d\n",j+1,vendedores[j]);
				break;
			}
			case 'f':
				deboSeguir = 1;
				break;
			default:
				printf("Error, los valores posibles son:\n\tc - ObtenerCliente\n\tv - ObtenerVendedor\n\ta - DevolverCliente\n\tb - DevolverVendedor\n\tr - Resetear Clientes\n\tt - Resetar Vendedores\n\tl - Listar Clientes\n\tk - ListarVendedores\n\tf - Fin\n");
				break;	
		}
	}
	
	exit (0);
}
