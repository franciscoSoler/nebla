cmake_minimum_required (VERSION 2.6)
project("Ejercicio 7")

set(SRC_DIR src/)
set(OUT_DIR out/)

set(COMPILE_FLAGS "-g")
add_definitions(${COMPILE_FLAGS})

include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}CSO")
include_directories("${SRC_DIR}CSO/IPCs")
include_directories("${SRC_DIR}CSO/Logger")
include_directories("${SRC_DIR}Objects")
include_directories("${SRC_DIR}CSO/ConfigFileParser")
include_directories("${SRC_DIR}CSO/Exceptions")
include_directories("${SRC_DIR}API")
include_directories("${SRC_DIR}API/Robot11")
include_directories("${SRC_DIR}API/Robot14")
include_directories("${SRC_DIR}API/Robot14/State")
include_directories("${SRC_DIR}API/Robot16")
include_directories("${SRC_DIR}API/Robot16/State")

aux_source_directory("${SRC_DIR}CSO/IPCs"               COMMON_SRC)
aux_source_directory("${SRC_DIR}CSO/Logger"             COMMON_SRC)
aux_source_directory("${SRC_DIR}Objects"                COMMON_SRC)
aux_source_directory("${SRC_DIR}CSO/ConfigFileParser"   COMMON_SRC)
aux_source_directory("${SRC_DIR}CSO/Exceptions"         COMMON_SRC)
aux_source_directory("${SRC_DIR}API/Robot11"            COMMON_SRC)
aux_source_directory("${SRC_DIR}API/Robot14"            COMMON_SRC)
aux_source_directory("${SRC_DIR}API/Robot14/State"      COMMON_SRC)
aux_source_directory("${SRC_DIR}API/Robot16"            COMMON_SRC)
aux_source_directory("${SRC_DIR}API/Robot16/State"      COMMON_SRC)


add_executable(${OUT_DIR}Robot11        ${SRC_DIR}Robot11.cpp      ${COMMON_SRC})
add_executable(${OUT_DIR}Robot14        ${SRC_DIR}Robot14.cpp      ${COMMON_SRC})
add_executable(${OUT_DIR}Robot16        ${SRC_DIR}Robot16.cpp      ${COMMON_SRC})
add_executable(${OUT_DIR}Terminator     ${SRC_DIR}Terminator.cpp   ${COMMON_SRC})
add_executable(${OUT_DIR}Launcher	    ${SRC_DIR}Launcher.cpp     ${COMMON_SRC})


