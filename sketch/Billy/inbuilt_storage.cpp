/*************** FILE DESCRIPTION ***************/

/**
 * Filename: inbuilt_storage.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Writing and reading strings to and from
 *           an inbuilt storage using EEPROM.h library.
 *           Intended for use with the Arduino framework.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    AVR devices use an inbuilt EEPROM.
 *           ESP32, ESP8266 and STM32 devices emulate EEPROM in a flash memory.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "inbuilt_storage.h"

// Arduino core header file.
#include <Arduino.h>

// Additional Arduino libraries.
#include <EEPROM.h>


/******************* FUNCTIONS ******************/

/* A conditional compilation is used because the EEPROM.h
 * variant for AVR devices lacks the respective method.
 */
#ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32_PRESUMABLY
bool inbuilt_storage::init(uint32_t emulated_eeprom_size)
{
    if (EEPROM.begin(emulated_eeprom_size)) {
        return true;
    } else {
        Serial.println("Inbuilt storage: storage initialization error.");
        return false;
    }
}
#endif

int32_t inbuilt_storage::read_string_from_storage(char *buf, size_t buf_size, size_t str_max_len, uint32_t addr)
{
    if (buf == nullptr) {
        Serial.println("Inbuilt storage: nullptr error.");
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    if (buf_size < str_max_len + 1) {
        Serial.println("Inbuilt storage: buffer size error.");
        return INBUILT_STORAGE_ERR_SIZE;
    }

    size_t i = 0;
    while (true) {
        buf[i] = EEPROM.read(addr);

        if (buf[i] == '\0') {
            break;
        }

        if (i == str_max_len) {
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
        Serial.println("Inbuilt storage: nullptr error.");
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    size_t bytes_to_read = 0;
    uint32_t _addr = addr;
    int32_t c = 0;
    while (true) {
        c = EEPROM.read(_addr);

        if (c == '\0') {
            ++bytes_to_read;
            break;
        }

        if (c < '\0' || c > ASCII_CODE_HIGHEST) {
            Serial.println("Inbuilt storage: invalid byte read error.");
            //break;
            return INBUILT_STORAGE_ERR_BYTE;
        }

        ++bytes_to_read;
        ++_addr;
    }

    if (buf_size < bytes_to_read) {
        Serial.println("Inbuilt storage: buffer size error.");
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
        Serial.println("Inbuilt storage: nullptr error.");
        return INBUILT_STORAGE_ERR_NULLPTR;
    }

    size_t str_len = strlen(str);
    if (str_len > str_max_len) {
        Serial.println("Inbuilt storage: string length error.");
        return INBUILT_STORAGE_ERR_SIZE;
    }

    size_t i = 0;
    for (; str[i] != '\0'; ++i, ++addr) {
        EEPROM.write(addr, str[i]);
    }
    EEPROM.write(addr, '\0');

    /* A conditional compilation is used because the EEPROM.h
     * variant for AVR devices lacks the respective method.
     */
    #ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32_PRESUMABLY
        EEPROM.commit();
    #endif

    return static_cast<int32_t>(i);
}
