/****************** DESCRIPTION *****************/

/**
 * Filename: Billy.ino
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: the main file of an Arduino sketch written for ESP32 and ESP8266
 *          systems-on-chip (SoCs). See README files for more information.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes: project homepages https://github.com/ErlingSigurdson/Billy
 *                          https://gitflic.ru/efimov-d-v/billy
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Arduino.h, the essential Arduino library, is already included by default.

// Project configs.
#include "config_general.h"
#include "config_cmd.h"
#include "config_inbuilt_storage.h"

// Additional Arduino libraries are included in the local modules.

// Local modules.
#include "utilities.h"
#include "inbuilt_storage.h"
#include "stored_configs.h"
#include "HW_UART.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "ESP_HTTP.h"
#include "cmd.h"

#if defined ESP32 && defined BTCLASSIC_PROVIDED
    #include "ESP32_BTClassic.h"
#endif


/************** FUNCTION PROTOTYPES *************/

/*--- Wireless connectivity setup functions ---*/

void setup_WiFi(stored_configs_t *stored_configs);
void setup_BTClassic(stored_configs_t *stored_configs);


/*--- Command reception functions ---*/

// Put a data received by a hardware UART into the buffer.
void receive_cmd_HW_UART(char *buf);

// Local TCP server reads a data from a remote client.
void receive_cmd_TCP_local(char *buf);

/* Local TCP client sends a request to a remote server
 * and reads a response.
 */
void receive_cmd_TCP_IoT(char *buf, stored_configs_t *stored_configs);

// Handle HTTP request and write value from a request body into a buffer.
void receive_cmd_HTTP(char *buf);

// Read data from a Bluetooth master device.
void receive_cmd_BTClassic(char *buf, stored_configs_t *stored_configs, bool *BTClassic_was_connected);


/******************* FUNCTIONS ******************/

/*--- Basic functions ---*/

void setup()
{
    /*--- Starting hardware UART ---*/

    Serial.begin(HW_UART_BAUD_RATE);
    delay(HW_UART_STARTUP_PAUSE);     // A tiny pause to allow for the interface startup.

    Serial.println("");
    Serial.println("*** HELLO, HUMAN! ***");
    

    /*--- GPIO ---*/

    if (DIGITAL_OUTPUT_PIN == PWM_OUTPUT_PIN) {
        Serial.println("Warning! Digital and PWM output pins coincide. "
                       "It will most probably render digital control inoperable.");
    }

    // Configuration.
    pinMode(DIGITAL_OUTPUT_PIN, OUTPUT);
    pinMode(PWM_OUTPUT_PIN, OUTPUT);
    pinMode(WIFI_INDICATOR_LED_PIN, OUTPUT);

    // Setting digital outputs to respective initial digital levels.
    digitalWrite(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_LOAD_OFF);
    digitalWrite(WIFI_INDICATOR_LED_PIN, DIGITAL_OUTPUT_LOAD_OFF);


    /*--- Interaction with the inbuilt storage ---*/

    /* In contrast to AVR-based devices, ESP32 and ESP8266 SoCs'
     * inbuilt storage must be initialized before use.
     */
    inbuilt_storage_init(INBUILT_STORAGE_SIZE);

    // Read stored configs from the inbuilt storage into the struct.
    stored_configs_t stored_configs;
    stored_configs_read(&stored_configs);


    /*--- Starting wireless communications ---*/

    setup_WiFi(&stored_configs);
    setup_BTClassic(&stored_configs);

    Serial.println("");
    Serial.flush();
}

void loop()
{
    /*--- Interaction with inbuilt storage ---*/

    static bool time_to_refresh_stored_configs = 1;

    // Read config values from inbuilt storage into struct.
    static stored_configs_t stored_configs;
    if (time_to_refresh_stored_configs) {    // Check if update is necessary.
        stored_configs_read(&stored_configs);
        time_to_refresh_stored_configs = 0;
    }


    /*--- Array of valid commands ---*/

    static const char *cmd_list[] = {
        0,      // Placeholder to bump first command's index up to 1.
        CMD_1,
        CMD_2,
        CMD_3,
        CMD_4,
        CMD_5,
        CMD_6,
        CMD_7,
        CMD_8,
        CMD_9,
        CMD_10,
        CMD_11,
        CMD_12,
        CMD_13,
        CMD_14,
        CMD_15,
        CMD_16,
        CMD_17,
        CMD_18,
        CMD_19,
        CMD_20,
        CMD_21,
        CMD_22,
        CMD_23
    };


    /*--- Receiving commands ---*/

    // Command buffer.
    char terminal_input[STR_MAX_LEN + 1] = {0};

    bool BTClassic_was_connected = 0;

    receive_cmd_HW_UART(terminal_input);
    receive_cmd_TCP_local(terminal_input);
    receive_cmd_TCP_IoT(terminal_input, &stored_configs);
    receive_cmd_HTTP(terminal_input);
    receive_cmd_BTClassic(terminal_input, &stored_configs, &BTClassic_was_connected);


    /*--- Handling commands ---*/

    // Essentially it's the central hub of the whole sketch.

    // Check for a non-empty buffer string.
    if (terminal_input[0] != '\0' ) {
        utilities_nullify_first_CR_or_LF_in_string(terminal_input);

        // Check for valid commands.
        int32_t func_to_call = cmd_check(terminal_input, CMD_PREFIX, cmd_list, CMD_LIST_LEN);
        switch (func_to_call) {
            case -1:
                cmd_handler_err_prefix();
                break;

            case 0:
                cmd_handler_err_cmd();
                break;

            case 1:
                cmd_handler_set_load_digital(terminal_input);
                break;

            case 2:
                cmd_handler_set_load_PWM(terminal_input);
                break;

            case 3:
                cmd_handler_output_load_digital();
                break;

            case 4:
                cmd_handler_set_WiFi_SSID(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 5:
                cmd_handler_output_WiFi_SSID();
                break;

            case 6:
                cmd_handler_set_WiFi_pswd(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 7:
                cmd_handler_set_WiFi_RSSI_output_flag(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 8:
                cmd_handler_set_WiFi_autoreconnect_flag(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 9:
                cmd_handler_output_local_server_IP();
                break;

            case 10:
                cmd_handler_set_local_server_port(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 11:
                cmd_handler_output_local_server_port();
                break;

            case 12:
                cmd_handler_set_IoT_flag(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 13:
                cmd_handler_set_IoT_server_IP(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 14:
                cmd_handler_output_IoT_server_IP();
                break;

            case 15:
                cmd_handler_set_IoT_server_port(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 16:
                cmd_handler_output_IoT_server_port();
                break;

            case 17:
                cmd_handler_set_IoT_req_msg(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 18:
                cmd_handler_output_IoT_req_msg();
                break;

            case 19:
                cmd_handler_set_IoT_req_period(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 20:
                cmd_handler_set_BTClassic_flag(terminal_input,
                                               setup_BTClassic,
                                               &stored_configs, 
                                               &time_to_refresh_stored_configs);
                break;

            case 21:
                cmd_handler_set_BTClassic_dev_name(terminal_input,
                                                   setup_BTClassic,
                                                   &stored_configs, 
                                                   &time_to_refresh_stored_configs);
                break;

            case 22:
                cmd_handler_output_BTClassic_dev_name();
                break;

            case 23:
                cmd_handler_all_conn_rst(setup_WiFi, setup_BTClassic, &stored_configs);
                break;

            default:
                break;  // Do nothing and hail MISRA.
        }
    }


    /*--- Current RSSI output ---*/

    uint32_t WiFi_RSSI_print_period = WIFI_RSSI_OUTPUT_PERIOD;
    if (WiFi_RSSI_print_period == 0) {
        WiFi_RSSI_print_period = DEFAULT_WIFI_RSSI_OUTPUT_PERIOD;  // Divide by zero prevention.
    }

    static bool RSSI_output_loaded = 0;
    if (millis() % WiFi_RSSI_print_period != 0) {
        RSSI_output_loaded = 1;
    }

    if (stored_configs.WiFi_RSSI_output_flag && millis() % WiFi_RSSI_print_period == 0 && RSSI_output_loaded) {
        ESP_WiFi_RSSI_print();
        RSSI_output_loaded = 0;
    }

    Serial.flush();


    /*--- Finishing communications ---*/

    // TCP clients disconnection.
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);

    // Bluetooth Classic disconnection.
    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        if (BTClassic_was_connected) {  /* Another call for connected() method
                                          * caused RTOS crash, hence additional
                                          * flag was introduced.
                                          */
            ESP32_BTClassic_disconnect(CONN_SHUTDOWN_DOWNTIME);
        }
    #endif


    /*--- Wi-Fi autoreconnect ---*/
    
    static uint64_t WiFi_autoreconnect_current_millis = millis();
    static uint64_t WiFi_autoreconnect_previous_millis = WiFi_autoreconnect_current_millis;

    static bool WiFi_was_unconnected = !ESP_WiFi_is_connected();
    static bool WiFi_autoreconnect_due_time = 0;

    if (stored_configs.WiFi_autoreconnect_flag) {
        if (WiFi_autoreconnect_due_time && !WiFi_was_unconnected) {
            WiFi_was_unconnected = !ESP_WiFi_is_connected();
            WiFi_autoreconnect_previous_millis = WiFi_autoreconnect_current_millis; 
        } else if (WiFi_autoreconnect_due_time && WiFi_was_unconnected) {
            ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
            ESP_TCP_server_stop(CONN_SHUTDOWN_DOWNTIME);
            setup_WiFi(&stored_configs);
            WiFi_was_unconnected = !ESP_WiFi_is_connected();
            WiFi_autoreconnect_previous_millis = WiFi_autoreconnect_current_millis;        
        } else {
            WiFi_autoreconnect_current_millis = millis();
        }

        WiFi_autoreconnect_due_time = (WiFi_autoreconnect_current_millis - WiFi_autoreconnect_previous_millis)
                                       > WIFI_RECONNECT_CHECK_PERIOD;
    }


}


/*--- Wireless connectivity setup functions ---*/

void setup_WiFi(stored_configs_t *stored_configs)
{
    /* Initializing certain objects (class instances) requires specifying
     * their parameters compile-time, since the latter are to be passed
     * to a constructor function. However, it's not possible to specify
     * a proper port number for a WiFiServer class object in advance because
     * the relevant port number is to be read from an inbuilt storage. Therefore
     * the object gets initialized with a dummy value and then becomes updated.
     */
    ESP_TCP_server_port_update(stored_configs->local_server_port);

    // Connect to Wi-Fi network.
    bool WiFi_connected = ESP_WiFi_set_connection(stored_configs->WiFi_SSID,
                                                  stored_configs->WiFi_pswd,
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

    // Check for RSSI output flag.
    Serial.print("RSSI output: ");
    if (stored_configs->WiFi_RSSI_output_flag != 0) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }

    // Check for automatic reconnect attempts flag.
    Serial.print("Automatic reconnect attempts: ");
    if (stored_configs->WiFi_autoreconnect_flag != 0) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
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

    Serial.println("");
}

void setup_BTClassic(stored_configs_t *stored_configs)
{
    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        // Check for Bluetooth Classic functionality flag.
        Serial.print("Bluetooth Classic: ");
        if (stored_configs->BTClassic_flag != 0) {
            ESP32_BTClassic_start(stored_configs->BTClassic_dev_name);
            Serial.println("ON");
            Serial.print("Bluetooth Classic device name: ");
            Serial.println(stored_configs->BTClassic_dev_name);
        } else {
            ESP32_BTClassic_stop(CONN_SHUTDOWN_DOWNTIME);
            Serial.println("OFF");
        }
    #endif
}


/*--- Command reception functions ---*/

void receive_cmd_HW_UART(char *buf)
{   
    uint32_t HW_UART_bytes_read = HW_UART_read_line(buf,
                                                    STR_MAX_LEN,
                                                    CONN_TIMEOUT,
                                                    HW_UART_READ_SLOWDOWN);
    if (HW_UART_bytes_read > STR_MAX_LEN) {
        buf[0] = '\0';
        cmd_handler_err_len();
    }
}

void receive_cmd_TCP_local(char *buf)
{
    if (ESP_TCP_server_get_client()) {
        uint32_t TCP_server_bytes_read = ESP_TCP_server_read_line(buf,
                                                                  STR_MAX_LEN,
                                                                  CONN_TIMEOUT);
        if (TCP_server_bytes_read > STR_MAX_LEN) {
            buf[0] = '\0';
            cmd_handler_err_len();
        }
    }
}

void receive_cmd_TCP_IoT(char *buf, stored_configs_t *stored_configs)
{
    if (stored_configs->IoT_req_period == 0) {
        stored_configs->IoT_req_period = DEFAULT_IOT_REQ_PERIOD;  // Divide by zero prevention.
    }

    if (stored_configs->IoT_flag && millis() % stored_configs->IoT_req_period == 0) {
        Serial.print ("Sending request to an IoT server, target IP: ");
        Serial.print(stored_configs->IoT_server_IP);
        Serial.print(", target port: ");
        Serial.println(stored_configs->IoT_server_port);

        if (ESP_TCP_client_get_server(stored_configs->IoT_server_IP, stored_configs->IoT_server_port)) {
            Serial.println("Remote server reached.");
            ESP_TCP_client_send_msg(stored_configs->IoT_req_msg);

            uint32_t TCP_client_bytes_read = ESP_TCP_client_read_line(buf,
                                                                      STR_MAX_LEN,
                                                                      CONN_TIMEOUT);

            if (TCP_client_bytes_read > 0 && TCP_client_bytes_read < STR_MAX_LEN) {
                utilities_nullify_first_CR_or_LF_in_string(buf);
                Serial.print("Message received from remote server: ");
                Serial.println(buf);
            }

            if (TCP_client_bytes_read > STR_MAX_LEN) {
                buf[0] = '\0';
                cmd_handler_err_len();
            }
        } else {
            Serial.println("Remote server unavailable.");
        }
    }
}

void receive_cmd_HTTP(char *buf)
{
    ESP_HTTP_handle_client_in_loop();
    ESP_HTTP_copy_value(buf, STR_MAX_LEN);
}

void receive_cmd_BTClassic(char *buf, stored_configs_t *stored_configs, bool *BTClassic_was_connected)
{
    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        *BTClassic_was_connected = 0;
        if (stored_configs->BTClassic_flag && ESP32_BTClassic_check_connection()) {
            *BTClassic_was_connected = 1;
            uint32_t BTClassic_bytes_read = 0;
            BTClassic_bytes_read = ESP32_BTClassic_read_line(buf, STR_MAX_LEN, CONN_TIMEOUT);

            if (BTClassic_bytes_read > STR_MAX_LEN) {
                buf[0] = '\0';
                cmd_handler_err_len();
            }
        }
    #endif
}
