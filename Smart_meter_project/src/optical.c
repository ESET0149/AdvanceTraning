#include "optical.h"
#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h" // Assuming STM32 HAL is used
 
#define OPTICAL_BAUDRATE 9600
#define OPTICAL_TIMEOUT_MS 1000
#define OPTICAL_BUFFER_SIZE 256
static UART_HandleTypeDef huart_optical;
static uint8_t rx_buffer[OPTICAL_BUFFER_SIZE];
static size_t rx_index = 0;
static bool probe_connected = false;
 
void optical_init(void) {
    // Initialize UART for optical communication
    huart_optical.Instance = USART2; // Example: using USART2
    huart_optical.Init.BaudRate = OPTICAL_BAUDRATE;
    huart_optical.Init.WordLength = UART_WORDLENGTH_8B;
    huart_optical.Init.StopBits = UART_STOPBITS_1;
    huart_optical.Init.Parity = UART_PARITY_NONE;
    huart_optical.Init.Mode = UART_MODE_TX_RX;
    huart_optical.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart_optical.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart_optical);
   
    // Initialize GPIO for probe detect pin (example)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0; // Example pin
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
void optical_task(void *argument) {
    // Main task loop for optical communication
    while (1) {
        if (optical_probe_connected()) {
            // Perform handshake and data exchange
            // Example: send a snapshot every 10 seconds
            optical_send_snapshot();
            HAL_Delay(10000);
        } else {
            HAL_Delay(1000); // Check every second if probe is connected
        }
    }
}
 
bool optical_probe_connected(void) {
    // Read the probe detect pin
    probe_connected = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET);
    return probe_connected;
}
int optical_send_snapshot(void) {
    if (!probe_connected) return -1; // Probe not connected
 
    const char *snapshot = "{\"status\":\"ok\",\"voltage\":230,\"current\":5.0}";
    size_t len = strlen(snapshot);
    if (HAL_UART_Transmit(&huart_optical, (uint8_t*)snapshot, len, OPTICAL_TIMEOUT_MS) != HAL_OK) {
        return -1; // Transmission error
    }
    return 0; // Success
}
int optical_send_param(const char *key) {
    if (!probe_connected || key == NULL) return -1;
 
    // Example: respond with dummy values
    char response[64];
    if (strcmp(key, "voltage") == 0) {
        snprintf(response, sizeof(response), "{\"voltage\":230}");
    } else if (strcmp(key, "current") == 0) {
        snprintf(response, sizeof(response), "{\"current\":5.0}");
    } else {
        snprintf(response, sizeof(response), "{\"error\":\"unknown_key\"}");
    }
 
    size_t len = strlen(response);
    if (HAL_UART_Transmit(&huart_optical, (uint8_t*)response, len, OPTICAL_TIMEOUT_MS) != HAL_OK) {
        return -1; // Transmission error
    }
    return 0; // Success
}
 
bool optical_process_one_command(uint32_t timeout_ms) {
    if (!probe_connected) return false;
 
    uint8_t byte;
    if (HAL_UART_Receive(&huart_optical, &byte, 1, timeout_ms) == HAL_OK) {
        if (rx_index < OPTICAL_BUFFER_SIZE - 1) {
            rx_buffer[rx_index++] = byte;
            rx_buffer[rx_index] = '\0'; // Null-terminate
 
            // Check for end of command (newline)
            if (byte == '\n') {
                // Process command
                if (strncmp((char*)rx_buffer, "GET_PARAM ", 10) == 0) {
                    char *param_key = (char*)rx_buffer + 10;
                    param_key[strcspn(param_key, "\r\n")] = 0; // Remove newline
                    optical_send_param(param_key);
                }
                // Reset buffer for next command
                rx_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
                return true; // Command processed
            }
        } else {
            // Buffer overflow, reset
            rx_index = 0;
            memset(rx_buffer, 0, sizeof(rx_buffer));
        }
    }
    return false; // No command processed
}
 
 
 
 