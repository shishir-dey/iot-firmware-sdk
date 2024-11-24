#include "iot/mqtt_client_port.h"
#include "iot/mqtt_client.h"

MQTTClientContext* get_mqtt_client_context();

uint32_t get_current_time(void)
{
    // TODO: Implement actual time retrieval
    return 0;
}

int network_send(NetworkContext_t* pNetworkContext, const void* pBuffer, size_t bytesToSend)
{
    return mbedtls_ssl_write(&(get_mqtt_client_context()->ssl_context), pBuffer, bytesToSend);
}

int network_recv(NetworkContext_t* pNetworkContext, void* pBuffer, size_t bytesToRecv)
{
    return mbedtls_ssl_read(&(get_mqtt_client_context()->ssl_context), pBuffer, bytesToRecv);
}