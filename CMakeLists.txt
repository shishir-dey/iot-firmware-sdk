cmake_minimum_required(VERSION 3.10)

project(bytebeam-firmware-sdk VERSION 0.1.0 LANGUAGES C)

# Create a static library
add_library(bytebeam_firmware_sdk STATIC
    src/stream.c
)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
target_include_directories(bytebeam_firmware_sdk PUBLIC include)
target_include_directories(bytebeam_firmware_sdk PUBLIC external/cJSON)

# Set output name to ensure .a extension
set_target_properties(bytebeam_firmware_sdk PROPERTIES OUTPUT_NAME "bytebeam_firmware_sdk")

# Install rules
install(TARGETS bytebeam_firmware_sdk
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

