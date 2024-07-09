cmake_minimum_required(VERSION 3.12)

project(Klangstrom_KLST_PANDA_STM32)

if(NOT ${KLST_ENV})
    message(FATAL_ERROR "KLST_ENV is not defined")
endif ()

add_library(Klangstrom_KLST_PANDA_STM32
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Klangstrom_BSP_KLST_PANDA_STM32.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/WM8904.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/FT5206.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromAudioCodec_BSP_KLST_PANDA_STM32.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromLEDs_BSP_KLST_PANDA_STM32.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromSDCard_BSP_KLST_PANDA_STM32.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromSerialDebug_BSP_KLST_PANDA_STM32.cpp
)

target_include_directories(Klangstrom_KLST_PANDA_STM32 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(Klangstrom_KLST_PANDA_STM32 PRIVATE KLST_ENV=${KLST_ENV})
target_link_libraries(Klangstrom_KLST_PANDA_STM32 PRIVATE klangstrom)
