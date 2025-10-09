#include "nvm.h"
#include "inc/storage.h"
#include <string.h>         
#include <stdio.h>
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_rcc.h"

#define NVM_BASE_ADDR 0x08080000 // Example flash sector

void nvm_init(void) {
    // Initialize NVM if needed
}
void NVM_save_gps_data(const gps_data_t* data) {
    // Simulate saving GPS data to flash by returning success
    // In real implementation, write to flash memory at NVM_BASE_ADDR
    HAL_FLASH_Unlock();
    unit_32_t *flash_ptr = (uint32_t *)NVM_BASE_ADDR;
    HAL_FLASH_Program(TYPEPROGRAM_WORD, (uint32_t)flash_ptr, *((uint32_t*)&data->latitude));
    HAL_FLASH_Program(TYPEPROGRAM_WORD, (uint32_t)(flash_ptr + 1), *((uint32_t*)&data->longitude));
    HAL_FLASH_Lock();
    return;
}
