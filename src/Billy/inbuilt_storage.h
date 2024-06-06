/****************** DESCRIPTION *****************/

/**
 * Filename: inbuilt_storage.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: reading and writing strings from and to an inbuilt storage
 * using EEPROM.h library.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 * AVR devices use inbuilt EEPROM.
 * ESP32, ESP8266 and STM32 emulate EEPROM in their inbuilt flash memory.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Include guards.
#ifndef INBUILT_STORAGE_H
#define INBUILT_STORAGE_H

// General Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_inbuilt_storage.h"


/************** FUNCTION PROTOTYPES *************/

// Storage initialization. Necessary for ESP32, ESP8266 and STM32. Not to be used with AVR devices.
void inbuilt_storage_init(uint32_t emulated_EEPROM_size);

// Read string.
void inbuilt_storage_read(char *buf, size_t buf_size, uint32_t str_max_len, uint32_t addr);

// Write string.
void inbuilt_storage_write(char *str, uint32_t str_len, uint32_t str_max_len, uint32_t addr);


#endif  // Include guards.
