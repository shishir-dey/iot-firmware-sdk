#include "bytebeam/custom_types.h"
#include "cJSON.h"

typedef cJSON stream;

stream* stream_create()
{
    return cJSON_CreateObject();
}

void stream_add_bool(stream* obj, const char* key, bb_bool_t value)
{
    cJSON_AddBoolToObject(obj, key, value);
}

void stream_add_float32(stream* obj, const char* key, bb_float32_t value)
{
    cJSON_AddNumberToObject(obj, key, (double)value);
}

void stream_add_float64(stream* obj, const char* key, bb_float64_t value)
{
    cJSON_AddNumberToObject(obj, key, value);
}

void stream_add_int8(stream* obj, const char* key, bb_int8_t value)
{
    cJSON_AddNumberToObject(obj, key, (int)value);
}

void stream_add_int16(stream* obj, const char* key, bb_int16_t value)
{
    cJSON_AddNumberToObject(obj, key, (int)value);
}

void stream_add_int32(stream* obj, const char* key, bb_int32_t value)
{
    cJSON_AddNumberToObject(obj, key, (int)value);
}

void stream_add_int64(stream* obj, const char* key, bb_int64_t value)
{
    cJSON_AddNumberToObject(obj, key, (double)value);
}

void stream_add_uint8(stream* obj, const char* key, bb_uint8_t value)
{
    cJSON_AddNumberToObject(obj, key, (unsigned int)value);
}

void stream_add_uint16(stream* obj, const char* key, bb_uint16_t value)
{
    cJSON_AddNumberToObject(obj, key, (unsigned int)value);
}

void stream_add_uint32(stream* obj, const char* key, bb_uint32_t value)
{
    cJSON_AddNumberToObject(obj, key, (double)value);
}

void stream_add_uint64(stream* obj, const char* key, bb_uint64_t value)
{
    cJSON_AddNumberToObject(obj, key, (double)value);
}

void stream_add_string(stream* obj, const char* key, bb_string_t value)
{
    cJSON_AddStringToObject(obj, key, value);
}

void stream_add_datetime64(stream* obj, const char* key, bb_datetime64_t value)
{
    cJSON_AddNumberToObject(obj, key, (double)value);
}

void stream_add_timestamp(stream* obj, const char* key, bb_timestamp_t value)
{
    cJSON_AddNumberToObject(obj, key, (double)value);
}

void stream_add_timestamp_ms(stream* obj, const char* key, bb_timestamp_ms_t value)
{
    cJSON_AddNumberToObject(obj, key, (double)value);
}

void stream_add_timestamp_us(stream* obj, const char* key, bb_timestamp_us_t value)
{
    cJSON_AddNumberToObject(obj, key, (double)value);
}

char* stream_print(stream* obj)
{
    return cJSON_Print(obj);
}

void stream_delete(stream* obj)
{
    cJSON_Delete(obj);
}
