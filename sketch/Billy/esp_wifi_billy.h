/*************** FILE DESCRIPTION ***************/

/**
 * Filename: esp_wifi_billy.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Wi-Fi wrapper functions.
 *           Intended for use with the ESP32/ESP8266 Arduino core.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP_WIFI_H
#define ESP_WIFI_H


/*--- Includes ---*/

// Arduino core header file.
#include <Arduino.h>


/************** FUNCTION PROTOTYPES *************/

namespace esp_wifi_billy {
    bool set_connection(char *SSID, char *pswd, uint32_t conn_attempt_timeout);
    void indicate_connection(uint32_t LED_pin, uint32_t cycles, uint32_t period);
    String get_devices_current_ip();
    void rssi_output();
    bool is_connected();
}

#endif  // Include guards.
