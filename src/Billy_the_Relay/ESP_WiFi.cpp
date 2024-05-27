/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_WiFi.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: Wi-Fi-related functions.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// General Arduino library.
#include <Arduino.h>

// Local modules.
#include "ESP_WiFi.h"
#include "config_general.h"

// Additional libraries for Arduino IDE.
#ifdef ESP32
    #include <WiFi.h>
#endif
#ifdef ESP8266
    #include <ESP8266WiFi.h>
#endif


/******************* FUNCTIONS ******************/

bool ESP_WiFi_set_connection(char *SSID, char *pswd, uint32_t conn_attempt_timeout)
{
    if (WiFi.isConnected()) {
        WiFi.disconnect();
    }
    while (WiFi.isConnected()) {                 // Pause to ensure termination of a previous connection.
        yield();                                 // Avoid a reset caused by the watchdog timer.
    }

    Serial.print("Connecting to Wi-Fi access point ");
    Serial.println(SSID);

    WiFi.begin(SSID, pswd);
    uint64_t current_millis = millis();
    uint64_t previous_millis = current_millis;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);                             // Once a second.
        Serial.print(".");
        digitalWrite(WIFI_INDICATOR_LED_PIN, DIGITAL_LOAD_ON);
        delay(WIFI_INDICATE_CONNECTION_PERIOD);
        digitalWrite(WIFI_INDICATOR_LED_PIN, DIGITAL_LOAD_OFF);

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

String ESP_WiFi_get_current_IP()
{
    return WiFi.localIP().toString();
}

void ESP_WiFi_RSSI_print()
{
    int64_t RSSI = WiFi.RSSI();
    Serial.print("Current RSSI is: ");
    Serial.println(RSSI);
}
