/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _NUMERADORCLIENTES_H_RPCGEN
#define _NUMERADORCLIENTES_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct retorno {
	int cod_ret;
	union {
		int numero;
		char informacion[800];
	} retorno_u;
};
typedef struct retorno retorno;

#define NUMERADORCLIENTE 0x20000099
#define NUMERADORCLIENTE1 1

#if defined(__STDC__) || defined(__cplusplus)
#define OBTENERIDCLIENTE 1
extern  retorno * obteneridcliente_1(void *, CLIENT *);
extern  retorno * obteneridcliente_1_svc(void *, struct svc_req *);
#define DEVOLVERIDCLIENTE 2
extern  void * devolveridcliente_1(int *, CLIENT *);
extern  void * devolveridcliente_1_svc(int *, struct svc_req *);
#define RESETEARCLIENTES 3
extern  void * resetearclientes_1(void *, CLIENT *);
extern  void * resetearclientes_1_svc(void *, struct svc_req *);
#define LISTADOCLIENTES 4
extern  retorno * listadoclientes_1(void *, CLIENT *);
extern  retorno * listadoclientes_1_svc(void *, struct svc_req *);
extern int numeradorcliente_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define OBTENERIDCLIENTE 1
extern  retorno * obteneridcliente_1();
extern  retorno * obteneridcliente_1_svc();
#define DEVOLVERIDCLIENTE 2
extern  void * devolveridcliente_1();
extern  void * devolveridcliente_1_svc();
#define RESETEARCLIENTES 3
extern  void * resetearclientes_1();
extern  void * resetearclientes_1_svc();
#define LISTADOCLIENTES 4
extern  retorno * listadoclientes_1();
extern  retorno * listadoclientes_1_svc();
extern int numeradorcliente_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_retorno (XDR *, retorno*);

#else /* K&R C */
extern bool_t xdr_retorno ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_NUMERADORCLIENTES_H_RPCGEN */
