/*************** FILE DESCRIPTION ***************/

/**
 * Filename: inbuilt_storage.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Writing and reading C-style (null-terminated) strings
 *           to and from inbuilt storage using the EEPROM.h library.
 *           Intended for use with the Arduino framework.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
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
            #ifdef INBUILT_STORAGE_VERBOSE_MODE
                Serial.println("Inbuilt storage error: storage initialization failed.");
            #endif
            return false;
        }
    }
#endif

int32_t inbuilt_storage::read_string_from_storage(char *buf, size_t buf_size, size_t str_max_len, size_t addr)
{
    if (buf == nullptr) {
        #ifdef INBUILT_STORAGE_VERBOSE_MODE
            Serial.println("Inbuilt storage error: null pointer detected.");
        #endif
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    #if defined ARDUINO_ARCH_ESP32 || defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_STM32
        size_t storage_size = EEPROM.length();
        if (addr + (str_max_len + 1) > storage_size) {
            #ifdef INBUILT_STORAGE_VERBOSE_MODE
                Serial.println("Inbuilt storage error: address is out of bounds.");
            #endif
            return INBUILT_STORAGE_ERR_ADDR_OUT_OF_BOUNDS;
        }
    #endif

    if (buf_size < str_max_len + 1) {
        #ifdef INBUILT_STORAGE_VERBOSE_MODE
            Serial.println("Inbuilt storage error: insufficient buffer size.");
        #endif
        return INBUILT_STORAGE_ERR_ARR_SIZE;
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

int32_t inbuilt_storage::read_string_from_storage(char *buf, size_t buf_size, size_t addr)
{
    if (buf == nullptr) {
        #ifdef INBUILT_STORAGE_VERBOSE_MODE
            Serial.println("Inbuilt storage error: null pointer detected.");
        #endif
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    size_t bytes_to_read = 0;
    size_t _addr = addr;
    uint8_t c = 0;
    while (true) {
        c = EEPROM.read(_addr);

        // In this context any non-ASCII value is considered to be a garbage value.
        if (c > INBUILT_STORAGE_HIGHEST_ASCII_CODE) {
            #ifdef INBUILT_STORAGE_VERBOSE_MODE
                Serial.println("Inbuilt storage error: invalid byte read.");
            #endif
            return INBUILT_STORAGE_ERR_INVALID_BYTE;
        }

        ++bytes_to_read;
        ++_addr;

        if (bytes_to_read > buf_size) {
            #ifdef INBUILT_STORAGE_VERBOSE_MODE
                Serial.println("Inbuilt storage error: insufficient buffer size.");
            #endif
            return INBUILT_STORAGE_ERR_ARR_SIZE;
        }

        if (c == '\0') {
            break;
        }
    }

    #if defined ARDUINO_ARCH_ESP32 || defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_STM32
        size_t storage_size = EEPROM.length();
        if (addr + bytes_to_read > storage_size) {
            #ifdef INBUILT_STORAGE_VERBOSE_MODE
                Serial.println("Inbuilt storage error: address is out of bounds.");
            #endif
            return INBUILT_STORAGE_ERR_ADDR_OUT_OF_BOUNDS;
        }
    #endif

    size_t i = 0;
    for (; i < bytes_to_read; ++i, ++addr) {
        buf[i] = EEPROM.read(addr);
    }

    return static_cast<int32_t>(i);
}

int32_t inbuilt_storage::write_string_to_storage(const char *str, size_t str_max_len, size_t addr)
{
    if (str == nullptr) {
        #ifdef INBUILT_STORAGE_VERBOSE_MODE
            Serial.println("Inbuilt storage error: null pointer detected.");
        #endif
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    #if defined ARDUINO_ARCH_ESP32 || defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_STM32
        size_t storage_size = EEPROM.length();
        if (addr + (str_max_len + 1) > storage_size) {
            return INBUILT_STORAGE_ERR_ADDR_OUT_OF_BOUNDS;
        }
    #endif

    size_t str_len = strlen(str);
    if (str_len > str_max_len) {
        #ifdef INBUILT_STORAGE_VERBOSE_MODE
            Serial.println("Inbuilt storage error: string is too long.");
        #endif
        return INBUILT_STORAGE_ERR_ARR_SIZE;
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
        if (!EEPROM.commit()) {
            return INBUILT_STORAGE_ERR_FAILED_COMMIT;
        }
    #endif

    return static_cast<int32_t>(i);
}

int32_t inbuilt_storage::write_string_to_storage(const char *str, size_t addr)
{
    if (str == nullptr) {
        #ifdef INBUILT_STORAGE_VERBOSE_MODE
            Serial.println("Inbuilt storage error: null pointer detected.");
        #endif
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    #if defined ARDUINO_ARCH_ESP32 || defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_STM32
        size_t str_len = strlen(str);
        size_t storage_size = EEPROM.length();
        if (addr + (str_len + 1) > storage_size) {
            return INBUILT_STORAGE_ERR_ADDR_OUT_OF_BOUNDS;
        }
    #endif

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
        if (!EEPROM.commit()) {
            return INBUILT_STORAGE_ERR_FAILED_COMMIT;
        }
    #endif

    return static_cast<int32_t>(i);
}
