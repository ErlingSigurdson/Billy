/*************** FILE DESCRIPTION ***************/

/**
 * Filename: inbuilt_storage.cpp
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

/*--- Includes ---*/

// This source file's own header file.
#include "inbuilt_storage.h"

// Arduino core header file.
#include <Arduino.h>

// Additional Arduino libraries.
#include <EEPROM.h>


/******************* FUNCTIONS ******************/

/* Conditional compilation is used because the EEPROM.h variant
 * for AVR devices just lacks the respective method.
 */
#ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32_PRESUMABLY
void inbuilt_storage::init(uint32_t emulated_eeprom_size)
{
    EEPROM.begin(emulated_eeprom_size);
}
#endif

void inbuilt_storage::read_string_from_storage(char *buf, size_t buf_size, uint32_t str_max_len, uint32_t addr)
{
    if (buf == nullptr) {
        return;
    }

    if (buf_size < str_max_len + 1) {
        Serial.println("Error reading from inbuilt storage.");
        return;
    }

    for (uint32_t i = 0; buf[i] != '\0' && i < str_max_len; ++i, ++addr) {
        buf[i] = EEPROM.read(addr);
    }
}

void inbuilt_storage::write_string_to_storage(const char *str, uint32_t str_max_len, uint32_t addr)
{
    if (str == nullptr) {
        return;
    }

    size_t str_len = strlen(str);
    if (str_len > str_max_len) {
        Serial.println("Error reading from inbuilt storage.");
        return;
    }

    for (uint32_t i = 0; str[i] != '\0'; ++i, ++addr) {
        EEPROM.write(addr, str[i]);
    }
    EEPROM.write(addr, '\0');

    /* Conditional compilation is used because the EEPROM.h variant
     * for AVR devices just lacks the respective method.
     */
    #ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32_PRESUMABLY
        EEPROM.commit();
    #endif
}
