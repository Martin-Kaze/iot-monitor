#ifndef TELEMETRY_H
#define TELEMETRY_H

#define DEVICE_ID_MAX 64

typedef struct telemetry
{
    char device_id[DEVICE_ID_MAX]; /* Device name */
    double temperature;            /* Degrees Celsius */
    double humidity;               /* Relative humidity, 0-100 % */
    int battery;                   /* Battery level, 0-100 % */
    long long timestamp;           /* Unix time */
} telemetry_t;

typedef enum
{
    TELEMETRY_OK,          /* Parsed and valid */
    TELEMETRY_ERR_JSON,    /* Payload is not valid JSON */
    TELEMETRY_ERR_MISSING, /* Required field is absent */
    TELEMETRY_ERR_TYPE,    /* Field has a wrong type */
    TELEMETRY_ERR_RANGE,   /* Value is out of range */
} telemetry_result_t;

/* Parses and validates a JSON telemetry payload of len bytes */
telemetry_result_t telemetry_parse(const char *payload, int len, telemetry_t *out);

#endif