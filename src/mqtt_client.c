#include "mqtt_client.h"
#include "mqtt_client_port.h"
#include "transport.h"
#include <stdio.h>
#include <string.h>

static MQTTClientContext client_context;

MQTTClientContext* get_mqtt_client_context()
{
    return &client_context;
}

void debug(void* ctx, int level, const char* file, int line, const char* str)
{
    printf("%s:%04d: %s", file, line, str);
}

static int check_private_key(mbedtls_pk_context* client_key)
{
    int ret = -1;
    if (mbedtls_pk_can_do(client_key, MBEDTLS_PK_RSA)) {
        ret = 0;
    } else if (mbedtls_pk_can_do(client_key, MBEDTLS_PK_ECDSA)) {
        ret = 0;
    }
    return ret;
}

void mqtt_event_callback(MQTTContext_t* pMqttContext, MQTTPacketInfo_t* pPacketInfo, MQTTDeserializedInfo_t* pDeserializedInfo)
{
    // TODO: Implement MQTT event handling
}

int mqtt_init()
{
    int ret;

    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t fixed_buffer = {
        .pBuffer = client_context.fixed_buffer,
        .size = MQTT_BUFFER_SIZE
    };

    transport.pNetworkContext = NULL;
    transport.send = network_send;
    transport.recv = network_recv;

    ret = MQTT_Init(&client_context.mqtt_context, &transport, get_current_time, mqtt_event_callback, &fixed_buffer);
    if (ret != 0) {
        printf("MQTT_Init failed with error: %d\n", ret);
        return ret;
    }

    printf("MQTT_Init success\n");
}

int mqtt_connect(const char* host, int port, const char* client_id, const char* root_ca, const char* client_cert, const char* private_key)
{
    int ret;

    char port_str[6];
    sprintf(port_str, "%d", port);

    ret = port_transport_open(&client_context.transport_ctx, host, port_str);
    if (ret != 0) {
        return ret;
    }

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
        printf("mbedtls_ctr_drbg_seed returned: %d\n", ret);
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    ret = mbedtls_net_connect(&client_context.net_context, host, port_str, MBEDTLS_NET_PROTO_TCP);
    if (ret != 0) {
        printf("mbedtls_net_connect returned: %d\n", ret);
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    ret = mbedtls_ssl_config_defaults(&client_context.ssl_config, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0) {
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    mbedtls_ssl_conf_dbg(&client_context.ssl_config, debug, NULL);
    mbedtls_debug_set_threshold(4);

    mbedtls_ssl_conf_min_version(&client_context.ssl_config, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3);
    mbedtls_ssl_conf_max_version(&client_context.ssl_config, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3);

    ret = mbedtls_x509_crt_parse(&client_context.ca_cert, (const unsigned char*)root_ca, strlen(root_ca) + 1);
    if (ret != 0) {
        printf("Failed to parse CA certificate: %d\n", ret);
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    ret = mbedtls_x509_crt_parse(&client_context.client_cert, (const unsigned char*)client_cert, strlen(client_cert) + 1);
    if (ret != 0) {
        printf("Failed to parse client certificate: %d\n", ret);
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    ret = mbedtls_pk_parse_key(&client_context.client_key, (const unsigned char*)private_key, strlen(private_key) + 1, NULL, 0, mbedtls_ctr_drbg_random, &client_context.ctr_drbg);
    if (ret != 0) {
        printf("Failed to parse client private key: %d\n", ret);
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    ret = check_private_key(&client_context.client_key);
    if (ret != 0) {
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    mbedtls_ssl_conf_authmode(&client_context.ssl_config, MBEDTLS_SSL_VERIFY_REQUIRED);
    mbedtls_ssl_conf_ca_chain(&client_context.ssl_config, &client_context.ca_cert, NULL);
    mbedtls_ssl_conf_rng(&client_context.ssl_config, mbedtls_ctr_drbg_random, &client_context.ctr_drbg);

    ret = mbedtls_ssl_conf_own_cert(&client_context.ssl_config, &client_context.client_cert, &client_context.client_key);
    if (ret != 0) {
        printf("mbedtls_ssl_conf_own_cert returned: %d\n", ret);
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    ret = mbedtls_ssl_setup(&client_context.ssl_context, &client_context.ssl_config);
    if (ret != 0) {
        printf("mbedtls_ssl_setup returned: %d\n", ret);
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    ret = mbedtls_ssl_set_hostname(&client_context.ssl_context, host);
    if (ret != 0) {
        printf("mbedtls_ssl_set_hostname returned: %d\n", ret);
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    mbedtls_ssl_set_bio(&client_context.ssl_context, &client_context.net_context, port_transport_send, port_transport_recv, NULL);

    while ((ret = mbedtls_ssl_handshake(&client_context.ssl_context)) != 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {

            char error_buf[100];
            mbedtls_strerror(ret, error_buf, sizeof(error_buf));
            printf("mbedtls_ssl_handshake failed with error: %x - %s\n", ret, error_buf);

            port_transport_close(client_context.transport_ctx);
            return ret;
        }
    }

    printf("mbedtls_ssl_handshake success\n");

    MQTTConnectInfo_t connect_info = {
        .cleanSession = true,
        .pClientIdentifier = client_id,
        .clientIdentifierLength = strlen(client_id),
        .keepAliveSeconds = 60,
    };

    bool session_present = false;

    if (&client_context.mqtt_context == NULL) {
        printf("MQTT_Connect failed: MQTTContext is NULL\n");
        return -1;
    }

    ret = MQTT_Connect(&client_context.mqtt_context, &connect_info, NULL, 30000, &session_present);
    if (ret != 0) {
        printf("MQTT_Connect failed with error: %d\n", ret);
        if (ret == MQTTBadParameter) {
            printf("Bad parameter in MQTT_Connect\n");
        } else if (ret == MQTTNoMemory) {
            printf("Insufficient memory for MQTT_Connect\n");
        } else if (ret == MQTTSendFailed) {
            printf("Send operation failed in MQTT_Connect\n");
        } else if (ret == MQTTRecvFailed) {
            printf("Receive operation failed in MQTT_Connect\n");
        } else if (ret == MQTTBadResponse) {
            printf("Invalid response received in MQTT_Connect\n");
        } else if (ret == MQTTServerRefused) {
            printf("Server refused the connection in MQTT_Connect\n");
        } else {
            printf("Unexpected error in MQTT_Connect\n");
        }
        port_transport_close(client_context.transport_ctx);
        return ret;
    }

    printf("MQTT_Connect success\n");
    return 0;
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

    port_transport_close(client_context.transport_ctx);
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

int mqtt_process()
{
    return MQTT_ProcessLoop(&client_context.mqtt_context);
}
