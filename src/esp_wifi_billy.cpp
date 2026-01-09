/*************** FILE DESCRIPTION ***************/

/**
 * Filename: esp_wifi_billy.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Wi-Fi wrapper functions.
 *           Intended for use with the ESP32/ESP8266 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "esp_wifi_billy.h"

// Project configs.
#include "../config_general.h"

// Arduino core header file.
#include <Arduino.h>

// Additional Arduino libraries.
#ifdef ESP32
    #include <WiFi.h>
#endif
#ifdef ESP8266
    #include <ESP8266WiFi.h>
#endif


/******************* FUNCTIONS ******************/

bool esp_wifi_billy::set_connection(char *SSID, char *pswd, uint32_t conn_attempt_timeout)
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

void esp_wifi_billy::indicate_connection(uint32_t led_pin, uint32_t cycles, uint32_t period)
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

String esp_wifi_billy::get_devices_current_ip()
{
    return WiFi.localIP().toString();
}

void esp_wifi_billy::rssi_output()
{
    int64_t RSSI = WiFi.RSSI();
    Serial.print("Current RSSI is: ");
    Serial.println(RSSI);
}

bool esp_wifi_billy::is_connected()
{
    if (WiFi.status() == WL_CONNECTED) {
        return 1;
    } else {
        return 0;
    }
}
