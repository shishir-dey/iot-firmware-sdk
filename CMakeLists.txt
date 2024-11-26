cmake_minimum_required(VERSION 3.10)

# Project name and version
project(iot-firmware-sdk VERSION 0.1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define library and executable output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define the SDK library
add_library(iot_firmware_sdk STATIC
    src/data/internet_object.c
    src/connectivity/mqtt_client.c
    src/connectivity/mqtt_client_port.c
)

# iot-firmware-sdk include directories
target_include_directories(iot_firmware_sdk PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/portable
)

# CoreMQTT
set(CORE_MQTT_SOURCES
    external/coreMQTT/source/core_mqtt.c
    external/coreMQTT/source/core_mqtt_state.c
    external/coreMQTT/source/core_mqtt_serializer.c
)
target_include_directories(iot_firmware_sdk PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/coreMQTT/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/coreMQTT/source/interface
)
target_sources(iot_firmware_sdk PRIVATE ${CORE_MQTT_SOURCES})

# cJSON configuration
set(ENABLE_CJSON_TEST OFF CACHE BOOL "Disable building cJSON test")
add_subdirectory(external/cJSON)
target_include_directories(iot_firmware_sdk PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/cJSON
)
set_target_properties(cjson PROPERTIES COMPILE_FLAGS "-Wno-error")
set_target_properties(cjson PROPERTIES COMPILE_FLAGS "-Wno-float-conversion")

# mbedTLS configuration
set(ENABLE_PROGRAMS OFF CACHE BOOL "Build mbed TLS programs.")
set(ENABLE_TESTING OFF CACHE BOOL "Build mbed TLS tests.")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build mbed TLS shared library.")
add_subdirectory(external/mbedtls)
target_link_libraries(iot_firmware_sdk mbedtls mbedx509 mbedcrypto)

# Install rules for SDK and example
install(TARGETS iot_firmware_sdk
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# Build the examples if host is Windows
# if(WIN32)
#     set(PLATFORM_SOURCES
#         platform/Win32/platform_win32_port_clock.c
#         platform/Win32/platform_win32_port_transport.c
#         external/cJSON/cJSON.c
#     )
    
#     target_include_directories(iot_firmware_sdk PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}/platform/Win32
#         ${CMAKE_CURRENT_SOURCE_DIR}/portable
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/iot
#         ${CMAKE_CURRENT_SOURCE_DIR}/external/cJSON
#     )

#     add_executable(stream_publish
#         examples/stream_publish.c
#         ${PLATFORM_SOURCES}
#     )

#     target_link_libraries(stream_publish iot_firmware_sdk)

#     set_target_properties(stream_publish PROPERTIES
#         RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
#     )

#     install(TARGETS stream_publish
#         RUNTIME DESTINATION bin
#     )

# else()
#     set(PLATFORM_SOURCES "")
# endif()
