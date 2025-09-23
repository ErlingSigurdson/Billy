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
 *
 *           Portability note for STM32 devices
 *           Check if the Arduino core you're using exposes EEPROM.commit()
 *           method. If it's unavailable, remove its call from the code.
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
#define INBUILT_STORAGE_ERR_NULLPTR            -1
#define INBUILT_STORAGE_ERR_ADDR_OUT_OF_BOUNDS -2
#define INBUILT_STORAGE_ERR_ARR_SIZE           -3
#define INBUILT_STORAGE_ERR_INVALID_BYTE       -4
#define INBUILT_STORAGE_ERR_FAILED_COMMIT      -5

// In a particular context all non-ASCII values are considered to be garbage values.
#define INBUILT_STORAGE_HIGHEST_ASCII_CODE 127

// Comment out to prevent Serial.println() calls that output error messages.
#define INBUILT_STORAGE_ERROR_OUTPUT_MODE


/************** FUNCTION PROTOTYPES *************/

namespace inbuilt_storage {

    /* Initialize storage.
     *
     * Returns: true if successful, false otherwise.
     *
     * Necessary for ESP32, ESP8266 and STM32 devices. Not to be used with AVR devices
     * Call it just once before you start using the storage.
     *
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
    int32_t read_string_from_storage(char *buf, size_t buf_size, size_t str_max_len, size_t addr);

    /* Read a string from storage and write it to a buffer.
     *
     * Returns: number of bytes read if successful (including null terminator),
     * negative integer code otherwise (see the preprocessor macros list).
     *
     * This overload uses buf_size sufficiency check based on
     * a preliminary counting of valid bytes to be read from storage,
     * not on a max string length constraint. "Valid bytes" in this
     * context means ASCII codes. Use with caution.
     */
    int32_t read_string_from_storage(char *buf, size_t buf_size, size_t addr);

    /* Write a string to storage.
     *
     * Returns: number of bytes written if successful (including null terminator),
     * negative integer code otherwise (see the preprocessor macros list).
     */
    int32_t write_string_to_storage(const char *str, size_t str_max_len, size_t addr);

    /* Write a string to storage.
     *
     * Returns: number of bytes written if successful (including null terminator),
     * negative integer code otherwise (see the preprocessor macros list).
     *
     * This overload omits the string length check. Use with caution.
     */
    int32_t write_string_to_storage(const char *str,  size_t addr);
}


#endif  // Include guards.
