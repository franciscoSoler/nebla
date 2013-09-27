cmake_minimum_required (VERSION 2.6)
project("Ej 5")

set(SRC_DIR src/)
set(OUT_DIR bin/)

file(MAKE_DIRECTORY ${OUT_DIR})

set(COMPILE_FLAGS "-g")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}IPC/MessageQueue")
include_directories("${SRC_DIR}IPC/Semaphore")
include_directories("${SRC_DIR}IPC/SharedMemory")
include_directories("${SRC_DIR}Controladores")

aux_source_directory ("${SRC_DIR}IPC/"		 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/MessageQueue" 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/Semaphore" 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/SharedMemory"	COMMON_SRC)
aux_source_directory ("${SRC_DIR}Controladores" 	COMMON_SRC)

add_executable(${OUT_DIR}terminator	 			${SRC_DIR}Terminator.cpp 			${COMMON_SRC})
add_executable(${OUT_DIR}launcher				${SRC_DIR}Launcher.cpp				${COMMON_SRC})
add_executable(${OUT_DIR}productor				${SRC_DIR}ProductorPersonas.cpp			${COMMON_SRC})
add_executable(${OUT_DIR}puerta					${SRC_DIR}Puerta.cpp				${COMMON_SRC})
add_executable(${OUT_DIR}bus					${SRC_DIR}Bus.cpp				${COMMON_SRC})
