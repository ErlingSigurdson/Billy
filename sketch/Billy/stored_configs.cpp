/****************** DESCRIPTION *****************/

/**
 * Filename: stored_configs_t
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Data type for config values stored in an inbuilt storage.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_inbuilt_storage.h"

// Local modules.
#include "stored_configs.h"
#include "inbuilt_storage.h"


/******************* FUNCTIONS ******************/

// Read config values from an inbuilt storage into the struct.
void stored_configs_read(stored_configs_t *stored_configs)
{
    // Indices in the array of strings read from an inbuilt storage.
    #define INDEX_WIFI_SSID               0
    #define INDEX_WIFI_PSWD               1
    #define INDEX_WIFI_RSSI_OUTPUT_FLAG   2
    #define INDEX_WIFI_AUTORECONNECT_FLAG 3
    #define INDEX_LOCAL_SERVER_PORT       4
    #define INDEX_IOT_FLAG                5
    #define INDEX_IOT_SERVER_IP           6
    #define INDEX_IOT_SERVER_PORT         7
    #define INDEX_IOT_REQ_MSG             8
    #define INDEX_IOT_REQ_PERIOD          9
    #define INDEX_BTCLASSIC_FLAG          10
    #define INDEX_BTCLASSIC_DEV_NAME      11


    /*--- Reading strings from an inbuilt storage ---*/

    char stored_configs_str[INBUILT_STORAGE_ITEM_LIST_LEN][INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
    uint32_t stored_configs_addr[] = {
        INBUILT_STORAGE_ADDR_WIFI_SSID,
        INBUILT_STORAGE_ADDR_WIFI_PSWD,
        INBUILT_STORAGE_ADDR_WIFI_RSSI_OUTPUT_FLAG,
        INBUILT_STORAGE_ADDR_WIFI_AUTORECONNECT_FLAG,
        INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
        INBUILT_STORAGE_ADDR_IOT_FLAG,
        INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
        INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
        INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
        INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
        INBUILT_STORAGE_ADDR_BTCLASSIC_FLAG,
        INBUILT_STORAGE_ADDR_BTCLASSIC_DEV_NAME
    };

    uint32_t stored_configs_addr_list_len = sizeof(stored_configs_addr) / sizeof(uint32_t);
    if (INBUILT_STORAGE_ITEM_LIST_LEN != stored_configs_addr_list_len) {
        Serial.println("");
        Serial.println("Error reading from inbuilt storage: function stored_configs_read().");

        return;
    }

    for (uint32_t i = 0; i < INBUILT_STORAGE_ITEM_LIST_LEN; ++i) {
        inbuilt_storage_read_string(stored_configs_str[i],
                                    sizeof(stored_configs_str[i]),
                                    INBUILT_STORAGE_STR_MAX_LEN,
                                    stored_configs_addr[i]);
    }


    /*--- Writing strings and extracted integers into the struct ---*/

    strcpy(stored_configs->WiFi_SSID, stored_configs_str[INDEX_WIFI_SSID]);

    strcpy(stored_configs->WiFi_pswd, stored_configs_str[INDEX_WIFI_PSWD]);

    if (!strcmp(stored_configs_str[INDEX_WIFI_RSSI_OUTPUT_FLAG], "ON")) {
        stored_configs->WiFi_RSSI_output_flag = 1;
    } else {
        stored_configs->WiFi_RSSI_output_flag = 0;
    }

    if (!strcmp(stored_configs_str[INDEX_WIFI_AUTORECONNECT_FLAG], "ON")) {
        stored_configs->WiFi_autoreconnect_flag = 1;
    } else {
        stored_configs->WiFi_autoreconnect_flag = 0;
    }    

    stored_configs->local_server_port = strtol(stored_configs_str[INDEX_LOCAL_SERVER_PORT], NULL, 10);

    if (!strcmp(stored_configs_str[INDEX_IOT_FLAG], "ON")) {
        stored_configs->IoT_flag = 1;
    } else {
        stored_configs->IoT_flag = 0;
    }

    strcpy(stored_configs->IoT_server_IP, stored_configs_str[INDEX_IOT_SERVER_IP]);

    stored_configs->IoT_server_port = strtol(stored_configs_str[INDEX_IOT_SERVER_PORT], NULL, 10);

    strcpy(stored_configs->IoT_req_msg, stored_configs_str[INDEX_IOT_REQ_MSG]);

    stored_configs->IoT_req_period = strtol(stored_configs_str[INDEX_IOT_REQ_PERIOD], NULL, 10);

    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        if (!strcmp(stored_configs_str[INDEX_BTCLASSIC_FLAG], "ON")) {
            stored_configs->BTClassic_flag = 1;
        } else {
            stored_configs->BTClassic_flag = 0;
        }

        strcpy(stored_configs->BTClassic_dev_name, stored_configs_str[INDEX_BTCLASSIC_DEV_NAME]);
    #endif
}
