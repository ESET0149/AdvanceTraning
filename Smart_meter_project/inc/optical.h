#ifndef OPTICAL_H
#define OPTICAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//Initialize optical/manual comms subsystem.
//Must be called once during startup.
void optical_init(void);

//Run the optical communication state machine.
//- This function blocks while performing handshake and request/response.
// - Intended to be run from a dedicated FreeRTOS task or from main loop.
void optical_task(void *argument);

//Check if probe currently connected (reads detect pin).
bool optical_probe_connected(void);


//Send a status snapshot (JSON-like) to the probe.
//The function will encrypt/format if required by your system.
//Returns 0 on success, -1 on error.
int optical_send_snapshot(void);


//Send a parameter value as response to GET_PARAM command.
int optical_send_param(const char *key);

//If you want to process a single incoming command (blocking read with timeout)
//use this helper: returns true if processed.
bool optical_process_one_command(uint32_t timeout_ms);

#endif // OPTICAL_H