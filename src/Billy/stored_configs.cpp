/****************** DESCRIPTION *****************/

/**
 * Filename: stored_configs.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: data type for config values stored in the inbuilt storage.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/*--- Includes ---*/

// Main Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_inbuilt_storage.h"

// Local modules.
#include "stored_configs.h"
#include "inbuilt_storage.h"


/******************* FUNCTIONS ******************/

// Read config values from the inbuilt storage into the struct.
void stored_configs_read(stored_configs_t *_stored_configs)
{
    // Indices in the array of strings read from the inbuilt storage.
    #define INDEX_LOCAL_SSID               0
    #define INDEX_LOCAL_PSWD               1
    #define INDEX_LOCAL_AUTORECONNECT_FLAG 2
    #define INDEX_LOCAL_RSSI_OUTPUT_FLAG   3
    #define INDEX_LOCAL_SERVER_PORT        4
    #define INDEX_IOT_FLAG                 5
    #define INDEX_IOT_SERVER_IP            6
    #define INDEX_IOT_SERVER_PORT          7
    #define INDEX_IOT_REQ_MSG              8
    #define INDEX_IOT_REQ_PERIOD           9
    #define INDEX_BTCLASSIC_FLAG           10
    #define INDEX_BTCLASSIC_DEV_NAME       11



    /*--- Reading strings from the inbuilt storage ---*/

    char stored_configs_str[INBUILT_STORAGE_ITEM_LIST_LEN][INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
    uint32_t stored_configs_addr[] = {
        INBUILT_STORAGE_ADDR_LOCAL_SSID,
        INBUILT_STORAGE_ADDR_LOCAL_PSWD,
        INBUILT_STORAGE_ADDR_LOCAL_AUTORECONNECT_FLAG,
        INBUILT_STORAGE_ADDR_LOCAL_RSSI_OUTPUT_FLAG,
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
        Serial.println("Error reading from the inbuilt storage: function stored_configs_read().");

        return;
    }

    for (uint32_t i = 0; i < INBUILT_STORAGE_ITEM_LIST_LEN; ++i) {
        inbuilt_storage_read(stored_configs_str[i],
                             sizeof(stored_configs_str[i]),
                             INBUILT_STORAGE_STR_MAX_LEN,
                             stored_configs_addr[i]);
    }


    /*--- Writing of strings and extracted integers into the struct ---*/

    // Local Wi-Fi network SSID.
    strcpy(_stored_configs->local_SSID, stored_configs_str[INDEX_LOCAL_SSID]);

    // Local Wi-Fi access point password.
    strcpy(_stored_configs->local_pswd, stored_configs_str[INDEX_LOCAL_PSWD]);

    // Local Wi-Fi autoreconnect flag.
    if (!strcmp(stored_configs_str[INDEX_LOCAL_AUTORECONNECT_FLAG], "ON")) {
        _stored_configs->local_autoreconnect_flag = 1;
    } else {
        _stored_configs->local_autoreconnect_flag = 0;
    }    

    // RSSI output flag.
    if (!strcmp(stored_configs_str[INDEX_LOCAL_RSSI_OUTPUT_FLAG], "ON")) {
        _stored_configs->local_RSSI_output_flag = 1;
    } else {
        _stored_configs->local_RSSI_output_flag = 0;
    }

    // Local TCP server port.
    _stored_configs->local_server_port = strtol(stored_configs_str[INDEX_LOCAL_SERVER_PORT], NULL, 10);

    // IoT mode (attempts to connect to a remote server) flag.
    if (!strcmp(stored_configs_str[INDEX_IOT_FLAG], "ON")) {
        _stored_configs->IoT_flag = 1;
    } else {
        _stored_configs->IoT_flag = 0;
    }

    // IoT (remote) server IP.
    strcpy(_stored_configs->IoT_server_IP, stored_configs_str[INDEX_IOT_SERVER_IP]);

    // IoT (remote) server port.
    _stored_configs->IoT_server_port = strtol(stored_configs_str[INDEX_IOT_SERVER_PORT], NULL, 10);

    // Text of a request to an IoT (remote) server.
    strcpy(_stored_configs->IoT_req_msg, stored_configs_str[INDEX_IOT_REQ_MSG]);

    // Interval for sending requests to an IoT (remote) server.
    _stored_configs->IoT_req_period = strtol(stored_configs_str[INDEX_IOT_REQ_PERIOD], NULL, 10);

    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        // Bluetooth Classic functionality flag.
        if (!strcmp(stored_configs_str[INDEX_BTCLASSIC_FLAG], "ON")) {
            _stored_configs->BTClassic_flag = 1;
        } else {
            _stored_configs->BTClassic_flag = 0;
        }

        // ESP's name as a Bluetooth Classic slave device
        strcpy(_stored_configs->BTClassic_dev_name, stored_configs_str[INDEX_BTCLASSIC_DEV_NAME]);
    #endif
}
