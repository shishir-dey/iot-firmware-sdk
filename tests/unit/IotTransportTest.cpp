#include "interface/transport.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Mock class for transport functions
class MockIotTransport {
public:
    MOCK_METHOD(int, iot_transport_open, (void** ctx, const char* host, const char* port), ());
    MOCK_METHOD(void, iot_transport_close, (void* ctx), ());
    MOCK_METHOD(int, iot_transport_send, (void* ctx, const unsigned char* buf, size_t len), ());
    MOCK_METHOD(int, iot_transport_recv, (void* ctx, unsigned char* buf, size_t len), ());
};

// Test fixture for IotTransport
class IotTransportTest : public ::testing::Test {
protected:
    MockIotTransport mockIotTransport;
    void* context = nullptr; // Mock context pointer

    // Override the iot_transport_open function to use the mock
    int iot_transport_open(void** ctx, const char* host, const char* port)
    {
        return mockIotTransport.iot_transport_open(ctx, host, port);
    }

    // Override the iot_transport_close function to use the mock
    void iot_transport_close(void* ctx)
    {
        mockIotTransport.iot_transport_close(ctx);
    }

    // Override the iot_transport_send function to use the mock
    int iot_transport_send(void* ctx, const unsigned char* buf, size_t len)
    {
        return mockIotTransport.iot_transport_send(ctx, buf, len);
    }

    // Override the iot_transport_recv function to use the mock
    int iot_transport_recv(void* ctx, unsigned char* buf, size_t len)
    {
        return mockIotTransport.iot_transport_recv(ctx, buf, len);
    }
};

// Test: Open connection with valid parameters
TEST_F(IotTransportTest, OpenConnection_Success)
{
    const char* host = "192.168.1.1";
    const char* port = "8080";

    // Expect the open function to return 0 for success
    EXPECT_CALL(mockIotTransport, iot_transport_open(&context, host, port))
        .WillOnce(testing::Return(0));

    int result = iot_transport_open(&context, host, port);
    EXPECT_EQ(result, 0); // Verify that the function returns success
}

// Test: Open connection with invalid parameters (failure case)
TEST_F(IotTransportTest, OpenConnection_Failure)
{
    const char* host = "invalid_host";
    const char* port = "8080";

    // Expect the open function to return a negative value for failure
    EXPECT_CALL(mockIotTransport, iot_transport_open(&context, host, port))
        .WillOnce(testing::Return(-1));

    int result = iot_transport_open(&context, host, port);
    EXPECT_EQ(result, -1); // Verify that the function returns failure
}

// Test: Close connection correctly
TEST_F(IotTransportTest, CloseConnection_Success)
{
    // Expect the close function to be called with the context
    EXPECT_CALL(mockIotTransport, iot_transport_close(context))
        .Times(1);

    iot_transport_close(context); // Verify that close is called correctly
}

// Test: Send data successfully
TEST_F(IotTransportTest, SendData_Success)
{
    unsigned char buf[] = { 0x01, 0x02, 0x03, 0x04 };
    size_t len = sizeof(buf);

    // Mock the send function to return the number of bytes sent
    EXPECT_CALL(mockIotTransport, iot_transport_send(context, buf, len))
        .WillOnce(testing::Return(len));

    int bytesSent = iot_transport_send(context, buf, len);
    EXPECT_EQ(bytesSent, len); // Verify that the correct number of bytes were sent
}

// Test: Send data failure
TEST_F(IotTransportTest, SendData_Failure)
{
    unsigned char buf[] = { 0x01, 0x02, 0x03, 0x04 };
    size_t len = sizeof(buf);

    // Mock the send function to return a negative error code
    EXPECT_CALL(mockIotTransport, iot_transport_send(context, buf, len))
        .WillOnce(testing::Return(-1));

    int result = iot_transport_send(context, buf, len);
    EXPECT_EQ(result, -1); // Verify that the function returns failure
}

// Test: Receive data successfully
TEST_F(IotTransportTest, ReceiveData_Success)
{
    unsigned char buf[4] = { 0 };
    size_t len = sizeof(buf);

    // Mock the recv function to return the number of bytes received
    EXPECT_CALL(mockIotTransport, iot_transport_recv(context, buf, len))
        .WillOnce(testing::Return(len));

    int bytesReceived = iot_transport_recv(context, buf, len);
    EXPECT_EQ(bytesReceived, len); // Verify that the correct number of bytes were received
}

// Test: Receive data failure
TEST_F(IotTransportTest, ReceiveData_Failure)
{
    unsigned char buf[4] = { 0 };
    size_t len = sizeof(buf);

    // Mock the recv function to return a negative error code
    EXPECT_CALL(mockIotTransport, iot_transport_recv(context, buf, len))
        .WillOnce(testing::Return(-1));

    int result = iot_transport_recv(context, buf, len);
    EXPECT_EQ(result, -1); // Verify that the function returns failure
}

// Test: Open connection without proper context initialization
TEST_F(IotTransportTest, OpenConnection_NoContext)
{
    void* badContext = nullptr;
    const char* host = "192.168.1.1";
    const char* port = "8080";

    // Expect the open function to return failure when the context is null
    EXPECT_CALL(mockIotTransport, iot_transport_open(&badContext, host, port))
        .WillOnce(testing::Return(-1));

    int result = iot_transport_open(&badContext, host, port);
    EXPECT_EQ(result, -1); // Verify that the function returns failure with bad context
}

// Test: Close connection when context is already null
TEST_F(IotTransportTest, CloseConnection_NullContext)
{
    EXPECT_CALL(mockIotTransport, iot_transport_close(nullptr))
        .Times(1);

    iot_transport_close(nullptr); // Verify that the function handles null context
}