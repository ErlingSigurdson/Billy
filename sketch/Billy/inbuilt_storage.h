/*************** FILE DESCRIPTION ***************/

/**
 * Filename: inbuilt_storage.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Writing and reading strings to and from
 *           an inbuilt storage using EEPROM.h library.
 *           Intended for use with the Arduino framework.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    AVR devices use an inbuilt EEPROM.
 *           ESP32, ESP8266 and STM32 devices emulate EEPROM in a flash memory.
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

// Function return codes.
#define INBUILT_STORAGE_ERR_NULLPTR -1
#define INBUILT_STORAGE_ERR_SIZE    -2
#define INBUILT_STORAGE_ERR_BYTE    -3

/* Indicate if the sketch uses a device that requires
 * some additional calls for EEPROM.h library methods.
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

#define ASCII_CODE_HIGHEST 127


/************** FUNCTION PROTOTYPES *************/

namespace inbuilt_storage {

    // Storage initialization. Necessary for ESP32, ESP8266 and STM32 devices. Not to be used with AVR devices
    // Returns true if successful, false otherwise.
    /* A conditional compilation is used because the EEPROM.h variant for AVR devices lacks the respective method.
     *
     *
     */
    #ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32_PRESUMABLY
        bool init(uint32_t emulated_eeprom_size);
    #endif

    // Read a string from storage and write it to a buffer.
    int32_t read_string_from_storage(char *buf, size_t buf_size, size_t str_max_len, uint32_t addr);

    // Read a string from storage and write it to a buffer.
    /* This overload uses buf_size check based on a preliminary
     * counting of valid bytes to be read from storage, not on
     * a max string length constraint. Use with caution.
     */
    int32_t read_string_from_storage(char *buf, size_t buf_size, uint32_t addr);

    // Write a string to storage.
    int32_t write_string_to_storage(const char *str, size_t str_max_len, uint32_t addr);
}


#endif  // Include guards.
