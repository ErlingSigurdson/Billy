/*************** FILE DESCRIPTION ***************/

/**
 * Filename: esp32_btclassic_billy.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Bluetooth Classic wrapper functions for ESP32 SoC.
 *           Intended for use with the ESP32 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "src/esp32_btclassic_billy.h"

// Project configs.
#include "src/config_general.h"

// Arduino core header file.
#include <Arduino.h>

// Conditional compilation.
#if defined ESP32 && defined BTCLASSIC_USED

// Additional Arduino libraries.
#include <BluetoothSerial.h>


/*************** GLOBAL VARIABLES ***************/

BluetoothSerial BTClassic_Serial;


/******************* FUNCTIONS ******************/

void esp32_btclassic_billy::start(const char *dev_name)
{
    BTClassic_Serial.begin(dev_name);
}

bool esp32_btclassic_billy::check_connection()
{
    return BTClassic_Serial.connected();
}

uint32_t esp32_btclassic_billy::read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout, uint32_t read_slowdown)
{
    // Connection timeout counter.
    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;

    uint32_t i = 0, j = 0;
    bool lf = 0;
    while (buf[0] == '\0' && current_millis - previous_millis < conn_timeout && !lf) {
        while (BTClassic_Serial.available() && current_millis - previous_millis < conn_timeout && !lf) {
            char c = BTClassic_Serial.read();
            ++i;

            if (j < str_max_len) {
                buf[j] = c;
                ++j;
            }

            if (c == '\n') {
                lf = 1;
            }

            if (read_slowdown > 0) {
                // A pause that prevents reading from a buffer ahead of writing to it.
                delay(read_slowdown);
            }

            current_millis = millis();
        }
        current_millis = millis();
    }

    return i;
}

void esp32_btclassic_billy::send_msg(const char *msg)
{
    BTClassic_Serial.println(msg);
}

void esp32_btclassic_billy::disconnect(uint32_t shutdown_downtime)
{
    delay(shutdown_downtime);
    BTClassic_Serial.disconnect();
}

void esp32_btclassic_billy::stop(uint32_t shutdown_downtime)
{
    delay(shutdown_downtime);
    BTClassic_Serial.end();
}


#endif  // Conditional compilation.
