#include <stdio.h>
#include <mosquitto.h>
#include <cjson/cJSON.h>

int main( void){

    int major, minor, rev;
    mosquitto_lib_version(&major, &minor, &rev);
    printf("The: %d.%d.%d, libmosquitto Version \n", major, minor, rev);
    printf("The: %s, cJSON Version \n", cJSON_Version());
    return 0;
}