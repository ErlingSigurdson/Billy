/****************** DESCRIPTION *****************/

/**
 * Filename: Billy_the_Relay.ino
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: a central file of an Arduino sketch written for ESP32
 * and ESP8266 SoCs. Provides the control over a simple ON/OFF load
 * with ASCII commands sent via UART (over a cable), Wi-Fi (TCP, HTTP)
 * and (optionally) Bluetooth Classic.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes: project homepage https://github.com/ErlingSigurdson/Billy_the_Relay
 *                         https://gitflic.ru/efimov-d-v/billy_the_relay
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Additional Arduino libraries are included in the local modules.

// Project configs.
#include "config_general.h"          // Also included in some local modules.

#include "config_ASCII_cmd_check.h"  /* Also included in ESP_HTTP.h because the local module
                                      * needs to "know" the commands' syntax.
                                      */

#include "config_inbuilt_storage.h"  /* Also included in inbuilt_storage.h because the local module
                                      * needs to "know" that it works with ESP32/ESP8266.
                                      */

// Local modules.
#include "utilities.h"
#include "strd_vals.h"
#include "ASCII_cmd_check.h"
#include "cmd_handlers.h"
#include "inbuilt_storage.h"
#include "HW_UART.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "ESP_HTTP.h"

#if defined ESP32 && defined BT_CLASSIC_PROVIDED
    #include "ESP32_Bluetooth.h"
#endif








/******************* FUNCTIONS ******************/

/*--- Basic functions ---*/

void setup()
{
    /*--- GPIO configuration ---*/

    pinMode(LOAD_PIN, OUTPUT);
    pinMode(WIFI_INDICATOR_LED_PIN, OUTPUT);


    /*--- Interaction with the inbuilt storage ---*/

    /* In contrast to AVR-based devices, ESP32 and ESP8266
     * inbuilt storage must be initialized before use.
     */
    inbuilt_storage_init(INBUILT_STORAGE_SIZE);

    // Read stored configs from the inbuilt storage into the struct.
    strd_vals_t strd_vals;
    strd_vals_read(&strd_vals);

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

    //#define SET_BT_DEV_NAME_AT_UPLOAD
    #ifdef SET_BT_DEV_NAME_AT_UPLOAD
        inbuilt_storage_write("NEW_BT_DEV_NAME",
                              strlen("NEW_BT_DEV_NAME"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_BT_DEV_NAME);
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
    ESP_TCP_server_port_update(strd_vals.local_server_port);

    // Connect to Wi-Fi network.
    bool WiFi_connected = ESP_WiFi_set_connection(strd_vals.SSID,
                                                  strd_vals.pswd,
                                                  CONN_TIMEOUT);

    if (WiFi_connected) {
        ESP_WiFi_indicate_connection(WIFI_INDICATOR_LED_PIN,
                                     WIFI_INDICATE_CONNECTION_CYCLES,
                                     WIFI_INDICATE_CONNECTION_PERIOD);

        Serial.print("Current local IP address is: ");
        Serial.println(ESP_WiFi_get_current_IP());

        ESP_TCP_server_start();
        Serial.print("Local TCP server started at port ");
        Serial.println(strd_vals.local_server_port);

        ESP_HTTP_server_start();
        ESP_HTTP_set_handlers();
        Serial.print("Local HTTP server started at port ");
        Serial.println(HTTP_PORT);
    }
    Serial.println("");

    // Bluetooth startup.
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        if (strd_vals.BT_flag) {
            ESP32_BT_start(strd_vals.BT_dev_name);
        }
    #endif


    /*--- Config values printout ---*/

    // Check for IoT mode flag.
    Serial.print("Requests to IoT server: ");
    if (strd_vals.IoT_flag) {
        Serial.println("ON");
        Serial.print("Target IoT server IP address: ");
        Serial.println(strd_vals.IoT_server_IP);
        Serial.print("Target IoT server port: ");
        Serial.println(strd_vals.IoT_server_port);
        Serial.print("Request text: ");
        Serial.println(strd_vals.IoT_req_msg);
        Serial.print("Request period (once per ms): ");
        Serial.println(strd_vals.IoT_req_period);
    } else {
        Serial.println("OFF");
    }

    // Check for Bluetooth functionality flag.
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        Serial.print("Bluetooth: ");
        if (strd_vals.BT_flag) {
            Serial.println("ON");
            Serial.print("Bluetooth device name: ");
            Serial.println(strd_vals.BT_dev_name);
        } else {
            Serial.println("OFF");
        }
    #endif

    // Check for RSSI printout flag.
    Serial.print("Print RSSI: ");
    if (strd_vals.RSSI_print_flag) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }
    
    Serial.println("");
    Serial.flush();
}

void loop()
{
    // Read config values from the inbuilt storage into the struct.
    static strd_vals_t strd_vals;
    if (time_to_refresh_strd_vals) {    // Check if update is necessary.
        strd_vals_read(&strd_vals);
        time_to_refresh_strd_vals = 0;
    }

    // Array of valid commands.
    static const char *cmd_list[] = {
        CMD_0,
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
        CMD_19
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

    if (strd_vals.RSSI_print_flag && millis() % WiFi_RSSI_print_period == 0 && RSSI_print_loaded) {
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
        handle_cmd_err_len();
    }

    // Local TCP server reads data from a remote client.
    if (ESP_TCP_server_get_client()) {
        uint32_t TCP_server_bytes_read = ESP_TCP_server_read_line(terminal_input,
                                                                  STR_MAX_LEN,
                                                                  CONN_TIMEOUT);
        if (TCP_server_bytes_read > STR_MAX_LEN) {
            terminal_input[0] = '\0';
            handle_cmd_err_len();
        }
    }

    // Handle HTTP request and write value from a request body into a buffer.
    ESP_HTTP_handle_client_in_loop();
    ESP_HTTP_copy_value(terminal_input, STR_MAX_LEN);

    /* Local TCP client sends a request to a remote server
     * and reads the response.
     */
    if (strd_vals.IoT_req_period == 0) {
        strd_vals.IoT_req_period = DEFAULT_IOT_REQ_PERIOD;  // Divide by zero prevention.
    }

    if (strd_vals.IoT_flag && millis() % strd_vals.IoT_req_period == 0) {
        Serial.print ("Sending request to an IoT server, target IP: ");
        Serial.print(strd_vals.IoT_server_IP);
        Serial.print(", target port: ");
        Serial.println(strd_vals.IoT_server_port);

        if (ESP_TCP_client_get_server(strd_vals.IoT_server_IP, strd_vals.IoT_server_port)) {
            Serial.println("Remote server reached.");
            ESP_TCP_client_send_msg(strd_vals.IoT_req_msg);

            uint32_t TCP_client_bytes_read = ESP_TCP_client_read_line(terminal_input,
                                                                      STR_MAX_LEN,
                                                                      CONN_TIMEOUT);
            if (TCP_client_bytes_read > STR_MAX_LEN) {
                terminal_input[0] = '\0';
                handle_cmd_err_len();
            }
        } else {
            Serial.println("Remote server unavailable.");
        }
    }

    // Read data from a Bluetooth master device.
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        bool BT_was_connected = 0;
        if (strd_vals.BT_flag && ESP32_BT_check_connection()) {
            BT_was_connected = 1;
            uint32_t BT_bytes_read = 0;
            BT_bytes_read = ESP32_BT_read_line(terminal_input, STR_MAX_LEN, CONN_TIMEOUT);
            
            if (BT_bytes_read > STR_MAX_LEN) {
                terminal_input[0] = '\0';
                handle_cmd_err_len();
            }
        }
    #endif


    /*--- Handling commands ---*/
    
    // Essentially it's a central hub of the whole sketch.

    // Check for non-empty buffer string and correct command prefix.
    if (terminal_input[0] != '\0' && ASCII_cmd_check_prefix(terminal_input, CMD_PREFIX)) {
        utilities_nullify_first_CR_or_LF_in_string(terminal_input);

        // Check for valid commands.
        int32_t func_to_call = ASCII_cmd_check_cmd(terminal_input, cmd_list, CMD_LIST_LEN);
        switch (func_to_call) {
            case -1:
                handle_cmd_err_cmd();
                break;

            case 0:
                handle_cmd_set_load(terminal_input);
                break;

            case 1:
                handle_cmd_set_local_SSID(terminal_input);
                break;

            case 2:
                handle_cmd_print_local_SSID();
                break;

            case 3:
                handle_cmd_set_local_pswd(terminal_input);
                break;

            case 4:
                handle_cmd_set_local_port(terminal_input);
                break;

            case 5:
                handle_cmd_print_local_port();
                break;

            case 6:
                handle_cmd_print_local_IP();
                break;

            case 7:
                handle_cmd_rst_local_conn(setup);
                break;

            case 8:
                handle_cmd_set_IoT_flag(terminal_input);
                break;

            case 9:
                handle_cmd_set_IoT_server_IP(terminal_input);
                break;

            case 10:
                handle_cmd_print_IoT_server_IP();
                break;

            case 11:
                handle_cmd_set_IoT_server_port(terminal_input);
                break;

            case 12:
                handle_cmd_print_IoT_server_port();
                break;

            case 13:
                handle_cmd_set_IoT_req_msg(terminal_input);
                break;

            case 14:
                handle_cmd_print_IoT_req_msg();
                break;

            case 15:
                handle_cmd_set_IoT_req_period(terminal_input);
                break;

            case 16:
                handle_cmd_set_BT_flag(terminal_input, setup);
                break;

            case 17:
                handle_cmd_set_BT_dev_name(terminal_input);
                break;

            case 18:
                handle_cmd_print_BT_dev_name();
                break;

            case 19:
                handle_cmd_set_RSSI_print_flag(terminal_input);
                break;

            default:
                break;  // Do nothing and hail MISRA.
        }
    } else if (terminal_input[0] != '\0') {
        handle_cmd_err_prefix();
        Serial.flush();
        ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
    }
    
    
    /*--- Closing communications ---*/

    // TCP clients' disconnection.
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);

    // Bluetooth disconnection.
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        if (BT_was_connected) {  /* Another call for connected() method
                                  * caused RTOS crash, hence additional
                                  * flag was introduced.
                                  */
            ESP32_BT_disconnect(CONN_SHUTDOWN_DOWNTIME);
        }
    #endif
}