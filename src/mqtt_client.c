#include <stdio.h>
#include <stdbool.h>
#include <mosquitto.h>
#include "mqtt_client.h"

#define KEEPALIVE_SECONDS 60
#define TELEMETRY_TOPIC "iot/devices/+/telemetry"

/* Called by the network thread when the broker answers CONNECT */
static void on_connect(struct mosquitto *mosq, void *userdata, int rc)
{

    (void)userdata;
    if (rc != 0)
    {
        fprintf(stderr, "Connection rejected : %s\n", mosquitto_connack_string(rc));
        return;
    }
    int sub_rc = mosquitto_subscribe(mosq, NULL, TELEMETRY_TOPIC, 0);
    if (sub_rc != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Failed subscription : %s\n", mosquitto_strerror(sub_rc));
    }
    else
    {
        printf("Subscribed to %s\n", TELEMETRY_TOPIC);
    }
}

/* Called by the network thread for every received message.*/
static void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg)
{
    (void)mosq;
    (void)userdata;
    printf("%s: %.*s\n", msg->topic, msg->payloadlen, (char *)msg->payload);
}

struct mosquitto *mqtt_start(const config_t *cfg)
{

    if (mosquitto_lib_init() != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Failed to initialize libmosquitto\n");
        return NULL;
    }
    /* NULL id = random client id; requires clean_session = true */
    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);
    if (mosq == NULL)
    {
        fprintf(stderr, "Failed to create MQTT client\n");
        mosquitto_lib_cleanup();
        return NULL;
    }

    /* Register callbacks before connecting */
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    int rc = mosquitto_connect(mosq, cfg->host, cfg->port, KEEPALIVE_SECONDS);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Failed to connect to %s:%d: %s\n", cfg->host, cfg->port, mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return NULL;
    }
    printf("Connected to %s:%d\n", cfg->host, cfg->port);

    /* Network thread: reads incoming packets, sends pings, calls the callbacks */
    rc = mosquitto_loop_start(mosq);

    if (rc != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Failed to start loop, error: %s \n", mosquitto_strerror(rc));
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return NULL;
    }

    return mosq;
}


void mqtt_stop(struct mosquitto *mosq)
{
    mosquitto_disconnect(mosq);
    mosquitto_loop_stop(mosq, false);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}