#include "bytebeam/stream.h"
#include <stdlib.h>
#include <string.h>

stream_t* stream_create(const char* name)
{
    stream_t* handle = malloc(sizeof(stream_t));
    handle->obj = cJSON_CreateObject();
    strncpy(handle->name, name, STREAM_NAME_MAX_LEN);
    return handle;
}

void stream_set_name(stream_t* handle, const char* name)
{
    handle->obj = cJSON_CreateObject();
    strncpy(handle->name, name, STREAM_NAME_MAX_LEN);
}

char* stream_get_name(stream_t* handle)
{
    return handle->name;
}

void stream_add_bool(stream_t* handle, const char* key, bb_bool_t value)
{
    cJSON_AddBoolToObject(handle->obj, key, value);
}

void stream_add_float32(stream_t* handle, const char* key, bb_float32_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (double)value);
}

void stream_add_float64(stream_t* handle, const char* key, bb_float64_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, value);
}

void stream_add_int8(stream_t* handle, const char* key, bb_int8_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (int)value);
}

void stream_add_int16(stream_t* handle, const char* key, bb_int16_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (int)value);
}

void stream_add_int32(stream_t* handle, const char* key, bb_int32_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (int)value);
}

void stream_add_int64(stream_t* handle, const char* key, bb_int64_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (double)value);
}

void stream_add_uint8(stream_t* handle, const char* key, bb_uint8_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (unsigned int)value);
}

void stream_add_uint16(stream_t* handle, const char* key, bb_uint16_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (unsigned int)value);
}

void stream_add_uint32(stream_t* handle, const char* key, bb_uint32_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (double)value);
}

void stream_add_uint64(stream_t* handle, const char* key, bb_uint64_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (double)value);
}

void stream_add_string(stream_t* handle, const char* key, bb_string_t value)
{
    cJSON_AddStringToObject(handle->obj, key, value);
}

void stream_add_datetime64(stream_t* handle, const char* key, bb_datetime64_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (double)value);
}

void stream_add_timestamp(stream_t* handle, const char* key, bb_timestamp_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (double)value);
}

void stream_add_timestamp_ms(stream_t* handle, const char* key, bb_timestamp_ms_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (double)value);
}

void stream_add_timestamp_us(stream_t* handle, const char* key, bb_timestamp_us_t value)
{
    cJSON_AddNumberToObject(handle->obj, key, (double)value);
}

char* stream_print(stream_t* handle)
{
    return cJSON_Print(handle->obj);
}

void stream_delete(stream_t* handle)
{
    cJSON_Delete(handle->obj);
}
