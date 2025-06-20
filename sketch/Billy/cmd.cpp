/****************** DESCRIPTION *****************/

/**
 * Filename: cmd.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Text commands processing.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_general.h"
#include "config_inbuilt_storage.h"

// Local modules.
#include "cmd.h"
#include "stored_configs.h"
#include "inbuilt_storage.h"
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "utilities.h"
#include "version.h"

#if defined ESP32 && defined BTCLASSIC_PROVIDED
    #include "ESP32_BTClassic.h"
#endif


/******************* FUNCTIONS ******************/

/*--- Buffer contents check ---*/

int32_t cmd_check(char *buf, const char *prefix, const char *cmd_list[], uint32_t cmd_list_len)
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

    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        char config_val[STR_MAX_LEN + 1] = {0};
        inbuilt_storage_read_string(config_val,
                                    sizeof(config_val),
                                    INBUILT_STORAGE_STR_MAX_LEN,
                                    INBUILT_STORAGE_ADDR_BTCLASSIC_FLAG);
        if (!strcmp(config_val, "ON")) {
            ESP32_BTClassic_send_msg(msg);
        }
    #endif
}

bool cmd_aux_has_decimal_only(const char *str)
{
    for (uint32_t i = 0; i < (uint32_t)strlen(str); ++i) {
        if ((str[i] < '0' && str[i] != '.') ||
            (str[i] > '9' && str[i] != '.')) {
            return 0;
        }
    }

    return 1;
}

void cmd_aux_set_output_digital(uint8_t pin, uint8_t state, const char *topic)
{
    if (pin == 0) {
        cmd_aux_output("Digital output pin not specified.");
        return;
    }

    digitalWrite(pin, state);
    cmd_aux_output(topic);
}

void cmd_aux_set_output_PWM(uint8_t pin, uint32_t val, const char *topic)
{
    if (pin == 0) {
        cmd_aux_output("PWM output pin not specified.");
        return;
    }

    analogWrite(pin, val);
    cmd_aux_output(topic);
}

void cmd_aux_set_config(set_config_params_t *params)
{
    char *cmd_val = strstr(params->cmd, "=") + 1;

    if (params->decimal_only) {
        if (!cmd_aux_has_decimal_only(cmd_val)) {
            cmd_handler_err_val();
            return;
        }
    }

    inbuilt_storage_write_string(cmd_val,
                                 strlen(cmd_val),
                                 INBUILT_STORAGE_STR_MAX_LEN,
                                 params->addr);
    *(params->refresh_flag) = 1;

    char msg[STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, params->topic);

    if (params->echo_val) {
        strcat(msg, cmd_val);
    }

    cmd_aux_output(msg);
}

void cmd_aux_output_config(uint32_t addr, const char *topic)
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, topic);

    char config_val[STR_MAX_LEN + 1] = {0};
    inbuilt_storage_read_string(config_val,
                                sizeof(config_val),
                                INBUILT_STORAGE_STR_MAX_LEN,
                                addr);
    strcat(msg, config_val);

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
void cmd_handler_set_load_digital(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "TOGGLE")) {
        if (digitalRead(DIGITAL_OUTPUT_PIN) == DIGITAL_OUTPUT_LOAD_ON) {
            cmd_aux_set_output_digital(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_LOAD_OFF, "Two-state load is now OFF");
            return;
        } else {
            cmd_aux_set_output_digital(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_LOAD_ON, "Two-state load is now ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "ON")) {
        if (digitalRead(DIGITAL_OUTPUT_PIN) != DIGITAL_OUTPUT_LOAD_ON) {
            cmd_aux_set_output_digital(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_LOAD_ON, "Two-state load is now ON");
            return;
        } else {
            cmd_aux_output("Two-state load is already ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "OFF")) {
        if (digitalRead(DIGITAL_OUTPUT_PIN) != DIGITAL_OUTPUT_LOAD_OFF) {
            cmd_aux_set_output_digital(DIGITAL_OUTPUT_PIN, DIGITAL_OUTPUT_LOAD_OFF, "Two-state load is now OFF");
            return;
        } else {
            cmd_aux_output("Two-state load is already OFF");
            return;
        }
    }

    cmd_handler_err_val();
}

// Command #2
void cmd_handler_set_load_PWM(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!cmd_aux_has_decimal_only(cmd_val)) {
        cmd_handler_err_val();
        return;
    }

    uint32_t val_len = (uint32_t)strlen(cmd_val);
    if (val_len < 1 || val_len > 3) {  // Valid duty cycle values are 0 to 255.
        cmd_handler_err_val();
        return;
    }

    uint32_t duty_cycle = strtol(cmd_val, 0, 10);  // Convert to decimal.
    if (duty_cycle > 255) {                        // Valid duty cycle values are 0 to 255.
        cmd_handler_err_val();
        return;
    }

    char msg[STR_MAX_LEN * 2 + 1] = "PWM duty cycle is set to ";
    strcat(msg, cmd_val);

    cmd_aux_set_output_PWM(PWM_OUTPUT_PIN, duty_cycle, msg);
}

// Command #3
void cmd_handler_output_load_digital()
{
    if (DIGITAL_OUTPUT_PIN == 0) {
        cmd_aux_output("Digital output pin not specified.");
        return;
    }

    if (digitalRead(DIGITAL_OUTPUT_PIN) == DIGITAL_OUTPUT_LOAD_ON) {
        cmd_aux_output("Current load state is ON");
    } else {
        cmd_aux_output("Current load state is OFF");
    }
}

// Command #4
void cmd_handler_set_WiFi_SSID(char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_WIFI_SSID,
                                  ANY_CHAR,
                                  "SSID changed successfully! New SSID is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd_aux_set_config(&params);
}

// Command #5
void cmd_handler_output_WiFi_SSID()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_WIFI_SSID ,
                          "Current SSID is: ");
}

// Command #6
void cmd_handler_set_WiFi_pswd(char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_WIFI_PSWD,
                                  ANY_CHAR,
                                  "Password changed successfully!",
                                  ECHO_VAL_OFF,
                                  refresh_flag};
    cmd_aux_set_config(&params);
}

// Command #7
void cmd_handler_set_WiFi_RSSI_output_flag(char *cmd, bool *refresh_flag)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
        set_config_params_t params = {cmd,
                                      INBUILT_STORAGE_ADDR_WIFI_RSSI_OUTPUT_FLAG,
                                      ANY_CHAR,
                                      "RSSI output: ",
                                      ECHO_VAL_ON,
                                      refresh_flag};
        cmd_aux_set_config(&params);
    } else {
        cmd_handler_err_val();
    }
}

// Command #8
void cmd_handler_set_WiFi_autoreconnect_flag(char *cmd, bool *refresh_flag)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
        set_config_params_t params = {cmd,
                                      INBUILT_STORAGE_ADDR_WIFI_AUTORECONNECT_FLAG,
                                      ANY_CHAR,
                                      "Automatic Wi-Fi reconnect attempts: ",
                                      ECHO_VAL_ON,
                                      refresh_flag};
        cmd_aux_set_config(&params);
    } else {
        cmd_handler_err_val();
    }
}

// Command #9
void cmd_handler_output_local_server_IP()
{
    char msg[STR_MAX_LEN * 2 + 1] = {0};
    String current_IP = ESP_WiFi_get_devices_current_IP();
    strcpy(msg, "Current local IP address is: ");
    strcat(msg, current_IP.c_str());
    cmd_aux_output(msg);
}

// Command #10
void cmd_handler_set_local_server_port(char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                                  DECIMAL_ONLY,
                                  "Local server port changed successfully! New port is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd_aux_set_config(&params);

    stored_configs_t stored_configs;
    stored_configs_read(&stored_configs);

    ESP_TCP_server_stop(CONN_SHUTDOWN_DOWNTIME);
    ESP_TCP_server_port_update(stored_configs.local_server_port);
    ESP_TCP_server_start();
}

// Command #11
void cmd_handler_output_local_server_port()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                          "Current local server port is: ");
}

// Command #12
void cmd_handler_set_IoT_flag(char *cmd, bool *refresh_flag)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
        set_config_params_t params = {cmd,
                                      INBUILT_STORAGE_ADDR_IOT_FLAG,
                                      ANY_CHAR,
                                      "Requests to an IoT server: ",
                                      ECHO_VAL_ON,
                                      refresh_flag};
        cmd_aux_set_config(&params);
    } else {
        cmd_handler_err_val();
    }
}

// Command #13
void cmd_handler_set_IoT_server_IP(char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
                                  DECIMAL_ONLY,
                                  "IoT server target IP changed successfully! New IP is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd_aux_set_config(&params);
}

// Command #14
void cmd_handler_output_IoT_server_IP()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
                          "Current IoT server target IP is: ");
}

// Command #15
void cmd_handler_set_IoT_server_port(char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
                                  DECIMAL_ONLY,
                                  "IoT server target port changed successfully! New port is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd_aux_set_config(&params);
}

// Command #16
void cmd_handler_output_IoT_server_port()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
                          "Current IoT server target port is: ");
}

// Command #17
void cmd_handler_set_IoT_req_msg(char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
                                  ANY_CHAR,
                                  "IoT server request text changed successfully! New text is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd_aux_set_config(&params);
}

// Command #18
void cmd_handler_output_IoT_req_msg()
{
    cmd_aux_output_config(INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
                          "Current IoT server request text is: ");
}

// Command #19
void cmd_handler_set_IoT_req_period(char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
                                  DECIMAL_ONLY,
                                  "IoT server request period changed successfully! New period (in ms) is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd_aux_set_config(&params);
}

// Command #20
void cmd_handler_set_BTClassic_flag(char *cmd,
                                    void (*setup_BTClassic_ptr)(stored_configs_t *),
                                    bool *refresh_flag)
{
    // Dummy statements to prevent warnings connected to a conditional compilation (unused parameter).
    (void)cmd;
    (void)setup_BTClassic_ptr;
    (void)refresh_flag;

    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        char *cmd_val = strstr(cmd, "=") + 1;

        if (!strcmp(cmd_val, "ON") || !strcmp(cmd_val, "OFF")) {
            set_config_params_t params = {cmd,
                                          INBUILT_STORAGE_ADDR_BTCLASSIC_FLAG,
                                          ANY_CHAR,
                                          "Bluetooth Classic status updated successfully!",
                                          ECHO_VAL_OFF,
                                          refresh_flag};
            cmd_aux_set_config(&params);

            stored_configs_t stored_configs;
            stored_configs_read(&stored_configs);
            setup_BTClassic_ptr(&stored_configs);

            Serial.println("");
        } else {
            cmd_handler_err_val();
        }
    #else
        cmd_handler_err_cmd();
    #endif
}

// Command #21
void cmd_handler_set_BTClassic_dev_name(char *cmd,
                                        void (*setup_BTClassic_ptr)(stored_configs_t *),
                                        bool *refresh_flag)
{
    // Dummy statements to prevent warnings connected to a conditional compilation (unused parameter).
    (void)cmd;
    (void)setup_BTClassic_ptr;
    (void)refresh_flag;

    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        set_config_params_t params = {cmd,
                                      INBUILT_STORAGE_ADDR_BTCLASSIC_DEV_NAME,
                                      ANY_CHAR,
                                      "Bluetooth Classic device name changed successfully!",
                                      ECHO_VAL_OFF,
                                      refresh_flag};
        cmd_aux_set_config(&params);

        stored_configs_t stored_configs;
        stored_configs_read(&stored_configs);
        setup_BTClassic_ptr(&stored_configs);

        Serial.println("");
    #else
        cmd_handler_err_cmd();
    #endif
}

// Command #22
void cmd_handler_output_BTClassic_dev_name()
{
    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        cmd_aux_output_config(INBUILT_STORAGE_ADDR_BTCLASSIC_DEV_NAME,
                              "Current Bluetooth Classic device name is: ");
    #else
        cmd_handler_err_cmd();
    #endif
}

// Command #23
void cmd_handler_all_conn_rst(bool (*setup_WiFi_ptr)(stored_configs_t *, uint32_t),
                              void (*setup_BTClassic_ptr)(stored_configs_t *),
                              stored_configs_t *stored_configs)
{
    cmd_aux_output("Resetting local connections...");

    ESP_TCP_clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
    ESP_TCP_server_stop(CONN_SHUTDOWN_DOWNTIME);

    #if defined ESP32 && defined BTCLASSIC_PROVIDED
        ESP32_BTClassic_stop(CONN_SHUTDOWN_DOWNTIME);
    #endif

    setup_WiFi_ptr(stored_configs, CONN_TIMEOUT);
    setup_BTClassic_ptr(stored_configs);

    Serial.println("");
}

// Command #24
void cmd_handler_output_version()
{
    char msg[STR_MAX_LEN * 4] = "\n" \
                                "\"Billy\" firmware version: " VERSION "\n" \
                                LINK_MESSAGE "\n" \
                                LINK_GITHUB "\n" \
                                LINK_GITFLIC "\n" \
                                "\n";

    strcat(msg, "Output settings:" "\n");
    if (DIGITAL_OUTPUT_PIN > 0) {
        strcat(msg, "Two-state output pin number (Arduino pinout): " STRINGIFY(DIGITAL_OUTPUT_PIN) "\n");
    } else {
        strcat(msg, "Two-state output pin not specified" "\n");
    }

    if (PWM_OUTPUT_PIN > 0) {
        strcat(msg, "PWM output pin number (Arduino pinout): " STRINGIFY(PWM_OUTPUT_PIN) "\n");
    } else {
        strcat(msg, "PWM output pin not specified" "\n");
    }

    if (WIFI_INDICATOR_LED_PIN > 0) {
        strcat(msg, "Wi-Fi indicator LED pin number (Arduino pinout): " STRINGIFY(WIFI_INDICATOR_LED_PIN) "\n");
    } else {
        strcat(msg, "Wi-Fi indicator LED pin number not specified" "\n");
    }

    cmd_aux_output(msg);
}
