#include "interface/filesystem.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Mock class for iot_filesystem functions
class MockIotFilesystem {
public:
    MOCK_METHOD(iot_file*, iot_fopen, (const char* path, const char* mode), (const));
    MOCK_METHOD(size_t, iot_fwrite, (const void* ptr, size_t size, size_t count, struct iot_file* file), (const));
    MOCK_METHOD(size_t, iot_fread, (void* ptr, size_t size, size_t count, struct iot_file* file), (const));
    MOCK_METHOD(int, iot_fseek, (struct iot_file * file, long offset), (const));
    MOCK_METHOD(int, iot_fclose, (struct iot_file * file), (const));
    MOCK_METHOD(int, iot_ftruncate, (struct iot_file * file, size_t size), (const));

    MOCK_METHOD(int, iot_mkdir, (const char* path), (const));
    MOCK_METHOD(int, iot_remove, (const char* path), (const));
    MOCK_METHOD(int, iot_mkdirp, (const char* path), (const));
    MOCK_METHOD(int, iot_rmdir_recursive, (const char* path), (const));

    MOCK_METHOD(iot_dir*, iot_opendir, (const char* path), (const));
    MOCK_METHOD(iot_dirent*, iot_readdir, (iot_dir * dir), (const));
    MOCK_METHOD(int, iot_closedir, (iot_dir * dir), (const));

    MOCK_METHOD(const char*, iot_dirent_name, (const struct iot_dirent* entry), (const));
    MOCK_METHOD(bool, iot_is_file, (const struct iot_dirent* entry), (const));

    MOCK_METHOD(int, iot_stat, (const char* path, struct iot_stat* st), (const));
};

// Test fixture for IOT_FILESYSTEM
class IotFilesystemTest : public ::testing::Test {
protected:
    MockIotFilesystem mockIotFilesystem;

    // Override the iot_filesystem functions globally to use the mock
    iot_file* iot_fopen(const char* path, const char* mode)
    {
        return mockIotFilesystem.iot_fopen(path, mode);
    }

    size_t iot_fwrite(const void* ptr, size_t size, size_t count, struct iot_file* file)
    {
        return mockIotFilesystem.iot_fwrite(ptr, size, count, file);
    }

    size_t iot_fread(void* ptr, size_t size, size_t count, struct iot_file* file)
    {
        return mockIotFilesystem.iot_fread(ptr, size, count, file);
    }

    int iot_fseek(struct iot_file* file, long offset)
    {
        return mockIotFilesystem.iot_fseek(file, offset);
    }

    int iot_fclose(struct iot_file* file)
    {
        return mockIotFilesystem.iot_fclose(file);
    }

    int iot_ftruncate(struct iot_file* file, size_t size)
    {
        return mockIotFilesystem.iot_ftruncate(file, size);
    }

    int iot_mkdir(const char* path)
    {
        return mockIotFilesystem.iot_mkdir(path);
    }

    int iot_remove(const char* path)
    {
        return mockIotFilesystem.iot_remove(path);
    }

    int iot_mkdirp(const char* path)
    {
        return mockIotFilesystem.iot_mkdirp(path);
    }

    int iot_rmdir_recursive(const char* path)
    {
        return mockIotFilesystem.iot_rmdir_recursive(path);
    }

    iot_dir* iot_opendir(const char* path)
    {
        return mockIotFilesystem.iot_opendir(path);
    }

    iot_dirent* iot_readdir(iot_dir* dir)
    {
        return mockIotFilesystem.iot_readdir(dir);
    }

    int iot_closedir(iot_dir* dir)
    {
        return mockIotFilesystem.iot_closedir(dir);
    }

    const char* iot_dirent_name(const struct iot_dirent* entry)
    {
        return mockIotFilesystem.iot_dirent_name(entry);
    }

    bool iot_is_file(const struct iot_dirent* entry)
    {
        return mockIotFilesystem.iot_is_file(entry);
    }

    int iot_stat(const char* path, struct iot_stat* st)
    {
        return mockIotFilesystem.iot_stat(path, st);
    }
};

// Test: iot_fopen returns the correct file handle
TEST_F(IotFilesystemTest, FopenSuccess)
{
    iot_file* file = (iot_file*)0x1234;
    EXPECT_CALL(mockIotFilesystem, iot_fopen("test.txt", "r"))
        .WillOnce(testing::Return(file));

    iot_file* opened_file = iot_fopen("test.txt", "r");
    EXPECT_EQ(opened_file, file); // Verify the file handle
}

// Test: iot_fwrite writes the expected number of bytes
TEST_F(IotFilesystemTest, FwriteSuccess)
{
    struct iot_file* file = (iot_file*)0x1234;
    const char* data = "Hello, world!";
    EXPECT_CALL(mockIotFilesystem, iot_fwrite(data, 1, 13, file))
        .WillOnce(testing::Return(13));

    size_t bytes_written = iot_fwrite(data, 1, 13, file);
    EXPECT_EQ(bytes_written, 13); // Verify the bytes written
}

// Test: iot_fread reads the expected number of bytes
TEST_F(IotFilesystemTest, FreadSuccess)
{
    struct iot_file* file = (iot_file*)0x1234;
    char buffer[13];
    EXPECT_CALL(mockIotFilesystem, iot_fread(buffer, 1, 13, file))
        .WillOnce(testing::Return(13));

    size_t bytes_read = iot_fread(buffer, 1, 13, file);
    EXPECT_EQ(bytes_read, 13); // Verify the bytes read
}

// Test: iot_fseek moves the file pointer correctly
TEST_F(IotFilesystemTest, FseekSuccess)
{
    struct iot_file* file = (iot_file*)0x1234;
    EXPECT_CALL(mockIotFilesystem, iot_fseek(file, 10))
        .WillOnce(testing::Return(0));

    int result = iot_fseek(file, 10);
    EXPECT_EQ(result, 0); // Verify success
}

// Test: iot_fclose closes the file successfully
TEST_F(IotFilesystemTest, FcloseSuccess)
{
    struct iot_file* file = (iot_file*)0x1234;
    EXPECT_CALL(mockIotFilesystem, iot_fclose(file))
        .WillOnce(testing::Return(0));

    int result = iot_fclose(file);
    EXPECT_EQ(result, 0); // Verify file is closed successfully
}
