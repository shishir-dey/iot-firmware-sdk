# CoreMQTT Configuration
function(configure_coremqtt)
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
endfunction()

# cJSON Configuration
function(configure_cjson)
    set(ENABLE_CJSON_TEST OFF CACHE BOOL "Disable building cJSON test")
    add_subdirectory(external/cJSON)
    target_include_directories(iot_firmware_sdk PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/external/cJSON
    )
    set_target_properties(cjson PROPERTIES COMPILE_FLAGS "-Wno-error")
    set_target_properties(cjson PROPERTIES COMPILE_FLAGS "-Wno-float-conversion")
    target_link_libraries(iot_firmware_sdk cjson)
endfunction()

# mbedTLS Configuration
function(configure_mbedtls)
    set(ENABLE_PROGRAMS OFF CACHE BOOL "Build mbed TLS programs.")
    set(ENABLE_TESTING OFF CACHE BOOL "Build mbed TLS tests.")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build mbed TLS shared library.")
    add_subdirectory(external/mbedtls)
    target_link_libraries(iot_firmware_sdk mbedtls mbedx509 mbedcrypto)
endfunction() 