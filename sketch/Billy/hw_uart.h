/*************** FILE DESCRIPTION ***************/

/**
 * Filename: hw_uart.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  Hardware UART wrapper functions.
 *           Written for use with the Arduino framework.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef HW_UART_H
#define HW_UART_H


/*--- Includes ---*/

// Arduino core header file.
#include <Arduino.h>


/************** FUNCTION PROTOTYPES *************/

namespace hw_uart {
    void setup(uint32_t baud_rate, uint32_t antinoise_pause, uint32_t startup_pause, const char *startup_msg);
    void print(const char *str);
    void flush();
    uint32_t read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout, uint32_t read_slowdown);
}


#endif  // Include guards.
