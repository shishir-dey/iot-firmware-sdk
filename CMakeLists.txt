cmake_minimum_required(VERSION 3.10)

# Project name and version
project(bytebeam-firmware-sdk VERSION 0.1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories for external libraries
include_directories(include external/cJSON)

# Add external library subdirectories
add_subdirectory(external/cJSON)
add_subdirectory(external/mbedtls)

# CoreMQTT sources (manual addition due to lack of CMakeLists.txt)
set(CORE_MQTT_SOURCES
    external/coreMQTT/source/core_mqtt.c
    external/coreMQTT/source/core_mqtt_state.c
    external/coreMQTT/source/core_mqtt_serializer.c
)

# Create a static library
add_library(bytebeam_firmware_sdk STATIC
    src/stream.c
    src/mqtt_client.c
    ${CORE_MQTT_SOURCES}  # Include CoreMQTT sources
)

# Add config directory to include path
target_include_directories(bytebeam_firmware_sdk PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/config
)

# Add CoreMQTT include directories
target_include_directories(bytebeam_firmware_sdk PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/coreMQTT/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/coreMQTT/source/interface
)

# Link against external libraries
target_link_libraries(bytebeam_firmware_sdk PUBLIC 
    cJSON 
    mbedtls
)

# Remove -Werror flag from cJSON compilation
set_target_properties(cjson PROPERTIES COMPILE_FLAGS "-Wno-error")

# Set output name to ensure .a extension
set_target_properties(bytebeam_firmware_sdk PROPERTIES OUTPUT_NAME "bytebeam_firmware_sdk")

# Install rules
install(TARGETS bytebeam_firmware_sdk
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)
