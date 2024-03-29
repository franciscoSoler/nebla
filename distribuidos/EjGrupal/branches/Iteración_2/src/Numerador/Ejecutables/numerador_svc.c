/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "numerador.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
numeradorcliente_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		int devolveridcliente_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case OBTENERIDCLIENTE:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_retornoCliente;
		local = (char *(*)(char *, struct svc_req *)) obteneridcliente_1_svc;
		break;

	case DEVOLVERIDCLIENTE:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_void;
		local = (char *(*)(char *, struct svc_req *)) devolveridcliente_1_svc;
		break;

	case RESETEARCLIENTES:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_void;
		local = (char *(*)(char *, struct svc_req *)) resetearclientes_1_svc;
		break;

	case LISTADOCLIENTES:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_retornoCliente;
		local = (char *(*)(char *, struct svc_req *)) listadoclientes_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

static void
numeradorvendedor_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		int vendedorlibre_1_arg;
		int devolveridvendedor_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case OBTENERIDVENDEDOR:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_retornoVendedor;
		local = (char *(*)(char *, struct svc_req *)) obteneridvendedor_1_svc;
		break;

	case VENDEDORLIBRE:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_void;
		local = (char *(*)(char *, struct svc_req *)) vendedorlibre_1_svc;
		break;

	case DEVOLVERIDVENDEDOR:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_void;
		local = (char *(*)(char *, struct svc_req *)) devolveridvendedor_1_svc;
		break;

	case RESETEARVENDEDORES:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_void;
		local = (char *(*)(char *, struct svc_req *)) resetearvendedores_1_svc;
		break;

	case LISTADOVENDEDORES:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_retornoVendedor;
		local = (char *(*)(char *, struct svc_req *)) listadovendedores_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (NUMERADORCLIENTE, NUMERADORCLIENTE1);
	pmap_unset (NUMERADORVENDEDOR, NUMERADORVENDEDOR1);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, NUMERADORCLIENTE, NUMERADORCLIENTE1, numeradorcliente_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (NUMERADORCLIENTE, NUMERADORCLIENTE1, udp).");
		exit(1);
	}
	if (!svc_register(transp, NUMERADORVENDEDOR, NUMERADORVENDEDOR1, numeradorvendedor_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (NUMERADORVENDEDOR, NUMERADORVENDEDOR1, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, NUMERADORCLIENTE, NUMERADORCLIENTE1, numeradorcliente_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (NUMERADORCLIENTE, NUMERADORCLIENTE1, tcp).");
		exit(1);
	}
	if (!svc_register(transp, NUMERADORVENDEDOR, NUMERADORVENDEDOR1, numeradorvendedor_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (NUMERADORVENDEDOR, NUMERADORVENDEDOR1, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
