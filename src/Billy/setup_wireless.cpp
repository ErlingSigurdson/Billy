/****************** DESCRIPTION *****************/

/**
 * Filename: setup_wireless.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: setting up wireless connectivity.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/
 
/*--- Includes ---*/
 
// Main Arduino library.
#include <Arduino.h>
 
// Local modules.
#include "setup_wireless.h"
#include "utilities.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "ESP_HTTP.h"

#if defined ESP32 && defined BTCLASSIC_PROVIDED
    #include "ESP32_BTClassic.h"
#endif


/******************* FUNCTIONS ******************/

void setup_WiFi(stored_configs_t *stored_configs)
{
    // Connect to Wi-Fi network.
    bool WiFi_connected = ESP_WiFi_set_connection(stored_configs->local_SSID,
                                                  stored_configs->local_pswd,
                                                  CONN_TIMEOUT);

    if (WiFi_connected) {
        ESP_WiFi_indicate_connection(WIFI_INDICATOR_LED_PIN,
                                     WIFI_INDICATE_CONNECTION_CYCLES,
                                     WIFI_INDICATE_CONNECTION_PERIOD);

        Serial.print("Current local IP address is: ");
        Serial.println(ESP_WiFi_get_devices_current_IP());

        ESP_TCP_server_start();
        Serial.print("Local TCP server started at port ");
        Serial.println(stored_configs->local_server_port);

        ESP_HTTP_server_start();
        ESP_HTTP_set_handlers();
        Serial.print("Local HTTP server started at port ");
        Serial.println(HTTP_PORT);
    }
    Serial.println("");

    // Check for IoT mode flag.
    Serial.print("Requests to IoT server: ");
    if (stored_configs->IoT_flag != 0) {
        Serial.println("ON");
        Serial.print("Target IoT server IP address: ");
        Serial.println(stored_configs->IoT_server_IP);
        Serial.print("Target IoT server port: ");
        Serial.println(stored_configs->IoT_server_port);
        Serial.print("Request message: ");
        Serial.println(stored_configs->IoT_req_msg);
        Serial.print("Request period (once per ms): ");
        Serial.println(stored_configs->IoT_req_period);
    } else {
        Serial.println("OFF");
    }

    // Check for automatic reconnect attempts flag.
    Serial.print("Automatic reconnect attempts: ");
    if (stored_configs->local_autoreconnect_flag != 0) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }

    // Check for RSSI output flag.
    Serial.print("RSSI output: ");
    if (stored_configs->local_RSSI_output_flag != 0) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }

    Serial.println("");
}

void setup_BTClassic(stored_configs_t *stored_configs)
{
    
    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        // Check for Bluetooth Classic functionality flag.
        Serial.print("Bluetooth Classic: ");
        if (stored_configs->BTClassic_flag != 0) {
            Serial.println("ON");
            Serial.print("Bluetooth Classic device name: ");
            Serial.println(stored_configs->BTClassic_dev_name);
        } else {
            Serial.println("OFF");
        }

        // Bluetooth Classic startup.
        if (stored_configs->BTClassic_flag) {
            ESP32_BTClassic_start(stored_configs->BTClassic_dev_name);
        }
    #endif
}
