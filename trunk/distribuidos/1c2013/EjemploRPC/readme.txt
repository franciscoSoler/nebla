Secuencia de programas RPC con la interacci—n request / reply 
           Se envia un mensaje a una subrutina remota que  
                         responde con codigo de retorno (OBLIGATORIO) y un mensaje si la operaci—n fue exitosa
            Se usan tipos de datos que requieran rutinas expl’citas de XDR
			

Programados:
   printmsg_iv.c: programa con la subrutina SIN RPC


   msg_iv.x: declaraci—n de las subrutinas en RPC (para el rpcgen)
 
   remprint_iv.c: programa cliente que invoca a una subrutina RPC que env’a un mensaje y 
                devuelve el codigo de retorno y un mensaje:
                   si devuelve = 1: la subrutina recibi— el mensaje, lo registr— y devuelve un mensaje
                      devuelve = 0: error en el registro, no devuelve un mensaje 
					  
   msg_iv_proc.c: subrutina remota que recibe el mensaje y lo graba en un log (log_iv.txt) y 
                   devuelve el codigo de retorno (1 o 0 (ver anterior)) y un mensaje si corresponde

Generados por el rpcgen:
   msg_iv.h: estructuras, constantes y declaraci—n de las subrutinas en "C" del msg_iv.x

   msg_iv_clnt.c: client stub  (subrutina local que emula la remota) 

   msg_iv_svc.c: server stub   (programa remoto que emula el programa principal)

   msg-iv_xdr.c: subrutinas XDR que debe invocar el client-stub y el server_stub.



