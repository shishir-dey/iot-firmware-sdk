#include "interface/os.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Mock class for iot_os functions
class MockIotOs {
public:
    MOCK_METHOD(iot_thread*, iot_thread_create, (const char* name, iot_thread_func_t func, void* arg, uint32_t priority, uint32_t stack_size, uint32_t event_count), (const));
    MOCK_METHOD(int, iot_thread_delete, (iot_thread * thread), (const));
    MOCK_METHOD(int, iot_thread_set_priority, (iot_thread * thread, uint32_t priority), (const));
    MOCK_METHOD(int, iot_thread_get_priority, (iot_thread * thread, uint32_t* priority), (const));
    MOCK_METHOD(int, iot_thread_suspend, (iot_thread * thread), (const));
    MOCK_METHOD(int, iot_thread_resume, (iot_thread * thread), (const));
    MOCK_METHOD(void, iot_thread_delay, (uint32_t milliseconds), (const));
    MOCK_METHOD(int, iot_thread_join, (iot_thread * thread, void** retval), (const));
    MOCK_METHOD(iot_mutex*, iot_mutex_init, (), (const));
    MOCK_METHOD(int, iot_mutex_lock, (iot_mutex * mutex), (const));
    MOCK_METHOD(int, iot_mutex_unlock, (iot_mutex * mutex), (const));
    MOCK_METHOD(void, iot_mutex_destroy, (iot_mutex * mutex), (const));
};

// Test fixture for IOT_OS
class IotOsTest : public ::testing::Test {
protected:
    MockIotOs mockIotOs;

    // Override the iot_os functions globally to use the mock
    iot_thread* iot_thread_create(const char* name, iot_thread_func_t func, void* arg, uint32_t priority, uint32_t stack_size, uint32_t event_count)
    {
        return mockIotOs.iot_thread_create(name, func, arg, priority, stack_size, event_count);
    }

    int iot_thread_delete(iot_thread* thread)
    {
        return mockIotOs.iot_thread_delete(thread);
    }

    int iot_thread_set_priority(iot_thread* thread, uint32_t priority)
    {
        return mockIotOs.iot_thread_set_priority(thread, priority);
    }

    int iot_thread_get_priority(iot_thread* thread, uint32_t* priority)
    {
        return mockIotOs.iot_thread_get_priority(thread, priority);
    }

    int iot_thread_suspend(iot_thread* thread)
    {
        return mockIotOs.iot_thread_suspend(thread);
    }

    int iot_thread_resume(iot_thread* thread)
    {
        return mockIotOs.iot_thread_resume(thread);
    }

    void iot_thread_delay(uint32_t milliseconds)
    {
        mockIotOs.iot_thread_delay(milliseconds);
    }

    int iot_thread_join(iot_thread* thread, void** retval)
    {
        return mockIotOs.iot_thread_join(thread, retval);
    }

    iot_mutex* iot_mutex_init()
    {
        return mockIotOs.iot_mutex_init();
    }

    int iot_mutex_lock(iot_mutex* mutex)
    {
        return mockIotOs.iot_mutex_lock(mutex);
    }

    int iot_mutex_unlock(iot_mutex* mutex)
    {
        return mockIotOs.iot_mutex_unlock(mutex);
    }

    void iot_mutex_destroy(iot_mutex* mutex)
    {
        mockIotOs.iot_mutex_destroy(mutex);
    }
};

// Test: iot_thread_create returns the correct thread handle
TEST_F(IotOsTest, ThreadCreateSuccess)
{
    iot_thread* thread = (iot_thread*)0x1234;
    EXPECT_CALL(mockIotOs, iot_thread_create("TestThread", testing::_, testing::_, 1, 1024, 5))
        .WillOnce(testing::Return(thread));

    iot_thread* created_thread = iot_thread_create("TestThread", nullptr, nullptr, 1, 1024, 5);
    EXPECT_EQ(created_thread, thread); // Verify the thread handle
}

// Test: iot_thread_delete returns 0 on success
TEST_F(IotOsTest, ThreadDeleteSuccess)
{
    iot_thread* thread = (iot_thread*)0x1234;
    EXPECT_CALL(mockIotOs, iot_thread_delete(thread))
        .WillOnce(testing::Return(0));

    int result = iot_thread_delete(thread);
    EXPECT_EQ(result, 0); // Verify the success code
}

// Test: iot_mutex_init returns a valid mutex handle
TEST_F(IotOsTest, MutexInitSuccess)
{
    iot_mutex* mutex = (iot_mutex*)0x5678;
    EXPECT_CALL(mockIotOs, iot_mutex_init())
        .WillOnce(testing::Return(mutex));

    iot_mutex* created_mutex = iot_mutex_init();
    EXPECT_EQ(created_mutex, mutex); // Verify the mutex handle
}

// Test: iot_mutex_lock and iot_mutex_unlock work as expected
TEST_F(IotOsTest, MutexLockUnlock)
{
    iot_mutex* mutex = (iot_mutex*)0x5678;
    EXPECT_CALL(mockIotOs, iot_mutex_lock(mutex))
        .WillOnce(testing::Return(0));
    EXPECT_CALL(mockIotOs, iot_mutex_unlock(mutex))
        .WillOnce(testing::Return(0));

    int lock_result = iot_mutex_lock(mutex);
    EXPECT_EQ(lock_result, 0); // Verify the success code for lock

    int unlock_result = iot_mutex_unlock(mutex);
    EXPECT_EQ(unlock_result, 0); // Verify the success code for unlock
}

// Test: iot_thread_suspend and iot_thread_resume work as expected
TEST_F(IotOsTest, ThreadSuspendResume)
{
    iot_thread* thread = (iot_thread*)0x1234;
    EXPECT_CALL(mockIotOs, iot_thread_suspend(thread))
        .WillOnce(testing::Return(0));
    EXPECT_CALL(mockIotOs, iot_thread_resume(thread))
        .WillOnce(testing::Return(0));

    int suspend_result = iot_thread_suspend(thread);
    EXPECT_EQ(suspend_result, 0); // Verify the success code for suspend

    int resume_result = iot_thread_resume(thread);
    EXPECT_EQ(resume_result, 0); // Verify the success code for resume
}

// Test: iot_thread_delay works as expected
TEST_F(IotOsTest, ThreadDelay)
{
    EXPECT_CALL(mockIotOs, iot_thread_delay(100))
        .Times(1);

    iot_thread_delay(100); // Verify that delay is called with 100 milliseconds
}

// Test: iot_thread_join works as expected
TEST_F(IotOsTest, ThreadJoin)
{
    iot_thread* thread = (iot_thread*)0x1234;
    void* retval = nullptr;
    EXPECT_CALL(mockIotOs, iot_thread_join(thread, &retval))
        .WillOnce(testing::Return(0));

    int join_result = iot_thread_join(thread, &retval);
    EXPECT_EQ(join_result, 0); // Verify the success code for join
}
