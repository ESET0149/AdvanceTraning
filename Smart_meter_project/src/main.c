#include "data_refresh.h"
#include "storage.h"
#include <string.h> 
#include "MeterReadings.h"
#include "Meter_Info.h"
#include "STM32F373RC.h"
#include <stdio.h>
#include <time.h>
#include "hal.h"


int main(void) {
    hal_init();
     // Initialize HAL library
    system_clock_config(); // Configure the system clock

    data_refresh_init(); // Initialize data refresh module
    storage_init(); // Initialize storage module

    while (1) {
        hal_delay(1000); // Simulate a delay
        // Simulate adding a new daily entry        
        daily_data_t new_entry;
        memset(&new_entry, 0, sizeof(new_entry));
        new_entry.kWh_import = 5.0; // Example data
        new_entry.kWh_export = 1.0; // Example data 
        new_entry.timestamp = (uint32_t)time(NULL); // Current timestamp
        data_refresh_add_entry(&new_entry);
        // In a real application, other tasks would be performed here
        // Main loop
    }

    return 0;
}