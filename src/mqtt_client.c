#include <stdio.h>
#include <stdbool.h>
#include <mosquitto.h>
#include "mqtt_client.h"

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
    int rc = mosquitto_connect(mosq, cfg->host, cfg->port, 60);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Failed to connect to %s:%d: %s\n", cfg->host, cfg->port, mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return NULL;
    }
    printf("Connected to %s:%d\n", cfg->host, cfg->port);
    return mosq;
}

void mqtt_stop(struct mosquitto *mosq){
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}