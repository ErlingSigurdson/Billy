/****************** DESCRIPTION *****************/

/**
 * Filename: ESP32_Bluetooth.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: Bluetooth Classic-related functions for ESP32 modules.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// General Arduino library.
#include <Arduino.h>

// Conditional compilation.
#if defined ESP32 && defined BT_CLASSIC_PROVIDED

// Additional libraries for Arduino IDE.
#include <BluetoothSerial.h>

// Local modules.
#include "ESP32_Bluetooth.h"


/*************** GLOBAL VARIABLES ***************/

BluetoothSerial BT_Serial;


/******************* FUNCTIONS ******************/

void ESP32_BT_start(char *dev_name)
{
    BT_Serial.begin(dev_name);
}

bool ESP32_BT_check_connection()
{
    return BT_Serial.connected();
}

uint32_t ESP32_BT_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout)
{
    // Connection timeout counter.
    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;

    uint32_t i = 0, j = 0;
    bool lf = 0;
    while (buf[0] == '\0' && current_millis - previous_millis < conn_timeout && !lf) {
        while (BT_Serial.available() && current_millis - previous_millis < conn_timeout && !lf) {
            char c = BT_Serial.read();
            ++i;

            if (j < str_max_len) {
                buf[j] = c;
                ++j;
            }

            if (c == '\n') {
                lf = 1;
            }

            current_millis = millis();

            #define BT_READ_SLOWDOWN 2
            delay(BT_READ_SLOWDOWN);    /* Pause to ensure that reading from the buffer
                                         * won't run ahead of writing to it.
                                         */
        }
        current_millis = millis();
    }

    return i;
}

void ESP32_BT_send_msg(const char *msg)
{
    if (BT_Serial.connected()) {
        BT_Serial.println(msg);
    }
}

void ESP32_BT_disconnect(uint32_t shutdown_downtime)
{
    delay(shutdown_downtime);
    BT_Serial.disconnect();
}

void ESP32_BT_stop(uint32_t shutdown_downtime)
{
    delay(shutdown_downtime);
    BT_Serial.end();
}

#endif  // Conditional compilation.
