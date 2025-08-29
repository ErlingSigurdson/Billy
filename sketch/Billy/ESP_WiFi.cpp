/*************** FILE DESCRIPTION ***************/

/**
 * Filename: ESP_WiFi.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Wi-Fi wrapper functions.
 *           Written for use with the ESP32/ESP8266 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Source file's own header.
#include "ESP_WiFi.h"

// Essential Arduino library.
#include <Arduino.h>

// Additional libraries for Arduino IDE.
#ifdef ESP32
    #include <WiFi.h>
#endif
#ifdef ESP8266
    #include <ESP8266WiFi.h>
#endif

// Local modules.
#include "config_general.h"


/******************* FUNCTIONS ******************/

bool ESP_WiFi_set_connection(char *SSID, char *pswd, uint32_t conn_attempt_timeout)
{
    if (WiFi.isConnected()) {
        WiFi.disconnect();
    }

    while (WiFi.status() == WL_CONNECTED) {  // A pause to ensure a termination of the previous connection.
        yield();                             // Avoiding a reset caused by the watchdog timer.
    }

    Serial.print("Connecting to Wi-Fi access point ");
    Serial.println(SSID);

    WiFi.begin(SSID, pswd);

    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_FAILED_CONN_ATTEMPT_PAUSE);
        Serial.print(".");

        if (WIFI_INDICATOR_LED_PIN > 0) {
            digitalWrite(WIFI_INDICATOR_LED_PIN, !digitalRead(WIFI_INDICATOR_LED_PIN));
        }

        delay(WIFI_INDICATE_CONNECTION_PERIOD);

        if (WIFI_INDICATOR_LED_PIN > 0) {
            digitalWrite(WIFI_INDICATOR_LED_PIN, !digitalRead(WIFI_INDICATOR_LED_PIN));
        }

        current_millis = millis();
        if (current_millis - previous_millis >= conn_attempt_timeout) {
            Serial.println("");
            Serial.println("Connection timeout. Please set valid SSID and password and/or restart the connection.");

            return 0;
        }
    }

    Serial.println("");
    Serial.println("Connected to Wi-Fi network!");

    return 1;
}

void ESP_WiFi_indicate_connection(uint32_t led_pin, uint32_t cycles, uint32_t period)
{
    if (led_pin == 0) {
        return;
    }

    if (cycles % 2) {
        ++cycles;
    }

    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;

    for (uint32_t i = 0; i < cycles; ++i) {
        digitalWrite(led_pin, !digitalRead(led_pin));

        while (current_millis - previous_millis < period) {
            current_millis = millis();
        }
        previous_millis = current_millis;
    }
}

String ESP_WiFi_get_devices_current_IP()
{
    return WiFi.localIP().toString();
}

void ESP_WiFi_RSSI_output()
{
    int64_t RSSI = WiFi.RSSI();
    Serial.print("Current RSSI is: ");
    Serial.println(RSSI);
}

bool ESP_WiFi_is_connected()
{
    if (WiFi.status() == WL_CONNECTED) {
        return 1;
    } else {
        return 0;
    }
}
