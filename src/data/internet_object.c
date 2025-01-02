#include "data/internet_object.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a new IO and initialize an empty JSON object
IO* io_create()
{
    IO* obj = (IO*)malloc(sizeof(IO));
    if (obj != NULL) {
        obj->json_obj = cJSON_CreateObject();
    }
    return obj;
}

// Destroy the IO, freeing memory
void io_destroy(IO* obj)
{
    if (obj != NULL) {
        cJSON_Delete(obj->json_obj);
        free(obj);
    }
}

// Add a string key-value pair to the IO
int io_add_string(IO* obj, const char* key, const char* value)
{
    if (obj == NULL || key == NULL || value == NULL) {
        return -1; // Error: invalid input
    }
    cJSON_AddStringToObject(obj->json_obj, key, value);
    return 0; // Success
}

// Add an integer key-value pair to the IO
int io_add_int(IO* obj, const char* key, int value)
{
    if (obj == NULL || key == NULL) {
        return -1; // Error: invalid input
    }
    cJSON_AddNumberToObject(obj->json_obj, key, value);
    return 0; // Success
}

// Get a string value from the IO by key
char* io_get_string(IO* obj, const char* key)
{
    if (obj == NULL || key == NULL) {
        return NULL; // Error: invalid input
    }
    cJSON* item = cJSON_GetObjectItemCaseSensitive(obj->json_obj, key);
    if (cJSON_IsString(item) && item->valuestring != NULL) {
        return item->valuestring;
    }
    return NULL; // Not found or not a string
}

// Get an integer value from the IO by key
int io_get_int(IO* obj, const char* key)
{
    if (obj == NULL || key == NULL) {
        return 0; // Error: invalid input
    }
    cJSON* item = cJSON_GetObjectItemCaseSensitive(obj->json_obj, key);
    if (cJSON_IsNumber(item)) {
        return item->valueint;
    }
    return 0; // Not found or not a number
}

// Convert the IO to a JSON string (for serialization)
char* io_to_string(IO* obj)
{
    if (obj == NULL) {
        return NULL; // Error: invalid input
    }
    char* json_string = cJSON_Print(obj->json_obj);
    return json_string;
}

// Parse a JSON string into an IO (for deserialization)
IO* io_from_string(const char* json_string)
{
    if (json_string == NULL) {
        return NULL; // Error: invalid input
    }
    IO* obj = (IO*)malloc(sizeof(IO));
    if (obj != NULL) {
        obj->json_obj = cJSON_Parse(json_string);
        if (obj->json_obj == NULL) {
            free(obj);
            return NULL; // Error: invalid JSON string
        }
    }
    return obj;
}
