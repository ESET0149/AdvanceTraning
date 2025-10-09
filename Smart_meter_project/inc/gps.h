#ifndef GPS_H
#define GPS_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float latitude; 
    float longitude;
} gps_data_t;
void gps_init(void);
bool gps_get_location(gps_data_t* data);
#endif