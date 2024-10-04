#ifdef __has_include
#if __has_include("device_config.h")
#include "device_config.h"
#else
#include "template_device_config.h"
#endif
#else
#include "template_device_config.h"
#endif
#include "clock.h"
#include "mqtt_client.h"
#include "bytebeam/stream.h"
#include "transport.h"
#include <stdio.h>
#include <string.h>

int main()
{
    printf("Starting Bytebeam Stream Publish Example\n");
    
    /* Initialize the MQTT client */
    mqtt_init();

    const char* broker = BROKER;
    int port = PORT;
    const char* device_id = DEVICE_ID;
    const char* project_id = PROJECT_ID;
    const char* root_ca = CA_CERTIFICATE;
    const char* client_cert = DEVICE_CERTIFICATE;
    const char* private_key = DEVICE_PRIVATE_KEY;

    /* Connect to the MQTT broker */
    int ret = mqtt_connect(broker, port, device_id, root_ca, client_cert, private_key);
    if (ret != 0) {
        printf("Failed to connect to MQTT broker. Error code: %d\n", ret);
        return -1;
    }

    /* Create a stream */
    stream_t* test_stream = stream_create("test_stream");
    stream_add_float32(test_stream, "temperature", 18.0);
    stream_add_uint8(test_stream, "humidity", 60);
    stream_add_timestamp_ms(test_stream, "timestamp", port_clock_get_timestamp_ms());
    stream_add_uint32(test_stream, "sequence", 1);
    sprintf(test_stream->topic, "/tenants/%s/devices/%s/events/%s/jsonarray", project_id, device_id, "test_stream");

    /* Publish the stream */
    char payload[256];
    snprintf(payload, sizeof(payload), "[%s]", stream_print(test_stream));
    printf("Payload: %s\n", payload);
    printf("Topic: %s\n", test_stream->topic);

    ret = mqtt_publish(test_stream->topic, (const uint8_t*)payload, strlen(payload), 0);
    if (ret != 0) {
        printf("Failed to publish message. Error code: %d\n", ret);
    } else {
        printf("Message published successfully\n");
    }

    /* Disconnect from the MQTT broker */
    mqtt_disconnect();
    return 0;
}
