cmake_minimum_required (VERSION 2.6)
project("Ej Grupal")

set(SRC_DIR src/)
set(OUT_DIR bin/)

file(MAKE_DIRECTORY ${OUT_DIR})

set(COMPILE_FLAGS "-g")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}CintaTransportadora")
include_directories("${SRC_DIR}IPC/Semaphore")
include_directories("${SRC_DIR}IPC/SharedMemory")
include_directories("${SRC_DIR}IPC/MessageQueue")
include_directories("${SRC_DIR}Controladores")

aux_source_directory ("${SRC_DIR}IPC/"		 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/Semaphore" 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/MessageQueue" 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}IPC/SharedMemory" 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}Controladores" 	COMMON_SRC)
aux_source_directory ("${SRC_DIR}CintaTransportadora" 	COMMON_SRC)

add_executable(${OUT_DIR}robot5                   ${SRC_DIR}Robot5.cpp                  ${COMMON_SRC})
add_executable(${OUT_DIR}controladorRobot5Agv     ${SRC_DIR}ControladorRobot5Agv.cpp    ${COMMON_SRC})
add_executable(${OUT_DIR}controladorRobot5Cinta   ${SRC_DIR}ControladorRobot5Cinta.cpp  ${COMMON_SRC})

add_executable(${OUT_DIR}launcherRobot5           ${SRC_DIR}LauncherRobot5.cpp          ${COMMON_SRC})
add_executable(${OUT_DIR}terminatorRobot5         ${SRC_DIR}TerminatorRobot5.cpp        ${COMMON_SRC})

add_executable(${OUT_DIR}pruebaRobot5AGV          ${SRC_DIR}PruebaRobot5AGV.cpp          ${COMMON_SRC})
