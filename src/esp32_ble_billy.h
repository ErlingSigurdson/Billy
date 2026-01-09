/*************** FILE DESCRIPTION ***************/

/**
 * Filename: esp32_ble_billy.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Bluetooth Low Energy (BLE) UART wrapper functions for ESP32 SoC.
 *           Intended for use with the ESP32 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    Uses the ESP32 BLE Arduino library to emulate a UART service.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP32_BLE_BILLY_H
#define ESP32_BLE_BILLY_H


/*--- Includes ---*/

// Relevant standard libraries.
#include <cstdint>


/*--- Misc ---*/

#define BTLE_READ_SLOWDOWN 2


/************** FUNCTION PROTOTYPES *************/

namespace esp32_ble_billy {
    void start(const char *dev_name);
    bool check_connection();
    uint32_t read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout);
    void send_msg(const char *msg);
    void disconnect(uint32_t shutdown_downtime);
    void stop(uint32_t shutdown_downtime);
}


#endif  // Include guards.
