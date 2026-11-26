/*
 * iot_monitor - MQTT IoT device monitor
 * Subscribes to device telemetry, tracks devices, and publishes alerts.
 */

#include "config.h"
#include <stdio.h>

int main(int argc, char *argv[])
{

    config_t cfg;

    config_result_t result = config_parse(argc, argv, &cfg);

    if (result == CONFIG_EXIT)
        return 0;
    if (result == CONFIG_ERROR)
        return 1;

    const char *log_text = (cfg.log_path != NULL) ? cfg.log_path : "(none)";
    printf("Host: %s, Port: %i, Log Path: %s \n", cfg.host, cfg.port, log_text);
}