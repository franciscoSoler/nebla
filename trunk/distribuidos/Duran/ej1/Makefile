# CONFIGURACION
################

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

# Si usa funciones de math.h, descomentá (quitale el '#' a) la siguiente línea.
#math = si

# Descomentar si se quiere ver como se invoca al compilador
verbose = si


# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
CFLAGS = -Wall -pedantic -pedantic-errors

# Para que explote lo mas posible
#CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
#CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++98

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Linkeo con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

ifdef static
LDFLAGS += -static
endif

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

# Si no especifica archivos, tomo todos.

fuentes_contAdministrador ?= $(wildcard ControladorAdministrador.$(extension))
fuentes_puertaEntrada ?= $(wildcard PuertaEntrada.$(extension))
fuentes_puertaSalida ?= $(wildcard PuertaSalida.$(extension))
fuentes_ipc ?= $(wildcard ./IPC/*/*.$(extension))
fuentes_terminator ?= $(wildcard Terminator.$(extension))
fuentes_launcher ?= $(wildcard Launcher.$(extension))
fuentes_administrador ?= $(wildcard Administrador.$(extension))


# REGLAS
#########

.PHONY: all clean

all: launcher terminator administrador puertaEntrada puertaSalida

o_ipc_files = $(patsubst %.$(extension),%.o,$(fuentes_ipc))
o_terminator_files = $(patsubst %.$(extension),%.o,$(fuentes_terminator)) $(o_ipc_files)
o_launcher_files = $(patsubst %.$(extension),%.o,$(fuentes_launcher)) $(o_ipc_files)

o_contAdministrador_files = $(patsubst %.$(extension),%.o,$(fuentes_contAdministrador)) $(o_ipc_files)
o_administrador_files = $(patsubst %.$(extension),%.o,$(fuentes_administrador)) $(o_contAdministrador_files)

o_puertaEntrada_files = $(patsubst %.$(extension),%.o,$(fuentes_puertaEntrada)) $(o_ipc_files)
o_puertaSalida_files = $(patsubst %.$(extension),%.o,$(fuentes_puertaSalida)) $(o_ipc_files)

%.deps: %.$(extension)
	@set -e; rm -f $@; \
		$(old) -MM -MG -MP $(CXXFLAGS) $(CFLAGS) $< -MF $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(o_files:.o=.deps)

launcher: $(o_launcher_files)
	@if [ -z "$(o_launcher_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa launcher (archivos launcher*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_launcher_files) $(LOADLIBES) $(LDLIBS) -o launcher $(LDADD)

terminator: $(o_terminator_files)
	@if [ -z "$(o_terminator_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa terminator (archivos terminator*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_terminator_files) $(LOADLIBES) $(LDLIBS) -o terminator $(LDADD)
	
puertaEntrada: $(o_puertaEntrada_files)
	@if [ -z "$(o_puertaEntrada_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa puertaEntrada (archivos puertaEntrada*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_puertaEntrada_files) $(LOADLIBES) $(LDLIBS) -o puertaEntrada $(LDADD)

puertaSalida: $(o_puertaSalida_files)
	@if [ -z "$(o_puertaSalida_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa puertaSalida (archivos puertaSalida*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_puertaSalida_files) $(LOADLIBES) $(LDLIBS) -o puertaSalida $(LDADD)

administrador: $(o_administrador_files)
	@if [ -z "$(o_administrador_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa administrador (archivos administrador*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_administrador_files) $(LOADLIBES) $(LDLIBS) -o administrador $(LDADD)

clean:
	@$(RM) -fv *.o launcher terminator puertaEntrada puertaSalida administrador IPC/*/*.o

