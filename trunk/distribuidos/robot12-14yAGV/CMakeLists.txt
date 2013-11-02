cmake_minimum_required (VERSION 2.8.7)
project("Ejercicio 7")

set(SRC_DIR src/)
set(OUT_DIR out/)

set(COMPILE_FLAGS "-g")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}API")
include_directories("${SRC_DIR}Controladores")
include_directories("${SRC_DIR}Exceptions")
include_directories("${SRC_DIR}IPC")
include_directories("${SRC_DIR}IPC/IPCObject")
include_directories("${SRC_DIR}IPC/MessageQueue")
include_directories("${SRC_DIR}IPC/Semaphore")
include_directories("${SRC_DIR}IPC/SharedMemory")
include_directories("${SRC_DIR}Logger")

aux_source_directory("${SRC_DIR}API"                    COMMON_SRC)
aux_source_directory("${SRC_DIR}Controladores"          COMMON_SRC)
aux_source_directory("${SRC_DIR}Exceptions"             COMMON_SRC)
aux_source_directory("${SRC_DIR}IPC/IPCObject"          COMMON_SRC)
aux_source_directory("${SRC_DIR}IPC/MessageQueue"       COMMON_SRC)
aux_source_directory("${SRC_DIR}IPC/Semaphore"          COMMON_SRC)
aux_source_directory("${SRC_DIR}IPC/SharedMemory"       COMMON_SRC)
aux_source_directory("${SRC_DIR}Logger"                 COMMON_SRC)


add_executable(${OUT_DIR}Robot11            ${SRC_DIR}Robot11.cpp           ${COMMON_SRC})
add_executable(${OUT_DIR}Robot12            ${SRC_DIR}Robot12.cpp           ${COMMON_SRC})
add_executable(${OUT_DIR}AGV                ${SRC_DIR}AGV.cpp               ${COMMON_SRC})
add_executable(${OUT_DIR}Terminator         ${SRC_DIR}Terminator.cpp        ${COMMON_SRC})
add_executable(${OUT_DIR}Launcher           ${SRC_DIR}Launcher.cpp          ${COMMON_SRC})
add_executable(${OUT_DIR}MockRobot5Cinta    ${SRC_DIR}MockRobot5Cinta.cpp   ${COMMON_SRC})
add_executable(${OUT_DIR}MockRobot5AGV      ${SRC_DIR}MockRobot5AGV.cpp     ${COMMON_SRC})


