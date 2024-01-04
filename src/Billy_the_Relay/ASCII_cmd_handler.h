/****************** DESCRIPTION *****************/

/**
 * Filename: ASCII_cmd_handler.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: обработка текстовых команд.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ASCII_CMD_HANDLER_H
#define ASCII_CMD_HANDLER_H


/************** FUNCTION PROTOTYPES *************/

bool ASCII_cmd_handler_check_prefix(char *buf, const char *prefix);
int32_t ASCII_cmd_handler_check_cmd(char *buf, const char *cmd_list[], uint32_t cmd_list_len);


#endif  // Include guards.
