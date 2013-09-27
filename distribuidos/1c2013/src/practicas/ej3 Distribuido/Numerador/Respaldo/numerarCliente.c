/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "numerador.h"


int numerarCliente(char *host)
{
	CLIENT *clnt;
	retorno  *result_1;
	char *numeradorcliente_2_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, NUMERADOR, NUMERADOR2, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = numeradorcliente_2((void*)&numeradorcliente_2_arg, clnt);
	if (result_1 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */

	return (*result_1).retorno_u.numero;
}

