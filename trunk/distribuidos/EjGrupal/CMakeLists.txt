cmake_minimum_required (VERSION 2.8.7)
project("Ejercicio 7")

set(SRC_DIR src/)
set(OUT_DIR out/)

set(COMPILE_FLAGS "-g")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")

include_directories("${SRC_DIR}API")
include_directories("${SRC_DIR}API/AGV")
include_directories("${SRC_DIR}API/Robot5")
include_directories("${SRC_DIR}API/Robot11")

include_directories("${SRC_DIR}Exceptions")

include_directories("${SRC_DIR}IPCs")
include_directories("${SRC_DIR}IPCs/IPCObject")
include_directories("${SRC_DIR}IPCs/IPCAbstracto")
include_directories("${SRC_DIR}IPCs/IPCAbstracto/AbstractMessageQueue")
include_directories("${SRC_DIR}IPCs/IPCAbstracto/MessageQueue")
include_directories("${SRC_DIR}IPCs/IPCAbstracto/AbstractSharedMemory")
include_directories("${SRC_DIR}IPC/IPCAbstracto/SharedMemory")
include_directories("${SRC_DIR}IPC/Semaphore")

include_directories("${SRC_DIR}Logger")

aux_source_directory("${SRC_DIR}API"                                     COMMON_SRC)
aux_source_directory("${SRC_DIR}API/AGV"                                 COMMON_SRC)
aux_source_directory("${SRC_DIR}API/Robot5"                              COMMON_SRC)
aux_source_directory("${SRC_DIR}API/Robot11"                             COMMON_SRC)
aux_source_directory("${SRC_DIR}Exceptions"                              COMMON_SRC)
aux_source_directory("${SRC_DIR}IPCs"                                    COMMON_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCObject"                          COMMON_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCAbstracto"                       COMMON_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCAbstracto/AbstractMessageQueue"  COMMON_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCAbstracto/MessageQueue"          COMMON_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCAbstracto/AbstractSharedMemory"  COMMON_SRC)
aux_source_directory("${SRC_DIR}IPC/IPCAbstracto/SharedMemory"           COMMON_SRC)
aux_source_directory("${SRC_DIR}IPC/Semaphore"                           COMMON_SRC)
aux_source_directory("${SRC_DIR}Logger"                                  COMMON_SRC)


add_executable(${OUT_DIR}AGV                    ${SRC_DIR}AGV.cpp                    ${COMMON_SRC})
add_executable(${OUT_DIR}Terminator             ${SRC_DIR}Terminator.cpp             ${COMMON_SRC})
add_executable(${OUT_DIR}Launcher               ${SRC_DIR}Launcher.cpp               ${COMMON_SRC})
add_executable(${OUT_DIR}Robot5		        ${SRC_DIR}Robot5.cpp   	             ${COMMON_SRC})
add_executable(${OUT_DIR}ControladorRobot5Agv   ${SRC_DIR}ControladorRobot5Agv.cpp   ${COMMON_SRC})
add_executable(${OUT_DIR}ControladorRobot5Cinta ${SRC_DIR}ControladorRobot5Cinta.cpp ${COMMON_SRC})