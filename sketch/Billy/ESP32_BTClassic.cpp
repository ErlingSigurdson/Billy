/****************** DESCRIPTION *****************/

/**
 * Filename: ESP32_Bluetooth.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Bluetooth Classic wrapper functions for ESP32 SoC.
 *           Written for use with the ESP32 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_general.h"

// Conditional compilation
/* ESP32 is the macro provided by the Arduino framework or the compiler
 * (not exactly sure by which one of those two) for the respective SoC family.
 */
#if defined ESP32 && defined BTCLASSIC_PROVIDED

// Additional libraries for Arduino IDE.
#include <BluetoothSerial.h>

// Local modules.
#include "ESP32_BTClassic.h"


/*************** GLOBAL VARIABLES ***************/

BluetoothSerial BTClassic_Serial;


/******************* FUNCTIONS ******************/

void ESP32_BTClassic_start(const char *dev_name)
{
    BTClassic_Serial.begin(dev_name);
}

bool ESP32_BTClassic_check_connection()
{
    return BTClassic_Serial.connected();
}

uint32_t ESP32_BTClassic_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout)
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

            current_millis = millis();

            delay(BTCLASSIC_READ_SLOWDOWN);    /* A pause to ensure that reading from a buffer
                                                * won't run ahead of writing to it.
                                                */
        }
        current_millis = millis();
    }

    return i;
}

void ESP32_BTClassic_send_msg(const char *msg)
{
    BTClassic_Serial.println(msg);
}

void ESP32_BTClassic_disconnect(uint32_t shutdown_downtime)
{
    delay(shutdown_downtime);
    BTClassic_Serial.disconnect();
}

void ESP32_BTClassic_stop(uint32_t shutdown_downtime)
{
    delay(shutdown_downtime);
    BTClassic_Serial.end();
}


#endif  // Conditional compilation.
