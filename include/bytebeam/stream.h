#ifndef BYTEBEAM_STREAM_H
#define BYTEBEAM_STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef cJSON stream;

#include "bytebeam/custom_types.h"

/**
 * @brief Creates a new stream object.
 * @return A pointer to the newly created stream object.
 */
stream* stream_create();

/**
 * @brief Adds a boolean value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The boolean value to add.
 */
void stream_add_bool(stream* obj, const char* key, bb_bool_t value);

/**
 * @brief Adds a 32-bit float value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 32-bit float value to add.
 */
void stream_add_float32(stream* obj, const char* key, bb_float32_t value);

/**
 * @brief Adds a 64-bit float value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 64-bit float value to add.
 */
void stream_add_float64(stream* obj, const char* key, bb_float64_t value);

/**
 * @brief Adds an 8-bit integer value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 8-bit integer value to add.
 */
void stream_add_int8(stream* obj, const char* key, bb_int8_t value);

/**
 * @brief Adds a 16-bit integer value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 16-bit integer value to add.
 */
void stream_add_int16(stream* obj, const char* key, bb_int16_t value);

/**
 * @brief Adds a 32-bit integer value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 32-bit integer value to add.
 */
void stream_add_int32(stream* obj, const char* key, bb_int32_t value);

/**
 * @brief Adds a 64-bit integer value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 64-bit integer value to add.
 */
void stream_add_int64(stream* obj, const char* key, bb_int64_t value);

/**
 * @brief Adds an 8-bit unsigned integer value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 8-bit unsigned integer value to add.
 */
void stream_add_uint8(stream* obj, const char* key, bb_uint8_t value);

/**
 * @brief Adds a 16-bit unsigned integer value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 16-bit unsigned integer value to add.
 */
void stream_add_uint16(stream* obj, const char* key, bb_uint16_t value);

/**
 * @brief Adds a 32-bit unsigned integer value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 32-bit unsigned integer value to add.
 */
void stream_add_uint32(stream* obj, const char* key, bb_uint32_t value);

/**
 * @brief Adds a 64-bit unsigned integer value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The 64-bit unsigned integer value to add.
 */
void stream_add_uint64(stream* obj, const char* key, bb_uint64_t value);

/**
 * @brief Adds a string value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The string value to add.
 */
void stream_add_string(stream* obj, const char* key, bb_string_t value);

/**
 * @brief Adds a datetime64 value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The datetime64 value to add.
 */
void stream_add_datetime64(stream* obj, const char* key, bb_datetime64_t value);

/**
 * @brief Adds a timestamp value to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The timestamp value to add.
 */
void stream_add_timestamp(stream* obj, const char* key, bb_timestamp_t value);

/**
 * @brief Adds a timestamp in milliseconds to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The timestamp value in milliseconds to add.
 */
void stream_add_timestamp_ms(stream* obj, const char* key, bb_timestamp_ms_t value);

/**
 * @brief Adds a timestamp in microseconds to the stream object.
 * @param obj The stream object to add the value to.
 * @param key The key for the value.
 * @param value The timestamp value in microseconds to add.
 */
void stream_add_timestamp_us(stream* obj, const char* key, bb_timestamp_us_t value);

/**
 * @brief Prints the stream object as a JSON string.
 * @param obj The stream object to print.
 * @return A pointer to the JSON string representation of the stream object.
 */
char* stream_print(stream* obj);

/**
 * @brief Deletes the stream object and frees associated memory.
 * @param obj The stream object to delete.
 */
void stream_delete(stream* obj);

#ifdef __cplusplus
}
#endif

#endif /* BYTEBEAM_STREAM_H */
