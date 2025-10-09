
#ifndef CONFIG_H
#define CONFIG_H

/* Build options */
#define USE_FREERTOS        1
#define USE_EXTERNAL_FLASH  1   /* Set 0 to use MCU internal flash/APROM if available */

/* Region-specific */
#define REGION_INDIA        1

/* Storage sizes / policies */
#define NVM_MAX_ENTRIES     1024
#define RECORD_SIZE_BYTES   128
#define STORAGE_RETENTION_DAYS 90

/* MQTT topics */
#define MQTT_TOPIC_METERS_DATA "meters/data"
#define MQTT_TOPIC_CONTROL     "control/onoff"
#define MQTT_TOPIC_ALERTS      "meters/alerts"
#define MQTT_QOS               1

/* Security - do NOT hardcode keys in production */
#define AES_KEY_SIZE_BYTES 16

extern const unsigned char GLOBAL_AES_KEY[AES_KEY_SIZE_BYTES];
extern const unsigned char GLOBAL_HMAC_KEY[16];

#endif
