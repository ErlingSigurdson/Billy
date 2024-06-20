/****************** DESCRIPTION *****************/

/**
 * Filename: config_inbuilt_storage.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: projects configs related to ESP32/ESP8266
 *          flash memory used as an inbuilt storage.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CONFIG_INBUILT_STORAGE_H
#define CONFIG_INBUILT_STORAGE_H


/*--- Basic values ---*/

#define INBUILT_STORAGE_ITEM_LIST_LEN 12  // Number of strings to store.
#define INBUILT_STORAGE_SIZE 2048
#define INBUILT_STORAGE_STR_MAX_LEN 100


/*--- Config strings addresses ---*/

// Wi-Fi network settings.
#define INBUILT_STORAGE_ADDR_WIFI_SSID 0
#define INBUILT_STORAGE_ADDR_WIFI_PSWD 101
#define INBUILT_STORAGE_ADDR_WIFI_RSSI_OUTPUT_FLAG 202
#define INBUILT_STORAGE_ADDR_WIFI_AUTORECONNECT_FLAG 303

// Local server settings.
#define INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT 404

// IoT mode (attempts to connect to a remote server) settings.
#define INBUILT_STORAGE_ADDR_IOT_FLAG 505
#define INBUILT_STORAGE_ADDR_IOT_SERVER_IP 606
#define INBUILT_STORAGE_ADDR_IOT_SERVER_PORT 707
#define INBUILT_STORAGE_ADDR_IOT_REQ_MSG 808
#define INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD 909

// Bluetooth Classic settings.
#define INBUILT_STORAGE_ADDR_BTCLASSIC_FLAG 1010
#define INBUILT_STORAGE_ADDR_BTCLASSIC_DEV_NAME 1111


/*--- Misc ---*/

/* Directive indicating that the project uses a device (ESP32/ESP8266)
 * that requires some additional calls for EEPROM.h library methods.
 */
#define THIS_IS_ESP32_OR_ESP8266_OR_STM32


#endif  // Include guards.
