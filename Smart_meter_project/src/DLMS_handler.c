#include "DLMS_handler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/storage.h"
#include "inc/MeterReadings.h"
#include "inc/Meter_Info.h"
#include "inc/app_log.h"
#include "stm32f3xx_hal.h" // Assuming STM32 HAL is used
#include "stm32f3xx_hal_rcc.h"
#include "stm32f3xx_hal_rtc.h"
#include <time.h>

// Initialize DLMS Handler
void dlms_Handler_Init(void){
    // Initialize any required structures or state 
    APP_LOG("DLMS Handler Initialized");
}
//process DLMS Request
int dlms_process_request(dlms_request_t* req,dlms_response_t*resp){
    memset(resp,0,sizeof(dlms_response_t));

    switch(req->type) {
        case DLMS_GET:
            //handle GET
            resp->type = DLMS_GET;
            resp->obis_code = req->obis_code;
            //dummy payload
            resp->payload = (uint8_t*)malloc(16);
            if (resp->payload){
                snprintf((char*)resp->payload,16,"Value1234");
                resp->length = strlen((char*)resp->payload);
                encrypt_response(resp->payload,resp->length);
            }
            break;
        case DLMS_SET:
            //handle SET
            resp->type = DLMS_SET;
            resp->obis_code = req->obis_code;
            //assume success
            resp->payload = (uint8_t*)malloc(8);
            if (resp->payload){
                snprintf((char*)resp->payload,8,"OK");
                resp->length = strlen((char*)resp->payload);
                encrypt_response(resp->payload,resp->length);
            }
            break;
            case DLMS_WRITE:
            RESP->type = DLMS_WRITE;
            resp->obis_code = req->obis_code;       
            //assume success
            resp->payload = (uint8_t*)malloc(8);
            if (resp->payload){
                snprintf((char*)resp->payload,8,"WR_OK");
                resp->length = strlen((char*)resp->payload);
                encrypt_response(resp->payload,resp->length);
            }       
            break;

        case DLMS_ACTION:
            //handle ACTION
            resp->type = DLMS_ACTION;
            resp->obis_code = req->obis_code;
            //assume success
            resp->payload = (uint8_t*)malloc(8);
            if (resp->payload){
                snprintf((char*)resp->payload,8,"DONE");
                resp->length = strlen((char*)resp->payload);
                encrypt_response(resp->payload,resp->length);
            }
            break;
        default:
            //unknown request
            resp->type = DLMS_UNKNOWN;
            resp->obis_code = req->obis_code;
            resp->payload = (uint8_t*)malloc(16);
            if (resp->payload){
                snprintf((char*)resp->payload,16,"ERR_UNKNOWN");
                resp->length = strlen((char*)resp->payload);
                encrypt_response(resp->payload,resp->length);
            }
            break;
    }
}