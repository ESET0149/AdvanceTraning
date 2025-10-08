#include "inc\storage.h"
 
#define READINGS_ADDR FLASH_BASE_ADDR
#define INFO_ADDR (FLASH_BASE_ADDR + 0X1000)
void storage_init(void) {
    // Initialize storage if needed
}
int storage_save_readings(const meter_readings_t* data) {
    // Simulate saving to flash by returning success
    storage_readings_block_t block;
    memcpy(&block.readings, data, sizeof(meter_readings_t));
    block.crc32 = calculate_crc32(&block.readings, sizeof(meter_readings_t));
    return flash_write(READINGS_ADDR, (uint8_t*)&block, sizeof(block));
   
   
}
int storage_load_readings(meter_readings_t* data) {
    // Simulate loading from flash by returning success
    storage_readings_block_t *block = (storage_readings_block_t *)READINGS_ADDR;
    uint32_t crc = calculate_crc32(&block->readings, sizeof(meter_readings_t));
    if (crc == block->crc) {
        memcpy(data, &block->readings, sizeof(meter_readings_t));
        return 0; // Success
    }
    return -1; // CRC mismatch
}
int storage_save_info(const consumer_info_t* cinfo, const meter_hardware_info_t* hinfo) {
    // Simulate saving to flash by returning success
    return 0; // Success
}
int storage_load_info(consumer_info_t* cinfo, meter_hardware_info_t* hinfo) {
    // Simulate loading from flash by returning success
    return 0; // Success
}
uint32_t calculate_crc32(const void* data, size_t length) {
    // Simple CRC32 calculation (placeholder)
    uint32_t crc = 0xFFFFFFFF;
    const uint8_t* byte_data = (const uint8_t*)data;
    for (size_t i = 0; i < length; i++) {
        crc ^= byte_data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc;
}
 
 