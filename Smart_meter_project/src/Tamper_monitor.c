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

/* Forward declarations of platform hooks (weakly linked in platform file or implemented by user) */
extern int platform_read_magnetic_sensor(void);
extern int platform_read_cover_switch(void);
extern int platform_read_reverse_current_detector(void);


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