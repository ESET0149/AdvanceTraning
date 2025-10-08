#ifndef TAMPER_MONITOR_H
#define TAMPER_MONITOR_H

#include <stdint.h>
#include <stdbool.h>

// === Tamper Event Types ===
typedef enum {
    TAMPER_NONE = 0,
    TAMPER_MAGNETIC_INFLUENCE = 1,
    TAMPER_COVER_OPEN = 2,
    TAMPER_REVERSE_CURRENT = 3,
    TAMPER_VOLTAGE_TAMPER = 4,
    TAMPER_CURRENT_TAMPER = 5,  
    TAMPER_POWER_TAMPER = 6,
    TAMPER_CLOCK_TAMPER = 7
} TamperType;

// === Tamper Event Structure ===
typedef struct {
    TamperType type;
    uint32_t timestamp;     // Time of event (e.g., from RTC)
    uint8_t encrypted_data[64]; // Placeholder for encrypted event data
} TamperEvent;


// === Function Prototypes ===

void TamperMonitor_Init(unit32_t poll_interval_ms);

TamperType TamperMonitor_Check(void);

void TamperMonitor_LogEvent(const TamperEvent* event);

void TamperMonitor_EncryptEvent(TamperEvent* event);

void TamperMonitor_SendMQTT(const TamperEvent* event);

#endif // TAMPER_MONITOR_H
