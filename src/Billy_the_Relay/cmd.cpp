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


/*--- Auxiliary functions ---*/

void cmd_aux_send(const char *msg)
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


void cmd_aux_write_digital(uint8_t pin, uint8_t state, const char *msg)
{
    digitalWrite(pin, state);
    cmd_aux_send(msg);
}

void cmd_aux_write_analog(uint8_t pin, uint32_t val, const char *msg)
{
    analogWrite(pin, val);
    cmd_aux_send(msg);
}

void cmd_handler_set(char *cmd, const char *topic, uint32_t addr, bool echo)
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

