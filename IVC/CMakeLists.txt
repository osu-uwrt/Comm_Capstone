# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include_directories(include)

project(IVC C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# add_library(dsp src/tools/dsp.c)
# target_link_libraries(dsp -lm)
# add_library(communication src/communication/receiver.c src/communication/message.c)
# target_link_libraries(communication dsp hardware_adc hardware_dma)

# Add executable. Default name is the project name, version 0.1

add_executable(IVC IVC.c src/tools/dsp.c src/communication/message.c src/communication/receiver.c src/communication/transmittor.c)

pico_set_program_name(IVC "IVC")
pico_set_program_version(IVC "0.1")

# Add the standard library to the build
target_link_libraries(IVC
        pico_stdlib
        hardware_adc
        hardware_pwm
        hardware_dma
        -lm)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(IVC 0)
pico_enable_stdio_usb(IVC 1)

# Add the standard include files to the build
target_include_directories(IVC PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(IVC)

