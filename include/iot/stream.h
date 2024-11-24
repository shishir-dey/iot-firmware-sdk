#ifndef IOT_STREAM_H
#define IOT_STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cJSON.h"
#include "iot/custom_types.h"

#define STREAM_NAME_MAX_LEN 32

typedef struct
{
    cJSON* obj;
    char name[STREAM_NAME_MAX_LEN];
    bb_timestamp_ms_t timestamp;
    bb_uint32_t sequence;
    char topic[256];
} stream_t;

/**
 * @brief Creates a new stream object with the given name.
 * @param name The name of the stream.
 * @return A pointer to the newly created stream object.
 */
stream_t* stream_create(const char* name);

/**
 * @brief Sets the name of an existing stream object.
 * @param handle The stream object to set the name for.
 * @param name The new name for the stream.
 */
void stream_set_name(stream_t* handle, const char* name);

/**
 * @brief Gets the name of a stream object.
 * @param handle The stream object to get the name from.
 * @return The name of the stream.
 */
char* stream_get_name(stream_t* handle);

/**
 * @brief Adds a float32 value to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The float32 value to add.
 */
void stream_add_float32(stream_t* handle, const char* key, bb_float32_t value);

/**
 * @brief Adds a float64 value to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The float64 value to add.
 */
void stream_add_float64(stream_t* handle, const char* key, bb_float64_t value);

/**
 * @brief Adds an unsigned 8-bit integer value to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The unsigned 8-bit integer value to add.
 */
void stream_add_uint8(stream_t* handle, const char* key, bb_uint8_t value);

/**
 * @brief Adds an unsigned 16-bit integer value to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The unsigned 16-bit integer value to add.
 */
void stream_add_uint16(stream_t* handle, const char* key, bb_uint16_t value);

/**
 * @brief Adds an unsigned 32-bit integer value to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The unsigned 32-bit integer value to add.
 */
void stream_add_uint32(stream_t* handle, const char* key, bb_uint32_t value);

/**
 * @brief Adds an unsigned 64-bit integer value to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The unsigned 64-bit integer value to add.
 */
void stream_add_uint64(stream_t* handle, const char* key, bb_uint64_t value);

/**
 * @brief Adds a string value to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The string value to add.
 */
void stream_add_string(stream_t* handle, const char* key, bb_string_t value);

/**
 * @brief Adds a datetime64 value to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The datetime64 value to add.
 */
void stream_add_datetime64(stream_t* handle, const char* key, bb_datetime64_t value);

/**
 * @brief Adds a timestamp value to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The timestamp value to add.
 */
void stream_add_timestamp(stream_t* handle, const char* key, bb_timestamp_t value);

/**
 * @brief Adds a timestamp in milliseconds to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The timestamp value in milliseconds to add.
 */
void stream_add_timestamp_ms(stream_t* handle, const char* key, bb_timestamp_ms_t value);

/**
 * @brief Adds a timestamp in microseconds to the stream object.
 * @param handle The stream object to add the value to.
 * @param key The key for the value.
 * @param value The timestamp value in microseconds to add.
 */
void stream_add_timestamp_us(stream_t* handle, const char* key, bb_timestamp_us_t value);

/**
 * @brief Prints the stream object as a JSON string.
 * @param handle The stream object to print.
 * @return A JSON string representation of the stream object.
 */
char* stream_print(stream_t* handle);

/**
 * @brief Deletes a stream object and frees associated memory.
 * @param handle The stream object to delete.
 */
void stream_delete(stream_t* handle);

#ifdef __cplusplus
}
#endif

#endif /* IOT_STREAM_H */
