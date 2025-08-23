/****************** DESCRIPTION *****************/

/**
 * Filename: ESP32_BLE.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Bluetooth Low Energy (BLE) UART wrapper functions for ESP32 SoC.
 *           Written for use with the ESP32 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    Uses the ESP32 BLE Arduino library to emulate a UART service.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP32_BTLE_H
#define ESP32_BTLE_H


/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>


/*--- Misc ---*/

#define BTLE_READ_SLOWDOWN 2


/************** FUNCTION PROTOTYPES *************/

void ESP32_BTLE_start(const char *dev_name);
bool ESP32_BTLE_check_connection();
uint32_t ESP32_BTLE_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout);
void ESP32_BTLE_send_msg(const char *msg);
void ESP32_BTLE_disconnect(uint32_t shutdown_downtime);
void ESP32_BTLE_stop(uint32_t shutdown_downtime);


#endif  // Include guards.
