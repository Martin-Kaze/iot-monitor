/*
 * iot_monitor - MQTT IoT device monitor
 * Subscribes to device telemetry, tracks devices, and publishes alerts.
 */

#include "config.h"
#include <stdio.h>
#include "mqtt_client.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    /* Flush stdout at every newline, even when piped or run as a service */
    setvbuf(stdout, NULL, _IOLBF, 0);

    config_t cfg;

    config_result_t result = config_parse(argc, argv, &cfg);

    if (result == CONFIG_EXIT)
        return 0;
    if (result == CONFIG_ERROR)
        return 1;

    struct mosquitto *mosq = mqtt_start(&cfg);
    if( mosq == NULL){
        return 1;
    }

    /* Wait while the network thread handles messages (Ctrl+C to quit for now) */
    while (1){
         sleep(1);
    }
   

    mqtt_stop(mosq);

}