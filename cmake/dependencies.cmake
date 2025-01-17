include(FetchContent)
include(CheckIPOSupported)

# Common configuration options
set(BUILD_SHARED_LIBS
    OFF
    CACHE BOOL "Build all libraries as static by default")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Function to ensure Git submodules are initialized
function(initialize_submodules)
  find_package(Git QUIET REQUIRED)
  message(STATUS "Updating Git submodules...")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMOD_RESULT)
  if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(
      FATAL_ERROR "Failed to update Git submodules. Error: ${GIT_SUBMOD_RESULT}"
    )
  endif()
endfunction()

# CoreMQTT Configuration
function(configure_coremqtt)
  # Ensure iot_firmware_sdk is defined before proceeding
  if(NOT TARGET iot_firmware_sdk)
    add_library(iot_firmware_sdk STATIC)
  endif()

  set(CORE_MQTT_SOURCES
      external/coreMQTT/source/core_mqtt.c
      external/coreMQTT/source/core_mqtt_state.c
      external/coreMQTT/source/core_mqtt_serializer.c)
  target_include_directories(
    iot_firmware_sdk
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/coreMQTT/source/include
           ${CMAKE_CURRENT_SOURCE_DIR}/external/coreMQTT/source/interface)
  target_sources(iot_firmware_sdk PRIVATE ${CORE_MQTT_SOURCES})
  set(COREMQTT_INCLUDE_DIRS
      ${CMAKE_CURRENT_SOURCE_DIR}/external/coreMQTT/source/include
      ${CMAKE_CURRENT_SOURCE_DIR}/external/coreMQTT/source/interface
      PARENT_SCOPE)
endfunction()

# coreHTTP Configuration
function(configure_corehttp)
  # Configure llhttp first
  if(NOT TARGET llhttp_static)
    set(LLHTTP_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/external/coreHTTP/source/dependency/3rdparty/llhttp
    )
    if(NOT EXISTS "${LLHTTP_DIR}/CMakeLists.txt")
      message(
        FATAL_ERROR "llhttp submodule not found. Please update submodules.")
    endif()

    set(LLHTTP_SOURCES ${LLHTTP_DIR}/src/llhttp.c
                       # Add other necessary llhttp source files here
    )
    add_library(llhttp_static STATIC ${LLHTTP_SOURCES})

    set(LLHTTP_STATIC_LIB
        ON
        CACHE BOOL "Build llhttp as static library")
    set(LLHTTP_SHARED_LIB
        OFF
        CACHE BOOL "Build llhttp as shared library")
  endif()

  # Ensure llhttp_static is built before linking
  if(TARGET llhttp_static)
    message(STATUS "Linking with llhttp_static")
  else()
    message(
      FATAL_ERROR
        "llhttp_static target not found. Ensure it is built correctly.")
  endif()

  # Manually add CoreHTTP source files
  set(CORE_HTTP_SOURCES external/coreHTTP/source/core_http_client.c
                        # Add other necessary source files here
  )

  # Add CoreHTTP as a library target if not already defined
  if(NOT TARGET core_http)
    add_library(core_http STATIC ${CORE_HTTP_SOURCES})

    # Add include directories
    target_include_directories(
      core_http
      PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/coreHTTP/source/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/coreHTTP/source/interface>
        $<BUILD_INTERFACE:${LLHTTP_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/config>)
  endif()

  # Link CoreHTTP and llhttp to main target
  target_link_libraries(iot_firmware_sdk PUBLIC core_http llhttp_static)
  set(COREHTTP_INCLUDE_DIRS
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/coreHTTP/source/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/coreHTTP/source/interface>
      $<BUILD_INTERFACE:${LLHTTP_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/config>
      PARENT_SCOPE)
endfunction()

# cJSON Configuration
function(configure_cjson)
  if(NOT TARGET cjson)
    # Configure cJSON build options
    set(ENABLE_CJSON_TEST
        OFF
        CACHE BOOL "Disable building cJSON test")
    set(ENABLE_CJSON_UTILS
        OFF
        CACHE BOOL "Disable building cJSON utils")
    set(ENABLE_TARGET_EXPORT
        OFF
        CACHE BOOL "Disable cJSON cmake export")

    # Add cJSON using its CMake configuration
    add_subdirectory(external/cJSON EXCLUDE_FROM_ALL)
  endif()

  # Configure cJSON target properties
  set_target_properties(
    cjson PROPERTIES COMPILE_FLAGS "-Wno-error -Wno-float-conversion"
                     POSITION_INDEPENDENT_CODE ON)

  # Ensure iot_firmware_sdk is defined before linking libraries
  if(NOT TARGET iot_firmware_sdk)
    add_library(iot_firmware_sdk STATIC)
  endif()

  # Add cJSON include directories
  target_include_directories(iot_firmware_sdk
                             PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/cJSON)

  # Link cJSON to main target
  target_link_libraries(iot_firmware_sdk PUBLIC cjson)
  set(CJSON_INCLUDE_DIRS
      ${CMAKE_CURRENT_SOURCE_DIR}/external/cJSON
      PARENT_SCOPE)
endfunction()

# mbedTLS Configuration
function(configure_mbedtls)
  if(NOT TARGET mbedtls)
    # Configure mbedTLS build options
    set(ENABLE_PROGRAMS
        OFF
        CACHE BOOL "Build mbed TLS programs.")
    set(ENABLE_TESTING
        OFF
        CACHE BOOL "Build mbed TLS tests.")

    # Add mbedTLS using its CMake configuration
    add_subdirectory(external/mbedtls EXCLUDE_FROM_ALL)
  endif()

  # Configure mbedTLS components
  foreach(COMPONENT mbedtls mbedx509 mbedcrypto)
    set_target_properties(${COMPONENT} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endforeach()

  # Link mbedTLS to main target
  target_link_libraries(iot_firmware_sdk PUBLIC mbedtls mbedx509 mbedcrypto)
  set(MBEDTLS_INCLUDE_DIRS
      ${CMAKE_CURRENT_SOURCE_DIR}/external/mbedtls/include
      PARENT_SCOPE)
endfunction()
