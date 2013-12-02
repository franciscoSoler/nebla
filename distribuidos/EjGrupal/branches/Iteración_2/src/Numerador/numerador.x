/* 
 * estructura de la devolucion de resultados de la invocacion a la subrutina en RPC 
 * ESTE TIPO DE ESTRUCTURAS SON OBLIGATORIAS SI HAY QUE DEVOLVER RESULTADOS
 */

struct RetornoCliente{
	int idCliente;
	int idVendedorLibre;
};

union retornoCliente
      switch (int cod_ret) {
             case 1: struct RetornoCliente retorno;
             case 2: char informacion[800];
             default: void;
             };

union retornoVendedor
      switch (int cod_ret) {
             case 1: int idVendedor;
             case 2: char informacion[800];
             default: void;
             };
			 
program NUMERADORCLIENTE {
	version NUMERADORCLIENTE1 {
		retornoCliente OBTENERIDCLIENTE() = 1;
		void DEVOLVERIDCLIENTE(int) = 2;
		void RESETEARCLIENTES() = 3;
		retornoCliente LISTADOCLIENTES() = 4;
	} = 1;
} = 0x20000099;

program NUMERADORVENDEDOR {
	version NUMERADORVENDEDOR1 {
		retornoVendedor OBTENERIDVENDEDOR() = 1;
		void VENDEDORLIBRE(int) = 2;
		void DEVOLVERIDVENDEDOR(int) = 3;
		void RESETEARVENDEDORES() = 4;
		retornoVendedor LISTADOVENDEDORES() = 5;
	} = 1;
} = 0x2000009A;