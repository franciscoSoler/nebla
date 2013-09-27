cmake_minimum_required (VERSION 2.6)
project("Ej 8")

set(SRC_DIR src/)
set(OUT_DIR bin/)

file(MAKE_DIRECTORY ${OUT_DIR})

set(COMPILE_FLAGS "-g")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}IPC/Semaphore")

aux_source_directory ("${SRC_DIR}IPC/"		 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/Semaphore" 	COMMON_SRC)

add_executable(${OUT_DIR}terminator	 			${SRC_DIR}Terminator.cpp 			${COMMON_SRC})
add_executable(${OUT_DIR}launcher				${SRC_DIR}Launcher.cpp				${COMMON_SRC})

add_executable(${OUT_DIR}etiquetadora				${SRC_DIR}Etiquetadora.cpp			${COMMON_SRC})
add_executable(${OUT_DIR}tapadora				${SRC_DIR}Tapadora.cpp				${COMMON_SRC})
add_executable(${OUT_DIR}embotelladora				${SRC_DIR}Embotelladora.cpp			${COMMON_SRC})
add_executable(${OUT_DIR}almacenadora				${SRC_DIR}Almacenadora.cpp			${COMMON_SRC})
