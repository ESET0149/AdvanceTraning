#include "data_refresh.h"
#include "storage.h"
#include "MeterReadings.h"
#include "Meter_Info.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAX_DAILY_ENTRIES 90
#define REPORT_TOPIC "meter/report/90days"

#ifndef REFRESH_LOG
#define REFRESH_LOG(fmt, ...) printf("[DATA_REFRESH] " fmt "\n", ##__VA_ARGS__)
#endif

// internal circular buffer to store ;last 90 days of data
static daily_data_t daily_store[MAX_DAYS];
static uint8_t entry_count = 0;
static uint8_t head_index = 0; //oldest entry


//helper function
//get current epoch timestamp
static unit32_t get_current_time(void){
    #ifdef HAL_EXISTS


    #else
           return (uint32_t)time(NULL);
    #endif


// serialize data into a byte buffer for encryption
static size_t serialize_report (unit8_t *out_buf, size_t max_len){
    //public function 

    void data_refresh_init(void){
        memset(daily_store, 0, sizeof(daily_store));
        entry_count = 0;
        head_index = 0;
        REFRESH_LOG("Intialized daily data store (max %d days)", MAX_DAYS);
        

        //add new daily entry 
        //if store is full -->remove the oldest entry, insert new
        void data_refresh_add_entry(const daily_data_t* entry){
            if (entry_count < MAX_DAYS){
                //store not full
                entry_count++;
                uint8_t insert_index = (head_index + entry_count) % MAX_DAYS;
                memcpy(&daily_store[insert_index], entry, sizeof(daily_data_t));

                REFRESH_LOG("Added new daily entry at index %d, total entries %d", insert_index, entry_count);
            } else if (entry_count >= MAX_DAYS){
                //store full, overwrite oldest
                memcpy(&daily_store[head_index], entry, sizeof(daily_data_t));
                head_index = (head_index + 1) % MAX_DAYS; //move head to next oldest
                REFRESH_LOG("Store full. Overwrote oldest entry. New head index %d", head_index);
            }
            //After update, complile report and send 
            data_refresh_send_report();
        }

    }
}
}