cmake_minimum_required(VERSION 3.10)

project(bytebeam-firmware-sdk VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

# Add source directory
add_subdirectory(src)

# Add external dependencies
add_subdirectory(external/cJSON)

# Create the main static library
add_library(bytebeam_firmware_sdk STATIC
    src/action.c
    src/alert.c
    src/geofence.c
    src/gps.c
    src/metadata.c
    src/ota.c
    src/shadow.c
    src/shell.c
    src/stream.c
)

# Link the library with cJSON
target_link_libraries(bytebeam_firmware_sdk PRIVATE cjson)

# Include directories
target_include_directories(bytebeam_firmware_sdk PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/cJSON
)

# Install rules
install(TARGETS bytebeam_firmware_sdk
    ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/build/lib
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/build/lib
    RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/build/bin
)

install(DIRECTORY src/
    DESTINATION ${CMAKE_BINARY_DIR}/build/include
    FILES_MATCHING PATTERN "*.h"
)

# Enable testing
enable_testing()
# Add test subdirectory if you have tests
# add_subdirectory(tests)

# Set the build directory
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

