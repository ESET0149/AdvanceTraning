#include "inc\Meter_Info.h"
#include <string.h>

//inistialize with default (in real use, load from flash/EEPROM)
consumer_info_t g_consumer_info;
meter_hardware_info_t g_meter_hw_info;

void meter_info_init(void){
    //consumer info (example)
    strcpy(g_consumer_info.consumer_id, "CUST12345");
    strcpy(g_consumer_info.consumer_name, "John Doe");
    strcpy(g_consumer_info.tariff_plan, "Domestic");
    strcpy(g_consumer_info.address, "123 Main st, indore");

    //hardware info (example)
    strcpy(g_meter_hw_info.meter_serial, "STM32L1XX-001");
    strcpy(g_meter_hw_info.firmware_version,"V1.0.0");
    strcpy(g_meter_hw_info.hardware_version,"RevA");
    
    g_meter_hw_info.voltage_calibration = 1.002f;
    g_meter_hw_info.current_calibration = 0.998f;
    g_meter_hw_info.power_calibration = 1.001f;

    
}