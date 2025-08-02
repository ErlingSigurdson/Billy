/****************** DESCRIPTION *****************/

/**
 * Filename: RGB_LED.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose:  RGB LED control.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes: I actually doubted whether I want a separate pair of files for this functionality, but eventually I decided
 *        to have them in case I change the library in use or decide to create some complex wrappers.
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef RGB_LED_H
#define RGB_LED_H


/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>  /* I guess I could do with <cstdint> and <cstring>, but I decided to keep it
                       * in case I need to use millis() or something.
                       */


/************** FUNCTION PROTOTYPES *************/

void RGB_LED_init(uint32_t red_pin, uint32_t green_pin, uint32_t blue_pin, bool is_common_anode);
void RGB_LED_color_output(const char *cmd_buf);


#endif  // Include guards.
