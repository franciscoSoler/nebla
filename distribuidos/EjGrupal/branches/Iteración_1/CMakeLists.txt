cmake_minimum_required (VERSION 2.6.0)
project("Ejercicio 15 - Iteración_1")

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

add_library(IPCLibrary
            SHARED
            ${LIBRARY_SRC})


set_target_properties(IPCLibrary PROPERTIES 
  LIBRARY_OUTPUT_DIRECTORY "${LIB_DIR}")
          

# Numerador

add_executable(${OUT_DIR}PruebaNumeradorClientes            ${SRC_DIR}Numerador/Ejecutables/numeradorClientes_client.c             ${COMMON_SRC} ${NUMERADOR_SRC} )   
add_executable(${OUT_DIR}NumeradorClientesServer            ${SRC_DIR}Numerador/Ejecutables/numeradorClientes_svc.c             ${COMMON_SRC} ${NUMERADOR_SRC})   


add_executable(${OUT_DIR}Launcher                           ${SRC_DIR}Launcher.cpp                              ${COMMON_SRC})
add_executable(${OUT_DIR}LauncherClientes                   ${SRC_DIR}LauncherClientes.cpp                      ${COMMON_SRC})
add_executable(${OUT_DIR}LauncherNumeradorClientes          ${SRC_DIR}LauncherNumeradorClientes.cpp             ${COMMON_SRC})   
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
add_executable(${OUT_DIR}Vendedor                           ${SRC_DIR}Vendedor.cpp                              ${COMMON_SRC} ${VENDEDOR_SRC})
add_executable(${OUT_DIR}AlmacenPiezas                      ${SRC_DIR}AlmacenPiezas.cpp                         ${COMMON_SRC} ${ALMACEN_SRC})
add_executable(${OUT_DIR}RepositorCanastos                  ${SRC_DIR}RepositorCanastos.cpp                     ${COMMON_SRC} ${REPOSITOR_SRC} ${ALMACEN_SRC})
add_executable(${OUT_DIR}RepositorGabinetes                 ${SRC_DIR}RepositorGabinetes.cpp                    ${COMMON_SRC} ${REPOSITOR_SRC} ${ALMACEN_SRC})
add_executable(${OUT_DIR}MonitorAlmacenProductosTerminados  ${SRC_DIR}MonitorAlmacenProductosTerminados.cpp     ${COMMON_SRC} ${VENDEDOR_SRC})
#add_executable(${OUT_DIR}MonitorAlmacenPiezas              ${SRC_DIR}MonitorAlmacenPiezas.cpp                  ${COMMON_SRC} ${VENDEDOR_SRC})

# Procesos utilizados por el Middleware para entablar y realizar comunicaciones
# Cliente
add_executable(${OUT_DIR}CanalEntradaCliente                ${SRC_DIR}Comunicaciones/CanalEntradaCliente.cpp       ${COMMON_SRC})
add_executable(${OUT_DIR}CanalSalidaCliente                 ${SRC_DIR}Comunicaciones/CanalSalidaCliente.cpp        ${COMMON_SRC})
add_executable(${OUT_DIR}ClienteCanalConDespacho            ${SRC_DIR}Comunicaciones/ClienteCanalConDespacho.cpp   ${COMMON_SRC})
add_executable(${OUT_DIR}ClienteCanalConR16                 ${SRC_DIR}Comunicaciones/ClienteCanalConR16.cpp        ${COMMON_SRC})

# Vendedor
add_executable(${OUT_DIR}CanalEntradaVendedor               ${SRC_DIR}Comunicaciones/CanalEntradaVendedor.cpp      ${COMMON_SRC})
add_executable(${OUT_DIR}CanalSalidaVendedor                ${SRC_DIR}Comunicaciones/CanalSalidaVendedor.cpp       ${COMMON_SRC})
add_executable(${OUT_DIR}VendedorCanalConDespacho           ${SRC_DIR}Comunicaciones/VendedorCanalConDespacho.cpp  ${COMMON_SRC})

# Despacho
add_executable(${OUT_DIR}DespachoCanalConCliente            ${SRC_DIR}Comunicaciones/DespachoCanalConCliente.cpp   ${COMMON_SRC})
add_executable(${OUT_DIR}DespachoCanalConVendedor           ${SRC_DIR}Comunicaciones/DespachoCanalConVendedor.cpp  ${COMMON_SRC})
add_executable(${OUT_DIR}DespachoCanalConR16                ${SRC_DIR}Comunicaciones/DespachoCanalConR16.cpp       ${COMMON_SRC})
add_executable(${OUT_DIR}DespachoCanalConR16_Cinta15        ${SRC_DIR}Comunicaciones/DespachoCanalConR16_Cinta15.cpp ${COMMON_SRC})

# Robot16
add_executable(${OUT_DIR}R16CanalConCliente                 ${SRC_DIR}Comunicaciones/R16CanalConCliente.cpp        ${COMMON_SRC})
add_executable(${OUT_DIR}R16CanalConDespacho                ${SRC_DIR}Comunicaciones/R16CanalConDespacho.cpp       ${COMMON_SRC})
add_executable(${OUT_DIR}R16_Cinta15CanalConDespacho        ${SRC_DIR}Comunicaciones/R16_Cinta15CanalConDespacho.cpp ${COMMON_SRC})


add_executable(${OUT_DIR}CreadorCanalesCliente              ${SRC_DIR}Comunicaciones/CreadorCanalesCliente.cpp     ${COMMON_SRC})
add_executable(${OUT_DIR}ConcurrentServer                   ${SRC_DIR}Comunicaciones/ConcurrentServer.cpp          ${COMMON_SRC})



target_link_libraries(${OUT_DIR}Launcher                                    IPCLibrary)
target_link_libraries(${OUT_DIR}LauncherClientes                            IPCLibrary)
target_link_libraries(${OUT_DIR}LauncherNumeradorClientes                   IPCLibrary)
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

target_link_libraries(${OUT_DIR}CanalEntradaCliente                         IPCLibrary)
target_link_libraries(${OUT_DIR}CanalSalidaCliente                          IPCLibrary)
target_link_libraries(${OUT_DIR}ClienteCanalConDespacho                     IPCLibrary)
target_link_libraries(${OUT_DIR}ClienteCanalConR16                          IPCLibrary)
target_link_libraries(${OUT_DIR}DespachoCanalConCliente                     IPCLibrary)
target_link_libraries(${OUT_DIR}DespachoCanalConVendedor                    IPCLibrary)
target_link_libraries(${OUT_DIR}DespachoCanalConR16                         IPCLibrary)
target_link_libraries(${OUT_DIR}DespachoCanalConR16_Cinta15                 IPCLibrary)
target_link_libraries(${OUT_DIR}R16CanalConCliente                          IPCLibrary)
target_link_libraries(${OUT_DIR}R16CanalConDespacho                         IPCLibrary)
target_link_libraries(${OUT_DIR}R16_Cinta15CanalConDespacho                 IPCLibrary)
target_link_libraries(${OUT_DIR}CanalEntradaVendedor                        IPCLibrary)
target_link_libraries(${OUT_DIR}CanalSalidaVendedor                         IPCLibrary)
target_link_libraries(${OUT_DIR}VendedorCanalConDespacho                    IPCLibrary)
target_link_libraries(${OUT_DIR}CreadorCanalesCliente                       IPCLibrary)
target_link_libraries(${OUT_DIR}ConcurrentServer                            IPCLibrary)
