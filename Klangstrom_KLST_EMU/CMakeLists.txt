cmake_minimum_required(VERSION 3.12)

project(klangstrom_KLST_EMU)

if(NOT DEFINED KLST_ENV)
    message(FATAL_ERROR "KLST_ENV is not defined")
endif()

add_library(klangstrom_KLST_EMU 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Klangstrom_BSP_KLST_EMU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromAudioCodec_BSP_KLST_EMU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromLEDs_BSP_KLST_EMU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromSerialDebug_BSP_KLST_EMU.cpp
)

target_include_directories(klangstrom_KLST_EMU PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(klangstrom_KLST_EMU PRIVATE KLST_ENV=${KLST_ENV})
target_link_libraries(klangstrom_KLST_EMU PRIVATE klangstrom)
target_link_libraries(klangstrom_KLST_EMU PRIVATE klangstrom-emulator)
target_link_libraries(klangstrom_KLST_EMU PRIVATE umgebung-lib)
