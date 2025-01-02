#ifndef IOT_DATA_INTERNET_OBJECT_H
#define IOT_DATA_INTERNET_OBJECT_H

#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

// Define a struct for IO (Internet Object)
typedef struct {
    cJSON* json_obj;
} IO;

// Function declarations

// Create a new IO and initialize an empty JSON object
IO* io_create();

// Destroy the IO, freeing memory
void io_destroy(IO* obj);

// Add a string key-value pair to the IO
int io_add_string(IO* obj, const char* key, const char* value);

// Add an integer key-value pair to the IO
int io_add_int(IO* obj, const char* key, int value);

// Get a string value from the IO by key
char* io_get_string(IO* obj, const char* key);

// Get an integer value from the IO by key
int io_get_int(IO* obj, const char* key);

// Convert the IO to a JSON string (for serialization)
char* io_to_string(IO* obj);

// Parse a JSON string into an IO (for deserialization)
IO* io_from_string(const char* json_string);

#ifdef __cplusplus
}
#endif

#endif // IOT_DATA_INTERNET_OBJECT_H