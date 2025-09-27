/*************** FILE DESCRIPTION ***************/

/**
 * Filename: cmd.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Text commands processing.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- Includes ---*/

// This source file's own header file.
#include "cmd.h"

// Local modules.
#include "esp_wifi_billy.h"
#include "esp_tcp_billy.h"
#include "inbuilt_storage.h"
#include "stored_configs.h"
#include "cstring_utils.h"
#include "version.h"

#if defined ESP32 && defined BTCLASSIC_USED
    #include "esp32_btclassic_billy.h"
#endif

#if defined ESP32 && defined BLE_USED
    #include "esp32_ble_billy.h"
#endif

#ifdef RGB_LED
    #include "rgb_led.h"
#endif

// Project configs.
#include "config_general.h"
#include "config_inbuilt_storage.h"

// Arduino core header file.
#include <Arduino.h>


/******************* FUNCTIONS ******************/

/*--- Buffer contents check ---*/

int32_t cmd::match_in_buf(char *buf, const char *prefix, const char *cmd_list[], uint32_t cmd_list_len)
{
    static size_t prefix_len = strlen(prefix);

    if (!cstring_utils::are_equal_within_bytes(buf, prefix, prefix_len)) {
        return CMD_MATCH_IN_BUF_ERR_PREFIX;
    }

    char *buf_payload = buf + static_cast<uint32_t>(prefix_len);
    for (uint32_t i = 1; i <= cmd_list_len; ++i) {
        if (strstr(buf_payload, cmd_list[i]) == buf_payload) {
            return i;
        }
    }

    return CMD_MATCH_IN_BUF_ERR_CMD;
}


/*--- Auxiliary functions (helper functions, accessories) ---*/

void cmd::aux::output_msg(const char *msg)
{
    Serial.println(msg);
    esp_tcp_billy::server_send_msg(msg);

    #if defined ESP32 && defined BTCLASSIC_USED
        char config_val[BILLY_STR_MAX_LEN + 1] = {0};
        inbuilt_storage::read_string_from_storage(config_val,
                                                  sizeof(config_val),
                                                  BILLY_STR_MAX_LEN,
                                                  INBUILT_STORAGE_ADDR_BTCLASSIC_FLAG);
        if (cstring_utils::are_equal(config_val, "ON")) {
            esp32_btclassic_billy::send_msg(msg);
        }
    #endif
}

bool cmd::aux::has_decimal_only(const char *str)
{
    for (uint32_t i = 0; i < (uint32_t)strlen(str); ++i) {
        if ((str[i] < '0' && str[i] != '.') ||
            (str[i] > '9' && str[i] != '.')) {
            return 0;
        }
    }

    return 1;
}

void cmd::aux::set_output_digital(uint32_t pin, uint32_t state, const char *topic)
{
    if (pin == 0) {
        cmd::aux::output_msg("Digital output pin not specified.");
        return;
    }

    digitalWrite(pin, state);
    cmd::aux::output_msg(topic);
}

void cmd::aux::set_output_PWM(uint32_t pin, uint32_t val, const char *topic)
{
    if (pin == 0) {
        cmd::aux::output_msg("PWM output pin not specified.");
        return;
    }

    analogWrite(pin, val);
    cmd::aux::output_msg(topic);
}

void cmd::aux::set_config(set_config_params_t *params)
{
    char *cmd_val = strstr(params->cmd, "=") + 1;

    if (params->decimal_only) {
        if (!cmd::aux::has_decimal_only(cmd_val)) {
            cmd::handler::err_val();
            return;
        }
    }

    inbuilt_storage::write_string_to_storage(cmd_val,
                                             BILLY_STR_MAX_LEN,
                                             params->addr);
    *(params->refresh_flag) = 1;

    char msg[BILLY_STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, params->topic);

    if (params->echo_val) {
        strcat(msg, cmd_val);
    }

    cmd::aux::output_msg(msg);
}

void cmd::aux::output_config(uint32_t addr, const char *topic)
{
    char msg[BILLY_STR_MAX_LEN * 2 + 1] = {0};
    strcpy(msg, topic);

    char config_val[BILLY_STR_MAX_LEN + 1] = {0};
    inbuilt_storage::read_string_from_storage(config_val,
                                              sizeof(config_val),
                                              BILLY_STR_MAX_LEN,
                                              addr);
    strcat(msg, config_val);

    cmd::aux::output_msg(msg);
}


/*--- Handler functions ---*/

void cmd::handler::err_len()
{
    cmd::aux::output_msg("Command buffer overflow.");
}

void cmd::handler::err_prefix()
{
    cmd::aux::output_msg("Invalid or absent command prefix.");
}

void cmd::handler::err_cmd()
{
    cmd::aux::output_msg("No valid command entered.");
}

void cmd::handler::err_val()
{
    cmd::aux::output_msg("No valid value submitted.");
}

// Command #1
void cmd::handler::set_load_digital(char *cmd, uint32_t pin, bool active_state)
{
    static char *cmd_val = strstr(cmd, "=") + 1;  /* Needs to be calculated just once
                                                   * because subsequent commands won't be different.
                                                   */

    cstring_utils::to_uppercase(cmd_val);         // Valid commmand values for this function are all-uppercase.

    if (cstring_utils::are_equal(cmd_val, "TOGGLE")) {
        if (digitalRead(pin) == active_state) {
            cmd::aux::set_output_digital(pin, !active_state, "Two-state load is now OFF");
            return;
        } else {
            cmd::aux::set_output_digital(pin, active_state, "Two-state load is now ON");
            return;
        }
    }

    if (cstring_utils::are_equal(cmd_val, "ON")) {
        if (digitalRead(pin) != active_state) {
            cmd::aux::set_output_digital(pin, active_state, "Two-state load is now ON");
            return;
        } else {
            cmd::aux::output_msg("Two-state load is already ON");
            return;
        }
    }

    if (cstring_utils::are_equal(cmd_val, "OFF")) {
        if (digitalRead(pin) == active_state) {
            cmd::aux::set_output_digital(pin, !active_state, "Two-state load is now OFF");
            return;
        } else {
            cmd::aux::output_msg("Two-state load is already OFF");
            return;
        }
    }

    cmd::handler::err_val();
}

// Command #2
void cmd::handler::set_load_PWM(const char *cmd, uint32_t pin)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!cmd::aux::has_decimal_only(cmd_val)) {
        cmd::handler::err_val();
        return;
    }

    uint32_t val_len = (uint32_t)strlen(cmd_val);
    if (val_len < 1 || val_len > 3) {              // Valid duty cycle values are 0 to 255.
        cmd::handler::err_val();
        return;
    }

    uint32_t duty_cycle = strtol(cmd_val, 0, 10);  // Convert to decimal.
    if (duty_cycle > 255) {                        // Valid duty cycle values are 0 to 255.
        cmd::handler::err_val();
        return;
    }

    char msg[BILLY_STR_MAX_LEN * 2 + 1] = "PWM duty cycle is set to ";
    strcat(msg, cmd_val);

    cmd::aux::set_output_PWM(pin, duty_cycle, msg);
}

// Command #3
void cmd::handler::output_load_digital(uint32_t pin, bool active_state)
{
    if (pin == 0) {
        cmd::aux::output_msg("Digital output pin not specified.");
        return;
    }

    if (digitalRead(pin) == active_state) {
        cmd::aux::output_msg("Current load state is ON");
    } else {
        cmd::aux::output_msg("Current load state is OFF");
    }
}

// Command #4
void cmd::handler::set_WiFi_SSID(const char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_WIFI_SSID,
                                  ANY_CHAR,
                                  "SSID changed successfully! New SSID is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd::aux::set_config(&params);
}

// Command #5
void cmd::handler::output_WiFi_SSID()
{
    cmd::aux::output_config(INBUILT_STORAGE_ADDR_WIFI_SSID ,
                            "Current SSID is: ");
}

// Command #6
void cmd::handler::set_WiFi_pswd(const char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_WIFI_PSWD,
                                  ANY_CHAR,
                                  "Password changed successfully!",
                                  ECHO_VAL_OFF,
                                  refresh_flag};
    cmd::aux::set_config(&params);
}

// Command #7
void cmd::handler::set_WiFi_RSSI_output_flag(char *cmd, bool *refresh_flag)
{
    static char *cmd_val = strstr(cmd, "=") + 1;  /* Needs to be calculated just once
                                                   * because subsequent commands won't be different.
                                                   */

    cstring_utils::to_uppercase(cmd_val);         // Valid commmand values for this function are all-uppercase.

    if (cstring_utils::are_equal(cmd_val, "ON") || cstring_utils::are_equal(cmd_val, "OFF")) {
        set_config_params_t params = {cmd,
                                      INBUILT_STORAGE_ADDR_WIFI_RSSI_OUTPUT_FLAG,
                                      ANY_CHAR,
                                      "RSSI output: ",
                                      ECHO_VAL_ON,
                                      refresh_flag};
        cmd::aux::set_config(&params);
    } else {
        cmd::handler::err_val();
    }
}

// Command #8
void cmd::handler::set_WiFi_autoreconnect_flag(char *cmd, bool *refresh_flag)
{
    static char *cmd_val = strstr(cmd, "=") + 1;  /* Needs to be calculated just once
                                                   * because subsequent commands won't be different.
                                                   */

    cstring_utils::to_uppercase(cmd_val);         // Valid commmand values for this function are all-uppercase.

    if (cstring_utils::are_equal(cmd_val, "ON") || cstring_utils::are_equal(cmd_val, "OFF")) {
        set_config_params_t params = {cmd,
                                      INBUILT_STORAGE_ADDR_WIFI_AUTORECONNECT_FLAG,
                                      ANY_CHAR,
                                      "Automatic Wi-Fi reconnect attempts: ",
                                      ECHO_VAL_ON,
                                      refresh_flag};
        cmd::aux::set_config(&params);
    } else {
        cmd::handler::err_val();
    }
}

// Command #9
void cmd::handler::output_local_server_IP()
{
    char msg[BILLY_STR_MAX_LEN * 2 + 1] = {0};
    String current_IP = esp_wifi_billy::get_devices_current_ip();
    strcpy(msg, "Current local IP address is: ");
    strcat(msg, current_IP.c_str());
    cmd::aux::output_msg(msg);
}

// Command #10
void cmd::handler::set_local_server_port(const char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                                  DECIMAL_ONLY,
                                  "Local server port changed successfully! New port is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd::aux::set_config(&params);

    stored_configs_t stored_configs;
    stored_configs_read(&stored_configs);

    esp_tcp_billy::server_stop(CONN_SHUTDOWN_DOWNTIME);
    esp_tcp_billy::server_port_update(stored_configs.local_server_port);
    esp_tcp_billy::server_start();
}

// Command #11
void cmd::handler::output_local_server_port()
{
    cmd::aux::output_config(INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                          "Current local server port is: ");
}

// Command #12
void cmd::handler::set_IoT_flag(char *cmd, bool *refresh_flag)
{
    static char *cmd_val = strstr(cmd, "=") + 1;  /* Needs to be calculated just once
                                                   * because subsequent commands won't be different.
                                                   */

    cstring_utils::to_uppercase(cmd_val);         // Valid commmand values for this function are all-uppercase.

    if (cstring_utils::are_equal(cmd_val, "ON") || cstring_utils::are_equal(cmd_val, "OFF")) {
        set_config_params_t params = {cmd,
                                      INBUILT_STORAGE_ADDR_IOT_FLAG,
                                      ANY_CHAR,
                                      "Requests to an IoT server: ",
                                      ECHO_VAL_ON,
                                      refresh_flag};
        cmd::aux::set_config(&params);
    } else {
        cmd::handler::err_val();
    }
}

// Command #13
void cmd::handler::set_IoT_server_IP(const char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
                                  DECIMAL_ONLY,
                                  "IoT server target IP changed successfully! New IP is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd::aux::set_config(&params);
}

// Command #14
void cmd::handler::output_IoT_server_IP()
{
    cmd::aux::output_config(INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
                          "Current IoT server target IP is: ");
}

// Command #15
void cmd::handler::set_IoT_server_port(const char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
                                  DECIMAL_ONLY,
                                  "IoT server target port changed successfully! New port is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd::aux::set_config(&params);
}

// Command #16
void cmd::handler::output_IoT_server_port()
{
    cmd::aux::output_config(INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
                          "Current IoT server target port is: ");
}

// Command #17
void cmd::handler::set_IoT_req_msg(const char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
                                  ANY_CHAR,
                                  "IoT server request text changed successfully! New text is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd::aux::set_config(&params);
}

// Command #18
void cmd::handler::output_IoT_req_msg()
{
    cmd::aux::output_config(INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
                          "Current IoT server request text is: ");
}

// Command #19
void cmd::handler::set_IoT_req_period(const char *cmd, bool *refresh_flag)
{
    set_config_params_t params = {cmd,
                                  INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
                                  DECIMAL_ONLY,
                                  "IoT server request period changed successfully! New period (in ms) is: ",
                                  ECHO_VAL_ON,
                                  refresh_flag};
    cmd::aux::set_config(&params);
}

// Command #20
void cmd::handler::set_BTClassic_flag(char *cmd,
                                      void (*setup_BTClassic_ptr)(stored_configs_t *),
                                      bool *refresh_flag)
{
    // Dummy statements to prevent warnings connected to a conditional compilation (unused parameter).
    (void)cmd;
    (void)setup_BTClassic_ptr;
    (void)refresh_flag;

    #if defined ESP32 && defined BTCLASSIC_USED
        static char *cmd_val = strstr(cmd, "=") + 1;  /* Needs to be calculated just once
                                                       * because subsequent commands won't be different.
                                                       */

        cstring_utils::to_uppercase(cmd_val);         // Valid commmand values for this function are all-uppercase.

        if (cstring_utils::are_equal(cmd_val, "ON") || cstring_utils::are_equal(cmd_val, "OFF")) {
            set_config_params_t params = {cmd,
                                          INBUILT_STORAGE_ADDR_BTCLASSIC_FLAG,
                                          ANY_CHAR,
                                          "Bluetooth Classic status updated successfully!",
                                          ECHO_VAL_OFF,
                                          refresh_flag};
            cmd::aux::set_config(&params);

            stored_configs_t stored_configs;
            stored_configs_read(&stored_configs);
            setup_BTClassic_ptr(&stored_configs);

            Serial.println("");
        } else {
            cmd::handler::err_val();
        }
    #else
        cmd::handler::err_cmd();
    #endif
}

// Command #21
void cmd::handler::set_BTClassic_dev_name(const char *cmd,
                                          void (*setup_BTClassic_ptr)(stored_configs_t *),
                                          bool *refresh_flag)
{
    // Dummy statements to prevent warnings connected to a conditional compilation (unused parameter).
    (void)cmd;
    (void)setup_BTClassic_ptr;
    (void)refresh_flag;

    #if defined ESP32 && defined BTCLASSIC_USED
        set_config_params_t params = {cmd,
                                      INBUILT_STORAGE_ADDR_BTCLASSIC_DEV_NAME,
                                      ANY_CHAR,
                                      "Bluetooth Classic device name changed successfully!",
                                      ECHO_VAL_OFF,
                                      refresh_flag};
        cmd::aux::set_config(&params);

        stored_configs_t stored_configs;
        stored_configs_read(&stored_configs);
        setup_BTClassic_ptr(&stored_configs);

        Serial.println("");
    #else
        cmd::handler::err_cmd();
    #endif
}

// Command #22
void cmd::handler::output_BTClassic_dev_name()
{
    #if defined ESP32 && defined BTCLASSIC_USED
        cmd::aux::output_config(INBUILT_STORAGE_ADDR_BTCLASSIC_DEV_NAME,
                              "Current Bluetooth Classic device name is: ");
    #else
        cmd::handler::err_cmd();
    #endif
}

// Command #23
void cmd::handler::all_conn_rst(bool (*setup_WiFi_ptr)(stored_configs_t *, uint32_t),
                                void (*setup_BTClassic_ptr)(stored_configs_t *),
                                stored_configs_t *stored_configs)
{
    cmd::aux::output_msg("Resetting local connections...");

    esp_tcp_billy::clients_disconnect(CONN_SHUTDOWN_DOWNTIME);
    esp_tcp_billy::server_stop(CONN_SHUTDOWN_DOWNTIME);

    #if defined ESP32 && defined BTCLASSIC_USED
        esp32_btclassic_billy::stop(CONN_SHUTDOWN_DOWNTIME);
    #endif

    setup_WiFi_ptr(stored_configs, CONN_TIMEOUT);
    setup_BTClassic_ptr(stored_configs);

    Serial.println("");
}

// Command #24
void cmd::handler::output_version()
{
    char msg[BILLY_STR_MAX_LEN * 4] = "\n" \
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

    cmd::aux::output_msg(msg);
}

// Command #25
void cmd::handler::RGB_output_color(char *cmd)
{
    static char *cmd_val = strstr(cmd, "=") + 1;  /* Needs to be calculated just once
                                                   * because subsequent commands won't be different.
                                                   */

    cstring_utils::to_uppercase(cmd_val);         // Valid commmand values for this function are all-uppercase.

    uint32_t ret_val = RGB_LED_output_color(cmd_val);

    if (ret_val) {
        char msg[BILLY_STR_MAX_LEN * 2 + 1] = {0};
        strcpy(msg, "Outputting RGB color ");
        strcat(msg, cmd_val);
        strcat(msg, ".");
        cmd::aux::output_msg(msg);
    } else {
        cmd::handler::err_val();
    }
}

// Command #26
void cmd::handler::RGB_output_on()
{
    RGB_LED_output_on();
    cmd::aux::output_msg("RGB output ON.");
}

// Command #27
void cmd::handler::RGB_output_off()
{
    RGB_LED_output_off();
    cmd::aux::output_msg("RGB output OFF.");
}
