#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <sys/types.h>

/**
 * @brief Open a connection to a remote host
 *
 * @param ctx Pointer to a void pointer that will be set to the context
 * @param host Hostname or IP address of the remote host
 * @param port Port number as a string
 * @return int 0 on success, negative value on error
 */
int port_open(void** ctx, const char* host, const char* port);

/**
 * @brief Close the connection and free resources
 *
 * @param ctx Context pointer
 */
void port_close(void* ctx);

/**
 * @brief Send data over SSL connection
 *
 * @param ctx Context pointer (typically contains socket information)
 * @param buf Buffer containing data to send
 * @param len Length of data to send
 * @return int Number of bytes sent, or error code
 */
int port_ssl_send(void* ctx, const unsigned char* buf, size_t len);

/**
 * @brief Receive data over SSL connection
 *
 * @param ctx Context pointer (typically contains socket information)
 * @param buf Buffer to store received data
 * @param len Maximum length of data to receive
 * @return int Number of bytes received, or error code
 */
int port_ssl_recv(void* ctx, unsigned char* buf, size_t len);

#endif // TRANSPORT_H
