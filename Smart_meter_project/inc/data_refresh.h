#ifndef DATA_REFRESH_H
#define DATA_REFRESH_H

#include "MeterReadings.h"
#include "Meter_Info.h"
//initialize refresh system
void data_refresh_init(void);

//called whenever a new daily entry is available
void data_refresh_add_entry(const daily_data_t* entry);

//compile 90 days 
void data_refresh_send_report(void);

#endif // DATA_REFRESH_H 