#ifndef __MQTT_CLIENT_PORT_H__
#define __MQTT_CLIENT_PORT_H__

#include "core_mqtt.h"
#include <stdint.h>

uint32_t get_current_time(void);
int network_send(NetworkContext_t* pNetworkContext, const void* pBuffer, size_t bytesToSend);
int network_recv(NetworkContext_t* pNetworkContext, void* pBuffer, size_t bytesToRecv);

#endif