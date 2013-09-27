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

fuentes_ipc ?= $(wildcard ./IPC/*.$(extension))
fuentes_terminator ?= $(wildcard ProcesoTerminator.$(extension)) 
fuentes_launcher ?= $(wildcard ProcesoLauncher.$(extension))
fuentes_hidrogeno ?= $(wildcard ProcesoHidrogeno.$(extension)) $(wildcard Hidrogeno.$(extension))
fuentes_oxigeno ?= $(wildcard ProcesoOxigeno.$(extension)) $(wildcard Oxigeno.$(extension))
fuentes_logger ?= $(wildcard Logger.$(extension))
fuentes_sorteador ?= $(wildcard SorteadorEntero.$(extension))

# REGLAS
#########

.PHONY: all clean

all: launcher terminator hidrogeno oxigeno

o_ipc_files = $(patsubst %.$(extension),%.o,$(fuentes_ipc))
o_sorteador= $(patsubst %.$(extension),%.o,$(fuentes_sorteador))
o_logger_files = $(patsubst %.$(extension),%.o,$(fuentes_logger))
o_terminator_files = $(patsubst %.$(extension),%.o,$(fuentes_terminator))  $(o_ipc_files)  $(o_logger_files)
o_launcher_files = $(patsubst %.$(extension),%.o,$(fuentes_launcher))  $(o_ipc_files) $(o_logger_files)

o_hidrogeno_files = $(patsubst %.$(extension),%.o,$(fuentes_hidrogeno))  $(o_ipc_files) $(o_logger_files) $(o_sorteador)
o_oxigeno_files = $(patsubst %.$(extension),%.o,$(fuentes_oxigeno))  $(o_ipc_files) $(o_logger_files) $(o_sorteador)

%.deps: %.$(extension)
	@set -e; rm -f $@; \
		$(old) -MM -MG -MP $(CXXFLAGS) $(CFLAGS) $< -MF $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(o_files:.o=.deps)

launcher: $(o_launcher_files)
	@if [ -z "$(o_launcher_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa launcher."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_launcher_files) $(LOADLIBES) $(LDLIBS) -o launcher $(LDADD)

terminator: $(o_terminator_files)
	@if [ -z "$(o_terminator_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa terminator."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_terminator_files) $(LOADLIBES) $(LDLIBS) -o terminator $(LDADD)
	
hidrogeno: $(o_hidrogeno_files)
	@if [ -z "$(o_hidrogeno_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa hidrogeno ."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_hidrogeno_files) $(LOADLIBES) $(LDLIBS) -o hidrogeno $(LDADD)

oxigeno: $(o_oxigeno_files)
	@if [ -z "$(o_oxigeno_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa oxigeno ."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_oxigeno_files) $(LOADLIBES) $(LDLIBS) -o oxigeno $(LDADD)

clean:
	@$(RM) -fv *.o launcher terminator hidrogeno oxigeno