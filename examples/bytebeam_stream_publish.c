#ifdef __has_include
    #if __has_include("device_config.h")
        #include "device_config.h"
    #else
        #include "template_device_config.h"
    #endif
#else
    #include "template_device_config.h"
#endif
#include "mqtt_client.h"
#include "transport.h"
#include <stdio.h>
#include <string.h>

int main()
{
    mqtt_init();

    const char* broker = BROKER;
    int port = PORT;
    const char* device_id = DEVICE_ID;
    const char* project_id = PROJECT_ID;
    const char* root_ca = CA_CERTIFICATE;
    const char* client_cert = DEVICE_CERTIFICATE;
    const char* private_key = DEVICE_PRIVATE_KEY;

    int ret = mqtt_connect(broker, port, device_id, root_ca, client_cert, private_key);
    if (ret != 0) {
        printf("Failed to connect to MQTT broker. Error code: %d\n", ret);
        return -1;
    }

    const char* stream_name = "test_stream";
    char topic[256];
    snprintf(topic, sizeof(topic), "/tenants/%s/devices/%s/events/%s/jsonarray", project_id, device_id, stream_name);
    const char* payload = "[{\"temperature\": 25.5, \"humidity\": 60, \"timestamp\": 1727516135000, \"sequence\": 1}]";
    ret = mqtt_publish(topic, (const uint8_t*)payload, strlen(payload), 0);
    if (ret != 0) {
        printf("Failed to publish message. Error code: %d\n", ret);
    } else {
        printf("Message published successfully\n");
    }

    mqtt_disconnect();
    return 0;
}
