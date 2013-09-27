# CONFIGURACION
################

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

# Descomentar si se quiere ver como se invoca al compilador
verbose = si

# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
CFLAGS = -Wall -pedantic -pedantic-errors

# Para que explote lo mas posible
CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline -g

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++98

# Pongo flags de C para C++ también
CXXFLAGS += $(CFLAGS)

CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif

CLIENT = Numerador/clienteNumerador
SERVER = Numerador/numerador

SOURCES_CLNT.c = 
SOURCES_CLNT.h = 
SOURCES_SVC.c = 
SOURCES_SVC.h = 
SOURCES.x = Numerador/numerador.x

TARGETS_CLNT.c = Numerador/numerador_clnt.c Numerador/numerador_client.c Numerador/numerador_xdr.c Numerador/utilsClientes.c 
TARGETS_SVC.c = Numerador/numerador_svc.c Numerador/numerador_server.c Numerador/numerador_xdr.c 

TARGETS = Numerador/numerador.h Numerador/numerador_xdr.c Numerador/numerador_clnt.c Numerador/numerador_svc.c Numerador/numerador_client.c Numerador/numerador_server.c

OBJECTS_CLNT = $(SOURCES_CLNT.c:%.c=%.o) $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_SVC = $(SOURCES_SVC.c:%.c=%.o) $(TARGETS_SVC.c:%.c=%.o)

CFLAGS += -g 
LDLIBS += -lnsl
RPCGENFLAGS = 

# Si no especifica archivos, tomo todos.

fuentes_tcp ?= $(wildcard Socket/*.$(extension))

fuentes_canalEntradaCliente ?= $(wildcard ./Comunicacion/CanalEntradaCliente*.$(extension))
fuentes_canalSalidaCliente ?= $(wildcard ./Comunicacion/CanalSalidaCliente*.$(extension))
fuentes_canalEntradaVendedor ?= $(wildcard ./Comunicacion/CanalEntradaVendedor*.$(extension))
fuentes_canalSalidaVendedor ?= $(wildcard ./Comunicacion/CanalSalidaVendedor*.$(extension))

fuentes_creadorCanalesCliente ?= $(wildcard ./Comunicacion/CreadorCanalesCliente*.$(extension))

fuentes_servidorVendedorEntrada ?= $(wildcard ./Comunicacion/ServidorVendedorEntrada*.$(extension))
fuentes_servidorVendedorSalida ?= $(wildcard ./Comunicacion/ServidorVendedorSalida*.$(extension))

fuentes_contCliente ?= $(wildcard ./ControladorCliente.$(extension))
fuentes_contVendedor ?= $(wildcard ./ControladorVendedor.$(extension)) $(wildcard ControladorTickets.$(extension))

fuentes_ipc ?= $(wildcard ./IPC/*/*.$(extension))
fuentes_terminator ?= $(wildcard Terminator.$(extension))
fuentes_launcher_clientes ?= $(wildcard LauncherClientes.$(extension))
fuentes_launcher_vendedores ?= $(wildcard LauncherVendedores.$(extension))
fuentes_vendedor ?= $(wildcard Vendedor.$(extension))
fuentes_cliente ?= $(wildcard Cliente.$(extension))

fuentes_launcher_numerador ?= $(wildcard LauncherNumerador.$(extension))

# REGLAS
#########

.PHONY: all clean

all: launcherNumerador launcherClientes launcherVendedores terminator cliente vendedor canalEntradaCliente canalSalidaCliente canalEntradaVendedor canalSalidaVendedor creadorCanalesCliente servidorVendedorEntrada servidorVendedorSalida $(SERVER) $(CLIENT)

$(TARGETS) : $(SOURCES.x) 
	rpcgen $(RPCGENFLAGS) $(SOURCES.x)

$(OBJECTS_CLNT) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(TARGETS_CLNT.c) 

$(OBJECTS_SVC) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(TARGETS_SVC.c) 

o_tcp_files = $(patsubst %.$(extension),%.o,$(fuentes_tcp)) 
o_ipc_files = $(patsubst %.$(extension),%.o,$(fuentes_ipc))

o_canalEntradaCliente_files = $(patsubst %.$(extension),%.o,$(fuentes_canalEntradaCliente)) $(o_ipc_files) $(o_tcp_files)
o_canalSalidaCliente_files = $(patsubst %.$(extension),%.o,$(fuentes_canalSalidaCliente)) $(o_ipc_files) $(o_tcp_files)
o_canalEntradaVendedor_files = $(patsubst %.$(extension),%.o,$(fuentes_canalEntradaVendedor)) $(o_ipc_files) $(o_tcp_files)
o_canalSalidaVendedor_files = $(patsubst %.$(extension),%.o,$(fuentes_canalSalidaVendedor)) $(o_ipc_files) $(o_tcp_files)
o_creadorCanalesCliente_files = $(patsubst %.$(extension),%.o,$(fuentes_creadorCanalesCliente)) $(o_ipc_files) $(o_tcp_files) 
o_servidorVendedorEntrada_files = $(patsubst %.$(extension),%.o,$(fuentes_servidorVendedorEntrada)) $(o_ipc_files) $(o_tcp_files) 
o_servidorVendedorSalida_files = $(patsubst %.$(extension),%.o,$(fuentes_servidorVendedorSalida)) $(o_ipc_files) $(o_tcp_files) 

o_terminator_files = $(patsubst %.$(extension),%.o,$(fuentes_terminator))  $(o_ipc_files)
o_launcher_clientes_files = $(patsubst %.$(extension),%.o,$(fuentes_launcher_clientes))  $(o_ipc_files)
o_launcher_vendedores_files = $(patsubst %.$(extension),%.o,$(fuentes_launcher_vendedores))  $(o_ipc_files)

o_contCliente_files = $(patsubst %.$(extension),%.o,$(fuentes_contCliente)) $(o_ipc_files)

o_contVendedor_files = $(patsubst %.$(extension),%.o,$(fuentes_contVendedor)) $(o_ipc_files)

o_vendedor_files = $(patsubst %.$(extension),%.o,$(fuentes_vendedor)) $(o_contVendedor_files)
o_cliente_files = $(patsubst %.$(extension),%.o,$(fuentes_cliente))  $(o_contCliente_files)

o_launcher_numerador_files = $(patsubst %.$(extension),%.o,$(fuentes_launcher_numerador)) $(o_ipc_files)

$(CLIENT) : $(OBJECTS_CLNT) 
	$(LINK.c) -o $(CLIENT) $(OBJECTS_CLNT) $(LDLIBS) 

$(SERVER) : $(OBJECTS_SVC) 
	$(LINK.c) -o $(SERVER) $(OBJECTS_SVC) $(LDLIBS)

%.deps: %.$(extension)
	@set -e; rm -f $@; \
		$(old) -MM -MG -MP $(CXXFLAGS) $(CFLAGS) $< -MF $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(o_files:.o=.deps)

launcherNumerador: $(o_launcher_numerador_files)
	@if [ -z "$(o_launcher_numerador_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa launcher (archivos launcherNumerador*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_launcher_numerador_files) $(LOADLIBES) $(LDLIBS) -o launcherNumerador $(LDADD)

launcherClientes: $(o_launcher_clientes_files)
	@if [ -z "$(o_launcher_clientes_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa launcher (archivos launcherClientes*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_launcher_clientes_files) $(LOADLIBES) $(LDLIBS) -o launcherClientes $(LDADD)

launcherVendedores: $(o_launcher_vendedores_files)
	@if [ -z "$(o_launcher_vendedores_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa launcher (archivos launcherVendedores*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_launcher_vendedores_files) $(LOADLIBES) $(LDLIBS) -o launcherVendedores $(LDADD)


terminator: $(o_terminator_files)
	@if [ -z "$(o_terminator_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa terminator (archivos terminator*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_terminator_files) $(LOADLIBES) $(LDLIBS) -o terminator $(LDADD)
	
vendedor: $(o_vendedor_files)
	@if [ -z "$(o_vendedor_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa vendedor (archivos vendedor*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_vendedor_files) $(LOADLIBES) $(LDLIBS) -o vendedor $(LDADD)

cliente: $(o_cliente_files)
	@if [ -z "$(o_cliente_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa cliente (archivos cliente*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_cliente_files) $(LOADLIBES) $(LDLIBS) -o cliente $(LDADD)

canalEntradaCliente: $(o_canalEntradaCliente_files)
	@if [ -z "$(o_canalEntradaCliente_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa cliente (archivos canalEntradaCliente*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_canalEntradaCliente_files) $(LOADLIBES) $(LDLIBS) -o canalEntradaCliente $(LDADD)

canalSalidaCliente: $(o_canalSalidaCliente_files)
	@if [ -z "$(o_canalSalidaCliente_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa cliente (archivos canalSalidaCliente*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_canalSalidaCliente_files) $(LOADLIBES) $(LDLIBS) -o canalSalidaCliente $(LDADD)

canalEntradaVendedor: $(o_canalEntradaVendedor_files)
	@if [ -z "$(o_canalEntradaVendedor_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa cliente (archivos canalEntradaVendedor*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_canalEntradaVendedor_files) $(LOADLIBES) $(LDLIBS) -o canalEntradaVendedor $(LDADD)

canalSalidaVendedor: $(o_canalSalidaVendedor_files)
	@if [ -z "$(o_canalSalidaVendedor_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa cliente (archivos canalSalidaVendedor*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_canalSalidaVendedor_files) $(LOADLIBES) $(LDLIBS) -o canalSalidaVendedor $(LDADD)

creadorCanalesCliente: $(o_creadorCanalesCliente_files)
	@if [ -z "$(o_creadorCanalesCliente_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa cliente (archivos creadorCanalesCliente*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_creadorCanalesCliente_files) $(LOADLIBES) $(LDLIBS) -o creadorCanalesCliente $(LDADD)

servidorVendedorEntrada: $(o_servidorVendedorEntrada_files)
	@if [ -z "$(o_servidorVendedorEntrada_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa cliente (archivos servidorVendedorEntrada*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_servidorVendedorEntrada_files) $(LOADLIBES) $(LDLIBS) -o servidorVendedorEntrada $(LDADD)

servidorVendedorSalida: $(o_servidorVendedorSalida_files)
	@if [ -z "$(o_servidorVendedorSalida_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa cliente (archivos servidorVendedorSalida*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_servidorVendedorSalida_files) $(LOADLIBES) $(LDLIBS) -o servidorVendedorSalida $(LDADD)

clean:
	@$(RM) -fv *.o launcherNumerador launcherClientes launcherVendedores terminator cliente vendedor canalEntradaCliente canalSalidaCliente canalEntradaVendedor canalSalidaVendedor creadorCanalesCliente servidorVendedorEntrada servidorVendedorSalida Comunicacion/*.o Socket/*.o IPC/MessageQueue/*.o IPC/Semaphore/*.o IPC/SharedMemory/*.o Numerador/*.o $(SERVER) $(CLIENT)


