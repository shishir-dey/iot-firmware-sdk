cmake_minimum_required(VERSION 3.10)

# Project name and version
project(iot-firmware-sdk VERSION 0.1.0 LANGUAGES C)

# Include dependency configurations
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define library and executable output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define SDK source files
set(SDK_SOURCES
    src/data/internet_object.c
    src/connectivity/mqtt_client.c
)

# Define the SDK library
add_library(iot_firmware_sdk STATIC ${SDK_SOURCES})

# SDK include directories
target_include_directories(iot_firmware_sdk PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/portable
)

# Configure external dependencies
configure_coremqtt()
configure_cjson()
configure_mbedtls()

# Install rules for SDK
install(TARGETS iot_firmware_sdk
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)
