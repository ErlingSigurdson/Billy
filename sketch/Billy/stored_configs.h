/****************** DESCRIPTION *****************/

/**
 * Filename: stored_configs_t
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Data type for config values stored in an inbuilt storage.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef STORED_CONFIGS_H
#define STORED_CONFIGS_H


/*--- Includes ---*/

// Project configs.
#include "config_general.h"


/****************** DATA TYPES ******************/

// Struct for holding values read from an inbuilt storage.
typedef struct stored_configs_t {
    char WiFi_SSID[STR_MAX_LEN + 1];
    char WiFi_pswd[STR_MAX_LEN + 1];
    bool WiFi_RSSI_output_flag;
    bool WiFi_autoreconnect_flag;
    uint32_t local_server_port;
    bool IoT_flag;
    char IoT_server_IP[STR_MAX_LEN + 1];
    uint32_t IoT_server_port;
    char IoT_req_msg[STR_MAX_LEN + 1];
    uint32_t IoT_req_period;
    bool BTClassic_flag;
    char BTClassic_dev_name[STR_MAX_LEN + 1];
} stored_configs_t;


/************** FUNCTION PROTOTYPES *************/

/*--- Misc functions ---*/

// Read config values from an inbuilt storage into the struct.
void stored_configs_read(stored_configs_t *stored_configs);


#endif  // Include guards.