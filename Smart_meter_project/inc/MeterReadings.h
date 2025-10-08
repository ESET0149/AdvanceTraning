#ifndef METER_READING_H
#define METER_READING_H
#include <stdint.h>
#include <time.h>
typedef struct {
    double kWh_import;
    double kWh_export;
    double kVAh;
    double kVArh_import;
    double kVArh_export;
 
    // Demand
    double max_demand_kw;
    double max_demand_kva;
    time_t max_demand_timestamp;
 
    // Power quality
    float voltage_avg;
    float voltage_min;
    float voltage_max;
 
    float current_avg;
    float current_peak;
 
    float power_factor_avg;
    float power_factor_min;
    float power_factor_max;
 
    float frequency;
 
    // Event flags
    uint8_t tamper_detected;
    uint8_t power_fail_count;
    uint8_t cover_open;
 } meter_reading_t;
 
// Global Instance
 
extern meter_readings_t g_meter_readings;
 
 
//API
 
void meter_readings_init(void);
void meter_readings_update_energy(double delta_kwh);
void meter_readings_update_quality(float voltage, float current, float pf, float freq);
 
 
 #endif