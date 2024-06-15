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
 #include "cmd.h"
 #include "inbuilt_storage.h"
 #include "stored_configs.h"
 #include "HW_UART.h"
 #include "ESP_WiFi.h"
 #include "ESP_TCP.h"
 #include "ESP_HTTP.h"

 #if defined ESP32 && defined BT_CLASSIC_PROVIDED
     #include "ESP32_Bluetooth_Classic.h"
 #endif


 /******************* FUNCTIONS ******************/



     // Check for IoT mode flag.
    Serial.print("Requests to IoT server: ");
    if (stored_configs.IoT_flag != 0) {
        Serial.println("ON");
        Serial.print("Target IoT server IP address: ");
        Serial.println(stored_configs.IoT_server_IP);
        Serial.print("Target IoT server port: ");
        Serial.println(stored_configs.IoT_server_port);
        Serial.print("Request text: ");
        Serial.println(stored_configs.IoT_req_msg);
        Serial.print("Request period (once per ms): ");
        Serial.println(stored_configs.IoT_req_period);
    } else {
        Serial.println("OFF");
    }

 