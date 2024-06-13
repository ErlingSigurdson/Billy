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

// Main Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_general.h"
#include "config_inbuilt_storage.h"

// Local modules.
#include "cmd.h"
#include "inbuilt_storage.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"

#if defined ESP32 && defined BT_CLASSIC_PROVIDED
    #include "ESP32_Bluetooth_Classic.h"
#endif


/******************* FUNCTIONS ******************/

/*--- Buffer contents check ---*/

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


/*--- Auxiliary functions (helper functions, accessories) ---*/

void cmd_aux_output(const char *msg)
{
    Serial.println(msg);
    ESP_TCP_server_send_msg(msg);

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        char val[STR_MAX_LEN + 1] = {0};
        inbuilt_storage_read(val,
                             sizeof(val),
                             INBUILT_STORAGE_STR_MAX_LEN,
                             INBUILT_STORAGE_ADDR_BT_CLASSIC_FLAG);
        if (!strcmp(val, "ON")) {
            ESP32_BT_Classic_send_msg(msg);
        }
    #endif
}

void cmd_aux_set_digital_output(uint8_t pin, uint8_t state, const char *topic)
{
    digitalWrite(pin, state);
    cmd_aux_output(topic);
}

void cmd_aux_set_PWM(uint8_t pin, uint32_t val, const char *topic)
{
    analogWrite(pin, val);
    cmd_aux_output(topic);
}

void cmd_aux_set_config(const char *cmd, uint32_t addr, const char *topic, bool echo_val, bool *refresh_flag)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    inbuilt_storage_write(cmd_val,
                          strlen(cmd_val),
                          INBUILT_STORAGE_STR_MAX_LEN,
                          addr);

    char msg[STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, topic);

    if (echo_val) {
        strcat(msg, cmd_val);
    }
    cmd_aux_output(msg);

    *refresh_flag = 1;
}

void cmd_aux_output_config(uint32_t addr, const char *topic)
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, topic);

    char val[STR_MAX_LEN + 1] = {0};
    inbuilt_storage_read(val,
                         sizeof(val),
                         INBUILT_STORAGE_STR_MAX_LEN,
                         addr);
    strcat(msg, val);

    cmd_aux_output(msg);
}


/*--- Handler functions ---*/

void cmd_handler_err_len()
{
    cmd_aux_output("Command buffer overflow.");
}

void cmd_handler_err_prefix()
{
    cmd_aux_output("Invalid or absent command prefix.");
}

void cmd_handler_err_cmd()
{
    cmd_aux_output("No valid command entered.");
}

void cmd_handler_err_val()
{
    cmd_aux_output("No valid value submitted.");
}

// Command #1
void cmd_handler_set_load_digital(const char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "TOGGLE")) {
        if (digitalRead(DIGITAL_OUTPUT_PIN) == DIGITAL_OUTPUT_LOAD_ON) {
            cmd_aux_set_digital_output(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_LOAD_OFF, "Two-state load is now OFF");
            return;
        } else {
            cmd_aux_set_digital_output(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_LOAD_ON, "Two-state load is now ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "ON")) {
        if (digitalRead(DIGITAL_OUTPUT_PIN) != DIGITAL_OUTPUT_LOAD_ON) {
            cmd_aux_set_digital_output(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_LOAD_ON, "Two-state load is now ON");
            return;
        } else {
            cmd_aux_output("Two-state load is already ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "OFF")) {
        if (digitalRead(DIGITAL_OUTPUT_PIN) != DIGITAL_OUTPUT_LOAD_OFF) {
            cmd_aux_set_digital_output(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_LOAD_OFF, "Two-state load is now OFF");
            return;
        } else {
            cmd_aux_output("Two-state load is already OFF");
            return;
        }
    }

    cmd_handler_err_val();
}

// Command #2
void cmd_handler_set_load_PWM(const char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    uint32_t val_len = (uint32_t)strlen(cmd_val);
    if (val_len < 1 || val_len > 3) {  // Valid duty cycle values are 0 to 255.
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
    if (val > 255) {                        // Valid duty cycle values are 0 to 255.
        cmd_handler_err_val();
        return;
    }

    char msg[STR_MAX_LEN * 2 + 1] = "Analog load duty cycle set to ";
    strcat(msg, cmd_val);

    cmd_aux_set_PWM(PWM_OUTPUT_PIN, val, msg);
}

// Command #3
void cmd_handler_output_load_digital()
{
    if (digitalRead(DIGITAL_OUTPUT_PIN) == DIGITAL_OUTPUT_LOAD_ON) {
        cmd_aux_output("Current digital load state is ON");
    } else {
        cmd_aux_output("Current digital load state is OFF");
    }
}

// Command #4
void cmd_handler_set_local_SSID(const char *cmd, bool *refresh_flag)
{
    cmd_aux_set_config(cmd,
                       INBUILT_STORAGE_ADDR_LOCAL_SSID,
                       "SSID changed successfully! New SSID is: ",
                       ECHO_VAL_ON,
                       refresh_flag);
}

// Command #5
void cmd_handler_output_local_SSID()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_LOCAL_SSID,
                          "Current SSID is: ");
}

// Command #6
void cmd_handler_set_local_pswd(const char *cmd, bool *refresh_flag)
{
    cmd_aux_set_config(cmd,
                       INBUILT_STORAGE_ADDR_LOCAL_PSWD,
                       "Password changed successfully!",
                       ECHO_VAL_OFF,
                       refresh_flag);
}

// Command #7
void cmd_handler_set_local_server_port(const char *cmd, bool *refresh_flag)
{
    cmd_aux_set_config(cmd,
                       INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                       "Local server port changed successfully! New port is: ",
                       ECHO_VAL_ON,
                       refresh_flag);

    cmd_aux_output("Please reboot your device or reset local connections to put changes into effect.");
}

// Command #8
void cmd_handler_output_local_server_port()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                          "Current local server port is: ");
}

// Command #9
void cmd_handler_output_local_server_IP()
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    String current_IP = ESP_WiFi_get_devices_current_IP();
    strcpy(msg, "Current IP address is: ");
    strcat(msg, current_IP.c_str());
    cmd_aux_output(msg);
}

// Command #10
void cmd_handler_local_conn_rst(void (*setup_ptr)(void))
{
    cmd_aux_output("Resetting local connections...");
    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
    ESP_TCP_server_stop(CONN_SHUTDOWN_DOWNTIME);

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        ESP32_BT_Classic_stop(CONN_SHUTDOWN_DOWNTIME);
    #endif

    setup_ptr();
}

// Command #11
void cmd_handler_set_IoT_flag(const char *cmd, bool *refresh_flag)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
        cmd_aux_set_config(cmd,
                           INBUILT_STORAGE_ADDR_IOT_FLAG,
                           "Requests to an IoT server: ",
                           ECHO_VAL_ON,
                           refresh_flag);
    } else {
        cmd_handler_err_val();
    }
}

// Command #12
void cmd_handler_set_IoT_server_IP(const char *cmd, bool *refresh_flag)
{
    cmd_aux_set_config(cmd,
                       INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
                       "IoT server target IP changed successfully! New IP is: ",
                       ECHO_VAL_ON,
                       refresh_flag);
}

// Command #13
void cmd_handler_output_IoT_server_IP()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
                          "Current IoT server target IP is: ");
}

// Command #14
void cmd_handler_set_IoT_server_port(const char *cmd, bool *refresh_flag)
{
    cmd_aux_set_config(cmd,
                       INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
                       "IoT server target port changed successfully! New port is: ",
                       ECHO_VAL_ON,
                       refresh_flag);
}

// Command #15
void cmd_handler_output_IoT_server_port()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
                          "Current IoT server target port is: ");
}

// Command #16
void cmd_handler_set_IoT_req_msg(const char *cmd, bool *refresh_flag)
{
    cmd_aux_set_config(cmd,
                       INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
                       "IoT server request text changed successfully! New text is: ",
                       ECHO_VAL_ON,
                       refresh_flag);
}

// Command #17
void cmd_handler_output_IoT_req_msg()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
                          "Current IoT server request text is: ");
}

// Command #18
void cmd_handler_set_IoT_req_period(const char *cmd, bool *refresh_flag)
{
    cmd_aux_set_config(cmd,
                       INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
                       "IoT server request period changed successfully! New period (in ms) is: ",
                       ECHO_VAL_ON,
                       refresh_flag);
}

// Command #19
void cmd_handler_set_BT_Classic_flag(const char *cmd, void (*setup_ptr)(void), bool *refresh_flag)
{
    // Dummy statements to prevent warnings connected to a conditional compilation.
    (void)cmd;
    (void)setup_ptr;
    (void)refresh_flag;

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        char *cmd_val = strstr(cmd, "=") + 1;

        if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
            cmd_aux_set_config(cmd,
                               INBUILT_STORAGE_ADDR_BT_CLASSIC_FLAG,
                               "Bluetooth Classic: ",
                               ECHO_VAL_ON,
                               refresh_flag);
            cmd_handler_local_conn_rst(setup_ptr);
        } else {
            cmd_handler_err_val();
        }
    #endif
}

// Command #20
void cmd_handler_set_BT_Classic_dev_name(const char *cmd, bool *refresh_flag)
{
    // Dummy statements to prevent warnings connected to a conditional compilation.
    (void)cmd;
    (void)refresh_flag;

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        cmd_aux_set_config(cmd,
                           INBUILT_STORAGE_ADDR_BT_CLASSIC_DEV_NAME,
                           "Bluetooth Classic device name changed successfully! New name is: ",
                           ECHO_VAL_ON,
                           refresh_flag);

        cmd_aux_output("Please reboot your device or reset local connections to put changes into effect.");
    #endif
}

// Command #21
void cmd_handler_output_BT_Classic_dev_name()
{
    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        cmd_aux_output_config(INBUILT_STORAGE_ADDR_BT_CLASSIC_DEV_NAME,
                              "Current Bluetooth Classic device name is: ");
    #endif
}

// Command #22
void cmd_handler_set_RSSI_output_flag(const char *cmd, bool *refresh_flag)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
        cmd_aux_set_config(cmd,
                           INBUILT_STORAGE_ADDR_RSSI_OUTPUT_FLAG,
                           "RSSI print: ",
                           ECHO_VAL_ON,
                           refresh_flag);
    } else {
        cmd_handler_err_val();
    }
}
