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

fuentes_ipc ?= $(wildcard ./IPC/*/*.$(extension))

fuentes_contEtiquetador ?= $(wildcard ControladorEtiquetador.$(extension)) $(wildcard ControladorConsumidor.$(extension))
fuentes_contLlenador ?= $(wildcard ControladorLlenador.$(extension)) $(wildcard ControladorConsumidor.$(extension))
fuentes_contImpresor ?= $(wildcard ControladorImpresor.$(extension)) $(wildcard ControladorConsumidor.$(extension))

fuentes_contProductor ?= $(wildcard ControladorProductor.$(extension))

fuentes_terminator ?= $(wildcard Terminator.$(extension))
fuentes_launcher ?= $(wildcard Launcher.$(extension))

fuentes_productor ?= $(wildcard Productor.$(extension))

fuentes_llenador ?= $(wildcard Llenador.$(extension))
fuentes_etiquetador ?= $(wildcard Etiquetador.$(extension))
fuentes_impresor ?= $(wildcard Impresor.$(extension))

# REGLAS
#########

.PHONY: all clean

all: launcher terminator productor etiquetador llenador impresor

o_ipc_files = $(patsubst %.$(extension),%.o,$(fuentes_ipc))
o_terminator_files = $(patsubst %.$(extension),%.o,$(fuentes_terminator))  $(o_ipc_files)
o_launcher_files = $(patsubst %.$(extension),%.o,$(fuentes_launcher))  $(o_ipc_files)

o_contLlenador_files = $(patsubst %.$(extension),%.o,$(fuentes_contLlenador)) $(o_ipc_files)
o_contEtiquetador_files = $(patsubst %.$(extension),%.o,$(fuentes_contEtiquetador)) $(o_ipc_files)
o_contImpresor_files = $(patsubst %.$(extension),%.o,$(fuentes_contImpresor)) $(o_ipc_files)

o_contProductor_files = $(patsubst %.$(extension),%.o,$(fuentes_contProductor)) $(o_ipc_files)

o_etiquetador_files = $(patsubst %.$(extension),%.o,$(fuentes_etiquetador)) $(o_contEtiquetador_files)
o_llenador_files = $(patsubst %.$(extension),%.o,$(fuentes_llenador)) $(o_contLlenador_files)
o_impresor_files = $(patsubst %.$(extension),%.o,$(fuentes_impresor)) $(o_contImpresor_files)
o_productor_files = $(patsubst %.$(extension),%.o,$(fuentes_productor)) $(o_contProductor_files)

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
	
etiquetador: $(o_etiquetador_files)
	@if [ -z "$(o_etiquetador_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa etiquetador (archivos etiquetador*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_etiquetador_files) $(LOADLIBES) $(LDLIBS) -o etiquetador $(LDADD)

llenador: $(o_llenador_files)
	@if [ -z "$(o_llenador_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa llenador (archivos llenador*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_llenador_files) $(LOADLIBES) $(LDLIBS) -o llenador $(LDADD)

impresor: $(o_impresor_files)
	@if [ -z "$(o_impresor_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa impresor (archivos impresor*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_impresor_files) $(LOADLIBES) $(LDLIBS) -o impresor $(LDADD)

productor: $(o_productor_files)
	@if [ -z "$(o_productor_files)" ]; \
	then \
		echo "No hay archivos de entrada para el programa productor (archivos productor*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_productor_files) $(LOADLIBES) $(LDLIBS) -o productor $(LDADD)

clean:
	@$(RM) -fv *.o launcher terminator productor etiquetador llenador impresor IPC/*/*.o

