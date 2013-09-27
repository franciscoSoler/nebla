Secuencia de programas RPC con la interaccion minima y necesaria
           Se envia un mensaje a una subrutina remota que  
                         responde con codigo de retorno (OBLIGATORIO)  solamente:
            No se usan tipos de datos que requieran rutinas explicitas de XDR
			

Programados:
   printmsg_ida.c: programa con la subrutina SIN RPC

   msg_ida.x: declaracion de las subrutinas en RPC (para el rpcgen)
 
   rprintmsg_ida.c: programa cliente que invoca a una subrutina RPC que envia un mensaje y 
                devuelve el codigo de retorno de como se ejecuto la operacion:
                   si devuelve = 1: la subrutina recibio el mensaje y lo registro
                      devuelve = 0: error en el registro
					  
   msg_ida_proc.c: subrutina remota que recibe el mensaje y lo graba en un log (log_ida.txt) y 
                   devuelve el codigo de retorno (1 o 0 (ver anterior))

Generados por el rpcgen:
   msg_ida.h: estructuras, constantes y declaracion de las subrutinas en "C" del msg_ida.x
   
   msg_ida_clnt.c: client stub  (subrutina local que emula la remota) 
   
   msg_ida_svc.c: server stub   (programa remoto que emula el programa principal)



