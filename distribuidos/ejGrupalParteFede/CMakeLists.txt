cmake_minimum_required (VERSION 2.6)
project("TP Grupal Fede")

set(SRC_DIR src/)
# No puedo hacer que los ejecutables funcionen en otro directorio que no
# sea el src. Ver bien el tema de paths relativos 
set(OUT_DIR out/)

set(COMPILE_FLAGS "-g -Wall -pedantic")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}Objetos")
include_directories("${SRC_DIR}include")

aux_source_directory ("${SRC_DIR}Objetos" COMMON_SRC)
aux_source_directory ("${SRC_DIR}include" COMMON_SRC)

add_executable(${OUT_DIR}Vendedor				${SRC_DIR}Vendedor.cpp				${COMMON_SRC})
add_executable(${OUT_DIR}AlmacenPiezas				${SRC_DIR}AlmacenPiezas.cpp			${COMMON_SRC})
add_executable(${OUT_DIR}CreadorIPC				${SRC_DIR}CreadorIPC.cpp			${COMMON_SRC})
add_executable(${OUT_DIR}DestructorIPC				${SRC_DIR}DestructorIPC.cpp			${COMMON_SRC})
add_executable(${OUT_DIR}AlmacenProductosTerminados		${SRC_DIR}AlmacenProductosTerminados.cpp	${COMMON_SRC})
add_executable(${OUT_DIR}Cliente				${SRC_DIR}Cliente.cpp				${COMMON_SRC})


