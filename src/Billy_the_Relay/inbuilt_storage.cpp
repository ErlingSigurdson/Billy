/****************** DESCRIPTION *****************/

/**
 * Filename: inbuilt_storage.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: чтение и запись строк во встроенный накопитель устройства
 * с помощью библиотеки EEPROM.h.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 * У микроконтроллеров AVR используется встроенная EEPROM.
 * У микроконтроллеров ESP32, ESP8266 и STM32 используется флеш-память
 * с эмуляцией EEPROM.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// General Arduino library.
#include <Arduino.h>

// Local modules.
#include "inbuilt_storage.h"

// Additional libraries for Arduino IDE.
#include <EEPROM.h>


/******************* FUNCTIONS ******************/

// Инициализация хранилища. Требуется для ESP32, ESP8266 и STM32. Не требуется для AVR.
void inbuilt_storage_init(uint32_t emulated_EEPROM_size)
{
    /* Определяется на этапе компиляции, поскольку вариант библиотеки EEPROM.h
     * для AVR попросту не содержит соответствующего метода.
     */
    #ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32
        EEPROM.begin(emulated_EEPROM_size);
    #endif
}

// Чтение строки.
void inbuilt_storage_read(char *buf, size_t buf_size, uint32_t str_max_len, uint32_t addr)
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

// Запись строки.
void inbuilt_storage_write(char *str, uint32_t str_len, uint32_t str_max_len, uint32_t addr)
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

    /* Определяется на этапе компиляции, поскольку вариант библиотеки EEPROM.h
     * для AVR попросту не содержит соответствующего метода.
     */
    #ifdef THIS_IS_ESP32_OR_ESP8266_OR_STM32
        EEPROM.commit();
    #endif
}
