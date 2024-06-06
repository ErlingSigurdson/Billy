/****************** DESCRIPTION *****************/

/**
 * Filename: Billy.ino
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: a central file of an Arduino sketch written for ESP32
 * and ESP8266 SoCs. Provides the control over a simple ON/OFF load
 * with ASCII commands sent via UART (over a cable), Wi-Fi (TCP, HTTP)
 * and (optionally) Bluetooth Classic.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes: project homepage https://github.com/ErlingSigurdson/Billy
 *                         https://gitflic.ru/efimov-d-v/billy
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Additional Arduino libraries are included in the local modules.

// Project configs.
#include "config_general.h"          // Also included in some local modules.

#include "config_cmd.h"              /* Also included in ESP_HTTP.h because the local module
                                      * needs to "know" the commands' syntax.
                                      */

#include "config_inbuilt_storage.h"  /* Also included in inbuilt_storage.h because the local module
                                      * needs to "know" that it works with ESP32/ESP8266.
                                      */

// Local modules.
#include "utilities.h"
#include "stored_configs.h"
#include "cmd.h"
#include "inbuilt_storage.h"
#include "HW_UART.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "ESP_HTTP.h"

#if defined ESP32 && defined BT_CLASSIC_PROVIDED
    #include "ESP32_Bluetooth_Classic.h"
#endif


/******************* FUNCTIONS ******************/

/*--- Basic functions ---*/

void setup()
{
    /*--- GPIO configuration ---*/

    pinMode(DIGITAL_LOAD_PIN, OUTPUT);
    pinMode(ANALOG_LOAD_PIN, OUTPUT);
    pinMode(WIFI_INDICATOR_LED_PIN, OUTPUT);


    /*--- Setting initial digital load state ---*/

    digitalWrite(DIGITAL_LOAD_PIN, DIGITAL_LOAD_OFF);


    /*--- Interaction with the inbuilt storage ---*/

    /* In contrast to AVR-based devices, ESP32 and ESP8266
     * inbuilt storage must be initialized before use.
     */
    inbuilt_storage_init(INBUILT_STORAGE_SIZE);

    // Read stored configs from the inbuilt storage into the struct.
    stored_configs_t stored_configs;
    stored_configs_read(&stored_configs);

    /* If necessary, you can write any config value into the inbuilt storage
     * at the same time with uploading the sketch. To achieve this, uncomment
     * respective #define directive and specify the desired value right
     * in the code, pre-compile-time. Then upload the sketch. After that
     * comment out the same directives you've uncommented before and upload
     * the sketch once again. Without that last step a config value will always
     * be reverted to the hardcoded value upon every device startup. 
     */
    
    //#define SET_SSID_AT_UPLOAD
    #ifdef SET_SSID_AT_UPLOAD
        inbuilt_storage_write("NEWSSID",
                              strlen("NEWSSID"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_SSID);
    #endif

    //#define SET_PSWD_AT_UPLOAD
    #ifdef SET_PSWD_AT_UPLOAD
        inbuilt_storage_write("123456",
                              strlen("123456"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_PSWD);
    #endif

    //#define SET_LOCAL_SERVER_PORT_AT_UPLOAD
    #ifdef SET_LOCAL_SERVER_PORT_AT_UPLOAD
        inbuilt_storage_write("451",
                              strlen("451"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT);
    #endif

    //#define SET_IOT_FLAG_AT_UPLOAD
    #ifdef SET_IOT_FLAG_AT_UPLOAD
        inbuilt_storage_write("OFF",
                              strlen("OFF"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_FLAG);
    #endif

    //#define SET_IOT_SERVER_IP_AT_UPLOAD
    #ifdef SET_IOT_SERVER_IP_AT_UPLOAD
        inbuilt_storage_write("111.111.111.111",
                              strlen("111.111.111.111"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_SERVER_IP);
    #endif

    //#define SET_IOT_SERVER_PORT_AT_UPLOAD
    #ifdef SET_IOT_SERVER_PORT_AT_UPLOAD
        inbuilt_storage_write("451",
                              strlen("451"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_SERVER_PORT);
    #endif

    //#define SET_IOT_REQ_MSG_AT_UPLOAD
    #ifdef SET_IOT_REQ_MSG_AT_UPLOAD
        inbuilt_storage_write("SERVE_CURRENT_CMD",
                              strlen("SERVE_CURRENT_CMD"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_REQ_MSG);
    #endif

    //#define SET_IOT_REQ_PERIOD_AT_UPLOAD
    #ifdef SET_IOT_REQ_PERIOD_AT_UPLOAD
        inbuilt_storage_write("500",
                              strlen("500"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD);
    #endif

    //#define SET_BT_CLASSIC_FLAG_AT_UPLOAD
    #ifdef SET_BT_CLASSIC_FLAG_AT_UPLOAD
        inbuilt_storage_write("OFF",
                              strlen("OFF"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_BT_CLASSIC_FLAG);
    #endif

    //#define SET_BT_CLASSIC_DEV_NAME_AT_UPLOAD
    #ifdef SET_BT_CLASSIC_DEV_NAME_AT_UPLOAD
        inbuilt_storage_write("NEW_BT_CLASSIC_DEV_NAME",
                              strlen("NEW_BT_CLASSIC_DEV_NAME"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_BT_CLASSIC_DEV_NAME);
    #endif

    //#define SET_RSSI_PRINT_FLAG_AT_UPLOAD
    #ifdef SET_RSSI_PRINT_FLAG_AT_UPLOAD
        inbuilt_storage_write("OFF",
                              strlen("OFF"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG);
    #endif


    /*--- Starting communications ---*/

    // Hardware UART startup.
    Serial.begin(HW_UART_BAUD_RATE);
    delay(HW_UART_STARTUP_PAUSE);    // Tiny pause to allow interface startup.

    Serial.println("");
    Serial.println("*** HELLO, HUMAN! ***");

    /* Initializing certain objects (class instances) requires specifying
     * their parameters compile-time, since the latter are to be passed
     * to a constructor function. However, it's not possible to specify
     * a proper port number for a WiFiServer class object in advance because
     * the relevant port number is to be read from an inbuilt storage. Therefore
     * the object gets initialized with a dummy value and then becomes updated.
     */
    ESP_TCP_server_port_update(stored_configs.local_server_port);

    // Connect to Wi-Fi network.
    bool WiFi_connected = ESP_WiFi_set_connection(stored_configs.SSID,
                                                  stored_configs.pswd,
                                                  CONN_TIMEOUT);

    if (WiFi_connected) {
        ESP_WiFi_indicate_connection(WIFI_INDICATOR_LED_PIN,
                                     WIFI_INDICATE_CONNECTION_CYCLES,
                                     WIFI_INDICATE_CONNECTION_PERIOD);

        Serial.print("Current local IP address is: ");
        Serial.println(ESP_WiFi_get_current_IP());

        ESP_TCP_server_start();
        Serial.print("Local TCP server started at port ");
        Serial.println(stored_configs.local_server_port);

        ESP_HTTP_server_start();
        ESP_HTTP_set_handlers();
        Serial.print("Local HTTP server started at port ");
        Serial.println(HTTP_PORT);
    }
    Serial.println("");

    // Bluetooth startup.
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        if (stored_configs.BT_Classic_flag) {
            ESP32_BT_Classic_start(stored_configs.BT_Classic_dev_name);
        }
    #endif


    /*--- Config values printout ---*/

    // Check for IoT mode flag.
    Serial.print("Requests to IoT server: ");
    if (stored_configs.IoT_flag) {
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

    // Check for Bluetooth functionality flag.
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        Serial.print("Bluetooth: ");
        if (stored_configs.BT_Classic_flag) {
            Serial.println("ON");
            Serial.print("Bluetooth device name: ");
            Serial.println(stored_configs.BT_Classic_dev_name);
        } else {
            Serial.println("OFF");
        }
    #endif

    // Check for RSSI printout flag.
    Serial.print("Print RSSI: ");
    if (stored_configs.RSSI_print_flag) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }
    
    Serial.println("");
    Serial.flush();
}

void loop()
{
    static bool time_to_refresh_stored_configs = 1;

    // Read config values from the inbuilt storage into the struct.
    static stored_configs_t stored_configs;
    if (time_to_refresh_stored_configs) {    // Check if update is necessary.
        stored_configs_read(&stored_configs);
        time_to_refresh_stored_configs = 0;
    }

    // Array of valid commands.
    static const char *cmd_list[] = {
        "placeholder",                       // A placeholder to bump first command's index up to 1.
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
        CMD_22
    };


    /*--- Current RSSI printout ---*/
    
    uint32_t WiFi_RSSI_print_period = WIFI_RSSI_PRINT_PERIOD;
    if (WiFi_RSSI_print_period == 0) {
        WiFi_RSSI_print_period = DEFAULT_WIFI_RSSI_PRINT_PERIOD;  // Divide by zero prevention.
    }

    static bool RSSI_print_loaded = 0;
    if (millis() % WiFi_RSSI_print_period != 0) {
        RSSI_print_loaded = 1;
    }

    if (stored_configs.RSSI_print_flag && millis() % WiFi_RSSI_print_period == 0 && RSSI_print_loaded) {
        ESP_WiFi_RSSI_print();
        RSSI_print_loaded = 0;
    }


    /*--- Receiving commands ---*/

    // Command buffer.
    char terminal_input[STR_MAX_LEN + 1] = {0};

    // Read data received by hardware UART.
    uint32_t HW_UART_bytes_read = HW_UART_read_line(terminal_input,
                                                    STR_MAX_LEN,
                                                    CONN_TIMEOUT,
                                                    HW_UART_READ_SLOWDOWN);
    if (HW_UART_bytes_read > STR_MAX_LEN) {
        terminal_input[0] = '\0';
        cmd_handler_err_len();
    }

    // Local TCP server reads data from a remote client.
    if (ESP_TCP_server_get_client()) {
        uint32_t TCP_server_bytes_read = ESP_TCP_server_read_line(terminal_input,
                                                                  STR_MAX_LEN,
                                                                  CONN_TIMEOUT);
        if (TCP_server_bytes_read > STR_MAX_LEN) {
            terminal_input[0] = '\0';
            cmd_handler_err_len();
        }
    }

    // Handle HTTP request and write value from a request body into a buffer.
    ESP_HTTP_handle_client_in_loop();
    ESP_HTTP_copy_value(terminal_input, STR_MAX_LEN);

    /* Local TCP client sends a request to a remote server
     * and reads the response.
     */
    if (stored_configs.IoT_req_period == 0) {
        stored_configs.IoT_req_period = DEFAULT_IOT_REQ_PERIOD;  // Divide by zero prevention.
    }

    if (stored_configs.IoT_flag && millis() % stored_configs.IoT_req_period == 0) {
        Serial.print ("Sending request to an IoT server, target IP: ");
        Serial.print(stored_configs.IoT_server_IP);
        Serial.print(", target port: ");
        Serial.println(stored_configs.IoT_server_port);

        if (ESP_TCP_client_get_server(stored_configs.IoT_server_IP, stored_configs.IoT_server_port)) {
            Serial.println("Remote server reached.");
            ESP_TCP_client_send_msg(stored_configs.IoT_req_msg);

            uint32_t TCP_client_bytes_read = ESP_TCP_client_read_line(terminal_input,
                                                                      STR_MAX_LEN,
                                                                      CONN_TIMEOUT);
            if (TCP_client_bytes_read > STR_MAX_LEN) {
                terminal_input[0] = '\0';
                cmd_handler_err_len();
            }
        } else {
            Serial.println("Remote server unavailable.");
        }
    }

    // Read data from a Bluetooth master device.
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        bool BT_Classic_was_connected = 0;
        if (stored_configs.BT_Classic_flag && ESP32_BT_Classic_check_connection()) {
            BT_Classic_was_connected = 1;
            uint32_t BT_Classic_bytes_read = 0;
            BT_Classic_bytes_read = ESP32_BT_Classic_read_line(terminal_input, STR_MAX_LEN, CONN_TIMEOUT);
            
            if (BT_Classic_bytes_read > STR_MAX_LEN) {
                terminal_input[0] = '\0';
                cmd_handler_err_len();
            }
        }
    #endif


    /*--- Handling commands ---*/
    
    // Essentially it's a central hub of the whole sketch.

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
                cmd_handler_set_digital_load(terminal_input);
                break;
                
            case 2:
                cmd_handler_set_analog_load(terminal_input);
                break;
                
            case 3:
                cmd_handler_output_digital_load();
                break;

            case 4:
                cmd_handler_set_local_SSID(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 5:
                cmd_handler_output_local_SSID();
                break;

            case 6:
                cmd_handler_set_local_pswd(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 7:
                cmd_handler_set_local_port(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 8:
                cmd_handler_output_local_port();
                break;

            case 9:
                cmd_handler_output_local_IP();
                break;

            case 10:
                cmd_handler_rst_local_conn(setup);
                break;

            case 11:
                cmd_handler_set_IoT_flag(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 12:
                cmd_handler_set_IoT_server_IP(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 13:
                cmd_handler_output_IoT_server_IP();
                break;

            case 14:
                cmd_handler_set_IoT_server_port(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 15:
                cmd_handler_output_IoT_server_port();
                break;

            case 16:
                cmd_handler_set_IoT_req_msg(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 17:
                cmd_handler_output_IoT_req_msg();
                break;

            case 18:
                cmd_handler_set_IoT_req_period(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 19:
                cmd_handler_set_BT_Classic_flag(terminal_input, setup, &time_to_refresh_stored_configs);
                break;

            case 20:
                cmd_handler_set_BT_Classic_dev_name(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 21:
                cmd_handler_output_BT_Classic_dev_name();
                break;

            case 22:
                cmd_handler_set_RSSI_print_flag(terminal_input, &time_to_refresh_stored_configs);
                break;

            default:
                break;  // Do nothing and hail MISRA.
        }
    }
    
    
    /*--- Finishing communications ---*/

    Serial.flush();

    // TCP clients' disconnection.
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);

    // Bluetooth disconnection.
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        if (BT_Classic_was_connected) {  /* Another call for connected() method
                                          * caused RTOS crash, hence additional
                                          * flag was introduced.
                                          */
            ESP32_BT_Classic_disconnect(CONN_SHUTDOWN_DOWNTIME);
        }
    #endif
}
