/*************** FILE DESCRIPTION ***************/

/**
 * Filename: esp32_btclassic.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Bluetooth Classic wrapper functions for ESP32 SoC.
 *           Intended for use with the ESP32 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP32_BTCLASSIC_H
#define ESP32_BTCLASSIC_H


/*--- Includes ---*/

// Arduino core header file.
#include <Arduino.h>


/************** FUNCTION PROTOTYPES *************/

namespace esp32_btclassic_billy {
    void start(const char *dev_name);
    bool check_connection();
    uint32_t read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout, uint32_t read_slowdown);
    void send_msg(const char *msg);
    void disconnect(uint32_t shutdown_downtime);
    void stop(uint32_t shutdown_downtime);
}


#endif  // Include guards.
