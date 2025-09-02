/*************** FILE DESCRIPTION ***************/

/**
 * Filename: ESP32_BTClassic.h
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

void ESP32_BTClassic_start(const char *dev_name);
bool ESP32_BTClassic_check_connection();
uint32_t ESP32_BTClassic_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout, uint32_t read_slowdown);
void ESP32_BTClassic_send_msg(const char *msg);
void ESP32_BTClassic_disconnect(uint32_t shutdown_downtime);
void ESP32_BTClassic_stop(uint32_t shutdown_downtime);


#endif  // Include guards.
