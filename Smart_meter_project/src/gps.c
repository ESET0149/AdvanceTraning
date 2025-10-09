#include "gps.h"
#include "hal.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    
#include <math.h>
#include <time.h>
#include "STM32F373RC.h"
#include "inc/gps.h"
#include "inc/Tamper_monitor.h" // Include the tamper monitor header    
#include "inc/Power_management.h" // Include the power management header
#include "stm3211xx_hal.h" // Include the STM32 HAL header

//simulated UART for GPS (replace with real UART handle)

extern UART_HandleTypeDef huart2; // Assume huart2 is configured for GPS
void gps_init(void) {
    // Initialize UART for GPS communication
  //  HAL_UART_Transmit(&huart2, (uint8_t *)"GPS INIT\r\n", 10,100); // Power on GPS
    GPS_LOG("GPS UART Initialized");T
}
bool gps_get_location(gps_data_t* data) {
    char gps_buffer[128] = {0};

    // Simulated NMEA sentence (replace with real UART read and parsing)
    strcpy(gps_buffer, "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");

    //basic parsing (simulate success)
    if (strlen(gps_buffer) > 0) {
        data->latitude = 48.1173f; // Example latitude
        data->longitude = 11.5167f; // Example longitude

        return true;
    }
    return false;

}