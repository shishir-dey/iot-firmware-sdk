#include "connectivity/http_client.h"
#include "core_http_client.h"
#include "interface/transport.h"
#include <stdlib.h>
#include <string.h>

#define MAX_URL_LENGTH 256
#define MAX_HEADERS 20
#define DEFAULT_TIMEOUT_MS 5000

// Define the NetworkContext structure
struct NetworkContext {
    void* impl; // Implementation-specific data
};

typedef struct {
    void* transport_ctx;
    char base_url[MAX_URL_LENGTH];
    char host[128];
    char port[8];
    const char* headers[MAX_HEADERS];
    size_t header_count;
    TransportInterface_t transport_interface;
    HTTPRequestHeaders_t request_headers;
    HTTPResponse_t response;
    struct NetworkContext network_context;
} IotHttpContext_t;

static IotHttpContext_t* http_ctx = NULL;

// Transport interface implementations
static int32_t transport_recv(NetworkContext_t* pNetworkContext, void* pBuffer, size_t bytesToRecv)
{
    if (pNetworkContext == NULL) {
        return -1;
    }
    return iot_transport_recv(pNetworkContext->impl, pBuffer, bytesToRecv);
}

static int32_t transport_send(NetworkContext_t* pNetworkContext, const void* pBuffer, size_t bytesToSend)
{
    if (pNetworkContext == NULL) {
        return -1;
    }
    return iot_transport_send(pNetworkContext->impl, pBuffer, bytesToSend);
}

int iot_http_init(void)
{
    if (http_ctx != NULL) {
        return -1; // Already initialized
    }

    http_ctx = (IotHttpContext_t*)calloc(1, sizeof(IotHttpContext_t));
    if (http_ctx == NULL) {
        return -1;
    }

    // Initialize transport interface
    http_ctx->transport_interface.recv = transport_recv;
    http_ctx->transport_interface.send = transport_send;
    http_ctx->transport_interface.pNetworkContext = &http_ctx->network_context;
    http_ctx->network_context.impl = NULL;

    return 0;
}

int iot_http_set_url(const char* url)
{
    if (http_ctx == NULL || url == NULL) {
        return -1;
    }

    // Parse URL to extract host, port, and path
    const char* protocol = strstr(url, "://");
    if (protocol == NULL) {
        return -1;
    }

    protocol += 3;
    const char* port_start = strchr(protocol, ':');
    const char* path_start = strchr(protocol, '/');

    if (port_start && (!path_start || port_start < path_start)) {
        size_t host_len = port_start - protocol;
        strncpy(http_ctx->host, protocol, host_len);
        http_ctx->host[host_len] = '\0';

        size_t port_len = (path_start ? path_start - port_start - 1 : strlen(port_start) - 1);
        strncpy(http_ctx->port, port_start + 1, port_len);
        http_ctx->port[port_len] = '\0';
    } else {
        size_t host_len = path_start ? path_start - protocol : strlen(protocol);
        strncpy(http_ctx->host, protocol, host_len);
        http_ctx->host[host_len] = '\0';
        strcpy(http_ctx->port, "80");
    }

    strncpy(http_ctx->base_url, url, MAX_URL_LENGTH - 1);
    http_ctx->base_url[MAX_URL_LENGTH - 1] = '\0';

    return iot_transport_open(&http_ctx->network_context.impl,
        http_ctx->host,
        http_ctx->port);
}

static int perform_http_request(const char* method,
    const char* path,
    const uint8_t* payload,
    size_t payload_length,
    char* response,
    size_t response_length)
{
    if (http_ctx == NULL) {
        return -1;
    }

    // Prepare request headers buffer
    uint8_t* header_buffer = (uint8_t*)malloc(1024);
    if (header_buffer == NULL) {
        return -1;
    }

    HTTPRequestHeaders_t request_headers = {
        .pBuffer = header_buffer,
        .bufferLen = 1024,
        .headersLen = 0
    };

    // Prepare response structure
    HTTPResponse_t http_response = {
        .pBuffer = (uint8_t*)response,
        .bufferLen = response_length,
        .pHeaders = header_buffer, // Reuse header buffer for response headers
        .headersLen = 1024
    };

    // Send the request using CoreHTTP's API
    HTTPStatus_t status = HTTPClient_Send(
        &http_ctx->transport_interface,
        &request_headers,
        payload,
        payload_length,
        &http_response,
        0 // No special flags
    );

    free(header_buffer);
    return (status == HTTPSuccess) ? 0 : -1;
}

int iot_http_get(const char* path, char* response, size_t response_length)
{
    return perform_http_request("GET", path, NULL, 0, response, response_length);
}

int iot_http_post(const char* path, const uint8_t* payload, size_t payload_length,
    char* response, size_t response_length)
{
    return perform_http_request("POST", path, payload, payload_length, response, response_length);
}

int iot_http_put(const char* path, const uint8_t* payload, size_t payload_length,
    char* response, size_t response_length)
{
    return perform_http_request("PUT", path, payload, payload_length, response, response_length);
}

int iot_http_delete(const char* path, char* response, size_t response_length)
{
    return perform_http_request("DELETE", path, NULL, 0, response, response_length);
}

int iot_http_set_headers(const char* headers[], size_t header_count)
{
    if (http_ctx == NULL || header_count > MAX_HEADERS) {
        return -1;
    }

    for (size_t i = 0; i < header_count; i++) {
        http_ctx->headers[i] = strdup(headers[i]);
        if (http_ctx->headers[i] == NULL) {
            // Clean up on failure
            for (size_t j = 0; j < i; j++) {
                free((void*)http_ctx->headers[j]);
            }
            return -1;
        }
    }

    http_ctx->header_count = header_count;
    return 0;
}

int iot_http_cleanup(void)
{
    if (http_ctx == NULL) {
        return -1;
    }

    // Clean up headers
    for (size_t i = 0; i < http_ctx->header_count; i++) {
        free((void*)http_ctx->headers[i]);
    }

    // Close transport
    if (http_ctx->network_context.impl) {
        iot_transport_close(http_ctx->network_context.impl);
    }

    // Free main context
    free(http_ctx);
    http_ctx = NULL;

    return 0;
}
