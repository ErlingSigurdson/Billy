/****************** DESCRIPTION *****************/

/**
 * Filename: Billy.ino
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: the main file of an Arduino sketch written for ESP32 and ESP8266
 * systems-on-chip (SoCs). See README files for more information.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes: project homepages https://github.com/ErlingSigurdson/Billy
 *                          https://gitflic.ru/efimov-d-v/billy
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Arduino.h, the main Arduino library, is already included by default.

// Project configs.
#include "config_general.h"
#include "config_cmd.h"
#include "config_inbuilt_storage.h"

// Additional Arduino libraries are included in the local modules.

// Local modules.
#include "utilities.h"
#include "cmd.h"
#include "inbuilt_storage.h"
#include "stored_configs.h"
#include "HW_UART.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "ESP_HTTP.h"

#if defined ESP32 && defined BTCLASSIC_PROVIDED
    #include "ESP32_Bluetooth_Classic.h"
#endif


/******************* FUNCTIONS ******************/

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

    /* If necessary, you can write any config value into the inbuilt storage
     * at the same time with uploading the sketch. To achieve this, uncomment
     * the respective #define directive and specify the desired value right
     * in the code, pre-compile-time. Then upload the sketch. After that
     * comment out the same directives you've uncommented before and upload
     * the sketch once again. Without that last step a config value will always
     * be reverted to the hardcoded value upon every device startup.
     */
    //#define SET_LOCAL_SSID_AT_UPLOAD
    #ifdef SET_LOCAL_SSID_AT_UPLOAD
        char new_local_ssid[STR_MAX_LEN] = "NEW_LOCAL_SSID";
        inbuilt_storage_write(new_local_ssid,
                              strlen(new_local_ssid),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_LOCAL_SSID);
    #endif

    //#define SET_LOCAL_PSWD_AT_UPLOAD
    #ifdef SET_LOCAL_PSWD_AT_UPLOAD
        char new_local_pswd[STR_MAX_LEN] = "NEW_LOCAL_PSWD";
        inbuilt_storage_write(new_local_pswd,
                              strlen(new_local_pswd),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_LOCAL_PSWD);
    #endif

    //#define SET_LOCAL_SERVER_PORT_AT_UPLOAD
    #ifdef SET_LOCAL_SERVER_PORT_AT_UPLOAD
        char new_local_server_port[STR_MAX_LEN] = "451";
        inbuilt_storage_write(new_local_server_port,
                              strlen(new_local_server_port),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT);
    #endif

    //#define SET_IOT_FLAG_AT_UPLOAD
    #ifdef SET_IOT_FLAG_AT_UPLOAD
        char IoT_flag[STR_MAX_LEN] = "OFF";
        inbuilt_storage_write(IoT_flag,
                              strlen(IoT_flag),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_FLAG);
    #endif

    //#define SET_IOT_SERVER_IP_AT_UPLOAD
    #ifdef SET_IOT_SERVER_IP_AT_UPLOAD
        char new_IoT_server_IP[STR_MAX_LEN] = "111.111.111.111";
        inbuilt_storage_write(new_IoT_server_IP,
                              strlen(new_IoT_server_IP),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_SERVER_IP);
    #endif

    //#define SET_IOT_SERVER_PORT_AT_UPLOAD
    #ifdef SET_IOT_SERVER_PORT_AT_UPLOAD
        char new_IoT_server_port[STR_MAX_LEN] = "451";
        inbuilt_storage_write(new_IoT_server_port,
                              strlen(new_IoT_server_port),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_SERVER_PORT);
    #endif

    //#define SET_IOT_REQ_MSG_AT_UPLOAD
    #ifdef SET_IOT_REQ_MSG_AT_UPLOAD
        char new_IoT_req_msg[STR_MAX_LEN] = "SEND_ME_INFO";
        inbuilt_storage_write(new_IoT_req_msg,
                              strlen(new_IoT_req_msg),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_REQ_MSG);
    #endif

    //#define SET_IOT_REQ_PERIOD_AT_UPLOAD
    #ifdef SET_IOT_REQ_PERIOD_AT_UPLOAD
        char new_IoT_req_period[STR_MAX_LEN] = "500";
        inbuilt_storage_write(new_IoT_req_period,
                              strlen(new_IoT_req_period),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD);
    #endif

    //#define SET_BTCLASSIC_FLAG_AT_UPLOAD
    #ifdef SET_BTCLASSIC_FLAG_AT_UPLOAD
        char BT_Classic_flag[STR_MAX_LEN] = "OFF";
        inbuilt_storage_write(BT_Classic_flag,
                              strlen(BT_Classic_flag),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_BTCLASSIC_FLAG);
    #endif

    //#define SET_BTCLASSIC_DEV_NAME_AT_UPLOAD
    #ifdef SET_BTCLASSIC_DEV_NAME_AT_UPLOAD
        char new_BT_Classic_dev_name[STR_MAX_LEN] = "NEW_BTCLASSIC_DEV_NAME";
        inbuilt_storage_write(new_BT_Classic_dev_name,
                              strlen(new_BT_Classic_dev_name),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_BTCLASSIC_DEV_NAME);
    #endif

    //#define SET_RSSI_OUTPUT_FLAG_AT_UPLOAD
    #ifdef SET_RSSI_OUTPUT_FLAG_AT_UPLOAD
        char RSSI_output_flag[STR_MAX_LEN] = "OFF";
        inbuilt_storage_write(RSSI_output_flag,
                              strlen(RSSI_output_flag),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_RSSI_OUTPUT_FLAG);
    #endif

    // Read stored configs from the inbuilt storage into the struct.
    stored_configs_t stored_configs;
    stored_configs_read(&stored_configs);


    /*--- Starting wireless communications ---*/

    /* Initializing certain objects (class instances) requires specifying
     * their parameters compile-time, since the latter are to be passed
     * to a constructor function. However, it's not possible to specify
     * a proper port number for a WiFiServer class object in advance because
     * the relevant port number is to be read from an inbuilt storage. Therefore
     * the object gets initialized with a dummy value and then becomes updated.
     */
    ESP_TCP_server_port_update(stored_configs.local_server_port);

    setup_WiFi(&stored_configs);
    setup_BTClassic(&stored_configs);

    // Bluetooth Classic startup.
    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        if (stored_configs.BT_Classic_flag) {
            ESP32_BT_Classic_start(stored_configs.BT_Classic_dev_name);
        }
    #endif

    Serial.println("");
    Serial.flush();
}

void loop()
{
    /*--- Interaction with the inbuilt storage ---*/

    static bool time_to_refresh_stored_configs = 1;

    // Read config values from the inbuilt storage into the struct.
    static stored_configs_t stored_configs;
    if (time_to_refresh_stored_configs) {    // Check if update is necessary.
        stored_configs_read(&stored_configs);
        time_to_refresh_stored_configs = 0;
    }


    /*--- Array of valid commands ---*/

    static const char *cmd_list[] = {
        0,      // A placeholder to bump first command's index up to 1.
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


    /*--- Receiving commands ---*/

    // Command buffer.
    char terminal_input[STR_MAX_LEN + 1] = {0};

    // Read data received by the hardware UART.
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

    /* Local TCP client sends the request to a remote server
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

            if (TCP_client_bytes_read > 0 && TCP_client_bytes_read < STR_MAX_LEN) {
                utilities_nullify_first_CR_or_LF_in_string(terminal_input);
                Serial.print("Message received from remote server: ");
                Serial.println(terminal_input);
            }

            if (TCP_client_bytes_read > STR_MAX_LEN) {
                terminal_input[0] = '\0';
                cmd_handler_err_len();
            }
        } else {
            Serial.println("Remote server unavailable.");
        }
    }

    // Read data from a Bluetooth Classic master device.
    #if defined ESP32 && defined BTCLASSIC_PROVIDED
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
                cmd_handler_set_local_SSID(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 5:
                cmd_handler_output_local_SSID();
                break;

            case 6:
                cmd_handler_set_local_pswd(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 7:
                cmd_handler_set_local_server_port(terminal_input, &time_to_refresh_stored_configs);
                break;

            case 8:
                cmd_handler_output_local_server_port();
                break;

            case 9:
                cmd_handler_output_local_server_IP();
                break;

            case 10:
                cmd_handler_local_conn_rst(setup);
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
                cmd_handler_set_RSSI_output_flag(terminal_input, &time_to_refresh_stored_configs);
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

    if (stored_configs.RSSI_output_flag && millis() % WiFi_RSSI_print_period == 0 && RSSI_output_loaded) {
        ESP_WiFi_RSSI_print();
        RSSI_output_loaded = 0;
    }


    /*--- Finishing communications ---*/

    Serial.flush();

    // TCP clients' disconnection.
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);

    // Bluetooth Classic disconnection.
    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        if (BT_Classic_was_connected) {  /* Another call for connected() method
                                          * caused RTOS crash, hence additional
                                          * flag was introduced.
                                          */
            ESP32_BT_Classic_disconnect(CONN_SHUTDOWN_DOWNTIME);
        }
    #endif


    /*--- Wi-Fi reconnect ---*/

    #define WIFI_RECONNECT_CHECK_PERIOD 30000
    
    static uint64_t WiFi_reconnect_current_millis = millis();
    static uint64_t WiFi_reconnect_previous_millis = WiFi_reconnect_current_millis;

    static bool WiFi_was_unconnected = !ESP_WiFi_is_connected();
    static bool WiFi_reconnect_due_time =
           WiFi_reconnect_current_millis - WiFi_reconnect_previous_millis()  > WIFI_RECONNECT_CHECK_PERIOD;
    
    if (WiFi_reconnect_due_time && !WiFi_was_unconnected) {
        WiFi_was_unconnected = !ESP_WiFi_is_connected();
        WiFi_reconnect_previous_millis = WiFi_reconnect_current_millis; 
    } else if (WiFi_reconnect_due_time && WiFi_was_unconnected)
        ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
        ESP_TCP_server_stop(CONN_SHUTDOWN_DOWNTIME);

        #if defined ESP32 && defined BTCLASSIC_PROVIDED
            ESP32_BT_Classic_stop(CONN_SHUTDOWN_DOWNTIME);
        #endif
        
        setup_WiFi();
        WiFi_reconnect_previous_millis = WiFi_reconnect_current_millis;        
    } else {
        WiFi_reconnect_current_millis = millis();
    }
}
