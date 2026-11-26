/*
 * iot_monitor - MQTT IoT device monitor
 * Subscribes to device telemetry, tracks devices, and publishes alerts.
 */
#include <stdio.h>
#include <mosquitto.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include <stdlib.h>

/* Defaults from the task spec, can be overridden by command-line flags. */
typedef struct config
{
    const char *host;     /* MQTT broker hostname or IP */
    int port;             /* MQTT broker port (1-65535) */
    const char *log_path; /* Log file path */
} config_t;

int main(int argc, char *argv[])
{

    config_t cfg = {
        .host = "localhost",
        .port = 1883,
        .log_path = NULL,
    };

    int opt;
    while ((opt = getopt(argc, argv, "H:P:o:h")) != -1)
    {
        switch (opt)
        {
        case 'H':
            cfg.host = optarg;
            break;
        case 'P':
            cfg.port = atoi(optarg);
            break;
        case 'o':
            cfg.log_path = optarg;
            break;
        case 'h':
            printf("Display help info: .....\n");
            return 0;

        default:
            fprintf(stderr, "Error, no flag exists\n");
            return 1;
        }
    }

  
    const char *log_text = (cfg.log_path != NULL) ? cfg.log_path : "(none)";
    printf("Host: %s, Port: %i, Log Path: %s \n", cfg.host, cfg.port, log_text);
    return 0;
}