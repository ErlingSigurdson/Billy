/****************** DESCRIPTION *****************/

/**
 * Filename: HW_UART.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: hardware UART-related functions.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef HW_UART_H
#define HW_UART_H


/************** FUNCTION PROTOTYPES *************/

uint32_t HW_UART_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout, uint32_t read_slowdown);


#endif  // Include guards.
