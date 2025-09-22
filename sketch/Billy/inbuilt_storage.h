/*************** FILE DESCRIPTION ***************/

/**
 * Filename: inbuilt_storage.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Writing and reading strings to and from
 *           inbuilt storage using EEPROM.h library.
 *           Intended for use with the Arduino framework.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    AVR devices use inbuilt EEPROM.
 *           ESP32, ESP8266 and STM32 devices emulate EEPROM in flash memory.
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
#define INBUILT_STORAGE_ERR_NULLPTR       -1
#define INBUILT_STORAGE_ERR_SIZE          -2
#define INBUILT_STORAGE_ERR_BYTE          -3
#define INBUILT_STORAGE_ERR_NO_TERMINATOR -4

#define ASCII_CODE_HIGHEST 127


/************** FUNCTION PROTOTYPES *************/

namespace inbuilt_storage {

    /* Initialize storage.
     *
     * Returns: true if successful, false otherwise.
     *
     * Necessary for ESP32, ESP8266 and STM32 devices. Not to be used with AVR devices
     * Conditional compilation is used because the EEPROM.h
     * variant for AVR devices lacks the respective method.
     */
    #if defined ARDUINO_ARCH_ESP32 || defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_STM32
        bool init(uint32_t emulated_eeprom_size);
    #endif

    /* Read a string from storage and write it to a buffer.
     *
     * Returns: number of bytes read if successful (including null terminator),
     * negative integer code otherwise (see the preprocessor macros list).
     */
    int32_t read_string_from_storage(char *buf, size_t buf_size, size_t str_max_len, uint32_t addr);

    /* Read a string from storage and write it to a buffer.
     *
     * Returns: number of bytes read if successful (including null terminator),
     * negative integer code otherwise (see the preprocessor macros list).
     *
     * This overload uses buf_size sufficiency check based on
     * a preliminary counting of valid bytes to be read from storage,
     * not on a max string length constraint. Use with caution.
     */
    int32_t read_string_from_storage(char *buf, size_t buf_size, uint32_t addr);

    /* Write a string to storage.
     *
     * Returns: number of bytes written if successful (including null terminator),
     * negative integer code otherwise (see the preprocessor macros list).
     */
    int32_t write_string_to_storage(const char *str, size_t str_max_len, uint32_t addr);
}


#endif  // Include guards.
