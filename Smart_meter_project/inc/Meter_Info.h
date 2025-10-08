#ifndef METER_INFO_H
#define METER_INFO_H
#include<stdint.h>
// Consumer Details
 
typedef struct {
    char consumer_id[16];
    char consumer_name[32];
    char tariff_plan[16];
    char address[64];
} consumer_info_t;
 
// Meter Hardware Details
 
typedef struct {
    char meter_serial[20];
    char firmware_version[12];
    char hardware_version[12];
    float voltage_calibration;
    float current_calibration;
    float power_calibration;
} meter_hardware_info_t;
 
// Global Instances
 
extern consumer_info_t g_consumer_info;
extern meter_hardware_info_t g_meter_hw_info;
 
//API Call
void meter_info_init(void);
 
#endif
 