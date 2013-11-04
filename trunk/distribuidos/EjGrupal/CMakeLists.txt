cmake_minimum_required (VERSION 2.6.0)
project("Ejercicio 7")

set(SRC_DIR src/)
set(OUT_DIR out/)
set(LIB_DIR lib/)

file(MAKE_DIRECTORY ${OUT_DIR})

set(COMPILE_FLAGS "-g -Wall -pedantic")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}API")
include_directories("${SRC_DIR}API/AGV")
include_directories("${SRC_DIR}API/Objects")
include_directories("${SRC_DIR}API/Robot5")
include_directories("${SRC_DIR}API/Robot5/CintaTransportadora")
include_directories("${SRC_DIR}API/Robot11")
include_directories("${SRC_DIR}API/Robot12")
include_directories("${SRC_DIR}API/Robot14")
include_directories("${SRC_DIR}API/Robot14/State")
include_directories("${SRC_DIR}API/Robot16")
include_directories("${SRC_DIR}API/Despacho")
include_directories("${SRC_DIR}API/Cliente")
include_directories("${SRC_DIR}API/Vendedor")
include_directories("${SRC_DIR}API/AlmacenPiezas")
include_directories("${SRC_DIR}Exceptions")
include_directories("${SRC_DIR}IPCs")
include_directories("${SRC_DIR}IPCs/IPCObject")
include_directories("${SRC_DIR}IPCs/IPCAbstractos")
include_directories("${SRC_DIR}IPCs/IPCAbstractos/AbstractMessageQueue")
include_directories("${SRC_DIR}IPCs/IPCAbstractos/MessageQueue")
include_directories("${SRC_DIR}IPCs/IPCAbstractos/AbstractSharedMemory")
include_directories("${SRC_DIR}IPCs/IPCAbstractos/SharedMemory")
include_directories("${SRC_DIR}IPCs/IPCTemplate")
include_directories("${SRC_DIR}IPCs/Barrios")
include_directories("${SRC_DIR}IPCs/Semaphore")
include_directories("${SRC_DIR}Logger")
include_directories("${SRC_DIR}Parser")


aux_source_directory("${SRC_DIR}API"                                     COMMON_SRC)
aux_source_directory("${SRC_DIR}API/AGV"                                 AGV_SRC)
aux_source_directory("${SRC_DIR}API/Robot5"                              ROBOT5_SRC)
aux_source_directory("${SRC_DIR}API/Robot11"                             ROBOT11_SRC)
aux_source_directory("${SRC_DIR}API/Robot12"                             ROBOT12_SRC)
aux_source_directory("${SRC_DIR}API/Robot14"                             ROBOT14_SRC)
aux_source_directory("${SRC_DIR}API/Robot14/State"                       ROBOT14_SRC)
aux_source_directory("${SRC_DIR}API/Robot16"                             ROBOT16_SRC)               
aux_source_directory("${SRC_DIR}API/Despacho"                            DESPACHO_SRC)
aux_source_directory("${SRC_DIR}API/Cliente"                             CLIENTE_SRC)
aux_source_directory("${SRC_DIR}API/Vendedor"                            VENDEDOR_SRC)
aux_source_directory("${SRC_DIR}API/AlmacenPiezas"                       ALMACEN_SRC)

aux_source_directory("${SRC_DIR}API/Objects"                             LIBRARY_SRC)
aux_source_directory("${SRC_DIR}API/Robot5/CintaTransportadora"          LIBRARY_SRC)
aux_source_directory("${SRC_DIR}Exceptions"                              LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs"                                    LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCObject"                          LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCTemplate"                        LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCAbstractos"                      LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCAbstractos/AbstractMessageQueue" LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCAbstractos/MessageQueue"         LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCAbstractos/AbstractSharedMemory" LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs/IPCAbstractos/SharedMemory"         LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs/Semaphore"                          LIBRARY_SRC)
aux_source_directory("${SRC_DIR}IPCs/Barrios"                            LIBRARY_SRC)
aux_source_directory("${SRC_DIR}Logger"                                  LIBRARY_SRC)
aux_source_directory("${SRC_DIR}Parser"                                  LIBRARY_SRC)

add_library(IPCLibrary
            SHARED
#            ${SRC_DIR}/IPCs/IPCObject/IPCObject.cpp
#            ${SRC_DIR}/IPCs/Semaphore/Semaphore.cpp
#            ${SRC_DIR}/IPCs/IPCAbstractos/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.cpp
#            ${SRC_DIR}/IPCs/IPCAbstractos/SharedMemory/BufferCanastosSharedMemory.cpp
#            ${SRC_DIR}/IPCs/IPCAbstractos/SharedMemory/Cinta6SharedMemory.cpp
#            ${SRC_DIR}/IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.cpp
#            ${SRC_DIR}/IPCs/IPCAbstractos/AbstractSharedMemory/AbstractSharedMemory.cpp
#            ${SRC_DIR}/IPCs/IPCAbstractos/AbstractMessageQueue/AbstractMessageQueue.cpp
#            ${SRC_DIR}/IPCs/Barrios/MemoriaCompartida.cpp
#            ${SRC_DIR}/Logger/Logger.cpp
#            ${SRC_DIR}/Parser/Parser.cpp
#            ${SRC_DIR}/Logger/LockFile.cpp
#            ${SRC_DIR}/Exceptions/Exception.cpp
#            ${SRC_DIR}/API/Objects/DataSM_R11_R14.cpp
#            ${SRC_DIR}/API/Objects/DataSM_R14_R16.cpp
#            ${SRC_DIR}/API/Objects/BufferODC.cpp
#            ${SRC_DIR}/API/Objects/Util.cpp
#            ${SRC_DIR}/API/Objects/SmMemAlmacenProductosTerminados.cpp)
            ${LIBRARY_SRC})


set_target_properties(IPCLibrary PROPERTIES 
  LIBRARY_OUTPUT_DIRECTORY "${LIB_DIR}")
             

add_executable(${OUT_DIR}Launcher               ${SRC_DIR}Launcher.cpp                  ${COMMON_SRC})
add_executable(${OUT_DIR}Terminator             ${SRC_DIR}Terminator.cpp                ${COMMON_SRC})
add_executable(${OUT_DIR}AGV                    ${SRC_DIR}AGV.cpp                       ${COMMON_SRC} ${AGV_SRC})
add_executable(${OUT_DIR}Robot5		            ${SRC_DIR}Robot5.cpp                    ${COMMON_SRC} ${ROBOT5_SRC})
add_executable(${OUT_DIR}ControladorRobot5Agv   ${SRC_DIR}ControladorRobot5Agv.cpp      ${COMMON_SRC})
add_executable(${OUT_DIR}ControladorRobot5Cinta	${SRC_DIR}ControladorRobot5Cinta.cpp    ${COMMON_SRC})
add_executable(${OUT_DIR}Robot11		        ${SRC_DIR}Robot11.cpp                   ${COMMON_SRC} ${ROBOT11_SRC})
add_executable(${OUT_DIR}Robot12	        	${SRC_DIR}Robot12.cpp                   ${COMMON_SRC} ${ROBOT12_SRC})
add_executable(${OUT_DIR}Robot14		        ${SRC_DIR}Robot14.cpp                   ${COMMON_SRC} ${ROBOT14_SRC})
add_executable(${OUT_DIR}Robot16		        ${SRC_DIR}Robot16.cpp                   ${COMMON_SRC} ${ROBOT16_SRC})
add_executable(${OUT_DIR}Despacho		        ${SRC_DIR}Despacho.cpp                  ${COMMON_SRC} ${DESPACHO_SRC})
add_executable(${OUT_DIR}Cliente		        ${SRC_DIR}Cliente.cpp                   ${COMMON_SRC} ${CLIENTE_SRC})
add_executable(${OUT_DIR}Vendedor		        ${SRC_DIR}Vendedor.cpp                  ${COMMON_SRC} ${VENDEDOR_SRC})
add_executable(${OUT_DIR}AlmacenPiezas		    ${SRC_DIR}AlmacenPiezas.cpp             ${COMMON_SRC} ${ALMACEN_SRC})

target_link_libraries(${OUT_DIR}Launcher                                    IPCLibrary)
target_link_libraries(${OUT_DIR}Terminator                                  IPCLibrary)
target_link_libraries(${OUT_DIR}AGV                                         IPCLibrary)
target_link_libraries(${OUT_DIR}Robot5                                      IPCLibrary)
target_link_libraries(${OUT_DIR}ControladorRobot5Agv                        IPCLibrary)
target_link_libraries(${OUT_DIR}ControladorRobot5Cinta                      IPCLibrary)
target_link_libraries(${OUT_DIR}Robot11                                     IPCLibrary)
target_link_libraries(${OUT_DIR}Robot12                                     IPCLibrary)
target_link_libraries(${OUT_DIR}Robot14                                     IPCLibrary)
target_link_libraries(${OUT_DIR}Robot16                                     IPCLibrary)
target_link_libraries(${OUT_DIR}Despacho                                    IPCLibrary)
target_link_libraries(${OUT_DIR}Cliente                                     IPCLibrary)
target_link_libraries(${OUT_DIR}Vendedor                                    IPCLibrary)
target_link_libraries(${OUT_DIR}AlmacenPiezas                               IPCLibrary)

