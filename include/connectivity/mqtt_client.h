#ifndef IOT_MQTT_CLIENT_H
#define IOT_MQTT_CLIENT_H

#include "core_mqtt.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/entropy.h"
#include "mbedtls/error.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/pk.h"
#include "mbedtls/ssl.h"

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
    void* transport_ctx;
} MQTTClientContext;

/**
 * @brief Initialize the MQTT client
 *
 * @return int 0 on success, negative value on error
 */
int mqtt_init();

/**
 * @brief Connect to the MQTT broker
 *
 * @param host Hostname of the MQTT broker
 * @param port Port number of the MQTT broker
 * @param client_id Client identifier for the MQTT connection
 * @param root_ca Root CA certificate
 * @param client_cert Client certificate
 * @param private_key Client private key
 * @return int 0 on success, negative value on error
 */
int mqtt_connect(const char* host, int port, const char* client_id, const char* root_ca, const char* client_cert, const char* private_key);

/**
 * @brief Disconnect from the MQTT broker
 *
 * @return int 0 on success, negative value on error
 */
int mqtt_disconnect();

/**
 * @brief Publish a message to an MQTT topic
 *
 * @param topic The topic to publish to
 * @param payload The message payload
 * @param payload_length Length of the payload
 * @param qos Quality of Service level (0, 1, or 2)
 * @return int 0 on success, negative value on error
 */
int mqtt_publish(const char* topic, const uint8_t* payload, size_t payload_length, uint8_t qos);

/**
 * @brief Subscribe to an MQTT topic
 *
 * @param topic The topic to subscribe to
 * @param qos Quality of Service level (0, 1, or 2)
 * @return int 0 on success, negative value on error
 */
int mqtt_subscribe(const char* topic, uint8_t qos);

/**
 * @brief Set the root CA certificate for SSL/TLS connection
 *
 * @param cert_pem PEM-formatted root CA certificate
 * @return int 0 on success, negative value on error
 */
int mqtt_set_root_ca(const char* cert_pem);

/**
 * @brief Set the client certificate for SSL/TLS connection
 *
 * @param cert_pem PEM-formatted client certificate
 * @return int 0 on success, negative value on error
 */
int mqtt_set_client_cert(const char* cert_pem);

/**
 * @brief Set the client private key for SSL/TLS connection
 *
 * @param key_pem PEM-formatted client private key
 * @return int 0 on success, negative value on error
 */
int mqtt_set_client_key(const char* key_pem);

/**
 * @brief Process MQTT messages and maintain the connection
 *
 * @return int 0 on success, negative value on error
 */
int mqtt_process();

/**
 * @brief Portable transport send function for CoreMQTT IoT communications
 *
 * This is a portable implementation that can be used across different platforms.
 * It sends data over the network using the configured transport layer for
 * CoreMQTT communications.
 *
 * @param pNetworkContext Network context containing connection info
 * @param pBuffer Buffer containing data to send
 * @param bytesToSend Number of bytes to send
 * @return int Number of bytes sent on success, negative value on error
 */
int coreMQTT_iot_transport_send(NetworkContext_t* pNetworkContext, const void* pBuffer, size_t bytesToSend);

/**
 * @brief Portable transport receive function for CoreMQTT IoT communications
 *
 * This is a portable implementation that can be used across different platforms.
 * It receives data from the network using the configured transport layer for
 * CoreMQTT communications.
 *
 * @param pNetworkContext Network context containing connection info
 * @param pBuffer Buffer to store received data
 * @param bytesToRecv Maximum number of bytes to receive
 * @return int Number of bytes received on success, negative value on error
 */
int coreMQTT_iot_transport_recv(NetworkContext_t* pNetworkContext, void* pBuffer, size_t bytesToRecv);

#endif /* IOT_MQTT_CLIENT_H */
