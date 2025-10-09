#ifndef NVM_H
#define NVM_H

#include <stdint.h>
#include "gps.h"

void nvm_init(void);
void nvm_save_gps_data(const gps_data_t* data);
#endif // NVM_H