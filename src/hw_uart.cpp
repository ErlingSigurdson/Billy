/*************** FILE DESCRIPTION ***************/

/**
 * Filename: hw_uart.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Hardware UART wrapper functions.
 *           Intended for use with the Arduino framework.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "hw_uart.h"

// Arduino core header file.
#include <Arduino.h>


/******************* FUNCTIONS ******************/

void hw_uart::setup(uint32_t baud_rate, uint32_t antinoise_pause, uint32_t startup_pause, const char *startup_msg)
{
    delay(antinoise_pause);     // Wait for an input noise to stop.
    Serial.begin(baud_rate);
    delay(startup_pause);       // A tiny pause to allow for an interface startup.
    hw_uart::print(startup_msg);
}

void hw_uart::print(const char *str)
{
    Serial.print(str);
}

void hw_uart::flush()
{
    Serial.flush();
}

uint32_t hw_uart::read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout, uint32_t read_slowdown)
{
    // Connection timeout counter.
    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;

    uint32_t i = 0, j = 0;
    bool newline = false;
    while (Serial.available() && current_millis - previous_millis < conn_timeout && !newline) {
        char c = Serial.read();
        ++i;

        if (j < str_max_len) {
            buf[j] = c;
            ++j;
        }

        if (c == '\n') {
            newline = true;
        }

        if (read_slowdown > 0) {
            // A pause that prevents reading from a buffer ahead of writing to it.
            delay(read_slowdown);
        }

        current_millis = millis();
    }

    return i;
}
