/****************** DESCRIPTION *****************/

/**
 * Filename: stored_configs.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: data type for config values stored in the inbuilt storage.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef STORED_CONFIGS_H
#define STORED_CONFIGS_H


/*--- Includes ---*/

// Main Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_general.h"


/****************** DATA TYPES ******************/

// Struct for holding values read from the inbuilt storage.
typedef struct stored_configs_t {
    char local_SSID[STR_MAX_LEN + 1];
    char local_pswd[STR_MAX_LEN + 1];
    uint32_t local_server_port;
    bool IoT_flag;
    char IoT_server_IP[STR_MAX_LEN + 1];
    uint32_t IoT_server_port;
    char IoT_req_msg[STR_MAX_LEN + 1];
    uint32_t IoT_req_period;
    bool BT_Classic_flag;
    char BT_Classic_dev_name[STR_MAX_LEN + 1];
    bool RSSI_output_flag;
} stored_configs_t;


/************** FUNCTION PROTOTYPES *************/

/*--- Misc functions ---*/

// Read config values from the inbuilt storage into the struct.
void stored_configs_read(stored_configs_t *_stored_configs);


#endif  // Include guards.