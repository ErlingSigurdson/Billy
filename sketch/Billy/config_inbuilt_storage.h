/*************** FILE DESCRIPTION ***************/

/**
 * Filename: config_inbuilt_storage.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Project configs related to use of ESP32/ESP8266
 *           flash memory as an inbuilt storage.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CONFIG_INBUILT_STORAGE_H
#define CONFIG_INBUILT_STORAGE_H


/*--- Includes ---*/

// Project configs.
#include "config_general.h"  // For STR_MAX_LEN.


/*--- Basic values ---*/

#define INBUILT_STORAGE_ITEM_LIST_LEN 12  /* Number of strings to store. Must be kept in sync
                                           * with the number of addresses defined below.
                                           */
#define INBUILT_STORAGE_SIZE 2048
#define INBUILT_STORAGE_DEFAULT_BLOCK_SIZE ((STR_MAX_LEN) + 1)


/*--- Config strings addresses ---*/

#define INBUILT_STORAGE_ADDR(x) ((x) * (INBUILT_STORAGE_DEFAULT_BLOCK_SIZE))

// Wi-Fi settings.
#define INBUILT_STORAGE_ADDR_WIFI_SSID                (INBUILT_STORAGE_ADDR(0))
#define INBUILT_STORAGE_ADDR_WIFI_PSWD                (INBUILT_STORAGE_ADDR(1))
#define INBUILT_STORAGE_ADDR_WIFI_RSSI_OUTPUT_FLAG    (INBUILT_STORAGE_ADDR(2))
#define INBUILT_STORAGE_ADDR_WIFI_AUTORECONNECT_FLAG  (INBUILT_STORAGE_ADDR(3))

// Local TCP server settings.
#define INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT        (INBUILT_STORAGE_ADDR(4))

// IoT mode (attempts to connect to a remote server) settings.
#define INBUILT_STORAGE_ADDR_IOT_FLAG                 (INBUILT_STORAGE_ADDR(5))
#define INBUILT_STORAGE_ADDR_IOT_SERVER_IP            (INBUILT_STORAGE_ADDR(6))
#define INBUILT_STORAGE_ADDR_IOT_SERVER_PORT          (INBUILT_STORAGE_ADDR(7))
#define INBUILT_STORAGE_ADDR_IOT_REQ_MSG              (INBUILT_STORAGE_ADDR(8))
#define INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD           (INBUILT_STORAGE_ADDR(9))

// Bluetooth Classic settings.
#define INBUILT_STORAGE_ADDR_BTCLASSIC_FLAG           (INBUILT_STORAGE_ADDR(10))
#define INBUILT_STORAGE_ADDR_BTCLASSIC_DEV_NAME       (INBUILT_STORAGE_ADDR(11))


/*--- Safety checks ---*/

#if (INBUILT_STORAGE_ITEM_LIST_LEN) <= 0
    #error "Inbuilt storage config error: item list length must be a positive integer."
#endif

#if (INBUILT_STORAGE_DEFAULT_BLOCK_SIZE) <= 0
    #error "Inbuilt storage config error: block size must be a positive integer."
#endif

#if INBUILT_STORAGE_ADDR(INBUILT_STORAGE_ITEM_LIST_LEN) > (INBUILT_STORAGE_SIZE)
    #error "Inbuilt storage config error: insufficient defined storage size."
#endif


#endif  // Include guards.
