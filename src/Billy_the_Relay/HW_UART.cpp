/****************** DESCRIPTION *****************/

/**
 * Filename: HW_UART.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: обмен данными через аппаратный интерфейс UART.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// General Arduino library.
#include <Arduino.h>

// Local modules.
#include "HW_UART.h"


/******************* FUNCTIONS ******************/

uint32_t HW_UART_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout, uint32_t read_slowdown)
{
    // Счётчик таймаута подключения.
    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;

    uint32_t i = 0, j = 0;
    bool lf = 0;
    while (Serial.available() && current_millis - previous_millis < conn_timeout && !lf) {
        char c = Serial.read();
        ++i;

        if (j < str_max_len) {
                buf[j] = c;
            ++j;
        }

        if (c == '\n') {
            lf = 1;
        }

        current_millis = millis();

        if (read_slowdown > 0) {
            delay(read_slowdown);  // Чтобы байты из буфера не считывались быстрее, чем они туда поступают.
        }
    }

    return i;
}
