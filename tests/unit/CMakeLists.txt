# Add test executable
add_executable(iot_firmware_sdk_tests
    IotClockTest.cpp
    IotTransportTest.cpp
    IotOsTest.cpp
    IotFilesystemTest.cpp
)

# Link test executable with Google Test and iot-firmware-sdk
target_link_libraries(iot_firmware_sdk_tests
    gtest_main
    gmock_main
    iot_firmware_sdk
)

# Add test to CTest
add_test(NAME iot_firmware_sdk_tests COMMAND iot_firmware_sdk_tests)

target_include_directories(iot_firmware_sdk_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
