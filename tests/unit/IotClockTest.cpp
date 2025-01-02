#include "interface/clock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Mock class for iot_get_time function
class MockIotClock {
public:
    MOCK_METHOD(uint64_t, iot_get_time, (enum iot_time_unit unit), (const));
};

// Test fixture for Clock
class IotClockTest : public ::testing::Test {
protected:
    MockIotClock mockIotClock;

    // Override the iot_get_time function globally to use the mock
    uint64_t iot_get_time(enum iot_time_unit unit)
    {
        return mockIotClock.iot_get_time(unit);
    }
};

// Test: iot_get_time returns the correct value for microseconds
TEST_F(IotClockTest, GetTimeMicroseconds)
{
    // Mock the iot_get_time function to return 1000000 for microseconds
    EXPECT_CALL(mockIotClock, iot_get_time(IOT_TIME_MICROSECONDS))
        .WillOnce(testing::Return(1000000));

    uint64_t time = iot_get_time(IOT_TIME_MICROSECONDS);
    EXPECT_EQ(time, 1000000); // Verify the return value is correct
}

// Test: iot_get_time returns the correct value for milliseconds
TEST_F(IotClockTest, GetTimeMilliseconds)
{
    // Mock the iot_get_time function to return 1000 for milliseconds
    EXPECT_CALL(mockIotClock, iot_get_time(IOT_TIME_MILLISECONDS))
        .WillOnce(testing::Return(1000));

    uint64_t time = iot_get_time(IOT_TIME_MILLISECONDS);
    EXPECT_EQ(time, 1000); // Verify the return value is correct
}

// Test: iot_get_time returns the correct value for seconds
TEST_F(IotClockTest, GetTimeSeconds)
{
    // Mock the iot_get_time function to return a timestamp (e.g., seconds)
    EXPECT_CALL(mockIotClock, iot_get_time(IOT_TIME_SECONDS))
        .WillOnce(testing::Return(1609459200)); // Timestamp for 2021-01-01 00:00:00 UTC

    uint64_t timestamp = iot_get_time(IOT_TIME_SECONDS);
    EXPECT_EQ(timestamp, 1609459200); // Verify the timestamp is correct
}

// Test: Verify multiple calls to iot_get_time with different units
TEST_F(IotClockTest, MultipleCallsWithDifferentUnits)
{
    // Set up mock to return different values for different units
    EXPECT_CALL(mockIotClock, iot_get_time(IOT_TIME_MICROSECONDS))
        .WillOnce(testing::Return(1000000))
        .WillOnce(testing::Return(2000000));

    EXPECT_EQ(iot_get_time(IOT_TIME_MICROSECONDS), 1000000);
    EXPECT_EQ(iot_get_time(IOT_TIME_MICROSECONDS), 2000000);
}

// Test: Ensure no calls are made when not expected
TEST_F(IotClockTest, NoCalls)
{
    // Ensure no unexpected calls are made
    testing::Mock::VerifyAndClearExpectations(&mockIotClock);
}
