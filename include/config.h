#ifndef CONFIG_H
#define CONFIG_H

/* Runtime settings: defaults, overridden by command-line flags. */
typedef struct config
{
    const char *host;     /* MQTT broker hostname or IP */
    int port;             /* MQTT broker port (1-65535) */
    const char *log_path; /* Log file path, or NULL if logging is off */
} config_t;

typedef enum {
    CONFIG_OK,     /* settings parsed, keep running */
    CONFIG_EXIT,   /* help was printed, exit successfully */
    CONFIG_ERROR   /* invalid arguments, exit with failure */
} config_result_t;

config_result_t config_parse(int argc, char *argv[], config_t *cfg);

#endif