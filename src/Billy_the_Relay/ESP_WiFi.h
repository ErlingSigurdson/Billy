/****************** DESCRIPTION *****************/

/**
 * Filename: ESP_WiFi.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: управление встроенным интерфейсом WiFi для модулей
 * ESP32 и ESP8266.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP_WIFI_H
#define ESP_WIFI_H

// Прочее.
#define DEFAULT_WIFI_RSSI_PRINT_PERIOD 2000


/************** FUNCTION PROTOTYPES *************/

bool ESP_WiFi_set_connection(char *SSID, char *pswd, uint32_t conn_attempt_timeout);
void ESP_WiFi_indicate_connection(uint32_t LED_pin, uint32_t cycles, uint32_t period);
String ESP_WiFi_get_current_IP();
void ESP_WiFi_RSSI_print();


#endif  // Include guards.
