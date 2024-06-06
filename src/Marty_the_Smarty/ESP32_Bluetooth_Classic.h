/****************** DESCRIPTION *****************/

/**
 * Filename: ESP32_Bluetooth_Classic.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: Bluetooth Classic-related functions for ESP32 modules.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP32_BT_CLASSIC_H
#define ESP32_BT_CLASSIC_H


/*--- Includes ---*/

// General Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_general.h"


/*--- Misc ---*/

// Conditional compilation.
#if defined ESP32 && defined BT_CLASSIC_PROVIDED


/************** FUNCTION PROTOTYPES *************/

void ESP32_BT_Classic_start(char *dev_name);
bool ESP32_BT_Classic_check_connection();
uint32_t ESP32_BT_Classic_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout);
void ESP32_BT_Classic_send_msg(const char *msg);
void ESP32_BT_Classic_disconnect(uint32_t shutdown_downtime);
void ESP32_BT_Classic_stop(uint32_t shutdown_downtime);


#endif  // Conditional compilation.
#endif  // Include guards.
