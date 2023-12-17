/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: HW_UART.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: обмен данными через аппаратный интерфейс UART.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Основная библиотека Arduino.
#include <Arduino.h>

// Локальные модули.
#include "HW_UART.h"


/******************** ФУНКЦИИ *******************/

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
