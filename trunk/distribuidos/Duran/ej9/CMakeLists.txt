cmake_minimum_required (VERSION 2.6)
project("Ej 3")

set(SRC_DIR src/)
# No puedo hacer que los ejecutables funcionen en otro directorio que no
# sea el src. Ver bien el tema de paths relativos 
set(OUT_DIR src/)

set(COMPILE_FLAGS "-g")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}IPC/Semaphore")
include_directories("${SRC_DIR}IPC/SharedMemory")

aux_source_directory ("${SRC_DIR}IPC/"		 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/Semaphore" 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/SharedMemory"	COMMON_SRC)

add_executable(${OUT_DIR}terminator	 			${SRC_DIR}Terminator.cpp 			${COMMON_SRC})
add_executable(${OUT_DIR}launcher				${SRC_DIR}Launcher.cpp				${COMMON_SRC})
add_executable(${OUT_DIR}procesador				${SRC_DIR}Procesador.cpp				${COMMON_SRC})
