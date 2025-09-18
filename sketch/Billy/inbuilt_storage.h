/*************** FILE DESCRIPTION ***************/

/**
 * Filename: inbuilt_storage.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Reading and writing strings to and from an inbuilt storage
 *           using EEPROM.h library.
 *           Intended for use with the Arduino framework.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    AVR devices use inbuilt EEPROM.
 *           ESP32, ESP8266 and STM32 emulate EEPROM
 *           in their inbuilt flash memory.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef INBUILT_STORAGE_H
#define INBUILT_STORAGE_H


/*--- Includes ---*/

// Relevant standard libraries.
#include <cstddef>  // For size_t.
#include <cstdint>


/*--- Misc ---*/

/* Indicate that the project uses a device that requires
 * some additional calls for the EEPROM.h library methods.
 *
 * From https://onlinedocs.microchip.com:
 *
 * Options for the C compiler avr-gcc
 * Machine-specific options for the AVR
 *
 * The following machine-specific options are recognized by the C compiler frontend.
 * In addition to the preprocessor macros indicated in the tables below, the preprocessor will define the macros
 * __AVR and __AVR__ (to the value 1) when compiling for an AVR target. The macro AVR will be defined as well
 * when using the standard levels gnu89 (default) and gnu99 but not with c89 and c99.
 */
#ifndef __AVR__
    #define THIS_IS_ESP32_OR_ESP8266_OR_STM32_PRESUMABLY
#endif


/************** FUNCTION PROTOTYPES *************/

/* Storage initialization. Necessary for ESP32, ESP8266 and STM32. Not to be used with AVR devices.
 * Conditional compilation is used because the EEPROM.h variant for AVR devices just lacks the respective method.
 */
#ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32_PRESUMABLY
    void inbuilt_storage_init(uint32_t emulated_eeprom_size);
#endif

// Read a string.
void inbuilt_storage_read_string_from_storage(char *buf, size_t buf_size, uint32_t str_max_len, uint32_t addr);

// Write a string.
void inbuilt_storage_write_string_to_storage(const char *str, uint32_t str_max_len, uint32_t addr);


#endif  // Include guards.
