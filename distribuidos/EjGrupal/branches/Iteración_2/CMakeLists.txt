cmake_minimum_required (VERSION 2.6.0)
project("Ejercicio 15 - Iteración_2")

set(SRC_DIR src/)
set(OUT_DIR out/)
set(LIB_DIR lib/)

file(MAKE_DIRECTORY ${OUT_DIR})

set(COMPILE_FLAGS "-g -Wall -pedantic")
set(LDLIBS "-lnsl")



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
include_directories("${SRC_DIR}API/Robot16_Despacho")
include_directories("${SRC_DIR}API/Robot16_Cinta15")
include_directories("${SRC_DIR}API/Despacho")
include_directories("${SRC_DIR}API/Cliente")
include_directories("${SRC_DIR}API/Vendedor")
include_directories("${SRC_DIR}API/AlmacenPiezas")
include_directories("${SRC_DIR}API/AlmacenPiezas/EspacioAlmacenPiezas")
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
include_directories("${SRC_DIR}ConfigFileParser")

include_directories("${SRC_DIR}Socket")
include_directories("${SRC_DIR}Comunicaciones")
include_directories("${SRC_DIR}Comunicaciones/Objects")
include_directories("${SRC_DIR}Comunicaciones/Signals")
include_directories("${SRC_DIR}Comunicaciones/Signals/Handlers")

include_directories("${SRC_DIR}Comunicaciones/Agentes")
include_directories("${SRC_DIR}Comunicaciones/Brokers")
include_directories("${SRC_DIR}Comunicaciones/Canales")



include_directories("${SRC_DIR}Numerador")

aux_source_directory("${SRC_DIR}Numerador"                               NUMERADOR_SRC)
aux_source_directory("${SRC_DIR}Numerador/Ejecutables"                   NUMERADOR_EJ_SRC)


aux_source_directory("${SRC_DIR}API"                                     COMMON_SRC)
aux_source_directory("${SRC_DIR}API/AGV"                                 AGV_SRC)
aux_source_directory("${SRC_DIR}API/Robot5"                              ROBOT5_SRC)
aux_source_directory("${SRC_DIR}API/Robot11"                             ROBOT11_SRC)
aux_source_directory("${SRC_DIR}API/Robot12"                             ROBOT12_SRC)
aux_source_directory("${SRC_DIR}API/Robot14"                             ROBOT14_SRC)
aux_source_directory("${SRC_DIR}API/Robot14/State"                       ROBOT14_SRC)
aux_source_directory("${SRC_DIR}API/Robot16_Despacho"                    ROBOT16_DESPACHO_SRC)               
aux_source_directory("${SRC_DIR}API/Robot16_Cinta15"                     ROBOT16_CINTA15_SRC)               
aux_source_directory("${SRC_DIR}API/Despacho"                            DESPACHO_SRC)
aux_source_directory("${SRC_DIR}API/Cliente"                             CLIENTE_SRC)
aux_source_directory("${SRC_DIR}API/Vendedor"                            VENDEDOR_SRC)
aux_source_directory("${SRC_DIR}API/AlmacenPiezas"                       ALMACEN_SRC)
aux_source_directory("${SRC_DIR}API/AlmacenPiezas/EspacioAlmacenPiezas"  ALMACEN_SRC)
aux_source_directory("${SRC_DIR}API/Repositor"                           REPOSITOR_SRC)

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
aux_source_directory("${SRC_DIR}ConfigFileParser"                        LIBRARY_SRC)

aux_source_directory("${SRC_DIR}Socket"                                  LIBRARY_SRC)
aux_source_directory("${SRC_DIR}Comunicaciones/Objects"                  LIBRARY_SRC)
aux_source_directory("${SRC_DIR}Comunicaciones/Signals"                  LIBRARY_SRC)
aux_source_directory("${SRC_DIR}Comunicaciones/Signals/Handlers"         LIBRARY_SRC)

# Only for QTcreator. With this, QTcreator can recognize headers files. The process is a
# little bit annoying, but works :)
file(GLOB SRC_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")
file(GLOB MSGQUEUE_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/IPCs/IPCTemplate/*.h")
file(GLOB MESSAGE_QUEUE_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/IPCs/IPCAbstractos/MessageQueue/*.h")
file(GLOB COMM_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/Comunicaciones/Objects/*.h")

add_library(IPCLibrary
            SHARED
            ${SRC_HEADERS}
            ${MSGQUEUE_HEADERS}
            ${MESSAGE_QUEUE_HEADERS}
            ${COMM_HEADERS}
            ${LIBRARY_SRC})


set_target_properties(IPCLibrary PROPERTIES 
  LIBRARY_OUTPUT_DIRECTORY "${LIB_DIR}")
          

# Numerador
add_executable(${OUT_DIR}PruebaNumerador                    ${SRC_DIR}Numerador/Ejecutables/numerador_client.c             ${COMMON_SRC} ${NUMERADOR_SRC} )   
add_executable(${OUT_DIR}NumeradorServer                    ${SRC_DIR}Numerador/Ejecutables/numerador_svc.c             ${COMMON_SRC} ${NUMERADOR_SRC})   


add_executable(${OUT_DIR}Launcher                           ${SRC_DIR}Launcher.cpp                              ${COMMON_SRC})
add_executable(${OUT_DIR}LauncherClientes                   ${SRC_DIR}LauncherClientes.cpp                      ${COMMON_SRC})
add_executable(${OUT_DIR}LauncherNumerador                  ${SRC_DIR}LauncherNumerador.cpp                     ${COMMON_SRC})
add_executable(${OUT_DIR}LauncherDespacho                   ${SRC_DIR}LauncherDespacho.cpp                      ${COMMON_SRC})
add_executable(${OUT_DIR}Terminator                         ${SRC_DIR}Terminator.cpp                            ${COMMON_SRC})
add_executable(${OUT_DIR}TerminatorClientes                 ${SRC_DIR}TerminatorClientes.cpp                    ${COMMON_SRC})
add_executable(${OUT_DIR}TerminatorNumerador                ${SRC_DIR}TerminatorNumerador.cpp                   ${COMMON_SRC})
add_executable(${OUT_DIR}TerminatorDespacho                 ${SRC_DIR}TerminatorDespacho.cpp                    ${COMMON_SRC})

add_executable(${OUT_DIR}AGV                                ${SRC_DIR}AGV.cpp                                   ${COMMON_SRC} ${AGV_SRC})
add_executable(${OUT_DIR}Robot5-Agv                         ${SRC_DIR}Robot5-Agv.cpp                            ${COMMON_SRC} ${ROBOT5_SRC} ${ALMACEN_SRC})
add_executable(${OUT_DIR}Robot5-Cinta                       ${SRC_DIR}Robot5-Cinta.cpp                          ${COMMON_SRC} ${ROBOT5_SRC} ${ALMACEN_SRC})

add_executable(${OUT_DIR}Robot11                            ${SRC_DIR}Robot11.cpp                               ${COMMON_SRC} ${ROBOT11_SRC})
add_executable(${OUT_DIR}Robot12                            ${SRC_DIR}Robot12.cpp                               ${COMMON_SRC} ${ROBOT12_SRC})
add_executable(${OUT_DIR}Robot14                            ${SRC_DIR}Robot14.cpp                               ${COMMON_SRC} ${ROBOT14_SRC})
add_executable(${OUT_DIR}Robot16_Despacho                   ${SRC_DIR}Robot16_Despacho.cpp                      ${COMMON_SRC} ${ROBOT16_DESPACHO_SRC})
add_executable(${OUT_DIR}Robot16_Cinta15                    ${SRC_DIR}Robot16_Cinta15.cpp                       ${COMMON_SRC} ${ROBOT16_CINTA15_SRC})
add_executable(${OUT_DIR}Despacho                           ${SRC_DIR}Despacho.cpp                              ${COMMON_SRC} ${DESPACHO_SRC})
add_executable(${OUT_DIR}Cliente                            ${SRC_DIR}Cliente.cpp                               ${COMMON_SRC} ${CLIENTE_SRC} ${NUMERADOR_SRC})
add_executable(${OUT_DIR}Vendedor                           ${SRC_DIR}Vendedor.cpp                              ${COMMON_SRC} ${VENDEDOR_SRC} ${NUMERADOR_SRC})
add_executable(${OUT_DIR}AlmacenPiezas                      ${SRC_DIR}AlmacenPiezas.cpp                         ${COMMON_SRC} ${ALMACEN_SRC})
add_executable(${OUT_DIR}RepositorCanastos                  ${SRC_DIR}RepositorCanastos.cpp                     ${COMMON_SRC} ${REPOSITOR_SRC} ${ALMACEN_SRC})
add_executable(${OUT_DIR}RepositorGabinetes                 ${SRC_DIR}RepositorGabinetes.cpp                    ${COMMON_SRC} ${REPOSITOR_SRC} ${ALMACEN_SRC})
add_executable(${OUT_DIR}MonitorAlmacenProductosTerminados  ${SRC_DIR}MonitorAlmacenProductosTerminados.cpp     ${COMMON_SRC} ${VENDEDOR_SRC})
#add_executable(${OUT_DIR}MonitorAlmacenPiezas              ${SRC_DIR}MonitorAlmacenPiezas.cpp                  ${COMMON_SRC} ${VENDEDOR_SRC})

add_executable(${OUT_DIR}ConcurrentServer                   ${SRC_DIR}Comunicaciones/ConcurrentServer.cpp          ${COMMON_SRC})

# Broker - Agente
add_executable(${OUT_DIR}LauncherAgentes                    ${SRC_DIR}Comunicaciones/Agentes/LauncherAgentes.cpp     ${COMMON_SRC})
add_executable(${OUT_DIR}TerminatorAgentes                  ${SRC_DIR}Comunicaciones/Agentes/TerminatorAgentes.cpp   ${COMMON_SRC})
add_executable(${OUT_DIR}CanalSalidaAgente                  ${SRC_DIR}Comunicaciones/Canales/CanalSalidaAgente.cpp   ${COMMON_SRC})
add_executable(${OUT_DIR}CanalEntradaAgente                 ${SRC_DIR}Comunicaciones/Canales/CanalEntradaAgente.cpp  ${COMMON_SRC})
add_executable(${OUT_DIR}CanalSalidaBroker                  ${SRC_DIR}Comunicaciones/Canales/CanalSalidaBroker.cpp   ${COMMON_SRC})
add_executable(${OUT_DIR}CanalEntradaBroker                 ${SRC_DIR}Comunicaciones/Canales/CanalEntradaBroker.cpp  ${COMMON_SRC})
add_executable(${OUT_DIR}LauncherBrokers                    ${SRC_DIR}Comunicaciones/Brokers/LauncherBrokers.cpp     ${COMMON_SRC})
add_executable(${OUT_DIR}TerminatorBrokers                  ${SRC_DIR}Comunicaciones/Brokers/TerminatorBrokers.cpp   ${COMMON_SRC})
add_executable(${OUT_DIR}Broker                             ${SRC_DIR}Comunicaciones/Brokers/Broker.cpp              ${COMMON_SRC})




target_link_libraries(${OUT_DIR}Launcher                                    IPCLibrary)
target_link_libraries(${OUT_DIR}LauncherClientes                            IPCLibrary)
target_link_libraries(${OUT_DIR}LauncherNumerador                           IPCLibrary)
target_link_libraries(${OUT_DIR}LauncherDespacho                            IPCLibrary)

target_link_libraries(${OUT_DIR}Terminator                                  IPCLibrary)
target_link_libraries(${OUT_DIR}TerminatorClientes                          IPCLibrary)
target_link_libraries(${OUT_DIR}TerminatorNumerador                         IPCLibrary)
target_link_libraries(${OUT_DIR}TerminatorDespacho                          IPCLibrary)

target_link_libraries(${OUT_DIR}AGV                                         IPCLibrary)
target_link_libraries(${OUT_DIR}Robot5-Agv                                  IPCLibrary)
target_link_libraries(${OUT_DIR}Robot5-Cinta                                IPCLibrary)
target_link_libraries(${OUT_DIR}Robot11                                     IPCLibrary)
target_link_libraries(${OUT_DIR}Robot12                                     IPCLibrary)
target_link_libraries(${OUT_DIR}Robot14                                     IPCLibrary)
target_link_libraries(${OUT_DIR}Robot16_Despacho                            IPCLibrary)
target_link_libraries(${OUT_DIR}Robot16_Cinta15                             IPCLibrary)
target_link_libraries(${OUT_DIR}Despacho                                    IPCLibrary)
target_link_libraries(${OUT_DIR}Cliente                                     IPCLibrary)
target_link_libraries(${OUT_DIR}Vendedor                                    IPCLibrary)
target_link_libraries(${OUT_DIR}AlmacenPiezas                               IPCLibrary)
#target_link_libraries(${OUT_DIR}MonitorAlmacenPiezas                       IPCLibrary)
target_link_libraries(${OUT_DIR}MonitorAlmacenProductosTerminados           IPCLibrary)
target_link_libraries(${OUT_DIR}RepositorCanastos                           IPCLibrary)
target_link_libraries(${OUT_DIR}RepositorGabinetes                          IPCLibrary)

target_link_libraries(${OUT_DIR}ConcurrentServer                            IPCLibrary)
target_link_libraries(${OUT_DIR}LauncherAgentes                             IPCLibrary)
target_link_libraries(${OUT_DIR}TerminatorAgentes                           IPCLibrary)
target_link_libraries(${OUT_DIR}CanalSalidaAgente                           IPCLibrary)
target_link_libraries(${OUT_DIR}CanalEntradaAgente                          IPCLibrary)
target_link_libraries(${OUT_DIR}CanalSalidaBroker                           IPCLibrary)
target_link_libraries(${OUT_DIR}CanalEntradaBroker                          IPCLibrary)
target_link_libraries(${OUT_DIR}LauncherBrokers                             IPCLibrary)
target_link_libraries(${OUT_DIR}TerminatorBrokers                           IPCLibrary)
target_link_libraries(${OUT_DIR}Broker                                      IPCLibrary)
