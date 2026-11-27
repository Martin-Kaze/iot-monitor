#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "config.h"

struct mosquitto;
/* Initializes libmosquitto and connects to cfg->host:cfg->port. Returns the client, or NULL on failure*/
struct mosquitto *mqtt_start(const config_t *cfg);
/* Disconnects and frees everything created by mqtt_start*/
void mqtt_stop(struct mosquitto *mosq);



#endif