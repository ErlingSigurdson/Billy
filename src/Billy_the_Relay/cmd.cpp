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
#include "ESP_WiFi.h"
#include "ESP_TCP.h"
#include "ESP_HTTP.h"


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

void cmd_handler_err_prefix()
{
    handle_cmd_helper_send("Invalid prefix for a command.");
}

void cmd_handler_err_cmd()
{
    handle_cmd_helper_send("No valid command entered.");
}

void cmd_handler_err_val()
{
    handle_cmd_helper_send("No valid value submitted.");
}

void cmd_handler_err_len()
{
    handle_cmd_helper_send("Command buffer overflow.");
}

// Command #0
void cmd_handler_set_load_digital(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    if (!strcmp(cmd_val, "TOGGLE")) {
        if (digitalRead(DIGITAL_LOAD_PIN) == DIGITAL_LOAD_ON) {
            cmd_aux_set_load_digital(DIGITAL_LOAD_PIN, 0, "Digital load is now OFF");
            return;
        } else {
            cmd_aux_set_load_digital(DIGITAL_LOAD_PIN, 1, "Digital load is now ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "ON")) {
        if (digitalRead(DIGITAL_LOAD_PIN) != DIGITAL_LOAD_ON) {
            cmd_aux_set_load_digital(DIGITAL_LOAD_PIN, 1, "Digital load is now ON");
            return;
        } else {
            cmd_aux_send("Digital load is already ON");
            return;
        }
    }

    if (!strcmp(cmd_val, "OFF")) {
        if (digitalRead(DIGITAL_LOAD_PIN) != DIGITAL_LOAD_OFF) {
            cmd_aux_set_load_digital(DIGITAL_LOAD_PIN, 0, "Digital load is now OFF");
            return;
        } else {
            cmd_aux_send("Digital load is already OFF");
            return;
        }
    }

    cmd_handler_err_val();
}

// Command #1
void cmd_handler_set_load_analog(char *cmd)
{
    char *cmd_val = strstr(cmd, "=") + 1;

    uint32_t val_len = (uint32_t)strlen(cmd_val);
    if (val_len < 1 || val_len > 3) {
        cmd_handler_err_val();
        return;
    }

    uint32_t val = strtol(cmd_val, 0, 10);  // Convert to decimal.
     
    cmd_aux_set_load_analog(ANALOG_LOAD_PIN, val, msg);
}

// Command #2
void cmd_handler_get_load_digital()
{
    if (digitalRead(DIGITAL_LOAD_PIN) == DIGITAL_LOAD_ON) {
        cmd_aux_print_and_send_msg("Current digital load stat is ON");
    } else {
        cmd_aux_print_and_send_msg("Current digital load stat is OFF");
    }
}

/* To print or not to print a new value of an updated config.
 * OFF for password, ON for other cases.
 */
#define ECHO_ON 1
#define ECHO_OFF 0

// Command #3
void cmd_handler_set_local_SSID(char *cmd, bool *refresh_flag)
{
    cmd_aux_set_config(cmd,
                       "SSID changed successfully! New SSID is: ",
                       INBUILT_STORAGE_ADDR_SSID,
                       ECHO_ON
                       refresh_flag);
}

// Command #4
void cmd_handler_get_local_SSID()
{
    cmd_aux_gset_config("Current SSID is: ",
                        INBUILT_STORAGE_ADDR_SSID);
}

// Command #5
void cmd_handler_set_local_pswd(char *cmd, bool *refresh_flag)
{
    cmd_aux_set_config(cmd,
                       "Password changed successfully!",
                       INBUILT_STORAGE_ADDR_PSWD,
                       ECHO_OFF,
                       refresh_flag);
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
                             INBUILT_STORAGE_ADDR_BT_FLAG);

        if (!strcmp(val, "ON")) {
            ESP32_BTClassic_send_msg(msg);
        }
    #endif
}

void cmd_aux_get_val(const char *topic, uint32_t addr)
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



void cmd_handler_set_val(char *cmd, const char *topic, uint32_t addr, bool echo, bool *refresh_flag)
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

void cmd_aux_set_load_digital(uint8_t pin, uint8_t state, const char *msg)
{
    digitalWrite(pin, state);
    cmd_aux_print_and_send_msg(msg);
}

void cmd_aux_set_load_analog(uint8_t pin, uint32_t val, const char *msg)
{
    analogWrite(pin, val);
    cmd_aux_print_and_send_msg(msg);    
}