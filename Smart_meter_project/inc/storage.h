#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include "MeterReadings.h"
#include "Meter_Info.h"

//STORAGE ADDRESSES IN FLASH (ADJUST AS PER MCU MEMPORY MAP)
#define FLASH_BASE_ADDR 0x08060000U //example flash sector
#define STORAGE_SIZE    0x2000    // 8 KB ALLOCATED

//DATA BLOCKS
 typedef struct {
    meter_reading_t readings;
    unit32_t crc;
 } storage_readiongs_blocks_t;

 typedef struct {
    consumer_info_t consumer;
    meter_hardware_info_t hardware;
    uint32_t crc;
    } storage_info_block_t;

    //API
    void storage_init(void);
    int storage_save_readings(const meter_reading_t* readings); 
    int storage_load_readings(meter_reading_t* readings);
    int storage_save_info(const consumer_info_t* consumer, const meter_hardware_info_t* hardware);
    int storage_load_info(consumer_info_t* consumer, meter_hardware_info_t* hardware);

    //api
    uint32_t calculate_crc(const void* data, size_t length);
#endif // STORAGE_H
