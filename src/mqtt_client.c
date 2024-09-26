

#include "core_mqtt.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/error.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/pk.h"
#include "mbedtls/ssl.h"
#include <stdio.h>
#include <string.h>

#define MQTT_BUFFER_SIZE 1024

typedef struct {
    MQTTContext_t mqtt_context;
    mbedtls_ssl_context ssl_context;
    mbedtls_net_context net_context;
    mbedtls_ssl_config ssl_config;
    mbedtls_x509_crt ca_cert;
    mbedtls_x509_crt client_cert;
    mbedtls_pk_context client_key;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    uint8_t network_buffer[MQTT_BUFFER_SIZE];
    uint8_t fixed_buffer[MQTT_BUFFER_SIZE];
} MQTTClientContext;

static MQTTClientContext client_context;

static int network_send(NetworkContext_t* pNetworkContext, const void* pBuffer, size_t bytesToSend)
{
    return mbedtls_ssl_write(&client_context.ssl_context, pBuffer, bytesToSend);
}

static int network_recv(NetworkContext_t* pNetworkContext, void* pBuffer, size_t bytesToRecv)
{
    return mbedtls_ssl_read(&client_context.ssl_context, pBuffer, bytesToRecv);
}

int mqtt_init()
{
    TransportInterface_t transport;
    MQTTFixedBuffer_t fixed_buffer = {
        .pBuffer = client_context.fixed_buffer,
        .size = MQTT_BUFFER_SIZE
    };

    transport.pNetworkContext = NULL;
    transport.send = network_send;
    transport.recv = network_recv;

    return MQTT_Init(&client_context.mqtt_context, &transport, NULL, NULL, &fixed_buffer);
}

int mqtt_connect(const char* host, int port, const char* client_id)
{
    int ret;
    mbedtls_ssl_init(&client_context.ssl_context);
    mbedtls_net_init(&client_context.net_context);
    mbedtls_ssl_config_init(&client_context.ssl_config);
    mbedtls_x509_crt_init(&client_context.ca_cert);
    mbedtls_x509_crt_init(&client_context.client_cert);
    mbedtls_pk_init(&client_context.client_key);
    mbedtls_entropy_init(&client_context.entropy);
    mbedtls_ctr_drbg_init(&client_context.ctr_drbg);

    ret = mbedtls_ctr_drbg_seed(&client_context.ctr_drbg, mbedtls_entropy_func, &client_context.entropy, NULL, 0);
    if (ret != 0) {
        return ret;
    }

    ret = mbedtls_net_connect(&client_context.net_context, host, NULL, MBEDTLS_NET_PROTO_TCP);
    if (ret != 0) {
        return ret;
    }

    ret = mbedtls_ssl_config_defaults(&client_context.ssl_config, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0) {
        return ret;
    }

    mbedtls_ssl_conf_authmode(&client_context.ssl_config, MBEDTLS_SSL_VERIFY_REQUIRED);
    mbedtls_ssl_conf_ca_chain(&client_context.ssl_config, &client_context.ca_cert, NULL);
    mbedtls_ssl_conf_rng(&client_context.ssl_config, mbedtls_ctr_drbg_random, &client_context.ctr_drbg);

    ret = mbedtls_ssl_conf_own_cert(&client_context.ssl_config, &client_context.client_cert, &client_context.client_key);
    if (ret != 0) {
        return ret;
    }

    ret = mbedtls_ssl_setup(&client_context.ssl_context, &client_context.ssl_config);
    if (ret != 0) {
        return ret;
    }

    ret = mbedtls_ssl_set_hostname(&client_context.ssl_context, host);
    if (ret != 0) {
        return ret;
    }

    mbedtls_ssl_set_bio(&client_context.ssl_context, &client_context.net_context, mbedtls_net_send, mbedtls_net_recv, NULL);

    while ((ret = mbedtls_ssl_handshake(&client_context.ssl_context)) != 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            return ret;
        }
    }

    MQTTConnectInfo_t connect_info = {
        .cleanSession = true,
        .pClientIdentifier = client_id,
        .keepAliveSeconds = 60,
    };

    return MQTT_Connect(&client_context.mqtt_context, &connect_info, NULL, 30000, NULL);
}

int mqtt_disconnect()
{
    int ret = MQTT_Disconnect(&client_context.mqtt_context);
    mbedtls_ssl_close_notify(&client_context.ssl_context);
    mbedtls_net_free(&client_context.net_context);
    mbedtls_ssl_free(&client_context.ssl_context);
    mbedtls_ssl_config_free(&client_context.ssl_config);
    mbedtls_x509_crt_free(&client_context.ca_cert);
    mbedtls_x509_crt_free(&client_context.client_cert);
    mbedtls_pk_free(&client_context.client_key);
    mbedtls_entropy_free(&client_context.entropy);
    mbedtls_ctr_drbg_free(&client_context.ctr_drbg);
    return ret;
}

int mqtt_publish(const char* topic, const uint8_t* payload, size_t payload_length, uint8_t qos)
{
    MQTTPublishInfo_t publish_info = {
        .qos = qos,
        .pTopicName = topic,
        .topicNameLength = strlen(topic),
        .pPayload = payload,
        .payloadLength = payload_length
    };

    uint16_t packet_id = MQTT_GetPacketId(&client_context.mqtt_context);
    return MQTT_Publish(&client_context.mqtt_context, &publish_info, packet_id);
}

int mqtt_subscribe(const char* topic, uint8_t qos)
{
    MQTTSubscribeInfo_t subscribe_info = {
        .qos = qos,
        .pTopicFilter = topic,
        .topicFilterLength = strlen(topic)
    };

    uint16_t packet_id = MQTT_GetPacketId(&client_context.mqtt_context);
    return MQTT_Subscribe(&client_context.mqtt_context, &subscribe_info, 1, packet_id);
}

int mqtt_set_root_ca(const char* cert_pem)
{
    return mbedtls_x509_crt_parse(&client_context.ca_cert, (const unsigned char*)cert_pem, strlen(cert_pem) + 1);
}

int mqtt_set_client_cert(const char* cert_pem)
{
    return mbedtls_x509_crt_parse(&client_context.client_cert, (const unsigned char*)cert_pem, strlen(cert_pem) + 1);
}

int mqtt_set_client_key(const char* key_pem)
{
    return mbedtls_pk_parse_key(&client_context.client_key,
        (const unsigned char*)key_pem,
        strlen(key_pem) + 1,
        NULL, 0,
        mbedtls_ctr_drbg_random,
        &client_context.ctr_drbg);
}

int mqtt_process()
{
    return MQTT_ProcessLoop(&client_context.mqtt_context);
}
