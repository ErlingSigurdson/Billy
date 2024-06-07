/****************** DESCRIPTION *****************/

/**
 * Filename: config_inbuilt_storage.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: projects configs related to ESP32/ESP8266
 * flash memory used as an inbuilt storage.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CONFIG_INBUILT_STORAGE_H
#define CONFIG_INBUILT_STORAGE_H


/*--- Basic values ---*/

#define INBUILT_STORAGE_ITEM_LIST_LEN 11  // Number of strings to store.
#define INBUILT_STORAGE_SIZE 1024
#define INBUILT_STORAGE_STR_MAX_LEN 100


/*--- Config strings addresses ---*/

// Local Wi-Fi network authentication data.
#define INBUILT_STORAGE_ADDR_LOCAL_SSID 0
#define INBUILT_STORAGE_ADDR_LOCAL_PSWD 101

// Local server settings.
#define INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT 202

// IoT mode (attempts to connect to a remote server) settings.
#define INBUILT_STORAGE_ADDR_IOT_FLAG 303
#define INBUILT_STORAGE_ADDR_IOT_SERVER_IP 404
#define INBUILT_STORAGE_ADDR_IOT_SERVER_PORT 505
#define INBUILT_STORAGE_ADDR_IOT_REQ_MSG 606
#define INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD 707

// Bluetooth settings.
#define INBUILT_STORAGE_ADDR_BT_CLASSIC_FLAG 808
#define INBUILT_STORAGE_ADDR_BT_CLASSIC_DEV_NAME 909

// Current RSSI printout settings.
#define INBUILT_STORAGE_ADDR_RSSI_OUTPUT_FLAG 1010


/*--- Misc ---*/

/* Directive indicating that the project uses a device (ESP32/ESP8266)
 * that requires some additional calls for EEPROM.h library methods.
 */
#define THIS_IS_ESP32_OR_ESP8266_OR_STM32


#endif  // Include guards.
