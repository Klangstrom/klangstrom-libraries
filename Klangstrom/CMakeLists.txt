cmake_minimum_required(VERSION 3.12)

project(klangstrom)

if(NOT DEFINED KLST_ENV)
    message(FATAL_ERROR "KLST_ENV is not defined")
endif()

add_library(klangstrom 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-ADCDAC.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-Backlight.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-ExternalMemory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-IDC_Serial.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-InternalMemory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-LTDC.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-MIDI_analog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-OnBoardMic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-Touch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Klangstrom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromAudioCodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromLEDs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromMechanicalKey.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KlangstromSerialDebug.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KLST_PANDA-RotaryEncoder.cpp
)

target_include_directories(klangstrom PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(klangstrom PRIVATE KLST_ENV=${KLST_ENV})
#target_link_libraries(klangstrom PRIVATE klangstrom_KLST_EMU)
