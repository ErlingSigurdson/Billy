#include <Arduino.h>
#include "cmd_handlers.h"

#include "utilities.h"
#include "inbuilt_storage.h"
#include "HW_UART.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "ESP_HTTP.h"

#if defined ESP32 && defined BT_CLASSIC_PROVIDED
    #include "ESP32_Bluetooth.h"
#endif


/*************** GLOBAL VARIABLES ***************/

bool time_to_refresh_strd_vals = 1;


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
    uint32_t current_state = digitalRead(LOAD_PIN);

    if (!strcmp(cmd_val, "TOGGLE")) {
        if (current_state == PIN_ON) {
            strcpy(cmd_val, "OFF");
        } else {
            strcpy(cmd_val, "ON");
        }
    }

    if (!strcmp(cmd_val, "ON")) {
        if (current_state != PIN_ON) {
            digitalWrite(LOAD_PIN, (uint8_t)PIN_ON);
            handle_cmd_helper_send("LOAD is now ON");

            return;
        } else {
            handle_cmd_helper_send("LOAD is already ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "OFF")) {
        if (current_state != PIN_OFF) {
            digitalWrite(LOAD_PIN, (uint8_t)PIN_OFF);
            handle_cmd_helper_send("LOAD is now OFF");

            return;
        } else {
            handle_cmd_helper_send("LOAD is already OFF");
            return;
        }
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
void handle_cmd_rst_local_conn(void (*setup_ptr)(void))
{
    handle_cmd_helper_send("Resetting local connections...");
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
    ESP_TCP_server_stop(CONN_SHUTDOWN_DOWNTIME);

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        ESP32_BT_stop(CONN_SHUTDOWN_DOWNTIME);
    #endif

    setup_ptr();
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
void handle_cmd_set_BTClassic_flag(char *cmd, void (*setup_ptr)(void))
{
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        char *cmd_val = strstr(cmd, "=") + 1;

        if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
            handle_cmd_helper_set(cmd,
                                  "Bluetooth: ",
                                  INBUILT_STORAGE_ADDR_BT_CLASSIC_FLAG,
                                  ECHO_ON);
            handle_cmd_rst_local_conn(setup_ptr);
        } else {
            handle_cmd_err_val();
        }
    #endif
}

// Command #17
void handle_cmd_set_BTClassic_dev_name(char *cmd)
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
void handle_cmd_print_BTClassic_dev_name()
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
                             INBUILT_STORAGE_ADDR_BT_CLASSIC_FLAG);

        if (!strcmp(val, "ON")) {
            ESP32_BT_send_msg(msg);
        }
    #endif
}
