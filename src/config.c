#include "config.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Parses text as a port, returns 0 and stores it in *out, or -1 if invalid. */
static int parse_port(const char *text, int *out)
{
    char *end;
    long value = strtol(text, &end, 10);
    /* check if string  or port is invalid */
    if (end == text || *end != '\0' || value < 1 || value > 65535)
    {
        return -1;
    }
    *out = (int)value;
    return 0;
}

/* Prints command-line help to out */
static void print_usage(FILE *out, const char *prog)
{
    fprintf(out, "Usage: %s [-H HOST] [-P PORT] [-o FILE] [-h]\n", prog);
    fprintf(out, "\n");
    fprintf(out, "Monitor IoT device telemetry over MQTT and publish alerts.\n");
    fprintf(out, "\n");
    fprintf(out, "Options:\n");
    fprintf(out, "  -H HOST   MQTT broker hostname or IP address (default: localhost)\n");
    fprintf(out, "  -P PORT   MQTT broker port, 1-65535 (default: 1883)\n");
    fprintf(out, "  -o FILE   Also write received messages and alerts to FILE\n");
    fprintf(out, "  -h        Show this help and exit\n");
    fprintf(out, "\n");
    fprintf(out, "Example:\n");
    fprintf(out, "  %s -H 192.168.1.100 -P 1883 -o monitor.log\n", prog);
}

 /* fill the stuct with argv or just sets as default */
config_result_t config_parse(int argc, char *argv[], config_t *cfg)
{
    /* set the defaults */
    cfg->host = "localhost";
    cfg->port = 1883;
    cfg->log_path = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "H:P:o:h")) != -1)
    {
        switch (opt)
        {
        case 'H':
            cfg->host = optarg;
            break;
        case 'P':
            if (parse_port(optarg, &cfg->port) != 0)
            {
                fprintf(stderr, "%s: invalid port '%s'\n", argv[0], optarg);
                print_usage(stderr, argv[0]);
                return CONFIG_ERROR;
            }
            break;
        case 'o':
            cfg->log_path = optarg;
            break;
        case 'h':
            print_usage(stdout, argv[0]);
             return CONFIG_EXIT;

        default:
            print_usage(stderr, argv[0]);
            return CONFIG_ERROR;
        }
    }

    return CONFIG_OK;
}