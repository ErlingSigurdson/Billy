/****************** DESCRIPTION *****************/

/**
 * Filename: cmd.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: command processing.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// General Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_general.h"

// Local modules.
#include "cmd.h"
#include "inbuilt_storage.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"

#if defined ESP32 && defined BT_CLASSIC_PROVIDED
    #include "ESP32_Bluetooth_Classic.h"
#endif


/******************* FUNCTIONS ******************/

/*--- Contents checking function ---*/

int32_t cmd_check(const char *buf, const char *prefix, const char *cmd_list[], uint32_t cmd_list_len)
{
    if (strstr(buf, prefix) != buf) {
        return -1;
    }

    for (uint32_t i = 1; i <= cmd_list_len; ++i) {
        if (strstr(buf, cmd_list[i]) == buf + strlen(prefix)) {
            return i;
        }
    }

    return 0;
}


/*--- Handler functions ---*/

void cmd_handler_err_len()
{
    cmd_aux_print_and_send_msg("Command buffer overflow.");
}

void cmd_handler_err_prefix()
{
    cmd_aux_print_and_send_msg("Invalid prefix for a command.");
}

void cmd_handler_err_cmd()
{
    cmd_aux_print_and_send_msg("No valid command entered.");
}

void cmd_handler_err_val()
{
    cmd_aux_print_and_send_msg("No valid value submitted.");
}

// Command #0
void cmd_handler_set_digital_load(const char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "TOGGLE")) {
        if (digitalRead(DIGITAL_LOAD_PIN) == DIGITAL_LOAD_ON) {
            cmd_aux_set_digital_load(DIGITAL_LOAD_PIN, 0, "Digital load is now OFF");
            return;
        } else {
            cmd_aux_set_digital_load(DIGITAL_LOAD_PIN, 1, "Digital load is now ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "ON")) {
        if (digitalRead(DIGITAL_LOAD_PIN) != DIGITAL_LOAD_ON) {
            cmd_aux_set_digital_load(DIGITAL_LOAD_PIN, 1, "Digital load is now ON");
            return;
        } else {
            cmd_aux_print_and_send_msg("Digital load is already ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "OFF")) {
        if (digitalRead(DIGITAL_LOAD_PIN) != DIGITAL_LOAD_OFF) {
            cmd_aux_set_digital_load(DIGITAL_LOAD_PIN, 0, "Digital load is now OFF");
            return;
        } else {
            cmd_aux_print_and_send_msg("Digital load is already OFF");
            return;
        }
    }

    cmd_handler_err_val();
}

// Command #1
void cmd_handler_set_analog_load(const char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    uint32_t val_len = (uint32_t)strlen(cmd_val);
    if (val_len < 1 || val_len > 3) {
        cmd_handler_err_val();
        return;
    }

    for (uint32_t i = 0; i < val_len; ++i) {
        if (cmd_val[i] < '0' || cmd_val[i] > '9') {
            cmd_handler_err_val();
            return;
        }
    }

    uint32_t val = strtol(cmd_val, 0, 10);  // Convert to decimal.
    if (val > (2 << 7) - 1) {
        cmd_handler_err_val();
        return;
    }
    
    char msg[STR_MAX_LEN + 1] = "Analog load duty cycle set to ";
    strcat(msg, cmd_val);

    cmd_aux_set_analog_load(ANALOG_LOAD_PIN, val, msg);
}

// Command #2
void cmd_handler_output_digital_load()
{
    if (digitalRead(DIGITAL_LOAD_PIN) == DIGITAL_LOAD_ON) {
        cmd_aux_print_and_send_msg("Current digital load state is ON");
    } else {
        cmd_aux_print_and_send_msg("Current digital load state is OFF");
    }
}

/* To print or not to print a new value of an updated config.
 * OFF for password, ON for other cases.
 */
#define ECHO_ON 1
#define ECHO_OFF 0

// Command #3
void cmd_handler_update_local_SSID(const char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "SSID changed successfully! New SSID is: ",
                          INBUILT_STORAGE_ADDR_SSID,
                          ECHO_ON,
                          refresh_flag);
}

// Command #4
void cmd_handler_output_local_SSID()
{
    cmd_aux_output_config("Current SSID is: ",
                          INBUILT_STORAGE_ADDR_SSID);
}

// Command #5
void cmd_handler_update_local_pswd(const char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "Password changed successfully!",
                          INBUILT_STORAGE_ADDR_PSWD,
                          ECHO_OFF,
                          refresh_flag);
}

// Command #6
void cmd_handler_update_local_port(const char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "Local server port changed successfully! New port is: ",
                          INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                          ECHO_ON,
                          refresh_flag);

    cmd_aux_print_and_send_msg("Please reboot your device or reset local connection to put changes into effect.");
}

// Command #7
void cmd_handler_output_local_port()
{
    cmd_aux_output_config("Current local server port is: ",
                          INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT);
}

// Command #8
void cmd_handler_output_local_IP()
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    String current_IP = ESP_WiFi_get_current_IP();
    strcpy(msg, "Current IP address is: ");
    strcat(msg, current_IP.c_str());
    cmd_aux_print_and_send_msg(msg);
}

// Command #9
void cmd_handler_rst_local_conn(void (*setup_ptr)(void))
{
    cmd_aux_print_and_send_msg("Resetting local connections...");
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
    ESP_TCP_server_stop(CONN_SHUTDOWN_DOWNTIME);

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        ESP32_BT_Classic_stop(CONN_SHUTDOWN_DOWNTIME);
    #endif

    setup_ptr();
}

// Command #10
void cmd_handler_update_IoT_flag(const char *cmd, bool *refresh_flag)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
        cmd_aux_update_config(cmd,
                              "Requests to an IoT server: ",
                              INBUILT_STORAGE_ADDR_IOT_FLAG,
                              ECHO_ON,
                              refresh_flag);
    } else {
        cmd_handler_err_val();
    }
}

// Command #11
void cmd_handler_update_IoT_server_IP(const char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "IoT server target IP changed successfully! New IP is: ",
                          INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
                          ECHO_ON,
                          refresh_flag);
}

// Command #12
void cmd_handler_output_IoT_server_IP()
{
    cmd_aux_output_config("Current IoT server target IP is: ",
                          INBUILT_STORAGE_ADDR_IOT_SERVER_IP);
}

// Command #13
void cmd_handler_update_IoT_server_port(const char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "IoT server target port changed successfully! New port is: ",
                          INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
                          ECHO_ON,
                          refresh_flag);
}

// Command #14
void cmd_handler_output_IoT_server_port()
{
    cmd_aux_output_config("Current IoT server target port is: ",
                            INBUILT_STORAGE_ADDR_IOT_SERVER_PORT);
}

// Command #15
void cmd_handler_update_IoT_req_msg(const char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "IoT server request text changed successfully! New text is: ",
                          INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
                          ECHO_ON,
                          refresh_flag);
}

// Command #16
void cmd_handler_output_IoT_req_msg()
{
    cmd_aux_output_config("Current IoT server request text is: ",
                          INBUILT_STORAGE_ADDR_IOT_REQ_MSG);
}

// Command #17
void cmd_handler_update_IoT_req_period(const char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "IoT server request period changed successfully! New period (ms) is: ",
                          INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
                          ECHO_ON,
                          refresh_flag);
}

// Command #18
void cmd_handler_update_BT_Classic_flag(const char *cmd, void (*setup_ptr)(void), bool *refresh_flag)
{
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        char *cmd_val = strstr(cmd, "=") + 1;

        if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
            cmd_aux_update_config(cmd,
                                  "Bluetooth: ",
                                  INBUILT_STORAGE_ADDR_BT_CLASSIC_FLAG,
                                  ECHO_ON,
                                  refresh_flag);
            cmd_handler_rst_local_conn(setup_ptr);
        } else {
            cmd_handler_err_val();
        }
    #endif
}

// Command #19
void cmd_handler_update_BT_Classic_dev_name(const char *cmd, bool *refresh_flag)
{
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        cmd_aux_update_config(cmd,
                              "Bluetooth device name changed successfully! New name is: ",
                              INBUILT_STORAGE_ADDR_BT_CLASSIC_DEV_NAME,
                              ECHO_ON,
                              refresh_flag);

        cmd_aux_print_and_send_msg("Please reboot your device or reset local connection to put changes into effect.");
    #endif
}

// Command #20
void cmd_handler_output_BT_Classic_dev_name()
{
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        cmd_aux_output_config("Current Bluetooth device name is: ",
                              INBUILT_STORAGE_ADDR_BT_CLASSIC_DEV_NAME);
    #endif
}

// Command #21
void cmd_handler_update_RSSI_print_flag(const char *cmd, bool *refresh_flag)
{
    char *cmd_val = strstr(cmd, "=") + 1;
    
    if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
        cmd_aux_update_config(cmd,
                              "RSSI print: ",
                              INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG,
                              ECHO_ON,
                              refresh_flag);
    } else {
        cmd_handler_err_val();
    }
}


/*--- Auxiliary functions ---*/

void cmd_aux_print_and_send_msg(const char *msg)
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
            ESP32_BT_Classic_send_msg(msg);
        }
    #endif
}

void cmd_aux_output_config(const char *topic, uint32_t addr)
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, topic);

    char val[INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
    inbuilt_storage_read(val,
                         sizeof(val),
                         STR_MAX_LEN,
                         addr);
    strcat(msg, val);
    
    cmd_aux_print_and_send_msg(msg);
}

void cmd_aux_update_config(const char *cmd, const char *topic, uint32_t addr, bool echo, bool *refresh_flag)
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
    cmd_aux_print_and_send_msg(msg);

    *refresh_flag = 1;
}

void cmd_aux_set_digital_load(uint8_t pin, uint8_t state, const char *msg)
{
    digitalWrite(pin, state);
    cmd_aux_print_and_send_msg(msg);
}

void cmd_aux_set_analog_load(uint8_t pin, uint32_t val, const char *msg)
{
    analogWrite(pin, val);
    cmd_aux_print_and_send_msg(msg);    
}
