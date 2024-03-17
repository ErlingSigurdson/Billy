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
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

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
#include "ASCII_cmd_check.h"
#include "inbuilt_storage.h"
#include "HW_UART.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "ESP_HTTP.h"

#if defined ESP32 && defined BT_CLASSIC_PROVIDED
    #include "ESP32_Bluetooth.h"
#endif

// Additional Arduino libraries are included in the local modules.


/****************** DATA TYPES ******************/

// Struct for holding values read from the inbuilt storage.
typedef struct strd_vals_t {
    char SSID[STR_MAX_LEN + 1];
    char pswd[STR_MAX_LEN + 1];
    uint32_t local_server_port;
    bool IoT_flag;
    char IoT_server_IP[STR_MAX_LEN + 1];
    uint32_t IoT_server_port;
    char IoT_req_msg[STR_MAX_LEN + 1];
    uint32_t IoT_req_period;
    bool BT_flag;
    char BT_dev_name[STR_MAX_LEN + 1];
    bool RSSI_print_flag;
} strd_vals_t;


/*************** GLOBAL VARIABLES ***************/

bool time_to_refresh_strd_vals = 1;


/************** FUNCTION PROTOTYPES *************/

/*--- Handler functions ---*/

/* Functions called in response to received commands
 * and command processing errors. 
 */

// Incorrect/absent command prefix.
void handle_cmd_err_prefix();

// No command after the prefix.
void handle_cmd_err_cmd();

// No value after the command which requires it.
void handle_cmd_err_val();

// Incoming byte string is too long.
void handle_cmd_err_len();

/* Command #0:
 * turn load ON of OFF. Main workhorse.
 */
void handle_cmd_set_load(char *cmd);

/* Command #1:
 * change an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void handle_cmd_set_local_SSID(char *cmd);

/* Command #2:
 * print an SSID of a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void handle_cmd_print_local_SSID();

/* Command #3:
 * change a password for a local Wi-Fi access point
 * stored in the inbuilt storage.
 */
void handle_cmd_set_local_pswd(char *cmd);

/* Command #4:
 * change a local TCP server port number
 * stored in the inbuilt storage.
 */
void handle_cmd_set_local_port(char *cmd);

/* Command #5:
 * print a local TCP server port number
 * stored in the inbuilt storage.
 */
void handle_cmd_print_local_port();

/* Command #6:
 * print local TCP server current local IP.
 */
void handle_cmd_print_local_IP();

/* Command #7:
 * reset local connections.
 */
void handle_cmd_rst_local_conn();

/* Command #8:
 * set IoT mode (attempts to connect to a remote server) ON or OFF.
 */
void handle_cmd_set_IoT_flag(char *cmd);

/* Command #9:
 * change an IP address of a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_set_IoT_server_IP(char *cmd);

/* Command #10:
 * print an IP address of a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_print_IoT_server_IP();

/* Command #11:
 * change port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_set_IoT_server_port(char *cmd);

/* Command #12:
 * print port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_print_IoT_server_port();

/* Command #13:
 * change request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_set_IoT_req_msg(char *cmd);

/* Command #14:
 * print request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_print_IoT_req_msg();

/* Command #15:
 * change the interval (in ms) for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void handle_cmd_set_IoT_req_period(char *cmd);

/* Command #16:
 * set Bluetooth Classic functionality ON or OFF.
 */
void handle_cmd_set_BT_flag(char *cmd);

/* Command #17:
 * change ESP's name as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void handle_cmd_set_BT_dev_name(char *cmd);

/* Command #18:
 * print ESP's name as a Bluetooth slave device
 * stored in the inbuilt storage.
 */
void handle_cmd_print_BT_dev_name();

/* Command #19:
 * set periodical printount of a current RSSI value ON or OFF.
 */
void handle_cmd_set_RSSI_print_flag(char *cmd);


/*--- Handler function subroutines ---*/

// Subroutine for "set" handlers.
void handle_cmd_helper_set(char *cmd, const char *topic, uint32_t addr, bool echo);

// Subroutine for "print" handlers.
void handle_cmd_helper_print(const char *topic, uint32_t addr);

// Subroutine for sending messages.
void handle_cmd_helper_send(const char *msg);


/*--- Misc functions ---*/

// Read values from the inbuilt storage.
void strd_vals_read(strd_vals_t *_strd_vals);


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
        inbuilt_storage_write("UPD_REQ",
                              strlen("UPD_REQ"),
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

    //#define SET_BT_FLAG_AT_UPLOAD
    #ifdef SET_BT_FLAG_AT_UPLOAD
        inbuilt_storage_write("OFF",
                              strlen("OFF"),
                              INBUILT_STORAGE_STR_MAX_LEN,
                              INBUILT_STORAGE_ADDR_BT_FLAG);
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

    /* Initializing of certain objects (class instances) requires specifying
     * their parameters compile-time, since the latter are to be passed
     * to a constructor function. However, to avoid mentioning such objects
     * in an .ino file, they are initialized with dummy values and then get
     * updated with the values read from the inbuilt storage.
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
                handle_cmd_rst_local_conn();
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
                handle_cmd_set_BT_flag(terminal_input);
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


/*--- Handler functions ---*/

void handle_cmd_err_prefix()
{
    handle_cmd_helper_send("Invalid prefix for a command.");
}

void handle_cmd_err_cmd()
{
    handle_cmd_helper_send("No valid command entered.");
}

void handle_cmd_err_val()
{
    handle_cmd_helper_send("No valid value submitted.");
}

void handle_cmd_err_len()
{
    handle_cmd_helper_send("Command buffer overflow.");
}

// Command #0
void handle_cmd_set_load(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "TOGGLE")) {
        if (digitalRead(LOAD_PIN) == LOAD_ON) {
            strcpy(cmd_val, "OFF");
        } else {
            strcpy(cmd_val, "ON");
        }
    }

    if (!strcmp(cmd_val, "ON")) {
        digitalWrite(LOAD_PIN, LOAD_ON);
        handle_cmd_helper_send("LOAD ON");

        return;
    }

    if (!strcmp(cmd_val, "OFF")) {
        digitalWrite(LOAD_PIN, LOAD_OFF);
        handle_cmd_helper_send("LOAD OFF");

        return;
    }

    handle_cmd_err_val();
}

/* To print or not to print new value of an updated config.
 * OFF for password, ON for other cases.
 */
#define ECHO_ON 1
#define ECHO_OFF 0

// Command #1
void handle_cmd_set_local_SSID(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "SSID changed successfully! New SSID is: ",
                          INBUILT_STORAGE_ADDR_SSID,
                          ECHO_ON);
}

// Command #2
void handle_cmd_print_local_SSID()
{
    handle_cmd_helper_print("Current SSID is: ",
                            INBUILT_STORAGE_ADDR_SSID);
}

// Command #3
void handle_cmd_set_local_pswd(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "Password changed successfully!",
                          INBUILT_STORAGE_ADDR_PSWD,
                          ECHO_OFF);
}

// Command #4
void handle_cmd_set_local_port(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "Local server port changed successfully! New port is: ",
                          INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                          ECHO_ON);

    handle_cmd_helper_send("Please reboot your device or reset local connection to put changes into effect.");
}

// Command #5
void handle_cmd_print_local_port()
{
    handle_cmd_helper_print("Current local server port is: ",
                            INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT);
}

// Command #6
void handle_cmd_print_local_IP()
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    String current_IP = ESP_WiFi_get_current_IP();
    strcpy(msg, "Current IP address is: ");
    strcat(msg, current_IP.c_str());
    handle_cmd_helper_send(msg);
}

// Command #7
void handle_cmd_rst_local_conn()
{
    handle_cmd_helper_send("Resetting local connections...");
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
    ESP_TCP_server_stop(CONN_SHUTDOWN_DOWNTIME);

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        ESP32_BT_stop(CONN_SHUTDOWN_DOWNTIME);
    #endif

    setup();
}

// Command #8
void handle_cmd_set_IoT_flag(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
        handle_cmd_helper_set(cmd,
                              "Requests to an IoT server: ",
                              INBUILT_STORAGE_ADDR_IOT_FLAG,
                              ECHO_ON);
    } else {
        handle_cmd_err_val();
    }
}

// Command #9
void handle_cmd_set_IoT_server_IP(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "IoT server target IP changed successfully! New IP is: ",
                          INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
                          ECHO_ON);
}

// Command #10
void handle_cmd_print_IoT_server_IP()
{
    handle_cmd_helper_print("Current IoT server target IP is: ",
                            INBUILT_STORAGE_ADDR_IOT_SERVER_IP);
}

// Command #11
void handle_cmd_set_IoT_server_port(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "IoT server target port changed successfully! New port is: ",
                          INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
                          ECHO_ON);
}

// Command #12
void handle_cmd_print_IoT_server_port()
{
    handle_cmd_helper_print("Current IoT server target port is: ",
                            INBUILT_STORAGE_ADDR_IOT_SERVER_PORT);
}

// Command #13
void handle_cmd_set_IoT_req_msg(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "IoT server request text changed successfully! New text is: ",
                          INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
                          ECHO_ON);
}

// Command #14
void handle_cmd_print_IoT_req_msg()
{
    handle_cmd_helper_print("Current IoT server request text is: ",
                            INBUILT_STORAGE_ADDR_IOT_REQ_MSG);
}

// Command #15
void handle_cmd_set_IoT_req_period(char *cmd)
{
    handle_cmd_helper_set(cmd,
                          "IoT server request period changed successfully! New period (ms) is: ",
                          INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
                          ECHO_ON);
}

// Command #16
void handle_cmd_set_BT_flag(char *cmd)
{
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        char *cmd_val = strstr(cmd, "=") + 1;

        if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
            handle_cmd_helper_set(cmd,
                                  "Bluetooth: ",
                                  INBUILT_STORAGE_ADDR_BT_FLAG,
                                  ECHO_ON);
            handle_cmd_rst_local_conn();
        } else {
            handle_cmd_err_val();
        }
    #endif
}

// Command #17
void handle_cmd_set_BT_dev_name(char *cmd)
{
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        handle_cmd_helper_set(cmd,
                              "Bluetooth device name changed successfully! New name is: ",
                              INBUILT_STORAGE_ADDR_BT_DEV_NAME,
                              ECHO_ON);

        handle_cmd_helper_send("Please reboot your device or reset local connection to put changes into effect.");
    #endif
}

// Command #18
void handle_cmd_print_BT_dev_name()
{
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        handle_cmd_helper_print("Current Bluetooth device name is: ",
                                INBUILT_STORAGE_ADDR_BT_DEV_NAME);
    #endif
}

// Command #19
void handle_cmd_set_RSSI_print_flag(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;
    
    if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
        handle_cmd_helper_set(cmd,
                              "RSSI print: ",
                              INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG,
                              ECHO_ON);
    } else {
        handle_cmd_err_val();
    }
}


/*--- Handler function subroutines ---*/

// Subroutine for "set" handlers.
void handle_cmd_helper_set(char *cmd, const char *topic, uint32_t addr, bool echo)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    inbuilt_storage_write(cmd_val,
                          strlen(cmd_val),
                          STR_MAX_LEN,
                          addr);

    char msg[STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, topic);

    if (echo) {
        strcat(msg, cmd_val);
    }

    handle_cmd_helper_send(msg);
    time_to_refresh_strd_vals = 1;
}

// Subroutine for "print" handlers.
void handle_cmd_helper_print(const char *topic, uint32_t addr)
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, topic);

    char val[INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
    inbuilt_storage_read(val,
                         sizeof(val),
                         STR_MAX_LEN,
                         addr);
    strcat(msg, val);
    handle_cmd_helper_send(msg);
}

// Subroutine for sending messages.
void handle_cmd_helper_send(const char *msg)
{
    Serial.println(msg);
    ESP_TCP_server_send_msg(msg);

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        char val[INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
        inbuilt_storage_read(val,
                             sizeof(val),
                             STR_MAX_LEN,
                             INBUILT_STORAGE_ADDR_BT_FLAG);

        if (!strcmp(val, "ON")) {
            ESP32_BT_send_msg(msg);
        }
    #endif
}


/*--- Misc functions ---*/

// Read config values from the inbuilt storage into the struct.
void strd_vals_read(strd_vals_t *_strd_vals)
{
    // Indices in the array of strings read from the inbuilt storage.
    #define INDEX_SSID               0
    #define INDEX_PSWD               1
    #define INDEX_LOCAL_SERVER_PORT  2
    #define INDEX_IOT_FLAG           3
    #define INDEX_IOT_SERVER_IP      4
    #define INDEX_IOT_SERVER_PORT    5
    #define INDEX_IOT_REQ_MSG        6
    #define INDEX_IOT_REQ_PERIOD     7
    #define INDEX_BT_FLAG            8
    #define INDEX_BT_DEV_NAME        9
    #define INDEX_RSSI_PRINT_FLAG    10


    /*--- Reading strings from the inbuilt storage ---*/
    
    char strd_vals_str[INBUILT_STORAGE_ITEM_LIST_LEN][INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
    uint32_t strd_vals_addr[] = {
        INBUILT_STORAGE_ADDR_SSID,
        INBUILT_STORAGE_ADDR_PSWD,
        INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
        INBUILT_STORAGE_ADDR_IOT_FLAG,
        INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
        INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
        INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
        INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
        INBUILT_STORAGE_ADDR_BT_FLAG,
        INBUILT_STORAGE_ADDR_BT_DEV_NAME,
        INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG
    };

    uint32_t strd_vals_addr_list_len = sizeof(strd_vals_addr) / sizeof(uint32_t);
    if (INBUILT_STORAGE_ITEM_LIST_LEN != strd_vals_addr_list_len) {
        Serial.println("");
        Serial.println("Error reading from the inbuilt storage: function strd_vals_read().");

        return;
    }

    for (uint32_t i = 0; i < INBUILT_STORAGE_ITEM_LIST_LEN; ++i) {
        inbuilt_storage_read(strd_vals_str[i],
                             sizeof(strd_vals_str[i]),
                             INBUILT_STORAGE_STR_MAX_LEN,
                             strd_vals_addr[i]);
    }


    /*--- Writing of strings and extracted integers into the struct ---*/
    
    // Local SSID.
    strcpy(_strd_vals->SSID, strd_vals_str[INDEX_SSID]);
    
    // Local access point password.
    strcpy(_strd_vals->pswd, strd_vals_str[INDEX_PSWD]);
    
    // Local TCP server port.
    _strd_vals->local_server_port = strtol(strd_vals_str[INDEX_LOCAL_SERVER_PORT], NULL, 10);

    // IoT mode (attempts to connect to a remote server) flag.
    if (!strcmp(strd_vals_str[INDEX_IOT_FLAG], "ON")) {
        _strd_vals->IoT_flag = 1;
    } else {
        _strd_vals->IoT_flag = 0;
    }

    // Remote server IP.
    strcpy(_strd_vals->IoT_server_IP, strd_vals_str[INDEX_IOT_SERVER_IP]);
    
    // Remote server port.
    _strd_vals->IoT_server_port = strtol(strd_vals_str[INDEX_IOT_SERVER_PORT], NULL, 10);
    
    // Text of a request to a remote server.
    strcpy(_strd_vals->IoT_req_msg, strd_vals_str[INDEX_IOT_REQ_MSG]);
    
    // Interval for sending requests to a remote server.
    _strd_vals->IoT_req_period = strtol(strd_vals_str[INDEX_IOT_REQ_PERIOD], NULL, 10);

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        // Bluetooth functionality flag.
        if (!strcmp(strd_vals_str[INDEX_BT_FLAG], "ON")) {
            _strd_vals->BT_flag = 1;
        } else {
            _strd_vals->BT_flag = 0;
        }

        // ESP's name as a Bluetooth slave device
        strcpy(_strd_vals->BT_dev_name, strd_vals_str[INDEX_BT_DEV_NAME]);
    #endif

    // RSSI printout flag.
    if (!strcmp(strd_vals_str[INDEX_RSSI_PRINT_FLAG], "ON")) {
        _strd_vals->RSSI_print_flag = 1;
    } else {
        _strd_vals->RSSI_print_flag = 0;
    }
}
