#include "tamper_monitor.h"
#include "storage.h"         // for storage timestamp or bs_store_event (billing store)
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#ifndef TAMPER_LOG_TOPIC
#define TAMPER_LOG_TOPIC "meter/tamper"
#endif

#ifndef TAMPER_MONITOR_LOG
#define TAMPER_MONITOR_LOG(fmt, ...) printf("[TAMPER] " fmt "\n", ##__VA_ARGS__)
#endif

/* Configuration */
static uint32_t g_poll_interval_ms = 500; // default poll every 500 ms
static uint32_t g_last_poll_ms = 0;
static const uint8_t DEBOUNCE_SAMPLES = 3; // require 3 consistent samples for event

/* Debounce counters */
static uint8_t mag_counter = 0;
static uint8_t cover_counter = 0;
static uint8_t rev_counter = 0;

/* Last stable states (0 = no tamper, 1 = tamper active) */
static uint8_t mag_state = 0;
static uint8_t cover_state = 0;
static uint8_t rev_state = 0;

/* Simple millisecond tick helper: use HAL_GetTick() if available */
#ifdef HAL_EXISTS
extern uint32_t HAL_GetTick(void);
static inline uint32_t tamper_get_tick(void) { return HAL_GetTick(); }
#else
#include <time.h>
static inline uint32_t tamper_get_tick(void) {
    return (uint32_t)(time(NULL) * 1000UL);
}
#endif

/* Forward declarations of platform hooks (weakly linked in platform file or implemented by user) */
extern int platform_read_magnetic_sensor(void);
extern int platform_read_cover_switch(void);
extern int platform_read_reverse_current_detector(void);

/* Internal helper: get epoch timestamp (seconds) */
static uint32_t current_epoch_sec(void) {
#ifdef HAL_RTC_MODULE_ENABLED
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
	
	// Make sure RTC handle is defined externally in your main.c
    extern RTC_HandleTypeDef hrtc;

    // Read RTC date & time
    if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK &&
        HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) == HAL_OK)
    {
        struct tm t;
        t.tm_sec  = sTime.Seconds;
        t.tm_min  = sTime.Minutes;
        t.tm_hour = sTime.Hours;
        t.tm_mday = sDate.Date;
        t.tm_mon  = sDate.Month - 1;   // tm_mon is 0–11
        t.tm_year = sDate.Year + 100;  // HAL gives years since 2000 → +100 = 20xx
        t.tm_isdst = 0;
        time_t epoch = mktime(&t);
        return (uint32_t)epoch;
    }

    // RTC read failed → fallback
    return (uint32_t)(HAL_GetTick() / 1000);

#elif defined(HAL_EXISTS)
    // Placeholder if HAL is present but RTC not configured
    return (uint32_t)(HAL_GetTick() / 1000);

#else
    // Non-embedded fallback (PC simulation, unit test, etc.)
    return (uint32_t)time(NULL);
#endif
}

/* Build event details and publish */
static void handle_tamper_detected(tamper_event_code_t code, const uint8_t *details, uint8_t dlen)
{
    uint32_t ts = current_epoch_sec();
    TAMPER_MONITOR_LOG("Tamper detected: code=%u ts=%u", (unsigned)code, (unsigned)ts);

    // pack a compact alert message: [event_code(1)] [ts(4)] [dlen(1)] [details...]
    uint8_t alert_payload[256];
    size_t idx = 0;
    alert_payload[idx++] = (uint8_t)code;
    alert_payload[idx++] = (uint8_t)((ts >> 24) & 0xFF);
    alert_payload[idx++] = (uint8_t)((ts >> 16) & 0xFF);
    alert_payload[idx++] = (uint8_t)((ts >> 8) & 0xFF);
    alert_payload[idx++] = (uint8_t)(ts & 0xFF);
    if (dlen > 200) dlen = 200;
    alert_payload[idx++] = dlen;
    if (dlen && details) {
        memcpy(&alert_payload[idx], details, dlen);
        idx += dlen;
    }

    // Encrypt & sign
    uint8_t encrypted[512];
    size_t encrypted_len = sizeof(encrypted);
    if (security_encrypt_and_sign(alert_payload, idx, encrypted, &encrypted_len) != 0) {
        TAMPER_MONITOR_LOG("Failed to encrypt tamper alert");
        return;
    }

    // Publish via MQTT
    if (mqtt_bridge_publish(TAMPER_LOG_TOPIC, encrypted, encrypted_len) == 0) {
        TAMPER_MONITOR_LOG("Tamper alert published (%u bytes) to %s", (unsigned)encrypted_len, TAMPER_LOG_TOPIC);
    } else {
        TAMPER_MONITOR_LOG("Failed to publish tamper alert");
    }
}

/* Public API: initialize module */
void tamper_monitor_init(uint32_t poll_interval_ms)
{
    if (poll_interval_ms > 0) g_poll_interval_ms = poll_interval_ms;
    g_last_poll_ms = tamper_get_tick();

    mag_counter = cover_counter = rev_counter = 0;
    mag_state = cover_state = rev_state = 0;

    TAMPER_MONITOR_LOG("Initialized (poll %u ms)", (unsigned)g_poll_interval_ms);
}

/* Public API: force event for test */
int tamper_monitor_force_event(tamper_event_code_t code)
{
    uint8_t details[8];
    int dlen = snprintf((char*)details, sizeof(details), "FORCE");
    handle_tamper_detected(code, details, (uint8_t)dlen);
    return 0;
}

/* Poll function: call frequently in main loop or RTOS thread */
void tamper_monitor_poll(void)
{
    uint32_t now = tamper_get_tick();
    if ((now - g_last_poll_ms) < g_poll_interval_ms) return;
    g_last_poll_ms = now;

    // Read platform sensors (implement these to return 0/1)
    int mag = platform_read_magnetic_sensor();
    int cover = platform_read_cover_switch();
    int rev  = platform_read_reverse_current_detector();

    // DEBOUNCE: increase counter on high, reset on low; trigger when reaches DEBOUNCE_SAMPLES
    if (mag) {
        if (mag_counter < 255) mag_counter++;    } else {

    } else {
        if (mag_counter > 0) mag_counter--;
    }
    if (cover) {
        if (cover_counter < 255) cover_counter++;
        if (cover_counter > 0) cover_counter--;
    }
    if (rev) {
        if (rev_counter < 255) rev_counter++;
    } else {
        if (rev_counter > 0) rev_counter--;
    }

    // Determine stable states and trigger events on rising edge
    uint8_t new_mag_state = (mag_counter >= DEBOUNCE_SAMPLES) ? 1 : 0;
    uint8_t new_cover_state = (cover_counter >= DEBOUNCE_SAMPLES) ? 1 : 0;
    uint8_t new_rev_state = (rev_counter >= DEBOUNCE_SAMPLES) ? 1 : 0;

    if (!mag_state && new_mag_state) {
        // rising edge: magnetic tamper event
        const char *info = "magnet_detected";
        handle_tamper_detected(TAMPER_MAGNETIC_INFLUENCE, (const uint8_t*)info, (uint8_t)strlen(info));
    }
    if (!cover_state && new_cover_state) {
        const char *info = "cover_open";
        handle_tamper_detected(TAMPER_COVER_OPEN, (const uint8_t*)info, (uint8_t)strlen(info));
    }
    if (!rev_state && new_rev_state) {
        const char *info = "reverse_current";
        handle_tamper_detected(TAMPER_REVERSE_CURRENT, (const uint8_t*)info, (uint8_t)strlen(info));
    }

    // Update latched states
    mag_state = new_mag_state;
    cover_state = new_cover_state;
    rev_state = new_rev_state;
}

/* Example weak platform hooks (for testing).
 * Replace these with real GPIO/ADC reads in your board support package (BSP).
 * If you provide your own definitions, remove the weak attribute or provide them elsewhere.
 */
__attribute__((weak)) int platform_read_magnetic_sensor(void) {
    // Return 0/1 from GPIO or hall-sensor input
    return 0;
}
__attribute__((weak)) int platform_read_cover_switch(void) {
    // Return 0 when closed, 1 when open (or vice-versa). Adjust logic accordingly.
    return 0;
}
__attribute__((weak)) int platform_read_reverse_current_detector(void) {
    // Example: read sign of current from ADC front-end or comparator
    return 0;
}