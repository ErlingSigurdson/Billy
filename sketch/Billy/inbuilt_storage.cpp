/****************** DESCRIPTION *****************/

/**
 * Filename: inbuilt_storage.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Reading and writing strings to and from an inbuilt storage
 *           using EEPROM.h library.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:    AVR devices use inbuilt EEPROM.
 *           ESP32, ESP8266 and STM32 emulate EEPROM
 *           in their inbuilt flash memory.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_inbuilt_storage.h"

// Additional libraries for Arduino IDE.
#include <EEPROM.h>

// Local modules.
#include "inbuilt_storage.h"


/******************* FUNCTIONS ******************/

void inbuilt_storage_init(uint32_t emulated_EEPROM_size)
{
    /* Conditional compilation is used because the EEPROM.h variant
     * for AVR devices just lacks the respective method.
     */
    #ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32_PRESUMABLY
        EEPROM.begin(emulated_EEPROM_size);
    #endif
}

void inbuilt_storage_read_string(char *buf, size_t buf_size, uint32_t str_max_len, uint32_t addr)
{
    if (buf_size < str_max_len + 1) {
        Serial.println("Error reading from inbuilt storage.");
        return;
    }

    for (uint32_t i = 0; i < str_max_len; ++i, ++addr) {
        buf[i] = EEPROM.read(addr);

        if (buf[i] == '\0') {
            break;
        }
    }
}

void inbuilt_storage_write_string(char *str, uint32_t str_len, uint32_t str_max_len, uint32_t addr)
{
    if (str_len > str_max_len) {
        Serial.println("Error reading from inbuilt storage.");

        return;
    }

    char buf[INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
    strcpy(buf, str);

    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';
        }
    }

    for (uint32_t i = 0; buf[i] != '\0'; ++i, ++addr) {
        EEPROM.write(addr, buf[i]);
    }
    EEPROM.write(addr, '\0');

    /* Conditional compilation is used because the EEPROM.h variant
     * for AVR devices just lacks the respective method.
     */
    #ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32_PRESUMABLY
        EEPROM.commit();
    #endif
}
