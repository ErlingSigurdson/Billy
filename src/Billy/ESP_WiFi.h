/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_WiFi.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: Wi-Fi-related functions.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP_WIFI_H
#define ESP_WIFI_H


/*--- Includes ---*/

// Main Arduino library.
#include <Arduino.h>


/*--- Misc ---*/

#define DEFAULT_WIFI_RSSI_OUTPUT_PERIOD 2000


/************** FUNCTION PROTOTYPES *************/

bool ESP_WiFi_set_connection(char *SSID, char *pswd, uint32_t conn_attempt_timeout);
void ESP_WiFi_indicate_connection(uint32_t LED_pin, uint32_t cycles, uint32_t period);
String ESP_WiFi_get_devices_current_IP();
void ESP_WiFi_RSSI_print();
bool ESP_WiFi_is_connected();


#endif  // Include guards.
