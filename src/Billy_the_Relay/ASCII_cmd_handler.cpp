/****************** DESCRIPTION *****************/

/**
 * Filename: ASCII_cmd_handler.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: обработка текстовых команд.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// General Arduino library.
#include <Arduino.h>

// Local modules.
#include "ASCII_cmd_handler.h"


/******************* FUNCTIONS ******************/

bool ASCII_cmd_handler_check_prefix(char *buf, const char *prefix)
{
    if (strstr(buf, prefix) == buf) {
        return 1;
    } else {
        return 0;
    }
}

int32_t ASCII_cmd_handler_check_cmd(char *buf, const char *cmd_list[], uint32_t cmd_list_len)
{
    for (uint32_t i = 0; i < cmd_list_len; ++i) {
        if (strstr(buf, cmd_list[i]) != NULL) {
            return i;
        }
    }

    return -1;
}
