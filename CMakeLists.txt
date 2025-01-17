cmake_minimum_required(VERSION 3.13...3.31.2)

# Project name and version
project(
  iot-firmware-sdk
  VERSION 0.1.0
  DESCRIPTION "Firmware SDK that connects any IoT device to cloud"
  HOMEPAGE_URL "https://github.com/shishir-dey/iot-firmware-sdk"
  LANGUAGES C CXX)

# Include helpful CMake utilities
include(CMakeDependentOption)
include(CheckIPOSupported)
include(GNUInstallDirs)

# Build options
option(IOT_SDK_BUILD_TESTS "Build unit tests" ON)
option(IOT_SDK_BUILD_EXAMPLES "Build example applications" OFF)
option(IOT_SDK_ENABLE_COVERAGE "Enable code coverage reporting" OFF)
option(IOT_SDK_ENABLE_SANITIZERS "Enable sanitizers in debug build" OFF)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build" FORCE)
endif()

# Set compilation flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define common compiler warnings
set(COMMON_WARNINGS -Wall -Wextra -Wpedantic -Werror)

# Platform-specific settings
if(UNIX)
  list(
    APPEND
    COMMON_WARNINGS
    -Wformat=2
    -Wno-unused-parameter
    -Wshadow
    -Wwrite-strings
    -Wstrict-prototypes
    -Wold-style-definition
    -Wredundant-decls
    -Wnested-externs
    -Wmissing-include-dirs)
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include dependency configurations
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)

# Configure external dependencies
configure_coremqtt()
configure_cjson()
configure_mbedtls()
configure_corehttp()

# Include directories for external dependencies
include_directories(${COREMQTT_INCLUDE_DIRS} ${CJSON_INCLUDE_DIRS}
                    ${MBEDTLS_INCLUDE_DIRS} ${COREHTTP_INCLUDE_DIRS})

# Define SDK source files
set(SDK_SOURCES src/data/internet_object.c src/connectivity/mqtts_client.c
                src/connectivity/http_client.c)

# Define the SDK library
add_library(${PROJECT_NAME} STATIC ${SDK_SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Set library properties
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION_MAJOR}
             POSITION_INDEPENDENT_CODE ON
             C_VISIBILITY_PRESET hidden
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN ON)

# Set include directories
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/config)

# Configure compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_WARNINGS})

# Enable LTO for Release builds if supported
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  check_ipo_supported(RESULT LTO_SUPPORTED)
  if(LTO_SUPPORTED)
    set_target_properties(${PROJECT_NAME}
                          PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
endif()

# Configure sanitizers for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND IOT_SDK_ENABLE_SANITIZERS)
  target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined
                                                 -fno-omit-frame-pointer)
  target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
endif()

# Install rules
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.h")

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Testing configuration
if(IOT_SDK_BUILD_TESTS)
  enable_testing()

  # Add Google Test
  set(BUILD_GMOCK
      ON
      CACHE BOOL "Build GMock" FORCE)
  set(INSTALL_GTEST
      OFF
      CACHE BOOL "Install GTest" FORCE)
  set(gtest_force_shared_crt
      ON
      CACHE BOOL "Use shared CRT" FORCE)

  add_subdirectory(external/googletest EXCLUDE_FROM_ALL)

  # Add test directory
  add_subdirectory(tests/unit)
endif()

# Examples configuration
if(IOT_SDK_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
