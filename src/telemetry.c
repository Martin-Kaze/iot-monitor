#include "telemetry.h"
#include <cjson/cJSON.h>
#include <string.h>

/* Accepted value ranges (inclusive) */
#define BATTERY_MIN 0.0
#define BATTERY_MAX 100.0

#define TEMP_MIN -50.0
#define TEMP_MAX 150.0

#define HUMIDITY_MIN 0.0
#define HUMIDITY_MAX 100.0

#define TIMESTAMP_MIN 0.0
#define TIMESTAMP_MAX 4102444800.0

const char *telemetry_strerror(telemetry_result_t rc)
{
    switch (rc)
    {
    case TELEMETRY_OK:
        return "ok";
    case TELEMETRY_ERR_JSON:
        return "invalid JSON";
    case TELEMETRY_ERR_MISSING:
        return "missing required field";
    case TELEMETRY_ERR_TYPE:
        return "field has wrong type";
    case TELEMETRY_ERR_RANGE:
        return "value out of range";
    }
    return "unknown error";
}

/* Finds a numeric field, returns MISSING or TYPE if absent or not a number. */
static telemetry_result_t get_number(const cJSON *root, const char *name, double *value)
{
    cJSON *node = cJSON_GetObjectItemCaseSensitive(root, name);
    if (node == NULL)
    {
        return TELEMETRY_ERR_MISSING;
    }
    if (!cJSON_IsNumber(node))
    {
        return TELEMETRY_ERR_TYPE;
    }
    *value = node->valuedouble;
    return TELEMETRY_OK;
}

/* Finds a string field; *value points into the cJSON tree (valid until cJSON_Delete). */
static telemetry_result_t get_string(const cJSON *root, const char *name, const char **value)
{
    cJSON *node = cJSON_GetObjectItemCaseSensitive(root, name);
    if (node == NULL)
    {
        return TELEMETRY_ERR_MISSING;
    }
    if (!cJSON_IsString(node))
    {
        return TELEMETRY_ERR_TYPE;
    }
    *value = node->valuestring;
    return TELEMETRY_OK;
}

/* Validates all fields into *t; returns at the first error. Does not own root. */
static telemetry_result_t extract_fields(const cJSON *root, telemetry_t *t)
{
    const char *id;
    double value;
    telemetry_result_t rc;

    /* Setting Device id*/
    rc = get_string(root, "device_id", &id);
    if (rc != TELEMETRY_OK)
    {
        return rc;
    }
    size_t n = strlen(id);
    if (n == 0 || n >= DEVICE_ID_MAX)
    {
        return TELEMETRY_ERR_RANGE;
    }

    memcpy(t->device_id, id, n + 1);

    /* Setting temperature */
    rc = get_number(root, "temperature", &value);
    if (rc != TELEMETRY_OK)
    {
        return rc;
    }
    if (value < TEMP_MIN || value > TEMP_MAX)
    {
        return TELEMETRY_ERR_RANGE;
    }
    t->temperature = value;

    /* Setting humidity */
    rc = get_number(root, "humidity", &value);
    if (rc != TELEMETRY_OK)
    {
        return rc;
    }
    if (value < HUMIDITY_MIN || value > HUMIDITY_MAX)
    {
        return TELEMETRY_ERR_RANGE;
    }
    t->humidity = value;

    /* Setting battery */
    rc = get_number(root, "battery", &value);
    if (rc != TELEMETRY_OK)
    {
        return rc;
    }
    if (value < BATTERY_MIN || value > BATTERY_MAX)
    {
        return TELEMETRY_ERR_RANGE;
    }
    if (value != (int)value)
    {
        return TELEMETRY_ERR_TYPE;
    }
    t->battery = (int)value;

    /* Setting timestamp */
    rc = get_number(root, "timestamp", &value);
    if (rc != TELEMETRY_OK)
    {
        return rc;
    }
    if (value <= TIMESTAMP_MIN || value > TIMESTAMP_MAX)
    {
        return TELEMETRY_ERR_RANGE;
    }
    if (value != (long long)value)
    {
        return TELEMETRY_ERR_TYPE;
    }
    t->timestamp = (long long)value;

    return TELEMETRY_OK;
}

/* Owns the cJSON tree,  *out changes only on success. */
telemetry_result_t telemetry_parse(const char *payload, int len, telemetry_t *out)
{

    cJSON *root = cJSON_ParseWithLength(payload, len);
    telemetry_t tmp;

    if (root == NULL)
    {
        return TELEMETRY_ERR_JSON;
    }

    telemetry_result_t rc = extract_fields(root, &tmp);

    if (rc == TELEMETRY_OK)
    {
        *out = tmp;
    }

    cJSON_Delete(root);

    return rc;
}