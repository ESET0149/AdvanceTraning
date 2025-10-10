#ifndef DLMS_HANDLER_H
#define DLMS_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

//DLMS Request types
typedef enum {
    DLMS_GET ,
    DLMS_SET ,
    DLMS_ACTION,
    DLMS_READ,
    DLMS_WRITE,
    DLMS_UNKNOWN
} dlms_request_type_t;
//DLMS Request structure
typedef struct {
    dlms_request_type_t type;
    uint32_t obis_code;
    uint8_t *payload;
    uint16_t length; // Pointer to data for SET/WRITE
} dlms_response_t;

//API

void dlms_Handler_Init(void);
int dlms_process_request(const dlms_request_t* request, dlms_response_t* response);
bool DLMS_DECRYPTRequest(const uint8_t *input, size_t in_len, uint8_t *output, size_t *out_len);
void encrypt_response(uint8_t *data, uint16_t len);
void decrypt_response(uint8_t *data, uint16_t len);
void DLMS_ProcessAPDU(const unit8_t *apdu, size_t len);
void dlms_receive_handler(const uint8_t *payload, size_t len);
void send_response(uint8_t *data, size_t len);
bool DLMS_DecryptRequest(const uint8_t *input, size_t input_len, uint8_t *output, size_t *max_output_len);
#endif
