/*************** FILE DESCRIPTION ***************/

/**
 * Filename: inbuilt_storage.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Writing and reading strings to and from
 *           inbuilt storage using EEPROM.h library.
 *           Intended for use with the Arduino framework.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    AVR devices use inbuilt EEPROM.
 *           ESP32, ESP8266 and STM32 devices emulate EEPROM in flash memory.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "inbuilt_storage.h"

// Arduino core header file.
#include <Arduino.h>

// Additional Arduino libraries.
#include <EEPROM.h>


/*--- Misc ---*/

// Use update() method instead of write() method for AVR devices to reduce EEPROM wear out.
#if defined ARDUINO_ARCH_AVR
    #define INBUILT_STORAGE_WRITE(x, y) EEPROM.update(x, y)
#else
    #define INBUILT_STORAGE_WRITE(x, y) EEPROM.write(x, y)
#endif


/******************* FUNCTIONS ******************/

/* Conditional compilation is used because the EEPROM.h
 * variant for AVR devices lacks the respective method.
 */
#if defined ARDUINO_ARCH_ESP32 || defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_STM32
    bool inbuilt_storage::init(uint32_t emulated_eeprom_size)
    {
        if (EEPROM.begin(emulated_eeprom_size)) {
            return true;
        } else {
            Serial.println("Inbuilt storage error: storage initialization failed.");
            return false;
        }
    }
#endif

int32_t inbuilt_storage::read_string_from_storage(char *buf, size_t buf_size, size_t str_max_len, uint32_t addr)
{
    if (buf == nullptr) {
        Serial.println("Inbuilt storage error: nullptr detected.");
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    if (buf_size < str_max_len + 1) {
        Serial.println("Inbuilt storage error: insufficient buffer size.");
        return INBUILT_STORAGE_ERR_SIZE;
    }

    size_t i = 0;
    while (true) {
        buf[i] = EEPROM.read(addr);

        if (buf[i] == '\0' || i == str_max_len) {
            buf[i] = '\0';
            break;
        }

        ++i;
        ++addr;
    }

    return static_cast<int32_t>(i);
}

int32_t inbuilt_storage::read_string_from_storage(char *buf, size_t buf_size, uint32_t addr)
{
    if (buf == nullptr) {
        Serial.println("Inbuilt storage error: nullptr detected.");
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    size_t bytes_to_read = 0;
    uint32_t _addr = addr;
    int32_t c = 0;
    while (true) {
        c = EEPROM.read(_addr);

        /* Technically value read from EEPROM can't be negative,
         * but second condition is preserved regardless, just in case.
         */
        if (c > ASCII_CODE_HIGHEST || c < '\0') {
            Serial.println("Inbuilt storage error: invalid byte read.");
            return INBUILT_STORAGE_ERR_BYTE;
        }

        if (bytes_to_read > buf_size) {
            Serial.println("Inbuilt storage error: no null terminator encountered.");
            return INBUILT_STORAGE_ERR_NO_TERMINATOR;
        }

        if (c == '\0') {
            ++bytes_to_read;
            break;
        }

        ++bytes_to_read;
        ++_addr;
    }

    if (buf_size < bytes_to_read) {
        Serial.println("Inbuilt storage error: insufficient buffer size.");
        return INBUILT_STORAGE_ERR_SIZE;
    }

    size_t i = 0;
    for (; i < bytes_to_read; ++i, ++addr) {
        buf[i] = EEPROM.read(addr);
    }

    return static_cast<int32_t>(i);
}

int32_t inbuilt_storage::write_string_to_storage(const char *str, size_t str_max_len, uint32_t addr)
{
    if (str == nullptr) {
        Serial.println("Inbuilt storage error: nullptr detected.");
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    size_t str_len = strlen(str);
    if (str_len > str_max_len) {
        Serial.println("Inbuilt storage error: string is too long.");
        return INBUILT_STORAGE_ERR_SIZE;
    }

    size_t i = 0;
    for (; str[i] != '\0'; ++i, ++addr) {
        INBUILT_STORAGE_WRITE(addr, str[i]);
    }
    INBUILT_STORAGE_WRITE(addr, '\0');
    ++i;  // To return the correct count of bytes written (including null terminator).

    /* Conditional compilation is used because the EEPROM.h
     * variant for AVR devices lacks the respective method.
     */
    #if defined ARDUINO_ARCH_ESP32 || defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_STM32
        EEPROM.commit();
    #endif

    return static_cast<int32_t>(i);
}
