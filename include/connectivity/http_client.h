#ifndef IOT_HTTP_CLIENT_H
#define IOT_HTTP_CLIENT_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Initialize the HTTP client
 *
 * @return int 0 on success, negative value on error
 */
int iot_http_init(void);

/**
 * @brief Set the server URL
 *
 * @param url URL of the HTTP server
 * @return int 0 on success, negative value on error
 */
int iot_http_set_url(const char* url);

/**
 * @brief Perform an HTTP GET request
 *
 * @param path Resource path (relative to the base URL)
 * @param response Buffer to store the response
 * @param response_length Size of the response buffer
 * @return int 0 on success, negative value on error
 */
int iot_http_get(const char* path, char* response, size_t response_length);

/**
 * @brief Perform an HTTP POST request
 *
 * @param path Resource path (relative to the base URL)
 * @param payload Data to send in the POST request body
 * @param payload_length Length of the payload
 * @param response Buffer to store the response
 * @param response_length Size of the response buffer
 * @return int 0 on success, negative value on error
 */
int iot_http_post(const char* path, const uint8_t* payload, size_t payload_length, char* response, size_t response_length);

/**
 * @brief Perform an HTTP PUT request
 *
 * @param path Resource path (relative to the base URL)
 * @param payload Data to send in the PUT request body
 * @param payload_length Length of the payload
 * @param response Buffer to store the response
 * @param response_length Size of the response buffer
 * @return int 0 on success, negative value on error
 */
int iot_http_put(const char* path, const uint8_t* payload, size_t payload_length, char* response, size_t response_length);

/**
 * @brief Perform an HTTP DELETE request
 *
 * @param path Resource path (relative to the base URL)
 * @param response Buffer to store the response
 * @param response_length Size of the response buffer
 * @return int 0 on success, negative value on error
 */
int iot_http_delete(const char* path, char* response, size_t response_length);

/**
 * @brief Set custom headers for the HTTP requests
 *
 * @param headers Array of header strings (e.g., "Key: Value")
 * @param header_count Number of headers in the array
 * @return int 0 on success, negative value on error
 */
int iot_http_set_headers(const char* headers[], size_t header_count);

/**
 * @brief Close the HTTP client and release resources
 *
 * @return int 0 on success, negative value on error
 */
int iot_http_cleanup(void);

#endif
