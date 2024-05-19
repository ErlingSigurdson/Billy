#include "config_general.h"


/****************** DATA TYPES ******************/

// Struct for holding values read from the inbuilt storage.
typedef struct strd_vals_t {
    char SSID[STR_MAX_LEN + 1];
    char pswd[STR_MAX_LEN + 1];
    uint32_t local_server_port;
    bool IoT_flag;
    char IoT_server_IP[STR_MAX_LEN + 1];
    uint32_t IoT_server_port;
    char IoT_req_msg[STR_MAX_LEN + 1];
    uint32_t IoT_req_period;
    bool BT_flag;
    char BT_dev_name[STR_MAX_LEN + 1];
    bool RSSI_print_flag;
} strd_vals_t;


/************** FUNCTION PROTOTYPES *************/

/*--- Misc functions ---*/

// Read values from the inbuilt storage.
void strd_vals_read(strd_vals_t *_strd_vals);
