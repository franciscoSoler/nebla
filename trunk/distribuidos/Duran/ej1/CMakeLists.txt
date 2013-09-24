cmake_minimum_required (VERSION 2.6)
project("Ej 1")

set(SRC_DIR src/)
set(OUT_DIR src/)

set(COMPILE_FLAGS "-g")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}IPC/Semaphore")
include_directories("${SRC_DIR}IPC/SharedMemory")
include_directories("${SRC_DIR}Controladores")

aux_source_directory ("${SRC_DIR}IPC/"		 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/Semaphore" 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/SharedMemory"	COMMON_SRC)
aux_source_directory ("${SRC_DIR}Controladores" 	COMMON_SRC)

add_executable(${OUT_DIR}terminator	 			${SRC_DIR}Terminator.cpp 			${COMMON_SRC})
add_executable(${OUT_DIR}launcher				${SRC_DIR}Launcher.cpp				${COMMON_SRC})
add_executable(${OUT_DIR}administrador			${SRC_DIR}Administrador.cpp			${COMMON_SRC})
add_executable(${OUT_DIR}puertaEntrada			${SRC_DIR}PuertaEntrada.cpp			${COMMON_SRC})
add_executable(${OUT_DIR}puertaSalida			${SRC_DIR}puertaSalida.cpp			${COMMON_SRC})
