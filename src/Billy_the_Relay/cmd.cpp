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

// Local modules.
#include "cmd.h"
#include "inbuilt_storage.h"
#include "ESP_TCP.h"

#if defined ESP32 && defined BT_CLASSIC_PROVIDED
    #include "ESP32_Bluetooth.h"
#endif


/******************* FUNCTIONS ******************/

/*--- Contents checking functions ---*/

bool cmd_check_prefix(char *buf, const char *prefix)
{
    if (strstr(buf, prefix) == buf) {
        return 1;
    } else {
        return 0;
    }
}

int32_t cmd_check_body(char *buf, const char *cmd_list[], uint32_t cmd_list_len)
{
    for (uint32_t i = 0; i < cmd_list_len; ++i) {
        if (strstr(buf, cmd_list[i]) == buf) {
            return i;
        }
    }

    return -1;
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
void cmd_handler_set_digital_load(char *cmd)
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
            cmd_aux_send("Digital load is already ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "OFF")) {
        if (digitalRead(DIGITAL_LOAD_PIN) != DIGITAL_LOAD_OFF) {
            cmd_aux_set_digital_load(DIGITAL_LOAD_PIN, 0, "Digital load is now OFF");
            return;
        } else {
            cmd_aux_send("Digital load is already OFF");
            return;
        }
    }

    cmd_handler_err_val();
}

// Command #1
void cmd_handler_set_analog_load(char *cmd)
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
    
    char msg = "Analog load duty cycle set to ";
    strcpy(msg, cmd_val);
     
    cmd_aux_set_analog_load(ANALOG_LOAD_PIN, val, msg);
}

// Command #2
void cmd_handler_output_load_digital()
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
void cmd_handler_update_local_SSID(char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "SSID changed successfully! New SSID is: ",
                          INBUILT_STORAGE_ADDR_SSID,
                          ECHO_ON
                          refresh_flag);
}

// Command #4
void cmd_handler_output_local_SSID()
{
    cmd_aux_output_config("Current SSID is: ",
                          INBUILT_STORAGE_ADDR_SSID);
}

// Command #5
void cmd_handler_update_local_pswd(char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "Password changed successfully!",
                          INBUILT_STORAGE_ADDR_PSWD,
                          ECHO_OFF,
                          refresh_flag);
}

// Command #6
void handle_cmd_update_local_port(char *cmd, bool *refresh_flag)
{
    cmd_aux_update_config(cmd,
                          "Local server port changed successfully! New port is: ",
                          INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
                          ECHO_ON,
                          refresh_flag);

    cmd_aux_print_and_send_msg("Please reboot your device or reset local connection to put changes into effect.");
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
            ESP32_BTClassic_send_msg(msg);
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

void cmd_handler_update_config(char *cmd, const char *topic, uint32_t addr, bool echo, bool *refresh_flag)
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