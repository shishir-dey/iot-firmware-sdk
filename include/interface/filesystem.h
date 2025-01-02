#ifndef IOT_FILESYSTEM_H
#define IOT_FILESYSTEM_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque file handle
struct iot_file;

// File operation interfaces
struct iot_file* iot_fopen(const char* path, const char* mode);
size_t iot_fwrite(const void* ptr, size_t size, size_t count, struct iot_file* file);
size_t iot_fread(void* ptr, size_t size, size_t count, struct iot_file* file);
int iot_fseek(struct iot_file* file, long offset);
int iot_fclose(struct iot_file* file);
int iot_ftruncate(struct iot_file* file, size_t size);

// Directory operations
int iot_mkdir(const char* path);
int iot_remove(const char* path);
int iot_mkdirp(const char* path);
int iot_rmdir_recursive(const char* path);

struct iot_dir;
struct iot_dirent;

struct iot_dir* iot_opendir(const char* path);
struct iot_dirent* iot_readdir(struct iot_dir* dir);
int iot_closedir(struct iot_dir* dir);

// Dirent operations
const char* iot_dirent_name(const struct iot_dirent* entry);
bool iot_is_file(const struct iot_dirent* entry);

// File information
struct iot_stat {
    size_t st_size;
};

int iot_stat(const char* path, struct iot_stat* st);

#ifdef __cplusplus
}
#endif

#endif // IOT_FILESYSTEM_H
